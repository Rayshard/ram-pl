#include "pch.h"
#include "Value.h"
#include "Environment.h"
#include "Statement.h"
#include "Expression.h"
#include "Interpreter.h"

#pragma region Signatures
const std::string IValue::SIGNATURE_VOID = "<VOID>";
const std::string IValue::SIGNATURE_UNKNOWN = "<UNKNOWN>";
const std::string IValue::SIGNATURE_EXCEPTION = "<EXCEPTION>";
const std::string IValue::SIGNATURE_MEMBERED = "<MEMBERED>";
const std::string IValue::SIGNATURE_ARRAY = "<ARRAY>";
const std::string IValue::SIGNATURE_FUNC = "<FUNC>";
const std::string IValue::SIGNATURE_NAMEDSPACE = "<NAMEDSPACE>";
const std::string IValue::SIGNATURE_POINTER = "<POINTER>";
const std::string IValue::SIGNATURE_INT = "<INT>";
const std::string IValue::SIGNATURE_FLOAT = "<FLOAT>";
const std::string IValue::SIGNATURE_BOOL = "<BOOL>";
const std::string IValue::SIGNATURE_STRING = "<STRING>";
#pragma endregion

IValue::IValue(ValueType _type, Position _pos, Environment* _intrEnv)
{
	type = _type;
	position = _pos;
	intrinsicEnv = _intrEnv;

	builtInFunc string_cast_body = [this](Environment* _env, Position _execPos) { return SHARE(new StringValue(this->ToString(), _execPos)); };
	AddCast(SIGNATURE_STRING, string_cast_body);
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
		intrinsicEnv = new Environment(Environment::GLOBAL, GetSignature());

#pragma region StringValue Instrinsic Members
		if(type == VSTRING)
		{
#pragma region StringValue Index Function
			builtInFunc index_body = [this](Environment* _env, Position _execPos)
			{
				std::string value = ((StringValue*)this)->value;
				int index = _env->GetValue("index", _execPos, false)->AsInt()->value;

				if(index < 0 || index >= (int)value.size()) { return SHARE(Exception_OutOfRange("index", Trace(_execPos, intrinsicEnv->name, intrinsicEnv->filePath))); }
				else { return SHARE(new StringValue(std::string(1, value[index]), _execPos)); }
			};
			std::vector<std::string> index_argNames({ "index" }), index_argSigs({ SIGNATURE_INT });
			SharedValue func_index = SHARE(new FuncValue(intrinsicEnv, "__index__", index_body, index_argNames, index_argSigs, SIGNATURE_STRING, position));

			intrinsicEnv->AddFuncDeclaration("__index__", func_index, position);
#pragma endregion

#pragma region StringValue Length Function
			builtInFunc length_body = [this](Environment* _env, Position _execPos) { return SHARE(new IntValue(((StringValue*)this)->value.size(), _execPos)); };
			std::vector<std::string> length_argNames({}), length_argSigs({});
			SharedValue func_length = SHARE(new FuncValue(intrinsicEnv, "length", length_body, length_argNames, length_argSigs, SIGNATURE_INT, position));

			intrinsicEnv->AddFuncDeclaration("length", func_length, position);
#pragma endregion
		}
#pragma endregion
	}

	return intrinsicEnv;
}

IValue * IValue::FromRamValue(RamAny* _ramVal, Position _execPos)
{
	switch(_ramVal->type)
	{
		case RamAny::RVOID: return new VoidValue(_execPos);
		case RamAny::RINT: return new IntValue(_ramVal->_int, _execPos);
		case RamAny::RFLOAT: return new FloatValue(_ramVal->_float, _execPos);
		case RamAny::RBOOL: return new BoolValue(_ramVal->_bool, _execPos);
		case RamAny::RSTRING: { return new StringValue(_ramVal->_string, _execPos); }
		default: throw std::runtime_error("Missing case in IValue.FromRamValue()");
	}
}

SharedValue IValue::Cast(Environment* _execEnv, Signature _retSig, Position _execPos)
{
	auto search = castingFuncs.find(_retSig);
	if(search != castingFuncs.end()) { return search->second(_execEnv, _execPos); }
	else { return SHARE(Exception_CastNotFound(_retSig, Trace(_execPos, _execEnv->name, _execEnv->filePath))); }
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
	: IValue(VEXCEPTION, _trace.position, nullptr)
{
	name = _name;
	message = _msg;
	stackTrace.push_back(_trace);
}

std::string ExceptionValue::ToString()
{
	std::string str = ">> " + Interpreter::GetSrcLine(Environment::GLOBAL->filePath, position.line) + "\n" + name + " Exception : " + message + "\"\n";

	for(auto it : stackTrace)
		str += "-> " + it.ToString() + "\n";

	return str;
}
#pragma endregion

#pragma region MemberedValue
MemberedValue::MemberedValue(DefinitionMap& _memDefs, Signature _sig, Environment* _intrEnv, Position _pos)
	: IValue(VMEMBERED, _pos, _intrEnv)
{
	memDefinitions = _memDefs;
	signature = _sig;
}

IValue* MemberedValue::GetCopy() { return new MemberedValue(memDefinitions, signature, intrinsicEnv->GetCopy(), position); }
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
ArrayValue::ArrayValue(Signature _elemSignature, std::vector<SharedValue>& _initElems, Position _pos)
	: IValue(VARRAY, _pos, nullptr)
{
	elemSignature = _elemSignature;
	elements = _initElems;

	GetIntrinsicEnv(); //Setting the intrinsic env so it has the basic stuff plus what I'm about to add

#pragma region Intrinsic Functions
#pragma region Append
	builtInFunc append_body = [this](Environment* _env, Position _execPos)
	{
		elements.push_back(_env->GetValue("value", _execPos, false));
		return SHARE_VOID(_execPos);
	};
	std::vector<std::string> append_argNames({ "value" }), append_argSigs({ elemSignature });
	SharedValue func_append = SHARE(new FuncValue(intrinsicEnv, "append", append_body, append_argNames, append_argSigs, "<VOID>", position));

	intrinsicEnv->AddFuncDeclaration("append", func_append, position);
#pragma endregion

#pragma region AppendArray
	builtInFunc appendArray_body = [this](Environment* _env, Position _execPos)
	{
		ArrayValue* arr = _env->GetValue("array", _execPos, false)->AsArray();
		std::vector<SharedValue> copiedElems;

		for(auto it : arr->elements)
			copiedElems.push_back(SHARE_COPY(it));

		elements.insert(elements.end(), copiedElems.begin(), copiedElems.end());
		return SHARE_VOID(_execPos);
	};
	std::vector<std::string> appendArray_argNames({ "array" }), appendArray_argSigs({ elemSignature + "|" + IValue::SIGNATURE_ARRAY });
	SharedValue func_appendArray = SHARE(new FuncValue(intrinsicEnv, "appendArray", appendArray_body, appendArray_argNames, appendArray_argSigs, "<VOID>", position));

	intrinsicEnv->AddFuncDeclaration("appendArray", func_appendArray, position);
#pragma endregion

#pragma region Insert
	builtInFunc insert_body = [this](Environment* _env, Position _execPos)
	{
		int index = _env->GetValue("index", _execPos, false)->AsInt()->value;

		if(index < 0 || index >(int)elements.size())
			return SHARE(Exception_OutOfRange("index", Trace(_execPos, _env->name, _env->filePath)));

		elements.insert(elements.begin() + index, _env->GetValue("value", _execPos, false));
		return SHARE_VOID(_execPos);
	};
	std::vector<std::string> insert_argNames({ "value", "index" }), insert_argSigs({ elemSignature, SIGNATURE_INT });
	SharedValue func_insert = SHARE(new FuncValue(intrinsicEnv, "insert", insert_body, insert_argNames, insert_argSigs, SIGNATURE_VOID, position));

	intrinsicEnv->AddFuncDeclaration("insert", func_insert, position);
#pragma endregion

#pragma region InsertArray
	builtInFunc insertArray_body = [this](Environment* _env, Position _execPos)
	{
		int index = _env->GetValue("index", _execPos, false)->AsInt()->value;

		if(index < 0 || index >(int)elements.size())
			return SHARE(Exception_OutOfRange("index", Trace(_execPos, _env->name, _env->filePath)));

		ArrayValue* arr = _env->GetValue("array", _execPos, false)->AsArray();
		std::vector<SharedValue> copiedElems;

		for(auto it : arr->elements)
			copiedElems.push_back(SHARE_COPY(it));

		elements.insert(elements.begin() + index, copiedElems.begin(), copiedElems.end());
		return SHARE_VOID(_execPos);
	};
	std::vector<std::string> insertArray_argNames({ "array", "index" }), insertArray_argSigs({ elemSignature + "|" + SIGNATURE_ARRAY, SIGNATURE_INT });
	SharedValue func_insertArray = SHARE(new FuncValue(intrinsicEnv, "insertArray", insertArray_body, insertArray_argNames, insertArray_argSigs, SIGNATURE_VOID, position));

	intrinsicEnv->AddFuncDeclaration("insertArray", func_insertArray, position);
#pragma endregion

#pragma region RemoveAt
	builtInFunc removeAt_body = [this](Environment* _env, Position _execPos)
	{
		int index = _env->GetValue("index", _execPos, false)->AsInt()->value;

		if(index < 0 || index >= (int)elements.size())
			return SHARE(Exception_OutOfRange("index", Trace(_execPos, _env->name, _env->filePath)));

		elements.erase(elements.begin() + index);
		return SHARE_VOID(_execPos);
	};
	std::vector<std::string> removeAt_argNames({ "index" }), removeAt_argSigs({ SIGNATURE_INT });
	SharedValue func_removeAt = SHARE(new FuncValue(intrinsicEnv, "removeAt", removeAt_body, removeAt_argNames, removeAt_argSigs, SIGNATURE_VOID, position));

	intrinsicEnv->AddFuncDeclaration("removeAt", func_removeAt, position);
#pragma endregion

#pragma region RemoveRange
	builtInFunc removeRange_body = [this](Environment* _env, Position _execPos)
	{
		int start = _env->GetValue("start", _execPos, false)->AsInt()->value;

		if(start < 0 || start >(int)elements.size())
			return SHARE(Exception_OutOfRange("start", Trace(_execPos, _env->name, _env->filePath)));

		int count = _env->GetValue("count", _execPos, false)->AsInt()->value;

		elements.erase(elements.begin() + start, elements.begin() + count);
		return SHARE_VOID(_execPos);
	};
	std::vector<std::string> removeRange_argNames({ "start", "count" }), removeRange_argSigs({ SIGNATURE_INT, SIGNATURE_INT });
	SharedValue func_removeRange = SHARE(new FuncValue(intrinsicEnv, "removeRange", removeRange_body, removeRange_argNames, removeRange_argSigs, SIGNATURE_VOID, position));

	intrinsicEnv->AddFuncDeclaration("removeRange", func_removeRange, position);
#pragma endregion

#pragma region Clear
	builtInFunc clear_body = [this](Environment* _env, Position _execPos)
	{
		elements.clear();
		return SHARE_VOID(_execPos);
	};
	std::vector<std::string> clear_argNames({}), clear_argSigs({});
	SharedValue func_clear = SHARE(new FuncValue(intrinsicEnv, "clear", clear_body, clear_argNames, clear_argSigs, "<VOID>", position));

	intrinsicEnv->AddFuncDeclaration("clear", func_clear, position);
#pragma endregion

#pragma region Size
	builtInFunc size_body = [this](Environment* _env, Position _execPos) { return SHARE(new IntValue(elements.size(), _execPos)); };
	std::vector<std::string> size_argNames({}), size_argSigs({});
	SharedValue func_size = SHARE(new FuncValue(intrinsicEnv, "size", size_body, size_argNames, size_argSigs, SIGNATURE_INT, position));

	intrinsicEnv->AddFuncDeclaration("size", func_size, position);
#pragma endregion

#pragma region Index
	builtInFunc index_body = [this](Environment* _env, Position _execPos)
	{
		int index = _env->GetValue("index", _execPos, false)->AsInt()->value;

		if(index < 0 || index >= (int)elements.size()) { return SHARE(Exception_OutOfRange("index", Trace(_execPos, intrinsicEnv->name, intrinsicEnv->filePath))); }
		else { return elements[index]; }
	};
	std::vector<std::string> index_argNames({ "index" }), index_argSigs({ SIGNATURE_INT });
	SharedValue func_index = SHARE(new FuncValue(intrinsicEnv, "__index__", index_body, index_argNames, index_argSigs, elemSignature, position));

	intrinsicEnv->AddFuncDeclaration("__index__", func_index, position);
#pragma endregion
#pragma endregion
}

IValue* ArrayValue::GetCopy()
{
	std::vector<SharedValue> copyElems;

	for(auto it : elements)
		copyElems.push_back(SHARE_COPY(it));

	return new ArrayValue(elemSignature, copyElems, position);
}

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

#pragma region PointerValue
std::string PointerValue::ToString() { return "Pointing to => " + Resolve(intrinsicEnv, position)->ToString(); }

SharedValue PointerValue::Resolve(Environment* _env, Position _execPos)
{
	if(value.expired()) { return SHARE(Exception_PointerExpired(Trace(_execPos, _env->name, _env->filePath))); }
	else { return SharedValue(value); }
}
#pragma endregion

#pragma region NamedspaceValue
IValue* NamedspaceValue::GetCopy() { return new NamedspaceValue(environment->GetCopy(), position); }
#pragma endregion

#pragma region FuncValue
FuncValue::FuncValue(Environment * _defEnv, std::string _name, std::vector<std::string>& _argNames, std::vector<Signature>& _argSigs, Signature _retSignature, BodyType _bodyType, Position _pos)
	: IValue(VFUNC, _pos, new Environment(_defEnv, _name))
{
	defEnvironment = _defEnv;
	name = _name;
	argNames = _argNames;
	argSigs = _argSigs;
	returnSignature = _retSignature;
	bodyType = _bodyType;
	signature = GenerateSignature(argSigs, returnSignature);
}

FuncValue::FuncValue(Environment* _defEnv, std::string _name, std::shared_ptr<IStatement> _body, std::vector<std::string>& _argNames, std::vector<Signature>& _argSigs, Signature _retSignature, Position _pos)
	: FuncValue(_defEnv, _name, _argNames, _argSigs, _retSignature, DECLARED, _pos)
{
	body = _body;
	needsReturn = body->_type == SCODE_BLOCK;
}

FuncValue::FuncValue(Environment* _defEnv, std::string _name, builtInFunc _ptr, std::vector<std::string>& _argNames, std::vector<Signature>& _argSigs, Signature _retSignature, Position _pos)
	: FuncValue(_defEnv, _name, _argNames, _argSigs, _retSignature, BUILT_IN, _pos)
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
		Signature argSig = argSigs[i];

		if(argVal->GetType() == VEXCEPTION) { return argVal; }
		else if(argVal->GetSignature() != argSig) { return SHARE(Exception_MismatchType(argSig, argVal->GetSignature(), Trace(_execPos, _execEnv->name, _execEnv->filePath))); }
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

		Signature retValSignature = retVal->GetSignature();

		if(retValSignature != returnSignature)
			return SHARE(Exception_MismatchType(returnSignature, retValSignature, trace));
	}

	return retVal;
}

IValue* FuncValue::GetCopy()
{
	if(bodyType == DECLARED) { return new FuncValue(defEnvironment, name, body, argNames, argSigs, returnSignature, position); }
	else { return new FuncValue(defEnvironment, name, pointer, argNames, argSigs, returnSignature, position); }
}

Signature FuncValue::GenerateSignature(std::vector<Signature> _argSigs, Signature _retSig)
{
	Signature signature;

	if(_argSigs.size() == 0) { signature += "(void)"; }
	else
	{
		signature += "(";

		for(auto it : _argSigs)
			signature += it + ",";

		signature.pop_back();
		signature += ")";
	}

	signature += ":" + _retSig + "#" + SIGNATURE_FUNC;
	return signature;
}
#pragma endregion

#pragma region UnkonwnValue
UnknownValue::UnknownValue(IValue * _val, Position _pos)
	: IValue(VUNKNOWN, _pos, nullptr)
{
	value = _val;

	//Casting Function
	builtInFunc body = [this](Environment* _env, Position _execPos)
	{
		if(resolved)
			return SHARE(Exception_Resolved(Trace(_execPos, _env->name, _env->filePath)));

		resolved = true;
		return SHARE(value);
	};
	AddCast(value->GetSignature(), body);
}

UnknownValue::~UnknownValue()
{
	if(!resolved)
		delete value;
}
#pragma endregion

SharedValue Value::Set(Environment* _execEnv, IValue * _val, Position _execPos)
{
	Signature orgSignature = value->GetSignature();
	Signature valSignature = _val->GetSignature();

	if(orgSignature == valSignature)
	{
		delete value;
		value = _val;
		return SHARE_VOID(_execPos);
	}
	else { return SHARE(Exception_MismatchType(orgSignature, valSignature, Trace(_execPos, _execEnv->name, _execEnv->filePath))); }
}
