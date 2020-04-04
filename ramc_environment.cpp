#include "pch.h"
#include "ramc_environment.h"
#include "ramc_typesystem.h"

namespace ramc {
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
		nextGlobalStackPos = 0;

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
		{
			delete it.second.type;
			delete it.second.source;
		}
	}

	TypeResult Environment::AddVariable(std::string _id, Type* _type, Argument* _source, Position _exePos)
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

	Argument* Environment::GetVarSource(std::string _id)
	{
		auto search = variables.find(_id);
		if (search == variables.end()) { return parent ? parent->GetVarSource(_id) : nullptr; }
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
	
	std::string Environment::GenFuncLabel(std::string _name, Type* _paramsType)
	{
		std::string postfix = StrReplace(_paramsType->ToString(0), "(,)", '_');
		return "%FUNC_" + _name + "_" + postfix;
	}
}