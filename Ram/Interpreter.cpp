#include "pch.h"
#include "Interpreter.h"
#include "Parser.h"
#include "Statement.h"
#include "Environment.h"
#include "Value.h"

std::map<std::string, std::vector<std::string>> Interpreter::SRC_LINES = std::map<std::string, std::vector<std::string>>();
std::chrono::time_point<std::chrono::steady_clock> Interpreter::INIT_TIME = std::chrono::high_resolution_clock::now();
Environment* Interpreter::ENV_STD = nullptr;
std::string Interpreter::MainFilePath = "";

void Interpreter::Init(const char* _mainFilePath)
{
	Interpreter::MainFilePath = _mainFilePath;
	Interpreter::INIT_TIME = std::chrono::high_resolution_clock::now();
	Interpreter::SetStandardEnvironment();
}

void Interpreter::SetStandardEnvironment()
{
	Environment* env = new Environment(0, "STD", "internal");

	//Type Defs
	env->typeDefs.insert_or_assign("STD.int", "<INT>");
	env->typeDefs.insert_or_assign("STD.float", "<FLOAT>");
	env->typeDefs.insert_or_assign("STD.string", "<STRING>");
	env->typeDefs.insert_or_assign("STD.bool", "<BOOL>");
	env->typeDefs.insert_or_assign("STD.void", "<VOID>");

#pragma region Functions
	Trace trace = Trace(Position(), "STD", "std");

	//Print
	FuncValue::built_in print_body = [](Environment* _env, Position _execPos)
	{
		std::cout << _env->GetValue("arg", _execPos, false)->ToString();
		return SHARE(new VoidValue(_execPos));
	};
	std::vector<std::string> print_argNames({ "arg" }), print_argSigs({ "<STRING>" });
	SharedValue func_print = SHARE(new FuncValue(env, print_body, print_argNames, print_argSigs, "<VOID>", Position()));

	env->AddFuncDeclaration("print", func_print, Position());

	//GetInput
	FuncValue::built_in getInput_body = [](Environment* _env, Position _execPos)
	{
		std::string input;
		std::getline(std::cin, input);
		return SHARE(new StringValue(input, _execPos));
	};
	std::vector<std::string> getInput_argNames({}), getInput_argSigs({ "<STRING>" });
	SharedValue func_getInput = SHARE(new FuncValue(env, getInput_body, getInput_argNames, getInput_argSigs, "<STRING>", Position()));

	env->AddFuncDeclaration("getInput", func_getInput, Position());

	//Time
	FuncValue::built_in time_body = [](Environment* _env, Position _execPos)
	{
		auto time = std::chrono::high_resolution_clock::now();
		return SHARE(new StringValue(std::to_string((time - Interpreter::INIT_TIME).count()), _execPos));
	};
	std::vector<std::string> time_argNames({}), time_argSigs({});
	SharedValue func_time = SHARE(new FuncValue(env, time_body, time_argNames, time_argSigs, "<STRING>", Position()));

	env->AddFuncDeclaration("time", func_time, Position());

	//GetElapsedTime
	FuncValue::built_in getElapsedTime_body = [trace](Environment* _env, Position _execPos)
	{
		std::string start = _env->GetValue("start", _execPos, false)->ToString();
		std::string end = _env->GetValue("end", _execPos, false)->ToString();
		long double convStart = 0.0, convEnd = 0.0;

		try { convStart = std::stold(start); }
		catch(...) { return SHARE(Exception_InvalidOperation("Start time was not in the correct format!", trace)); }

		try { convEnd = std::stold(end); }
		catch(...) { return SHARE(Exception_InvalidOperation("End time was not in the correct format!", trace)); }

		std::string format = _env->GetValue("precision", _execPos, false)->AsString()->value;

		long double elapsed = convEnd - convStart;

		if(format == "s") { elapsed /= 1000000000.0l; }
		else if(format == "ms") { elapsed /= 1000000.0l; }
		else if(format == "us") { elapsed /= 1000.0l; }
		else if(format != "ns") { return SHARE(Exception_Format("Expected format to be 's', 'ms', 'us', or 'ns'!", trace)); }

		return SHARE(new StringValue(std::to_string(elapsed), _execPos));
	};
	std::vector<std::string> getElapsedTime_argNames({ "start", "end", "precision" }), getElapsedTime_ardSigs({ "<STRING>", "<STRING>", "<STRING>" });
	SharedValue func_getElapsedTime = SHARE(new FuncValue(env, getElapsedTime_body, getElapsedTime_argNames, getElapsedTime_ardSigs, "<STRING>", Position()));

	env->AddFuncDeclaration("getElapsedTime", func_getElapsedTime, Position());

	//Include
	FuncValue::built_in include_body = [trace](Environment* _env, Position _execPos)
	{
		const char* filePath = _env->GetValue("filePath", _execPos, false)->AsString()->value.c_str();

		if(!FileExists(filePath) && !FileExists((Interpreter::MainFilePath + SYS_SEP + filePath).c_str()))
			return SHARE(Exception_File("File at " + std::string(filePath) + " does not exist!", trace));

		std::string namedSpaceName = _env->GetValue("name", _execPos, false)->ToString();

		Environment* prevGlobal = Environment::GLOBAL; // Keep copy of calling file's environment to return to when finished
		SharedValue retVal = Interpreter::RunFile(filePath, namedSpaceName); // The sets the global environment to the file begin run
		
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
	std::vector<std::string> include_argNames({ "filePath", "name", "open" }), include_argSigs({ "<STRING>", "<STRING>", "<BOOL>" });
	SharedValue func_include = SHARE(new FuncValue(env, include_body, include_argNames, include_argSigs, "<VOID>", Position()));

	env->AddFuncDeclaration("include", func_include, Position());

	//Unscope
	FuncValue::built_in unscope_body = [trace](Environment* _env, Position _execPos)
	{
		SharedValue ns = _env->GetValue("namedspace", _execPos, false);
		return _env->parent->OpenNamedspace(ns->AsNamedspace(), _execPos);
	};
	std::vector<std::string> unscope_argNames({ "namedspace" }), unscope_argSigs({ "<NAMEDSPACE>" });
	SharedValue func_unscope = SHARE(new FuncValue(env, unscope_body, unscope_argNames, unscope_argSigs, "<VOID>", Position()));

	env->AddFuncDeclaration("unscope", func_unscope, Position());
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
			else { endVal = SHARE(new VoidValue(Position())); }

			std::cout << "Exited Running " + path << std::endl << std::endl;
			return endVal;
		}
		else { return SHARE(Exception_CompilationError("There was a parsing error in " + path + "\n\n" + parsingResult.message, Trace(parsingResult.position, _name, _path))); }
	}
	else { return SHARE(Exception_CompilationError("There was a tokenizing error in " + path + "\n\n" + tokenizationResult.message, Trace(tokenizationResult.position, _name, _path))); }
}