#include "Expression.h"

IExpression::IExpression(Position _pos, ExpressionType _type) : _position(_pos), _type(_type) {}

IExpression::~IExpression()
{
}

ValueExpression::ValueExpression(IValue* _val)
	: IExpression(_val->_position, EVALUE)
{
	value = _val;
}

ValueExpression::~ValueExpression()
{
	delete value;
}

IValue* ValueExpression::Evaluate(Environment* _env) { return value->GetCopy(); }
IExpression* ValueExpression::GetCopy() { return new ValueExpression(value->GetCopy()); }

BinopExpression::BinopExpression(IExpression* _left, IExpression* _right, OP _op, Position _pos)
	: IExpression(_pos, EBINOP)
{
	left = _left;
	right = _right;
	operation = _op;
}

BinopExpression::~BinopExpression()
{
	delete left;
	delete right;
}

IValue* BinopExpression::Evaluate(Environment* _env)
{
	auto leftValU = std::unique_ptr<IValue>(left->Evaluate(_env)), rightValU = std::unique_ptr<IValue>(right->Evaluate(_env));
	IValue* eLeft = leftValU.get(), *eRight = rightValU.get();

	if(eLeft->_type == VEXCEPTION) { return leftValU.release(); }
	if(eRight->_type == VEXCEPTION) { return rightValU.release(); }

	ValueType rightValType = eRight->_type;

	switch(MakeBinop(eLeft->_type, eRight->_type, operation))
	{
		case MakeBinop(VINT, VINT, ADD): return new IntValue(((IntValue*)eLeft)->value + ((IntValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VINT, VINT, SUBTRACT): return new IntValue(((IntValue*)eLeft)->value - ((IntValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VINT, VINT, MULTIPY): return new IntValue(((IntValue*)eLeft)->value * ((IntValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VINT, VINT, DIVIDE): {
			if(((IntValue*)eRight)->value != 0) { return new IntValue(((IntValue*)eLeft)->value / ((IntValue*)eRight)->value, eLeft->_position); }
			else { return Exception_DivisionByZero(eRight->_position); }
		}
		case MakeBinop(VINT, VINT, LT): return new IntValue(((IntValue*)eLeft)->value < ((IntValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VINT, VINT, LTEQ): return new IntValue(((IntValue*)eLeft)->value <= ((IntValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VINT, VINT, GT): return new IntValue(((IntValue*)eLeft)->value > ((IntValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VINT, VINT, GTEQ): return new IntValue(((IntValue*)eLeft)->value >= ((IntValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VINT, VFLOAT, ADD): return new IntValue(((IntValue*)eLeft)->value + ((FloatValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VINT, VFLOAT, SUBTRACT): return new IntValue(((IntValue*)eLeft)->value - ((FloatValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VINT, VFLOAT, MULTIPY): return new IntValue(((IntValue*)eLeft)->value * ((FloatValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VINT, VFLOAT, DIVIDE): {
			if(((FloatValue*)eRight)->value != 0) { return new IntValue(((IntValue*)eLeft)->value / ((FloatValue*)eRight)->value, eLeft->_position); }
			else { return Exception_DivisionByZero(eRight->_position); }
		}
		case MakeBinop(VINT, VFLOAT, LT): return new IntValue(((IntValue*)eLeft)->value < ((FloatValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VINT, VFLOAT, LTEQ): return new IntValue(((IntValue*)eLeft)->value <= ((FloatValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VINT, VFLOAT, GT): return new IntValue(((IntValue*)eLeft)->value > ((FloatValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VINT, VFLOAT, GTEQ): return new IntValue(((IntValue*)eLeft)->value >= ((FloatValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VINT, VBOOL, ADD): return new IntValue(((IntValue*)eLeft)->value + ((BoolValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VINT, VBOOL, SUBTRACT): return new IntValue(((IntValue*)eLeft)->value - ((BoolValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VINT, VBOOL, MULTIPY): return new IntValue(((IntValue*)eLeft)->value * ((BoolValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VINT, VBOOL, DIVIDE): {
			if(((BoolValue*)eRight)->value != 0) { return new IntValue(((IntValue*)eLeft)->value / ((BoolValue*)eRight)->value, eLeft->_position); }
			else { return Exception_DivisionByZero(eRight->_position); }
		}
		case MakeBinop(VINT, VBOOL, LT): return new IntValue(((IntValue*)eLeft)->value < ((BoolValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VINT, VBOOL, LTEQ): return new IntValue(((IntValue*)eLeft)->value <= ((BoolValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VINT, VBOOL, GT): return new IntValue(((IntValue*)eLeft)->value > ((BoolValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VINT, VBOOL, GTEQ): return new IntValue(((IntValue*)eLeft)->value >= ((BoolValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VINT, VSTRING, ADD): return new StringValue(std::to_string(((IntValue*)eLeft)->value) + ((StringValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VINT, VSTRING, MULTIPY): {
			int count = ((IntValue*)eLeft)->value;
			std::string repeat = ((StringValue*)eRight)->value;
			std::string str;

			for(int i = 0; i < count; i++)
				str += repeat;

			return new StringValue(str, eLeft->_position);
		}

		case MakeBinop(VFLOAT, VINT, ADD): return new FloatValue(((FloatValue*)eLeft)->value + ((IntValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VFLOAT, VINT, SUBTRACT): return new FloatValue(((FloatValue*)eLeft)->value - ((IntValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VFLOAT, VINT, MULTIPY): return new FloatValue(((FloatValue*)eLeft)->value * ((IntValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VFLOAT, VINT, DIVIDE): {
			if(((IntValue*)eRight)->value != 0) { return new FloatValue(((FloatValue*)eLeft)->value / ((FloatValue*)eRight)->value, eLeft->_position); }
			else { return Exception_DivisionByZero(eRight->_position); }
		}
		case MakeBinop(VFLOAT, VINT, LT): return new FloatValue(((FloatValue*)eLeft)->value < ((IntValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VFLOAT, VINT, LTEQ): return new IntValue(((FloatValue*)eLeft)->value <= ((IntValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VFLOAT, VINT, GT): return new FloatValue(((FloatValue*)eLeft)->value > ((IntValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VFLOAT, VINT, GTEQ): return new FloatValue(((FloatValue*)eLeft)->value >= ((IntValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VFLOAT, VFLOAT, ADD): return new FloatValue(((FloatValue*)eLeft)->value + ((FloatValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VFLOAT, VFLOAT, SUBTRACT): return new FloatValue(((FloatValue*)eLeft)->value - ((FloatValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VFLOAT, VFLOAT, MULTIPY): return new FloatValue(((FloatValue*)eLeft)->value * ((FloatValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VFLOAT, VFLOAT, DIVIDE): {
			if(((FloatValue*)eRight)->value != 0) { return new FloatValue(((FloatValue*)eLeft)->value / ((FloatValue*)eRight)->value, eLeft->_position); }
			else { return Exception_DivisionByZero(eRight->_position); }
		}
		case MakeBinop(VFLOAT, VFLOAT, LT): return new FloatValue(((FloatValue*)eLeft)->value < ((FloatValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VFLOAT, VFLOAT, LTEQ): return new FloatValue(((FloatValue*)eLeft)->value <= ((FloatValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VFLOAT, VFLOAT, GT): return new FloatValue(((FloatValue*)eLeft)->value > ((FloatValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VFLOAT, VFLOAT, GTEQ): return new FloatValue(((FloatValue*)eLeft)->value >= ((FloatValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VFLOAT, VBOOL, ADD): return new FloatValue(((FloatValue*)eLeft)->value + ((BoolValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VFLOAT, VBOOL, SUBTRACT): return new FloatValue(((FloatValue*)eLeft)->value - ((BoolValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VFLOAT, VBOOL, MULTIPY): return new FloatValue(((FloatValue*)eLeft)->value * ((BoolValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VFLOAT, VBOOL, DIVIDE): {
			if(((BoolValue*)eRight)->value != 0) { return new FloatValue(((FloatValue*)eLeft)->value / ((BoolValue*)eRight)->value, eLeft->_position); }
			else { return Exception_DivisionByZero(eRight->_position); }
		}
		case MakeBinop(VFLOAT, VBOOL, LT): return new FloatValue(((FloatValue*)eLeft)->value < ((BoolValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VFLOAT, VBOOL, LTEQ): return new FloatValue(((FloatValue*)eLeft)->value <= ((BoolValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VFLOAT, VBOOL, GT): return new FloatValue(((FloatValue*)eLeft)->value > ((BoolValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VFLOAT, VBOOL, GTEQ): return new FloatValue(((FloatValue*)eLeft)->value >= ((BoolValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VFLOAT, VSTRING, ADD): return new StringValue(std::to_string(((FloatValue*)eLeft)->value) + ((StringValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VFLOAT, VSTRING, MULTIPY): {
			float count = ((FloatValue*)eLeft)->value;
			std::string repeat = ((StringValue*)eRight)->value;
			std::string str;

			for(int i = 0; i < count; i++)
				str += repeat;

			if(count - (int)count != 0)
				str += str.substr(0, repeat.size() * (count - (int)count));

			return new StringValue(str, eLeft->_position);
		}

		case MakeBinop(VSTRING, VINT, ADD): return new StringValue(((StringValue*)eLeft)->value + std::to_string(((IntValue*)eRight)->value), eLeft->_position);
		case MakeBinop(VSTRING, VINT, MULTIPY): {
			int count = ((IntValue*)eRight)->value;
			std::string repeat = ((StringValue*)eLeft)->value;
			std::string str;

			for(int i = 0; i < count; i++)
				str += repeat;

			return new StringValue(str, eLeft->_position);
		}
		case MakeBinop(VSTRING, VFLOAT, ADD): return new StringValue(((StringValue*)eLeft)->value + std::to_string(((FloatValue*)eRight)->value), eLeft->_position);
		case MakeBinop(VSTRING, VFLOAT, MULTIPY): {
			float count = ((FloatValue*)eRight)->value;
			std::string repeat = ((StringValue*)eLeft)->value;
			std::string str;

			for(int i = 0; i < count; i++)
				str += repeat;

			if(count - (int)count != 0)
				str += str.substr(0, repeat.size() * (count - (int)count));

			return new StringValue(str, eLeft->_position);
		}
		case MakeBinop(VSTRING, VSTRING, ADD): return new StringValue(((StringValue*)eLeft)->value + ((StringValue*)eRight)->value, eLeft->_position);

		case MakeBinop(VBOOL, VINT, ADD): return new IntValue(((BoolValue*)eLeft)->value + ((IntValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VBOOL, VINT, SUBTRACT): return new IntValue(((BoolValue*)eLeft)->value - ((IntValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VBOOL, VINT, MULTIPY): return new IntValue(((BoolValue*)eLeft)->value * ((IntValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VBOOL, VINT, DIVIDE): {
			if(((IntValue*)eRight)->value != 0) { return new IntValue(((BoolValue*)eLeft)->value / ((BoolValue*)eRight)->value, eLeft->_position); }
			else { return Exception_DivisionByZero(eRight->_position); }
		}
		case MakeBinop(VBOOL, VINT, LT): return new BoolValue(((BoolValue*)eLeft)->value < ((IntValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VBOOL, VINT, LTEQ): return new BoolValue(((BoolValue*)eLeft)->value <= ((IntValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VBOOL, VINT, GT): return new BoolValue(((BoolValue*)eLeft)->value > ((IntValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VBOOL, VINT, GTEQ): return new BoolValue(((BoolValue*)eLeft)->value >= ((IntValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VBOOL, VFLOAT, ADD): return new FloatValue(((BoolValue*)eLeft)->value + ((FloatValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VBOOL, VFLOAT, SUBTRACT): return new FloatValue(((BoolValue*)eLeft)->value - ((FloatValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VBOOL, VFLOAT, MULTIPY): return new FloatValue(((BoolValue*)eLeft)->value * ((FloatValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VBOOL, VFLOAT, DIVIDE): {
			if(((FloatValue*)eRight)->value != 0) { return new FloatValue(((BoolValue*)eLeft)->value / ((FloatValue*)eRight)->value, eLeft->_position); }
			else { return Exception_DivisionByZero(eRight->_position); }
		}
		case MakeBinop(VBOOL, VFLOAT, LT): return new BoolValue(((BoolValue*)eLeft)->value < ((FloatValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VBOOL, VFLOAT, LTEQ): return new BoolValue(((BoolValue*)eLeft)->value <= ((FloatValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VBOOL, VFLOAT, GT): return new BoolValue(((BoolValue*)eLeft)->value > ((FloatValue*)eRight)->value, eLeft->_position);
		case MakeBinop(VBOOL, VFLOAT, GTEQ): return new BoolValue(((BoolValue*)eLeft)->value >= ((FloatValue*)eRight)->value, eLeft->_position);

		default: return Exception_InvalidOperation("Cannot perform operation on these types.", eLeft->_position);
	}
}

IExpression* BinopExpression::GetCopy() { return new BinopExpression(left->GetCopy(), right->GetCopy(), operation, _position); }

BinopExpression::OP BinopExpression::TokenTypeToOp(TokenType _type)
{
	switch(_type)
	{
		case TT_PLUS:
			return BinopExpression::ADD;
			break;
		case TT_MINUS:
			return BinopExpression::SUBTRACT;
			break;
		case TT_TIMES:
			return BinopExpression::MULTIPY;
			break;
		case TT_DIVIDE:
			return BinopExpression::DIVIDE;
			break;
		case TT_LT:
			return BinopExpression::LT;
			break;
		case TT_GT:
			return BinopExpression::GT;
			break;
		case TT_LTEQ:
			return BinopExpression::LTEQ;
			break;
		case TT_GTEQ:
			return BinopExpression::GTEQ;
			break;
		default:
			throw std::invalid_argument("Incorrect Token Type");
			break;
	}
}

NegationExpression::NegationExpression(IExpression* _factor, Position _pos)
	: IExpression(_pos, ENEGATION)
{
	factor = _factor;
}

NegationExpression::~NegationExpression()
{
	delete factor;
}

IValue* NegationExpression::Evaluate(Environment* _env)
{
	auto val = std::unique_ptr<IValue>(factor->Evaluate(_env));
	ValueType type = val->_type;

	if(type == VEXCEPTION) { return val.release(); }
	else if(type == VINT) { return new IntValue(-((IntValue*)val.get())->value, _position); }
	else if(type == VFLOAT) { return new FloatValue(-((FloatValue*)val.get())->value, _position); }
	else { return Exception_InvalidOperation("Cannot perform arithmetic negation on this type.", _position); }
}

IExpression* NegationExpression::GetCopy() { return new NegationExpression(factor->GetCopy(), _position); }

CastExpression::CastExpression(std::string _typeName, IExpression* _factor, Position _pos)
	: IExpression(_pos, ECAST)
{
	typeName = _typeName;
	factor = _factor;
}

CastExpression::~CastExpression()
{
	delete factor;
}

IValue* CastExpression::Evaluate(Environment* _env)
{
	auto val = std::unique_ptr<IValue>(factor->Evaluate(_env));

	if(val->_type == VEXCEPTION) { return val.release(); }
	else
	{
		Environment* castingEnv = val->GetIntrinsicEnv();
		std::string castFuncName = "__" + typeName + "__";
		std::string castFuncTypeName = "func:(void)->" + typeName;

		std::vector<IExpression*> argExprs({});
		IValue* func = castingEnv->GetValue(castFuncName, _position, false, false);

		if(func->_type == VEXCEPTION) { return func; }
		else { return ((FuncValue*)func)->Call(_env, argExprs, _position); }
	}
}

IExpression* CastExpression::GetCopy() { return new CastExpression(typeName, factor->GetCopy(), _position); }

SymbolExpression::SymbolExpression(std::string _symbol, Position _pos)
	: IExpression(_pos, ESYMBOL)
{
	symbol = _symbol;
}

IValue* SymbolExpression::Evaluate(Environment* _env)
{
	return _env->GetValue(symbol, _position, true, true);
}

IExpression* SymbolExpression::GetCopy() { return new SymbolExpression(symbol, _position); }

MemberedExpression::MemberedExpression(std::map<std::string, IExpression*>& _memAssigns, Position _pos)
	: IExpression(_pos, EMEMBERED)
{
	memAssigns = _memAssigns;
}

MemberedExpression::~MemberedExpression()
{
	for(auto it : memAssigns)
		delete it.second;
}

IValue* MemberedExpression::Evaluate(Environment* _env) { return MemberedValue::Create(_env, memAssigns, _position); }

IExpression* MemberedExpression::GetCopy()
{
	std::map<std::string, IExpression*> copy_MemAssigns;

	for(auto it : memAssigns)
		copy_MemAssigns.insert_or_assign(it.first, it.second->GetCopy());

	return new MemberedExpression(copy_MemAssigns, _position);
}

AccessExpression::AccessExpression(IExpression* _base, std::string _memId, Position _pos)
	: IExpression(_pos, EACCESS)
{
	base = _base;
	memberIdentifer = _memId;
}

AccessExpression::~AccessExpression() { delete base; }

IValue * AccessExpression::Evaluate(Environment* _env)
{
	auto baseVal = std::unique_ptr<IValue>(base->Evaluate(_env));

	if(baseVal->_type == VEXCEPTION) { return baseVal.release(); }
	else if(baseVal->_type == VMEMBERED) { return ((MemberedValue*)baseVal.get())->GetMemberValue(memberIdentifer, _position, true); }
	else if(baseVal->_type == VNAMEDSPACE) { return ((NamedspaceValue*)baseVal.release())->environment->GetValue(memberIdentifer, _position, true, false); }
	else { return Exception_InvalidOperation("Symbol cannot be accessed.", _position); }
}

IExpression* AccessExpression::GetCopy() { return new AccessExpression(base->GetCopy(), memberIdentifer, _position); }

FuncCallExpression::FuncCallExpression(IExpression* _base, std::vector<IExpression*>& _argExprs, Position _pos)
	: IExpression(_pos, EFUNC_CALL)
{
	base = _base;
	argExprs = _argExprs;
}

FuncCallExpression::~FuncCallExpression()
{
	for(auto it : argExprs)
		delete it;

	delete base;
}

IValue * FuncCallExpression::Evaluate(Environment* _env)
{
	auto baseVal = std::unique_ptr<IValue>(base->Evaluate(_env));
	ValueType baseValType = baseVal->_type;

	if(baseValType == VFUNC) { return ((FuncValue*)baseVal.release())->Call(_env, argExprs, base->_position); }
	else if(baseValType == VEXCEPTION) { return baseVal.release(); }
	else { return Exception_InvalidOperation("Value not callable!", _position); }
}

IExpression* FuncCallExpression::GetCopy()
{
	std::vector<IExpression*> copy_argExprs;

	for(auto it : argExprs)
		copy_argExprs.push_back(it->GetCopy());

	return new FuncCallExpression(base->GetCopy(), copy_argExprs, _position);
}
