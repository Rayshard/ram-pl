#include "Environment.h"
#include "Grammar\Statement.h"

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

IValue* Environment::AddTypeDefinition(TypeName _typeName, TypeDef _typeDef, TypeSig _typeSig, Position _execPos)
{
	if(SymbolExists(_typeName, true, true, true, true, true)) { return Exception_SymbolInUse(_typeName, _execPos); }
	else
	{
		typeDefs.insert_or_assign(_typeName, DefSigPair(_typeDef, _typeSig));
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
			AddTypeDefinition(identifier + "." + it.first, it.second.first, it.second.second, _execPos);

		return new VoidValue(_execPos);
	}
}

IValue* Environment::SetValue(std::string _identifier, IValue* _val, Position _execPos)
{
	auto search = variables.find(name + "." + _identifier);
	if(search != variables.end())
	{
		std::string orgTypeName = _val->typeName;
		std::string valTypeName = search->second->typeName;

		if(orgTypeName == valTypeName || valTypeName == "void")
		{
			delete search->second;
			variables.insert_or_assign(name + "." + _identifier, _val);
			return new VoidValue(_execPos);
		}
		else { return Exception_MismatchType(orgTypeName, valTypeName, _execPos); }
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

IValue* Environment::GetTypeSigFromTypeName(TypeName _typeName, Position _execPos)
{
	auto search = typeDefs.find(_typeName);

	if(search != typeDefs.end()) { return new StringValue(search->second.second, _execPos); }
	else if(parent) { return parent->GetTypeSigFromTypeName(_typeName, _execPos); }
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
			os << prefix << it->first << ":" << it->second->GetTypeSig() << " = " << it->second->ToString(this) << std::endl << std::endl;

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
			os << prefix << it->first << ":" << it->second.first << std::endl << std::endl;

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
	delete env->AddTypeDefinition("int", "int", "<INT>", Position());
	delete env->AddTypeDefinition("float", "float", "<FLOAT>", Position());
	delete env->AddTypeDefinition("string", "string", "<STRING>", Position());
	delete env->AddTypeDefinition("bool", "bool", "<BOOL>", Position());

	//Functions (delete throws away the returned VoidValue)
	BuiltInStatement::built_in lambda;

	lambda = [](Environment* _env, Position _execPos) 
	{
		std::cout << _env->GetValue("arg", _execPos, false, false)->ToString(_env);
		return new VoidValue(_execPos);
	};
	IStatement* func_print_body = new BuiltInStatement(lambda);
	std::vector<Definition> func_print_argDefs({ Definition("arg", "string") });
	FuncValue* func_print = new FuncValue(env, func_print_body, func_print_argDefs, "void", func_print_body->_position);

	delete env->AddFuncDeclaration("print", func_print, func_print_body->_position);

	lambda = [](Environment* _env, Position _execPos)
	{
		std::string input;
		std::getline(std::cin, input);
		return new StringValue(input, _execPos);
	};
	IStatement* func_getInput_body = new BuiltInStatement(lambda);
	std::vector<Definition> func_getInput_argDefs({});
	FuncValue* func_getInput = new FuncValue(env, func_getInput_body, func_getInput_argDefs, "string", func_getInput_body->_position);

	delete env->AddFuncDeclaration("getInput", func_getInput, func_print_body->_position);

	return env;
}
