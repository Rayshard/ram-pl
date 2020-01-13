#pragma once

class IExpression;
class Environment;
struct Position;
class IStatement;

enum ValueType : char {
	VUNKNOWN, VINT, VFLOAT, VSTRING, VBOOL, VEXCEPTION,
	VVOID, VMEMBERED, VFUNC, VNAMEDSPACE,

	VARRAY, VPOINTER
};

class IValue
{
protected:
	Environment* intrinsicEnv;
	std::map<Signature, builtInFunc> castingFuncs; //Return Val Signature to Function
public:
#pragma region Signatures
	static const std::string SIGNATURE_VOID;
	static const std::string SIGNATURE_UNKNOWN;
	static const std::string SIGNATURE_EXCEPTION;
	static const std::string SIGNATURE_MEMBERED;
	static const std::string SIGNATURE_ARRAY;
	static const std::string SIGNATURE_FUNC;
	static const std::string SIGNATURE_NAMEDSPACE;
	static const std::string SIGNATURE_POINTER;
	static const std::string SIGNATURE_INT;
	static const std::string SIGNATURE_FLOAT;
	static const std::string SIGNATURE_BOOL;
	static const std::string SIGNATURE_STRING;
#pragma endregion

	ValueType type;
	Position position;

	IValue(ValueType _type, Position _pos, Environment* _intrEnv);
	virtual ~IValue();

	virtual IValue* GetCopy() = 0;
	virtual Signature GetSignature() = 0;
	virtual std::string ToString() = 0;

	Environment* GetIntrinsicEnv();
	SharedValue Cast(Environment* _execEnv, Signature _retSig, Position _execPos);

	void AddCast(Signature _retSig, builtInFunc _func) { castingFuncs.insert_or_assign(_retSig, _func); }
};

#pragma region VoidValue
class VoidValue : public IValue
{
public:
	VoidValue(Position _pos) : IValue(VVOID, _pos, nullptr) { }
	IValue* GetCopy() { return new VoidValue(position); }
	Signature GetSignature() { return SIGNATURE_VOID; }
	std::string ToString() { return "void"; }
};
#pragma endregion

#pragma region UnknownValue
class UnknownValue : public IValue
{
public:
	IValue* value;
	bool resolved;

	UnknownValue(IValue* _val, Position _pos);
	~UnknownValue();

	IValue* GetCopy() { return new UnknownValue(value->GetCopy(), position); }
	Signature GetSignature() { return SIGNATURE_UNKNOWN; }
	std::string ToString() { return SIGNATURE_UNKNOWN; }
};
#pragma endregion

#pragma region ExceptionValue
class ExceptionValue : public IValue
{
private:
	ExceptionValue(std::string _name, std::string _msg, StackTrace& _stackTrace);
public:
	std::string name;
	std::string message;
	StackTrace stackTrace;

	ExceptionValue(std::string _name, std::string _msg, Trace _trace);

	std::string ToString();

	IValue* GetCopy() { return new ExceptionValue(name, message, stackTrace); }
	Signature GetSignature() { return SIGNATURE_EXCEPTION; }
	void ExtendStackTrace(Trace _trace) { stackTrace.push_back(_trace); }
};

#pragma region Exceptions
inline IValue* Exception_CompilationError(std::string _msg, Trace _trace) { return new ExceptionValue("Compilation Error", _msg, _trace); }
inline IValue* Exception_File(std::string _msg, Trace _trace) { return new ExceptionValue("File Error", _msg, _trace); }
inline IValue* Exception_SymbolNotFound(std::string _symbolName, Trace _trace) { return new ExceptionValue("Symbol Not Found", "The symbol \"" + _symbolName + "\" does not exist.", _trace); }
inline IValue* Exception_SymbolInUse(std::string _symbolName, Trace _trace) { return new ExceptionValue("Symbol In Use", "The symbol \"" + _symbolName + "\" is in use.", _trace); }
inline IValue* Exception_MismatchType(TypeName _org, TypeName _mismatch, Trace _trace) { return new ExceptionValue("Mismatch Type", "Cannot assign " + _mismatch + " to variable of type " + _org, _trace); }
inline IValue* Exception_InvalidOperation(std::string _msg, Trace _trace) { return new ExceptionValue("Invalid Operation", _msg, _trace); }
inline IValue* Exception_DivisionByZero(Trace _trace) { return new ExceptionValue("Division By Zero", "Cannot divide by 0.", _trace); }
inline IValue* Exception_WrongNumArgs(int _org, int _passed, Trace _trace) { return new ExceptionValue("Wrong Number of Arguments", std::to_string(_org) + " arguments(s) were expected but " + std::to_string(_passed) + " were given.", _trace); }
inline IValue* Exception_MissingReturn(Trace _trace) { return new ExceptionValue("Missing Return", "A return statement is needed.", _trace); }
inline IValue* Exception_Format(std::string _msg, Trace _trace) { return new ExceptionValue("Invalid Format", _msg, _trace); }
inline IValue* Exception_OutOfRange(std::string _identifier, Trace _trace) { return new ExceptionValue("Out of Range", "'" + _identifier + "' is out of range!", _trace); }
inline IValue* Exception_InvailArgument(std::string _argName, std::string _msg, Trace _trace) { return new ExceptionValue("Invalid Argument", "'" + _argName + "' : " + _msg, _trace); }
inline IValue* Exception_DLLLoadFail(std::string _dllPath, Trace _trace) { return new ExceptionValue("DLL Load Fail", "Could not load dll at " + _dllPath, _trace); }
inline IValue* Exception_PointerExpired(Trace _trace) { return new ExceptionValue("Pointer Expired", "The value to which the pointer pointed no longer exists!", _trace); }
inline IValue* Exception_NotResolved(Trace _trace) { return new ExceptionValue("Not Resolved", "This unknown value must be resolved before use. Try casting it first.", _trace); }
inline IValue* Exception_Resolved(Trace _trace) { return new ExceptionValue("Resolved", "This unknown value has already been resolved. You cannot reuse it.", _trace); }
inline IValue* Exception_CastNotFound(Signature _retSig, Trace _trace) { return new ExceptionValue("Cast Not Found", "This value cannot be cast to '" + _retSig + "'.", _trace); }
#pragma endregion
#pragma endregion

#pragma region MemberedValue
class MemberedValue : public IValue
{
public:
	DefinitionMap memDefinitions;
	Signature signature;

	MemberedValue(DefinitionMap& _memDefs, Signature _sig, Environment* _intrEnv, Position _pos);

	IValue* GetCopy();
	std::string ToString();
	SharedValue GetMemberValue(std::string _memId, Position _execPos);

	Signature GetSignature() { return signature; }
};
#pragma endregion

#pragma region ArrayValue
class ArrayValue : public IValue
{
public:
	std::vector<SharedValue> elements;
	Signature elemSignature;

	ArrayValue(Signature _elemSignature, std::vector<SharedValue>& _initElems, Position _pos);
	~ArrayValue() { elements.clear(); }

	IValue* GetCopy();
	std::string ToString();

	Signature GetSignature() { return elemSignature + "|" + SIGNATURE_ARRAY; }
};
#pragma endregion

#pragma region PointerValue
class PointerValue : public IValue
{
public:
	ObservedValue value;

	PointerValue(ObservedValue _val, Position _pos) : IValue(VPOINTER, _pos, nullptr) { value = _val; }

	IValue* GetCopy() { return new PointerValue(value, position); }
	Signature GetSignature() { return SIGNATURE_POINTER; }

	std::string ToString();
	SharedValue Resolve(Environment* _env, Position _execPos);
};
#pragma endregion

#pragma region NamespacedValue
class NamedspaceValue : public IValue
{
public:
	Environment* environment;

	NamedspaceValue(Environment* _env, Position _pos) : IValue(VNAMEDSPACE, _pos, _env) { environment = intrinsicEnv; }

	Signature GetSignature() { return SIGNATURE_NAMEDSPACE; }
	std::string ToString() { return "namedspace"; }

	IValue* GetCopy();
};
#pragma endregion

#pragma region FuncValue
class FuncValue : public IValue
{
private:
	enum BodyType { BUILT_IN, DECLARED };

	FuncValue(Environment* _defEnv, std::string _name, std::vector<std::string>& _argNames, std::vector<Signature>& _argSigs, TypeName _retTypeName, BodyType _bodyType, Position _pos);
public:
	std::shared_ptr<IStatement> body;
	builtInFunc pointer;
	BodyType bodyType;
	bool needsReturn;
	std::string name;

	std::vector<std::string> argNames;
	std::vector<Signature> argSigs;
	Signature returnSignature;
	Signature signature;

	Environment* defEnvironment;

	FuncValue(Environment* _defEnv, std::string _name, std::shared_ptr<IStatement> _body, std::vector<std::string>& _argNames, std::vector<Signature>& _argSigs, Signature _retSignature, Position _pos);
	FuncValue(Environment* _defEnv, std::string _name, builtInFunc _ptr, std::vector<std::string>& _argNames, std::vector<Signature>& _argSigs, Signature _retSignature, Position _pos);

	SharedValue Call(Environment* _execEnv, ArgumentList& _argExprs, Position _execPos);
	IValue* GetCopy();

	Signature GetSignature() { return signature; }
	std::string ToString() { return GetSignature(); }

	static Signature GenerateSignature(std::vector<Signature> _argSigs, Signature _retSig);
};
#pragma endregion

#pragma region PrimitiveValue
template <typename T>
class PrimitiveValue : public IValue
{
public:
	T value;

	PrimitiveValue(T _value, Position _pos);

	Signature GetSignature();
	std::string ToString();

	IValue* GetCopy() { return new PrimitiveValue(value, position); }
};

typedef PrimitiveValue<int> IntValue;
typedef PrimitiveValue<float> FloatValue;
typedef PrimitiveValue<std::string> StringValue;
typedef PrimitiveValue<bool> BoolValue;

template<typename T>
inline PrimitiveValue<T>::PrimitiveValue(T _value, Position _pos)
	: IValue(VUNKNOWN, _pos, nullptr)
{
	value = _value;

	if(std::is_same<T, int>()) { type = VINT; }
	else if(std::is_same<T, float>()) { type = VFLOAT; }
	else if(std::is_same<T, std::string>()) { type = VSTRING; }
	else if(std::is_same<T, bool>()) { type = VBOOL; }
}

template<typename T>
inline Signature PrimitiveValue<T>::GetSignature()
{
	switch(type)
	{
		case VINT: return SIGNATURE_INT;
		case VFLOAT: return SIGNATURE_FLOAT;
		case VSTRING: return SIGNATURE_STRING;
		case VBOOL: return SIGNATURE_BOOL;
		default: throw std::runtime_error("Missing Case in PrimitiveValue.ToString()");
	}
}

template<typename T>
inline std::string PrimitiveValue<T>::ToString()
{
	switch(type)
	{
		case VINT: return std::to_string(((IntValue*)this)->value);
		case VFLOAT: return std::to_string(((FloatValue*)this)->value);
		case VSTRING: return ((StringValue*)this)->value;
		case VBOOL: return ((BoolValue*)this)->value ? "true" : "false";
		default: throw std::runtime_error("Missing Case in PrimitiveValue.ToString()");
	}
}
#pragma endregion

class Value
{
public:
	IValue* value;

	Value(IValue* _val) { value = _val; }
	~Value() { delete value; }

	SharedValue Set(Environment* _execEnv, IValue * _val, Position _execPos);

	ValueType GetType() { return value->type; }
	Position GetPosition() { return value->position; }
	IValue* GetCopy() { return value->GetCopy(); }
	Signature GetSignature() { return value->GetSignature(); }
	std::string ToString() { return value->ToString(); }
	Environment* GetIntrinsicEnv() { return value->GetIntrinsicEnv(); }
	SharedValue Cast(Environment* _execEnv, Signature _retSig, Position _execPos) { return value->Cast(_execEnv, _retSig, _execPos); }

	ExceptionValue* AsException() { return (ExceptionValue*)value; }
	VoidValue* AsVoid() { return (VoidValue*)value; }
	MemberedValue* AsMembered() { return (MemberedValue*)value; }
	NamedspaceValue* AsNamedspace() { return (NamedspaceValue*)value; }
	FuncValue* AsFunc() { return (FuncValue*)value; }
	IntValue* AsInt() { return (IntValue*)value; }
	FloatValue* AsFloat() { return (FloatValue*)value; }
	StringValue* AsString() { return (StringValue*)value; }
	BoolValue* AsBool() { return (BoolValue*)value; }
	ArrayValue* AsArray() { return (ArrayValue*)value; }
};
