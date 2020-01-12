#include "pch.h"
#include "Statement.h"
#include "Expression.h"
#include "Value.h"
#include "Environment.h"
#include "Interpreter.h"

IStatement::IStatement(Position _pos, StatementType _type) : _position(_pos), _type(_type) {}
IStatement::~IStatement() {}

#pragma region ExprStatement
ExprStatement::ExprStatement(IExpression* _expr) : IStatement(_expr->_position, SEXPR) { expr = _expr; }
ExprStatement::~ExprStatement() { delete expr; }
SharedValue ExprStatement::Execute(Environment* _env) { return expr->Evaluate(_env); }
IStatement* ExprStatement::GetCopy() { return new ExprStatement(expr->GetCopy()); }
#pragma endregion

#pragma region CodeBlock
CodeBlock::CodeBlock(std::vector<IStatement*>& _statements, Position _pos, std::string _name)
	: IStatement(_pos, SCODE_BLOCK)
{
	name = _name;
	statements = _statements;
	createSubEnv = true;
	canReturn = true;
	canBreak = true;
	canContinue = true;
}

CodeBlock::~CodeBlock()
{
	for(auto it : statements)
		delete it;
}

SharedValue CodeBlock::Execute(Environment* _env)
{
	Environment* env = createSubEnv ? new Environment(_env, name) : _env;
	SharedValue returnVal(nullptr);

	for(auto statement : statements)
	{
		SharedValue val = statement->Execute(env);
		PRINT_LINE(env->filePath, statement->_position.line);

		if(val->GetType() == VEXCEPTION)
		{
			returnVal = val;
			break;
		}

		PRINT_VAL(val);

		if(canReturn && env->IsVariable("return", false)) { break; }
		else if(canBreak && env->IsVariable("break", false)) { break; }
		else if(canContinue && env->IsVariable("continue", false)) { break; }
	}

	if(createSubEnv)
		delete env;

	return returnVal ? returnVal : SHARE_VOID(_position);
}

IStatement* CodeBlock::GetCopy()
{
	std::vector<IStatement*> copy_Stmnts;

	for(auto it : statements)
		copy_Stmnts.push_back(it->GetCopy());

	return new CodeBlock(copy_Stmnts, _position, name);
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

	if(val->GetType() == VEXCEPTION) { return val; }
	else { return _env->AddVariable(identifier, SHARE_COPY(val), _position); }
}

IStatement* LetStatement::GetCopy() { return new LetStatement(identifier, expr->GetCopy(), _position); }
#pragma endregion

#pragma region Assignment
Assignment::Assignment(IExpression* _base, IExpression* _valExpr, Position _pos)
	: IStatement(_pos, SASSIGNMENT)
{
	base = _base;
	valExpr = _valExpr;
}

Assignment::~Assignment()
{
	delete base;
	delete valExpr;
}

SharedValue Assignment::Execute(Environment* _env)
{
	SharedValue baseVal = base->Evaluate(_env);

	if(baseVal->GetType() == VEXCEPTION)
		return baseVal;

	SharedValue val = valExpr->Evaluate(_env);

	if(val->GetType() == VEXCEPTION)
		return val;

	return baseVal->Set(_env, val->GetCopy(), _position);
}

IStatement* Assignment::GetCopy() { return new Assignment(base->GetCopy(), valExpr->GetCopy(), _position); }
#pragma endregion

#pragma region ForLoop
ForLoop::ForLoop(LetStatement* _initLet, IExpression* _conditionExpr, IStatement* _statement, IStatement* _finStmnt, Position _pos)
	: IStatement(_pos, SFOR_LOOP)
{
	initLet = _initLet;
	conditionExpr = _conditionExpr;
	statement = _statement;
	finallyStmnt = _finStmnt;

	if(statement->_type == SCODE_BLOCK)
	{
		((CodeBlock*)statement)->canReturn = true;
		((CodeBlock*)statement)->canBreak = true;
		((CodeBlock*)statement)->canContinue = true;
	}
}

ForLoop::~ForLoop()
{
	delete initLet;
	delete conditionExpr;
	delete statement;
	delete finallyStmnt;
}

SharedValue ForLoop::Execute(Environment* _env)
{
	Environment env(_env, "<FORLOOP>");
	env.propReturn = true;
	env.propBreak = false;
	env.propContinue = false;

	SharedValue initAssignVal = initLet->Execute(&env);

	if(initAssignVal->GetType() == VEXCEPTION)
		return initAssignVal;

	PRINT_VAL(initAssignVal);
	PRINT_LINE(env.filePath, initLet->_position.line);

	while(true)
	{
		auto evalCondVal = conditionExpr->Evaluate(&env);

		if(evalCondVal->GetType() == VEXCEPTION) { return evalCondVal; }
		else if(evalCondVal->GetType() != VBOOL) { return SHARE(Exception_MismatchType("bool", evalCondVal->GetTypeSig(), Trace(evalCondVal->GetPosition(), _env->name, _env->filePath))); }

		PRINT_VAL(evalCondVal);
		PRINT_LINE(env.filePath, conditionExpr->_position.line);

		if(!evalCondVal->AsBool()->value)
			break;

		SharedValue statementVal = statement->Execute(&env);

		if(statementVal->GetType() == VEXCEPTION)
			return statementVal;

		PRINT_VAL(statementVal);
		PRINT_LINE(env.filePath, statement->_position.line);

		if(env.IsVariable("break", false) || env.IsVariable("return", false))
			break;

		SharedValue finallyStmntVal = finallyStmnt->Execute(&env);

		if(finallyStmntVal->GetType() == VEXCEPTION)
			return finallyStmntVal;

		PRINT_VAL(finallyStmntVal);
		PRINT_LINE(env.filePath, finallyStmnt->_position.line);
	}

	return SHARE_VOID(_position);
}

IStatement* ForLoop::GetCopy() { return new ForLoop((LetStatement*)initLet->GetCopy(), conditionExpr->GetCopy(), statement->GetCopy(), finallyStmnt->GetCopy(), _position); }
#pragma endregion

#pragma region If
IfStatement::IfStatement(IExpression* _conditionExpr, IStatement* _thenBranch, IStatement* _elseBranch, Position _pos)
	: IStatement(_pos, SIF)
{
	conditionExpr = _conditionExpr;
	thenBranch = _thenBranch;
	elseBranch = _elseBranch;

	if(thenBranch->_type == SCODE_BLOCK)
	{
		((CodeBlock*)thenBranch)->canReturn = true;
		((CodeBlock*)thenBranch)->canBreak = false;
		((CodeBlock*)thenBranch)->canContinue = false;
	}

	if(elseBranch && (elseBranch->_type == SCODE_BLOCK))
	{
		((CodeBlock*)elseBranch)->canReturn = true;
		((CodeBlock*)elseBranch)->canBreak = false;
		((CodeBlock*)elseBranch)->canContinue = false;
	}
}

IfStatement::~IfStatement()
{
	delete conditionExpr;
	delete thenBranch;

	if(elseBranch)
		delete elseBranch;
}

SharedValue IfStatement::Execute(Environment* _env)
{
	Environment env(_env, "<IF>");
	env.propReturn = true;
	env.propBreak = true;
	env.propContinue = true;

	auto evalCondVal = conditionExpr->Evaluate(&env);

	if(evalCondVal->GetType() == VEXCEPTION) { return evalCondVal; }
	else if(evalCondVal->GetType() != VBOOL) { return SHARE(Exception_MismatchType("bool", evalCondVal->GetTypeSig(), Trace(evalCondVal->GetPosition(), _env->name, _env->filePath))); }

	PRINT_VAL(evalCondVal);
	PRINT_LINE(env.filePath, conditionExpr->_position.line);

	if(evalCondVal->AsBool()->value) { return thenBranch->Execute(&env); }
	else if(elseBranch) { return elseBranch->Execute(&env); }
	else { return SHARE_VOID(_position); }
}

IStatement* IfStatement::GetCopy() { return new IfStatement(conditionExpr->GetCopy(), thenBranch->GetCopy(), elseBranch->GetCopy(), _position); }
#pragma endregion

#pragma region While
WhileStatement::WhileStatement(IExpression* _conditionExpr, IStatement* _statement, Position _pos)
	: IStatement(_pos, SWHILE)
{
	conditionExpr = _conditionExpr;
	statement = _statement;

	if(statement->_type == SCODE_BLOCK)
	{
		((CodeBlock*)statement)->canReturn = true;
		((CodeBlock*)statement)->canBreak = true;
		((CodeBlock*)statement)->canContinue = true;
	}
}

WhileStatement::~WhileStatement()
{
	delete conditionExpr;
	delete statement;
}

SharedValue WhileStatement::Execute(Environment* _env)
{
	Environment env(_env, "<WHILE>");
	env.propReturn = true;
	env.propBreak = false;
	env.propContinue = false;

	while(true)
	{
		auto evalCondVal = conditionExpr->Evaluate(&env);

		if(evalCondVal->GetType() == VEXCEPTION) { return evalCondVal; }
		else if(evalCondVal->GetType() != VBOOL) { return SHARE(Exception_MismatchType("bool", evalCondVal->GetTypeSig(), Trace(evalCondVal->GetPosition(), _env->name, _env->filePath))); }

		PRINT_VAL(evalCondVal);
		PRINT_LINE(env.filePath, conditionExpr->_position.line);

		if(!evalCondVal->AsBool()->value)
			break;

		SharedValue statementVal = statement->Execute(&env);

		if(statementVal->GetType() == VEXCEPTION)
			return statementVal;

		PRINT_VAL(statementVal);
		PRINT_LINE(env.filePath, statement->_position.line);

		if(env.IsVariable("break", false) || env.IsVariable("return", false))
			break;
	}

	return SHARE_VOID(_position);
}

IStatement* WhileStatement::GetCopy() { return new WhileStatement(conditionExpr->GetCopy(), statement->GetCopy(), _position); }
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

	if(val->GetType() == VEXCEPTION)
		return val;

	bool hasType = val->GetTypeSig() == definition.second;
	return SHARE(new BoolValue(hasType, _position));
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

		if(val->GetType() == VEXCEPTION) { return val; }
		else
		{
			argNames.push_back(it.first);
			argSigs.push_back(val->AsString()->value);
		}
	}

	SharedValue val = _env->GetTypeSig(retTypeName, _position);

	if(val->GetType() == VEXCEPTION) { return val; }
	else { retSig = val->AsString()->value; }

	SharedValue func = SHARE(new FuncValue(_env, identifier, body, argNames, argSigs, retSig, _position));
	return _env->AddFuncDeclaration(identifier, func, _position);
}

IStatement* FuncDeclaration::GetCopy() { return new FuncDeclaration(identifier, argDefs, retTypeName, body->GetCopy(), _position); }
#pragma endregion