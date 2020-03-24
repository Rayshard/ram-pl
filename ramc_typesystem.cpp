#include "pch.h"
#include "ramc_typesystem.h"

namespace ramc {
	std::string TypeResult::ToString()
	{
		switch (resType)
		{
			case ramc::TypeResultType::SUCCESS: return "SUCCESS";
			case ramc::TypeResultType::ID_NOT_FOUND: return "(" + errPosition.ToString() + ") \"" + errString + "\" was not found!";
			case ramc::TypeResultType::MISMATCH: return "(" + errPosition.ToString() + ") Mismatch of " + errString;
			default: return "(" + errPosition.ToString() + ") TypeResult::ToString - TypeResultType not handled!";
		}
	}

	TypeResult TypeResult::GenSuccess(PrimitiveType _type)
	{
		TypeResult result;
		result.resType = TypeResultType::SUCCESS;
		result.type = _type;
		return result;
	}

	TypeResult TypeResult::GenIDNotFound(std::string _id, Position _pos)
	{
		TypeResult result;
		result.resType = TypeResultType::ID_NOT_FOUND;
		result.errString = _id;
		result.errPosition = _pos;
		return result;
	}

	TypeResult TypeResult::GenMismatch(std::string _mismatch, Position _pos)
	{
		TypeResult result;
		result.resType = TypeResultType::MISMATCH;
		result.errString = _mismatch;
		result.errPosition = _pos;
		return result;
	}

	const std::string PrimTypeToString(PrimitiveType _type)
	{
		const std::map<PrimitiveType, std::string> strings{
			{ PrimitiveType::INT, "INT" },
			{ PrimitiveType::FLOAT, "FLOAT" },
			{ PrimitiveType::STRING, "STRING" },
			{ PrimitiveType::BOOL, "BOOL" },
			{ PrimitiveType::VOID, "VOID" },
		};

		auto search = strings.find(_type);
		return search == strings.end() ? "PrimTypeToString - PrimitiveType not handled!" : search->second;
	}

	Environment::Environment(Environment* _parent)
	{
		parent = _parent;
	}

	bool Environment::AddVariable(std::string _id, PrimitiveType _type)
	{
		auto search = variables.find(_id);
		if (search != variables.end()) { return false; }
		else
		{
			variables.insert_or_assign(_id, _type);
			return true;
		}
	}

	bool Environment::HasVariable(std::string _id)
	{
		return variables.find(_id) != variables.end() || (parent && HasVariable(_id));
	}

	bool Environment::HasVariable(std::string _id, PrimitiveType _type)
	{
		auto search = variables.find(_id);
		if (search == variables.end()) { return parent ? parent->HasVariable(_id, _type) : false; }
		else { return search->second == _type; }
	}

	TypeResult Environment::GetVariableType(std::string _id, Position _execPos)
	{
		auto search = variables.find(_id);
		if (search == variables.end()) { return parent ? parent->GetVariableType(_id, _execPos) : TypeResult::GenIDNotFound(_id, _execPos); }
		else { return TypeResult::GenSuccess(search->second); }
	}
}
