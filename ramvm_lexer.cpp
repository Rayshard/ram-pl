#include "pch.h"
#include "ramvm_lexer.h"

namespace ramvm {
	std::string LexerResult::ToString(bool _includePos)
	{
		std::string prefix = _includePos ? "(" + GetErrorPosition().ToString() + ") " : "";

		switch (GetType())
		{
			case LexerResultType::SUCCESS: return "SUCCESS";
			case LexerResultType::HEX_LIT_OOB: return prefix + "Literal Value Out Of Bounds: " + GetErrorString();
			case LexerResultType::HEX_LIT_INVALID: return prefix + "Hex value must express a sequence of bytes: " + GetErrorString();
			case LexerResultType::INVALID_REGISTER: return prefix + "Invalid Register: " + GetErrorString();
			case LexerResultType::INVALID_REG_IDX: return prefix + "Invalid Register Index: " + GetErrorString();
			case LexerResultType::INVAILD_LABEL_ID: return prefix + "Invalid Label Identifier: " + GetErrorString();
			case LexerResultType::INVALID_OFFSET: return prefix + "Invalid Offset: " + GetErrorString();
			case LexerResultType::COMMENT_CLOSE: return prefix + "Missing closing '`' for comment: " + GetErrorString();
			case LexerResultType::MISSING_CLOSE_BRACKET: return prefix + "Missing closing bracket: " + GetErrorString();
			case LexerResultType::INVALID: return prefix + "Invalid: " + GetErrorString();
			case LexerResultType::UNKNOWN_TOKEN: return prefix + "Unknown Token: " + GetErrorString();
			default: return prefix + "LexerResult::ToString - LexerResultType not handled!";
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

	LexerResult LexKeyword(std::string _tokStr, Position _tokStartPos)
	{
		if (_tokStr == "HALT") { return LexerResult::GenSuccess(Token(TokenType::KW_HALT, _tokStartPos, "")); }
		else if (std::regex_match(_tokStr, std::regex("RET<[BIFDL]*>"))) { return LexerResult::GenSuccess(Token(TokenType::KW_RET, _tokStartPos, _tokStr.substr(4, _tokStr.length() - 5))); }
		else if (_tokStr == "MALLOC") { return LexerResult::GenSuccess(Token(TokenType::KW_MALLOC, _tokStartPos, "")); }
		else if (_tokStr == "FREE") { return LexerResult::GenSuccess(Token(TokenType::KW_FREE, _tokStartPos, "")); }
		else if (std::regex_match(_tokStr, std::regex("PUSH<[BIFDL]+>"))) { return LexerResult::GenSuccess(Token(TokenType::KW_PUSH, _tokStartPos, _tokStr.substr(5, _tokStr.length() - 6))); }
		else if (std::regex_match(_tokStr, std::regex("POP<[BIFDL]>"))) { return LexerResult::GenSuccess(Token(TokenType::KW_POP, _tokStartPos, _tokStr.substr(4, _tokStr.length() - 5))); }
		else if (std::regex_match(_tokStr, std::regex("MOV<[BIFDL]>"))) { return LexerResult::GenSuccess(Token(TokenType::KW_MOV, _tokStartPos, _tokStr.substr(4, _tokStr.length() - 5))); }
		else if (_tokStr == "PRINT") { return LexerResult::GenSuccess(Token(TokenType::KW_PRINT, _tokStartPos, "")); }
		else if (_tokStr == "JUMP") { return LexerResult::GenSuccess(Token(TokenType::KW_JUMP, _tokStartPos, "")); }
		else if (_tokStr == "CJUMP") { return LexerResult::GenSuccess(Token(TokenType::KW_CJUMP, _tokStartPos, "")); }
		else if (std::regex_match(_tokStr, std::regex("CALL<[BIFDL]*>"))) { return LexerResult::GenSuccess(Token(TokenType::KW_CALL, _tokStartPos, _tokStr.substr(5, _tokStr.length() - 6))); }
		else if (std::regex_match(_tokStr, std::regex("STORE<[BIFDL]+>"))) { return LexerResult::GenSuccess(Token(TokenType::KW_STORE, _tokStartPos, _tokStr.substr(6, _tokStr.length() - 7))); }
		else if (std::regex_match(_tokStr, std::regex("ADD<[BIFDL][BIFDL][BIFDL]>"))) { return LexerResult::GenSuccess(Token(TokenType::KW_ADD, _tokStartPos, _tokStr.substr(4, _tokStr.length() - 5))); }
		else if (std::regex_match(_tokStr, std::regex("SUB<[BIFDL][BIFDL][BIFDL]>"))) { return LexerResult::GenSuccess(Token(TokenType::KW_SUB, _tokStartPos, _tokStr.substr(4, _tokStr.length() - 5))); }
		else if (std::regex_match(_tokStr, std::regex("MUL<[BIFDL][BIFDL][BIFDL]>"))) { return LexerResult::GenSuccess(Token(TokenType::KW_MUL, _tokStartPos, _tokStr.substr(4, _tokStr.length() - 5))); }
		else if (std::regex_match(_tokStr, std::regex("DIV<[BIFDL][BIFDL][BIFDL]>"))) { return LexerResult::GenSuccess(Token(TokenType::KW_DIV, _tokStartPos, _tokStr.substr(4, _tokStr.length() - 5))); }
		else if (std::regex_match(_tokStr, std::regex("MOD<[BIFDL][BIFDL][BIFDL]>"))) { return LexerResult::GenSuccess(Token(TokenType::KW_MOD, _tokStartPos, _tokStr.substr(4, _tokStr.length() - 5))); }
		else if (std::regex_match(_tokStr, std::regex("POW<[BIFDL][BIFDL][BIFDL]>"))) { return LexerResult::GenSuccess(Token(TokenType::KW_POW, _tokStartPos, _tokStr.substr(4, _tokStr.length() - 5))); }
		else if (std::regex_match(_tokStr, std::regex("LSHIFT<[BIFDL][BIFDL][BIFDL]>"))) { return LexerResult::GenSuccess(Token(TokenType::KW_LSHIFT, _tokStartPos, _tokStr.substr(7, _tokStr.length() - 8))); }
		else if (std::regex_match(_tokStr, std::regex("RSHIFT<[BIFDL][BIFDL][BIFDL]>"))) { return LexerResult::GenSuccess(Token(TokenType::KW_RSHIFT, _tokStartPos, _tokStr.substr(7, _tokStr.length() - 8))); }
		else if (std::regex_match(_tokStr, std::regex("LT<[BIFDL][BIFDL][BIFDL]>"))) { return LexerResult::GenSuccess(Token(TokenType::KW_LT, _tokStartPos, _tokStr.substr(3, _tokStr.length() - 4))); }
		else if (std::regex_match(_tokStr, std::regex("GT<[BIFDL][BIFDL][BIFDL]>"))) { return LexerResult::GenSuccess(Token(TokenType::KW_GT, _tokStartPos, _tokStr.substr(3, _tokStr.length() - 4))); }
		else if (std::regex_match(_tokStr, std::regex("LTEQ<[BIFDL][BIFDL][BIFDL]>"))) { return LexerResult::GenSuccess(Token(TokenType::KW_LTEQ, _tokStartPos, _tokStr.substr(5, _tokStr.length() - 6))); }
		else if (std::regex_match(_tokStr, std::regex("GTEQ<[BIFDL][BIFDL][BIFDL]>"))) { return LexerResult::GenSuccess(Token(TokenType::KW_GTEQ, _tokStartPos, _tokStr.substr(5, _tokStr.length() - 6))); }
		else if (std::regex_match(_tokStr, std::regex("EQ<[BIFDL][BIFDL][BIFDL]>"))) { return LexerResult::GenSuccess(Token(TokenType::KW_EQ, _tokStartPos, _tokStr.substr(3, _tokStr.length() - 4))); }
		else if (std::regex_match(_tokStr, std::regex("NEQ<[BIFDL][BIFDL][BIFDL]>"))) { return LexerResult::GenSuccess(Token(TokenType::KW_NEQ, _tokStartPos, _tokStr.substr(4, _tokStr.length() - 5))); }
		else if (std::regex_match(_tokStr, std::regex("BAND<[BIFDL][BIFDL][BIFDL]>"))) { return LexerResult::GenSuccess(Token(TokenType::KW_BAND, _tokStartPos, _tokStr.substr(5, _tokStr.length() - 6))); }
		else if (std::regex_match(_tokStr, std::regex("BOR<[BIFDL][BIFDL][BIFDL]>"))) { return LexerResult::GenSuccess(Token(TokenType::KW_BOR, _tokStartPos, _tokStr.substr(4, _tokStr.length() - 5))); }
		else if (std::regex_match(_tokStr, std::regex("BXOR<[BIFDL][BIFDL][BIFDL]>"))) { return LexerResult::GenSuccess(Token(TokenType::KW_BXOR, _tokStartPos, _tokStr.substr(5, _tokStr.length() - 6))); }
		else if (std::regex_match(_tokStr, std::regex("LAND<[BIFDL][BIFDL][BIFDL]>"))) { return LexerResult::GenSuccess(Token(TokenType::KW_LAND, _tokStartPos, _tokStr.substr(5, _tokStr.length() - 6))); }
		else if (std::regex_match(_tokStr, std::regex("LOR<[BIFDL][BIFDL][BIFDL]>"))) { return LexerResult::GenSuccess(Token(TokenType::KW_LOR, _tokStartPos, _tokStr.substr(4, _tokStr.length() - 5))); }
		else if (std::regex_match(_tokStr, std::regex("NEG<[BIFDL][BIFDL]>"))) { return LexerResult::GenSuccess(Token(TokenType::KW_NEG, _tokStartPos, _tokStr.substr(4, _tokStr.length() - 5))); }
		else if (std::regex_match(_tokStr, std::regex("LNOT<[BIFDL][BIFDL]>"))) { return LexerResult::GenSuccess(Token(TokenType::KW_LNOT, _tokStartPos, _tokStr.substr(4, _tokStr.length() - 5))); }
		else if (std::regex_match(_tokStr, std::regex("BNOT<[BIFDL][BIFDL]>"))) { return LexerResult::GenSuccess(Token(TokenType::KW_BNOT, _tokStartPos, _tokStr.substr(4, _tokStr.length() - 5))); }
		else { return LexerResult::GenError(LexerResultType::INVALID, _tokStr, _tokStartPos); }
	}

	bool IsWhitespace(char _char) { return _char == ' ' || _char == '\n' || _char == '\r' || _char == '\t'; }

	LexerResult Lexer::GetNextToken()
	{
		std::string tokenStr;
		Position tokStartPos;
		bool readingToken = false;

#pragma region Get Next Word
		while (true)
		{
			char peekedChar = (char)PeekNextChar();

			if (IsWhitespace(peekedChar))
			{
				if (readingToken) { break; }
				else { IGNORE(ReadNextChar()); }
			}
			else if (peekedChar == '`')
			{
				if (readingToken) { break; }
				else { IGNORE(ReadNextChar()); }

				LexerResult commentRes = LexComment(this, position);

				if (!commentRes.IsSuccess()) { return commentRes; }
				else { continue; }
			}
			else if (peekedChar == EOF)
			{
				if (readingToken) { break; }
				else { IGNORE(ReadNextChar()); }

				return LexerResult::GenSuccess(Token::END_OF_FILE(position));
			}
			else
			{
				tokenStr.push_back((char)ReadNextChar());

				if (!readingToken)
					tokStartPos = position;

				readingToken = true;
			}
		}
#pragma endregion

#pragma region Lex it
		char firstChar = tokenStr[0];

		switch (firstChar)
		{
			case '%': {
				if (std::regex_match(tokenStr, REGEX_LABEL))
				{
					std::string id = tokenStr.substr(1, tokenStr.length() - 1);
					return LexerResult::GenSuccess(Token(TokenType::LABEL, tokStartPos, id));
				}
				else { return LexerResult::GenError(LexerResultType::INVAILD_LABEL_ID, tokenStr, position); }
			}
			case 'R': {
				if (std::regex_match(tokenStr, REGEX_REG))
				{
					std::string idx = tokenStr.substr(1, tokenStr.length() - 1);

					if (IsInt(idx)) { return LexerResult::GenSuccess(Token(TokenType::REG, tokStartPos, idx)); }
					else { return LexerResult::GenError(LexerResultType::INVALID_REG_IDX, tokenStr, position); }
				}
				else { break; } //Break because RET starts with R
			}
			case '{': {
				if (tokenStr.back() != '}') { return LexerResult::GenError(LexerResultType::MISSING_CLOSE_BRACKET, "}", position); }
				else if (std::regex_match(tokenStr, REGEX_MEM_REG))
				{
					std::string idx = tokenStr.substr(2, tokenStr.length() - 3);

					if (IsInt(idx)) { return LexerResult::GenSuccess(Token(TokenType::MEM_REG, tokStartPos, idx)); }
					else { return LexerResult::GenError(LexerResultType::INVALID_REG_IDX, tokenStr, position); }
				}
				else { return LexerResult::GenError(LexerResultType::INVALID_REGISTER, tokenStr, position); }
			}
			case '[': {
				if (tokenStr.back() != ']') { return LexerResult::GenError(LexerResultType::MISSING_CLOSE_BRACKET, "]", position); }
				else
				{
					if (std::regex_match(tokenStr, REGEX_STACK_REG))
					{
						std::string idx = tokenStr.substr(2, tokenStr.length() - 3);

						if (IsInt(idx)) { return LexerResult::GenSuccess(Token(TokenType::STACK_REG, tokStartPos, idx)); }
						else { return LexerResult::GenError(LexerResultType::INVALID_REG_IDX, tokenStr, position); }
					}
					else if (std::regex_match(tokenStr, REGEX_SP_OFFSET))
					{
						std::string idx = tokenStr.substr(1, tokenStr.length() - 2);

						if (IsInt(idx)) { return LexerResult::GenSuccess(Token(TokenType::SP_OFFSET, tokStartPos, idx)); }
						else { return LexerResult::GenError(LexerResultType::INVALID_OFFSET, tokenStr, position); }
					}
					else { return LexerResult::GenExpectationError("register or offset", tokenStr.substr(1, tokenStr.length() - 2), position); }
				}
			}
		}

#pragma region Try Lex as Numeric Literal
		if (std::regex_match(tokenStr, std::regex("-?0|[1-9][0-9]*")))
		{
			try { return LexerResult::GenSuccess(Token(TokenType::HEX_LIT, tokStartPos, ToByteString(std::stoll(tokenStr)))); }
			catch (...) { return LexerResult::GenError(LexerResultType::HEX_LIT_OOB, tokenStr, tokStartPos); }
		}
		else if (std::regex_match(tokenStr, std::regex("-?(0|[1-9][0-9]*)(d|(\\.[0-9]+))")))
		{
			try { return LexerResult::GenSuccess(Token(TokenType::HEX_LIT, tokStartPos, ToByteString(std::stod(tokenStr)))); }
			catch (...) { return LexerResult::GenError(LexerResultType::HEX_LIT_OOB, tokenStr, tokStartPos); }
		}
		else if (std::regex_match(tokenStr, REGEX_HEX_LIT))
		{
			if (tokenStr.length() > LONG_SIZE * 2 + 2) { return LexerResult::GenError(LexerResultType::HEX_LIT_OOB, tokenStr, tokStartPos); }
			else if (tokenStr.length() % 2 == 1) { return LexerResult::GenError(LexerResultType::HEX_LIT_INVALID, tokenStr, tokStartPos); }
			else { return LexerResult::GenSuccess(Token(TokenType::HEX_LIT, tokStartPos, HexToByteStr(tokenStr) + std::string((LONG_SIZE - ((tokenStr.length() >> 1) - 1)), '\0'))); }
		}
#pragma endregion

		if (std::regex_match(tokenStr, REGEX_SP)) { return LexerResult::GenSuccess(Token(TokenType::KW_SP, tokStartPos, "")); }
		else { return LexKeyword(tokenStr, tokStartPos); }
#pragma endregion
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
