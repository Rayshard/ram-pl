#include <iostream>
#include "..\pch.h"
#include "Statement.h"
#include "..\Parser.h"

IStatement::IStatement(Position _pos, StatementType _type) : _position(_pos), _type(_type) {}

IStatement::~IStatement()
{
}

SimpleStatement::SimpleStatement(Position _pos) : IStatement(_pos, SSIMPLE) { expr = new ValueExpression(new VoidValue(_pos)); }
SimpleStatement::SimpleStatement(IExpression* _expr) : IStatement(_expr->_position, SSIMPLE) { expr = _expr; }
SimpleStatement::~SimpleStatement() { delete expr; }

IValue* SimpleStatement::Execute(Environment* _env)
{
	return expr->Evaluate(_env);
}

IStatement* SimpleStatement::GetCopy() { return new SimpleStatement(expr->GetCopy()); }

CodeBlock::CodeBlock(std::vector<IStatement*>& _statements, Position _pos, bool _useSubEnv)
	: IStatement(_pos, SCODE_BLOCK)
{
	if(_statements.size() == 0) { statements.push_back(new SimpleStatement(_pos)); }
	else { statements = _statements; }

	useSubEnv = _useSubEnv;
}

CodeBlock::~CodeBlock()
{
	for(auto it : statements)
		delete it;
}

IValue* CodeBlock::Execute(Environment* _env)
{
	Environment* env = useSubEnv ? new Environment(_env, "//") : _env;

	IValue* returnVal = 0;

	for(int i = 0; i < statements.size(); i++)
	{
		auto val = std::unique_ptr<IValue>(statements[i]->Execute(env));
		PRINT_LINE(statements[i]->_position.line);

		if(val->_type == VEXCEPTION)
		{
			returnVal = val.release();
			break;
		}
		else { PRINT_VAL(val, env); }
	}

	PRINT_ENV((*env));

	if(useSubEnv)
		delete env;

	return returnVal ? returnVal : new VoidValue(_position);
}

IStatement* CodeBlock::GetCopy()
{
	std::vector<IStatement*> copy_Stmnts;

	for(auto it : statements)
		copy_Stmnts.push_back(it->GetCopy());

	return new CodeBlock(copy_Stmnts, _position, useSubEnv);
}

Assignment::Assignment(std::string _identifier, IExpression* _expr, bool _isLet, Position _pos)
	: IStatement(_pos, SASSIGNMENT)
{
	identifier = _identifier;
	isLetAssignment = _isLet;
	expr = _expr;
}

Assignment::~Assignment() { delete expr; }

IValue* Assignment::Execute(Environment* _env)
{
	IValue* val = expr->Evaluate(_env);

	if(val->_type == VEXCEPTION)
		return val;

	IValue* assignResult = 0;

	if(isLetAssignment) { assignResult = _env->AddVariable(identifier, val, _position, false, false); /* Returns void value if successful */ }
	else { assignResult = _env->SetValue(identifier, val, _position); /* Returns void value if successful */ }

	if(assignResult->_type == VEXCEPTION)
	{
		delete val;
		return assignResult;
	}
	else
	{
		delete assignResult;
		return val->GetCopy();
	}
}

IStatement* Assignment::GetCopy() { return new Assignment(identifier, expr->GetCopy(), isLetAssignment, _position); }

ForLoop::ForLoop(Assignment* _initAssign, IExpression* _conditionExpr, IStatement* _statement, Assignment* _finAssign, Position _pos)
	: IStatement(_pos, SFOR_LOOP)
{
	initAssignment = _initAssign;
	conditionExpr = _conditionExpr;
	statement = _statement;
	finallyAssign = _finAssign;
}

ForLoop::~ForLoop()
{
	delete initAssignment;
	delete conditionExpr;
	delete statement;
	delete finallyAssign;
}

IValue* ForLoop::Execute(Environment* _env)
{
	Environment env(_env, "//");

	IValue* initAssignVal = initAssignment->Execute(&env);

	if(initAssignVal->_type == VEXCEPTION)
		return initAssignVal;

	PRINT_VAL(initAssignVal, &env);
	PRINT_LINE(initAssignment->_position.line);
	delete initAssignVal;

	while(true)
	{
		auto evalCondVal = std::unique_ptr<IValue>(conditionExpr->Evaluate(&env));

		if(evalCondVal->_type == VEXCEPTION) { return evalCondVal.release(); }
		else if(evalCondVal->_type != VBOOL) { return Exception_MismatchType("bool", evalCondVal->GetTypeSig(), evalCondVal->_position); }

		PRINT_VAL(evalCondVal, &env);
		PRINT_LINE(conditionExpr->_position.line);

		if(!((BoolValue*)evalCondVal.get())->value)
			break;

		IValue* statementVal = statement->Execute(&env);

		if(statementVal->_type == VEXCEPTION)
			return statementVal;

		PRINT_VAL(statementVal, &env);
		PRINT_LINE(statement->_position.line);
		delete statementVal;

		IValue* finallyAssignVal = finallyAssign->Execute(&env);

		if(finallyAssignVal->_type == VEXCEPTION)
			return finallyAssignVal;

		PRINT_VAL(finallyAssignVal, &env);
		PRINT_LINE(finallyAssign->_position.line);
		delete finallyAssignVal;
	}

	return new VoidValue(_position);
}

IStatement* ForLoop::GetCopy() { return new ForLoop((Assignment*)initAssignment->GetCopy(), conditionExpr->GetCopy(), statement->GetCopy(), (Assignment*)finallyAssign->GetCopy(), _position); }

MemberDefinition::MemberDefinition(std::string _identifier, std::string _typeDef, Position _pos)
	: IStatement(_pos, SMEMBER_DEF)
{
	definition = Definition(_identifier, _typeDef);
}

IValue* MemberDefinition::Execute(Environment* _env)
{
	IValue* val = _env->GetValue(definition.first, _position, true, false);

	if(val->_type == VEXCEPTION)
		return val;

	bool hasType = val->GetTypeSig() == definition.second;
	return new BoolValue(hasType, _position);
}

IStatement* MemberDefinition::GetCopy() { return new MemberDefinition(definition.first, definition.second, _position); }

TypeDefinition::TypeDefinition(std::string _identifier, DefinitionMap& _memDefs, Position _pos)
	: IStatement(_pos, STYPE_DEF)
{
	identifier = _identifier;
	memDefs = _memDefs;
}

IValue* TypeDefinition::Execute(Environment* _env) { return _env->AddTypeDefinition(identifier, memDefs, _position); }
IStatement* TypeDefinition::GetCopy() { return new TypeDefinition(identifier, memDefs, _position); }

FuncDeclaration::FuncDeclaration(std::string _identifier, DefinitionList& _argDefs, TypeName _retTypeName, IStatement* _body, Position _pos)
	: IStatement(_pos, SFUNC_DECL)
{
	identifier = _identifier;
	argDefs = _argDefs;
	retTypeName = _retTypeName;
	body = _body;
}

FuncDeclaration::~FuncDeclaration() { delete body; }

IValue* FuncDeclaration::Execute(Environment* _env)
{
	std::vector<std::string> argNames, argSigs;
	TypeSig retSig;

	for(auto it : argDefs)
	{
		IValue* val = _env->GetTypeSig(it.second, _position);

		if(val->_type == VEXCEPTION) { return val; }
		else 
		{
			argNames.push_back(it.first);
			argSigs.push_back(((StringValue*)val)->value);
			delete val;
		}
	}

	IValue* val = _env->GetTypeSig(retTypeName, _position);

	if(val->_type == VEXCEPTION) { return val; }
	else
	{
		retSig = ((StringValue*)val)->value;
		delete val;
	}

	FuncValue* func = new FuncValue(_env, body->GetCopy(), argNames, argSigs, retSig, _position);
	return _env->AddFuncDeclaration(identifier, func, _position);
}

IStatement* FuncDeclaration::GetCopy() { return new FuncDeclaration(identifier, argDefs, retTypeName, body->GetCopy(), _position); }
