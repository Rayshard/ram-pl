#pragma once

class NamedspaceValue;

class Environment
{
private:
	enum ValueType { VARIABLE, FUNC_DECL, NAMEDSPACE };

	std::map<ValueType, std::map<std::string, SharedValue>> values;

	Environment(Environment* _parent, std::string _name, std::string _filePath, TypeSigMap _typeDefs, std::map<ValueType, std::map<std::string, SharedValue>> _values);
	
	SharedValue AddValue(ValueType _type, std::string _identifier, SharedValue _val, Position _execPos);
public:
	static Environment* GLOBAL;

	Environment* parent;
	TypeSigMap typeDefs;

	std::string name;
	std::string filePath;
	bool propReturn, propBreak, propContinue;

	Environment(Environment* _parent, std::string _name, std::string _filePath);
	Environment(Environment* _parent, std::string _name);
	~Environment();

	void Clear();

	bool IsVariable(std::string _identifier, bool _checkParent);
	bool IsTypeName(std::string _identifier, bool _checkParent);
	bool IsFuncDecl(std::string _identifier, bool _checkParent);
	bool IsNamedspace(std::string _identifier, bool _checkParent);
	bool SymbolExists(std::string _symbol, bool _checkVariables, bool _checkTypeNames, bool _checkFuncDecls, bool _checkNamedSpaces, bool _checkParent);

	SharedValue AddTypeDefinition(TypeName _typeName, DefinitionMap& _typeDefs, Position _execPos);
	SharedValue AddVariable(std::string _id, SharedValue _val, Position _execPos);
	SharedValue AddFuncDeclaration(std::string _identifier, SharedValue _val, Position _execPos);
	SharedValue AddNamedspace(Environment* _ns, Position _execPos, bool _openName);

	SharedValue GetValue(std::string _identifier, Position _execPos, bool _checkParent);
	SharedValue OpenNamedspace(NamedspaceValue* _nsVal, Position _execPos);
	SharedValue GetTypeSig(TypeName _typeName, Position _execPos);

	Environment* GetCopy();
	std::string ToString();

	static Environment* CreateGlobal(std::string _name, std::string _filePath);
};

