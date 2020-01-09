#pragma once

class Environment
{
private:
	std::map<std::string, SharedValue> variables;
	TypeSigMap typeDefs;
	std::map<std::string, SharedValue> funcDecls;
	std::map<std::string, SharedValue> namedspaces;

	Environment(Environment* _parent, std::string _name, std::string _filePath, std::map<std::string, SharedValue> _variables, TypeSigMap _typeDefs, std::map<std::string, SharedValue> _funcDecls, std::map<std::string, SharedValue> _namedspaces);
public:
	std::string name;
	std::string filePath;

	static Environment* GLOBAL;

	Environment* parent;

	Environment(Environment* _parent, std::string _name, std::string _filePath);
	Environment(Environment* _parent, std::string _name);
	~Environment();

	void Clear(Environment* _newParent);

	bool IsVariable(std::string _identifier, bool _checkParent);
	bool IsTypeName(std::string _identifier, bool _checkParent);
	bool IsFuncDecl(std::string _identifier, bool _checkParent);
	bool IsNamedspace(std::string _identifier, bool _checkParent);
	bool SymbolExists(std::string _symbol, bool _checkVariables, bool _checkTypeNames, bool _checkFuncDecls, bool _checkNamedSpaces, bool _checkParent);

	SharedValue AddVariable(std::string _identifier, SharedValue _val, Position _execPos);
	SharedValue AddTypeDefinition(TypeName _typeName, DefinitionMap& _typeDefs, Position _execPos);
	SharedValue AddFuncDeclaration(std::string _identifier, SharedValue _val, Position _execPos);
	SharedValue AddNamedspace(Environment* _ns, Position _execPos);

	SharedValue SetValue(std::string _identifier, SharedValue _val, Position _execPos);
	SharedValue GetValue(std::string _identifier, Position _execPos, bool _checkParent);
	SharedValue GetTypeSig(TypeName _typeName, Position _execPos);

	Environment* GetCopy();
	std::string ToString();

	static Environment* CreateGlobal(std::string _name, std::string _filePath);
};

