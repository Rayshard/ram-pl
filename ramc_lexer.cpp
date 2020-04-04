#include "pch.h"
#include "ramc_lexer.h"

namespace ramc {
	std::string LexerResult::ToString(bool _includePos)
	{
		std::string prefix = _includePos ? "(" + GetErrorPosition().ToString() + ") " : "";

		switch (GetType())
		{
			case LexerResultType::SUCCESS: return "SUCCESS";
			case LexerResultType::BYTE_LIT_OOB: return prefix + "Byte Literal Out of 8 Bit Bound: " + GetErrorString();
			case LexerResultType::INT_LIT_OOB: return prefix + "Interger Literal Out of 32 Bit Bound: " + GetErrorString();
			case LexerResultType::DOUBLE_LIT_OOB: return prefix + "Double Literal Out of 64 Bit Bound: " + GetErrorString();
			case LexerResultType::LONG_LIT_OOB: return prefix + "Long Literal Out of 64 Bit Bound: " + GetErrorString();
			case LexerResultType::FLOAT_LIT_OOB: return prefix + "Float Literal Out of 32 Bit Bound: " + GetErrorString();
			case LexerResultType::STR_LIT_CLOSE: return prefix + "Missing closing '\"' for string literal: " + GetErrorString();
			case LexerResultType::COMMENT_CLOSE: return prefix + "Missing closing '`' for comment: " + GetErrorString();
			case LexerResultType::TOO_MANY_DECIMALS: return prefix + "Float Literal has more than one decimal: " + GetErrorString();
			case LexerResultType::STR_LIT_INVALID_ESC: return prefix + "String literal has an invalid escaped char: " + GetErrorString();
			case LexerResultType::UNKNOWN_SYMBOL: return prefix + "Unknown Symbol: " + GetErrorString();
			default: return prefix + "LexerResult::ToString - LexerResultType not handled!";
		}
	}

	const std::unordered_map<std::string, TokenType> KEYWORDS = {
		{ "true", TokenType::KW_TRUE }, { "false", TokenType::KW_FALSE },

		{ "byte", TokenType::KW_BYTE }, { "bool", TokenType::KW_BOOL }, { "int", TokenType::KW_INT },
		{ "double", TokenType::KW_DOUBLE }, { "float", TokenType::KW_FLOAT }, { "long", TokenType::KW_LONG },
		{ "string", TokenType::KW_STRING }, { "void", TokenType::KW_VOID },

		{ "let", TokenType::KW_LET }, { "if", TokenType::KW_IF }, { "then", TokenType::KW_THEN },
		{ "else", TokenType::KW_ELSE }, { "while", TokenType::KW_WHILE }, { "for", TokenType::KW_FOR },
		{ "do", TokenType::KW_DO }, { "func", TokenType::KW_FUNC },

		{ "continue", TokenType::KW_CONTINUE }, { "break", TokenType::KW_BREAK }, { "return", TokenType::KW_RETURN },

		{ "_", TokenType::UNDERSCORE },
	};

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
					if (peekedChar == 'd' || peekedChar == 'D')
					{
						try { IGNORE(_lexer->ReadNextChar()); std::stod(tokenStr); return LexerResult::GenSuccess(Token(TokenType::DOUBLE_LIT, _tokStartPos, tokenStr)); }
						catch (...) { return LexerResult::GenError(LexerResultType::DOUBLE_LIT_OOB, tokenStr, _lexer->GetPosition()); }
					}
					else
					{
						if (peekedChar == 'f' || peekedChar == 'F')
							IGNORE(_lexer->ReadNextChar());

						try { std::stof(tokenStr); return LexerResult::GenSuccess(Token(TokenType::FLOAT_LIT, _tokStartPos, tokenStr)); }
						catch (...) { return LexerResult::GenError(LexerResultType::FLOAT_LIT_OOB, tokenStr, _lexer->GetPosition()); }
					}
				}
				else
				{
					if (peekedChar == 'b' || peekedChar == 'B')
					{
						IGNORE(_lexer->ReadNextChar());

						try
						{
							rLong val = std::stoll(tokenStr);
							if (val < 0 || val >255) { throw 0; }
							else { return LexerResult::GenSuccess(Token(TokenType::BYTE_LIT, _tokStartPos, tokenStr)); }
						}
						catch (...) { return LexerResult::GenError(LexerResultType::BYTE_LIT_OOB, tokenStr, _lexer->GetPosition()); }
					}
					else if (peekedChar == 'l' || peekedChar == 'L')
					{
						try { IGNORE(_lexer->ReadNextChar()); std::stoll(tokenStr); return LexerResult::GenSuccess(Token(TokenType::LONG_LIT, _tokStartPos, tokenStr)); }
						catch (...) { return LexerResult::GenError(LexerResultType::LONG_LIT_OOB, tokenStr, _lexer->GetPosition()); }
					}
					else
					{
						try { std::stoi(tokenStr); return  LexerResult::GenSuccess(Token(TokenType::INT_LIT, _tokStartPos, tokenStr)); }
						catch (...) { return LexerResult::GenError(LexerResultType::INT_LIT_OOB, tokenStr, _lexer->GetPosition()); }
					}
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
				auto search = KEYWORDS.find(tokenStr);
				if (search != KEYWORDS.end()) { return LexerResult::GenSuccess(Token(search->second, _tokStartPos, "")); }
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

	LexerResult Lexer::GetNextToken()
	{
		char firstChar = (char)ReadNextChar();

#pragma region Ignores
		while (true)
		{
			if (ignoreWhitespace && isspace(firstChar)) { firstChar = (char)ReadNextChar(); }
			else if (ignoreComments && firstChar == '`')
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
				char peekedChar = (char)PeekNextChar();
				
				if (peekedChar == '=') {
					ReadNextChar();
					return LexerResult::GenSuccess(Token(TokenType::PLUS_EQ, tokStartPos, ""));
				}
				else { return LexerResult::GenSuccess(Token(TokenType::PLUS, tokStartPos, "")); }
			}
			case '-': {
				char peekedChar = (char)PeekNextChar();

				if (peekedChar == '>') {
					ReadNextChar();
					return LexerResult::GenSuccess(Token(TokenType::GOES_TO, tokStartPos, ""));
				}
				else if (peekedChar == '=') {
					ReadNextChar();
					return LexerResult::GenSuccess(Token(TokenType::MINUS_EQ, tokStartPos, ""));
				}
				else { return LexerResult::GenSuccess(Token(TokenType::MINUS, tokStartPos, "")); }
			}
			case '*': {
				char peekedChar = (char)PeekNextChar();

				if (peekedChar == '*') {
					ReadNextChar();

					if ((char)PeekNextChar() == '=') {
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
				if ((char)PeekNextChar() == '=') {
					ReadNextChar();
					return LexerResult::GenSuccess(Token(TokenType::DIVIDE_EQ, tokStartPos, ""));
				}
				else { return LexerResult::GenSuccess(Token(TokenType::DIVIDE, tokStartPos, "")); }
			}
			case '%': {
				if ((char)PeekNextChar() == '=') {
					ReadNextChar();
					return LexerResult::GenSuccess(Token(TokenType::MOD_EQ, tokStartPos, ""));
				}
				else { return LexerResult::GenSuccess(Token(TokenType::MOD, tokStartPos, "")); }
			}
			case '&': {
				char peekedChar = (char)PeekNextChar();

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
				char peekedChar = (char)PeekNextChar();

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
				char peekedChar = (char)PeekNextChar();

				if (peekedChar == '=') {
					ReadNextChar();
					return LexerResult::GenSuccess(Token(TokenType::BIN_XOR_EQ, tokStartPos, ""));
				}
				else { return LexerResult::GenSuccess(Token(TokenType::BIN_XOR, tokStartPos, "")); }
			}
			case '<': {
				char peekedChar = (char)PeekNextChar();

				if (peekedChar == '<') {
					ReadNextChar();

					if ((char)PeekNextChar() == '=') {
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
				char peekedChar = (char)PeekNextChar();

				if (peekedChar == '>') {
					ReadNextChar();

					if ((char)PeekNextChar() == '=') {
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
				if ((char)PeekNextChar() == '=') {
					ReadNextChar();
					return LexerResult::GenSuccess(Token(TokenType::EQ_EQ, tokStartPos, ""));
				}
				else { return LexerResult::GenSuccess(Token(TokenType::EQ, tokStartPos, "")); }
			}
			case '!': {
				if ((char)PeekNextChar() == '=') {
					ReadNextChar();
					return LexerResult::GenSuccess(Token(TokenType::NEQ, tokStartPos, ""));
				}
				else { return LexerResult::GenSuccess(Token(TokenType::LOG_NOT, tokStartPos, "")); }
			}
			case '~':  return LexerResult::GenSuccess(Token(TokenType::BIN_NOT, tokStartPos, ""));
			case '"': return LexStringLiteral(this, tokStartPos);
			case '`': return LexComment(this, tokStartPos);
		}

		if (isdigit(firstChar)) { return LexNumericLiteral(this, firstChar, tokStartPos); }
		else if (isalpha(firstChar) || firstChar == '_') { return LexIdentifier(this, firstChar, tokStartPos); }
		else { return LexerResult::GenError(LexerResultType::UNKNOWN_SYMBOL, std::string(1, firstChar), position); }
	}

	std::string LexFile(std::istream* _stream, bool _ignoreWhitespace, bool _ignoreComments, int _tabSize)
	{
		Lexer lexer = Lexer(_stream, _ignoreWhitespace, _ignoreComments, _tabSize);
		std::stringstream ss;

		while (true)
		{
			LexerResult res = lexer.GetNextToken();

			if (res.IsSuccess())
			{
				Token token = res.GetValue();
				ss << token.ToString(true) << std::endl;

				if (token.type == TokenType::END_OF_FILE)
					break;
			}
			else
			{
				ss << res.ToString(true) << std::endl;
				break;
			}
		}

		return ss.str();
	}
}
