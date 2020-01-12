#include "pch.h"
#include "Expression.h"
#include "Value.h"
#include "Environment.h"

IExpression::IExpression(Position _pos, ExpressionType _type) : _position(_pos), _type(_type) {}
IExpression::~IExpression() {}

#pragma region ValueExpression
ValueExpression::ValueExpression(IValue* _val) : IExpression(_val->position, EVALUE) { value = SHARE(_val); }
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

	if(eLeft->GetType() == VEXCEPTION) { return eLeft; }
	if(eRight->GetType() == VEXCEPTION) { return eRight; }

	IValue* result = 0;
	ValueType rightValType = eRight->GetType();

	switch(MakeBinop(eLeft->GetType(), eRight->GetType(), operation))
	{
		case MakeBinop(VINT, VINT, ADD): result = new IntValue(eLeft->AsInt()->value + eRight->AsInt()->value, eLeft->GetPosition()); break;
		case MakeBinop(VINT, VINT, SUBTRACT): result = new IntValue(eLeft->AsInt()->value - eRight->AsInt()->value, eLeft->GetPosition()); break;
		case MakeBinop(VINT, VINT, MULTIPY): result = new IntValue(eLeft->AsInt()->value * eRight->AsInt()->value, eLeft->GetPosition()); break;
		case MakeBinop(VINT, VINT, DIVIDE): {
			if(eRight->AsInt()->value != 0) { result = new IntValue(eLeft->AsInt()->value / eRight->AsInt()->value, eLeft->GetPosition()); }
			else { result = Exception_DivisionByZero(Trace(eRight->GetPosition(), _env->name, _env->filePath)); }
		}break;
		case MakeBinop(VINT, VINT, LT): result = new BoolValue(eLeft->AsInt()->value < eRight->AsInt()->value, eLeft->GetPosition()); break;
		case MakeBinop(VINT, VINT, LTEQ): result = new BoolValue(eLeft->AsInt()->value <= eRight->AsInt()->value, eLeft->GetPosition()); break;
		case MakeBinop(VINT, VINT, GT): result = new BoolValue(eLeft->AsInt()->value > eRight->AsInt()->value, eLeft->GetPosition()); break;
		case MakeBinop(VINT, VINT, GTEQ): result = new BoolValue(eLeft->AsInt()->value >= eRight->AsInt()->value, eLeft->GetPosition()); break;
		case MakeBinop(VINT, VFLOAT, ADD): result = new IntValue(eLeft->AsInt()->value + eRight->AsFloat()->value, eLeft->GetPosition()); break;
		case MakeBinop(VINT, VFLOAT, SUBTRACT): result = new IntValue(eLeft->AsInt()->value - eRight->AsFloat()->value, eLeft->GetPosition()); break;
		case MakeBinop(VINT, VFLOAT, MULTIPY): result = new IntValue(eLeft->AsInt()->value * eRight->AsFloat()->value, eLeft->GetPosition()); break;
		case MakeBinop(VINT, VFLOAT, DIVIDE): {
			if(eRight->AsFloat()->value != 0) { result = new IntValue(eLeft->AsInt()->value / eRight->AsFloat()->value, eLeft->GetPosition()); }
			else { result = Exception_DivisionByZero(Trace(eRight->GetPosition(), _env->name, _env->filePath)); }
		}break;
		case MakeBinop(VINT, VFLOAT, LT): result = new BoolValue(eLeft->AsInt()->value < eRight->AsFloat()->value, eLeft->GetPosition()); break;
		case MakeBinop(VINT, VFLOAT, LTEQ): result = new BoolValue(eLeft->AsInt()->value <= eRight->AsFloat()->value, eLeft->GetPosition()); break;
		case MakeBinop(VINT, VFLOAT, GT): result = new BoolValue(eLeft->AsInt()->value > eRight->AsFloat()->value, eLeft->GetPosition()); break;
		case MakeBinop(VINT, VFLOAT, GTEQ): result = new BoolValue(eLeft->AsInt()->value >= eRight->AsFloat()->value, eLeft->GetPosition()); break;
		case MakeBinop(VINT, VBOOL, ADD): result = new IntValue(eLeft->AsInt()->value + (int)eRight->AsBool()->value, eLeft->GetPosition()); break;
		case MakeBinop(VINT, VBOOL, SUBTRACT): result = new IntValue(eLeft->AsInt()->value - (int)eRight->AsBool()->value, eLeft->GetPosition()); break;
		case MakeBinop(VINT, VBOOL, MULTIPY): result = new IntValue(eLeft->AsInt()->value * (int)eRight->AsBool()->value, eLeft->GetPosition()); break;
		case MakeBinop(VINT, VBOOL, DIVIDE): {
			if(eRight->AsBool()->value) { result = new IntValue(eLeft->AsInt()->value / (int)eRight->AsBool()->value, eLeft->GetPosition()); }
			else { result = Exception_DivisionByZero(Trace(eRight->GetPosition(), _env->name, _env->filePath)); }
		}break;
		case MakeBinop(VINT, VBOOL, LT): result = new BoolValue(eLeft->AsInt()->value < (int)eRight->AsBool()->value, eLeft->GetPosition()); break;
		case MakeBinop(VINT, VBOOL, LTEQ): result = new BoolValue(eLeft->AsInt()->value <= (int)eRight->AsBool()->value, eLeft->GetPosition()); break;
		case MakeBinop(VINT, VBOOL, GT): result = new BoolValue(eLeft->AsInt()->value > (int)eRight->AsBool()->value, eLeft->GetPosition()); break;
		case MakeBinop(VINT, VBOOL, GTEQ): result = new BoolValue(eLeft->AsInt()->value >= (int)eRight->AsBool()->value, eLeft->GetPosition()); break;
		case MakeBinop(VINT, VSTRING, ADD): result = new StringValue(std::to_string(eLeft->AsInt()->value) + eRight->AsString()->value, eLeft->GetPosition()); break;
		case MakeBinop(VINT, VSTRING, MULTIPY): {
			int count = eLeft->AsInt()->value;
			std::string repeat = eRight->AsString()->value;
			std::string str;

			for(int i = 0; i < count; i++)
				str += repeat;

			result = new StringValue(str, eLeft->GetPosition());
		}break;

		case MakeBinop(VFLOAT, VINT, ADD): result = new FloatValue(eLeft->AsFloat()->value + eRight->AsInt()->value, eLeft->GetPosition()); break;
		case MakeBinop(VFLOAT, VINT, SUBTRACT): result = new FloatValue(eLeft->AsFloat()->value - eRight->AsInt()->value, eLeft->GetPosition()); break;
		case MakeBinop(VFLOAT, VINT, MULTIPY): result = new FloatValue(eLeft->AsFloat()->value * eRight->AsInt()->value, eLeft->GetPosition()); break;
		case MakeBinop(VFLOAT, VINT, DIVIDE): {
			if(eRight->AsInt()->value != 0) { result = new FloatValue(eLeft->AsFloat()->value / eRight->AsFloat()->value, eLeft->GetPosition()); }
			else { result = Exception_DivisionByZero(Trace(eRight->GetPosition(), _env->name, _env->filePath)); }
		}break;
		case MakeBinop(VFLOAT, VINT, LT): result = new BoolValue(eLeft->AsFloat()->value < eRight->AsInt()->value, eLeft->GetPosition()); break;
		case MakeBinop(VFLOAT, VINT, LTEQ): result = new BoolValue(eLeft->AsFloat()->value <= eRight->AsInt()->value, eLeft->GetPosition()); break;
		case MakeBinop(VFLOAT, VINT, GT): result = new BoolValue(eLeft->AsFloat()->value > eRight->AsInt()->value, eLeft->GetPosition()); break;
		case MakeBinop(VFLOAT, VINT, GTEQ): result = new BoolValue(eLeft->AsFloat()->value >= eRight->AsInt()->value, eLeft->GetPosition()); break;
		case MakeBinop(VFLOAT, VFLOAT, ADD): result = new FloatValue(eLeft->AsFloat()->value + eRight->AsFloat()->value, eLeft->GetPosition()); break;
		case MakeBinop(VFLOAT, VFLOAT, SUBTRACT): result = new FloatValue(eLeft->AsFloat()->value - eRight->AsFloat()->value, eLeft->GetPosition()); break;
		case MakeBinop(VFLOAT, VFLOAT, MULTIPY): result = new FloatValue(eLeft->AsFloat()->value * eRight->AsFloat()->value, eLeft->GetPosition()); break;
		case MakeBinop(VFLOAT, VFLOAT, DIVIDE): {
			if(eRight->AsFloat()->value != 0) { result = new FloatValue(eLeft->AsFloat()->value / eRight->AsFloat()->value, eLeft->GetPosition()); }
			else { result = Exception_DivisionByZero(Trace(eRight->GetPosition(), _env->name, _env->filePath)); }
		}break;
		case MakeBinop(VFLOAT, VFLOAT, LT): result = new BoolValue(eLeft->AsFloat()->value < eRight->AsFloat()->value, eLeft->GetPosition()); break;
		case MakeBinop(VFLOAT, VFLOAT, LTEQ): result = new BoolValue(eLeft->AsFloat()->value <= eRight->AsFloat()->value, eLeft->GetPosition()); break;
		case MakeBinop(VFLOAT, VFLOAT, GT): result = new BoolValue(eLeft->AsFloat()->value > eRight->AsFloat()->value, eLeft->GetPosition()); break;
		case MakeBinop(VFLOAT, VFLOAT, GTEQ): result = new BoolValue(eLeft->AsFloat()->value >= eRight->AsFloat()->value, eLeft->GetPosition()); break;
		case MakeBinop(VFLOAT, VBOOL, ADD): result = new FloatValue(eLeft->AsFloat()->value + eRight->AsBool()->value, eLeft->GetPosition()); break;
		case MakeBinop(VFLOAT, VBOOL, SUBTRACT): result = new FloatValue(eLeft->AsFloat()->value - eRight->AsBool()->value, eLeft->GetPosition()); break;
		case MakeBinop(VFLOAT, VBOOL, MULTIPY): result = new FloatValue(eLeft->AsFloat()->value * eRight->AsBool()->value, eLeft->GetPosition()); break;
		case MakeBinop(VFLOAT, VBOOL, DIVIDE): {
			if(eRight->AsBool()->value) { result = new FloatValue(eLeft->AsFloat()->value / (float)eRight->AsBool()->value, eLeft->GetPosition()); }
			else { result = Exception_DivisionByZero(Trace(eRight->GetPosition(), _env->name, _env->filePath)); }
		}break;
		case MakeBinop(VFLOAT, VBOOL, LT): result = new BoolValue(eLeft->AsFloat()->value < (float)eRight->AsBool()->value, eLeft->GetPosition()); break;
		case MakeBinop(VFLOAT, VBOOL, LTEQ): result = new BoolValue(eLeft->AsFloat()->value <= (float)eRight->AsBool()->value, eLeft->GetPosition()); break;
		case MakeBinop(VFLOAT, VBOOL, GT): result = new BoolValue(eLeft->AsFloat()->value > (float)eRight->AsBool()->value, eLeft->GetPosition()); break;
		case MakeBinop(VFLOAT, VBOOL, GTEQ): result = new BoolValue(eLeft->AsFloat()->value >= (float)eRight->AsBool()->value, eLeft->GetPosition()); break;
		case MakeBinop(VFLOAT, VSTRING, ADD): result = new StringValue(std::to_string(eLeft->AsFloat()->value) + eRight->AsString()->value, eLeft->GetPosition()); break;
		case MakeBinop(VFLOAT, VSTRING, MULTIPY): {
			float count = eLeft->AsFloat()->value;
			std::string repeat = eRight->AsString()->value;
			std::string str;

			for(int i = 0; i < count; i++)
				str += repeat;

			if(count - (int)count != 0)
				str += str.substr(0, repeat.size() * (count - (int)count));

			result = new StringValue(str, eLeft->GetPosition());
		}break;

		case MakeBinop(VSTRING, VINT, ADD): result = new StringValue(eLeft->AsString()->value + std::to_string(eRight->AsInt()->value), eLeft->GetPosition()); break;
		case MakeBinop(VSTRING, VINT, MULTIPY): {
			int count = eRight->AsInt()->value;
			std::string repeat = eLeft->AsString()->value;
			std::string str;

			for(int i = 0; i < count; i++)
				str += repeat;

			result = new StringValue(str, eLeft->GetPosition());
		}break;
		case MakeBinop(VSTRING, VFLOAT, ADD): result = new StringValue(eLeft->AsString()->value + std::to_string(eRight->AsFloat()->value), eLeft->GetPosition()); break;
		case MakeBinop(VSTRING, VFLOAT, MULTIPY): {
			float count = eRight->AsFloat()->value;
			std::string repeat = eLeft->AsString()->value;
			std::string str;

			for(int i = 0; i < count; i++)
				str += repeat;

			if(count - (int)count != 0)
				str += str.substr(0, repeat.size() * (count - (int)count));

			result = new StringValue(str, eLeft->GetPosition());
		}break;
		case MakeBinop(VSTRING, VSTRING, ADD): result = new StringValue(eLeft->AsString()->value + eRight->AsString()->value, eLeft->GetPosition()); break;

		case MakeBinop(VBOOL, VINT, ADD): result = new IntValue((int)eLeft->AsBool()->value + eRight->AsInt()->value, eLeft->GetPosition()); break;
		case MakeBinop(VBOOL, VINT, SUBTRACT): result = new IntValue((int)eLeft->AsBool()->value - eRight->AsInt()->value, eLeft->GetPosition()); break;
		case MakeBinop(VBOOL, VINT, MULTIPY): result = new IntValue((int)eLeft->AsBool()->value * eRight->AsInt()->value, eLeft->GetPosition()); break;
		case MakeBinop(VBOOL, VINT, DIVIDE): {
			if(eRight->AsInt()->value != 0) { result = new IntValue((int)eLeft->AsBool()->value / eRight->AsInt()->value, eLeft->GetPosition()); }
			else { result = Exception_DivisionByZero(Trace(eRight->GetPosition(), _env->name, _env->filePath)); }
		}break;
		case MakeBinop(VBOOL, VINT, LT): result = new BoolValue((int)eLeft->AsBool()->value < eRight->AsInt()->value, eLeft->GetPosition()); break;
		case MakeBinop(VBOOL, VINT, LTEQ): result = new BoolValue((int)eLeft->AsBool()->value <= eRight->AsInt()->value, eLeft->GetPosition()); break;
		case MakeBinop(VBOOL, VINT, GT): result = new BoolValue((int)eLeft->AsBool()->value > eRight->AsInt()->value, eLeft->GetPosition()); break;
		case MakeBinop(VBOOL, VINT, GTEQ): result = new BoolValue((int)eLeft->AsBool()->value >= eRight->AsInt()->value, eLeft->GetPosition()); break;
		case MakeBinop(VBOOL, VFLOAT, ADD): result = new FloatValue((float)eLeft->AsBool()->value + eRight->AsFloat()->value, eLeft->GetPosition()); break;
		case MakeBinop(VBOOL, VFLOAT, SUBTRACT): result = new FloatValue((float)eLeft->AsBool()->value - eRight->AsFloat()->value, eLeft->GetPosition()); break;
		case MakeBinop(VBOOL, VFLOAT, MULTIPY): result = new FloatValue((float)eLeft->AsBool()->value * eRight->AsFloat()->value, eLeft->GetPosition()); break;
		case MakeBinop(VBOOL, VFLOAT, DIVIDE): {
			if(eRight->AsFloat()->value != 0) { result = new FloatValue((float)eLeft->AsBool()->value / eRight->AsFloat()->value, eLeft->GetPosition()); }
			else { result = Exception_DivisionByZero(Trace(eRight->GetPosition(), _env->name, _env->filePath)); }
		}break;
		case MakeBinop(VBOOL, VFLOAT, LT): result = new BoolValue((float)eLeft->AsBool()->value < eRight->AsFloat()->value, eLeft->GetPosition()); break;
		case MakeBinop(VBOOL, VFLOAT, LTEQ): result = new BoolValue((float)eLeft->AsBool()->value <= eRight->AsFloat()->value, eLeft->GetPosition()); break;
		case MakeBinop(VBOOL, VFLOAT, GT): result = new BoolValue((float)eLeft->AsBool()->value > eRight->AsFloat()->value, eLeft->GetPosition()); break;
		case MakeBinop(VBOOL, VFLOAT, GTEQ): result = new BoolValue((float)eLeft->AsBool()->value >= eRight->AsFloat()->value, eLeft->GetPosition()); break;

		default: result = Exception_InvalidOperation("Cannot perform operation on these types.", Trace(_position, _env->name, _env->filePath));
	}

	return SHARE(result);
}

IExpression* BinopExpression::GetCopy() { return new BinopExpression(left->GetCopy(), right->GetCopy(), operation, _position); }

BinopExpression::OP BinopExpression::TokenTypeToOp(TokenType _type)
{
	switch(_type)
	{
		case TT_PLUS: return BinopExpression::ADD;
		case TT_MINUS: return BinopExpression::SUBTRACT;
		case TT_TIMES: return BinopExpression::MULTIPY;
		case TT_DIVIDE: return BinopExpression::DIVIDE;
		case TT_LT: return BinopExpression::LT;
		case TT_GT: return BinopExpression::GT;
		case TT_LT_EQ: return BinopExpression::LTEQ;
		case TT_GT_EQ: return BinopExpression::GTEQ;
		default: throw std::invalid_argument("Incorrect Token Type");
	}
}
#pragma endregion

#pragma region UnopExpression
UnopExpression::UnopExpression(IExpression* _expr, OP _op, Position _pos)
	: IExpression(_pos, EBINOP)
{
	expr = _expr;
	operation = _op;
}

UnopExpression::~UnopExpression() { delete expr; }

SharedValue UnopExpression::Evaluate(Environment* _env)
{
	SharedValue val = expr->Evaluate(_env);

	if(val->GetType() == VEXCEPTION)
		return val;

	IValue* result = 0;

	switch(MakeUnop(val->GetType(), operation))
	{
		case MakeUnop(VINT, NEG): result = new IntValue(-val->AsInt()->value, _position); break;
		case MakeUnop(VFLOAT, NEG): result = new FloatValue(-val->AsFloat()->value, _position); break;
		case MakeUnop(VINT, INC): result = new IntValue(val->AsInt()->value + 1, _position); break;
		case MakeUnop(VFLOAT, INC): result = new FloatValue(val->AsFloat()->value + 1, _position); break;
		case MakeUnop(VINT, DEC): result = new IntValue(val->AsInt()->value - 1, _position); break;
		case MakeUnop(VFLOAT, DEC): result = new IntValue(val->AsFloat()->value - 1, _position); break;
		case MakeUnop(VBOOL, NOT): result = new BoolValue(!val->AsBool(), _position); break;
		case MakeUnop(VINT, BIN_NOT): result = new IntValue(-val->AsInt()->value, _position); break;
		case MakeUnop(VFLOAT, BIN_NOT): result = new FloatValue(-val->AsFloat()->value, _position); break;

		default: return SHARE(Exception_InvalidOperation("Cannot perform operation on this type.", Trace(_position, _env->name, _env->filePath)));
	}

	if(operation == NEG || operation == NOT) { return SHARE(result); }
	else
	{
		val->Set(_env, result, _position);
		return SHARE_COPY(val);
	}
}

IExpression* UnopExpression::GetCopy() { return new UnopExpression(expr->GetCopy(), operation, _position); }

UnopExpression::OP UnopExpression::TokenTypeToOp(TokenType _type)
{
	switch(_type)
	{
		case TT_INC: return UnopExpression::INC;
		case TT_DEC: return UnopExpression::DEC;
		case TT_MINUS: return UnopExpression::NEG;
		case TT_NOT: return UnopExpression::NOT;
		case TT_BIN_NOT: return UnopExpression::BIN_NOT;
		default: throw std::invalid_argument("Incorrect Token Type");
	}
}
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

	if(val->GetType() == VEXCEPTION) { return val; }
	else
	{
		Environment* castingEnv = val->GetIntrinsicEnv();
		std::string castFuncName = "__" + typeName + "__";
		std::string castFuncSig = "func:(void)->" + typeName;

		std::vector<IExpression*> argExprs({});
		SharedValue func = castingEnv->GetValue(castFuncName, _position, false);
		Trace trace = Trace(_position, _env->name, _env->filePath);

		if(func->GetType() == VEXCEPTION) { return func; }
		else if(func->GetType() != VFUNC) { return SHARE(Exception_MismatchType(castFuncSig, func->GetTypeSig(), trace)); }
		else { return func->AsFunc()->Call(_env, argExprs, _position); }
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
		SharedValue val = SHARE_COPY(it.second->Evaluate(_env));

		if(val->GetType() == VEXCEPTION)
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

	return SHARE(new MemberedValue(memberDefs, typeSig, intrinsicEnv, _position));
}

IExpression* MemberedExpression::GetCopy()
{
	std::map<std::string, IExpression*> copy_MemAssigns;

	for(auto it : memAssigns)
		copy_MemAssigns.insert_or_assign(it.first, it.second->GetCopy());

	return new MemberedExpression(copy_MemAssigns, _position);
}
#pragma endregion

#pragma region ArrayExpression
ArrayExpression::ArrayExpression(std::vector<IExpression*>& _elemExprs, Position _pos) : IExpression(_pos, EMEMBERED) { elemExprs = _elemExprs; }

ArrayExpression::~ArrayExpression()
{
	for(auto it : elemExprs)
		delete it;
}

SharedValue ArrayExpression::Evaluate(Environment* _env)
{
	SharedValue firstVal = SHARE_COPY(elemExprs[0]->Evaluate(_env));

	if(firstVal->GetType() == VEXCEPTION)
		return firstVal;

	TypeSig elemTypeSig = firstVal->GetTypeSig();
	std::vector<SharedValue> elements({ firstVal });

	for(auto it : elemExprs)
	{
		SharedValue val = it->Evaluate(_env);
		TypeSig valTypeSig = val->GetTypeSig();

		if(valTypeSig != elemTypeSig) { return SHARE(Exception_MismatchType(elemTypeSig, valTypeSig, Trace(val->GetPosition(), _env->name, _env->filePath))); }
		else { elements.push_back(val); }
	}

	return SHARE(new ArrayValue(elemTypeSig, elements, _position));
}

IExpression* ArrayExpression::GetCopy()
{
	std::vector<IExpression*> copyElemExprs;

	for(auto it : elemExprs)
		copyElemExprs.push_back(it->GetCopy());

	return new ArrayExpression(copyElemExprs, _position);
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

	if(baseVal->GetType() == VEXCEPTION) { return baseVal; }
	else { return baseVal->AsMembered()->GetIntrinsicEnv()->GetValue(memberIdentifer, _position, false); }
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
	ValueType baseValType = baseVal->GetType();

	if(baseValType == VFUNC) { return baseVal->AsFunc()->Call(_env, argExprs, base->_position); }
	else if(baseValType == VEXCEPTION) { return baseVal; }
	else { return SHARE(Exception_InvalidOperation("Value not callable!", Trace(_position, _env->name, _env->filePath))); }
}

IExpression* FuncCallExpression::GetCopy()
{
	ArgumentList copy_argExprs;

	for(auto it : argExprs)
		copy_argExprs.push_back(it->GetCopy());

	return new FuncCallExpression(base->GetCopy(), copy_argExprs, _position);
}
#pragma endregion

#pragma region ArrayIndexExpression
ArrayIndexExpression::ArrayIndexExpression(IExpression* _base, IExpression* _indexExpr, Position _pos)
	: IExpression(_pos, EFUNC_CALL)
{
	base = _base;
	indexExpr = _indexExpr;
}

ArrayIndexExpression::~ArrayIndexExpression()
{
	delete indexExpr;
	delete base;
}

SharedValue ArrayIndexExpression::Evaluate(Environment* _env)
{
	SharedValue baseVal = base->Evaluate(_env);
	ValueType baseValType = baseVal->GetType();

	if(baseValType == VEXCEPTION) { return baseVal; }
	else
	{
		FuncValue* indexFunc = baseVal->GetIntrinsicEnv()->GetValue("__index__", _position, false)->AsFunc();
		std::vector<IExpression*> args({ indexExpr });
		return indexFunc->Call(_env, args, _position);
	}
}

IExpression* ArrayIndexExpression::GetCopy() { return new ArrayIndexExpression(base->GetCopy(), indexExpr->GetCopy(), _position); }
#pragma endregion