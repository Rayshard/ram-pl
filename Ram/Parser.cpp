#include "pch.h"
#include "Parser.h"
#include "Value.h"
#include "Statement.h"
#include "Expression.h"
#include "Environment.h"

ExpressionResult GetFuncCall(IExpression* _base, TokenReader _reader)
{
	if(_reader.GetCurType() == TT_LPAREN)
	{
		Position pos = _reader.GetCurPosition();
		TokenReader reader = TokenReader(_reader.Advance());
		std::vector<IExpression*> argExprs;

		if(reader.GetCurType() != TT_RPAREN) // Expecting at least on arg
		{
			ExpressionResult argExprResult = GetExpression(reader);

			if(argExprResult.success)
			{
				reader = argExprResult.reader;
				argExprs.push_back(argExprResult.value);

				while(true)
				{
					if(reader.GetCurType() == TT_COMMA)
					{
						reader.Advance();
						argExprResult = GetExpression(reader);

						if(argExprResult.success)
						{
							reader = argExprResult.reader;
							argExprs.push_back(argExprResult.value);
						}
						else
						{
							for(auto it : argExprs)
								delete it;

							return ExpressionResult::GenFailure("Expected function argument.", reader);
						}
					}
					else { break; }
				}
			}
			else { return ExpressionResult::GenFailure("Expected function argument or ')'.", reader); }
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
	if(_reader.GetCurType() == TT_LBRACKET)
	{
		TokenReader reader = TokenReader(_reader);
		auto parser = [](TokenReader _r) { return GetLet(_r, false); }; // Looks only for let statments without the let keyword
		StatementResult result = GetCodeBlock(reader, "<MEMBERED>", parser);

		if(result.success)
		{
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
		else { return ExpressionResult::GenFailure(result.message, reader); }
	}
	else { return ExpressionResult::GenFailure("Expected '{'.", _reader); }
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

ExpressionResult GetTypeName(TokenReader _reader)
{
	TokenReader reader = TokenReader(_reader.GetCurPtr());
	Position pos = reader.GetCurPosition();
	std::string typeName = "";

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
				IExpression* expr = new SymbolExpression(typeName, reader.GetCurPosition());
				return ExpressionResult::GenSuccess(expr, reader.GetCurPtr());
			}
		}
		else { return ExpressionResult::GenFailure("Expected type name.", reader); }
	}
}

ExpressionResult GetSingular(TokenReader _reader)
{
	Token token = _reader.current;

	if(token.type == TT_LPAREN) { return GetSubExpression(_reader); }
	else if(token.type == TT_IDENTIFIER) { return GetIdentifier(_reader); }
	else if(token.type == TT_LBRACKET) { return GetMembered(_reader); }
	else
	{
		IExpression* expr;

		if(token.type == TT_INTEGER_LIT) { expr = new ValueExpression(new IntValue(std::stoi(token.value), token.position)); }
		else if(token.type == TT_FLOAT_LIT) { expr = new ValueExpression(new FloatValue(std::stof(token.value), token.position)); }
		else if(token.type == TT_STRING_LIT) { expr = new ValueExpression(new StringValue(token.value, token.position)); }
		else if(token.type == TT_TRUE) { expr = new ValueExpression(new BoolValue(true, token.position)); }
		else if(token.type == TT_FALSE) { expr = new ValueExpression(new BoolValue(false, token.position)); }
		else if(token.type == TT_VOID) { expr = new ValueExpression(new VoidValue(token.position)); }
		else { return ExpressionResult::GenFailure("Value could not be parsed.", _reader); }

		return ExpressionResult::GenSuccess(expr, _reader.Advance());
	}
}

ExpressionResult GetCast(TokenReader _reader)
{
	if(_reader.GetCurType() == TT_LT)
	{
		Position pos = _reader.GetCurPosition();
		TokenReader reader = TokenReader(_reader.Advance());
		std::string typeName;

		ExpressionResult typeNameResult = GetTypeName(reader);

		if(typeNameResult.success)
		{
			reader = typeNameResult.reader;
			typeName = ((SymbolExpression*)typeNameResult.value)->symbol;
			delete typeNameResult.value;
		}
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
		bool negated = false;

		if(_reader.GetCurType() == TT_MINUS)
			negated = true;

		ExpressionResult result = GetSingular(negated ? _reader.Advance() : _reader.GetCurPtr());

		if(result.success)
		{
			TokenReader reader = result.reader;
			IExpression* factorExpr = negated ? new NegationExpression(result.value, pos) : result.value;

			while(true)
			{
				if(reader.GetCurType() == TT_PERIOD)
				{
					ExpressionResult accessResult = GetAccess(factorExpr, reader);

					if(accessResult.success)
					{
						reader = accessResult.reader;
						factorExpr = accessResult.value;
					}
					else
					{
						delete factorExpr;
						return accessResult;
					}
				}
				else if(reader.GetCurType() == TT_LPAREN)
				{
					ExpressionResult funcCallResult = GetFuncCall(factorExpr, reader);

					if(funcCallResult.success)
					{
						reader = funcCallResult.reader;
						factorExpr = funcCallResult.value;
					}
					else
					{
						delete factorExpr;
						return funcCallResult;
					}
				}
				else { break; }
			}

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

		if(tokenType == TT_LT || tokenType == TT_GT || tokenType == TT_LTEQ || tokenType == TT_GTEQ)
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
	if(_reader.GetCurType() == TT_EQUALS)
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

			if(reader.GetCurType() == TT_EQUALS)
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

			if(IsTypeName(reader.GetCurType()))
			{
				MemberDefinition* memDef = new MemberDefinition(identifier, reader.current.value, pos);
				return StatementResult::GenSuccess(memDef, reader.Advance());
			}
			else { return StatementResult::GenFailure("Expected typename after :.", reader); }
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
									IStatement* finStmt = (IStatement*)finResult.value;

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

						if(reader.GetCurType() == TT_EQUALS)
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

			if(reader.GetCurType() == TT_EQUALS)
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
				else { return StatementResult::GenFailure("Expected '{'. ", _reader); }
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

		if(reader.GetCurType() == TT_EQUALS) { return GetAssignment(reader, result.value); }
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
