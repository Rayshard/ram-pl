#include "pch.h"
#include "Value.h"
#include "Environment.h"
#include "Statement.h"
#include "Expression.h"
#include "Interpreter.h"

IValue::IValue(ValueType _type, Position _pos, Environment* _intrEnv)
	: type(_type), position(_pos), intrinsicEnv(_intrEnv)
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
		intrinsicEnv = new Environment(Environment::GLOBAL, GetTypeSig());

		FuncValue::built_in string_cast_body = [this](Environment* _env, Position _execPos) { return SHARE(new StringValue(this->ToString(), _execPos)); };
		std::vector<std::string> string_cast_argNames({}), string_cast_argSigs({});
		SharedValue func_string_cast = SHARE(new FuncValue(intrinsicEnv, "__string__", string_cast_body, string_cast_argNames, string_cast_argSigs, "<STRING>", position));

		intrinsicEnv->AddFuncDeclaration("__string__", func_string_cast, position);

#pragma region StringValue Instrinsic Members
		if(type == VSTRING)
		{
#pragma region StringValue Index Function
			FuncValue::built_in index_body = [this](Environment* _env, Position _execPos)
			{
				std::string value = ((StringValue*)this)->value;
				int index = _env->GetValue("index", _execPos, false)->AsInt()->value;

				if(index < 0 || index >= (int)value.size()) { return SHARE(Exception_OutOfRange("index", Trace(_execPos, intrinsicEnv->name, intrinsicEnv->filePath))); }
				else { return SHARE(new StringValue(std::string(1, value[index]), _execPos)); }
			};
			std::vector<std::string> index_argNames({ "index" }), index_argSigs({ "<INT>" });
			SharedValue func_index = SHARE(new FuncValue(intrinsicEnv, "__index__", index_body, index_argNames, index_argSigs, "<STRING>", position));

			intrinsicEnv->AddFuncDeclaration("__index__", func_index, position);
#pragma endregion
#pragma endregion
		}
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
	std::string str = ">> " + Interpreter::GetSrcLine(Environment::GLOBAL->filePath, position.line) + "\n" + name + " Exception : " + message + "\"\n";

	for(auto it : stackTrace)
		str += "-> " + it.ToString() + "\n";

	return str;
}
#pragma endregion

#pragma region VoidValue
VoidValue::VoidValue(Position _pos) : IValue(VVOID, _pos, nullptr) { }
IValue* VoidValue::GetCopy() { return new VoidValue(position); }
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

IValue* MemberedValue::GetCopy() { return new MemberedValue(memDefinitions, typeSig, intrinsicEnv->GetCopy(), position); }
TypeSig MemberedValue::GetTypeSig() { return typeSig; }

SharedValue MemberedValue::GetMemberValue(std::string _memId, Position _execPos) { return intrinsicEnv->GetValue(_memId, _execPos, false); }

std::string MemberedValue::ToString()
{
	std::string str = "{ ";

	for(auto it : memDefinitions)
		str += it.first + " = " + intrinsicEnv->GetValue(it.first, position, false)->ToString() + "; ";

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

#pragma region Intrinsic Functions
#pragma region Append
	FuncValue::built_in append_body = [this](Environment* _env, Position _execPos)
	{
		elements.push_back(_env->GetValue("value", _execPos, false));
		return SHARE_VOID(_execPos);
	};
	std::vector<std::string> append_argNames({ "value" }), append_argSigs({ elemTypeSig });
	SharedValue func_append = SHARE(new FuncValue(intrinsicEnv, "append", append_body, append_argNames, append_argSigs, "<VOID>", position));

	intrinsicEnv->AddFuncDeclaration("append", func_append, position);
#pragma endregion

#pragma region AppendArray
	FuncValue::built_in appendArray_body = [this](Environment* _env, Position _execPos)
	{
		ArrayValue* arr = _env->GetValue("array", _execPos, false)->AsArray();
		std::vector<SharedValue> copiedElems;

		for(auto it : arr->elements)
			copiedElems.push_back(SHARE_COPY(it));

		elements.insert(elements.end(), copiedElems.begin(), copiedElems.end());
		return SHARE_VOID(_execPos);
	};
	std::vector<std::string> appendArray_argNames({ "array" }), appendArray_argSigs({ "<[" + elemTypeSig + "]>" });
	SharedValue func_appendArray = SHARE(new FuncValue(intrinsicEnv, "appendArray", appendArray_body, appendArray_argNames, appendArray_argSigs, "<VOID>", position));

	intrinsicEnv->AddFuncDeclaration("appendArray", func_appendArray, position);
#pragma endregion

#pragma region Insert
	FuncValue::built_in insert_body = [this](Environment* _env, Position _execPos)
	{
		int index = _env->GetValue("index", _execPos, false)->AsInt()->value;

		if(index < 0 || index > (int)elements.size())
			return SHARE(Exception_OutOfRange("index", Trace(_execPos, _env->name, _env->filePath)));

		elements.insert(elements.begin() + index, _env->GetValue("value", _execPos, false));
		return SHARE_VOID(_execPos);
	};
	std::vector<std::string> insert_argNames({ "value", "index" }), insert_argSigs({ elemTypeSig, "<INT>" });
	SharedValue func_insert = SHARE(new FuncValue(intrinsicEnv, "insert", insert_body, insert_argNames, insert_argSigs, "<VOID>", position));

	intrinsicEnv->AddFuncDeclaration("insert", func_insert, position);
#pragma endregion

#pragma region InsertArray
	FuncValue::built_in insertArray_body = [this](Environment* _env, Position _execPos)
	{
		int index = _env->GetValue("index", _execPos, false)->AsInt()->value;

		if(index < 0 || index > (int)elements.size())
			return SHARE(Exception_OutOfRange("index", Trace(_execPos, _env->name, _env->filePath)));

		ArrayValue* arr = _env->GetValue("array", _execPos, false)->AsArray();
		std::vector<SharedValue> copiedElems;

		for(auto it : arr->elements)
			copiedElems.push_back(SHARE_COPY(it));

		elements.insert(elements.begin() + index, copiedElems.begin(), copiedElems.end());
		return SHARE_VOID(_execPos);
	};
	std::vector<std::string> insertArray_argNames({ "array", "index" }), insertArray_argSigs({ "<[" + elemTypeSig + "]>", "<INT>" });
	SharedValue func_insertArray = SHARE(new FuncValue(intrinsicEnv,"insertArray", insertArray_body, insertArray_argNames, insertArray_argSigs, "<VOID>", position));

	intrinsicEnv->AddFuncDeclaration("insertArray", func_insertArray, position);
#pragma endregion

#pragma region RemoveAt
	FuncValue::built_in removeAt_body = [this](Environment* _env, Position _execPos)
	{
		int index = _env->GetValue("index", _execPos, false)->AsInt()->value;

		if(index < 0 || index >= (int)elements.size())
			return SHARE(Exception_OutOfRange("index", Trace(_execPos, _env->name, _env->filePath)));

		elements.erase(elements.begin() + index);
		return SHARE_VOID(_execPos);
	};
	std::vector<std::string> removeAt_argNames({ "index" }), removeAt_argSigs({ "<INT>" });
	SharedValue func_removeAt = SHARE(new FuncValue(intrinsicEnv, "removeAt", removeAt_body, removeAt_argNames, removeAt_argSigs, "<VOID>", position));

	intrinsicEnv->AddFuncDeclaration("removeAt", func_removeAt, position);
#pragma endregion

#pragma region RemoveRange
	FuncValue::built_in removeRange_body = [this](Environment* _env, Position _execPos)
	{
		int start = _env->GetValue("start", _execPos, false)->AsInt()->value;

		if(start < 0 || start > (int)elements.size())
			return SHARE(Exception_OutOfRange("start", Trace(_execPos, _env->name, _env->filePath)));

		int count = _env->GetValue("count", _execPos, false)->AsInt()->value;

		elements.erase(elements.begin() + start, elements.begin() + count);
		return SHARE_VOID(_execPos);
	};
	std::vector<std::string> removeRange_argNames({ "start", "count" }), removeRange_argSigs({ "<INT>", "<INT>" });
	SharedValue func_removeRange = SHARE(new FuncValue(intrinsicEnv, "removeRange", removeRange_body, removeRange_argNames, removeRange_argSigs, "<VOID>", position));

	intrinsicEnv->AddFuncDeclaration("removeRange", func_removeRange, position);
#pragma endregion

#pragma region Clear
	FuncValue::built_in clear_body = [this](Environment* _env, Position _execPos)
	{
		elements.clear();
		return SHARE_VOID(_execPos);
	};
	std::vector<std::string> clear_argNames({}), clear_argSigs({});
	SharedValue func_clear = SHARE(new FuncValue(intrinsicEnv, "clear", clear_body, clear_argNames, clear_argSigs, "<VOID>", position));

	intrinsicEnv->AddFuncDeclaration("clear", func_clear, position);
#pragma endregion

#pragma region Size
	FuncValue::built_in size_body = [this](Environment* _env, Position _execPos) { return SHARE(new IntValue(elements.size(), _execPos)); };
	std::vector<std::string> size_argNames({}), size_argSigs({});
	SharedValue func_size = SHARE(new FuncValue(intrinsicEnv, "size", size_body, size_argNames, size_argSigs, "<INT>", position));

	intrinsicEnv->AddFuncDeclaration("size", func_size, position);
#pragma endregion

#pragma region Index
	FuncValue::built_in index_body = [this](Environment* _env, Position _execPos)
	{
		int index = _env->GetValue("index", _execPos, false)->AsInt()->value;

		if(index < 0 || index >= (int)elements.size()) { return SHARE(Exception_OutOfRange("index", Trace(_execPos, intrinsicEnv->name, intrinsicEnv->filePath))); }
		else { return elements[index]; }
	};
	std::vector<std::string> index_argNames({ "index" }), index_argSigs({ "<INT>" });
	SharedValue func_index = SHARE(new FuncValue(intrinsicEnv, "__index__", index_body, index_argNames, index_argSigs, elemTypeSig, position));

	intrinsicEnv->AddFuncDeclaration("__index__", func_index, position);
#pragma endregion
#pragma endregion
}

ArrayValue::~ArrayValue() { elements.clear(); }

IValue* ArrayValue::GetCopy()
{
	std::vector<SharedValue> copyElems;

	for(auto it : elements)
		copyElems.push_back(SHARE_COPY(it));

	return new ArrayValue(elemTypeSig, copyElems, position);
}

TypeSig ArrayValue::GetTypeSig() { return "<[" + elemTypeSig + "]>"; }

std::string ArrayValue::ToString()
{
	std::string str = "[ ";

for(auto it : elements)
		str += it->ToString() + ", ";

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

IValue* NamedspaceValue::GetCopy() { return new NamedspaceValue(environment->GetCopy(), position); }
TypeSig NamedspaceValue::GetTypeSig() { return "<NAMEDSPACE>"; }
std::string NamedspaceValue::ToString() { return "namedspace"; }
#pragma endregion

#pragma region FuncValue
FuncValue::FuncValue(Environment * _defEnv, std::string _name, std::vector<std::string>& _argNames, std::vector<TypeSig>& _argSigs, TypeSig _retTypeSig, BodyType _bodyType, Position _pos)
	: IValue(VFUNC, _pos, new Environment(_defEnv, _name))
{
	defEnvironment = _defEnv;
	name = _name;
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

FuncValue::FuncValue(Environment* _defEnv, std::string _name, std::shared_ptr<IStatement> _body, std::vector<std::string>& _argNames, std::vector<TypeSig>& _argSigs, TypeSig _retTypeSig, Position _pos)
	: FuncValue(_defEnv, _name, _argNames, _argSigs, _retTypeSig, DECLARED, _pos)
{
	body = _body;
	needsReturn = body->_type == SCODE_BLOCK;
}

FuncValue::FuncValue(Environment* _defEnv, std::string _name, built_in _ptr, std::vector<std::string>& _argNames, std::vector<TypeSig>& _argSigs, TypeSig _retTypeSig, Position _pos)
	: FuncValue(_defEnv, _name, _argNames, _argSigs, _retTypeSig, BUILT_IN, _pos)
{
	pointer = _ptr;
}

SharedValue FuncValue::Call(Environment* _execEnv, ArgumentList& _argExprs, Position _execPos)
{
	Trace trace = Trace(_execPos, intrinsicEnv->name, intrinsicEnv->filePath);

	if(_argExprs.size() != argNames.size())
		return SHARE(Exception_WrongNumArgs(argNames.size(), _argExprs.size(), trace));

	Environment env(_execEnv, name); //Doesn't have to be a ptr because it only needs to exist in this function
	env.propReturn = false;
	env.propBreak = false;
	env.propContinue = false;

	for(int i = 0; i < (int)_argExprs.size(); i++)
	{
		SharedValue argVal = _argExprs[i]->Evaluate(_execEnv);
		TypeSig argSig = argSigs[i];

		if(argVal->GetType() == VEXCEPTION) { return argVal; }
		else if(argVal->GetTypeSig() != argSig) { return SHARE(Exception_MismatchType(argSig, argVal->GetTypeSig(), Trace(_execPos, _execEnv->name, _execEnv->filePath))); }
		else { env.AddVariable(argNames[i], SHARE_COPY(argVal), _execPos); }
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
	if(bodyType == DECLARED) { return new FuncValue(defEnvironment, name, body, argNames, argSigs, returnTypeSig, position); }
	else { return new FuncValue(defEnvironment, name, pointer, argNames, argSigs, returnTypeSig, position); }
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
		return SHARE_VOID(_execPos);
	}
	else { return SHARE(Exception_MismatchType(orgTypeSig, valTypeSig, Trace(_execPos, _execEnv->name, _execEnv->filePath))); }
}
