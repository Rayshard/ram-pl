#pragma once

enum StatementType { SBASE, SEXPR, SCODE_BLOCK, SLET, SASSIGNMENT, SFOR_LOOP, SMEMBER_DEF, STYPE_DEF, SFUNC_DECL };

class IStatement
{
public:
	const Position _position;
	StatementType _type;

	IStatement(Position _pos, StatementType _type);
	virtual ~IStatement();

	virtual SharedValue Execute(Environment* _env) = 0;
	virtual IStatement* GetCopy() = 0;
};

class ExprStatement : public IStatement
{
public:
	IExpression* expr;

	ExprStatement(IExpression* _expr);
	~ExprStatement();

	SharedValue Execute(Environment* _env);
	IStatement* GetCopy();
};

class CodeBlock : public IStatement
{
public:
	std::vector<IStatement*> statements;
	
	CodeBlock(std::vector<IStatement*>& _statements, Position _pos);
	~CodeBlock();

	SharedValue Execute(Environment* _env);
	IStatement* GetCopy();
	template <typename T> std::vector<T*> GetStatements();
};

template<typename T>
inline std::vector<T*> CodeBlock::GetStatements()
{
	std::vector<T*> result;

	for(auto statement : statements)
	{
		T* cast = dynamic_cast<T*>(statement);

		if(cast)
			result.push_back(cast);
	}

	return result;
}

class LetStatement : public IStatement
{
public:
	std::string identifier;
	IExpression* expr;

	LetStatement(std::string _identifier, IExpression* _expr, Position _pos);
	~LetStatement();

	SharedValue Execute(Environment* _env);
	IStatement* GetCopy();
};

class Assignment : public IStatement
{
public:
	std::string identifier;
	IExpression* expr;
	bool isLetAssignment;

	Assignment(std::string _identifier, IExpression* _expr, bool _isLet, Position _pos);
	~Assignment();

	SharedValue Execute(Environment* _env);
	IStatement* GetCopy();
};

class ForLoop : public IStatement
{
public:
	Assignment* initAssignment;
	IExpression* conditionExpr;
	IStatement* statement;
	Assignment* finallyAssign;

	ForLoop(Assignment* _initAssign, IExpression* _conditionExpr, IStatement* _statement, Assignment* _finAssign, Position _pos);
	~ForLoop();

	SharedValue Execute(Environment* _env);
	IStatement* GetCopy();
};

class MemberDefinition : public IStatement
{
public:
	Definition definition;

	MemberDefinition(std::string _identifier, std::string _typeDef, Position _pos);

	SharedValue Execute(Environment* _env);
	IStatement* GetCopy();
};

class TypeDefinition : public IStatement
{
private:
	std::string identifier;
	DefinitionMap memDefs;

public:
	TypeDefinition(std::string _identifier, DefinitionMap& _memDefs, Position _pos);

	SharedValue Execute(Environment* _env);
	IStatement* GetCopy();
};

class FuncDeclaration : public IStatement
{
private:
	std::string identifier;
	DefinitionList argDefs;
	std::string retTypeName;
	std::shared_ptr<IStatement> body;

public:
	FuncDeclaration(std::string _identifier, DefinitionList& _argDefs, std::string _retTypeName, IStatement* _body, Position _pos);
	
	SharedValue Execute(Environment* _env);
	IStatement* GetCopy();
};