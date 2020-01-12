#pragma once

class IExpression
{
public:
	enum ExpressionType { EVALUE, EBINOP, EUNOP, ECAST, ESYMBOL, EMEMBERED, EACCESS, EFUNC_CALL };

	Position _position;
	ExpressionType _type;

	IExpression(Position _pos, ExpressionType _type);
	virtual ~IExpression();

	virtual SharedValue Evaluate(Environment* _env) = 0;
	virtual IExpression* GetCopy() = 0;
};

class ValueExpression : public IExpression
{
private:
	SharedValue value;

public:
	ValueExpression(IValue* _val);

	SharedValue Evaluate(Environment* _env);
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

	SharedValue Evaluate(Environment* _env);
	IExpression* GetCopy();
	
	static OP TokenTypeToOp(TokenType _type);
};

inline constexpr int MakeBinop(char _left, char _right, char _op) { return (((int)_left) << 16) | (((int)_right) << 8) | _op; }

class UnopExpression : public IExpression
{
public:
	enum OP : char { INC, DEC, NEG, NOT, BIN_NOT };

private:
	IExpression* expr;
	OP operation;

public:
	UnopExpression(IExpression* _expr, OP _op, Position _pos);
	~UnopExpression();

	SharedValue Evaluate(Environment* _env);
	IExpression* GetCopy();

	static OP TokenTypeToOp(TokenType _type);
};

inline constexpr short int MakeUnop(char _factor, char _op) { return (((short int)_factor) << 8) | _op; }
inline constexpr bool IsUnop(TokenType _type) { return _type == TT_NOT || _type == TT_MINUS || _type == TT_BIN_NOT; }

class CastExpression : public IExpression
{
private:
	TypeName typeName;
	IExpression* factor;

public:
	CastExpression(TypeName _typeName, IExpression* _factor, Position _pos);
	~CastExpression();

	SharedValue Evaluate(Environment* _env);
	IExpression* GetCopy();
};

class SymbolExpression : public IExpression
{
public:
	std::string symbol;

	SymbolExpression(std::string _symbol, Position _pos);

	SharedValue Evaluate(Environment* _env);
	IExpression* GetCopy();
};

class MemberedExpression : public IExpression
{
private:
	std::map<std::string, IExpression*> memAssigns;

public:
	MemberedExpression(std::map<std::string, IExpression*>& _memAssigns, Position _pos);
	~MemberedExpression();

	SharedValue Evaluate(Environment* _env);
	IExpression* GetCopy();
};

class ArrayExpression : public IExpression
{
private:
	std::vector<IExpression*> elemExprs;

public:
	ArrayExpression(std::vector<IExpression*>& _elemExpr, Position _pos);
	~ArrayExpression();

	SharedValue Evaluate(Environment* _env);
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

	SharedValue Evaluate(Environment* _env);
	IExpression* GetCopy();
};

class FuncCallExpression : public IExpression
{
private:
	IExpression* base;
	ArgumentList argExprs;

public:
	FuncCallExpression(IExpression* _base, ArgumentList& _argExprs, Position _pos);
	~FuncCallExpression();

	SharedValue Evaluate(Environment* _env);
	IExpression* GetCopy();
};

class ArrayIndexExpression : public IExpression
{
private:
	IExpression* base;
	IExpression* indexExpr;

public:
	ArrayIndexExpression(IExpression* _base, IExpression* _indexExpr, Position _pos);
	~ArrayIndexExpression();

	SharedValue Evaluate(Environment* _env);
	IExpression* GetCopy();
};