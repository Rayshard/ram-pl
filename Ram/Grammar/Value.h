#pragma once

#include <map>
#include "..\Lexer.h"

class IStatement;
class Environment;
class IExpression;

enum ValueType : char { VUNKNOWN, VINT, VFLOAT, VSTRING, VBOOL, VEXCEPTION, VVOID, VMEMBERED, VFUNC, VNAMEDSPACE };

typedef std::string TypeName;
typedef std::string TypeSig;
typedef std::string TypeDef;
typedef std::pair<TypeDef, TypeSig> DefSigPair;
typedef std::map<TypeName, DefSigPair> TypeSigMap;
typedef std::vector<IExpression*> ArgumentList;
typedef std::map<std::string, IExpression*> AssignmentMap;

struct Definition
{
	std::string identifier;
	TypeName typeName;

	Definition(std::string _identifier, TypeName _typename);
	Definition();

	bool operator==(const Definition& _other);
};

typedef std::vector<Definition> DefinitionList;
typedef std::map<std::string, TypeName> DefinitionMap;

class IValue
{
protected:
	Environment* intrinsicEnv;
public:
	ValueType _type;
	Position _position;
	TypeName typeName;

	IValue(ValueType _type, Position _pos, TypeName _typeName, Environment* _intrEnv);
	virtual ~IValue();

	virtual IValue* GetCopy() = 0;
	virtual TypeSig GetTypeSig() = 0;
	virtual bool ToBool(Environment* _execEnv) = 0;
	virtual std::string ToString(Environment* _execEnv) = 0;

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
	std::string ToString(Environment* _execEnv);
	bool ToBool(Environment* _execEnv);
};

inline IValue* Exception_CompilationError(std::string _msg) { return new ExceptionValue("Compilation Error", _msg, Position(-1, -1)); }
inline IValue* Exception_SymbolNotFound(std::string _symbolName, Position _pos) { return new ExceptionValue("Symbol Not Found", "The symbol \"" + _symbolName + "\" does not exist.", _pos); }
inline IValue* Exception_SymbolInUse(std::string _symbolName, Position _pos) { return new ExceptionValue("Symbol In Use", "The symbol \"" + _symbolName + "\" is in use.", _pos); }
inline IValue* Exception_MismatchType(TypeName _org, TypeName _mismatch, Position _pos) { return new ExceptionValue("Mismatch Type", "Cannot assign " + _mismatch + " to variable of type " + _org, _pos); }
inline IValue* Exception_InvalidOperation(std::string _msg, Position _pos) { return new ExceptionValue("Invalid Operation", _msg, _pos); }
inline IValue* Exception_DivisionByZero(Position _pos) { return new ExceptionValue("Division By Zero", "Cannot divide by 0.", _pos); }
inline IValue* Exception_WrongNumArgs(int _org, int _passed, Position _pos) { return new ExceptionValue("Wrong Number of Arguments", std::to_string(_org) + " arguments(s) were expected but " + std::to_string(_passed) + " were given.", _pos); }

class VoidValue : public IValue
{
private:
	TypeSig typeSig;

	VoidValue(TypeName _typeName, TypeSig _typeSig, Position _pos);
public:
	VoidValue(Position _pos);

	IValue* GetCopy();
	TypeSig GetTypeSig();
	bool ToBool(Environment* _execEnv);
	std::string ToString(Environment* _execEnv);
	
	static IValue* FromTypeName(TypeName _typeName, Environment* _execEnv, Position _pos);
};

class MemberedValue : public IValue
{
public:
	static std::string TYPESIG_CAST_BOOL;
	static std::string TYPESIG_CAST_STRING;

	DefinitionMap memDefinitions;

private:
	MemberedValue(TypeName _typeName, DefinitionMap& _memDefs, Environment* _intrEnv, Position _pos);
public:
	IValue* GetCopy();
	TypeSig GetTypeSig();
	IValue* GetMemberValue(std::string _memId, Position _execPos, bool _retCopy);
	std::string ToString(Environment* _execEnv);
	bool ToBool(Environment* _execEnv);
	bool HasMember(std::string _name, TypeName _typeName);

	static IValue* Create(Environment* _creationEnv, AssignmentMap& _memAssigns, Position _pos);
};

DefinitionList GetMemberDefs(ValueMap _mems);
TypeSig GetMemberedTypeDef(DefinitionMap& _memDefs);
IValue* GetMemberedTypeSig(Environment* _execEnv, DefinitionMap& _memDefs, Position _execPos);

class NamedspaceValue : public IValue
{
public:
	Environment* environment;

	NamedspaceValue(Environment* _env, Position _pos);
	~NamedspaceValue();

	IValue* GetCopy();
	TypeSig GetTypeSig();
	std::string ToString(Environment* _execEnv);
	bool ToBool(Environment* _execEnv);
};

class FuncValue : public IValue
{
public:
	IStatement* body;
	DefinitionList argDefs;
	TypeName returnTypename;
	Environment* defEnvironment;

	FuncValue(Environment* _defEnv, IStatement* _statement, DefinitionList& _argDefs, TypeName _retTypeName, Position _pos);
	~FuncValue();

	IValue* Call(Environment* _execEnv, ArgumentList& _argExprs, Position _execPos);
	IValue* GetCopy();
	TypeSig GetTypeSig();
	std::string ToString(Environment* _execEnv);
	bool ToBool(Environment* _execEnv);
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
	bool ToBool(Environment* _execEnv);
	std::string ToString(Environment* _execEnv);
};

typedef PrimitiveValue<int> IntValue;
typedef PrimitiveValue<float> FloatValue;
typedef PrimitiveValue<std::string> StringValue;
typedef PrimitiveValue<bool> BoolValue;

template<typename T>
inline PrimitiveValue<T>::PrimitiveValue(T _value, Position _pos)
	: IValue(VUNKNOWN, _pos, "", 0)
{
	value = _value;

	if(std::is_same<T, int>()) { _type = VINT; typeName = "int"; }
	else if(std::is_same<T, float>()) { _type = VFLOAT; typeName = "float"; }
	else if(std::is_same<T, std::string>()) { _type = VSTRING; typeName = "string"; }
	else if(std::is_same<T, bool>()) { _type = VBOOL; typeName = "bool"; }
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
inline bool PrimitiveValue<T>::ToBool(Environment* _execEnv)
{
	switch(_type)
	{
		case VINT: return ((IntValue*)this)->value != 0;
		case VFLOAT: return ((FloatValue*)this)->value != 0;
		case VSTRING: return !((StringValue*)this)->value.empty();
		case VBOOL: return ((BoolValue*)this)->value;
		default: throw std::runtime_error("Missing Case in PrimitiveValue.ToBool()");
	}
}

template<typename T>
inline std::string PrimitiveValue<T>::ToString(Environment* _execEnv)
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
