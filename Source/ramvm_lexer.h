#pragma once
#include "ramvm_token.h"
#include "lexer.h"

namespace ramvm {
	enum class LexerResultType {
		SUCCESS,
		HEX_LIT_OOB,
		BYTE_LIT_OOB,
		INT_LIT_OOB,
		FLOAT_LIT_OOB,
		DOUBLE_LIT_OOB,
		LONG_LIT_OOB,
		TOO_MANY_DECIMALS,
		INVAILD_LABEL_ID,
		INVALID_REGISTER,
		INVAILD_TYPE_POSTFIX,
		MISSING_TYPE_CLOSE,
		INVALID,
		UNKNOWN_TOKEN
	};

	class LexerResult : public Result<LexerResultType, Token> {
		LexerResult(LexerResultType _type, Token _val, std::string _errStr, Position _errPos)
			: Result(_type, _val, _errStr, _errPos) { }
	public:
		Token GetValue() override { return IsSuccess() ? value : Token::INVALID(errPosition); }
		std::string ToString(bool _includePos) override;

		static LexerResult GenSuccess(Token _token) { return LexerResult(LexerResultType::SUCCESS, _token, "", Position()); }
		static LexerResult GenError(LexerResultType _type, std::string _str, Position _pos) { return LexerResult(_type, Token(), _str, _pos); }
		static LexerResult GenExpectationError(std::string _expected, std::string _found, Position _pos) { return LexerResult(LexerResultType::INVALID, Token(), "Expected " + _expected + "but found " + _found, _pos); }
	};

	class Lexer : public RTLexer<LexerResult, Token>
	{
	public:
		Lexer(std::istream* _stream, int _tabSize)
			: RTLexer(_stream, _tabSize) { }

		LexerResult GetNextToken() override;
	};

	std::string LexFile(std::istream* _stream, int _tabSize);
}
