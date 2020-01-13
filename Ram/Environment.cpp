#include "pch.h"
#include "Environment.h"
#include "Value.h"
#include "Parser.h"
#include "Interpreter.h"

Environment* Environment::GLOBAL = 0;

Environment::Environment(Environment* _parent, std::string _name, std::string _filePath)
{
	parent = _parent;
	name = _name;
	filePath = _filePath;
	propReturn = true;
	propBreak = true;
	propContinue = true;
}

Environment::Environment(Environment* _parent, std::string _name, std::string _filePath, SignatureMap _typeDefs, std::map<ValueType, std::map<std::string, SharedValue>> _values)
	: Environment(_parent, _name, _filePath)
{
	typeDefs = _typeDefs;
	values = _values;
}

Environment::Environment(Environment* _parent, std::string _name) : Environment(_parent, _name, _parent->filePath) { }

Environment::~Environment()
{
	Clear();
}

void Environment::Clear()
{
	typeDefs.clear();
	values[VARIABLE].clear();
	values[FUNC_DECL].clear();
	values[NAMEDSPACE].clear();
}

bool Environment::IsVariable(std::string _identifier, bool _checkParent) { return values[VARIABLE].find(name + "." + _identifier) != values[VARIABLE].end() || (_checkParent && parent && parent->IsVariable(_identifier, true)); }
bool Environment::IsTypeName(std::string _identifier, bool _checkParent) { return typeDefs.find(name + "." + _identifier) != typeDefs.end() || (_checkParent && parent && parent->IsTypeName(_identifier, true)); }
bool Environment::IsFuncDecl(std::string _identifier, bool _checkParent) { return values[FUNC_DECL].find(name + "." + _identifier) != values[FUNC_DECL].end() || (_checkParent && parent && parent->IsFuncDecl(_identifier, true)); }
bool Environment::IsNamedspace(std::string _identifier, bool _checkParent) { return values[NAMEDSPACE].find(_identifier) != values[NAMEDSPACE].end() || (_checkParent && parent && parent->IsNamedspace(_identifier, true)); }

bool Environment::SymbolExists(std::string _symbol, bool _checkVariables, bool _checkTypeNames, bool _checkFuncDecls, bool _checkNamedspaces, bool _checkParent)
{
	if(_checkVariables && IsVariable(_symbol, _checkParent)) { return true; }
	else if(_checkTypeNames && IsTypeName(_symbol, _checkParent)) { return true; }
	else if(_checkFuncDecls && IsFuncDecl(_symbol, _checkParent)) { return true; }
	else if(_checkNamedspaces && IsNamedspace(_symbol, _checkParent)) { return true; }
	else { return false; }
}

SharedValue Environment::AddValue(ValueType _type, std::string _id, SharedValue _val, Position _execPos)
{
	if(SymbolExists(_id, true, true, true, true, false)) { return SHARE(Exception_SymbolInUse(_id, Trace(_execPos, name, filePath))); }
	else
	{
		std::string scopedID = name + "." + _id;

		if(_type == VARIABLE)
		{
			values[VARIABLE].erase(scopedID);
			values[VARIABLE].insert_or_assign(scopedID, _val);

			if(parent && ((propReturn && _id == "return") || (propBreak && _id == "break") || (propContinue && _id == "continue")))
				parent->AddValue(VARIABLE, _id, _val, _execPos);
		}
		else if(_type == FUNC_DECL) { values[FUNC_DECL].insert_or_assign(scopedID, _val); }
		else if(_type == NAMEDSPACE) { values[NAMEDSPACE].insert_or_assign(scopedID, _val); }

		return SHARE_VOID(_execPos);
	}
}

SharedValue Environment::AddMemberedTypeDefinition(TypeName _typeName, DefinitionMap& _memDefs, Position _execPos)
{
	if(SymbolExists(_typeName, true, true, true, true, false)) { return SHARE(Exception_SymbolInUse(_typeName, Trace(_execPos, name, filePath))); }
	else
	{
		std::string scopedTypeName = name + "." + _typeName;
		typeDefs.insert_or_assign(scopedTypeName, "<" + scopedTypeName + ">");

#pragma region Construct Type Sig
		Signature signature = "{";

		for(auto it : _memDefs)
		{
			SharedValue retVal = GetSignature(it.second, _execPos);

			if(retVal->GetType() == VEXCEPTION)
			{
				typeDefs.erase(scopedTypeName);
				return retVal;
			}
			else { signature += it.first + ":" + retVal->AsString()->value + "; "; }
		}

		if(_memDefs.size() != 0)
		{
			signature.pop_back();
			signature.pop_back();
		}

		signature += "}#" + IValue::SIGNATURE_MEMBERED;
#pragma endregion

		typeDefs.insert_or_assign(scopedTypeName, signature);
		return SHARE_VOID(_execPos);
	}
}

SharedValue Environment::AddFuncTypeDefinition(TypeName _typeName, std::vector<std::string>& _argTypeNames, std::string _retTypeName, Position _execPos)
{
	if(SymbolExists(_typeName, true, true, true, true, false)) { return SHARE(Exception_SymbolInUse(_typeName, Trace(_execPos, name, filePath))); }
	else
	{
		std::string scopedTypeName = name + "." + _typeName;
		typeDefs.insert_or_assign(scopedTypeName, "<" + scopedTypeName + ">");

#pragma region Construct Type Sig
		Signature signature;
		std::vector<Signature> argSigs;

		for(auto it : _argTypeNames)
		{
			SharedValue argSigVal = GetSignature(it, _execPos);

			if(argSigVal->GetType() == VEXCEPTION)
			{
				typeDefs.erase(scopedTypeName);
				return argSigVal;
			}
			else { argSigs.push_back(argSigVal->AsString()->value); }
		}


		SharedValue retSigVal = GetSignature(_retTypeName, _execPos);

		if(retSigVal->GetType() == VEXCEPTION)
		{
			typeDefs.erase(scopedTypeName);
			return retSigVal;
		}
		else { signature = FuncValue::GenerateSignature(argSigs, retSigVal->AsString()->value); }
#pragma endregion

		typeDefs.insert_or_assign(scopedTypeName, signature);
		return SHARE_VOID(_execPos);
	}
}

SharedValue Environment::AddVariable(std::string _identifier, SharedValue _val, Position _execPos) { return AddValue(VARIABLE, _identifier, _val, _execPos); }
SharedValue Environment::AddFuncDeclaration(std::string _identifier, SharedValue _val, Position _execPos) { return AddValue(FUNC_DECL, _identifier, _val, _execPos); }

SharedValue Environment::AddNamedspace(Environment * _ns, Position _execPos, bool _openName)
{
	NamedspaceValue* nsVal = new NamedspaceValue(_ns, _execPos);
	SharedValue addResult = AddValue(NAMEDSPACE, _ns->name, SHARE(nsVal), _execPos);

	if(addResult->GetType() != VEXCEPTION)
	{
		for(auto it : _ns->typeDefs)
			typeDefs.insert_or_assign(it.first, it.second);

		if(_openName)
			OpenNamedspace(nsVal, _execPos);
	}

	return addResult;
}

SharedValue Environment::GetValue(std::string _identifier, Position _execPos, bool _checkParent)
{
	std::string scopedID = name + "." + _identifier;
	auto search = values[VARIABLE].find(scopedID);

	if(search != values[VARIABLE].end()) { return search->second; }
	else if((search = values[FUNC_DECL].find(scopedID)) != values[FUNC_DECL].end()) { return search->second; }
	else if((search = values[NAMEDSPACE].find(scopedID)) != values[NAMEDSPACE].end()) { return search->second; }
	else if(parent && _checkParent) { return parent->GetValue(_identifier, _execPos, true); }
	else { return SHARE(Exception_SymbolNotFound(_identifier, Trace(_execPos, name, filePath))); }
}

SharedValue Environment::OpenNamedspace(NamedspaceValue* _nsVal, Position _execPos)
{
	Environment* nsEnv = _nsVal->environment;

	//Add Type Defs
	for(auto it : nsEnv->typeDefs)
	{
		auto firstPeriod = it.first.find_first_of(".");
		if(std::string::npos != firstPeriod)
		{
			std::string typeName = it.first.substr(firstPeriod + 1, it.first.size() - firstPeriod - 1);
			typeDefs.insert_or_assign(name + "." + typeName, it.second);
		}
	}

	//Add Values
	for(auto it : nsEnv->values)
	{
		for(auto it2 : it.second)
		{
			const size_t firstPeriod = it2.first.find_first_of(".");
			if(std::string::npos != firstPeriod)
			{
				std::string typeName = it2.first.substr(firstPeriod + 1, it2.first.size() - firstPeriod - 1);
				values[it.first].insert_or_assign(name + "." + typeName, it2.second);
			}
		}
	}

	return SHARE_VOID(_execPos);
}

SharedValue Environment::GetSignature(TypeName _typeName, Position _execPos)
{
	auto suffixStart = _typeName.find_first_of('|');
	if(suffixStart != std::string::npos)
	{
		SharedValue base = GetSignature(_typeName.substr(0, suffixStart), _execPos);

		if(base->GetType() == VEXCEPTION) { return base; }
		else
		{ 
			std::string newTypeName = base->AsString()->value + _typeName.substr(suffixStart, _typeName.size() - suffixStart);
			return SHARE(new StringValue(newTypeName, _execPos));
		}
	}

	std::string scopedTypeName = name + "." + _typeName;
	auto search = typeDefs.find(scopedTypeName);

	if(search != typeDefs.end()) { return SHARE(new StringValue(search->second, _execPos)); }
	else if((search = typeDefs.find(_typeName)) != typeDefs.end()) { return SHARE(new StringValue(search->second, _execPos)); }
	else if(parent) { return parent->GetSignature(_typeName, _execPos); }
	else { return SHARE(Exception_SymbolNotFound(_typeName, Trace(_execPos, name, filePath))); }
}

Environment* Environment::GetCopy()
{
	std::map<ValueType, std::map<std::string, SharedValue>> copyVals;

	for(auto it : values)
	{
		copyVals[it.first] = std::map<std::string, SharedValue>();

		for(auto it2 : it.second)
			copyVals[it.first].insert_or_assign(it2.first, SHARE_COPY(it2.second));
	}

	return new Environment(parent, name, filePath, typeDefs, copyVals);
}

std::string Environment::ToString()
{
	std::ostringstream os;
	os << "-------------------------------------------------------" << std::endl;
	os << "-----------------------VARIABLES-----------------------" << std::endl;

	Environment* parentEnv = this;
	std::string prefix = parent == 0 ? "(GLOBAL " : "(LOCAL ";

	while(parentEnv)
	{
		for(auto it = parentEnv->typeDefs.begin(); it != parentEnv->typeDefs.end(); it++)
			os << prefix << "TYPE) " << it->first << ":" << it->second << std::endl << std::endl;

		auto variables = parentEnv->values[VARIABLE];
		for(auto it = variables.begin(); it != variables.end(); it++)
			os << prefix << "VAR) " << it->first << ":" << it->second->GetSignature() << " = " << it->second->ToString() << std::endl << std::endl;

		auto funcDecls = parentEnv->values[FUNC_DECL];
		for(auto it = funcDecls.begin(); it != funcDecls.end(); it++)
			os << prefix << ("FUNC) ") << it->first << ":" << it->second->GetSignature() << " = " << it->second->ToString() << std::endl << std::endl;

		auto namedspaces = parentEnv->values[NAMEDSPACE];
		for(auto it = namedspaces.begin(); it != namedspaces.end(); it++)
			os << prefix << "NAMEDSPACE) " << it->first << ":" << it->second->GetSignature() << " = " << it->second->ToString() << std::endl << std::endl;

		parentEnv = parentEnv->parent;
		prefix = "(GLOBAL ";
	}

	os << "-----------------------------------------------------" << std::endl;
	return os.str();
}

Environment* Environment::CreateGlobal(std::string _name, std::string _filePath) { return new Environment(Interpreter::ENV_STD, _name, _filePath); }
