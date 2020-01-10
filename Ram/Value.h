#pragma once

class IExpression;
class Environment;
struct Position;
class IStatement;

enum ValueType : char { VUNKNOWN, VINT, VFLOAT, VSTRING, VBOOL, VEXCEPTION, VVOID, VMEMBERED, VFUNC, VNAMEDSPACE };

class IValue
{
protected:
	Environment* intrinsicEnv;
public:
	ValueType _type;
	Position _position;

	IValue(ValueType _type, Position _pos, Environment* _intrEnv);
	virtual ~IValue();

	virtual IValue* GetCopy() = 0;
	virtual TypeSig GetTypeSig() = 0;
	virtual std::string ToString() = 0;

	Environment* GetIntrinsicEnv();
};

class ExceptionValue : public IValue
{
private:
	ExceptionValue(std::string _name, std::string _msg, StackTrace& _stackTrace);
public:
	std::string name;
	std::string message;
	StackTrace stackTrace;

	ExceptionValue(std::string _name, std::string _msg, Trace _trace);

	IValue* GetCopy();
	TypeSig GetTypeSig();
	void ExtendStackTrace(Trace _trace);
	std::string ToString();
};

inline IValue* Exception_CompilationError(std::string _msg, Trace _trace) { return new ExceptionValue("Compilation Error", _msg, _trace); }
inline IValue* Exception_SymbolNotFound(std::string _symbolName, Trace _trace) { return new ExceptionValue("Symbol Not Found", "The symbol \"" + _symbolName + "\" does not exist.", _trace); }
inline IValue* Exception_SymbolInUse(std::string _symbolName, Trace _trace) { return new ExceptionValue("Symbol In Use", "The symbol \"" + _symbolName + "\" is in use.", _trace); }
inline IValue* Exception_MismatchType(TypeName _org, TypeName _mismatch, Trace _trace) { return new ExceptionValue("Mismatch Type", "Cannot assign " + _mismatch + " to variable of type " + _org, _trace); }
inline IValue* Exception_InvalidOperation(std::string _msg, Trace _trace) { return new ExceptionValue("Invalid Operation", _msg, _trace); }
inline IValue* Exception_DivisionByZero(Trace _trace) { return new ExceptionValue("Division By Zero", "Cannot divide by 0.", _trace); }
inline IValue* Exception_WrongNumArgs(int _org, int _passed, Trace _trace) { return new ExceptionValue("Wrong Number of Arguments", std::to_string(_org) + " arguments(s) were expected but " + std::to_string(_passed) + " were given.", _trace); }
inline IValue* Exception_MissingReturn(Trace _trace) { return new ExceptionValue("Missing Return", "A return statement is needed.", _trace); }

class VoidValue : public IValue
{
public:
	VoidValue(Position _pos);

	IValue* GetCopy();
	TypeSig GetTypeSig();
	std::string ToString();
};

class MemberedValue : public IValue
{
public:
	DefinitionMap memDefinitions;
	TypeSig typeSig;

public:
	MemberedValue(DefinitionMap& _memDefs, TypeSig _typeSig, Environment* _intrEnv, Position _pos);

	IValue* GetCopy();
	TypeSig GetTypeSig();
	std::string ToString();
	SharedValue GetMemberValue(std::string _memId, Position _execPos);
};

class NamedspaceValue : public IValue
{
public:
	Environment* environment;

	NamedspaceValue(Environment* _env, Position _pos);
	~NamedspaceValue();

	IValue* GetCopy();
	TypeSig GetTypeSig();
	std::string ToString();
};

class FuncValue : public IValue
{
private:
	enum BodyType { BUILT_IN, DECLARED };

	FuncValue(Environment* _defEnv, std::vector<std::string>& _argNames, std::vector<TypeSig>& _argSigs, TypeName _retTypeName, BodyType _bodyType, Position _pos);
public:
	typedef std::function<SharedValue(Environment*, Position)> built_in;

	std::shared_ptr<IStatement> body;
	built_in pointer;
	BodyType bodyType;
	bool needsReturn;

	std::vector<std::string> argNames;
	std::vector<TypeSig> argSigs;
	TypeSig returnTypeSig;
	TypeSig typeSig;

	Environment* defEnvironment;

	FuncValue(Environment* _defEnv, std::shared_ptr<IStatement> _body, std::vector<std::string>& _argNames, std::vector<TypeSig>& _argSigs, TypeSig _retTypeSig, Position _pos);
	FuncValue(Environment* _defEnv, built_in _ptr, std::vector<std::string>& _argNames, std::vector<TypeSig>& _argSigs, TypeSig _retTypeSig, Position _pos);

	SharedValue Call(Environment* _execEnv, ArgumentList& _argExprs, Position _execPos);
	IValue* GetCopy();
	TypeSig GetTypeSig();
	std::string ToString();
};

template <typename T>
class PrimitiveValue : public IValue
{
public:
	T value;

	PrimitiveValue(T _value, Position _pos);

	IValue* GetCopy();
	TypeSig GetTypeSig();
	std::string ToString();
};

typedef PrimitiveValue<int> IntValue;
typedef PrimitiveValue<float> FloatValue;
typedef PrimitiveValue<std::string> StringValue;
typedef PrimitiveValue<bool> BoolValue;

template<typename T>
inline PrimitiveValue<T>::PrimitiveValue(T _value, Position _pos)
	: IValue(VUNKNOWN, _pos, 0)
{
	value = _value;

	if(std::is_same<T, int>()) { _type = VINT; }
	else if(std::is_same<T, float>()) { _type = VFLOAT; }
	else if(std::is_same<T, std::string>()) { _type = VSTRING; }
	else if(std::is_same<T, bool>()) { _type = VBOOL; }
}

template<typename T>
inline IValue * PrimitiveValue<T>::GetCopy() { return new PrimitiveValue(value, _position); }

template<typename T>
inline TypeSig PrimitiveValue<T>::GetTypeSig()
{
	switch(_type)
	{
		case VINT: return "<INT>";
		case VFLOAT: return "<FLOAT>";
		case VSTRING: return "<STRING>";
		case VBOOL: return "<BOOL>";
		default: throw std::runtime_error("Missing Case in PrimitiveValue.ToString()");
	}
}

template<typename T>
inline std::string PrimitiveValue<T>::ToString()
{
	switch(_type)
	{
		case VINT: return std::to_string(((IntValue*)this)->value);
		case VFLOAT: return std::to_string(((FloatValue*)this)->value);
		case VSTRING: return ((StringValue*)this)->value;
		case VBOOL: return ((BoolValue*)this)->value ? "true" : "false";
		default: throw std::runtime_error("Missing Case in PrimitiveValue.ToString()");
	}
}

class Value
{
public:
	IValue* value;

	Value(IValue* _val) { value = _val; }
	~Value() { delete value; }

	SharedValue Set(Environment* _execEnv, IValue * _val, Position _execPos);

	ValueType GetType() { return value->_type; }
	Position GetPosition() { return value->_position; }
	IValue* GetCopy() { return value->GetCopy(); }
	TypeSig GetTypeSig() { return value->GetTypeSig(); }
	std::string ToString() { return value->ToString(); }
	Environment* GetIntrinsicEnv() { return value->GetIntrinsicEnv(); }

	ExceptionValue* AsException() { return (ExceptionValue*)value; }
	VoidValue* AsVoid() { return (VoidValue*)value; }
	MemberedValue* AsMembered() { return (MemberedValue*)value; }
	NamedspaceValue* AsNamedspace() { return (NamedspaceValue*)value; }
	FuncValue* AsFunc() { return (FuncValue*)value; }
	IntValue* AsInt() { return (IntValue*)value; }
	FloatValue* AsFloat() { return (FloatValue*)value; }
	StringValue* AsString() { return (StringValue*)value; }
	BoolValue* AsBool() { return (BoolValue*)value; }
};