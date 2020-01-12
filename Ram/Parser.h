#pragma once

template <typename T>
class ParseResult : public ResultT<T>
{
public:
	TokenReader reader;

private:
	ParseResult(bool _success, T _val, Token* _remainingTokens, std::string _msg)
		: ResultT<T>(_success, _val, _msg, _remainingTokens[0].position)
	{
		reader = TokenReader(_remainingTokens);
	}

public:
	static ParseResult GenSuccess(T _val, Token* _remainingTokens) { return ParseResult(true, _val, _remainingTokens, std::string()); }
	static ParseResult GenFailure(std::string _msg, TokenReader _reader) { return ParseResult(false, T(), _reader.GetCurPtr(), _msg); }
};

typedef ParseResult<IExpression*> ExpressionResult;
typedef ParseResult<IStatement*> StatementResult;
typedef ResultT<std::vector<IStatement*>> FileParseResult;

ExpressionResult GetFuncCall(IExpression* _base, TokenReader _reader);
ExpressionResult GetAccess(IExpression* _base, TokenReader _reader);
ExpressionResult GetMembered(TokenReader _reader);
ExpressionResult GetSubExpression(TokenReader _reader);
ExpressionResult GetIdentifier(TokenReader _reader);
ExpressionResult GetArrayTypeName(TokenReader _reader, std::string& _outTypeName);
ExpressionResult GetSingular(TokenReader _reader);
ExpressionResult GetFactor(TokenReader _reader);
ExpressionResult GetTerm(TokenReader _reader);
ExpressionResult GetBinopExpr(TokenReader _reader);
ExpressionResult GetExpression(TokenReader _reader);

StatementResult GetControlFlow(TokenReader _reader);
StatementResult GetAssignment(TokenReader _reader, IExpression* _base);
StatementResult GetLet(TokenReader _reader, bool _reqKeyword);
StatementResult GetDefinition(TokenReader _reader);
StatementResult GetCodeBlock(TokenReader _reader, std::string _name, std::function<StatementResult(TokenReader)> _parser);
StatementResult GetSimpleStatement(TokenReader _reader);
StatementResult GetForLoop(TokenReader _reader);
StatementResult GetFuncDeclaration(TokenReader _reader);
StatementResult GetTypeDefinition(TokenReader _reader);
StatementResult GetStatement(TokenReader _reader);
FileParseResult Parse(Token* _tokens);
