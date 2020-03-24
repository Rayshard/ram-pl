#pragma once
#include "ramc_token.h"

namespace ramc {
	enum class LexerResultType {
		SUCCESS,
		INT_LIT_OOB,
		FLOAT_LIT_OOB,
		STR_LIT_CLOSE,
		COMMENT_CLOSE,
		STR_LIT_INVALID_ESC,
		TOO_MANY_DECIMALS,
		UNKNOWN_SYMBOL
	};

	class LexerResult {
		LexerResultType type;
		Token token;
		
		std::string errString;
		Position errPosition;

	public:
		LexerResultType GetType() { return type; }
		Position GetErrorPosition() { return errPosition; }

		Token GetToken();
		bool IsSuccess();
		std::string ToString();

		static LexerResult GenSuccess(Token _token);
		static LexerResult GenError(LexerResultType _type, std::string _str, Position _pos);
	};

	class Lexer
	{
		std::istream* stream;
		Position position;
		int tabSize;

		Lexer() = default;
	public:
		Lexer(std::istream* _stream, int _tabSize);

		int ReadNextChar();
		int PeekNextChar();
		LexerResult GetNextToken(bool _ignoreWhitespace, bool _ignoreComments);

		Position GetPosition() { return position; }
	};
}
