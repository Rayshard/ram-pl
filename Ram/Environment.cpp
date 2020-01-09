#include "Environment.h"
#include "Grammar\Statement.h"
#include "Parser.h"

Environment* Environment::GLOBAL = 0;

Environment::Environment(Environment* _parent, std::string _name, ValueMap _variables, TypeSigMap _typeDefs, ValueMap _funcDecls, ValueMap _namedspaces)
	: Environment(_parent, _name)
{
	variables = _variables;
	typeDefs = _typeDefs;
	funcDecls = _funcDecls;
	namedspaces = _namedspaces;
}

Environment::Environment(Environment* _parent, std::string _name)
{
	parent = _parent;
	name = parent ? (_parent->name.empty() ? _name : _parent->name + '.' + _name) : _name;
}

Environment::~Environment()
{
	Clear(0);
}

void Environment::Clear(Environment* _newParent)
{
	parent = _newParent;

	for(auto it : variables)
		delete it.second;

	for(auto it : funcDecls)
		delete it.second;

	for(auto it : namedspaces)
		delete it.second;

	variables.clear();
	typeDefs.clear();
	funcDecls.clear();
	namedspaces.clear();
}

bool Environment::IsVariable(std::string _identifier, bool _checkParent) { return variables.find(_identifier) != variables.end() || (_checkParent && parent && parent->IsVariable(_identifier, true)); }
bool Environment::IsTypeName(std::string _identifier, bool _checkParent) { return typeDefs.find(_identifier) != typeDefs.end() || (_checkParent && parent && parent->IsTypeName(_identifier, true)); }
bool Environment::IsFuncDecl(std::string _identifier, bool _checkParent) { return funcDecls.find(_identifier) != funcDecls.end() || (_checkParent && parent && parent->IsFuncDecl(_identifier, true)); }
bool Environment::IsNamedspace(std::string _identifier, bool _checkParent) { return namedspaces.find(_identifier) != namedspaces.end() || (_checkParent && parent && parent->IsNamedspace(_identifier, true)); }

bool Environment::SymbolExists(std::string _symbol, bool _checkVariables, bool _checkTypeNames, bool _checkFuncDecls, bool _checkNamedspaces, bool _checkParent)
{
	if(_checkVariables && IsVariable(_symbol, _checkParent)) { return true; }
	else if(_checkTypeNames && IsTypeName(_symbol, _checkParent)) { return true; }
	else if(_checkFuncDecls && IsFuncDecl(_symbol, _checkParent)) { return true; }
	else if(_checkNamedspaces && IsNamedspace(_symbol, _checkParent)) { return true; }
	else { return false; }
}

IValue* Environment::AddVariable(std::string _identifier, IValue* _val, Position _execPos, bool _checkParent, bool _membered)
{
	std::string scopedID = name + "." + _identifier;
	auto search = variables.find(scopedID);
	if(search != variables.end())
		delete search->second;

	//We use !membered so that upon creating membered values, the assignment of
	//the members is not restricted by symbols outside
	if(SymbolExists(scopedID, true, true, true, true, _checkParent)) { return Exception_SymbolInUse(scopedID, _execPos); }
	else
	{
		variables.insert_or_assign(scopedID, _val);
		return new VoidValue(_execPos);
	}
}

IValue* Environment::AddTypeDefinition(TypeName _typeName, DefinitionMap& _memDefs, Position _execPos)
{
	if(SymbolExists(_typeName, true, true, true, true, true)) { return Exception_SymbolInUse(_typeName, _execPos); }
	else
	{
		typeDefs.insert_or_assign(_typeName, "<" + _typeName + ">");

#pragma region Construct Type Sig
		TypeSig typeSig = "<MEMBERED>{";

		for(auto it : _memDefs)
		{
			IValue* retVal = GetTypeSig(it.second, _execPos);

			if(retVal->_type == VEXCEPTION)
			{
				typeDefs.erase(_typeName);
				return retVal;
			}
			else
			{
				typeSig += it.first + ":" + ((StringValue*)retVal)->value + "; ";
				delete retVal;
			}
		}

		if(_memDefs.size() != 0)
		{
			typeSig.pop_back();
			typeSig.pop_back();
		}

		typeSig += "}";
#pragma endregion

		typeDefs.insert_or_assign(_typeName, typeSig);
		return new VoidValue(_execPos);
	}
}

IValue * Environment::AddFuncDeclaration(std::string _identifier, FuncValue * _val, Position _execPos)
{
	std::string scopedID = name + "." + _identifier;
	if(SymbolExists(scopedID, true, true, true, true, false)) { return Exception_SymbolInUse(scopedID, _execPos); }
	else
	{
		funcDecls.insert_or_assign(scopedID, _val);
		return new VoidValue(_execPos);
	}
}

IValue* Environment::AddNamedspace(Environment* _ns, Position _execPos)
{
	std::string identifier = name.empty() ? _ns->name : name + "." + _ns->name;

	if(SymbolExists(identifier, true, true, true, true, true)) { return Exception_SymbolInUse(identifier, _execPos); }
	else
	{
		namedspaces.insert_or_assign(identifier, new NamedspaceValue(_ns, _execPos));

		for(auto it : _ns->typeDefs)
			typeDefs.insert_or_assign(identifier + "." + it.first, it.second);

		return new VoidValue(_execPos);
	}
}

IValue* Environment::SetValue(std::string _identifier, IValue* _val, Position _execPos)
{
	auto search = variables.find(name + "." + _identifier);
	if(search != variables.end())
	{
		std::string orgTypeSig = _val->GetTypeSig();
		std::string valTypeSig = search->second->GetTypeSig();

		if(orgTypeSig == valTypeSig || valTypeSig == "void")
		{
			delete search->second;
			variables.insert_or_assign(name + "." + _identifier, _val);
			return new VoidValue(_execPos);
		}
		else { return Exception_MismatchType(orgTypeSig, valTypeSig, _execPos); }
	}
	else if(parent) { return parent->SetValue(_identifier, _val, _execPos); }
	else { return Exception_SymbolNotFound(_identifier, _execPos); }
}

IValue* Environment::GetValue(std::string _identifier, Position _execPos, bool _checkParent, bool _retCopy)
{
	std::string scopedID = name + "." + _identifier;
	auto search = variables.find(scopedID);

	if(search != variables.end()) { return _retCopy ? search->second->GetCopy() : search->second; }
	else if((search = funcDecls.find(scopedID)) != funcDecls.end()) { return _retCopy ? search->second->GetCopy() : search->second; }
	else if((search = namedspaces.find(_identifier)) != namedspaces.end()) { return search->second; }
	else if(parent && _checkParent) { return parent->GetValue(_identifier, _execPos, true, _retCopy); }
	else { return Exception_SymbolNotFound(_identifier, _execPos); }
}

IValue* Environment::GetTypeSig(TypeName _typeName, Position _execPos)
{
	auto search = typeDefs.find(_typeName);

	if(search != typeDefs.end()) { return new StringValue(search->second, _execPos); }
	else if(parent) { return parent->GetTypeSig(_typeName, _execPos); }
	else { return Exception_SymbolNotFound(_typeName, _execPos); }
}

Environment* Environment::GetCopy()
{
	ValueMap copyVars;
	ValueMap copyFuncDecls;
	ValueMap copyNamedspaces;

	for(auto it : variables)
		copyVars.insert_or_assign(it.first, it.second->GetCopy());

	for(auto it : funcDecls)
		copyFuncDecls.insert_or_assign(it.first, it.second->GetCopy());

	for(auto it : namedspaces)
		copyNamedspaces.insert_or_assign(it.first, it.second->GetCopy());

	return new Environment(parent, name, copyVars, typeDefs, copyFuncDecls, copyNamedspaces);
}

std::string Environment::ToString()
{
	std::ostringstream os;
	os << "-------------------------------------------------------" << std::endl;
	os << "-----------------------VARIABLES-----------------------" << std::endl;

	Environment* parentEnv = this;
	std::string prefix = parent == 0 ? "(GLOBAL) " : "(LOCAL) ";

	while(parentEnv)
	{
		for(auto it = parentEnv->variables.begin(); it != parentEnv->variables.end(); it++)
			os << prefix << it->first << ":" << it->second->GetTypeSig() << " = " << it->second->ToString() << std::endl << std::endl;

		parentEnv = parentEnv->parent;
		prefix = "(GLOBAL) ";
	}

	os << "-----------------------------------------------------" << std::endl << std::endl;
	os << "-------------------Type Definitions--------------------" << std::endl;

	parentEnv = this;
	prefix = parent == 0 ? "(GLOBAL) " : "(LOCAL) ";

	while(parentEnv)
	{
		for(auto it = parentEnv->typeDefs.begin(); it != parentEnv->typeDefs.end(); it++)
			os << prefix << it->first << ":" << it->second << std::endl << std::endl;

		parentEnv = parentEnv->parent;
		prefix = "(GLOBAL) ";
	}

	os << "-----------------------------------------------------" << std::endl << std::endl;
	os << "---------------Function Declarations-----------------" << std::endl;

	parentEnv = this;
	prefix = parent == 0 ? "(GLOBAL) " : "(LOCAL) ";

	while(parentEnv)
	{
		for(auto it = parentEnv->funcDecls.begin(); it != parentEnv->funcDecls.end(); it++)
			os << prefix << it->first << ":" << it->second->GetTypeSig() << std::endl << std::endl;

		parentEnv = parentEnv->parent;
		prefix = "(GLOBAL) ";
	}

	os << "-----------------------------------------------------" << std::endl << std::endl;
	os << "--------------------Namedspaces----------------------" << std::endl;

	parentEnv = this;
	prefix = parent == 0 ? "(GLOBAL) " : "(LOCAL) ";

	while(parentEnv)
	{
		for(auto it = parentEnv->namedspaces.begin(); it != parentEnv->namedspaces.end(); it++)
			os << prefix << it->first << ":" << it->second->GetTypeSig() << std::endl << std::endl;

		parentEnv = parentEnv->parent;
		prefix = "(GLOBAL) ";
	}

	os << "-----------------------------------------------------" << std::endl;

	return os.str();
}

Environment* Environment::CreateGlobal(std::string _namedspaceName)
{
	Environment* env = new Environment(0, _namedspaceName);

	//Type Defs (delete throws away the returned VoidValue)
	env->typeDefs.insert_or_assign("int", "<INT>");
	env->typeDefs.insert_or_assign("float", "<FLOAT>");
	env->typeDefs.insert_or_assign("string", "<STRING>");
	env->typeDefs.insert_or_assign("bool", "<BOOL>");
	env->typeDefs.insert_or_assign("void", "<VOID>");

	//Functions (delete throws away the returned VoidValue)
	FuncValue::built_in print_body = [](Environment* _env, Position _execPos)
	{
		std::cout << _env->GetValue("arg", _execPos, false, false)->ToString();
		return new VoidValue(_execPos);
	};
	std::vector<std::string> print_argNames({ "arg" }), print_argSigs({ "<STRING>" });
	FuncValue* func_print = new FuncValue(env, print_body, print_argNames, print_argSigs, "<VOID>", Position());

	delete env->AddFuncDeclaration("print", func_print, Position());

	FuncValue::built_in getInput_body = [](Environment* _env, Position _execPos)
	{
		std::string input;
		std::getline(std::cin, input);
		return new StringValue(input, _execPos);
	};
	std::vector<std::string> getInput_argNames({}), getInput_argSigs({ "<STRING>" });
	FuncValue* func_getInput = new FuncValue(env, getInput_body, getInput_argNames, getInput_argSigs, "<STRING>", Position());

	delete env->AddFuncDeclaration("getInput", func_getInput, Position());

	FuncValue::built_in include_body = [](Environment* _env, Position _execPos)
	{
		const char* fileName = _env->GetValue("fileName", _execPos, false, false)->ToString().c_str();
		std::string nameSpaceName = _env->GetValue("name", _execPos, false, false)->ToString().c_str();

		Environment* prevGlobal = Environment::GLOBAL;
		Environment* namedspace = Environment::CreateGlobal(nameSpaceName);
		Environment::GLOBAL = namedspace;

		IValue* retVal = RunFile(fileName, namedspace, false);
		IValue* nsAddResult = prevGlobal->AddNamedspace(namedspace, _execPos);

		if(nsAddResult->_type == VEXCEPTION)
		{
			delete retVal;
			delete namedspace;

			retVal = nsAddResult;
		}

		Environment::GLOBAL = prevGlobal;
		return retVal;
	};
	std::vector<std::string> include_argNames({ "fileName", "name" }), include_argSigs({ "<STRING>", "<STRING>" });
	FuncValue* func_include = new FuncValue(env, include_body, include_argNames, include_argSigs, "<VOID>", Position());

	delete env->AddFuncDeclaration("include", func_include, Position());

	return env;
}
