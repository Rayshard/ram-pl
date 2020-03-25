#include "pch.h"
#include "ramvm_lexer.h"

namespace ramvm {
	bool LexerResult::IsSuccess() { return type == LexerResultType::SUCCESS; }
	Token LexerResult::GetToken() { return IsSuccess() ? token : Token::INVALID(errPosition); }

	std::string LexerResult::ToString()
	{
		std::string prefix = "(" + errPosition.ToString() + ") ";

		switch (type)
		{
			case LexerResultType::SUCCESS: return "SUCCESS";
			case LexerResultType::HEX_LIT_OOB: return prefix + "Hex value can have at most 16 digits: " + errString;
			case LexerResultType::HEX_LIT_INVALID: return prefix + "Hex value must express a sequence of bytes: " + errString;
			case LexerResultType::INVALID_REGISTER: return prefix + "Invalid Register: " + errString;
			case LexerResultType::INVALID_REG_IDX: return prefix + "Invalid Register Index: " + errString;
			case LexerResultType::INVAILD_LABEL_ID: return prefix + "Invalid Label Identifier: " + errString;
			case LexerResultType::INVALID_OFFSET: return prefix + "Invalid Offset: " + errString;
			case LexerResultType::COMMENT_CLOSE: return prefix + "Missing closing '`' for comment: " + errString;
			case LexerResultType::MISSING_CLOSE_BRACKET: return prefix + "Missing closing bracket: " + errString;
			case LexerResultType::INVALID: return prefix + "Invalid: " + errString;
			case LexerResultType::UNKNOWN_TOKEN: return prefix + "Unknown Token: " + errString;
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
	LexerResult LexerResult::GenExpectationError(std::string _expected, std::string _found, Position _pos)
	{
		LexerResult result;
		result.type = LexerResultType::INVALID;
		result.errString = "Expected " + _expected + "but found " + _found;
		result.errPosition = _pos;
		return result;
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
		else if (_tokStr == "RET") { return LexerResult::GenSuccess(Token(TokenType::KW_RET, _tokStartPos, "")); }
		else if (_tokStr == "MALLOC") { return LexerResult::GenSuccess(Token(TokenType::KW_MALLOC, _tokStartPos, "")); }
		else if (_tokStr == "FREE") { return LexerResult::GenSuccess(Token(TokenType::KW_FREE, _tokStartPos, "")); }
		else if (_tokStr == "PUSH") { return LexerResult::GenSuccess(Token(TokenType::KW_PUSH, _tokStartPos, "")); }
		else if (std::regex_match(_tokStr, std::regex("POP[IB]"))) { return LexerResult::GenSuccess(Token(TokenType::KW_POP, _tokStartPos, std::string(1, _tokStr.back()))); }
		else if (_tokStr == "MOV") { return LexerResult::GenSuccess(Token(TokenType::KW_MOV, _tokStartPos, "")); }
		else if (_tokStr == "PRINT") { return LexerResult::GenSuccess(Token(TokenType::KW_PRINT, _tokStartPos, "")); }
		else if (_tokStr == "JUMP") { return LexerResult::GenSuccess(Token(TokenType::KW_JUMP, _tokStartPos, "")); }
		else if (_tokStr == "CJUMP") { return LexerResult::GenSuccess(Token(TokenType::KW_CJUMP, _tokStartPos, "")); }
		else if (_tokStr == "CALL") { return LexerResult::GenSuccess(Token(TokenType::KW_CALL, _tokStartPos, "")); }
		else if (_tokStr == "ADD") { return LexerResult::GenSuccess(Token(TokenType::KW_ADD, _tokStartPos, "")); }
		else if (_tokStr == "SUB") { return LexerResult::GenSuccess(Token(TokenType::KW_SUB, _tokStartPos, "")); }
		else if (_tokStr == "MUL") { return LexerResult::GenSuccess(Token(TokenType::KW_MUL, _tokStartPos, "")); }
		else if (_tokStr == "DIV") { return LexerResult::GenSuccess(Token(TokenType::KW_DIV, _tokStartPos, "")); }
		else if (_tokStr == "MOD") { return LexerResult::GenSuccess(Token(TokenType::KW_MOD, _tokStartPos, "")); }
		else if (_tokStr == "LSHIFT") { return LexerResult::GenSuccess(Token(TokenType::KW_LSHIFT, _tokStartPos, "")); }
		else if (_tokStr == "RSHIFT") { return LexerResult::GenSuccess(Token(TokenType::KW_RSHIFT, _tokStartPos, "")); }
		else if (_tokStr == "LT") { return LexerResult::GenSuccess(Token(TokenType::KW_LT, _tokStartPos, "")); }
		else if (_tokStr == "GT") { return LexerResult::GenSuccess(Token(TokenType::KW_GT, _tokStartPos, "")); }
		else if (_tokStr == "LTEQ") { return LexerResult::GenSuccess(Token(TokenType::KW_LTEQ, _tokStartPos, "")); }
		else if (_tokStr == "GTEQ") { return LexerResult::GenSuccess(Token(TokenType::KW_GTEQ, _tokStartPos, "")); }
		else if (_tokStr == "EQ") { return LexerResult::GenSuccess(Token(TokenType::KW_EQ, _tokStartPos, "")); }
		else if (_tokStr == "NEQ") { return LexerResult::GenSuccess(Token(TokenType::KW_NEQ, _tokStartPos, "")); }
		else if (_tokStr == "BAND") { return LexerResult::GenSuccess(Token(TokenType::KW_BAND, _tokStartPos, "")); }
		else if (_tokStr == "BOR") { return LexerResult::GenSuccess(Token(TokenType::KW_BOR, _tokStartPos, "")); }
		else if (_tokStr == "BXOR") { return LexerResult::GenSuccess(Token(TokenType::KW_BXOR, _tokStartPos, "")); }
		else if (_tokStr == "LAND") { return LexerResult::GenSuccess(Token(TokenType::KW_LAND, _tokStartPos, "")); }
		else if (_tokStr == "LOR") { return LexerResult::GenSuccess(Token(TokenType::KW_LOR, _tokStartPos, "")); }
		else if (_tokStr == "NEG") { return LexerResult::GenSuccess(Token(TokenType::KW_NEG, _tokStartPos, "")); }
		else if (_tokStr == "LNOT") { return LexerResult::GenSuccess(Token(TokenType::KW_LNOT, _tokStartPos, "")); }
		else { return LexerResult::GenError(LexerResultType::INVALID, _tokStr, _tokStartPos); }
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
				IGNORE(ReadNextChar());
				return LexerResult::GenSuccess(Token::END_OF_FILE(position));
			}
			else
			{
				if (!readingToken)
					tokStartPos = position;

				tokenStr.push_back((char)ReadNextChar());
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

		if (std::regex_match(tokenStr, std::regex("-?0|[1-9][0-9]*")))
		{
			if (IsInt(tokenStr)) { return LexerResult::GenSuccess(Token(TokenType::HEX_LIT, tokStartPos, ToHexString<int>(std::stoi(tokenStr)))); }
			else { return LexerResult::GenError(LexerResultType::HEX_LIT_OOB, tokenStr, tokStartPos); }
		}
		else if (std::regex_match(tokenStr, std::regex("(0|[1-9][0-9]*)b")))
		{
			if (IsByte(tokenStr)) { return LexerResult::GenSuccess(Token(TokenType::HEX_LIT, tokStartPos, ToHexString<byte>((byte)std::stoul(tokenStr)))); }
			else { return LexerResult::GenError(LexerResultType::HEX_LIT_OOB, tokenStr, tokStartPos); }
		}
		else if (std::regex_match(tokenStr, std::regex("-?(0|[1-9][0-9]*)l")))
		{
			if (IsLong(tokenStr)) { return LexerResult::GenSuccess(Token(TokenType::HEX_LIT, tokStartPos, ToHexString<long>(std::stol(tokenStr)))); }
			else { return LexerResult::GenError(LexerResultType::HEX_LIT_OOB, tokenStr, tokStartPos); }
		}
		else if (std::regex_match(tokenStr, std::regex("-?(0|[1-9][0-9]*)(\\.[0-9]+)?f")))
		{
			if (IsFloat(tokenStr)) { return LexerResult::GenSuccess(Token(TokenType::HEX_LIT, tokStartPos, ToHexString<float>(std::stof(tokenStr)))); }
			else { return LexerResult::GenError(LexerResultType::HEX_LIT_OOB, tokenStr, tokStartPos); }
		}
		else if (std::regex_match(tokenStr, std::regex("-?(0|[1-9][0-9]*)(\\.[0-9]+)?d")))
		{
			if (IsDouble(tokenStr)) { return LexerResult::GenSuccess(Token(TokenType::HEX_LIT, tokStartPos, ToHexString<double>(std::stod(tokenStr)))); }
			else { return LexerResult::GenError(LexerResultType::HEX_LIT_OOB, tokenStr, tokStartPos); }
		}
		else if (std::regex_match(tokenStr, REGEX_HEX_LIT))
		{
			if (tokenStr.length() > 10) { return LexerResult::GenError(LexerResultType::HEX_LIT_OOB, tokenStr, tokStartPos); }
			else if (tokenStr.length() % 2 == 1) { return LexerResult::GenError(LexerResultType::HEX_LIT_INVALID, tokenStr, tokStartPos); }
			else { return LexerResult::GenSuccess(Token(TokenType::HEX_LIT, tokStartPos, tokenStr)); }
		}
		else if (std::regex_match(tokenStr, REGEX_SP)) { return LexerResult::GenSuccess(Token(TokenType::KW_SP, tokStartPos, "")); }
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
