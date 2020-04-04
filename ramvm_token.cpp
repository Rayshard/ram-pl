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
            case TokenType::REG: str += "Register: " + value; break;
            case TokenType::MEM_REG: str += "Memory Register: " + value; break;
            case TokenType::STACK_REG: str += "Stack Register: " + value; break;
            case TokenType::SP_OFFSET: str += "Stack Pointer Offet: " + value; break;
            case TokenType::LABEL: str += "Label: " + value; break;
            case TokenType::INSTR_OFFSET: str += "Instruction Offset: " + value; break;
            case TokenType::STACK_POS: str += "Stack Position: " + value; break;
            case TokenType::KW_SP: str += "SP"; break;
            case TokenType::KW_HALT: str += "HALT"; break;
            case TokenType::KW_MOV: str += "MOV<" + value + ">"; break;
            case TokenType::KW_RET: str += "RET"; break;
            case TokenType::KW_MALLOC: str += "MALLOC"; break;
            case TokenType::KW_STORE: str += "STORE<" + value + ">"; break;
            case TokenType::KW_COMPARE: str += "COMPARE"; break;
            case TokenType::KW_FREE: str += "FREE"; break;
            case TokenType::KW_PUSH: str += "PUSH<" + value + ">"; break;
            case TokenType::KW_POP: str += "POP<" + value + ">"; break;
            case TokenType::KW_PRINT: str += "PRINT"; break;
            case TokenType::KW_JUMP: str += "JUMP"; break;
            case TokenType::KW_JUMPT: str += "JUMPT"; break;
            case TokenType::KW_JUMPF: str += "JUMPF"; break;
            case TokenType::KW_CALL: str += "CALL"; break;
            case TokenType::KW_ADD: str += "ADD<" + value + ">"; break;
            case TokenType::KW_SUB: str += "SUB<" + value + ">"; break;
            case TokenType::KW_MUL: str += "MUL<" + value + ">"; break;
            case TokenType::KW_DIV: str += "DIV<" + value + ">"; break;
            case TokenType::KW_MOD: str += "MOD<" + value + ">"; break;
            case TokenType::KW_POW: str += "POW<" + value + ">"; break;
            case TokenType::KW_LSHIFT: str += "LSHIFT<" + value + ">"; break;
            case TokenType::KW_RSHIFT: str += "RSHIFT<" + value + ">"; break;
            case TokenType::KW_BAND: str += "BAND<" + value + ">"; break;
            case TokenType::KW_BOR: str += "BOR<" + value + ">"; break;
            case TokenType::KW_BXOR: str += "BXOR<" + value + ">"; break;
            case TokenType::KW_LAND: str += "LAND<" + value + ">"; break;
            case TokenType::KW_LOR: str += "LOR<" + value + ">"; break;
            case TokenType::KW_LT: str += "LT<" + value + ">"; break;
            case TokenType::KW_GT: str += "GT<" + value + ">"; break;
            case TokenType::KW_LTEQ: str += "LTEQ<" + value + ">"; break;
            case TokenType::KW_GTEQ: str += "GTEQ<" + value + ">"; break;
            case TokenType::KW_EQ: str += "EQ<" + value + ">"; break;
            case TokenType::KW_NEQ: str += "NEQ<" + value + ">"; break;
            case TokenType::KW_NEG: str += "NEG<" + value + ">"; break;
            case TokenType::KW_LNOT: str += "LNOT<" + value + ">"; break;
            case TokenType::KW_BNOT: str += "BNOT<" + value + ">"; break;
            case TokenType::COMMENT: str += "Comment: " + value; break;
            case TokenType::INVALID: str += "Invalid"; break;
            case TokenType::END_OF_FILE: str += "EOF"; break;
            default: str += "Token::ToString - TokenType not handled."; break;
		}

		return str;
	}
}
