#include "Value.h"
#include "Statement.h"

Definition::Definition(std::string _identifier, TypeName _typename) : identifier(_identifier), typeName(_typename) { }
Definition::Definition() : Definition(std::string(), TypeName()) { }
bool Definition::operator==(const Definition& _other) { return identifier == _other.identifier && typeName == _other.typeName; }

IValue::IValue(ValueType _type, Position _pos, TypeName _typeName, Environment* _intrEnv)
	: _type(_type), _position(_pos), typeName(_typeName), intrinsicEnv(_intrEnv)
{

}

IValue::~IValue()
{
	if(intrinsicEnv)
		delete intrinsicEnv;
}

Environment * IValue::GetIntrinsicEnv()
{
	if(intrinsicEnv)
		return intrinsicEnv;

	intrinsicEnv = new Environment(Environment::GLOBAL, "<" + typeName + ">");

	// Adding Intrinsic Members
	FuncValue::built_in func_body = 0;
	FuncValue* func = 0;
	DefinitionList func_argDefs;

	func_body = [this](Environment* _env, Position _execPos) { return new StringValue(this->ToString(_env), Position()); };
	func = new FuncValue(intrinsicEnv, func_body, func_argDefs, "string", Position());
	delete intrinsicEnv->AddVariable("__string__", func, Position(), false, true);

	func_body = [this](Environment* _env, Position _execPos) { return new BoolValue(this->ToBool(_env), Position()); };
	func = new FuncValue(intrinsicEnv, func_body, func_argDefs, "bool", Position());
	delete intrinsicEnv->AddVariable("__bool__", func, Position(), false, true);

	return intrinsicEnv;
}

ExceptionValue::ExceptionValue(std::string _name, std::string _msg, Position _pos)
	: IValue(VEXCEPTION, _pos, "<exception>", 0)
{
	name = _name;
	message = _msg;
}

ExceptionValue::ExceptionValue(Position _pos) : ExceptionValue("Exception", "An Exception Occurred", _pos) { }
IValue* ExceptionValue::GetCopy() { return new ExceptionValue(name, message, _position); }
TypeSig ExceptionValue::GetTypeSig() { return "<exception>"; }
std::string ExceptionValue::ToString(Environment* _execEnv) { return "Exception : " + name + "\n" + _position.ToString() + "\n" + message + "\n>> " + GetSrcLine(_position.line); }
bool ExceptionValue::ToBool(Environment* _execEnv) { return false; }

VoidValue::VoidValue(TypeName _typeName, TypeSig _typeSig, Position _pos)
	: IValue(VVOID, _pos, _typeName, 0)
{
	typeSig = _typeSig;
}

VoidValue::VoidValue(Position _pos) : VoidValue("void", "<void>", _pos) { }
IValue* VoidValue::GetCopy() { return new VoidValue(typeName, typeSig, _position); }
TypeSig VoidValue::GetTypeSig() { return typeSig; }
bool VoidValue::ToBool(Environment* _execEnv) { return false; }
std::string VoidValue::ToString(Environment* _execEnv) { return typeName; }

IValue* VoidValue::FromTypeName(TypeName _typeName, Environment* _execEnv, Position _pos)
{
	IValue* val = _execEnv->GetTypeSigFromTypeName(_typeName, _pos);

	if(val->_type == VEXCEPTION) { return val; }
	else
	{
		TypeSig typeSig = ((StringValue*)val)->value;

		delete val;
		return new VoidValue(_typeName, typeSig, _pos);
	}
}

std::string MemberedValue::TYPESIG_CAST_BOOL = "<func>(void)->bool";
std::string MemberedValue::TYPESIG_CAST_STRING = "<func>(void)->string";

MemberedValue::MemberedValue(TypeName _typeName, DefinitionMap& _memDefs, Environment* _intrEnv, Position _pos)
	: IValue(VMEMBERED, _pos, _typeName, _intrEnv)
{
	memDefinitions = _memDefs;
}

IValue* MemberedValue::GetCopy() { return new MemberedValue(typeName, memDefinitions, intrinsicEnv->GetCopy(), _position); }

TypeSig MemberedValue::GetTypeSig() { return GetMemberedTypeDef(memDefinitions); }

IValue * MemberedValue::GetMemberValue(std::string _memId, Position _execPos, bool _retCopy) { return intrinsicEnv->GetValue(_memId, _execPos, false, _retCopy); }

std::string MemberedValue::ToString(Environment* _execEnv)
{
	FuncValue* func = ((FuncValue*)intrinsicEnv->GetValue("__string__", _position, false, false));
	ArgumentList argExprs = ArgumentList();
	auto returnVal = std::unique_ptr<StringValue>((StringValue*)func->Call(_execEnv, argExprs, _position));
	return returnVal.get()->value;
}

bool MemberedValue::ToBool(Environment* _execEnv)
{
	FuncValue* func = ((FuncValue*)intrinsicEnv->GetValue("__bool__", _position, false, false));
	ArgumentList argExprs = ArgumentList();
	auto returnVal = std::unique_ptr<BoolValue>((BoolValue*)func->Call(_execEnv, argExprs, _position));
	return returnVal.get()->value;
}

bool MemberedValue::HasMember(std::string _name, TypeName _typeName = "")
{
	bool hasMemName = memDefinitions.find(_name) != memDefinitions.end();

	if(hasMemName && !_typeName.empty()) { return memDefinitions[_name] == _typeName; }
	else { return hasMemName; }
}

IValue* MemberedValue::Create(Environment* _creationEnv, AssignmentMap& _memAssigns, Position _pos)
{
	Environment* intrinsicEnv = new Environment(_creationEnv, "<membered>");
	DefinitionMap memberDefs;

	for(auto it : _memAssigns)
	{
		// When the exprs are evaluated, they don't have access to the 
		// members being created along side them that's why I use
		// _creationEnv instead of membersEnv;
		IValue* val = it.second->Evaluate(_creationEnv);

		if(val->_type == VEXCEPTION)
		{
			delete intrinsicEnv;
			return val;
		}

		delete intrinsicEnv->AddVariable(it.first, val, _pos, false, true);
		memberDefs.insert_or_assign(it.first, val->GetTypeSig());
	}

	auto searchCastStr = memberDefs.find("__string__");
	if(searchCastStr == memberDefs.end())
	{
		auto func_body = [memberDefs, intrinsicEnv](Environment* _env, Position _execPos)
		{
			std::string str = "{ ";

			for(auto it : memberDefs)
				str += it.first + " = " + intrinsicEnv->GetValue(it.first, _execPos, false, false)->ToString(_env) + "; ";

			str.pop_back();
			str.pop_back();
			str += " }";

			return new StringValue(str, _execPos);
		};
		DefinitionList func_argDefs;
		FuncValue* func = new FuncValue(intrinsicEnv, func_body, func_argDefs, "string", _pos);

		delete intrinsicEnv->AddVariable("__string__", func, _pos, false, true);
		memberDefs.insert_or_assign("__string__", func->GetTypeSig());

	}
	else if(searchCastStr->second != TYPESIG_CAST_STRING)
	{
		delete intrinsicEnv;
		return Exception_MismatchType(TYPESIG_CAST_STRING, searchCastStr->second, _pos);
	}

	auto searchCastBool = memberDefs.find("__bool__");
	if(searchCastBool == memberDefs.end())
	{
		auto func_body = [](Environment* _env, Position _execPos) { return (IValue*)new BoolValue(true, _execPos); };
		DefinitionList func_argDefs;
		FuncValue* func = new FuncValue(intrinsicEnv, func_body, func_argDefs, "bool", _pos);

		delete intrinsicEnv->AddVariable("__bool__", func, _pos, false, true);
		memberDefs.insert_or_assign("__bool__", func->GetTypeSig());
	}
	else if(searchCastBool->second != TYPESIG_CAST_BOOL)
	{
		delete intrinsicEnv;
		return Exception_MismatchType(TYPESIG_CAST_BOOL, searchCastBool->second, _pos);
	}

	return new MemberedValue(GetMemberedTypeDef(memberDefs), memberDefs, intrinsicEnv, _pos);
}

DefinitionList GetMemberDefs(ValueMap _mems)
{
	DefinitionList memDefs;

	for(auto it : _mems)
		memDefs.push_back(Definition(it.first, it.second->typeName));

	return memDefs;
}

TypeDef GetMemberedTypeDef(DefinitionMap& _memDefs)
{
	TypeDef typeDef = "<membered>{";

	for(auto it : _memDefs)
		typeDef += it.first + ":" + it.second + "; ";

	if(_memDefs.size() != 0)
	{
		typeDef.pop_back();
		typeDef.pop_back();
	}

	typeDef += "}";
	return typeDef;
}

IValue* GetMemberedTypeSig(Environment* _execEnv, DefinitionMap& _memDefs, Position _execPos)
{
	TypeSig typeSig = "<membered>{";

	for(auto it : _memDefs)
	{
		IValue* retVal = _execEnv->GetTypeSigFromTypeName(it.second, _execPos);

		if(retVal->_type == VEXCEPTION) { return retVal; }
		else
		{
			typeSig += it.first + ":" + ((StringValue*)retVal)->value + "; ";
			delete retVal;
		}
	}

	if(_memDefs.size() != 0)
	{
		typeSig.pop_back();
		typeSig.pop_back();
	}

	typeSig += "}";
	return new StringValue(typeSig, _execPos);
}

NamedspaceValue::NamedspaceValue(Environment* _env, Position _pos)
	: IValue(VNAMEDSPACE, _pos, "<namedspace>", _env)
{
	environment = intrinsicEnv;
}

NamedspaceValue::~NamedspaceValue() { delete environment; }
IValue* NamedspaceValue::GetCopy() { return new NamedspaceValue(environment->GetCopy(), _position); }
TypeSig NamedspaceValue::GetTypeSig() { return "<namedspace>" + environment->name; }
std::string NamedspaceValue::ToString(Environment* _execEnv) { return "namedspace"; }
bool NamedspaceValue::ToBool(Environment* _execEnv) { return true; }

FuncValue::FuncValue(Environment* _defEnv, IStatement* _statement, DefinitionList& _argDefs, TypeName _retTypeName, Position _pos)
	: IValue(VFUNC, _pos, GetFuncTypeSig(_argDefs, _retTypeName), 0)
{
	defEnvironment = _defEnv;
	body = _statement;
	argDefs = _argDefs;
	returnTypename = _retTypeName;
	bodyType = DECLARED;
}

FuncValue::FuncValue(Environment* _defEnv, built_in _ptr, DefinitionList& _argDefs, TypeName _retTypeName, Position _pos)
	: IValue(VFUNC, _pos, GetFuncTypeSig(_argDefs, _retTypeName), 0)
{
	defEnvironment = _defEnv;
	pointer = _ptr;
	argDefs = _argDefs;
	returnTypename = _retTypeName;
	bodyType = BUILT_IN;
}

FuncValue::~FuncValue()
{
	if(body)
		delete body;
}

IValue * FuncValue::Call(Environment* _execEnv, ArgumentList& _argExprs, Position _execPos)
{
	if(_argExprs.size() != argDefs.size())
		return Exception_WrongNumArgs(argDefs.size(), _argExprs.size(), _execPos);

	Environment env(defEnvironment, "<func>"); //Doesn't have to be a ptr because it only needs to exist in this function

	for(int i = 0; i < argDefs.size(); i++)
	{
		IValue* argVal = _argExprs[i]->Evaluate(_execEnv);
		Definition argDef = argDefs[i];

		if(argVal->_type == VEXCEPTION) { return argVal; }
		else if(argVal->typeName != argDef.typeName) { return Exception_MismatchType(argDef.typeName, argVal->typeName, _execPos); }
		else { delete env.AddVariable(argDef.identifier, argVal, _execPos, false, true); }
	}

	if(dynamic_cast<CodeBlock*>(body) == 0) // Check if function has code block body or just a one line body
	{
#if PRINT_LINE
		PrintSrcLine(body->_position.line);
#endif
	}

	if(bodyType == DECLARED) { return body->Execute(&env); }
	else { return pointer(&env, _execPos); }
}

IValue* FuncValue::GetCopy()
{
	if(bodyType == DECLARED) { return new FuncValue(defEnvironment, body->GetCopy(), argDefs, returnTypename, _position); }
	else { return new FuncValue(defEnvironment, pointer, argDefs, returnTypename, _position); }
}

TypeSig FuncValue::GetTypeSig() { return GetFuncTypeSig(argDefs, returnTypename); }
std::string FuncValue::ToString(Environment* _execEnv) { return typeName; }
bool FuncValue::ToBool(Environment* _execEnv) { return true; }

TypeSig GetFuncTypeSig(DefinitionList& _argDefs, TypeName _retTypeName)
{
	TypeSig typeSig = "<func>";

	if(_argDefs.size() == 0) { typeSig += "(void)"; }
	else
	{
		typeSig += "(";

		for(auto it : _argDefs)
			typeSig += it.typeName + ", ";

		typeSig.pop_back();
		typeSig.pop_back();
		typeSig += ")";
	}

	return typeSig + "->" + _retTypeName;
}
