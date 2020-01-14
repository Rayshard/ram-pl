#include "pch.h"
#include "Interpreter.h"
#include "Parser.h"
#include "Statement.h"
#include "Environment.h"
#include "Value.h"
#include "RamDLL.h"

std::map<std::string, std::vector<std::string>> Interpreter::SRC_LINES = std::map<std::string, std::vector<std::string>>();
std::chrono::time_point<std::chrono::steady_clock> Interpreter::INIT_TIME = std::chrono::high_resolution_clock::now();
Environment* Interpreter::ENV_STD = nullptr;
std::string Interpreter::MainFilePath = "";

void Interpreter::Init(const char* _mainFilePath)
{
	srand(time(NULL));

	Interpreter::MainFilePath = _mainFilePath;
	Interpreter::INIT_TIME = std::chrono::high_resolution_clock::now();
	Interpreter::SetStandardEnvironment();
}

void Interpreter::SetStandardEnvironment()
{
	Environment* env = new Environment(0, "STD", "internal");

	//Type Defs
	env->typeDefs.insert_or_assign("STD.int", IValue::SIGNATURE_INT);
	env->typeDefs.insert_or_assign("STD.float", IValue::SIGNATURE_FLOAT);
	env->typeDefs.insert_or_assign("STD.string", IValue::SIGNATURE_STRING);
	env->typeDefs.insert_or_assign("STD.bool", IValue::SIGNATURE_BOOL);
	env->typeDefs.insert_or_assign("STD.void", IValue::SIGNATURE_VOID);

#pragma region Functions
#pragma region Print
	builtInFunc print_body = [](Environment* _env, Position _execPos)
	{
		std::cout << _env->GetValue("arg", _execPos, false)->ToString();
		return SHARE_VOID(_execPos);
	};
	std::vector<std::string> print_argNames({ "arg" }), print_argSigs({ IValue::SIGNATURE_STRING });
	SharedValue func_print = SHARE(new FuncValue(env, "print", print_body, print_argNames, print_argSigs, IValue::SIGNATURE_VOID, Position()));

	env->AddFuncDeclaration("print", func_print, Position());
#pragma endregion

#pragma region GetInput
	builtInFunc getInput_body = [](Environment* _env, Position _execPos)
	{
		std::string input;
		std::getline(std::cin, input);
		return SHARE(new StringValue(input, _execPos));
	};
	std::vector<std::string> getInput_argNames({}), getInput_argSigs({ IValue::SIGNATURE_STRING });
	SharedValue func_getInput = SHARE(new FuncValue(env, "getInput", getInput_body, getInput_argNames, getInput_argSigs, IValue::SIGNATURE_STRING, Position()));

	env->AddFuncDeclaration("getInput", func_getInput, Position());
#pragma endregion

#pragma region Include
	builtInFunc include_body = [](Environment* _env, Position _execPos)
	{
		Trace trace = Trace(_execPos, "STD", "std");
		std::string filePath = _env->GetValue("filePath", _execPos, false)->AsString()->value;

		if(!FileExists(filePath.c_str()))
		{
			if(FileExists((GetFileDir(Interpreter::MainFilePath) + SYS_SEP + filePath).c_str()))
			{
				filePath = (GetFileDir(Interpreter::MainFilePath) + filePath).c_str();
			}
			else { return SHARE(Exception_File("File at " + std::string(filePath) + " does not exist!", trace)); }
		}

		std::string namedSpaceName = _env->GetValue("name", _execPos, false)->ToString();

		Environment* prevGlobal = Environment::GLOBAL; // Keep copy of calling file's environment to return to when finished
		SharedValue retVal = Interpreter::RunFile(filePath.c_str(), namedSpaceName); // The sets the global environment to the file begin run

		if(retVal->GetType() != VEXCEPTION)
		{
			bool openNamedspace = _env->GetValue("open", _execPos, false)->AsBool()->value;
			SharedValue nsAddResult = prevGlobal->AddNamedspace(Environment::GLOBAL, _execPos, openNamedspace); // Adds the run file environment (global environment) to the call file's list of namedspaces

			if(nsAddResult->GetType() == VEXCEPTION)
			{
				retVal.reset();
				retVal = nsAddResult;
				delete Environment::GLOBAL;
			}
		}

		Environment::GLOBAL = prevGlobal; // Reset global environment to calling file
		return retVal;
	};
	std::vector<std::string> include_argNames({ "filePath", "name", "open" }), include_argSigs({ IValue::SIGNATURE_STRING, IValue::SIGNATURE_STRING, IValue::SIGNATURE_BOOL });
	SharedValue func_include = SHARE(new FuncValue(env, "include", include_body, include_argNames, include_argSigs, IValue::SIGNATURE_VOID, Position()));

	env->AddFuncDeclaration("include", func_include, Position());
#pragma endregion

#pragma region Unscope
	builtInFunc unscope_body = [](Environment* _env, Position _execPos)
	{
		SharedValue ns = _env->GetValue("namedspace", _execPos, false);
		return _env->parent->OpenNamedspace(ns->AsNamedspace(), _execPos);
	};
	std::vector<std::string> unscope_argNames({ "namedspace" }), unscope_argSigs({ IValue::SIGNATURE_NAMEDSPACE });
	SharedValue func_unscope = SHARE(new FuncValue(env, "unscope", unscope_body, unscope_argNames, unscope_argSigs, IValue::SIGNATURE_VOID, Position()));

	env->AddFuncDeclaration("unscope", func_unscope, Position());
#pragma endregion

#pragma region PrintEnv
	builtInFunc printEnv_body = [](Environment* _env, Position _execPos)
	{
		std::cout << _env->parent->ToString() << std::endl;
		return SHARE_VOID(_execPos);
	};
	std::vector<std::string> printEnv_argNames({}), printEnv_argSigs({});
	SharedValue func_printEnv = SHARE(new FuncValue(env, "printEnv", printEnv_body, printEnv_argNames, printEnv_argSigs, IValue::SIGNATURE_VOID, Position()));

	env->AddFuncDeclaration("printEnv", func_printEnv, Position());
#pragma endregion

#pragma region DLLOpen
	{
		builtInFunc body = [](Environment* _env, Position _execPos)
		{
			Trace trace = Trace(_execPos, "STD", "std");
			std::string dllPath = _env->GetValue("filePath", _execPos, false)->AsString()->value;
			int dllID = RamDLL::Load(dllPath.c_str());

			if(dllID == DLL_NOT_LOADED) { return SHARE(Exception_DLLLoadFail(dllPath, trace)); }
			else { return SHARE(new IntValue(dllID, _execPos)); }
		};
		std::vector<std::string> argNames({ "filePath" }), argSigs({ IValue::SIGNATURE_STRING });
		SharedValue funcVal = SHARE(new FuncValue(env, "dllOpen", body, argNames, argSigs, IValue::SIGNATURE_INT, Position()));

		env->AddFuncDeclaration(funcVal->AsFunc()->name, funcVal, Position());
	}
#pragma endregion

#pragma region DLLClose
	{
		builtInFunc body = [](Environment* _env, Position _execPos)
		{
			Trace trace = Trace(_execPos, "STD", "std");

			int id = _env->GetValue("id", _execPos, false)->AsInt()->value;

			if(!RamDLL::Free(id)) { return SHARE(new ExceptionValue("ID Not Found", "There was no loaded DLL with the id '" + std::to_string(id) + "'", trace)); }
			else { return SHARE_VOID(_execPos); }
		};
		std::vector<std::string> argNames({ "id" }), argSigs({ IValue::SIGNATURE_INT });
		SharedValue funcVal = SHARE(new FuncValue(env, "dllClose", body, argNames, argSigs, IValue::SIGNATURE_VOID, Position()));

		env->AddFuncDeclaration(funcVal->AsFunc()->name, funcVal, Position());
	}
#pragma endregion

#pragma region DLLLoadFunc
	{
		builtInFunc body = [](Environment* _env, Position _execPos)
		{
			Trace trace = Trace(_execPos, "STD", "std");

			int dllID = _env->GetValue("dllID", _execPos, false)->AsInt()->value;
			std::string funcName = _env->GetValue("funcName", _execPos, false)->AsString()->value;

			int funcID = RamDLL::LoadFunc(dllID, funcName.c_str());

			if(funcID == DLL_NOT_LOADED) { return SHARE(new ExceptionValue("ID Not Found", "There was no loaded DLL with the id '" + std::to_string(dllID) + "'", trace)); }
			else if(funcID == DLL_FUNC_NOT_FOUND) { return SHARE(Exception_SymbolNotFound(funcName, trace)); }
			else { return SHARE(new IntValue(funcID, _execPos)); }
		};
		std::vector<std::string> argNames({ "dllID", "funcName" }), argSigs({ IValue::SIGNATURE_INT, IValue::SIGNATURE_STRING });
		SharedValue funcVal = SHARE(new FuncValue(env, "dllLoadFunc", body, argNames, argSigs, IValue::SIGNATURE_INT, Position()));

		env->AddFuncDeclaration(funcVal->AsFunc()->name, funcVal, Position());
	}
#pragma endregion

#pragma region DLLGetFunc
	{
		builtInFunc body = [](Environment* _env, Position _execPos)
		{
			Trace trace = Trace(_execPos, "STD", "std");

			int dllFuncID = _env->GetValue("dllFuncID", _execPos, false)->AsInt()->value;
			ArrayValue* argTypesVal = _env->GetValue("argTypes", _execPos, false)->AsArray();

			std::vector<Signature> argSigs;

			for(auto it : argTypesVal->elements)
			{
				SharedValue argSigVal = _env->GetSignature(it->AsString()->value, _execPos);

				if(argSigVal->GetType() == VEXCEPTION) { return argSigVal; }
				else { argSigs.push_back(argSigVal->AsString()->value); }
			}

			std::string retTypeVal = _env->GetValue("retType", _execPos, false)->AsString()->value;
			SharedValue retSigVal = _env->GetSignature(retTypeVal, _execPos);
			Signature retSig;

			if(retSigVal->GetType() == VEXCEPTION) { return retSigVal; }
			else { retSig = retSigVal->AsString()->value; }

			FuncValue* funcVal = RamDLL::GetFuncValue(dllFuncID, _env->parent, argSigs, retSig, _execPos);

			if(!funcVal) { return SHARE(new ExceptionValue("ID Not Found", "There was no loaded DLL with a function with the id '" + std::to_string(dllFuncID) + "'", trace)); }
			else { return SHARE(new UnknownValue(funcVal, _execPos)); }
		};
		std::vector<std::string> argNames({ "dllFuncID", "argTypes", "retType" });
		std::vector<std::string> argSigs({ IValue::SIGNATURE_INT, IValue::SIGNATURE_STRING + "|" + IValue::SIGNATURE_ARRAY, IValue::SIGNATURE_STRING });
		SharedValue funcVal = SHARE(new FuncValue(env, "dllGetFunc", body, argNames, argSigs, IValue::SIGNATURE_UNKNOWN, Position()));

		env->AddFuncDeclaration(funcVal->AsFunc()->name, funcVal, Position());
	}
#pragma endregion
#pragma endregion

	Interpreter::ENV_STD = env;
}

std::string Interpreter::GetSrcLine(std::string _filePath, int _line) { return Interpreter::SRC_LINES[_filePath][_line - 1]; }

SharedValue Interpreter::RunFile(const char* _path, std::string _name)
{
	std::vector<std::string> fileLines;
	TokensResult tokenizationResult = TokenizeFile(_path, fileLines);

	Interpreter::SRC_LINES.insert_or_assign(_path, fileLines);
	Environment::GLOBAL = Environment::CreateGlobal(_name, _path);

	std::string path = _path;

	if(tokenizationResult.success)
	{
		std::cout << "Tokenized!" << std::endl;

		FileParseResult parsingResult = Parse(tokenizationResult.value.data());
		tokenizationResult.value.clear();

		if(parsingResult.success)
		{
			std::cout << "Parsed!" << std::endl << std::endl;

			std::vector<IStatement*> statements = parsingResult.value;
			std::cout << "Running " + path << std::endl;

			SharedValue endVal(nullptr);

			if(statements.size() > 0)
			{
				CodeBlock program = CodeBlock(statements, statements[0]->_position, _name);
				program.createSubEnv = false;
				endVal = program.Execute(Environment::GLOBAL);
			}
			else { endVal = SHARE_VOID(Position()); }

			std::cout << "Exited Running " + path << std::endl << std::endl;
			return endVal;
		}
		else { return SHARE(Exception_CompilationError("There was a parsing error in " + path + "\n\n" + parsingResult.message, Trace(parsingResult.position, _name, _path))); }
	}
	else { return SHARE(Exception_CompilationError("There was a tokenizing error in " + path + "\n\n" + tokenizationResult.message, Trace(tokenizationResult.position, _name, _path))); }
}