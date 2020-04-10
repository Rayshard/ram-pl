#pragma once
#include "ramc_token.h"
#include "lexer.h"

namespace ramc {
	enum class LexerResultType {
		SUCCESS,
		BYTE_LIT_OOB,
		INT_LIT_OOB,
		FLOAT_LIT_OOB,
		DOUBLE_LIT_OOB,
		LONG_LIT_OOB,
		STR_LIT_CLOSE,
		COMMENT_CLOSE,
		STR_LIT_INVALID_ESC,
		TOO_MANY_DECIMALS,
		UNKNOWN_SYMBOL
	};

	class LexerResult : public Result<LexerResultType, Token> {
		LexerResult(LexerResultType _type, Token _val, std::string _errStr, Position _errPos)
			: Result(_type, _val, _errStr, _errPos) { }
	public:
		Token GetValue() override { return IsSuccess() ? value : Token::INVALID(errPosition); }
		std::string ToString(bool _includePos) override;

		static LexerResult GenSuccess(Token _token) { return LexerResult(LexerResultType::SUCCESS, _token, "", Position()); }
		static LexerResult GenError(LexerResultType _type, std::string _str, Position _pos) { return LexerResult(_type, Token(), _str, _pos); }
	};

	class Lexer : public RTLexer<LexerResult, Token>
	{
		bool ignoreWhitespace, ignoreComments;
	public:
		static const std::unordered_map<std::string, TokenType> KEYWORDS;

		Lexer(std::istream* _stream, bool _ignoreWhitespace, bool _ignoreComments, int _tabSize)
			: RTLexer(_stream, _tabSize), ignoreWhitespace(_ignoreWhitespace), ignoreComments(_ignoreComments) { }

		LexerResult GetNextToken() override;
	};

	std::string LexFile(std::istream* _stream, bool _ignoreWhitespace, bool _ignoreComments, int _tabSize);
}
