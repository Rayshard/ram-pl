#include "pch.h"
#include "ramvm_lexer.h"

namespace ramvm {
	std::string LexerResult::ToString(bool _includePos)
	{
		std::string prefix = _includePos ? "(" + GetErrorPosition().ToString() + ") " : "";

		switch (GetType())
		{
			case LexerResultType::SUCCESS: return "SUCCESS";
			case LexerResultType::HEX_LIT_OOB: return prefix + "Hex Literal Value Out Of Bounds: " + GetErrorString();
			case LexerResultType::BYTE_LIT_OOB: return prefix + "Byte Literal Value Out Of Bounds: " + GetErrorString();
			case LexerResultType::INT_LIT_OOB: return prefix + "Integer Literal Value Out Of Bounds: " + GetErrorString();
			case LexerResultType::FLOAT_LIT_OOB: return prefix + "Float Literal Value Out Of Bounds: " + GetErrorString();
			case LexerResultType::DOUBLE_LIT_OOB: return prefix + "Doubel Literal Value Out Of Bounds: " + GetErrorString();
			case LexerResultType::LONG_LIT_OOB: return prefix + "Long Literal Value Out Of Bounds: " + GetErrorString();
			case LexerResultType::TOO_MANY_DECIMALS: return prefix + "Too many decimals in literal " + GetErrorString();
			case LexerResultType::INVALID_REGISTER: return prefix + "Invalid Register: " + GetErrorString();
			case LexerResultType::INVAILD_LABEL_ID: return prefix + "Invalid Label Identifier: " + GetErrorString();
			case LexerResultType::INVAILD_TYPE_POSTFIX: return prefix + "Invalid Type Postfix: " + GetErrorString();
			case LexerResultType::MISSING_TYPE_CLOSE: return prefix + "Missing closing '>': " + GetErrorString();
			case LexerResultType::INVALID: return prefix + "Invalid: " + GetErrorString();
			case LexerResultType::UNKNOWN_TOKEN: return prefix + "Unknown Token: " + GetErrorString();
			default: return prefix + "LexerResult::ToString - LexerResultType not handled!";
		}
	}

	const std::unordered_map<std::string, TokenType> KEYWORDS = {
		{ "HALT", TokenType::KW_HALT }, { "MOV", TokenType::KW_MOV },
		{ "RET", TokenType::KW_RET}, { "MALLOC", TokenType::KW_MALLOC }, { "FREE", TokenType::KW_FREE },
		{ "PUSH", TokenType::KW_PUSH }, { "POP", TokenType::KW_POP }, { "PRINT", TokenType::KW_PRINT },
		{ "JUMP", TokenType::KW_JUMP }, { "JUMPT", TokenType::KW_JUMPT }, { "JUMPF", TokenType::KW_JUMPF },
		{ "CALL", TokenType::KW_CALL }, { "STORE", TokenType::KW_STORE }, { "COMPARE", TokenType::KW_COMPARE },
		{ "ADD", TokenType::KW_ADD }, { "SUB", TokenType::KW_SUB }, { "MUL", TokenType::KW_MUL },
		{ "DIV", TokenType::KW_DIV }, { "MOD", TokenType::KW_MOD }, { "POW", TokenType::KW_POW },
		{ "LSHIFT", TokenType::KW_LSHIFT }, { "RSHIFT", TokenType::KW_RSHIFT }, { "AND", TokenType::KW_AND },
		{ "OR", TokenType::KW_OR }, { "XOR", TokenType::KW_XOR }, { "LT", TokenType::KW_LT },
		{ "GT", TokenType::KW_GT }, { "LTEQ", TokenType::KW_LTEQ }, { "GTEQ", TokenType::KW_GTEQ },
		{ "EQ", TokenType::KW_EQ }, { "NEQ", TokenType::KW_NEQ }, { "NEG", TokenType::KW_NEG },
		{ "NOT", TokenType::KW_NOT }
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

	LexerResult LexHexLiteral(Lexer* _lexer, char _firstChar, Position _tokStartPos)
	{
		std::string tokenStr = std::string(1, _firstChar);

		while (true)
		{
			char peekedChar = (char)_lexer->PeekNextChar();

			if (isxdigit(peekedChar)) { tokenStr.push_back((char)_lexer->ReadNextChar()); }
			else
			{
				try { return LexerResult::GenSuccess(Token(TokenType::HEX_LIT, _tokStartPos, ToByteString(std::stoll(tokenStr, 0, 16)))); }
				catch (...) { return LexerResult::GenError(LexerResultType::HEX_LIT_OOB, tokenStr, _lexer->GetPosition()); }
			}
		}
	}

	LexerResult LexLabel(Lexer* _lexer, char _firstChar, Position _tokStartPos)
	{
		std::string tokenStr = std::string(1, _firstChar);
		Position tokStartPos = _lexer->GetPosition();

		while (true)
		{
			char peekedChar = (char)_lexer->PeekNextChar();

			if (isalpha(peekedChar) || isdigit(peekedChar) || peekedChar == '_') { tokenStr.push_back((char)_lexer->ReadNextChar()); }
			else { return LexerResult::GenSuccess(Token(TokenType::LABEL, _tokStartPos, tokenStr)); }
		}
	}

	LexerResult LexKeyword(Lexer* _lexer, char _firstChar, Position _tokStartPos)
	{
		std::string tokenStr = std::string(1, _firstChar);
		Position tokStartPos = _lexer->GetPosition();

		while (true)
		{
			char peekedChar = (char)_lexer->PeekNextChar();

			if (isalpha(peekedChar)) { tokenStr.push_back((char)_lexer->ReadNextChar()); }
			else
			{
				auto search = KEYWORDS.find(tokenStr);
				if (search != KEYWORDS.end()) { return LexerResult::GenSuccess(Token(search->second, _tokStartPos, "")); }
				else { return LexerResult::GenExpectationError("keyword", tokenStr, _tokStartPos); }
			}
		}
	}

	LexerResult LexComment(Lexer* _lexer, Position _tokStartPos)
	{
		std::string tokenStr;
		Position tokStartPos = _lexer->GetPosition();

		while (true)
		{
			char peekedChar = (char)_lexer->PeekNextChar();

			if (peekedChar == '\n' || peekedChar == EOF) { return LexerResult::GenSuccess(Token(TokenType::COMMENT, _tokStartPos, tokenStr)); }
			else { tokenStr.push_back((char)_lexer->ReadNextChar()); }
		}
	}

	LexerResult Lexer::GetNextToken()
	{
		char firstChar = (char)ReadNextChar();
		Position tokStartPos = position;

#pragma region Ignores
		while (true)
		{
			if (isspace(firstChar)) { firstChar = (char)ReadNextChar(); }
			else if (firstChar == '#')
			{
				LexerResult commentRes = LexComment(this, position);

				if (!commentRes.IsSuccess()) { return commentRes; }
				else { firstChar = (char)ReadNextChar(); }
			}
			else { break; }
		}
#pragma endregion

		switch (firstChar)
		{
			case EOF: return LexerResult::GenSuccess(Token::END_OF_FILE(tokStartPos));
			case ',': return LexerResult::GenSuccess(Token(TokenType::COMMA, tokStartPos, ""));
			case '{': return LexerResult::GenSuccess(Token(TokenType::LCBRACKET, tokStartPos, ""));
			case '}': return LexerResult::GenSuccess(Token(TokenType::RCBRACKET, tokStartPos, ""));
			case '[': return LexerResult::GenSuccess(Token(TokenType::LSBRACKET, tokStartPos, ""));
			case ']': return LexerResult::GenSuccess(Token(TokenType::RSBRACKET, tokStartPos, ""));
			case '(': return LexerResult::GenSuccess(Token(TokenType::LPAREN, tokStartPos, ""));
			case ')': return LexerResult::GenSuccess(Token(TokenType::RPAREN, tokStartPos, ""));
			case '+':  return LexerResult::GenSuccess(Token(TokenType::PLUS, tokStartPos, ""));
			case '-':  return LexerResult::GenSuccess(Token(TokenType::MINUS, tokStartPos, ""));
			case ';':  return LexerResult::GenSuccess(Token(TokenType::SEMICOLON, tokStartPos, ""));
			case '%': {
				char peekedChar = (char)PeekNextChar();

				if (isalpha(peekedChar) || peekedChar == '_') { return LexLabel(this, (char)ReadNextChar(), tokStartPos); }
				else { return LexerResult::GenError(LexerResultType::INVAILD_LABEL_ID, "%" + peekedChar, tokStartPos); }
			}
			case '<': {
				std::string typeConcat;

				while (CharToDataType((char)PeekNextChar()) != DataType::UNKNOWN)
					typeConcat += (char)ReadNextChar();

				if ((char)PeekNextChar() != '>') { return LexerResult::GenError(LexerResultType::MISSING_TYPE_CLOSE, "<" + typeConcat, tokStartPos); }
				else { IGNORE(ReadNextChar()); }

				if (typeConcat.size() == 1) { return LexerResult::GenSuccess(Token(TokenType::SINGLE_TYPE, tokStartPos, typeConcat)); }
				else if (typeConcat.size() == 2) { return LexerResult::GenSuccess(Token(TokenType::DOUBLE_TYPE, tokStartPos, typeConcat)); }
				else if (typeConcat.size() == 3) { return LexerResult::GenSuccess(Token(TokenType::TRIPLE_TYPE, tokStartPos, typeConcat)); }
				else { return LexerResult::GenError(LexerResultType::INVAILD_TYPE_POSTFIX, "<" + typeConcat + ">", tokStartPos); }
			}
			case '$': {
				std::string reg;

				while (isalpha(PeekNextChar()))
					reg += (char)ReadNextChar();

				if (reg == "SP") { return LexerResult::GenSuccess(Token(TokenType::REG_SP, tokStartPos, "")); }
				else if (reg == "FP") { return LexerResult::GenSuccess(Token(TokenType::REG_FP, tokStartPos, "")); }
				else if (reg == "GP") { return LexerResult::GenSuccess(Token(TokenType::REG_GP, tokStartPos, "")); }
				else { return LexerResult::GenError(LexerResultType::INVALID_REGISTER, "$" + std::string(1, (char)PeekNextChar()), tokStartPos); }
			}
			case '0': {
				if ((char)PeekNextChar() == 'x')
				{
					ReadNextChar();

					if (isxdigit(PeekNextChar())) { return LexHexLiteral(this, (char)ReadNextChar(), tokStartPos); }
					else { return LexerResult::GenExpectationError("hex digit", std::string(1, (char)PeekNextChar()), tokStartPos); }
				}
			} break;
		}

		if (isdigit(firstChar)) { return LexNumericLiteral(this, firstChar, tokStartPos); }
		else if (isalpha(firstChar)) { return LexKeyword(this, firstChar, tokStartPos); }
		else { return LexerResult::GenError(LexerResultType::UNKNOWN_TOKEN, std::string(1, firstChar), position); }
	}

	std::string LexFile(std::istream* _stream, int _tabSize)
	{
		Lexer lexer = Lexer(_stream, _tabSize);
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
