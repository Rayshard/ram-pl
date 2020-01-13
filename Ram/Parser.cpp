#include "pch.h"
#include "Parser.h"
#include "Value.h"
#include "Statement.h"
#include "Expression.h"
#include "Environment.h"

ExpressionResult GetExprList(TokenReader _reader, TokenType _delimiter, std::vector<IExpression*>& _outList)
{
	ExpressionResult exprResult = GetExpression(_reader); // Expecting at least one arg

	if(exprResult.success)
	{
		TokenReader reader = exprResult.reader;
		std::vector<IExpression*> exprList({ exprResult.value });

		while(true)
		{
			if(reader.GetCurType() == _delimiter)
			{
				reader.Advance();
				exprResult = GetExpression(reader);

				if(exprResult.success)
				{
					reader = exprResult.reader;
					exprList.push_back(exprResult.value);
				}
				else
				{
					for(auto it : exprList)
						delete it;

					return ExpressionResult::GenFailure("Expected expression.", reader);
				}
			}
			else { break; }
		}

		_outList = exprList;
		return ExpressionResult::GenSuccess(nullptr, reader.GetCurPtr());
	}
	else { return ExpressionResult::GenFailure(exprResult.message, _reader); }
}

ExpressionResult GetFuncCall(IExpression* _base, TokenReader _reader)
{
	if(_reader.GetCurType() == TT_LPAREN)
	{
		Position pos = _reader.GetCurPosition();
		TokenReader reader = TokenReader(_reader.Advance());
		std::vector<IExpression*> argExprs;

		if(reader.GetCurType() != TT_RPAREN) // Expecting at least on arg
		{
			ExpressionResult argExprsResult = GetExprList(reader, TT_COMMA, argExprs);

			if(argExprsResult.success) { reader = argExprsResult.reader; }
			else { return argExprsResult; }
		}

		if(reader.GetCurType() == TT_RPAREN)
		{
			FuncCallExpression* funcCallExpr = new FuncCallExpression(_base, argExprs, pos);
			return ExpressionResult::GenSuccess(funcCallExpr, reader.Advance());
		}
		else
		{
			for(auto it : argExprs)
				delete it;

			return ExpressionResult::GenFailure("Expected ')'.", reader);
		}
	}
	else { return ExpressionResult::GenFailure("Expected '('.", _reader); }
}

ExpressionResult GetArrayIndex(IExpression* _base, TokenReader _reader)
{
	if(_reader.GetCurType() == TT_LSQRBRACKET)
	{
		Position pos = _reader.GetCurPosition();
		TokenReader reader = TokenReader(_reader.Advance());
		std::vector<IExpression*> argExprs;

		if(reader.GetCurType() != TT_RSQRBRACKET) // Needs at least on arg
		{
			ExpressionResult indexExpr = GetExpression(reader);

			if(indexExpr.success)
			{
				reader = indexExpr.reader;

				if(reader.GetCurType() == TT_RSQRBRACKET)
				{
					ArrayIndexExpression* arrayIndexExpr = new ArrayIndexExpression(_base, indexExpr.value, pos);
					return ExpressionResult::GenSuccess(arrayIndexExpr, reader.Advance());
				}
				else
				{
					delete indexExpr.value;
					return ExpressionResult::GenFailure("Expected ']'.", reader);
				}
			}
			else { return indexExpr; }
		}
		else { return ExpressionResult::GenFailure("Expected index expression.", reader); }
	}
	else { return ExpressionResult::GenFailure("Expected '['.", _reader); }
}

ExpressionResult GetAccess(IExpression* _base, TokenReader _reader)
{
	if(_reader.GetCurType() == TT_PERIOD)
	{
		TokenReader reader = TokenReader(_reader.Advance());
		ExpressionResult memIdResult = GetIdentifier(reader);

		if(memIdResult.success)
		{
			std::string memId = ((SymbolExpression*)memIdResult.value)->symbol;
			AccessExpression* accessExpr = new AccessExpression(_base, memId, reader.GetCurPosition());

			delete memIdResult.value;
			return ExpressionResult::GenSuccess(accessExpr, memIdResult.reader.GetCurPtr());
		}
		else { return memIdResult; }
	}
	else { return ExpressionResult::GenFailure("Expected '.'.", _reader); }
}

ExpressionResult GetMembered(TokenReader _reader)
{
	auto parser = [](TokenReader _r) { return GetLet(_r, false); }; // Looks only for let statments without the let keyword
	StatementResult result = GetCodeBlock(_reader, IValue::SIGNATURE_MEMBERED, parser);

	if(result.success)
	{
		TokenReader reader = result.reader;
		auto assignStmnts = ((CodeBlock*)result.value)->statements;
		std::map<std::string, IExpression*> memAssigns;

		for(auto stmnt : assignStmnts)
		{
			std::string identifier = ((LetStatement*)stmnt)->identifier;
			IExpression* expr = ((LetStatement*)stmnt)->expr;

			if(memAssigns.find(identifier) != memAssigns.end())
			{
				ExpressionResult exception = ExpressionResult::GenFailure("The member '" + identifier + "' was already assigned.", reader);
				delete result.value;
				return exception;
			}
			else { memAssigns.insert_or_assign(identifier, expr->GetCopy()); }
		}

		MemberedExpression* memExpr = new MemberedExpression(memAssigns, _reader.GetCurPosition());

		delete result.value;
		return ExpressionResult::GenSuccess(memExpr, result.reader.GetCurPtr());
	}
	else { return ExpressionResult::GenFailure(result.message, result.reader); }
}

ExpressionResult GetSubExpression(TokenReader _reader)
{
	if(_reader.GetCurType() == TT_LPAREN)
	{
		TokenReader reader = TokenReader(_reader.Advance());
		ExpressionResult result = GetExpression(reader);

		if(result.success)
		{
			if(result.reader.GetCurType() == TT_RPAREN) { return ExpressionResult::GenSuccess(result.value, result.reader.Advance()); }
			else
			{
				delete result.value;
				return ExpressionResult::GenFailure("Expected ')'.", reader);
			}
		}
		else { return result; }
	}
	else { return ExpressionResult::GenFailure("Expected '('.", _reader); }
}

ExpressionResult GetIdentifier(TokenReader _reader)
{
	if(_reader.GetCurType() == TT_IDENTIFIER)
	{
		IExpression* expr = new SymbolExpression(_reader.current.value, _reader.GetCurPosition());
		return ExpressionResult::GenSuccess(expr, _reader.Advance());
	}
	else { return ExpressionResult::GenFailure("Expected identifier.", _reader); }
}

ExpressionResult GetArray(TokenReader _reader)
{
	if(_reader.GetCurType() == TT_LSQRBRACKET)
	{
		Position pos = _reader.GetCurPosition();
		TokenReader reader = TokenReader(_reader.Advance());
		std::vector<IExpression*> elemExprs;

		if(_reader.GetCurType() != TT_RSQRBRACKET) // Expected at least one expression
		{
			ExpressionResult exprsResult = GetExprList(reader, TT_COMMA, elemExprs);

			if(exprsResult.success) { reader = exprsResult.reader; }
			else { return exprsResult; }
		}

		if(reader.GetCurType() == TT_RSQRBRACKET)
		{
			ArrayExpression* arrayExpr = new ArrayExpression(elemExprs, pos);
			return ExpressionResult::GenSuccess(arrayExpr, reader.Advance());
		}
		else
		{
			for(auto it : elemExprs)
				delete it;

			return ExpressionResult::GenFailure("Expected ']'.", reader);
		}
	}
	else { return ExpressionResult::GenFailure("Expected '['.", _reader); }
}

ExpressionResult GetTypeName(TokenReader _reader, std::string& _outTypeName)
{
	TokenReader reader = TokenReader(_reader.GetCurPtr());
	Position pos = reader.GetCurPosition();
	std::string typeName = "";

	if(reader.GetCurType() == TT_LSQRBRACKET)
		return GetArrayTypeName(_reader, _outTypeName);

	while(true)
	{
		if(IsTypeName(reader.GetCurType()))
		{
			typeName += reader.current.value;
			reader.Advance();

			if(reader.GetCurType() == TT_PERIOD)
			{
				reader.Advance();
				typeName += ".";
			}
			else
			{
				_outTypeName = typeName;
				return ExpressionResult::GenSuccess(nullptr, reader.GetCurPtr());
			}
		}
		else { return ExpressionResult::GenFailure("Expected type name.", reader); }
	}
}

ExpressionResult GetArrayTypeName(TokenReader _reader, std::string& _outTypeName)
{
	if(_reader.GetCurType() == TT_LSQRBRACKET)
	{
		_reader.Advance();
		std::string typeName;
		ExpressionResult innerResult = GetTypeName(_reader, typeName);

		if(innerResult.success)
		{
			TokenReader reader = innerResult.reader;

			if(reader.GetCurType() == TT_RSQRBRACKET)
			{
				_outTypeName = typeName + "|" + IValue::SIGNATURE_ARRAY;
				return ExpressionResult::GenSuccess(nullptr, reader.Advance());
			}
			else { return ExpressionResult::GenFailure("Expected ']'.", reader); }
		}
		else { return innerResult; }
	}
	else { return ExpressionResult::GenFailure("Expected '['.", _reader); }
}

ExpressionResult GetFuncSignature(TokenReader _reader, std::vector<std::string>& _argTypeNames, std::string& _retTypeName)
{
	if(_reader.GetCurType() == TT_LPAREN)
	{
		TokenReader reader = TokenReader(_reader.Advance());
		std::vector<std::string> argTypeNames;

		if(reader.GetCurType() != TT_RPAREN) // Expecting at least one arg
		{
			while(true)
			{
				std::string argTypeName;
				ExpressionResult argTypeNameResult = GetTypeName(reader, argTypeName);

				if(argTypeNameResult.success)
				{
					argTypeNames.push_back(argTypeName);
					reader = argTypeNameResult.reader;

					if(reader.GetCurType() == TT_COMMA) { reader.Advance(); }
					else if(reader.GetCurType() == TT_RPAREN)
					{
						reader.Advance();
						break;
					}
					else { return ExpressionResult::GenFailure("Expected ')'.", reader); }
				}
				else { return argTypeNameResult; }
			}
		}
		else
		{
			argTypeNames.push_back("void");
			reader.Advance();
		}

		if(reader.GetCurType() == TT_COLON) { reader.Advance(); }
		else { return ExpressionResult::GenFailure("Expected ':'.", reader); }

		std::string retTypeName;
		ExpressionResult retTypeNameResult = GetTypeName(reader, retTypeName);

		if(retTypeNameResult.success)
		{
			_argTypeNames = argTypeNames;
			_retTypeName = retTypeName;
			return ExpressionResult::GenSuccess(nullptr, retTypeNameResult.reader.GetCurPtr());
		}
		else { return retTypeNameResult; }
	}
	else { return ExpressionResult::GenFailure("Expected '('", _reader); }
}

ExpressionResult GetSingular(TokenReader _reader)
{
	Token token = _reader.current;
	ExpressionResult result = ExpressionResult::GenFailure("", _reader);

	if(token.type == TT_LPAREN) { result = GetSubExpression(_reader); }
	else if(token.type == TT_IDENTIFIER) { result = GetIdentifier(_reader); }
	else if(token.type == TT_LBRACKET) { result = GetMembered(_reader); }
	else if(token.type == TT_LSQRBRACKET) { result = GetArray(_reader); }
	else
	{
		IExpression* expr = nullptr;

		if(token.type == TT_INTEGER_LIT) { expr = new ValueExpression(new IntValue(std::stoi(token.value), token.position)); }
		else if(token.type == TT_FLOAT_LIT) { expr = new ValueExpression(new FloatValue(std::stof(token.value), token.position)); }
		else if(token.type == TT_STRING_LIT) { expr = new ValueExpression(new StringValue(token.value, token.position)); }
		else if(token.type == TT_TRUE) { expr = new ValueExpression(new BoolValue(true, token.position)); }
		else if(token.type == TT_FALSE) { expr = new ValueExpression(new BoolValue(false, token.position)); }
		else { result = ExpressionResult::GenFailure("Value could not be parsed.", _reader); }

		if(expr)
			result = ExpressionResult::GenSuccess(expr, _reader.Advance());
	}

	if(!result.success)
		return result;

	TokenReader reader = result.reader;
	IExpression* singular = result.value;

	// Get as an Access Expression if there are accessed members that follow 
	while(true)
	{
		ExpressionResult result = ExpressionResult::GenFailure("", reader);

		if(reader.GetCurType() == TT_PERIOD) { result = GetAccess(singular, reader); }
		else if(reader.GetCurType() == TT_LPAREN) { result = GetFuncCall(singular, reader); }
		else if(reader.GetCurType() == TT_LSQRBRACKET) { result = GetArrayIndex(singular, reader); }
		else { break; }

		if(result.success)
		{
			reader = result.reader;
			singular = result.value;
		}
		else
		{
			delete singular;
			return result;
		}
	}

	if(reader.GetCurType() == TT_INC)
	{
		singular = new UnopExpression(singular, UnopExpression::INC, singular->_position);
		reader.Advance();
	}
	else if(reader.GetCurType() == TT_DEC)
	{
		singular = new UnopExpression(singular, UnopExpression::DEC, singular->_position);
		reader.Advance();
	}

	return ExpressionResult::GenSuccess(singular, reader.GetCurPtr());
}

ExpressionResult GetCast(TokenReader _reader)
{
	if(_reader.GetCurType() == TT_LT)
	{
		Position pos = _reader.GetCurPosition();
		TokenReader reader = TokenReader(_reader.Advance());
		std::string typeName;
		ExpressionResult typeNameResult = GetTypeName(reader, typeName);

		if(typeNameResult.success) { reader = typeNameResult.reader; }
		else { return typeNameResult; }

		if(reader.GetCurType() == TT_GT)
		{
			reader.Advance();

			ExpressionResult factorResult = GetFactor(reader);

			if(factorResult.success)
			{
				CastExpression* castExpr = new CastExpression(typeName, factorResult.value, pos);
				return ExpressionResult::GenSuccess(castExpr, factorResult.reader.GetCurPtr());
			}
			else { return factorResult; }
		}
		else { return ExpressionResult::GenFailure("Expected '>'.", reader); }
	}
	else { return ExpressionResult::GenFailure("Expected '<'.", _reader); }
}

ExpressionResult GetFactor(TokenReader _reader)
{
	if(_reader.GetCurType() == TT_LT) { return GetCast(_reader); }
	else
	{
		Position pos = _reader.GetCurPosition();
		TokenType prefix = _reader.GetCurType();
		bool prefixed = false;

		if(IsUnop(prefix))
		{
			prefixed = true;
			_reader.Advance();
		}

		ExpressionResult result = GetSingular(_reader);

		if(result.success)
		{
			TokenReader reader = result.reader;
			IExpression* factorExpr = nullptr;

			if(prefixed) { factorExpr = new UnopExpression(result.value, UnopExpression::TokenTypeToOp(prefix), pos); }
			else { factorExpr = result.value; }

			return ExpressionResult::GenSuccess(factorExpr, reader.GetCurPtr());
		}
		else { return result; }
	}
}

ExpressionResult GetTerm(TokenReader _reader)
{
	ExpressionResult leftResult = GetFactor(_reader);

	if(leftResult.success)
	{
		TokenReader reader = leftResult.reader;
		TokenType tokenType = reader.GetCurType();

		if(tokenType == TT_TIMES || tokenType == TT_DIVIDE)
		{
			reader.Advance();
			ExpressionResult rightResult = GetTerm(reader);

			if(rightResult.success)
			{
				BinopExpression::OP operation = BinopExpression::TokenTypeToOp(tokenType);
				BinopExpression* binopExpr = new BinopExpression(leftResult.value, rightResult.value, operation, leftResult.reader.GetCurPosition());

				return ExpressionResult::GenSuccess(binopExpr, rightResult.reader.GetCurPtr());
			}
			else { return rightResult; }
		}
		else { return leftResult; }
	}
	else { return leftResult; }
}

ExpressionResult GetBinopExpr(TokenReader _reader)
{
	ExpressionResult leftResult = GetTerm(_reader);

	if(leftResult.success)
	{
		TokenReader reader = leftResult.reader;
		TokenType tokenType = reader.GetCurType();

		if(tokenType == TT_PLUS || tokenType == TT_MINUS)
		{
			reader.Advance();
			ExpressionResult rightResult = GetBinopExpr(reader);

			if(rightResult.success)
			{
				BinopExpression::OP operation = BinopExpression::TokenTypeToOp(tokenType);
				BinopExpression* binopExpr = new BinopExpression(leftResult.value, rightResult.value, operation, leftResult.reader.GetCurPosition());

				return ExpressionResult::GenSuccess(binopExpr, rightResult.reader.GetCurPtr());
			}
			else { return rightResult; }
		}
		else { return leftResult; }
	}
	else { return leftResult; }
}

ExpressionResult GetExpression(TokenReader _reader)
{
	ExpressionResult leftResult = GetBinopExpr(_reader);

	if(leftResult.success)
	{
		TokenReader reader = leftResult.reader;
		TokenType tokenType = reader.GetCurType();

		if(tokenType == TT_LT || tokenType == TT_GT || tokenType == TT_LT_EQ || tokenType == TT_GT_EQ)
		{
			reader.Advance();
			ExpressionResult rightResult = GetBinopExpr(reader);

			if(rightResult.success)
			{
				BinopExpression::OP operation = BinopExpression::TokenTypeToOp(tokenType);
				BinopExpression* binopExpr = new BinopExpression(leftResult.value, rightResult.value, operation, leftResult.reader.GetCurPosition());

				return ExpressionResult::GenSuccess(binopExpr, rightResult.reader.GetCurPtr());
			}
			else { return rightResult; }
		}
		else { return leftResult; }
	}
	else { return leftResult; }
}

StatementResult GetControlFlow(TokenReader _reader)
{
	Position pos = _reader.GetCurPosition();

	if(_reader.GetCurType() == TT_RETURN)
	{
		IExpression* expr = nullptr;
		Token* remainingTokens = _reader.Advance();
		ExpressionResult result = GetExpression(remainingTokens);

		if(result.success)
		{
			expr = result.value;
			remainingTokens = result.reader.GetCurPtr();
		}
		else { expr = new ValueExpression(new VoidValue(pos)); }

		LetStatement* statement = new LetStatement("return", expr, pos);
		statement->_type = SRETURN;
		return StatementResult::GenSuccess(statement, remainingTokens);
	}
	else if(_reader.GetCurType() == TT_BREAK)
	{
		LetStatement* statement = new LetStatement("break", new ValueExpression(new BoolValue(true, pos)), pos);
		statement->_type = SBREAK;

		return StatementResult::GenSuccess(statement, _reader.Advance());
	}
	else if(_reader.GetCurType() == TT_CONTINUE)
	{
		LetStatement* statement = new LetStatement("continue", new ValueExpression(new BoolValue(true, pos)), pos);
		statement->_type = SCONTINUE;

		return StatementResult::GenSuccess(statement, _reader.Advance());
	}
	else { return StatementResult::GenFailure("Expected a 'return', 'break', or 'continue'.", _reader); }
}

StatementResult GetAssignment(TokenReader _reader, IExpression* _base)
{
	if(_reader.GetCurType() == TT_EQ)
	{
		_reader.Advance();
		ExpressionResult result = GetExpression(_reader);

		if(result.success)
		{
			Assignment* assignment = new Assignment(_base, result.value, _base->_position);
			return StatementResult::GenSuccess(assignment, result.reader.GetCurPtr());
		}
		else { return StatementResult::GenFailure(result.message, _reader); }
	}
	else { return StatementResult::GenFailure("Expected '='.", _reader); }
}

StatementResult GetLet(TokenReader _reader, bool _reqKeyword = true)
{
	if(_reader.GetCurType() == TT_LET || !_reqKeyword)
	{
		Position pos = _reader.GetCurPosition();
		TokenReader reader = TokenReader(_reqKeyword ? _reader.Advance() : _reader.GetCurPtr());

		if(reader.GetCurType() == TT_IDENTIFIER)
		{
			std::string identifier = reader.current.value;
			reader.Advance();

			if(reader.GetCurType() == TT_EQ)
			{
				reader.Advance();

				ExpressionResult result = GetExpression(reader);

				if(result.success)
				{
					LetStatement* letStmnt = new LetStatement(identifier, result.value, pos);
					return StatementResult::GenSuccess(letStmnt, result.reader.GetCurPtr());
				}
				else { return StatementResult::GenFailure(result.message, reader); }
			}
			else { return StatementResult::GenFailure("Expected '='.", reader); }
		}
		else { return StatementResult::GenFailure("Expected identifier.", _reader); }
	}
	else { return StatementResult::GenFailure("Expected keyword 'let'.", _reader); }
}

StatementResult GetDefinition(TokenReader _reader)
{
	if(_reader.GetCurType() == TT_IDENTIFIER)
	{
		Position pos = _reader.GetCurPosition();
		std::string identifier = _reader.current.value;
		TokenReader reader = TokenReader(_reader.Advance());

		if(reader.GetCurType() == TT_COLON)
		{
			reader.Advance();
			std::string typeName;
			ExpressionResult typeNameResult = GetTypeName(reader, typeName);

			if(typeNameResult.success)
			{
				reader = typeNameResult.reader;

				MemberDefinition* memDef = new MemberDefinition(identifier, typeName, pos);
				return StatementResult::GenSuccess(memDef, reader.GetCurPtr());
			}
			else { return StatementResult::GenFailure(typeNameResult.message, reader); }
		}
		else { return StatementResult::GenFailure("Expected : after identifier.", reader); }
	}
	else { return StatementResult::GenFailure("Expected identifier.", _reader); }
}

StatementResult GetCodeBlock(TokenReader _reader, std::string _name, std::function<StatementResult(TokenReader)> _parser)
{
	if(_reader.GetCurType() == TT_LBRACKET)
	{
		Position pos = _reader.GetCurPosition();
		TokenReader reader = TokenReader(_reader.Advance());
		std::vector<IStatement*> statements;

		if(reader.GetCurType() == TT_RBRACKET)
		{
			reader.Advance();
			return StatementResult::GenSuccess(new CodeBlock(statements, pos, _name), reader.GetCurPtr());
		}

		while(!reader.AtEOF())
		{
			StatementResult result = _parser(reader);

			if(result.success)
			{
				reader = result.reader;

				if(reader.GetCurType() == TT_RBRACKET)
				{
					statements.push_back(result.value);
					reader.Advance();
					return StatementResult::GenSuccess(new CodeBlock(statements, pos, _name), reader.GetCurPtr());
				}
				else if(reader.GetCurType() == TT_SEMICOLON)
				{
					statements.push_back(result.value);
					reader.Advance();

					if(reader.GetCurType() == TT_RBRACKET)
					{
						reader.Advance();
						return StatementResult::GenSuccess(new CodeBlock(statements, pos, _name), reader.GetCurPtr());
					}
				}
				else { return StatementResult::GenFailure("Expected ';'. ", reader); }
			}
			else { return result; }
		}

		return StatementResult::GenFailure("Expected '}'. ", reader);
	}
	else { return StatementResult::GenFailure("Expected '{'. ", _reader); }
}

StatementResult GetSimpleStatement(TokenReader _reader)
{
	ExpressionResult result = GetExpression(_reader);

	if(result.success) { return StatementResult::GenSuccess(new ExprStatement(result.value), result.reader.GetCurPtr()); }
	else { return StatementResult::GenFailure(result.message, result.reader); }
}

StatementResult GetIfStatement(TokenReader _reader)
{
	if(_reader.GetCurType() == TT_IF)
	{
		Position pos = _reader.GetCurPosition();
		ExpressionResult condResult = GetExpression(_reader.Advance());

		if(condResult.success)
		{
			TokenReader reader = condResult.reader;
			if(reader.GetCurType() == TT_THEN)
			{
				reader.Advance();
				StatementResult thenResult = GetStatement(reader);

				if(thenResult.success)
				{
					reader = thenResult.reader;
					IStatement* elseBranch = nullptr;

					if(reader.GetCurType() == TT_ELSE)
					{
						reader.Advance();

						StatementResult elseResult = GetStatement(reader);

						if(elseResult.success)
						{
							elseBranch = elseResult.value;
							reader = elseResult.reader;
						}
						else { return elseResult; }
					}

					IExpression* condExpr = condResult.value;
					IStatement* thenBranch = thenResult.value;

					if(thenBranch->_type == SCODE_BLOCK) { ((CodeBlock*)thenBranch)->name = "<IF>"; }
					if(elseBranch && (elseBranch->_type == SCODE_BLOCK)) { ((CodeBlock*)elseBranch)->name = "<ELSE>"; }

					IfStatement* ifStatement = new IfStatement(condExpr, thenBranch, elseBranch, pos);
					return StatementResult::GenSuccess(ifStatement, reader.GetCurPtr());
				}
				else { return thenResult; }
			}
			else { return StatementResult::GenFailure("Expected keyword 'then'.", reader); }
		}
		else { return StatementResult::GenFailure(condResult.message, _reader); }
	}
	else { return StatementResult::GenFailure("Expected keyword 'if'.", _reader); }
}

StatementResult GetForLoop(TokenReader _reader)
{
	if(_reader.GetCurType() == TT_FOR)
	{
		Position pos = _reader.GetCurPosition();
		TokenReader reader = TokenReader(_reader.Advance());
		StatementResult assignResult = GetLet(reader, false);

		if(assignResult.success)
		{
			reader = assignResult.reader;
			if(reader.GetCurType() == TT_IF)
			{
				reader.Advance();
				ExpressionResult condResult = GetExpression(reader);

				if(condResult.success)
				{
					reader = condResult.reader;
					if(reader.GetCurType() == TT_THEN)
					{
						reader.Advance();
						StatementResult stmtResult = GetStatement(reader);

						if(stmtResult.success)
						{
							reader = stmtResult.reader;
							if(reader.GetCurType() == TT_FINALLY)
							{
								reader.Advance();
								StatementResult finResult = GetStatement(reader);

								if(finResult.success)
								{
									LetStatement* initLet = (LetStatement*)assignResult.value;
									IExpression* condExpr = condResult.value;
									IStatement* body = stmtResult.value;
									IStatement* finStmt = finResult.value;

									if(body->_type == SCODE_BLOCK)
										((CodeBlock*)body)->name = "<FORLOOP>";

									ForLoop* forLoop = new ForLoop(initLet, condExpr, body, finStmt, pos);
									return StatementResult::GenSuccess(forLoop, finResult.reader.GetCurPtr());
								}
								else { return finResult; }
							}
							else { return StatementResult::GenFailure("Expected keyword 'finally'.", reader); }
						}
						else { return stmtResult; }
					}
					else { return StatementResult::GenFailure("Expected keyword 'then'.", reader); }
				}
				else { return StatementResult::GenFailure(condResult.message, reader); }
			}
			else { return StatementResult::GenFailure("Expected keyword 'if'.", reader); }
		}
		else { return assignResult; }
	}
	else { return StatementResult::GenFailure("Expected keyword 'for'.", _reader); }
}

StatementResult GetWhile(TokenReader _reader)
{
	if(_reader.GetCurType() == TT_WHILE)
	{
		Position pos = _reader.GetCurPosition();
		ExpressionResult condResult = GetExpression(_reader.Advance());

		if(condResult.success)
		{
			TokenReader reader = condResult.reader;

			if(reader.GetCurType() == TT_DO)
			{
				reader.Advance();
				StatementResult stmtResult = GetStatement(reader);

				if(stmtResult.success)
				{
					reader = stmtResult.reader;

					IExpression* condExpr = condResult.value;
					IStatement* body = stmtResult.value;

					if(body->_type == SCODE_BLOCK)
						((CodeBlock*)body)->name = "<WHILE>";

					WhileStatement* whileStmnt = new WhileStatement(condExpr, body, pos);
					return StatementResult::GenSuccess(whileStmnt, stmtResult.reader.GetCurPtr());
				}
				else { return stmtResult; }
			}
			else { return StatementResult::GenFailure("Expected keyword 'do'.", reader); }
		}
		else { return StatementResult::GenFailure(condResult.message, _reader); }
	}
	else { return StatementResult::GenFailure("Expected keyword 'while'.", _reader); }
}

StatementResult GetFuncDeclaration(TokenReader _reader)
{
	if(_reader.GetCurType() == TT_FUNC)
	{
		Position pos = _reader.GetCurPosition();
		TokenReader reader = TokenReader(_reader.Advance());

		if(reader.GetCurType() == TT_IDENTIFIER)
		{
			std::string identifier = reader.current.value;
			reader.Advance();

			if(reader.GetCurType() == TT_LPAREN)
			{
				reader.Advance();
				std::vector<Definition> argDefs;
				std::vector<std::string> usedArgNames;

				if(reader.GetCurType() == TT_RPAREN) { reader.Advance(); }
				else
				{
					while(!reader.AtEOF())
					{
						StatementResult result = GetDefinition(reader);

						if(result.success)
						{
							Definition argDef = ((MemberDefinition*)result.value)->definition;

							reader = result.reader;
							delete result.value;

							if(std::find(usedArgNames.begin(), usedArgNames.end(), argDef.first) != usedArgNames.end())
								return StatementResult::GenFailure("The argument named '" + argDef.first + "' was already used.", reader);

							argDefs.push_back(argDef);
							usedArgNames.push_back(argDef.first);

							if(reader.GetCurType() == TT_COMMA) { reader.Advance(); }
							else if(reader.GetCurType() == TT_RPAREN)
							{
								reader.Advance();
								break;
							}
							else { return StatementResult::GenFailure("Expected ')'.", reader); }
						}
						else { return result; }
					}
				}

				if(reader.GetCurType() == TT_COLON)
				{
					reader.Advance();

					if(IsTypeName(reader.GetCurType()))
					{
						std::string retTypeName = reader.current.value;
						reader.Advance();

						if(reader.GetCurType() == TT_EQ)
						{
							reader.Advance();

							StatementResult stmtResult = GetStatement(reader);

							if(stmtResult.success)
							{
								IStatement* body = stmtResult.value;

								if(body->_type == SCODE_BLOCK)
								{
									((CodeBlock*)body)->name = identifier;
									((CodeBlock*)body)->createSubEnv = false;
									((CodeBlock*)body)->canReturn = true;
									((CodeBlock*)body)->canBreak = false;
									((CodeBlock*)body)->canContinue = false;
								}

								FuncDeclaration* funcDecl = new FuncDeclaration(identifier, argDefs, retTypeName, body, pos);
								return StatementResult::GenSuccess(funcDecl, stmtResult.reader.GetCurPtr());
							}
							else { return stmtResult; }
						}
						else { return StatementResult::GenFailure("Expected '='. ", _reader); }
					}
					else { return StatementResult::GenFailure("Expected identifier.", _reader); }
				}
				else { return StatementResult::GenFailure("Expected ':'. ", _reader); }
			}
			else { return StatementResult::GenFailure("Expected '('. ", _reader); }
		}
		else { return StatementResult::GenFailure("Expected identifier.", _reader); }
	}
	else { return StatementResult::GenFailure("Expected keyword 'def'.", _reader); }
}

StatementResult GetTypeDefinition(TokenReader _reader)
{
	if(_reader.GetCurType() == TT_DEF)
	{
		Position pos = _reader.GetCurPosition();
		TokenReader reader = TokenReader(_reader.Advance());

		if(reader.GetCurType() == TT_IDENTIFIER)
		{
			std::string identifier = reader.current.value;
			reader.Advance();

			if(reader.GetCurType() == TT_EQ)
			{
				reader.Advance();

				if(reader.GetCurType() == TT_LBRACKET)
				{
					reader.Advance();
					std::map<std::string, std::string> memDefs;

					while(!reader.AtEOF())
					{
						StatementResult result = GetDefinition(reader);

						if(result.success)
						{
							Definition memDef = ((MemberDefinition*)result.value)->definition;

							reader = result.reader;
							delete result.value;

							if(memDefs.find(memDef.first) != memDefs.end())
								return StatementResult::GenFailure("The member '" + memDef.first + "' was already defined.", reader);

							TokenType lastTokenType = reader.GetCurType();

							if(lastTokenType == TT_SEMICOLON || lastTokenType == TT_RBRACKET)
							{
								memDefs.insert_or_assign(memDef.first, memDef.second);
								reader.Advance();

								if(lastTokenType == TT_RBRACKET || reader.GetCurType() == TT_RBRACKET)
								{
									if(reader.GetCurType() == TT_RBRACKET)
										reader.Advance();

									return StatementResult::GenSuccess(new TypeDefinition(identifier, memDefs, pos), reader.GetCurPtr());
								}
							}
							else { return StatementResult::GenFailure("Expected ';' or '}'.", reader); }
						}
						else { return result; }
					}

					return StatementResult::GenFailure("Expected '}'. ", reader);
				}
				else if(reader.GetCurType() == TT_LPAREN)
				{
					std::vector<std::string> argTypeNames;
					std::string retTypeName;
					ExpressionResult funcSigResult = GetFuncSignature(reader, argTypeNames, retTypeName);

					if(funcSigResult.success)
					{
						TypeDefinition* typeDefiniton = new TypeDefinition(identifier, argTypeNames, retTypeName, pos);
						return StatementResult::GenSuccess(typeDefiniton, funcSigResult.reader.GetCurPtr());
					}
					else { return StatementResult::GenFailure(funcSigResult.message, funcSigResult.reader); }
				}
				else { return StatementResult::GenFailure("Expected type definition. ", _reader); }
			}
			else { return StatementResult::GenFailure("Expected = after identifier.", reader); }
		}
		else { return StatementResult::GenFailure("Expected identifier.", _reader); }
	}
	else { return StatementResult::GenFailure("Expected keyword 'def'.", _reader); }
}

StatementResult GetStatement(TokenReader _reader)
{
	Token token = _reader.current;

	if(_reader.GetCurType() == TT_LBRACKET)
		return GetCodeBlock(_reader, "<CODEBLOCK>", GetStatement);

	if(_reader.GetCurType() == TT_LET)
		return GetLet(_reader);

	if(_reader.GetCurType() == TT_FOR)
		return GetForLoop(_reader);

	if(_reader.GetCurType() == TT_WHILE)
		return GetWhile(_reader);

	if(_reader.GetCurType() == TT_IF)
		return GetIfStatement(_reader);

	if(_reader.GetCurType() == TT_DEF)
		return GetTypeDefinition(_reader);

	if(_reader.GetCurType() == TT_FUNC)
		return GetFuncDeclaration(_reader);

	if(IsControlFLow(_reader.GetCurType()))
		return GetControlFlow(_reader);

	ExpressionResult result = GetExpression(_reader);

	if(result.success)
	{
		TokenReader reader = result.reader;

		if(reader.GetCurType() == TT_EQ) { return GetAssignment(reader, result.value); }
		else { return StatementResult::GenSuccess(new ExprStatement(result.value), reader.GetCurPtr()); }
	}
	else { return StatementResult::GenFailure(result.message, _reader); }
}

FileParseResult Parse(Token* _tokens)
{
	TokenReader reader = TokenReader(_tokens);
	std::vector<IStatement*> statements;
	bool inIncludes = false;

	while(!reader.AtEOF())
	{
		StatementResult result = GetStatement(reader);

		if(result.success)
		{
			reader = result.reader;

			if(reader.GetCurType() == TT_SEMICOLON)
			{
				statements.push_back(result.value);
				reader.Advance();
			}
			else { return FileParseResult::GenFailure("Expected ';'.", reader.GetCurPosition()); }
		}
		else { return FileParseResult::GenFailure(result.message, result.reader.GetCurPosition()); }
	}

	return FileParseResult::GenSuccess(statements);
}
