#include "Value.h"
#include "Statement.h"

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
		intrinsicEnv = new Environment(Environment::GLOBAL, "");

		FuncValue::built_in string_cast_body = [this](Environment* _env, Position _execPos) { return std::shared_ptr<IValue>(new StringValue(this->ToString(), _execPos)); };
		std::vector<std::string> string_cast_argNames({}), string_cast_argSigs({});
		SharedValue func_string_cast = std::shared_ptr<IValue>(new FuncValue(intrinsicEnv, string_cast_body, string_cast_argNames, string_cast_argSigs, "<STRING>", _position));

		intrinsicEnv->AddFuncDeclaration("__string__", func_string_cast, _position);
	}

	return intrinsicEnv;
}

#pragma region ExceptionValue
ExceptionValue::ExceptionValue(std::string _name, std::string _msg, Position _pos)
	: IValue(VEXCEPTION, _pos, 0)
{
	name = _name;
	message = _msg;
}

IValue* ExceptionValue::GetCopy() { return new ExceptionValue(name, message, _position); }
TypeSig ExceptionValue::GetTypeSig() { return "<EXCEPTION>"; }
std::string ExceptionValue::ToString() { return "Exception : " + name + "\n" + _position.ToString() + "\n" + message + "\n>> " + GetSrcLine(_position.line); }
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
	: IValue(VFUNC, _pos, 0)
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

FuncValue::FuncValue(Environment* _defEnv, IStatement* _body, std::vector<std::string>& _argNames, std::vector<TypeSig>& _argSigs, TypeSig _retTypeSig, Position _pos)
	: FuncValue(_defEnv, _argNames, _argSigs, _retTypeSig, DECLARED, _pos)
{
	body = _body;
}

FuncValue::FuncValue(Environment* _defEnv, built_in _ptr, std::vector<std::string>& _argNames, std::vector<TypeSig>& _argSigs, TypeSig _retTypeSig, Position _pos)
	: FuncValue(_defEnv, _argNames, _argSigs, _retTypeSig, BUILT_IN, _pos)
{
	pointer = _ptr;
}

FuncValue::~FuncValue()
{
	if(body)
		delete body;
}

SharedValue FuncValue::Call(Environment* _execEnv, ArgumentList& _argExprs, Position _execPos)
{
	if(_argExprs.size() != argNames.size())
		return std::shared_ptr<IValue>(Exception_WrongNumArgs(argNames.size(), _argExprs.size(), _execPos));

	Environment env(defEnvironment, "<FUNC>"); //Doesn't have to be a ptr because it only needs to exist in this function

	for(int i = 0; i < _argExprs.size(); i++)
	{
		SharedValue argVal = _argExprs[i]->Evaluate(_execEnv);
		TypeSig argSig = argSigs[i];

		if(argVal->_type == VEXCEPTION) { return argVal; }
		else if(argVal->GetTypeSig() != argSig) { return std::shared_ptr<IValue>(Exception_MismatchType(argSig, argVal->GetTypeSig(), _execPos)); }
		else { env.AddVariable(argNames[i], argVal, _execPos, false); }
	}

	if(dynamic_cast<CodeBlock*>(body) == 0) // Check if function has code block body or just a one line body
	{
#if PRINT_LINE
		PrintSrcLine(body->_position.line);
#endif
	}

	if(bodyType == DECLARED) { return body->Execute(&env); }
	else { return pointer(&env, _execPos); }
}

IValue* FuncValue::GetCopy()
{
	if(bodyType == DECLARED) { return new FuncValue(defEnvironment, body->GetCopy(), argNames, argSigs, returnTypeSig, _position); }
	else { return new FuncValue(defEnvironment, pointer, argNames, argSigs, returnTypeSig, _position); }
}

TypeSig FuncValue::GetTypeSig() { return typeSig; }
std::string FuncValue::ToString() { return GetTypeSig(); }
#pragma endregion
