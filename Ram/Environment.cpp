#include "Environment.h"
#include "Grammar\Statement.h"
#include "Parser.h"

Environment* Environment::GLOBAL = 0;

Environment::Environment(Environment* _parent, std::string _name, std::map<std::string, SharedValue> _variables, TypeSigMap _typeDefs, std::map<std::string, SharedValue> _funcDecls, std::map<std::string, SharedValue> _namedspaces)
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
	name = _name;
}

Environment::~Environment()
{
	Clear(0);
}

void Environment::Clear(Environment* _newParent)
{
	parent = _newParent;

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

SharedValue Environment::AddVariable(std::string _identifier, SharedValue _val, Position _execPos)
{
	std::string scopedID = name + "." + _identifier;
	variables.erase(scopedID);

	if(SymbolExists(scopedID, true, true, true, true, false)) { return SharedValue(Exception_SymbolInUse(_identifier, Trace(_execPos, name, "Put FileName Here"))); }
	else
	{
		variables.insert_or_assign(scopedID, _val);
		return SharedValue(new VoidValue(_execPos));
	}
}

SharedValue Environment::AddTypeDefinition(TypeName _typeName, DefinitionMap& _memDefs, Position _execPos)
{
	std::string scopedTypeName = name + "." + _typeName;

	if(SymbolExists(scopedTypeName, true, true, true, true, false)) { return SharedValue(Exception_SymbolInUse(_typeName, Trace(_execPos, name, "Put FileName Here"))); }
	else
	{
		typeDefs.insert_or_assign(scopedTypeName, "<" + scopedTypeName + ">");

#pragma region Construct Type Sig
		TypeSig typeSig = "<MEMBERED>{";

		for(auto it : _memDefs)
		{
			SharedValue retVal = GetTypeSig(it.second, _execPos);

			if(retVal->_type == VEXCEPTION)
			{
				typeDefs.erase(scopedTypeName);
				return retVal;
			}
			else { typeSig += it.first + ":" + ((StringValue*)retVal.get())->value + "; "; }
		}

		if(_memDefs.size() != 0)
		{
			typeSig.pop_back();
			typeSig.pop_back();
		}

		typeSig += "}";
#pragma endregion

		typeDefs.insert_or_assign(scopedTypeName, typeSig);
		return SharedValue(new VoidValue(_execPos));
	}
}

SharedValue Environment::AddFuncDeclaration(std::string _identifier, SharedValue _val, Position _execPos)
{
	std::string scopedID = name + "." + _identifier;
	if(SymbolExists(scopedID, true, true, true, true, false)) { return SharedValue(Exception_SymbolInUse(_identifier, Trace(_execPos, name, "Put FileName Here"))); }
	else
	{
		funcDecls.insert_or_assign(scopedID, _val);
		return SharedValue(new VoidValue(_execPos));
	}
}

SharedValue Environment::AddNamedspace(Environment* _ns, Position _execPos)
{
	std::string identifier = name.empty() ? _ns->name : (name + "." + _ns->name);

	if(SymbolExists(identifier, true, true, true, true, true)) { return SharedValue(Exception_SymbolInUse(_ns->name, Trace(_execPos, name, "Put FileName Here"))); }
	else
	{
		namedspaces.insert_or_assign(identifier, SharedValue(new NamedspaceValue(_ns, _execPos)));

		for(auto it : _ns->typeDefs)
			typeDefs.insert_or_assign(identifier + "." + it.first, it.second);

		return SharedValue(new VoidValue(_execPos));
	}
}

SharedValue Environment::SetValue(std::string _identifier, SharedValue _val, Position _execPos)
{
	std::string scopeID = name + "." + _identifier;
	auto search = variables.find(scopeID);

	if(search != variables.end())
	{
		std::string orgTypeSig = _val->GetTypeSig();
		std::string valTypeSig = search->second->GetTypeSig();

		if(orgTypeSig == valTypeSig || valTypeSig == "void")
		{
			variables.erase(scopeID);
			variables.insert_or_assign(scopeID, _val);
			return SharedValue(new VoidValue(_execPos));
		}
		else { return SharedValue(Exception_MismatchType(orgTypeSig, valTypeSig, Trace(_execPos, name, "Put FileName Here"))); }
	}
	else if(parent) { return parent->SetValue(_identifier, _val, _execPos); }
	else { return SharedValue(Exception_SymbolNotFound(_identifier, Trace(_execPos, name, "Put FileName Here"))); }
}

SharedValue Environment::GetValue(std::string _identifier, Position _execPos, bool _checkParent)
{
	std::string scopedID = name + "." + _identifier;
	auto search = variables.find(scopedID);

	if(search != variables.end()) { return search->second; }
	else if((search = funcDecls.find(scopedID)) != funcDecls.end()) { return search->second; }
	else if((search = namedspaces.find(_identifier)) != namedspaces.end()) { return search->second; }
	else if(parent && _checkParent) { return parent->GetValue(_identifier, _execPos, true); }
	else { return SharedValue(Exception_SymbolNotFound(_identifier, Trace(_execPos, name, "Put FileName Here"))); }
}

SharedValue Environment::GetTypeSig(TypeName _typeName, Position _execPos)
{
	std::string scopedTypeName = name + "." + _typeName;
	auto search = typeDefs.find(scopedTypeName);

	if(search != typeDefs.end()) { return SharedValue(new StringValue(search->second, _execPos)); }
	else if(parent) { return parent->GetTypeSig(_typeName, _execPos); }
	else { return SharedValue(Exception_SymbolNotFound(_typeName, Trace(_execPos, name, "Put FileName Here"))); }
}

Environment* Environment::GetCopy() { return new Environment(parent, name, variables, typeDefs, funcDecls, namedspaces); }

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

Environment* Environment::CreateGlobal(std::string _name)
{
	Environment* env = new Environment(0, _name);

	//Type Defs (delete throws away the returned VoidValue)
	env->typeDefs.insert_or_assign(_name + ".int", "<INT>");
	env->typeDefs.insert_or_assign(_name + ".float", "<FLOAT>");
	env->typeDefs.insert_or_assign(_name + ".string", "<STRING>");
	env->typeDefs.insert_or_assign(_name + ".bool", "<BOOL>");
	env->typeDefs.insert_or_assign(_name + ".void", "<VOID>");

	//Functions (delete throws away the returned VoidValue)
	FuncValue::built_in print_body = [](Environment* _env, Position _execPos)
	{
		std::cout << _env->GetValue("arg", _execPos, false)->ToString();
		return SharedValue(new VoidValue(_execPos));
	};
	std::vector<std::string> print_argNames({ "arg" }), print_argSigs({ "<STRING>" });
	SharedValue func_print = SharedValue(new FuncValue(env, print_body, print_argNames, print_argSigs, "<VOID>", Position()));

	env->AddFuncDeclaration("print", func_print, Position());

	FuncValue::built_in getInput_body = [](Environment* _env, Position _execPos)
	{
		std::string input;
		std::getline(std::cin, input);
		return SharedValue(new StringValue(input, _execPos));
	};
	std::vector<std::string> getInput_argNames({}), getInput_argSigs({ "<STRING>" });
	SharedValue func_getInput = SharedValue(new FuncValue(env, getInput_body, getInput_argNames, getInput_argSigs, "<STRING>", Position()));

	env->AddFuncDeclaration("getInput", func_getInput, Position());

	FuncValue::built_in include_body = [](Environment* _env, Position _execPos)
	{
		const char* fileName = _env->GetValue("fileName", _execPos, false)->ToString().c_str();
		std::string nameSpaceName = _env->GetValue("name", _execPos, false)->ToString().c_str();

		Environment* prevGlobal = Environment::GLOBAL;
		Environment* namedspace = Environment::CreateGlobal(nameSpaceName);
		Environment::GLOBAL = namedspace;

		SharedValue retVal = RunFile(fileName, namedspace, false);
		SharedValue nsAddResult = prevGlobal->AddNamedspace(namedspace, _execPos);

		if(nsAddResult->_type == VEXCEPTION)
		{
			retVal.reset();
			retVal = nsAddResult;

			delete namedspace;
		}

		Environment::GLOBAL = prevGlobal;
		return retVal;
	};
	std::vector<std::string> include_argNames({ "fileName", "name" }), include_argSigs({ "<STRING>", "<STRING>" });
	SharedValue func_include = SharedValue(new FuncValue(env, include_body, include_argNames, include_argSigs, "<VOID>", Position()));

	env->AddFuncDeclaration("include", func_include, Position());

	return env;
}
