#include <iostream>
#include "..\pch.h"
#include "Statement.h"
#include "..\Parser.h"

IStatement::IStatement(Position _pos, StatementType _type) : _position(_pos), _type(_type) {}
IStatement::~IStatement() {}

#pragma region ExprStatement
ExprStatement::ExprStatement(IExpression* _expr) : IStatement(_expr->_position, SEXPR) { expr = _expr; }
ExprStatement::~ExprStatement() { delete expr; }
SharedValue ExprStatement::Execute(Environment* _env) { return expr->Evaluate(_env); }
IStatement* ExprStatement::GetCopy() { return new ExprStatement(expr->GetCopy()); }
#pragma endregion

#pragma region CodeBlock
CodeBlock::CodeBlock(std::vector<IStatement*>& _statements, Position _pos, bool _useSubEnv)
	: IStatement(_pos, SCODE_BLOCK)
{
	statements = _statements;
	useSubEnv = _useSubEnv;

	if(statements.size() == 0)
		statements.push_back(new ExprStatement(new ValueExpression(new VoidValue(_pos))));
}

CodeBlock::~CodeBlock()
{
	for(auto it : statements)
		delete it;
}
SharedValue CodeBlock::Execute(Environment* _env)
{
	Environment* env = useSubEnv ? new Environment(_env, "//") : _env;
	SharedValue returnVal(nullptr);

	for(int i = 0; i < statements.size(); i++)
	{
		SharedValue val = statements[i]->Execute(env);
		PRINT_LINE(statements[i]->_position.line);

		if(val->_type == VEXCEPTION)
		{
			returnVal = val;
			break;
		}
		else { PRINT_VAL(val, env); }
	}

	PRINT_ENV((*env));

	if(useSubEnv)
		delete env;

	return returnVal ? returnVal : std::shared_ptr<IValue>(new VoidValue(_position));
}

IStatement* CodeBlock::GetCopy()
{
	std::vector<IStatement*> copy_Stmnts;

	for(auto it : statements)
		copy_Stmnts.push_back(it->GetCopy());

	return new CodeBlock(copy_Stmnts, _position, useSubEnv);
}
#pragma endregion

#pragma region LetStatement
LetStatement::LetStatement(std::string _identifier, IExpression* _expr, Position _pos)
	: IStatement(_pos, SLET)
{
	identifier = _identifier;
	expr = _expr;
}

LetStatement::~LetStatement() { delete expr; }

SharedValue LetStatement::Execute(Environment* _env)
{
	SharedValue val = expr->Evaluate(_env);

	if(val->_type == VEXCEPTION) { return val; }
	else { return _env->AddVariable(identifier, SharedValue(val->GetCopy()), _position); }
}

IStatement* LetStatement::GetCopy() { return new LetStatement(identifier, expr->GetCopy(), _position); }
#pragma endregion

#pragma region Assignment
Assignment::Assignment(std::string _identifier, IExpression* _expr, bool _isLet, Position _pos)
	: IStatement(_pos, SASSIGNMENT)
{
	identifier = _identifier;
	isLetAssignment = _isLet;
	expr = _expr;
}

Assignment::~Assignment() { delete expr; }

SharedValue Assignment::Execute(Environment* _env)
{
	SharedValue val = expr->Evaluate(_env);

	if(val->_type == VEXCEPTION)
		return val;

	SharedValue assignResult(nullptr);

	if(isLetAssignment) { assignResult = _env->AddVariable(identifier, SharedValue(val->GetCopy()), _position); /* Returns void value if successful */ }
	else { assignResult = _env->SetValue(identifier, SharedValue(val->GetCopy()), _position); /* Returns void value if successful */ }

	return assignResult;
}

IStatement* Assignment::GetCopy() { return new Assignment(identifier, expr->GetCopy(), isLetAssignment, _position); }
#pragma endregion

#pragma region ForLoop
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

SharedValue ForLoop::Execute(Environment* _env)
{
	Environment env(_env, "//");

	SharedValue initAssignVal = initAssignment->Execute(&env);

	if(initAssignVal->_type == VEXCEPTION)
		return initAssignVal;

	PRINT_VAL(initAssignVal, &env);
	PRINT_LINE(initAssignment->_position.line);

	while(true)
	{
		auto evalCondVal = std::shared_ptr<IValue>(conditionExpr->Evaluate(&env));

		if(evalCondVal->_type == VEXCEPTION) { return evalCondVal; }
		else if(evalCondVal->_type != VBOOL) { return std::shared_ptr<IValue>(Exception_MismatchType("bool", evalCondVal->GetTypeSig(), Trace(evalCondVal->_position, _env->name, "Put FileName Here"))); }

		PRINT_VAL(evalCondVal, &env);
		PRINT_LINE(conditionExpr->_position.line);

		if(!((BoolValue*)evalCondVal.get())->value)
			break;

		SharedValue statementVal = statement->Execute(&env);

		if(statementVal->_type == VEXCEPTION)
			return statementVal;

		PRINT_VAL(statementVal, &env);
		PRINT_LINE(statement->_position.line);

		SharedValue finallyAssignVal = finallyAssign->Execute(&env);

		if(finallyAssignVal->_type == VEXCEPTION)
			return finallyAssignVal;

		PRINT_VAL(finallyAssignVal, &env);
		PRINT_LINE(finallyAssign->_position.line);
	}

	return std::shared_ptr<IValue>(new VoidValue(_position));
}

IStatement* ForLoop::GetCopy() { return new ForLoop((Assignment*)initAssignment->GetCopy(), conditionExpr->GetCopy(), statement->GetCopy(), (Assignment*)finallyAssign->GetCopy(), _position); }
#pragma endregion

#pragma region MemberDefinition
MemberDefinition::MemberDefinition(std::string _identifier, std::string _typeDef, Position _pos)
	: IStatement(_pos, SMEMBER_DEF)
{
	definition = Definition(_identifier, _typeDef);
}

SharedValue MemberDefinition::Execute(Environment* _env)
{
	SharedValue val = _env->GetValue(definition.first, _position, true);

	if(val->_type == VEXCEPTION)
		return val;

	bool hasType = val->GetTypeSig() == definition.second;
	return std::shared_ptr<IValue>(new BoolValue(hasType, _position));
}

IStatement* MemberDefinition::GetCopy() { return new MemberDefinition(definition.first, definition.second, _position); }
#pragma endregion

#pragma region TypeDefinition
TypeDefinition::TypeDefinition(std::string _identifier, DefinitionMap& _memDefs, Position _pos)
	: IStatement(_pos, STYPE_DEF)
{
	identifier = _identifier;
	memDefs = _memDefs;
}

SharedValue TypeDefinition::Execute(Environment* _env) { return _env->AddTypeDefinition(identifier, memDefs, _position); }
IStatement* TypeDefinition::GetCopy() { return new TypeDefinition(identifier, memDefs, _position); }
#pragma endregion

#pragma region FuncDeclaration
FuncDeclaration::FuncDeclaration(std::string _identifier, DefinitionList& _argDefs, TypeName _retTypeName, IStatement* _body, Position _pos)
	: IStatement(_pos, SFUNC_DECL)
{
	identifier = _identifier;
	argDefs = _argDefs;
	retTypeName = _retTypeName;
	body = std::shared_ptr<IStatement>(_body);
}

SharedValue FuncDeclaration::Execute(Environment* _env)
{
	std::vector<std::string> argNames, argSigs;
	TypeSig retSig;

	for(auto it : argDefs)
	{
		SharedValue val = _env->GetTypeSig(it.second, _position);

		if(val->_type == VEXCEPTION) { return val; }
		else
		{
			argNames.push_back(it.first);
			argSigs.push_back(((StringValue*)val.get())->value);
		}
	}

	SharedValue val = _env->GetTypeSig(retTypeName, _position);

	if(val->_type == VEXCEPTION) { return val; }
	else { retSig = ((StringValue*)val.get())->value; }

	SharedValue func = std::shared_ptr<IValue>(new FuncValue(_env, body, argNames, argSigs, retSig, _position));
	return _env->AddFuncDeclaration(identifier, func, _position);
}

IStatement* FuncDeclaration::GetCopy() { return new FuncDeclaration(identifier, argDefs, retTypeName, body->GetCopy(), _position); }
#pragma endregion