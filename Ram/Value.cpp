#include "pch.h"
#include "Value.h"
#include "Environment.h"
#include "Statement.h"
#include "Expression.h"

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

		FuncValue::built_in string_cast_body = [this](Environment* _env, Position _execPos) { return SharedValue(new StringValue(this->ToString(), _execPos)); };
		std::vector<std::string> string_cast_argNames({}), string_cast_argSigs({});
		SharedValue func_string_cast = SharedValue(new FuncValue(intrinsicEnv, string_cast_body, string_cast_argNames, string_cast_argSigs, "<STRING>", _position));

		intrinsicEnv->AddFuncDeclaration("__string__", func_string_cast, _position);
	}

	return intrinsicEnv;
}

#pragma region ExceptionValue
ExceptionValue::ExceptionValue(std::string _name, std::string _msg, StackTrace & _stackTrace)
	: IValue(VEXCEPTION, _stackTrace[0].position, 0)
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
	std::string str = ">> " + GetSrcLine(_position.line) + "\n" + name + " Exception : " + message + "\"\n";

	for(auto it : stackTrace)
		str += "-> " + it.ToString() + "\n";

	return str;
}
#pragma endregion

#pragma region VoidValue
VoidValue::VoidValue(Position _pos) : IValue(VVOID, _pos, 0) { }
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

#pragma region NamedspaceValue
NamedspaceValue::NamedspaceValue(Environment* _env, Position _pos)
	: IValue(VNAMEDSPACE, _pos, _env)
{
	environment = intrinsicEnv;
}

NamedspaceValue::~NamedspaceValue() { delete environment; }
IValue* NamedspaceValue::GetCopy() { return new NamedspaceValue(environment->GetCopy(), _position); }
TypeSig NamedspaceValue::GetTypeSig() { return "<NAMEDSPACE>" + environment->name; }
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
}

FuncValue::FuncValue(Environment* _defEnv, built_in _ptr, std::vector<std::string>& _argNames, std::vector<TypeSig>& _argSigs, TypeSig _retTypeSig, Position _pos)
	: FuncValue(_defEnv, _argNames, _argSigs, _retTypeSig, BUILT_IN, _pos)
{
	pointer = _ptr;
}

SharedValue FuncValue::Call(Environment* _execEnv, ArgumentList& _argExprs, Position _execPos)
{
	Trace trace = Trace(_position, intrinsicEnv->name, "Put FileName Here");

	if(_argExprs.size() != argNames.size())
		return SharedValue(Exception_WrongNumArgs(argNames.size(), _argExprs.size(), trace));

	Environment env(intrinsicEnv, "<FUNC>"); //Doesn't have to be a ptr because it only needs to exist in this function

	for(int i = 0; i < _argExprs.size(); i++)
	{
		SharedValue argVal = _argExprs[i]->Evaluate(_execEnv);
		TypeSig argSig = argSigs[i];

		if(argVal->_type == VEXCEPTION) { return argVal; }
		else if(argVal->GetTypeSig() != argSig) { return SharedValue(Exception_MismatchType(argSig, argVal->GetTypeSig(), Trace(_execPos, _execEnv->name, "Put FileName Here"))); }
		else { env.AddVariable(argNames[i], argVal, _execPos); }
	}

	if(dynamic_cast<CodeBlock*>(body.get()) == 0) // Check if function has code block body or just a one line body
	{
#if PRINT_LINE
		PrintSrcLine(body->_position.line);
#endif
	}

	SharedValue retVal(nullptr);

	if(bodyType == DECLARED) { retVal = body->Execute(&env); }
	else { retVal = pointer(&env, _execPos); }

	if(retVal->_type == VEXCEPTION)
		((ExceptionValue*)retVal.get())->ExtendStackTrace(trace);

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
