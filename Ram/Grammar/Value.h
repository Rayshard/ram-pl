#pragma once

#include <map>
#include <functional>
#include "..\Lexer.h"

class IStatement;
class Environment;
class IExpression;

enum ValueType : char { VUNKNOWN, VINT, VFLOAT, VSTRING, VBOOL, VEXCEPTION, VVOID, VMEMBERED, VFUNC, VNAMEDSPACE };

typedef std::string TypeName, TypeSig;
typedef std::map<TypeName, TypeSig> TypeSigMap;
typedef std::vector<IExpression*> ArgumentList;
typedef std::map<std::string, IExpression*> AssignmentMap;
typedef std::pair<std::string, std::string> Definition; // Either Identifier->TypeDef or Identifier->TypeSig
typedef std::vector<Definition> DefinitionList;
typedef std::map<std::string, TypeName> DefinitionMap;

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

typedef std::map<std::string, IValue*> ValueMap;

class ExceptionValue : public IValue
{
public:
	std::string name;
	std::string message;

	ExceptionValue(std::string _name, std::string _msg, Position _pos);
	ExceptionValue(Position _pos);

	IValue* GetCopy();
	TypeSig GetTypeSig();
	std::string ToString();
};

inline IValue* Exception_CompilationError(std::string _msg, Position _pos) { return new ExceptionValue("Compilation Error", _msg, _pos); }
inline IValue* Exception_SymbolNotFound(std::string _symbolName, Position _pos) { return new ExceptionValue("Symbol Not Found", "The symbol \"" + _symbolName + "\" does not exist.", _pos); }
inline IValue* Exception_SymbolInUse(std::string _symbolName, Position _pos) { return new ExceptionValue("Symbol In Use", "The symbol \"" + _symbolName + "\" is in use.", _pos); }
inline IValue* Exception_MismatchType(TypeName _org, TypeName _mismatch, Position _pos) { return new ExceptionValue("Mismatch Type", "Cannot assign " + _mismatch + " to variable of type " + _org, _pos); }
inline IValue* Exception_InvalidOperation(std::string _msg, Position _pos) { return new ExceptionValue("Invalid Operation", _msg, _pos); }
inline IValue* Exception_DivisionByZero(Position _pos) { return new ExceptionValue("Division By Zero", "Cannot divide by 0.", _pos); }
inline IValue* Exception_WrongNumArgs(int _org, int _passed, Position _pos) { return new ExceptionValue("Wrong Number of Arguments", std::to_string(_org) + " arguments(s) were expected but " + std::to_string(_passed) + " were given.", _pos); }

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
	static std::string TYPESIG_CAST_BOOL;
	static std::string TYPESIG_CAST_STRING;

	DefinitionMap memDefinitions;
	TypeSig typeSig;
private:
	MemberedValue(DefinitionMap& _memDefs, TypeSig _typeSig, Environment* _intrEnv, Position _pos);
public:
	IValue* GetCopy();
	TypeSig GetTypeSig();
	std::string ToString();
	IValue* GetMemberValue(std::string _memId, Position _execPos, bool _retCopy);
	bool HasMember(std::string _name, TypeName _typeName);

	static IValue* Create(Environment* _creationEnv, AssignmentMap& _memAssigns, Position _pos);
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
	typedef std::function<IValue*(Environment*, Position)> built_in;

	IStatement* body;
	built_in pointer;
	BodyType bodyType;

	std::vector<std::string> argNames;
	std::vector<TypeSig> argSigs;
	TypeSig returnTypeSig;
	TypeSig typeSig;

	Environment* defEnvironment;

	FuncValue(Environment* _defEnv, IStatement* _body, std::vector<std::string>& _argNames, std::vector<TypeSig>& _argSigs, TypeSig _retTypeSig, Position _pos);
	FuncValue(Environment* _defEnv, built_in _ptr, std::vector<std::string>& _argNames, std::vector<TypeSig>& _argSigs, TypeSig _retTypeSig, Position _pos);
	~FuncValue();

	IValue* Call(Environment* _execEnv, ArgumentList& _argExprs, Position _execPos);
	IValue* GetCopy();
	TypeSig GetTypeSig();
	std::string ToString();
};

TypeSig GetFuncTypeSig(DefinitionList& _argDefs, TypeName _retTypeName);

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
