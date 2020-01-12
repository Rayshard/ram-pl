#include "pch.h"
#include "Value.h"
#include "Environment.h"
#include "Statement.h"
#include "Expression.h"
#include "Interpreter.h"

IValue::IValue(ValueType _type, Position _pos, Environment* _intrEnv)
	: _type(_type), _position(_pos), intrinsicEnv(_intrEnv)
{

}

IValue::~IValue()
{
	if(intrinsicEnv)
		delete intrinsicEnv;
}

Environment * IValue::GetIntrinsicEnv()
{
	if(!intrinsicEnv)
	{
		intrinsicEnv = new Environment(Environment::GLOBAL, "<VALUE>");

		FuncValue::built_in string_cast_body = [this](Environment* _env, Position _execPos) { return SHARE(new StringValue(this->ToString(), _execPos)); };
		std::vector<std::string> string_cast_argNames({}), string_cast_argSigs({});
		SharedValue func_string_cast = SHARE(new FuncValue(intrinsicEnv, string_cast_body, string_cast_argNames, string_cast_argSigs, "<STRING>", _position));

		intrinsicEnv->AddFuncDeclaration("__string__", func_string_cast, _position);
	}

	return intrinsicEnv;
}

#pragma region ExceptionValue
ExceptionValue::ExceptionValue(std::string _name, std::string _msg, StackTrace & _stackTrace)
	: IValue(VEXCEPTION, _stackTrace[0].position, nullptr)
{
	name = _name;
	message = _msg;
	stackTrace = _stackTrace;
}

ExceptionValue::ExceptionValue(std::string _name, std::string _msg, Trace _trace)
	: IValue(VEXCEPTION, _trace.position, 0)
{
	name = _name;
	message = _msg;
	stackTrace.push_back(_trace);
}

IValue* ExceptionValue::GetCopy() { return new ExceptionValue(name, message, stackTrace); }
TypeSig ExceptionValue::GetTypeSig() { return "<EXCEPTION>"; }

void ExceptionValue::ExtendStackTrace(Trace _trace) { stackTrace.push_back(_trace); }

std::string ExceptionValue::ToString()
{
	std::string str = ">> " + Interpreter::GetSrcLine(Environment::GLOBAL->filePath, _position.line) + "\n" + name + " Exception : " + message + "\"\n";

	for(auto it : stackTrace)
		str += "-> " + it.ToString() + "\n";

	return str;
}
#pragma endregion

#pragma region VoidValue
VoidValue::VoidValue(Position _pos) : IValue(VVOID, _pos, nullptr) { }
IValue* VoidValue::GetCopy() { return new VoidValue(_position); }
TypeSig VoidValue::GetTypeSig() { return "<VOID>"; }
std::string VoidValue::ToString() { return "void"; }
#pragma endregion

#pragma region MemberedValue
MemberedValue::MemberedValue(DefinitionMap& _memDefs, TypeSig _typeSig, Environment* _intrEnv, Position _pos)
	: IValue(VMEMBERED, _pos, _intrEnv)
{
	memDefinitions = _memDefs;
	typeSig = _typeSig;
}

IValue* MemberedValue::GetCopy() { return new MemberedValue(memDefinitions, typeSig, intrinsicEnv->GetCopy(), _position); }
TypeSig MemberedValue::GetTypeSig() { return typeSig; }

SharedValue MemberedValue::GetMemberValue(std::string _memId, Position _execPos) { return intrinsicEnv->GetValue(_memId, _execPos, false); }

std::string MemberedValue::ToString()
{
	std::string str = "{ ";

	for(auto it : memDefinitions)
		str += it.first + " = " + intrinsicEnv->GetValue(it.first, _position, false)->ToString() + "; ";

	str.pop_back();
	str.pop_back();
	str += " }";

	return str;
}
#pragma endregion

#pragma region ArrayValue
ArrayValue::ArrayValue(TypeSig _elemTypeSig, std::vector<SharedValue>& _initElems, Position _pos)
	: IValue(VARRAY, _pos, nullptr)
{
	elemTypeSig = _elemTypeSig;
	elements = _initElems;

	GetIntrinsicEnv(); //Setting the intrinsic env so it has the basic stuff plus what I'm about to add

	FuncValue::built_in add_body = [this](Environment* _env, Position _execPos)
	{
		elements.push_back(_env->GetValue("value", _execPos, false));
		return SHARE_VOID(_execPos);
	};
	std::vector<std::string> add_argNames({ "value" }), add_argSigs({ elemTypeSig });
	SharedValue func_add = SHARE(new FuncValue(intrinsicEnv, add_body, add_argNames, add_argSigs, "<VOID>", _position));

	intrinsicEnv->AddFuncDeclaration("add", func_add, _position);
}

ArrayValue::~ArrayValue() { elements.clear(); }

IValue* ArrayValue::GetCopy()
{
	std::vector<SharedValue> copyElems;

	for(auto it : elements)
		copyElems.push_back(SHARE(it->GetCopy()));

	return new ArrayValue(elemTypeSig, copyElems, _position);
}

TypeSig ArrayValue::GetTypeSig() { return "<[" + elemTypeSig + "]>"; }

std::string ArrayValue::ToString()
{
	std::string str = "[ ";

	for(int i = 0; i < elements.size(); i++)
		str += elements[i]->ToString() + ", ";

	str.pop_back();
	str.pop_back();
	str += " ]";

	return str;
}
#pragma endregion

#pragma region NamedspaceValue
NamedspaceValue::NamedspaceValue(Environment* _env, Position _pos)
	: IValue(VNAMEDSPACE, _pos, _env)
{
	environment = intrinsicEnv;
}

IValue* NamedspaceValue::GetCopy() { return new NamedspaceValue(environment->GetCopy(), _position); }
TypeSig NamedspaceValue::GetTypeSig() { return "<NAMEDSPACE>"; }
std::string NamedspaceValue::ToString() { return "namedspace"; }
#pragma endregion

#pragma region FuncValue
FuncValue::FuncValue(Environment * _defEnv, std::vector<std::string>& _argNames, std::vector<TypeSig>& _argSigs, TypeSig _retTypeSig, BodyType _bodyType, Position _pos)
	: IValue(VFUNC, _pos, new Environment(_defEnv, "Func Name"))
{
	defEnvironment = _defEnv;
	argNames = _argNames;
	argSigs = _argSigs;
	returnTypeSig = _retTypeSig;
	bodyType = _bodyType;

#pragma region Construt Func Type Sig
	typeSig = "<FUNC>";

	if(argSigs.size() == 0) { typeSig += "(void)"; }
	else
	{
		typeSig += "(";

		for(auto it : argSigs)
			typeSig += it + ",";

		typeSig.pop_back();
		typeSig += ")";
	}

	typeSig += "->" + _retTypeSig;
#pragma endregion
}

FuncValue::FuncValue(Environment* _defEnv, std::shared_ptr<IStatement> _body, std::vector<std::string>& _argNames, std::vector<TypeSig>& _argSigs, TypeSig _retTypeSig, Position _pos)
	: FuncValue(_defEnv, _argNames, _argSigs, _retTypeSig, DECLARED, _pos)
{
	body = _body;
	needsReturn = body->_type == SCODE_BLOCK;
}

FuncValue::FuncValue(Environment* _defEnv, built_in _ptr, std::vector<std::string>& _argNames, std::vector<TypeSig>& _argSigs, TypeSig _retTypeSig, Position _pos)
	: FuncValue(_defEnv, _argNames, _argSigs, _retTypeSig, BUILT_IN, _pos)
{
	pointer = _ptr;
}

SharedValue FuncValue::Call(Environment* _execEnv, ArgumentList& _argExprs, Position _execPos)
{
	Trace trace = Trace(_position, intrinsicEnv->name, intrinsicEnv->filePath);

	if(_argExprs.size() != argNames.size())
		return SHARE(Exception_WrongNumArgs(argNames.size(), _argExprs.size(), trace));

	Environment env(_execEnv, intrinsicEnv->name); //Doesn't have to be a ptr because it only needs to exist in this function
	env.propReturn = false;
	env.propBreak = false;
	env.propContinue = false;

	for(int i = 0; i < _argExprs.size(); i++)
	{
		SharedValue argVal = _argExprs[i]->Evaluate(_execEnv);
		TypeSig argSig = argSigs[i];

		if(argVal->GetType() == VEXCEPTION) { return argVal; }
		else if(argVal->GetTypeSig() != argSig) { return SHARE(Exception_MismatchType(argSig, argVal->GetTypeSig(), Trace(_execPos, _execEnv->name, _execEnv->filePath))); }
		else { env.AddVariable(argNames[i], SHARE(argVal->GetCopy()), _execPos); }
	}

	SharedValue retVal(nullptr);

	if(bodyType == DECLARED)
	{
		if(body->_type != SCODE_BLOCK)
			PRINT_LINE(env.filePath, body->_position.line);

		retVal = body->Execute(&env);
	}
	else { retVal = pointer(&env, _execPos); }

	if(retVal->GetType() == VEXCEPTION) { retVal->AsException()->ExtendStackTrace(trace); }
	else
	{
		if(needsReturn)
		{
			SharedValue varRetVal = env.GetValue("return", _execPos, false);

			if(varRetVal->GetType() == VEXCEPTION) { return SHARE(Exception_MissingReturn(trace)); }
			else { retVal = varRetVal; }
		}

		TypeSig retValTypeSig = retVal->GetTypeSig();

		if(retValTypeSig != returnTypeSig)
			return SHARE(Exception_MismatchType(returnTypeSig, retValTypeSig, trace));
	}

	return retVal;
}

IValue* FuncValue::GetCopy()
{
	if(bodyType == DECLARED) { return new FuncValue(defEnvironment, body, argNames, argSigs, returnTypeSig, _position); }
	else { return new FuncValue(defEnvironment, pointer, argNames, argSigs, returnTypeSig, _position); }
}

TypeSig FuncValue::GetTypeSig() { return typeSig; }
std::string FuncValue::ToString() { return GetTypeSig(); }
#pragma endregion

SharedValue Value::Set(Environment* _execEnv, IValue * _val, Position _execPos)
{
	TypeSig orgTypeSig = value->GetTypeSig();
	TypeSig valTypeSig = _val->GetTypeSig();

	if(orgTypeSig == valTypeSig)
	{
		delete value;
		value = _val;
		return SHARE(new VoidValue(_execPos));
	}
	else { return SHARE(Exception_MismatchType(orgTypeSig, valTypeSig, Trace(_execPos, _execEnv->name, _execEnv->filePath))); }
}
