#pragma once

#include <map>
#include "Grammar\Value.h"

class IValue;
struct Position;

class Environment
{
private:
	ValueMap variables;
	TypeSigMap typeDefs;
	ValueMap funcDecls;
	ValueMap namedspaces;

	Environment(Environment* _parent, std::string _name, ValueMap _variables, TypeSigMap _typeDefs, ValueMap _funcDecls, ValueMap _namedspaces);
public:
	std::string name;

	static Environment* GLOBAL;
	
	Environment* parent;

	Environment(Environment* _parent, std::string _name);
	~Environment();

	void Clear(Environment* _newParent);
	
	bool IsVariable(std::string _identifier, bool _checkParent);
	bool IsTypeName(std::string _identifier, bool _checkParent);
	bool IsFuncDecl(std::string _identifier, bool _checkParent);
	bool IsNamedspace(std::string _identifier, bool _checkParent);
	bool SymbolExists(std::string _symbol, bool _checkVariables, bool _checkTypeNames, bool _checkFuncDecls, bool _checkNamedSpaces, bool _checkParent);
	
	IValue* AddVariable(std::string _identifier, IValue* _val, Position _execPos, bool _checkParent, bool membered);
	IValue* AddTypeDefinition(TypeName _typeName, TypeDef _typeDef, TypeSig _typeSig, Position _execPos);
	IValue* AddFuncDeclaration(std::string _identifier, FuncValue* _val, Position _execPos);
	IValue* AddNamedspace(Environment* _ns, Position _execPos);
	
	IValue* SetValue(std::string _identifier, IValue* _val, Position _execPos);
	IValue* GetValue(std::string _identifier, Position _execPos, bool _checkParent, bool _retCopy);
	IValue* GetTypeSigFromTypeName(TypeName _typeName, Position _execPos);

	Environment* GetCopy();
	std::string ToString();

	static Environment* CreateGlobal(std::string _name);
};

