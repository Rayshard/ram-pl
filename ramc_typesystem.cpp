#include "pch.h"
#include "ramc_typesystem.h"
#include "ramvm_instruction.h"

using ramvm::Argument;
using ramvm::ArgType;

namespace ramc {
	std::string Type::ToString(int _indentLvl)
	{
		switch (type)
		{
			case TypeSystemType::BYTE: return "BYTE";
			case TypeSystemType::INT: return "INT";
			case TypeSystemType::FLOAT: return "FLOAT";
			case TypeSystemType::STRING: return "STRING";
			case TypeSystemType::BOOL: return "BOOL";
			case TypeSystemType::DOUBLE: return "DOUBLE";
			case TypeSystemType::LONG: return "LONG";
			case TypeSystemType::UNIT: return "UNIT";
			default: return "Type::ToString - Type not handled!";
		}
	}

	int Type::GetByteSize()
	{
		switch (type)
		{
			case TypeSystemType::BYTE: return BYTE_SIZE;
			case TypeSystemType::INT: return INT_SIZE;
			case TypeSystemType::FLOAT: return FLOAT_SIZE;
			case TypeSystemType::BOOL: return BYTE_SIZE;
			case TypeSystemType::DOUBLE: return DOUBLE_SIZE;
			case TypeSystemType::LONG: return LONG_SIZE;
			case TypeSystemType::STRING: throw std::runtime_error("Type::GetByteSize - String not handled!");
			case TypeSystemType::UNIT: return 0;
			default: throw std::runtime_error("Type::GetByteSize - TypeSystemType not handled!");
		}
	}

	Type* Type::GetCopy()
	{
		switch (type)
		{
			case TypeSystemType::BYTE: return BYTE();
			case TypeSystemType::INT: return INT();
			case TypeSystemType::FLOAT: return FLOAT();
			case TypeSystemType::BOOL: return BOOL();
			case TypeSystemType::DOUBLE: return DOUBLE();
			case TypeSystemType::LONG: return LONG();
			case TypeSystemType::STRING: ASSERT_MSG(false, "Type::GetCopy - String not handled!");
			case TypeSystemType::UNIT: return UNIT();
			default: ASSERT_MSG(false, "Type::GetCopy - TypeSystemType not handled!");
		}
	}

#pragma region TypeResult
	std::string TypeResult::ToString(bool _includePos)
	{
		std::string prefix = _includePos ? "(" + errPosition.ToString() + ") " : "";
		switch (type)
		{
			case TypeResultType::SUCCESS: return "SUCCESS";
			case TypeResultType::ID_NOT_FOUND: return  prefix + "\"" + errString + "\" has not been declared!";
			case TypeResultType::MISMATCH: return prefix + "Mismatch of " + errString;
			case TypeResultType::EXPECTATION: return prefix + errString;
			case TypeResultType::REDEFINITION: return prefix + "Redefinition of \"" + errString + "\"";
			case TypeResultType::CODE_PATH_LACKS_RET: return prefix + "Not all code paths return a value : \"" + errString + "\"";
			case TypeResultType::AMBIGUOUS_FUNC_DECL: return prefix + "Ambiguous Function Declaration : \"" + errString + "\"";
			case TypeResultType::FUNC_ID_PARAMS_TYPE_PAIR_NOT_FOUND: return prefix + "Function " + errString + " was not found!";
			default: return prefix + "TypeResult::ToString - TypeResultType not handled!";
		}
	}
#pragma endregion

#pragma region TupleType
	TupleType::TupleType(TypeList _types)
		: Type(TypeSystemType::TUPLE), types(_types) { }

	TupleType::~TupleType()
	{
		for (auto const& type : types)
			delete type;
	}

	std::string TupleType::ToString(int _indentLvl)
	{
		std::string result = CreateIndent(_indentLvl) + "(";

		for (auto const& type : types)
			result += type->ToString(0) + ",";

		if (types.size() != 0)
			result.pop_back();

		return result + ")";
	}

	int TupleType::GetByteSize()
	{
		int acc = 0;

		for (auto const& it : types)
			acc += it->GetByteSize();

		return acc;
	}

	bool TupleType::Matches(Type* _t)
	{
		if (_t->GetType() != TypeSystemType::TUPLE)
			return false;

		TupleType* other = (TupleType*)_t;

		if (other->types.size() != types.size())
			return false;

		for (int i = 0; i < (int)types.size(); i++)
			if (!types[i]->Matches(other->types[i]))
				return false;

		return true;
	}

	Type* TupleType::GetCopy()
	{
		TypeList copiedTypes;

		for (auto const& type : types)
			copiedTypes.push_back(type->GetCopy());

		return new TupleType(copiedTypes);
	}
#pragma endregion

#pragma region FuncType
	FuncType::FuncType(Type* _params, Type* _ret)
		: Type(TypeSystemType::FUNC), params(_params), ret(_ret) { }

	FuncType::~FuncType()
	{
		delete ret;
		delete params;
	}

	std::string FuncType::ToString(int _indentLvl) { return params->ToString(0) + " -> " + ret->ToString(0); }
	int FuncType::GetByteSize() { return ret->GetByteSize(); }
	Type* FuncType::GetCopy() { return new FuncType(params->GetCopy(), ret->GetCopy()); }

	bool FuncType::Matches(Type* _t)
	{
		if (_t->GetType() != TypeSystemType::FUNC)
			return false;

		FuncType* other = (FuncType*)_t;

		return params->Matches(other->params) && ret->Matches(other->ret);
	}

#pragma endregion

#pragma region ArrayType
	ArrayType::ArrayType(Type* _storageType, int _numElems)
		: Type(TypeSystemType::ARRAY), storageType(_storageType), numElems(_numElems) { }

	ArrayType::~ArrayType() { delete storageType; }
	std::string ArrayType::ToString(int _indentLvl) { return CreateIndent(_indentLvl) + storageType->ToString(0) + "[" + std::to_string(numElems) + "]"; }
	int ArrayType::GetByteSize() { return storageType->GetByteSize() * numElems; }
	Type* ArrayType::GetCopy() { return new ArrayType(storageType->GetCopy(), numElems); }

	bool ArrayType::Matches(Type* _t)
	{
		if (_t->GetType() != TypeSystemType::ARRAY) { return false; }
		else
		{
			ArrayType* other = (ArrayType*)_t;
			return storageType->Matches(other->storageType) && numElems == other->numElems;
		}
	}

#pragma endregion
}
