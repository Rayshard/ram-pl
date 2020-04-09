#include "pch.h"
#include "ramvm_token.h"

namespace ramvm {
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
            case TokenType::HEX_LIT: str += "Hex Literal: " + ToHexString((byte*)value.c_str(), value.length()); break;
            case TokenType::BYTE_LIT: str += "Byte Literal: " + value; break;
            case TokenType::INT_LIT: str += "Integer Literal: " + value; break;
            case TokenType::FLOAT_LIT: str += "Float Literal: " + value; break;
            case TokenType::DOUBLE_LIT: str += "Double Literal: " + value; break;
            case TokenType::LONG_LIT: str += "Long Literal: " + value; break;
            case TokenType::LABEL: str += "Label: " + value; break;

            case TokenType::SINGLE_TYPE: str += "Single Type: " + value; break;
            case TokenType::DOUBLE_TYPE: str += "Double Type: " + value; break;
            case TokenType::TRIPLE_TYPE: str += "Triple Type: " + value; break;
            
            case TokenType::LSBRACKET: str += "["; break;
            case TokenType::RSBRACKET: str += "]"; break;
            case TokenType::LCBRACKET: str += "{"; break;
            case TokenType::RCBRACKET: str += "}"; break;
            case TokenType::LPAREN: str += "("; break;
            case TokenType::RPAREN: str += ")"; break;
            case TokenType::COMMA: str += ","; break;
            case TokenType::SEMICOLON: str += ";"; break;
            case TokenType::PLUS: str += "+"; break;
            case TokenType::MINUS: str += "-"; break;
            
            case TokenType::REG_SP: str += "$SP"; break;
            case TokenType::REG_FP: str += "$FP"; break;
            case TokenType::REG_GP: str += "$GP"; break;

            case TokenType::KW_HALT: str += "HALT"; break;
            case TokenType::KW_MOV: str += "MOV"; break;
            case TokenType::KW_RET: str += "RET"; break;
            case TokenType::KW_MALLOC: str += "MALLOC"; break;
            case TokenType::KW_STORE: str += "STORE"; break;
            case TokenType::KW_COMPARE: str += "COMPARE"; break;
            case TokenType::KW_FREE: str += "FREE"; break;
            case TokenType::KW_PUSH: str += "PUSH"; break;
            case TokenType::KW_POP: str += "POP"; break;
            case TokenType::KW_PRINT: str += "PRINT"; break;
            case TokenType::KW_JUMP: str += "JUMP"; break;
            case TokenType::KW_JUMPT: str += "JUMPT"; break;
            case TokenType::KW_JUMPF: str += "JUMPF"; break;
            case TokenType::KW_CALL: str += "CALL"; break;
            case TokenType::KW_ADD: str += "ADD"; break;
            case TokenType::KW_SUB: str += "SUB"; break;
            case TokenType::KW_MUL: str += "MUL"; break;
            case TokenType::KW_DIV: str += "DIV"; break;
            case TokenType::KW_MOD: str += "MOD"; break;
            case TokenType::KW_POW: str += "POW"; break;
            case TokenType::KW_LSHIFT: str += "LSHIFT"; break;
            case TokenType::KW_RSHIFT: str += "RSHIFT"; break;
            case TokenType::KW_AND: str += "AND"; break;
            case TokenType::KW_OR: str += "OR"; break;
            case TokenType::KW_XOR: str += "XOR"; break;
            case TokenType::KW_LT: str += "LT"; break;
            case TokenType::KW_GT: str += "GT"; break;
            case TokenType::KW_LTEQ: str += "LTEQ"; break;
            case TokenType::KW_GTEQ: str += "GTEQ"; break;
            case TokenType::KW_EQ: str += "EQ"; break;
            case TokenType::KW_NEQ: str += "NEQ"; break;
            case TokenType::KW_NEG: str += "NEG"; break;
            case TokenType::KW_NOT: str += "NOT"; break;
            case TokenType::INVALID: str += "Invalid"; break;

            case TokenType::COMMENT: str += "Comment: " + value; break;
            case TokenType::END_OF_FILE: str += "EOF"; break;
            default: ASSERT_MSG(false, "Token::ToString - TokenType not handled."); break;
		}

		return str;
	}
}
