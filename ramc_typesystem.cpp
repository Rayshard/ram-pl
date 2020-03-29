#include "pch.h"
#include "ramc_typesystem.h"
#include "ramvm_instruction.h"

using ramvm::Argument;
using ramvm::ArgType;

namespace ramc {
	TypePtr Type::BOOL = std::shared_ptr<Type>(new Type(TypeSystemType::BOOL));
	TypePtr Type::BYTE = std::shared_ptr<Type>(new Type(TypeSystemType::BYTE));
	TypePtr Type::INT = std::shared_ptr<Type>(new Type(TypeSystemType::INT));
	TypePtr Type::FLOAT = std::shared_ptr<Type>(new Type(TypeSystemType::FLOAT));
	TypePtr Type::DOUBLE = std::shared_ptr<Type>(new Type(TypeSystemType::DOUBLE));
	TypePtr Type::LONG = std::shared_ptr<Type>(new Type(TypeSystemType::LONG));
	TypePtr Type::STRING = std::shared_ptr<Type>(new Type(TypeSystemType::STRING));
	TypePtr Type::VOID = std::shared_ptr<Type>(new Type(TypeSystemType::VOID));
	TypePtr Type::UNIT = std::shared_ptr<Type>(new Type(TypeSystemType::UNIT));

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

	bool TupleType::Matches(TypePtr _t)
	{
		if (_t->GetType() != TypeSystemType::TUPLE)
			return false;

		TupleType* other = (TupleType*)_t.get();

		if (other->types.size() != types.size())
			return false;

		for (int i = 0; i < types.size(); i++)
			if (!types[i]->Matches(other->types[i]))
				return false;

		return true;
	}
#pragma endregion

#pragma region FuncType
	FuncType::FuncType(TypePtr _params, TypePtr _ret)
		: Type(TypeSystemType::FUNC), params(_params), ret(_ret) { }

	std::string FuncType::ToString(int _indentLvl) { return params->ToString(0) + " -> " + ret->ToString(0); }

	int FuncType::GetByteSize() { return ret->GetByteSize(); }

	bool FuncType::Matches(TypePtr _t)
	{
		if (_t->GetType() != TypeSystemType::FUNC)
			return false;

		FuncType* other = (FuncType*)_t.get();

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

	TypeResult Environment::AddVariable(std::string _id, TypePtr _type, ArgType _argType, Position _exePos)
	{
		auto search = variables.find(_id);
		if (search != variables.end()) { return TypeResult::GenRedefinition(_id, _exePos); }
		else
		{
			VarInfo info;
			info.type = _type;
			info.source = Argument(_argType, Argument::IsRegisterArgType(_argType) ? nextRegIdx++ : 0);

			SetMaxNumVarRegNeeded(nextRegIdx);

			variables.insert_or_assign(_id, info);
			return TypeResult::GenSuccess(_type);
		}
	}

	bool Environment::HasVariable(std::string _id, bool _localCheck)
	{
		return variables.find(_id) != variables.end() || (!_localCheck && (parent ? parent->HasVariable(_id, false) : false));
	}

	bool Environment::HasVariable(std::string _id, TypePtr _type, bool _localCheck)
	{
		auto search = variables.find(_id);
		if (search == variables.end()) { return !_localCheck && (parent ? parent->HasVariable(_id, _type, false) : false); }
		else { return search->second.type == _type; }
	}

	TypeResult Environment::GetVariableType(std::string _id, Position _execPos)
	{
		auto search = variables.find(_id);
		if (search == variables.end()) { return parent ? parent->GetVariableType(_id, _execPos) : TypeResult::GenIDNotFound(_id, _execPos); }
		else { return TypeResult::GenSuccess(search->second.type); }
	}

	Argument Environment::GetVarSource(std::string _id)
	{
		auto search = variables.find(_id);
		if (search == variables.end()) { return parent ? parent->GetVarSource(_id) : Argument(ArgType::INVALID, 0); }
		else { return search->second.source; }
	}

	TypeResult Environment::AddFunction(std::string _id, TypePtr _type, std::string& _label, Position _execPos)
	{
		std::string label = GenFuncLabel(_id, ((FuncType*)_type.get())->GetParamsType());

		auto search = functions.find(label);
		if (search != functions.end()) { return TypeResult::GenAmbiguousFuncDecl(_id, _type, _execPos); }
		else
		{
			FuncInfo info;
			info.type = _type;
			info.label = label;

			functions.insert_or_assign(label, info);
			_label = label;
			return TypeResult::GenSuccess(Type::UNIT);
		}
	}

	TypeResult Environment::GetFunctionRetType(std::string _id, TypePtr _paramsType, Position _execPos)
	{
		std::string label = GenFuncLabel(_id, _paramsType);

		auto search = functions.find(label);
		if (search == functions.end()) { return parent ? parent->GetFunctionRetType(_id, _paramsType, _execPos) : TypeResult::GenFuncIDParamsTypePairNotFound(_id, _paramsType, _execPos); }
		else { return TypeResult::GenSuccess(((FuncType*)search->second.type.get())->GetRetType()); }
	}
#pragma endregion
}
