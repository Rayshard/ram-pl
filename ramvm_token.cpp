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
            case ramvm::TokenType::HEX_LIT: str += "Hex Literal: " + ToHexString((byte*)value.c_str(), value.length()); break;
            case ramvm::TokenType::REG: str += "Register: " + value; break;
            case ramvm::TokenType::MEM_REG: str += "Memory Register: " + value; break;
            case ramvm::TokenType::STACK_REG: str += "Stack Register: " + value; break;
            case ramvm::TokenType::SP_OFFSET: str += "Stack Pointer Offet: " + value; break;
            case ramvm::TokenType::LABEL: str += "Label: " + value; break;
            case ramvm::TokenType::KW_SP: str += "SP"; break;
            case ramvm::TokenType::KW_HALT: str += "HALT"; break;
            case ramvm::TokenType::KW_MOV: str += "MOV"; break;
            case ramvm::TokenType::KW_RET: str += "RET"; break;
            case ramvm::TokenType::KW_MALLOC: str += "MALLOC"; break;
            case ramvm::TokenType::KW_STORE: str += "STORE"; break;
            case ramvm::TokenType::KW_FREE: str += "FREE"; break;
            case ramvm::TokenType::KW_PUSH: str += "PUSH"; break;
            case ramvm::TokenType::KW_POP: str += "POP" + value; break;
            case ramvm::TokenType::KW_PRINT: str += "PRINT"; break;
            case ramvm::TokenType::KW_JUMP: str += "JUMP"; break;
            case ramvm::TokenType::KW_CJUMP: str += "CJUMP"; break;
            case ramvm::TokenType::KW_CALL: str += "CALL"; break;
            case ramvm::TokenType::KW_ADD: str += "ADD"; break;
            case ramvm::TokenType::KW_SUB: str += "SUB"; break;
            case ramvm::TokenType::KW_MUL: str += "MUL"; break;
            case ramvm::TokenType::KW_DIV: str += "DIV"; break;
            case ramvm::TokenType::KW_MOD: str += "MOD"; break;
            case ramvm::TokenType::KW_POW: str += "POW"; break;
            case ramvm::TokenType::KW_LSHIFT: str += "LSHIFT"; break;
            case ramvm::TokenType::KW_RSHIFT: str += "RSHIFT"; break;
            case ramvm::TokenType::KW_BAND: str += "BAND"; break;
            case ramvm::TokenType::KW_BOR: str += "BOR"; break;
            case ramvm::TokenType::KW_BXOR: str += "BXOR"; break;
            case ramvm::TokenType::KW_LAND: str += "LAND"; break;
            case ramvm::TokenType::KW_LOR: str += "LOR"; break;
            case ramvm::TokenType::KW_LT: str += "LT"; break;
            case ramvm::TokenType::KW_GT: str += "GT"; break;
            case ramvm::TokenType::KW_LTEQ: str += "LTEQ"; break;
            case ramvm::TokenType::KW_GTEQ: str += "GTEQ"; break;
            case ramvm::TokenType::KW_EQ: str += "EQ"; break;
            case ramvm::TokenType::KW_NEQ: str += "NEQ"; break;
            case ramvm::TokenType::KW_NEG: str += "NEG"; break;
            case ramvm::TokenType::KW_LNOT: str += "LNOT"; break;
            case ramvm::TokenType::KW_BNOT: str += "BNOT"; break;
            case TokenType::COMMENT: str += "Comment: " + value; break;
            case TokenType::INVALID: str += "Invalid"; break;
            case TokenType::END_OF_FILE: str += "EOF"; break;
            default: str += "Token::ToString - TokenType not handled."; break;
		}

		return str;
	}
}
