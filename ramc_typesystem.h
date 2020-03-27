#pragma once
#include "result.h"
#include "ramvm_instruction.h"

using ramvm::Argument;
using ramvm::ArgType;

namespace ramc {
	enum class TypeSystemType {
		VOID, BOOL, BYTE, INT, FLOAT, DOUBLE, LONG, STRING
	};

	class Type {
	private:
		TypeSystemType type;
	protected:
		Type(TypeSystemType _type) : type(_type) { }
	public:
		static Type* INT, * FLOAT, * STRING, * BOOL, * VOID, * DOUBLE, * LONG,
			* BYTE;

		virtual std::string ToString(int _indentLvl);

		TypeSystemType GetType() { return type; }
		template<typename T> T* As() { return dynamic_cast<T*>(this); }
	};

	enum class TypeResultType {
		SUCCESS,
		ID_NOT_FOUND,
		MISMATCH,
		REDECLARATION,
		EXPECTATION
	};

	class TypeResult : public Result<TypeResultType, Type*> {
		TypeResult(TypeResultType _type, Type* _val, std::string _errStr, Position _errPos)
			: Result(_type, _val, _errStr, _errPos) { }
	public:
		Type* GetValue() override { return IsSuccess() ? value : nullptr; }
		std::string ToString(bool _includePos) override;

		static TypeResult GenSuccess(Type* _type) { return TypeResult(TypeResultType::SUCCESS, _type, "", Position()); }
		static TypeResult GenIDNotFound(std::string _id, Position _pos) { return TypeResult(TypeResultType::ID_NOT_FOUND, nullptr, _id, _pos); }
		static TypeResult GenExpectation(std::string _expected, std::string _found, Position _pos) { return TypeResult(TypeResultType::EXPECTATION, nullptr, "Expected " + _expected + " but found " + _found, _pos); }
		static TypeResult GenMismatch(std::string _mistach, Position _pos) { return TypeResult(TypeResultType::MISMATCH, nullptr, _mistach, _pos); }
		static TypeResult GenRedecalartion(std::string _id, Position _pos) { return TypeResult(TypeResultType::REDECLARATION, nullptr, _id, _pos); }
	};

	class Environment {
		struct VarInfo
		{
			Type* type;
			int regIdx;
		};

		Environment* parent;
		std::unordered_map<std::string, VarInfo> variables;
		int nextVarRegIdx;
	public:
		Environment(Environment* _parent);

		bool AddVariable(std::string _id, Type* _type);
		bool HasVariable(std::string _id);
		bool HasVariable(std::string _id, Type* _type);
		TypeResult GetVariableType(std::string _id, Position _execPos);
		Argument GetVarRegister(std::string _id);
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