#include "pch.h"
#include "ramc_token.h"

namespace ramc {
	Token::Token(TokenType _type, Position _pos, std::string _val)
		: TToken(_type, _pos, _val)
	{
		type = _type;
		position = _pos;
		value = _val;
	}

	std::string Token::ToString(bool _includePos)
	{
		std::string str = _includePos ? "(" + position.ToString() + ") " : "";

		switch (type)
		{
			case TokenType::INT_LIT: str += "Interger Literal: " + value; break;
			case TokenType::FLOAT_LIT: str += "Float Literal: " + value; break;
			case TokenType::STRING_LIT: str += "String Literal: " + value; break;
			
			case TokenType::KW_TRUE: str += "Keyword: true"; break;
			case TokenType::KW_FALSE: str += "Keyword: false"; break;

			case TokenType::KW_LET: str += "Keyword: let"; break;
			case TokenType::KW_IF: str += "Keyword: if"; break;
			case TokenType::KW_THEN: str += "Keyword: then"; break;
			case TokenType::KW_ELSE: str += "Keyword: else"; break;
			case TokenType::KW_WHILE: str += "Keyword: while"; break;
			case TokenType::KW_FOR: str += "Keyword: for"; break;
			case TokenType::KW_DO: str += "Keyword: do"; break;
			case TokenType::KW_FUNC: str += "Keyword: func"; break;

			case TokenType::KW_CONTINUE: str += "Keyword: continue"; break;
			case TokenType::KW_BREAK: str += "Keyword: break"; break;
			case TokenType::KW_RETURN: str += "Keyword: return"; break;
			
			case TokenType::KW_BYTE: str += "Keyword: byte"; break;
			case TokenType::KW_BOOL: str += "Keyword: bool"; break;
			case TokenType::KW_INT: str += "Keyword: int"; break;
			case TokenType::KW_FLOAT: str += "Keyword: float"; break;
			case TokenType::KW_DOUBLE: str += "Keyword: double"; break;
			case TokenType::KW_LONG: str += "Keyword: long"; break;
			case TokenType::KW_STRING: str += "Keyword: string"; break;
			case TokenType::KW_VOID: str += "Keyword: void"; break;

			case TokenType::IDENTIFIER: str += "Identifier: " + value; break;
			case TokenType::UNDERSCORE: str += "Underscore"; break;
			case TokenType::WHITESPACE: str += "Whitespace"; break;

			case TokenType::SEMICOLON: str += "Semicolon"; break;
			case TokenType::PERIOD: str += "Period"; break;
			case TokenType::COLON: str += "Colon"; break;
			case TokenType::COMMA: str += "Comma"; break;
			case TokenType::GOES_TO: str += "Goes To"; break;

			case TokenType::PLUS: str += "Plus"; break;
			case TokenType::MINUS: str += "Minus"; break;
			case TokenType::TIMES: str += "Times"; break;
			case TokenType::DIVIDE: str += "Divide"; break;
			case TokenType::POW: str += "Pow"; break;
			case TokenType::MOD: str += "Mod"; break;
			case TokenType::PLUS_EQ: str += "PlusEqual"; break;
			case TokenType::MINUS_EQ: str += "MinusEqual"; break;
			case TokenType::TIMES_EQ: str += "TimesEqual"; break;
			case TokenType::DIVIDE_EQ: str += "DivideEqual"; break;
			case TokenType::POW_EQ: str += "PowEqual"; break;
			case TokenType::MOD_EQ: str += "ModEqual"; break;

			case TokenType::BIN_AND: str += "BinaryAnd"; break;
			case TokenType::BIN_OR: str += "BinaryOr"; break;
			case TokenType::BIN_XOR: str += "BinaryXor"; break;
			case TokenType::LSHIFT: str += "ShiftLeft"; break;
			case TokenType::RSHIFT: str += "ShiftRight"; break;
			case TokenType::BIN_AND_EQ: str += "BinaryAndEqual"; break;
			case TokenType::BIN_OR_EQ: str += "BinaryOrEqual"; break;
			case TokenType::BIN_XOR_EQ: str += "BinaryXorEqual"; break;
			case TokenType::LSHIFT_EQ: str += "ShiftLeftEqual"; break;
			case TokenType::RSHIFT_EQ: str += "ShiftRightEqual"; break;

			case TokenType::LT: str += "LessThan"; break;
			case TokenType::GT: str += "GreaterThan"; break;
			case TokenType::LT_EQ: str += "LessThanEqual"; break;
			case TokenType::GT_EQ: str += "GreaterThanEqual"; break;
			case TokenType::EQ_EQ: str += "EqualEqual"; break;
			case TokenType::NEQ: str += "NotEqual"; break;
			case TokenType::EQ: str += "Equal"; break;
			case TokenType::LOG_AND: str += "LogicalAnd"; break;
			case TokenType::LOG_OR: str += "LogicalOr"; break;
			case TokenType::LOG_NOT: str += "LogicalNot"; break;
			case TokenType::BIN_NOT: str += "BinaryNot"; break;

			case TokenType::LPAREN: str += "LeftParenthesis"; break;
			case TokenType::RPAREN: str += "RightParenthesis"; break;
			case TokenType::LCBRACKET: str += "LeftCurlyBracket"; break;
			case TokenType::RCBRACKET: str += "RightCurlyBracket"; break;
			case TokenType::LSBRACKET: str += "LeftSquareBracket"; break;
			case TokenType::RSBRACKET: str += "RightSquareBracket"; break;

			case TokenType::COMMENT: str += "Comment: " + value; break;
			case TokenType::INVALID: str += "Invalid"; break;
			case TokenType::END_OF_FILE: str += "EOF"; break;
			default: str += "Token::ToString - TokenType not handled."; break;
		}

		return str;
	}
}
