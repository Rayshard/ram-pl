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
			case TypeSystemType::VOID: return "VOID";
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
			case TypeSystemType::VOID: return 0;
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
			case TypeSystemType::STRING: throw std::runtime_error("Type::GetCopy - String not handled!");
			case TypeSystemType::VOID: return VOID();
			case TypeSystemType::UNIT: return UNIT();
			default: throw std::runtime_error("Type::GetCopy - TypeSystemType not handled!");
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

#pragma region Environment
	void Environment::SetMaxNumVarRegNeeded(int _val)
	{
		if (_val > numRegNeeded)
			numRegNeeded = _val;

		if (parent)
		{
			auto search = parent->subEnvs.find(this);
			if (search->second)
				parent->SetMaxNumVarRegNeeded(_val);
		}
	}

	Environment::Environment(Environment* _parent, bool _incrParentRegCnt)
	{
		parent = _parent;

		if (parent != 0)
		{
			numRegNeeded = _parent->numRegNeeded;

			if (_incrParentRegCnt)
			{
				nextRegIdx = parent->nextRegIdx;
				parent->subEnvs.insert_or_assign(this, true);
			}
			else
			{
				nextRegIdx = 0;
				parent->subEnvs.insert_or_assign(this, false);
			}
		}
		else
		{
			nextRegIdx = 0;
			numRegNeeded = 0;
		}
	}

	Environment::~Environment()
	{
		for (auto it : functions)
			delete it.second.type;

		for (auto it : variables)
			delete it.second.type;
	}

	TypeResult Environment::AddVariable(std::string _id, Type* _type, Argument _source, Position _exePos)
	{
		auto search = variables.find(_id);
		if (search != variables.end()) { return TypeResult::GenRedefinition(_id, _exePos); }
		else
		{
			VarInfo info;
			info.type = _type->GetCopy();
			info.source = _source;

			SetMaxNumVarRegNeeded(nextRegIdx);

			variables.insert_or_assign(_id, info);
			return TypeResult::GenSuccess(nullptr);
		}
	}

	bool Environment::HasVariable(std::string _id, bool _localCheck)
	{
		return variables.find(_id) != variables.end() || (!_localCheck && (parent ? parent->HasVariable(_id, false) : false));
	}

	bool Environment::HasVariable(std::string _id, Type* _type, bool _localCheck)
	{
		auto search = variables.find(_id);
		if (search == variables.end()) { return !_localCheck && (parent ? parent->HasVariable(_id, _type, false) : false); }
		else { return _type->Matches(search->second.type); }
	}

	TypeResult Environment::GetVariableType(std::string _id, Position _execPos)
	{
		auto search = variables.find(_id);
		if (search == variables.end()) { return parent ? parent->GetVariableType(_id, _execPos) : TypeResult::GenIDNotFound(_id, _execPos); }
		else { return TypeResult::GenSuccess(search->second.type->GetCopy()); }
	}

	Argument Environment::GetVarSource(std::string _id)
	{
		auto search = variables.find(_id);
		if (search == variables.end()) { return parent ? parent->GetVarSource(_id) : Argument(ArgType::INVALID, 0); }
		else { return search->second.source; }
	}

	TypeResult Environment::AddFunction(std::string _id, Type* _type, std::string& _label, Position _execPos)
	{
		std::string label = GenFuncLabel(_id, ((FuncType*)_type)->GetParamsType());

		auto search = functions.find(label);
		if (search != functions.end()) { return TypeResult::GenAmbiguousFuncDecl(_id, _type, _execPos); }
		else
		{
			FuncInfo info;
			info.type = _type->GetCopy();
			info.label = label;

			_label = label;
			functions.insert_or_assign(label, info);
			return TypeResult::GenSuccess(nullptr);
		}
	}

	TypeResult Environment::GetFunctionRetType(std::string _id, Type* _paramsType, Position _execPos)
	{
		std::string label = GenFuncLabel(_id, _paramsType);

		auto search = functions.find(label);
		if (search == functions.end()) { return parent ? parent->GetFunctionRetType(_id, _paramsType, _execPos) : TypeResult::GenFuncIDParamsTypePairNotFound(_id, _paramsType, _execPos); }
		else { return TypeResult::GenSuccess(((FuncType*)search->second.type)->GetRetType()->GetCopy()); }
	}
#pragma endregion
}
