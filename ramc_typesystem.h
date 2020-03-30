#pragma once
#include "result.h"
#include "ramvm_instruction.h"

using ramvm::Argument;
using ramvm::ArgType;

namespace ramc {
	class Type;
	typedef std::vector<Type*> TypeList;

	enum class TypeSystemType {
		VOID, BOOL, BYTE, INT, FLOAT, DOUBLE, LONG, STRING,
		TUPLE, FUNC, UNIT
	};

	class Type {
	private:
		TypeSystemType type;
	protected:
		Type(TypeSystemType _type) : type(_type) { }
	public:
		static Type* BOOL() { return new Type(TypeSystemType::BOOL); }
		static Type* BYTE() { return new Type(TypeSystemType::BYTE); }
		static Type* INT() { return new Type(TypeSystemType::INT); }
		static Type* FLOAT() { return new Type(TypeSystemType::FLOAT); }
		static Type* DOUBLE() { return new Type(TypeSystemType::DOUBLE); }
		static Type* LONG() { return new Type(TypeSystemType::LONG); }
		static Type* STRING() { return new Type(TypeSystemType::STRING); }
		static Type* VOID() { return new Type(TypeSystemType::VOID); }
		static Type* UNIT() { return new Type(TypeSystemType::UNIT); }

		virtual std::string ToString(int _indentLvl);
		virtual int GetByteSize();
		virtual Type* GetCopy();
		virtual bool Matches(Type* _t) { return type == _t->type; }

		TypeSystemType GetType() { return type; }

		static bool Matches(Type* _t1, Type* _t2) { return _t1->Matches(_t2) && _t2->Matches(_t1); }
	};

#pragma region TupleType
	class TupleType : public Type {
		TypeList types;
	public:
		TupleType(TypeList _types);
		~TupleType();

		std::string ToString(int _indentLvl) override;
		int GetByteSize() override;
		bool Matches(Type* _t) override;
		Type* GetCopy() override;
	};
#pragma endregion

#pragma region FuncType
	class FuncType : public Type {
		Type* params, * ret;
	public:
		FuncType(Type* _params, Type* _ret);
		~FuncType();

		Type* GetParamsType() { return params; }
		Type* GetRetType() { return ret; }
		bool IsProcedure() { return ret->GetType() == TypeSystemType::UNIT; }

		std::string ToString(int _indentLvl) override;
		int GetByteSize() override;
		bool Matches(Type* _t) override;
		Type* GetCopy() override;
	};
#pragma endregion

#pragma region TypeResult
	enum class TypeResultType {
		SUCCESS,
		ID_NOT_FOUND,
		MISMATCH,
		REDEFINITION,
		EXPECTATION,
		CODE_PATH_LACKS_RET,
		AMBIGUOUS_FUNC_DECL,
		FUNC_ID_PARAMS_TYPE_PAIR_NOT_FOUND
	};

	class TypeResult : public Result<TypeResultType, std::shared_ptr<Type*>> {
		TypeResult(TypeResultType _type, Type* _val, std::string _errStr, Position _errPos)
			: Result(_type, std::make_shared<Type*>(_val), _errStr, _errPos) { }
	public:
		std::shared_ptr<Type*> GetValue() override { return IsSuccess() ? value : nullptr; }
		std::string ToString(bool _includePos) override;

		static TypeResult GenSuccess(Type* _type) { return TypeResult(TypeResultType::SUCCESS, _type, "", Position()); }
		static TypeResult GenIDNotFound(std::string _id, Position _pos) { return TypeResult(TypeResultType::ID_NOT_FOUND, nullptr, _id, _pos); }
		static TypeResult GenExpectation(std::string _expected, std::string _found, Position _pos) { return TypeResult(TypeResultType::EXPECTATION, nullptr, "Expected " + _expected + " but found " + _found, _pos); }
		static TypeResult GenMismatch(std::string _mistach, Position _pos) { return TypeResult(TypeResultType::MISMATCH, nullptr, _mistach, _pos); }
		static TypeResult GenRedefinition(std::string _id, Position _pos) { return TypeResult(TypeResultType::REDEFINITION, nullptr, _id, _pos); }
		static TypeResult GenCodePathLacksReturn(std::string _funcName, Position _pos) { return TypeResult(TypeResultType::CODE_PATH_LACKS_RET, nullptr, _funcName, _pos); }
		static TypeResult GenAmbiguousFuncDecl(std::string _funcName, Type* _type, Position _pos) { return TypeResult(TypeResultType::AMBIGUOUS_FUNC_DECL, nullptr, _funcName + ": " + _type->ToString(0), _pos); }
		static TypeResult GenFuncIDParamsTypePairNotFound(std::string _funcName, Type* _paramsType, Position _pos) { return TypeResult(TypeResultType::FUNC_ID_PARAMS_TYPE_PAIR_NOT_FOUND, nullptr, "\"" + _funcName + "\"" + " with param(s) " + _paramsType->ToString(0), _pos); }
	};
#pragma endregion

	class Environment {
		struct VarInfo { Type* type = nullptr; Argument source; };
		struct FuncInfo { std::string label; Type* type = nullptr; int regCnt = 0; };

		Environment* parent;
		std::unordered_map<std::string, FuncInfo> functions;
		std::unordered_map<std::string, VarInfo> variables;
		std::unordered_map<Environment*, bool> subEnvs;
		int nextRegIdx, numRegNeeded, nextGlobalStackPos;

		void SetMaxNumVarRegNeeded(int _val);
	public:
		Environment(Environment* _parent, bool _incrParentRegCnt);
		~Environment();

		TypeResult AddVariable(std::string _id, Type* _type, Argument _source, Position _execPos);
		bool HasVariable(std::string _id, bool _localCheck);
		bool HasVariable(std::string _id, Type* _type, bool _localCheck);
		TypeResult GetVariableType(std::string _id, Position _execPos);
		Argument GetVarSource(std::string _id);

		TypeResult AddFunction(std::string _id, Type* _type, std::string& _label, Position _execPos);
		TypeResult GetFunctionRetType(std::string _id, Type* _paramsType, Position _execPos);

		int GetNumRegNeeded() { return numRegNeeded; }
		int GetNextRegIdx() { return nextRegIdx++; }
		int GetNextGlobalStackPos(int _byteLen) { nextGlobalStackPos += _byteLen;  return nextGlobalStackPos - _byteLen; }

		static std::string GenFuncLabel(std::string _name, Type* _paramsType)
		{
			std::string postfix = StrReplace(_paramsType->ToString(0), "(,)", '_');
			return "%FUNC_" + _name + "_" + postfix;
		}
	};

	constexpr DataType TypeSysTypeToDataType(TypeSystemType _type)
	{
		switch (_type)
		{
		case TypeSystemType::BYTE: return DataType::BYTE;
		case TypeSystemType::INT: return DataType::INT;
		case TypeSystemType::FLOAT: return DataType::FLOAT;
		case TypeSystemType::BOOL: return DataType::BYTE;
		case TypeSystemType::DOUBLE: return DataType::DOUBLE;
		case TypeSystemType::LONG: return DataType::LONG;
		default: return DataType::UNKNOWN;
		}
	}
}