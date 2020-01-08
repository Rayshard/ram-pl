#pragma once

#include "..\Environment.h"
#include "Value.h"

std::string TypeDefFromMemDefs(std::map<std::string, std::string> _memDefs);

class IExpression
{
public:
	enum ExpressionType { EVALUE, EBINOP, ENEGATION, ECAST, ESYMBOL, EMEMBERED, EACCESS, EFUNC_CALL };

	Position _position;
	ExpressionType _type;

	IExpression(ExpressionType _type);
	IExpression(Position _pos, ExpressionType _type);
	virtual ~IExpression();

	virtual IValue* Evaluate(Environment* _env) = 0;
	virtual IExpression* GetCopy() = 0;
};

class ValueExpression : public IExpression
{
private:
	IValue* value;

public:
	ValueExpression(IValue* _val);
	~ValueExpression();

	IValue* Evaluate(Environment* _env);
	IExpression* GetCopy();
};

class BinopExpression : public IExpression
{
public:
	enum OP : char { ADD, SUBTRACT, MULTIPY, DIVIDE, LT, LTEQ, GT, GTEQ };

private:
	IExpression* left;
	IExpression* right;
	OP operation;

public:
	BinopExpression(IExpression* _left, IExpression* _right, OP _op, Position _pos);
	~BinopExpression();

	IValue* Evaluate(Environment* _env);
	IExpression* GetCopy();
	
	static OP TokenTypeToOp(TokenType _type);
};

inline constexpr int MakeBinop(char _left, char _right, char _op) { return (_left << 16) | (_right << 8) | _op; }

class NegationExpression : public IExpression
{
private:
	IExpression* factor;

public:
	NegationExpression(IExpression* _factor, Position _pos);
	~NegationExpression();

	IValue* Evaluate(Environment* _env);
	IExpression* GetCopy();
};

class CastExpression : public IExpression
{
private:
	std::string typeName;
	IExpression* factor;

public:
	CastExpression(std::string _typeName, IExpression* _factor, Position _pos);
	~CastExpression();

	IValue* Evaluate(Environment* _env);
	IExpression* GetCopy();
};

class SymbolExpression : public IExpression
{
public:
	std::string symbol;

	SymbolExpression(std::string _symbol, Position _pos);

	IValue* Evaluate(Environment* _env);
	IExpression* GetCopy();
};

class MemberedExpression : public IExpression
{
private:
	std::map<std::string, IExpression*> memAssigns;

public:
	MemberedExpression(std::map<std::string, IExpression*>& _memAssigns, Position _pos);
	~MemberedExpression();

	IValue* Evaluate(Environment* _env);
	IExpression* GetCopy();
};

class AccessExpression : public IExpression
{
private:
	IExpression* base;
	std::string memberIdentifer;

public:
	AccessExpression(IExpression* _base, std::string _memId, Position _pos);
	~AccessExpression();

	IValue* Evaluate(Environment* _env);
	IExpression* GetCopy();
};

class FuncCallExpression : public IExpression
{
private:
	IExpression* base;
	std::vector<IExpression*> argExprs;

public:
	FuncCallExpression(IExpression* _base, std::vector<IExpression*>& _argExprs, Position _pos);
	~FuncCallExpression();

	IValue* Evaluate(Environment* _env);
	IExpression* GetCopy();
};