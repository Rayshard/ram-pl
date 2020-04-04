#pragma once

#include "token.h"

namespace ramvm {
	enum class TokenType {
		LSBRACKET, RSBRACKET, LCBRACKET, RCBRACKET, LPAREN, RPAREN,
		SEMICOLON, COMMA, PLUS, MINUS,
		SINGLE_TYPE, DOUBLE_TYPE, TRIPLE_TYPE,
		
		HEX_LIT, BYTE_LIT, INT_LIT, FLOAT_LIT, DOUBLE_LIT, LONG_LIT,
		
		REGISTER, LABEL, KW_SP,

		KW_HALT, KW_MOV, KW_RET, KW_MALLOC, KW_FREE,
		KW_PUSH, KW_POP, KW_PRINT, KW_JUMP, KW_JUMPT,
		KW_JUMPF, KW_CALL, KW_STORE, KW_COMPARE,

		KW_ADD, KW_SUB, KW_MUL, KW_DIV, KW_MOD, KW_POW,
		KW_LSHIFT, KW_RSHIFT, KW_AND, KW_OR, KW_XOR,
		KW_LT, KW_GT, KW_LTEQ, KW_GTEQ, KW_EQ, KW_NEQ,
		KW_NEG, KW_NOT,

		COMMENT,
		INVALID, END_OF_FILE
	};

	struct Token : TToken<TokenType> {
		Token() = default;
		Token(TokenType _type, Position _pos, std::string _val);

		std::string ToString(bool _includePos) override;

		static Token INVALID(Position _pos) { return { TokenType::INVALID, _pos, "" }; }
		static Token END_OF_FILE(Position _pos) { return { TokenType::END_OF_FILE, _pos, "" }; }
	};
}