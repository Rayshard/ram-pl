#pragma once

#include "token.h"

namespace ramc {
	enum class TokenType {
		BYTE_LIT, INT_LIT, FLOAT_LIT, DOUBLE_LIT, LONG_LIT, STRING_LIT, 
		KW_TRUE, KW_FALSE,

		KW_LET, KW_IF, KW_THEN, KW_ELSE, KW_WHILE, KW_FOR,
		KW_CONTINUE, KW_BREAK, KW_RETURN,

		KW_BYTE, KW_BOOL, KW_INT, KW_FLOAT, KW_DOUBLE, KW_LONG,
		KW_STRING, KW_VOID,

		IDENTIFIER, UNDERSCORE,
		
		SEMICOLON, PERIOD, COLON, COMMA,

		PLUS, MINUS, TIMES, DIVIDE, POW, MOD,
		PLUS_EQ, MINUS_EQ, TIMES_EQ, DIVIDE_EQ, POW_EQ, MOD_EQ,

		BIN_AND, BIN_OR, BIN_XOR, LSHIFT, RSHIFT,
		BIN_AND_EQ, BIN_OR_EQ, BIN_XOR_EQ, LSHIFT_EQ, RSHIFT_EQ,

		LT, GT, LT_EQ, GT_EQ, EQ_EQ, NEQ, EQ,
		LOG_AND, LOG_OR, LOG_NOT, BIN_NOT,

		INC, DEC,

		LPAREN, RPAREN, LCBRACKET, RCBRACKET, LSBRACKET, RSBRACKET,
		
		WHITESPACE, COMMENT,
		
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