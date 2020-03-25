%skeleton "lalr1.cc"
%require "3.5"
%language "c++"

%defines
%define api.value.type variant
%define api.token.constructor
%define api.namespace {ramvm::bison}
%define api.parser.class {Parser}

%code top {
  #include "pch.h"
  #include "ramvm_lexer.h"
}

%code requires {
  #include "ramvm_instruction.h"
}

%code {
    namespace ramvm {
		namespace bison {
			// Return the next token.
			auto yylex(Lexer& _lexer, std::vector<Instruction*>& _result, Position& _pos) -> Parser::symbol_type
			{
				LexerResult readRes = _lexer.GetNextToken();

				if (!readRes.IsSuccess())
					throw std::runtime_error(readRes.ToString());

				Token token = readRes.GetToken();
				TokenType type = token.type;
				_pos = token.position;

				switch (type)
				{
					case TokenType::HEX_LIT: return Parser::make_TOK_HEX_LIT();
					case TokenType::REG: return Parser::make_TOK_REG();
					case TokenType::MEM_REG: return Parser::make_TOK_MEM_REG();
					case TokenType::STACK_REG: return Parser::make_TOK_STACK_REG();
					case TokenType::SP_OFFSET: return Parser::make_TOK_SP_OFFSET();
					case TokenType::LABEL: return Parser::make_TOK_LABEL();
					case TokenType::KW_SP: return Parser::make_TOK_SP();
					case TokenType::KW_HALT: return Parser::make_TOK_HALT();
					case TokenType::KW_MOV: return Parser::make_TOK_MOV();
					case TokenType::KW_RET: return Parser::make_TOK_RET();
					case TokenType::KW_MALLOC: return Parser::make_TOK_MALLOC();
					case TokenType::KW_FREE: return Parser::make_TOK_FREE();
					case TokenType::KW_PUSH: return Parser::make_TOK_PUSH();
					case TokenType::KW_POP: return Parser::make_TOK_POP();
					case TokenType::KW_PRINT: return Parser::make_TOK_PRINT();
					case TokenType::KW_JUMP: return Parser::make_TOK_JUMP();
					case TokenType::KW_CJUMP: return Parser::make_TOK_CJUMP();
					case TokenType::KW_CALL: return Parser::make_TOK_CALL();
					case TokenType::KW_ADD: return Parser::make_TOK_ADD();
					case TokenType::KW_SUB: return Parser::make_TOK_SUB();
					case TokenType::KW_MUL: return Parser::make_TOK_MUL();
					case TokenType::KW_DIV: return Parser::make_TOK_DIV();
					case TokenType::KW_MOD: return Parser::make_TOK_MOD();
					case TokenType::KW_LSHIFT: return Parser::make_TOK_LSHIFT();
					case TokenType::KW_RSHIFT: return Parser::make_TOK_RSHIFT();
					case TokenType::KW_BAND: return Parser::make_TOK_BAND();
					case TokenType::KW_BOR: return Parser::make_TOK_BOR();
					case TokenType::KW_BXOR: return Parser::make_TOK_BXOR();
					case TokenType::KW_LAND: return Parser::make_TOK_LAND();
					case TokenType::KW_LOR: return Parser::make_TOK_LOR();
					case TokenType::KW_LT: return Parser::make_TOK_LT();
					case TokenType::KW_GT: return Parser::make_TOK_GT();
					case TokenType::KW_LTEQ: return Parser::make_TOK_LTEQ();
					case TokenType::KW_GTEQ: return Parser::make_TOK_GTEQ();
					case TokenType::KW_EQ: return Parser::make_TOK_EQ();
					case TokenType::KW_NEQ: return Parser::make_TOK_NEQ();
					case TokenType::KW_NEG: return Parser::make_TOK_NEG();
					case TokenType::KW_LNOT: return Parser::make_TOK_LNOT();
					case TokenType::END_OF_FILE: return Parser::make_TOK_END_OF_FILE();
					default: throw std::runtime_error(token.ToString(true) + " is not parasble!");
				}
			}
		}
	}
}

%param { Lexer& lexer }
%param { std::vector<Instruction*>& result }
%param { Position& position }

%token TOK_HEX_LIT "hex"
%token TOK_REG "reg"
%token TOK_MEM_REG "mreg"
%token TOK_STACK_REG "sreg"
%token TOK_SP_OFFSET "spoff"
%token TOK_LABEL "label"
%token TOK_SP "SP"
%token TOK_HALT "HALT"
%token TOK_MOV "MOV"
%token TOK_RET "RET"
%token TOK_MALLOC "MALLOC"
%token TOK_FREE "FREE"
%token TOK_PUSH "PUSH"
%token TOK_POP "POP"
%token TOK_PRINT "PRINT"
%token TOK_JUMP "JUMP"
%token TOK_CJUMP "CJUMP"
%token TOK_CALL "CALL"
%token TOK_ADD "ADD"
%token TOK_SUB "SUB"
%token TOK_MUL "MUL"
%token TOK_DIV "DIV"
%token TOK_MOD "MOD"
%token TOK_LSHIFT "LSHIFT"
%token TOK_RSHIFT "RSHIFT"
%token TOK_BAND "BAND"
%token TOK_BOR "BOR"
%token TOK_BXOR "BXOR"
%token TOK_LAND "LAND"
%token TOK_LOR "LOR"
%token TOK_LT "LT"
%token TOK_GT "GT"
%token TOK_LTEQ "LTEQ"
%token TOK_GTEQ "GTEQ"
%token TOK_EQ "EQ"
%token TOK_NEQ "NEQ"
%token TOK_NEG "NEG"
%token TOK_LNOT "LNOT"
%token TOK_END_OF_FILE 0

%nterm <std::vector<Instruction*>> STMTS;
%nterm <Instruction*> STMT;

%%
%start PROGRAM;
PROGRAM: 
    STMTS   { result = $1; }
;

STMTS:
    %empty      { $$ = { }; }
  | STMTS STMT  { $1.push_back($2); $$ = $1; }
;

STMT:
    "HALT" { $$ = new InstrHalt(); }
;

%%
namespace ramvm {
	namespace bison {
		// Report an error to the user.
		auto Parser::error(const std::string& _msg) -> void
		{
			throw std::runtime_error(_msg);
		}
	}
}

