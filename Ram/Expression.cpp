#include "pch.h"
#include "Expression.h"
#include "Value.h"
#include "Environment.h"

IExpression::IExpression(Position _pos, ExpressionType _type) : _position(_pos), _type(_type) {}
IExpression::~IExpression() {}

#pragma region ValueExpression
ValueExpression::ValueExpression(IValue* _val) : IExpression(_val->_position, EVALUE) { value.reset(_val); }
SharedValue ValueExpression::Evaluate(Environment* _env) { return value; }
IExpression* ValueExpression::GetCopy() { return new ValueExpression(value->GetCopy()); }
#pragma endregion

#pragma region BinopExpression
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

SharedValue BinopExpression::Evaluate(Environment* _env)
{
	SharedValue eLeft = left->Evaluate(_env), eRight = right->Evaluate(_env);

	if(eLeft->_type == VEXCEPTION) { return eLeft; }
	if(eRight->_type == VEXCEPTION) { return eRight; }

	IValue* result = 0;
	ValueType rightValType = eRight->_type;

	switch(MakeBinop(eLeft.get()->_type, eRight.get()->_type, operation))
	{
		case MakeBinop(VINT, VINT, ADD): result = new IntValue(((IntValue*)eLeft.get())->value + ((IntValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VINT, VINT, SUBTRACT): result = new IntValue(((IntValue*)eLeft.get())->value - ((IntValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VINT, VINT, MULTIPY): result = new IntValue(((IntValue*)eLeft.get())->value * ((IntValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VINT, VINT, DIVIDE): {
			if(((IntValue*)eRight.get())->value != 0) { result = new IntValue(((IntValue*)eLeft.get())->value / ((IntValue*)eRight.get())->value, eLeft.get()->_position); }
			else { result = Exception_DivisionByZero(Trace(eRight.get()->_position, _env->name, "Put FileName Here")); }
		}break;
		case MakeBinop(VINT, VINT, LT): result = new IntValue(((IntValue*)eLeft.get())->value < ((IntValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VINT, VINT, LTEQ): result = new IntValue(((IntValue*)eLeft.get())->value <= ((IntValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VINT, VINT, GT): result = new IntValue(((IntValue*)eLeft.get())->value > ((IntValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VINT, VINT, GTEQ): result = new IntValue(((IntValue*)eLeft.get())->value >= ((IntValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VINT, VFLOAT, ADD): result = new IntValue(((IntValue*)eLeft.get())->value + ((FloatValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VINT, VFLOAT, SUBTRACT): result = new IntValue(((IntValue*)eLeft.get())->value - ((FloatValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VINT, VFLOAT, MULTIPY): result = new IntValue(((IntValue*)eLeft.get())->value * ((FloatValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VINT, VFLOAT, DIVIDE): {
			if(((FloatValue*)eRight.get())->value != 0) { result = new IntValue(((IntValue*)eLeft.get())->value / ((FloatValue*)eRight.get())->value, eLeft.get()->_position); }
			else { result = Exception_DivisionByZero(Trace(eRight.get()->_position, _env->name, "Put FileName Here")); }
		}break;
		case MakeBinop(VINT, VFLOAT, LT): result = new IntValue(((IntValue*)eLeft.get())->value < ((FloatValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VINT, VFLOAT, LTEQ): result = new IntValue(((IntValue*)eLeft.get())->value <= ((FloatValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VINT, VFLOAT, GT): result = new IntValue(((IntValue*)eLeft.get())->value > ((FloatValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VINT, VFLOAT, GTEQ): result = new IntValue(((IntValue*)eLeft.get())->value >= ((FloatValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VINT, VBOOL, ADD): result = new IntValue(((IntValue*)eLeft.get())->value + ((BoolValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VINT, VBOOL, SUBTRACT): result = new IntValue(((IntValue*)eLeft.get())->value - ((BoolValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VINT, VBOOL, MULTIPY): result = new IntValue(((IntValue*)eLeft.get())->value * ((BoolValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VINT, VBOOL, DIVIDE): {
			if(((BoolValue*)eRight.get())->value != 0) { result = new IntValue(((IntValue*)eLeft.get())->value / ((BoolValue*)eRight.get())->value, eLeft.get()->_position); }
			else { result = Exception_DivisionByZero(Trace(eRight.get()->_position, _env->name, "Put FileName Here")); }
		}break;
		case MakeBinop(VINT, VBOOL, LT): result = new IntValue(((IntValue*)eLeft.get())->value < ((BoolValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VINT, VBOOL, LTEQ): result = new IntValue(((IntValue*)eLeft.get())->value <= ((BoolValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VINT, VBOOL, GT): result = new IntValue(((IntValue*)eLeft.get())->value > ((BoolValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VINT, VBOOL, GTEQ): result = new IntValue(((IntValue*)eLeft.get())->value >= ((BoolValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VINT, VSTRING, ADD): result = new StringValue(std::to_string(((IntValue*)eLeft.get())->value) + ((StringValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VINT, VSTRING, MULTIPY): {
			int count = ((IntValue*)eLeft.get())->value;
			std::string repeat = ((StringValue*)eRight.get())->value;
			std::string str;

			for(int i = 0; i < count; i++)
				str += repeat;

			result = new StringValue(str, eLeft.get()->_position);
		}break;

		case MakeBinop(VFLOAT, VINT, ADD): result = new FloatValue(((FloatValue*)eLeft.get())->value + ((IntValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VFLOAT, VINT, SUBTRACT): result = new FloatValue(((FloatValue*)eLeft.get())->value - ((IntValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VFLOAT, VINT, MULTIPY): result = new FloatValue(((FloatValue*)eLeft.get())->value * ((IntValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VFLOAT, VINT, DIVIDE): {
			if(((IntValue*)eRight.get())->value != 0) { result = new FloatValue(((FloatValue*)eLeft.get())->value / ((FloatValue*)eRight.get())->value, eLeft.get()->_position); }
			else { result = Exception_DivisionByZero(Trace(eRight.get()->_position, _env->name, "Put FileName Here")); }
		}break;
		case MakeBinop(VFLOAT, VINT, LT): result = new FloatValue(((FloatValue*)eLeft.get())->value < ((IntValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VFLOAT, VINT, LTEQ): result = new IntValue(((FloatValue*)eLeft.get())->value <= ((IntValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VFLOAT, VINT, GT): result = new FloatValue(((FloatValue*)eLeft.get())->value > ((IntValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VFLOAT, VINT, GTEQ): result = new FloatValue(((FloatValue*)eLeft.get())->value >= ((IntValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VFLOAT, VFLOAT, ADD): result = new FloatValue(((FloatValue*)eLeft.get())->value + ((FloatValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VFLOAT, VFLOAT, SUBTRACT): result = new FloatValue(((FloatValue*)eLeft.get())->value - ((FloatValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VFLOAT, VFLOAT, MULTIPY): result = new FloatValue(((FloatValue*)eLeft.get())->value * ((FloatValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VFLOAT, VFLOAT, DIVIDE): {
			if(((FloatValue*)eRight.get())->value != 0) { result = new FloatValue(((FloatValue*)eLeft.get())->value / ((FloatValue*)eRight.get())->value, eLeft.get()->_position); }
			else { result = Exception_DivisionByZero(Trace(eRight.get()->_position, _env->name, "Put FileName Here")); }
		}break;
		case MakeBinop(VFLOAT, VFLOAT, LT): result = new FloatValue(((FloatValue*)eLeft.get())->value < ((FloatValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VFLOAT, VFLOAT, LTEQ): result = new FloatValue(((FloatValue*)eLeft.get())->value <= ((FloatValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VFLOAT, VFLOAT, GT): result = new FloatValue(((FloatValue*)eLeft.get())->value > ((FloatValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VFLOAT, VFLOAT, GTEQ): result = new FloatValue(((FloatValue*)eLeft.get())->value >= ((FloatValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VFLOAT, VBOOL, ADD): result = new FloatValue(((FloatValue*)eLeft.get())->value + ((BoolValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VFLOAT, VBOOL, SUBTRACT): result = new FloatValue(((FloatValue*)eLeft.get())->value - ((BoolValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VFLOAT, VBOOL, MULTIPY): result = new FloatValue(((FloatValue*)eLeft.get())->value * ((BoolValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VFLOAT, VBOOL, DIVIDE): {
			if(((BoolValue*)eRight.get())->value != 0) { result = new FloatValue(((FloatValue*)eLeft.get())->value / ((BoolValue*)eRight.get())->value, eLeft.get()->_position); }
			else { result = Exception_DivisionByZero(Trace(eRight.get()->_position, _env->name, "Put FileName Here")); }
		}break;
		case MakeBinop(VFLOAT, VBOOL, LT): result = new FloatValue(((FloatValue*)eLeft.get())->value < ((BoolValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VFLOAT, VBOOL, LTEQ): result = new FloatValue(((FloatValue*)eLeft.get())->value <= ((BoolValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VFLOAT, VBOOL, GT): result = new FloatValue(((FloatValue*)eLeft.get())->value > ((BoolValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VFLOAT, VBOOL, GTEQ): result = new FloatValue(((FloatValue*)eLeft.get())->value >= ((BoolValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VFLOAT, VSTRING, ADD): result = new StringValue(std::to_string(((FloatValue*)eLeft.get())->value) + ((StringValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VFLOAT, VSTRING, MULTIPY): {
			float count = ((FloatValue*)eLeft.get())->value;
			std::string repeat = ((StringValue*)eRight.get())->value;
			std::string str;

			for(int i = 0; i < count; i++)
				str += repeat;

			if(count - (int)count != 0)
				str += str.substr(0, repeat.size() * (count - (int)count));

			result = new StringValue(str, eLeft.get()->_position);
		}break;

		case MakeBinop(VSTRING, VINT, ADD): result = new StringValue(((StringValue*)eLeft.get())->value + std::to_string(((IntValue*)eRight.get())->value), eLeft.get()->_position); break;
		case MakeBinop(VSTRING, VINT, MULTIPY): {
			int count = ((IntValue*)eRight.get())->value;
			std::string repeat = ((StringValue*)eLeft.get())->value;
			std::string str;

			for(int i = 0; i < count; i++)
				str += repeat;

			result = new StringValue(str, eLeft.get()->_position);
		}break;
		case MakeBinop(VSTRING, VFLOAT, ADD): result = new StringValue(((StringValue*)eLeft.get())->value + std::to_string(((FloatValue*)eRight.get())->value), eLeft.get()->_position); break;
		case MakeBinop(VSTRING, VFLOAT, MULTIPY): {
			float count = ((FloatValue*)eRight.get())->value;
			std::string repeat = ((StringValue*)eLeft.get())->value;
			std::string str;

			for(int i = 0; i < count; i++)
				str += repeat;

			if(count - (int)count != 0)
				str += str.substr(0, repeat.size() * (count - (int)count));

			result = new StringValue(str, eLeft.get()->_position);
		}break;
		case MakeBinop(VSTRING, VSTRING, ADD): result = new StringValue(((StringValue*)eLeft.get())->value + ((StringValue*)eRight.get())->value, eLeft.get()->_position); break;

		case MakeBinop(VBOOL, VINT, ADD): result = new IntValue(((BoolValue*)eLeft.get())->value + ((IntValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VBOOL, VINT, SUBTRACT): result = new IntValue(((BoolValue*)eLeft.get())->value - ((IntValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VBOOL, VINT, MULTIPY): result = new IntValue(((BoolValue*)eLeft.get())->value * ((IntValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VBOOL, VINT, DIVIDE): {
			if(((IntValue*)eRight.get())->value != 0) { result = new IntValue(((BoolValue*)eLeft.get())->value / ((BoolValue*)eRight.get())->value, eLeft.get()->_position); }
			else { result = Exception_DivisionByZero(Trace(eRight.get()->_position, _env->name, "Put FileName Here")); }
		}break;
		case MakeBinop(VBOOL, VINT, LT): result = new BoolValue(((BoolValue*)eLeft.get())->value < ((IntValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VBOOL, VINT, LTEQ): result = new BoolValue(((BoolValue*)eLeft.get())->value <= ((IntValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VBOOL, VINT, GT): result = new BoolValue(((BoolValue*)eLeft.get())->value > ((IntValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VBOOL, VINT, GTEQ): result = new BoolValue(((BoolValue*)eLeft.get())->value >= ((IntValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VBOOL, VFLOAT, ADD): result = new FloatValue(((BoolValue*)eLeft.get())->value + ((FloatValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VBOOL, VFLOAT, SUBTRACT): result = new FloatValue(((BoolValue*)eLeft.get())->value - ((FloatValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VBOOL, VFLOAT, MULTIPY): result = new FloatValue(((BoolValue*)eLeft.get())->value * ((FloatValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VBOOL, VFLOAT, DIVIDE): {
			if(((FloatValue*)eRight.get())->value != 0) { result = new FloatValue(((BoolValue*)eLeft.get())->value / ((FloatValue*)eRight.get())->value, eLeft.get()->_position); }
			else { result = Exception_DivisionByZero(Trace(eRight.get()->_position, _env->name, "Put FileName Here")); }
		}break;
		case MakeBinop(VBOOL, VFLOAT, LT): result = new BoolValue(((BoolValue*)eLeft.get())->value < ((FloatValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VBOOL, VFLOAT, LTEQ): result = new BoolValue(((BoolValue*)eLeft.get())->value <= ((FloatValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VBOOL, VFLOAT, GT): result = new BoolValue(((BoolValue*)eLeft.get())->value > ((FloatValue*)eRight.get())->value, eLeft.get()->_position); break;
		case MakeBinop(VBOOL, VFLOAT, GTEQ): result = new BoolValue(((BoolValue*)eLeft.get())->value >= ((FloatValue*)eRight.get())->value, eLeft.get()->_position); break;

		default: result = Exception_InvalidOperation("Cannot perform operation on these types.", Trace(_position, _env->name, "Put FileName Here"));
	}

	return SharedValue(result);
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
#pragma endregion

#pragma region NegationExpression
NegationExpression::NegationExpression(IExpression* _factor, Position _pos) : IExpression(_pos, ENEGATION) { factor = _factor; }
NegationExpression::~NegationExpression() { delete factor; }

SharedValue NegationExpression::Evaluate(Environment* _env)
{
	SharedValue val = factor->Evaluate(_env);
	ValueType type = val->_type;

	if(type == VEXCEPTION) { return val; }
	else if(type == VINT) { return SharedValue(new IntValue(-((IntValue*)val.get())->value, _position)); }
	else if(type == VFLOAT) { return SharedValue(new FloatValue(-((FloatValue*)val.get())->value, _position)); }
	else { return SharedValue(Exception_InvalidOperation("Cannot perform arithmetic negation on this type.", Trace(_position, _env->name, "Put FileName Here"))); }
}

IExpression* NegationExpression::GetCopy() { return new NegationExpression(factor->GetCopy(), _position); }
#pragma endregion

#pragma region CastExpression
CastExpression::CastExpression(TypeName _typeName, IExpression* _factor, Position _pos)
	: IExpression(_pos, ECAST)
{
	typeName = _typeName;
	factor = _factor;
}

CastExpression::~CastExpression() { delete factor; }

SharedValue CastExpression::Evaluate(Environment* _env)
{
	SharedValue val = factor->Evaluate(_env);

	if(val->_type == VEXCEPTION) { return val; }
	else
	{
		Environment* castingEnv = val->GetIntrinsicEnv();
		std::string castFuncName = "__" + typeName + "__";
		std::string castFuncSig = "func:(void)->" + typeName;

		std::vector<IExpression*> argExprs({});
		SharedValue func = castingEnv->GetValue(castFuncName, _position, false);
		Trace trace = Trace(_position, _env->name, "Put FileName Here");

		if(func->_type == VEXCEPTION) { return func; }
		else if(func->_type != VFUNC) { return SharedValue(Exception_MismatchType(castFuncSig, func->GetTypeSig(), trace)); }
		else
		{
			SharedValue retVal = ((FuncValue*)func.get())->Call(_env, argExprs, _position);

			if(retVal->_type == VEXCEPTION)
				((ExceptionValue*)retVal.get())->ExtendStackTrace(trace);

			return retVal;
		}
	}
}

IExpression* CastExpression::GetCopy() { return new CastExpression(typeName, factor->GetCopy(), _position); }
#pragma endregion

#pragma region SymbolExpression
SymbolExpression::SymbolExpression(std::string _symbol, Position _pos) : IExpression(_pos, ESYMBOL) { symbol = _symbol; }
SharedValue SymbolExpression::Evaluate(Environment* _env) { return _env->GetValue(symbol, _position, true); }
IExpression* SymbolExpression::GetCopy() { return new SymbolExpression(symbol, _position); }
#pragma endregion

#pragma region MemberedExpression
MemberedExpression::MemberedExpression(std::map<std::string, IExpression*>& _memAssigns, Position _pos) : IExpression(_pos, EMEMBERED) { memAssigns = _memAssigns; }

MemberedExpression::~MemberedExpression()
{
	for(auto it : memAssigns)
		delete it.second;
}

SharedValue MemberedExpression::Evaluate(Environment* _env)
{
	Environment* intrinsicEnv = new Environment(_env, "<MEMBERED>");
	DefinitionMap memberDefs;
	TypeSig typeSig = "<MEMBERED>{";

	for(auto it : memAssigns)
	{
		// When the exprs are evaluated, they don't have access to the 
		// members being created along side them that's why I use
		// _creationEnv instead of membersEnv;
		SharedValue val = it.second->Evaluate(_env);

		if(val->_type == VEXCEPTION)
		{
			delete intrinsicEnv;
			return val;
		}

		intrinsicEnv->AddVariable(it.first, val, _position);

		TypeSig valTypeSig = val->GetTypeSig();

		memberDefs.insert_or_assign(it.first, valTypeSig);
		typeSig += it.first + ":" + valTypeSig + "; ";
	}

	typeSig.pop_back();
	typeSig.pop_back();
	typeSig += "}";

	return SharedValue(new MemberedValue(memberDefs, typeSig, intrinsicEnv, _position));
}

IExpression* MemberedExpression::GetCopy()
{
	std::map<std::string, IExpression*> copy_MemAssigns;

	for(auto it : memAssigns)
		copy_MemAssigns.insert_or_assign(it.first, it.second->GetCopy());

	return new MemberedExpression(copy_MemAssigns, _position);
}
#pragma endregion

#pragma region AccessExpression
AccessExpression::AccessExpression(IExpression* _base, std::string _memId, Position _pos)
	: IExpression(_pos, EACCESS)
{
	base = _base;
	memberIdentifer = _memId;
}

AccessExpression::~AccessExpression() { delete base; }

SharedValue AccessExpression::Evaluate(Environment* _env)
{
	SharedValue baseVal = base->Evaluate(_env);

	if(baseVal->_type == VEXCEPTION) { return baseVal; }
	else { return ((MemberedValue*)baseVal.get())->GetIntrinsicEnv()->GetValue(memberIdentifer, _position, false); }
}

IExpression* AccessExpression::GetCopy() { return new AccessExpression(base->GetCopy(), memberIdentifer, _position); }
#pragma endregion

#pragma region FuncCallExpression
FuncCallExpression::FuncCallExpression(IExpression* _base, ArgumentList& _argExprs, Position _pos)
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

SharedValue FuncCallExpression::Evaluate(Environment* _env)
{
	SharedValue baseVal = base->Evaluate(_env);
	ValueType baseValType = baseVal->_type;
	Trace trace = Trace(_position, _env->name, "Put FileName Here");

	if(baseValType == VFUNC)
	{ 
		SharedValue retVal = ((FuncValue*)baseVal.get())->Call(_env, argExprs, base->_position);
	
		if(retVal->_type == VEXCEPTION)
			((ExceptionValue*)retVal.get())->ExtendStackTrace(trace);

		return retVal;
	}
	else if(baseValType == VEXCEPTION) { return baseVal; }
	else { return SharedValue(Exception_InvalidOperation("Value not callable!", trace)); }
}

IExpression* FuncCallExpression::GetCopy()
{
	ArgumentList copy_argExprs;

	for(auto it : argExprs)
		copy_argExprs.push_back(it->GetCopy());

	return new FuncCallExpression(base->GetCopy(), copy_argExprs, _position);
}
#pragma endregion