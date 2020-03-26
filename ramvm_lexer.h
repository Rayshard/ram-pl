#pragma once
#include "ramvm_token.h"

namespace ramvm {
	enum class LexerResultType {
		SUCCESS,
		HEX_LIT_OOB,
		HEX_LIT_INVALID,
		INVAILD_LABEL_ID,
		INVALID_REGISTER,
		INVALID_REG_IDX,
		INVALID_OFFSET,
		MISSING_CLOSE_BRACKET,
		COMMENT_CLOSE,
		INVALID,
		UNKNOWN_TOKEN
	};

	class LexerResult {
		LexerResultType type;
		Token token;

		std::string errString;
		Position errPosition;

	public:
		LexerResultType GetType() { return type; }
		Position GetErrorPosition() { return errPosition; }
		std::string GetErrorString() { return errString; }

		Token GetToken();
		bool IsSuccess();
		std::string ToString(bool _includePos);

		static LexerResult GenSuccess(Token _token);
		static LexerResult GenError(LexerResultType _type, std::string _str, Position _pos);
		static LexerResult GenExpectationError(std::string _expected, std::string _found, Position _pos);
	};

	class Lexer
	{
		std::istream* stream;
		Position position;
		int tabSize;

		Lexer() = default;
	public:
		const std::regex REGEX_LABEL = std::regex("%[A-Za-z_][A-Za-z0-9_]*");
		const std::regex REGEX_REG = std::regex("R(0|[1-9][0-9]*)");
		const std::regex REGEX_MEM_REG = std::regex("\\{R(0|[1-9][0-9]*)\\}");
		const std::regex REGEX_STACK_REG = std::regex("\\[R(0|[1-9][0-9]*)\\]");
		const std::regex REGEX_SP_OFFSET = std::regex("\\[(0|1|-[1-9][0-9]*)\\]");
		const std::regex REGEX_HEX_LIT = std::regex("0x[A-Fa-f0-9]+");
		const std::regex REGEX_SP = std::regex("SP");

		Lexer(std::istream* _stream, int _tabSize);

		int ReadNextChar();
		int PeekNextChar();
		LexerResult GetNextToken();

		Position GetPosition() { return position; }
	};

	std::string LexFile(std::istream* _stream, int _tabSize);
}
