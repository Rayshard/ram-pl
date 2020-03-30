#pragma once
#include "ramvm_token.h"
#include "lexer.h"

namespace ramvm {
	enum class LexerResultType {
		SUCCESS,
		HEX_LIT_OOB,
		HEX_LIT_INVALID,
		INVAILD_LABEL_ID,
		INSTR_OFFSET_OOB,
		INVALID_REGISTER,
		INVALID_REG_IDX,
		INVALID_OFFSET,
		MISSING_CLOSE_BRACKET,
		COMMENT_CLOSE,
		INVALID,
		INVALID_STACK_POS,
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

		static const std::regex REGEX_LABEL, REGEX_REG, REGEX_MEM_REG, REGEX_STACK_REG, REGEX_INSTR_OFFSET,
			REGEX_SP_OFFSET, REGEX_HEX_LIT, REGEX_INT_LIT, REGEX_FLOAT_LIT, REGEX_DOUBLE_LIT, REGEX_SP,
			REGEX_STACK_POS;

		LexerResult GetNextToken() override;
	};

	std::string LexFile(std::istream* _stream, int _tabSize);
}
