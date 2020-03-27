#include "pch.h"
#include "ramc_typesystem.h"
#include "ramvm_instruction.h"

using ramvm::Argument;
using ramvm::ArgType;

namespace ramc {
	Type* Type::BOOL = new Type(TypeSystemType::BOOL);
	Type* Type::BYTE = new Type(TypeSystemType::BYTE);
	Type* Type::INT = new Type(TypeSystemType::INT);
	Type* Type::FLOAT = new Type(TypeSystemType::FLOAT);
	Type* Type::DOUBLE = new Type(TypeSystemType::DOUBLE);
	Type* Type::LONG = new Type(TypeSystemType::LONG);
	Type* Type::STRING = new Type(TypeSystemType::STRING);
	Type* Type::VOID = new Type(TypeSystemType::VOID);

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
			default: return "Type::ToString - Type not handled!";
		}
	}
	
	std::string TypeResult::ToString(bool _includePos)
	{
		std::string prefix = _includePos ? "(" + errPosition.ToString() + ") " : "";
		switch (type)
		{
			case TypeResultType::SUCCESS: return "SUCCESS";
			case TypeResultType::ID_NOT_FOUND: return  prefix + "\"" + errString + "\" has not been declared!";
			case TypeResultType::MISMATCH: return prefix + "Mismatch of " + errString;
			case TypeResultType::EXPECTATION: return prefix + errString;
			case TypeResultType::REDECLARATION: return prefix + "Redeclaration of \"" + errString + "\"";
			default: return prefix + "TypeResult::ToString - TypeResultType not handled!";
		}
	}

	Environment::Environment(Environment* _parent)
	{
		parent = _parent;
		nextVarRegIdx = 0;
	}

	bool Environment::AddVariable(std::string _id, Type* _type)
	{
		auto search = variables.find(_id);
		if (search != variables.end()) { return false; }
		else
		{
			VarInfo info;
			info.type = _type;
			info.regIdx = nextVarRegIdx++;

			variables.insert_or_assign(_id, info);
			return true;
		}
	}

	bool Environment::HasVariable(std::string _id)
	{
		return variables.find(_id) != variables.end() || (parent && HasVariable(_id));
	}

	bool Environment::HasVariable(std::string _id, Type* _type)
	{
		auto search = variables.find(_id);
		if (search == variables.end()) { return parent ? parent->HasVariable(_id, _type) : false; }
		else { return search->second.type == _type; }
	}

	TypeResult Environment::GetVariableType(std::string _id, Position _execPos)
	{
		auto search = variables.find(_id);
		if (search == variables.end()) { return parent ? parent->GetVariableType(_id, _execPos) : TypeResult::GenIDNotFound(_id, _execPos); }
		else { return TypeResult::GenSuccess(search->second.type); }
	}

	Argument Environment::GetVarRegister(std::string _id)
	{
		auto search = variables.find(_id);
		if (search == variables.end()) { return parent ? parent->GetVarRegister(_id) : Argument(ArgType::INVALID, 0); }
		else { return Argument(ArgType::REGISTER, search->second.regIdx); }
	}
}
