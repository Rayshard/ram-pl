#include "pch.h"
#include "ramc_lexer.h"

namespace ramc {
	bool LexerResult::IsSuccess() { return type == LexerResultType::SUCCESS; }
	Token LexerResult::GetToken() { return IsSuccess() ? token : Token::INVALID(errPosition); }

	std::string LexerResult::ToString()
	{
		std::string prefix = "(" + errPosition.ToString() + ") ";

		switch (type)
		{
			case LexerResultType::SUCCESS: return "SUCCESS";
			case LexerResultType::INT_LIT_OOB: return prefix + "Integer Literal Out of -(2^32) to (2^32 - 1) bound: " + errString;
			case LexerResultType::FLOAT_LIT_OOB: return prefix + "Float Literal Out of -(2^32) to (2^32 - 1) bound: " + errString;
			case LexerResultType::STR_LIT_CLOSE: return prefix + "Missing closing '\"' for string literal: " + errString;
			case LexerResultType::COMMENT_CLOSE: return prefix + "Missing closing '`' for comment: " + errString;
			case LexerResultType::TOO_MANY_DECIMALS: return prefix + "Float Literal has more than one decimal: " + errString;
			case LexerResultType::STR_LIT_INVALID_ESC: return prefix + "String literal has an invalid escaped char: " + errString;
			case LexerResultType::UNKNOWN_SYMBOL: return prefix + "Unknown Symbol: " + errString;
			default: return prefix + "LexerResult::ToString - LexerResultType not handled!";
		}
	}

	LexerResult LexerResult::GenSuccess(Token _token)
	{
		LexerResult result;
		result.type = LexerResultType::SUCCESS;
		result.token = _token;
		return result;
	}
	LexerResult LexerResult::GenError(LexerResultType _type, std::string _str, Position _pos)
	{
		LexerResult result;
		result.type = _type;
		result.errString = _str;
		result.errPosition = _pos;
		return result;
	}

	LexerResult LexNumericLiteral(Lexer* _lexer, char _firstChar, Position _tokStartPos)
	{
		std::string tokenStr = std::string(1, _firstChar);
		bool hasDecimal = false;

		while (true)
		{
			char peekedChar = (char)_lexer->PeekNextChar();

			if (isdigit(peekedChar)) { tokenStr.push_back((char)_lexer->ReadNextChar()); }
			else if (peekedChar == '.')
			{
				tokenStr.push_back((char)_lexer->ReadNextChar());

				if (hasDecimal) { return LexerResult::GenError(LexerResultType::TOO_MANY_DECIMALS, tokenStr, _lexer->GetPosition()); }
				else { hasDecimal = true; }
			}
			else
			{
				if (hasDecimal)
				{
					try
					{
						std::stof(tokenStr);
						return  LexerResult::GenSuccess(Token(TokenType::FLOAT_LIT, _tokStartPos, tokenStr));
					}
					catch (...) { return LexerResult::GenError(LexerResultType::FLOAT_LIT_OOB, tokenStr, _lexer->GetPosition()); }
				}
				else
				{
					try
					{
						std::stoi(tokenStr);
						return  LexerResult::GenSuccess(Token(TokenType::INT_LIT, _tokStartPos, tokenStr));
					}
					catch (...) { return LexerResult::GenError(LexerResultType::INT_LIT_OOB, tokenStr, _lexer->GetPosition()); }
				}
			}
		}
	}

	LexerResult LexIdentifier(Lexer* _lexer, char _firstChar, Position _tokStartPos)
	{
		std::string tokenStr = std::string(1, _firstChar);
		Position tokStartPos = _lexer->GetPosition();

		while (true)
		{
			char peekedChar = (char)_lexer->PeekNextChar();

			if (isalpha(peekedChar) || isdigit(peekedChar) || peekedChar == '_' || peekedChar == '\'') { tokenStr.push_back((char)_lexer->ReadNextChar()); }
			else
			{
				if (tokenStr == "_") { return LexerResult::GenSuccess(Token(TokenType::UNDERSCORE, _tokStartPos, "")); }
				else if (tokenStr == "true") { return LexerResult::GenSuccess(Token(TokenType::KW_TRUE, _tokStartPos, "")); }
				else if (tokenStr == "false") { return LexerResult::GenSuccess(Token(TokenType::KW_FALSE, _tokStartPos, "")); }
				else { return LexerResult::GenSuccess(Token(TokenType::IDENTIFIER, _tokStartPos, tokenStr)); }
			}
		}
	}

	LexerResult LexStringLiteral(Lexer* _lexer, Position _tokStartPos)
	{
		std::string tokenStr;
		Position tokStartPos = _lexer->GetPosition();
		bool escaped = false;

		while (true)
		{
			char peekedChar = (char)_lexer->PeekNextChar();

			if (escaped)
			{
				switch ((char)_lexer->ReadNextChar())
				{
					case 'n': tokenStr.push_back('\n'); break;
					case 't': tokenStr.push_back('\t'); break;
					case 'r': tokenStr.push_back('\r'); break;
					case '"': tokenStr.push_back('"'); break;
					case '\\': tokenStr.push_back('\\'); break;
					default: return LexerResult::GenError(LexerResultType::STR_LIT_INVALID_ESC, tokenStr, _lexer->GetPosition());
				}

				escaped = false;
				continue;
			}

			if (peekedChar == '\\')
			{
				_lexer->ReadNextChar();
				escaped = true;
			}
			else if (peekedChar == '"')
			{
				_lexer->ReadNextChar();
				return LexerResult::GenSuccess(Token(TokenType::STRING_LIT, _tokStartPos, tokenStr));
			}
			else if (peekedChar == EOF) { return LexerResult::GenError(LexerResultType::STR_LIT_CLOSE, tokenStr, _lexer->GetPosition()); }
			else { tokenStr.push_back((char)_lexer->ReadNextChar()); }
		}
	}

	LexerResult LexComment(Lexer* _lexer, Position _tokStartPos)
	{
		std::string tokenStr;
		Position tokStartPos = _lexer->GetPosition();
		
		while (true)
		{
			char peekedChar = (char)_lexer->PeekNextChar();

			if (peekedChar == '`')
			{
				_lexer->ReadNextChar();
				return LexerResult::GenSuccess(Token(TokenType::COMMENT, _tokStartPos, tokenStr));
			}
			else if (peekedChar == EOF) { return LexerResult::GenError(LexerResultType::COMMENT_CLOSE, tokenStr, _lexer->GetPosition()); }
			else { tokenStr.push_back((char)_lexer->ReadNextChar()); }
		}
	}

	bool IsWhitespace(char _char) { return _char == ' ' || _char == '\n' || _char == '\r' || _char == '\t'; }

	Lexer::Lexer(std::istream* _stream, int _tabSize)
	{
		stream = _stream;
		position = Position(1, 0);
		tabSize = _tabSize;
	}

	int Lexer::ReadNextChar()
	{
		int result = stream->get();

		if (result == '\n')
		{
			position.line++;
			position.colomn = 0;
		}
		else if (result == '\t') { position.colomn += tabSize; }
		else { position.colomn++; }

		return result;
	}

	int Lexer::PeekNextChar() { return stream->peek(); }

	LexerResult Lexer::GetNextToken(bool _ignoreWhitespace, bool _ignoreComments)
	{
		char firstChar = (char)ReadNextChar();

#pragma region Ignores
		while (true)
		{
			if (_ignoreWhitespace && IsWhitespace(firstChar)) { firstChar = (char)ReadNextChar(); }
			else if (_ignoreComments && firstChar == '`')
			{
				LexerResult commentRes = LexComment(this, position);

				if (!commentRes.IsSuccess()) { return commentRes; }
				else { firstChar = (char)ReadNextChar(); }
			}
			else { break; }
		}
#pragma endregion

		if (firstChar == EOF)
			return LexerResult::GenSuccess(Token::END_OF_FILE(position));

		Position tokStartPos = position;

		switch (firstChar)
		{
			case ' ': case '\n': case '\t': case '\r': return LexerResult::GenSuccess(Token(TokenType::WHITESPACE, tokStartPos, ""));
			case ';': return LexerResult::GenSuccess(Token(TokenType::SEMICOLON, tokStartPos, ""));
			case '.': return LexerResult::GenSuccess(Token(TokenType::PERIOD, tokStartPos, ""));
			case ':': return LexerResult::GenSuccess(Token(TokenType::COLON, tokStartPos, ""));
			case ',': return LexerResult::GenSuccess(Token(TokenType::COMMA, tokStartPos, ""));
			case '(': return LexerResult::GenSuccess(Token(TokenType::LPAREN, tokStartPos, ""));
			case ')': return LexerResult::GenSuccess(Token(TokenType::RPAREN, tokStartPos, ""));
			case '{': return LexerResult::GenSuccess(Token(TokenType::LCBRACKET, tokStartPos, ""));
			case '}': return LexerResult::GenSuccess(Token(TokenType::RCBRACKET, tokStartPos, ""));
			case '[': return LexerResult::GenSuccess(Token(TokenType::LSBRACKET, tokStartPos, ""));
			case ']': return LexerResult::GenSuccess(Token(TokenType::RSBRACKET, tokStartPos, ""));
			case '+': {
				char peekedChar = (char)stream->peek();

				if (peekedChar == '+') {
					ReadNextChar();
					return LexerResult::GenSuccess(Token(TokenType::INC, tokStartPos, ""));
				}
				else if (peekedChar == '=') {
					ReadNextChar();
					return LexerResult::GenSuccess(Token(TokenType::PLUS_EQ, tokStartPos, ""));
				}
				else { return LexerResult::GenSuccess(Token(TokenType::PLUS, tokStartPos, "")); }
			}
			case '-': {
				char peekedChar = (char)stream->peek();

				if (peekedChar == '-') {
					ReadNextChar();
					return LexerResult::GenSuccess(Token(TokenType::DEC, tokStartPos, ""));
				}
				else if (peekedChar == '=') {
					ReadNextChar();
					return LexerResult::GenSuccess(Token(TokenType::MINUS_EQ, tokStartPos, ""));
				}
				else { return LexerResult::GenSuccess(Token(TokenType::MINUS, tokStartPos, "")); }
			}
			case '*': {
				char peekedChar = (char)stream->peek();

				if (peekedChar == '*') {
					ReadNextChar();

					if ((char)stream->peek() == '=') {
						ReadNextChar();
						return LexerResult::GenSuccess(Token(TokenType::POW_EQ, tokStartPos, ""));
					}
					else { return LexerResult::GenSuccess(Token(TokenType::POW, tokStartPos, "")); }
				}
				else if (peekedChar == '=') {
					ReadNextChar();
					return LexerResult::GenSuccess(Token(TokenType::TIMES_EQ, tokStartPos, ""));
				}
				else { return LexerResult::GenSuccess(Token(TokenType::TIMES, tokStartPos, "")); }
			}
			case '/': {
				if ((char)stream->peek() == '=') {
					ReadNextChar();
					return LexerResult::GenSuccess(Token(TokenType::DIVIDE_EQ, tokStartPos, ""));
				}
				else { return LexerResult::GenSuccess(Token(TokenType::DIVIDE, tokStartPos, "")); }
			}
			case '%': {
				if ((char)stream->peek() == '=') {
					ReadNextChar();
					return LexerResult::GenSuccess(Token(TokenType::MOD_EQ, tokStartPos, ""));
				}
				else { return LexerResult::GenSuccess(Token(TokenType::MOD, tokStartPos, "")); }
			}
			case '&': {
				char peekedChar = (char)stream->peek();

				if (peekedChar == '&') {
					ReadNextChar();
					return LexerResult::GenSuccess(Token(TokenType::LOG_AND, tokStartPos, ""));
				}
				else if (peekedChar == '=') {
					ReadNextChar();
					return LexerResult::GenSuccess(Token(TokenType::BIN_AND_EQ, tokStartPos, ""));
				}
				else { return LexerResult::GenSuccess(Token(TokenType::BIN_AND, tokStartPos, "")); }
			}
			case '|': {
				char peekedChar = (char)stream->peek();

				if (peekedChar == '|') {
					ReadNextChar();
					return LexerResult::GenSuccess(Token(TokenType::LOG_OR, tokStartPos, ""));
				}
				else if (peekedChar == '=') {
					ReadNextChar();
					return LexerResult::GenSuccess(Token(TokenType::BIN_OR_EQ, tokStartPos, ""));
				}
				else { return LexerResult::GenSuccess(Token(TokenType::BIN_OR, tokStartPos, "")); }
			}
			case '^': {
				char peekedChar = (char)stream->peek();

				if (peekedChar == '=') {
					ReadNextChar();
					return LexerResult::GenSuccess(Token(TokenType::BIN_XOR_EQ, tokStartPos, ""));
				}
				else { return LexerResult::GenSuccess(Token(TokenType::BIN_XOR, tokStartPos, "")); }
			}
			case '<': {
				char peekedChar = (char)stream->peek();

				if (peekedChar == '<') {
					ReadNextChar();

					if ((char)stream->peek() == '=') {
						ReadNextChar();
						return LexerResult::GenSuccess(Token(TokenType::LSHIFT_EQ, tokStartPos, ""));
					}
					else { return LexerResult::GenSuccess(Token(TokenType::LSHIFT, tokStartPos, "")); }
				}
				else if (peekedChar == '=') {
					ReadNextChar();
					return LexerResult::GenSuccess(Token(TokenType::LT_EQ, tokStartPos, ""));
				}
				else { return LexerResult::GenSuccess(Token(TokenType::LT, tokStartPos, "")); }
			}
			case '>': {
				char peekedChar = (char)stream->peek();

				if (peekedChar == '>') {
					ReadNextChar();

					if ((char)stream->peek() == '=') {
						ReadNextChar();
						return LexerResult::GenSuccess(Token(TokenType::RSHIFT_EQ, tokStartPos, ""));
					}
					else { return LexerResult::GenSuccess(Token(TokenType::RSHIFT, tokStartPos, "")); }
				}
				else if (peekedChar == '=') {
					ReadNextChar();
					return LexerResult::GenSuccess(Token(TokenType::GT_EQ, tokStartPos, ""));
				}
				else { return LexerResult::GenSuccess(Token(TokenType::GT, tokStartPos, "")); }
			}
			case '=': {
				if ((char)stream->peek() == '=') {
					ReadNextChar();
					return LexerResult::GenSuccess(Token(TokenType::EQ_EQ, tokStartPos, ""));
				}
				else { return LexerResult::GenSuccess(Token(TokenType::EQ, tokStartPos, "")); }
			}
			case '!': {
				if ((char)stream->peek() == '=') {
					ReadNextChar();
					return LexerResult::GenSuccess(Token(TokenType::NEQ, tokStartPos, ""));
				}
				else { return LexerResult::GenSuccess(Token(TokenType::LOG_NOT, tokStartPos, "")); }
			}
			case '"': return LexStringLiteral(this, tokStartPos);
			case '`': return LexComment(this, tokStartPos);
		}

		if (isdigit(firstChar)) { return LexNumericLiteral(this, firstChar, tokStartPos); }
		else if (isalpha(firstChar) || firstChar == '_') { return LexIdentifier(this, firstChar, tokStartPos); }
		else { return LexerResult::GenError(LexerResultType::UNKNOWN_SYMBOL, std::string(1, firstChar), position); }
	}

	std::string LexFile(std::istream* _stream, bool _ignoreWhitespace, bool _ignoreComments, int _tabSize)
	{
		Lexer lexer = Lexer(_stream, _tabSize);
		std::stringstream ss;

		while (true)
		{
			LexerResult res = lexer.GetNextToken(_ignoreWhitespace, _ignoreComments);

			if (res.IsSuccess())
			{
				Token token = res.GetToken();
				ss << token.ToString(true) << std::endl;

				if (token.type == TokenType::END_OF_FILE)
					break;
			}
			else
			{
				ss << res.ToString() << std::endl;
				break;
			}
		}

		return ss.str();
	}
}
