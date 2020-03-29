%skeleton "lalr1.cc"
%require "3.5"
%language "c++"

%defines
%define api.value.type variant
%define api.token.constructor
%define api.namespace {ramvm::bison}
%define api.parser.class {Parser}
%define parse.lac
%define parse.error verbose

%code top {
	#include "pch.h"
	#include "ramvm_lexer.h"
}

%code requires {
  #include "ramvm_instruction.h"

  using namespace ramvm;
  
	namespace ramvm {
		namespace bison {
			std::vector<TypedArgument> BindArgDataTypes(std::vector<DataType>& _dataTypes, std::vector<Argument>& _args);
		}
	}
}

%code {
    namespace ramvm {
		namespace bison {
			// Return the next token.
			auto yylex(Lexer& _lexer,
						std::vector<Instruction*>& _result,
						Position& _pos,
						std::map<std::string, int>& _labels,
						std::map<Instruction*, std::pair<std::string, Position>>& _ctrlInstrs) -> Parser::symbol_type
			{
				LexerResult readRes = _lexer.GetNextToken();

				if (!readRes.IsSuccess())
				{
					_pos = readRes.GetErrorPosition();
					throw std::runtime_error(readRes.ToString(false));
				}

				Token token = readRes.GetValue();
                std::string value = token.value;
				_pos = token.position;

				switch (token.type)
				{
					case TokenType::HEX_LIT: return Parser::make_TOK_HEX_LIT(DataValue((byte*)value.c_str(), value.length()));
					case TokenType::REG: return Parser::make_TOK_REG(std::stoi(value));
					case TokenType::MEM_REG: return Parser::make_TOK_MEM_REG(std::stoi(value));
					case TokenType::STACK_REG: return Parser::make_TOK_STACK_REG(std::stoi(value));
					case TokenType::SP_OFFSET: return Parser::make_TOK_SP_OFFSET(std::stoi(value));
					case TokenType::LABEL: return Parser::make_TOK_LABEL(value);
					case TokenType::INSTR_OFFSET: return Parser::make_TOK_INSTR_OFFSET(std::stoi(value));
					case TokenType::KW_SP: return Parser::make_TOK_SP();
					case TokenType::KW_HALT: return Parser::make_TOK_HALT();
					case TokenType::KW_COMPARE: return Parser::make_TOK_COMPARE();
					case TokenType::KW_MOV: return Parser::make_TOK_MOV(CharToDataType(value[0]));
					case TokenType::KW_RET: return Parser::make_TOK_RET();
					case TokenType::KW_MALLOC: return Parser::make_TOK_MALLOC();
					case TokenType::KW_FREE: return Parser::make_TOK_FREE();
					case TokenType::KW_PUSH: return Parser::make_TOK_PUSH(CharsToDataTypes(value));
					case TokenType::KW_POP: return Parser::make_TOK_POP(CharToDataType(value[0]));
					case TokenType::KW_PRINT: return Parser::make_TOK_PRINT();
					case TokenType::KW_JUMP: return Parser::make_TOK_JUMP();
					case TokenType::KW_JUMPT: return Parser::make_TOK_JUMPT();
					case TokenType::KW_JUMPF: return Parser::make_TOK_JUMPF();
					case TokenType::KW_CALL: return Parser::make_TOK_CALL();
					case TokenType::KW_STORE: return Parser::make_TOK_STORE(CharsToDataTypes(value));
					case TokenType::KW_ADD: return Parser::make_TOK_ADD(CharsToDataTypes(value[0], value[1], value[2]));
					case TokenType::KW_SUB: return Parser::make_TOK_SUB(CharsToDataTypes(value[0], value[1], value[2]));
					case TokenType::KW_MUL: return Parser::make_TOK_MUL(CharsToDataTypes(value[0], value[1], value[2]));
					case TokenType::KW_DIV: return Parser::make_TOK_DIV(CharsToDataTypes(value[0], value[1], value[2]));
					case TokenType::KW_MOD: return Parser::make_TOK_MOD(CharsToDataTypes(value[0], value[1], value[2]));
					case TokenType::KW_POW: return Parser::make_TOK_POW(CharsToDataTypes(value[0], value[1], value[2]));
					case TokenType::KW_LSHIFT: return Parser::make_TOK_LSHIFT(CharsToDataTypes(value[0], value[1], value[2]));
					case TokenType::KW_RSHIFT: return Parser::make_TOK_RSHIFT(CharsToDataTypes(value[0], value[1], value[2]));
					case TokenType::KW_BAND: return Parser::make_TOK_BAND(CharsToDataTypes(value[0], value[1], value[2]));
					case TokenType::KW_BOR: return Parser::make_TOK_BOR(CharsToDataTypes(value[0], value[1], value[2]));
					case TokenType::KW_BXOR: return Parser::make_TOK_BXOR(CharsToDataTypes(value[0], value[1], value[2]));
					case TokenType::KW_LAND: return Parser::make_TOK_LAND(CharsToDataTypes(value[0], value[1], value[2]));
					case TokenType::KW_LOR: return Parser::make_TOK_LOR(CharsToDataTypes(value[0], value[1], value[2]));
					case TokenType::KW_LT: return Parser::make_TOK_LT(CharsToDataTypes(value[0], value[1], value[2]));
					case TokenType::KW_GT: return Parser::make_TOK_GT(CharsToDataTypes(value[0], value[1], value[2]));
					case TokenType::KW_LTEQ: return Parser::make_TOK_LTEQ(CharsToDataTypes(value[0], value[1], value[2]));
					case TokenType::KW_GTEQ: return Parser::make_TOK_GTEQ(CharsToDataTypes(value[0], value[1], value[2]));
					case TokenType::KW_EQ: return Parser::make_TOK_EQ(CharsToDataTypes(value[0], value[1], value[2]));
					case TokenType::KW_NEQ: return Parser::make_TOK_NEQ(CharsToDataTypes(value[0], value[1], value[2]));
					case TokenType::KW_NEG: return Parser::make_TOK_NEG(CharsToDataTypes(value[0], value[1]));
					case TokenType::KW_LNOT: return Parser::make_TOK_LNOT(CharsToDataTypes(value[0], value[1]));
					case TokenType::KW_BNOT: return Parser::make_TOK_BNOT(CharsToDataTypes(value[0], value[1]));
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
%param { std::map<std::string, int>& labels }
%param { std::map<Instruction*, std::pair<std::string, Position>>& ctrlInstrs }

%token <DataValue> TOK_HEX_LIT "hex"
%token <int> TOK_REG "reg"
%token <int> TOK_MEM_REG "mreg"
%token <int> TOK_STACK_REG "sreg"
%token <int> TOK_SP_OFFSET "spoff"
%token <std::string> TOK_LABEL "LABEL"
%token <int> TOK_INSTR_OFFSET "ipOff"
%token TOK_SP "SP"
%token TOK_HALT "HALT"
%token TOK_MALLOC "MALLOC"
%token TOK_FREE "FREE"
%token TOK_CALL "CALL"
%token TOK_RET "RET"
%token TOK_COMPARE "COMPARE"
%token TOK_PRINT "PRINT"
%token TOK_JUMP "JUMP"
%token TOK_JUMPT "JUMPT"
%token TOK_JUMPF "JUMPF"
%token <DataType> TOK_MOV "MOV"
%token <DataType> TOK_POP "POP"
%token <std::vector<DataType>> TOK_STORE "STORE"
%token <std::vector<DataType>> TOK_PUSH "PUSH"
%token <DataTypeTriple> TOK_ADD "ADD"
%token <DataTypeTriple> TOK_SUB "SUB"
%token <DataTypeTriple> TOK_MUL "MUL"
%token <DataTypeTriple> TOK_DIV "DIV"
%token <DataTypeTriple> TOK_MOD "MOD"
%token <DataTypeTriple> TOK_POW "POW"
%token <DataTypeTriple> TOK_LSHIFT "LSHIFT"
%token <DataTypeTriple> TOK_RSHIFT "RSHIFT"
%token <DataTypeTriple> TOK_BAND "BAND"
%token <DataTypeTriple> TOK_BOR "BOR"
%token <DataTypeTriple> TOK_BXOR "BXOR"
%token <DataTypeTriple> TOK_LAND "LAND"
%token <DataTypeTriple> TOK_LOR "LOR"
%token <DataTypeTriple> TOK_LT "LT"
%token <DataTypeTriple> TOK_GT "GT"
%token <DataTypeTriple> TOK_LTEQ "LTEQ"
%token <DataTypeTriple> TOK_GTEQ "GTEQ"
%token <DataTypeTriple> TOK_EQ "EQ"
%token <DataTypeTriple> TOK_NEQ "NEQ"
%token <DataTypeDouble> TOK_NEG "NEG"
%token <DataTypeDouble> TOK_LNOT "LNOT"
%token <DataTypeDouble> TOK_BNOT "BNOT"
%token TOK_END_OF_FILE 0

%nterm STMTS;
%nterm <std::vector<Argument>> ARGUMENTS;
%nterm <Instruction*> STMT;
%nterm <Argument> ARGUMENT;
%nterm <Argument> DEST_ARG;
%nterm <std::pair<Binop, DataTypeTriple>> BINOP;
%nterm <std::pair<Unop, DataTypeDouble>> UNOP;

%%
%start PROGRAM;
PROGRAM: STMTS;

STMTS:
		%empty      
	|	STMTS STMT		{ result.push_back($2); }
	|	STMTS "LABEL"	{ labels.find($2) == labels.end() ? labels.insert_or_assign($2, result.size()) : throw std::runtime_error("Duplicate Label: " + $2); }
;

STMT:
		"HALT"											{ $$ = new InstrHalt(); }
	|	"RET" ARGUMENT									{ $$ = new InstrReturn($2); }
	|	"MOV" ARGUMENT DEST_ARG							{ $$ = new InstrMove($1, $2, $3); }
	|	"MALLOC" ARGUMENT DEST_ARG						{ $$ = new InstrMalloc($2, $3); }
	|	"FREE" ARGUMENT									{ $$ = new InstrFree($2); }
	|	"PRINT" ARGUMENT ARGUMENT						{ $$ = new InstrPrint($2, $3); }
	|	"JUMP" "LABEL"									{ $$ = new InstrJump(-1); ctrlInstrs.insert_or_assign($$, std::make_pair($2, position)); }
	|	"JUMP" "ipOff"									{ $$ = new InstrJump(result.size() + $2); }
	|	"JUMPT" "LABEL" ARGUMENT						{ $$ = new InstrCJump(-1, $3, false); ctrlInstrs.insert_or_assign($$, std::make_pair($2, position)); }
	|	"JUMPT" "ipOff" ARGUMENT						{ $$ = new InstrCJump(result.size() + $2, $3, false); }
	|	"JUMPF" "LABEL" ARGUMENT						{ $$ = new InstrCJump(-1, $3, true); ctrlInstrs.insert_or_assign($$, std::make_pair($2, position)); }
	|	"JUMPF" "ipOff" ARGUMENT						{ $$ = new InstrCJump(result.size() + $2, $3, true); }
	|	"CALL" "LABEL" "hex" ARGUMENT					{ $$ = new InstrCall(-1, $3.i, $4); ctrlInstrs.insert_or_assign($$, std::make_pair($2, position)); }
	|	"CALL" "hex" "hex" ARGUMENT						{ $$ = new InstrCall(result.size() + $2.i, $3.i, $4); }
	|	"PUSH" ARGUMENTS								{ $$ = new InstrPush(BindArgDataTypes($1, $2)); }
	|	"POP" ARGUMENT									{ $$ = new InstrPop($1, $2); }
	|	"STORE" ARGUMENTS DEST_ARG 						{ $$ = $1.size() != 0 ? new InstrStore(BindArgDataTypes($1, $2), $3) : throw std::runtime_error("'STORE' expects at least one source argument!"); }
	|	"COMPARE" DEST_ARG DEST_ARG ARGUMENT DEST_ARG 	{ $$ = new InstrCompare($2, $3, $4, $5); }
	|	BINOP ARGUMENT ARGUMENT DEST_ARG				{ $$ = new InstrBinop($1.first, TypedArgument(std::get<0>($1.second), $2), TypedArgument(std::get<1>($1.second), $3), TypedArgument(std::get<2>($1.second), $4)); }
	|	UNOP ARGUMENT DEST_ARG							{ $$ = new InstrUnop($1.first, TypedArgument(std::get<0>($1.second), $2), TypedArgument(std::get<1>($1.second), $3)); }
;

ARGUMENTS:
		%empty				{ $$ = { }; }
	|	ARGUMENTS ARGUMENT  { $1.push_back($2); $$ = $1; }
;

ARGUMENT:
		"hex"		{ $$ = Argument(ArgType::VALUE, $1); }
	|	"reg"		{ $$ = Argument(ArgType::REGISTER, $1); }
	|	"mreg"		{ $$ = Argument(ArgType::MEM_REG, $1); }
	|	"sreg"		{ $$ = Argument(ArgType::STACK_REG, $1); }
	|	"SP"		{ $$ = Argument(ArgType::STACK_PTR, 0); }
	|	"spoff"		{ $$ = Argument(ArgType::SP_OFFSET, $1); }
;

DEST_ARG:
		"reg"		{ $$ = Argument(ArgType::REGISTER, $1); }
	|	"mreg"		{ $$ = Argument(ArgType::MEM_REG, $1); }
	|	"sreg"		{ $$ = Argument(ArgType::STACK_REG, $1); }
	|	"spoff"		{ $$ = Argument(ArgType::SP_OFFSET, $1); }
;

BINOP:
		"ADD"		{ $$ = { Binop::ADD, $1 }; }
	|	"SUB"		{ $$ = { Binop::SUB, $1 }; }
	|	"MUL"		{ $$ = { Binop::MUL, $1 }; }
	|	"DIV"		{ $$ = { Binop::DIV, $1 }; }
	|	"MOD"		{ $$ = { Binop::MOD, $1 }; }
	|	"POW"		{ $$ = { Binop::POW, $1 }; }
	|	"LSHIFT"	{ $$ = { Binop::LSHIFT, $1 }; }
	|	"RSHIFT"	{ $$ = { Binop::RSHIFT, $1 }; }
	|	"BAND"		{ $$ = { Binop::BIT_AND, $1 }; }
	|	"BOR"		{ $$ = { Binop::BIT_OR, $1 }; }
	|	"BXOR"		{ $$ = { Binop::BIT_XOR, $1 }; }
	|	"LAND"		{ $$ = { Binop::LOG_AND, $1 }; }
	|	"LOR"		{ $$ = { Binop::LOG_OR, $1 }; }
	|	"LT"		{ $$ = { Binop::LT, $1 }; }
	|	"GT"		{ $$ = { Binop::GT, $1 }; }
	|	"LTEQ"		{ $$ = { Binop::LTEQ, $1 }; }
	|	"GTEQ"		{ $$ = { Binop::GTEQ, $1 }; }
	|	"EQ"		{ $$ = { Binop::EQ, $1 }; }
	|	"NEQ"		{ $$ = { Binop::NEQ, $1 }; }

UNOP:
		"NEG"	{ $$ = std::make_pair(Unop::NEG, $1); }
	|	"LNOT"	{ $$ = std::make_pair(Unop::LOG_NOT, $1); }
	|	"BNOT"	{ $$ = std::make_pair(Unop::BIN_NOT, $1); }
;

%%
namespace ramvm {
	namespace bison {
		// Report an error to the user.
		auto Parser::error(const std::string& _msg) -> void
		{
			throw std::runtime_error(_msg);
		}

		std::vector<TypedArgument> BindArgDataTypes(std::vector<DataType>& _dataTypes, std::vector<Argument>& _args)
		{
			if(_dataTypes.size() != _args.size()) { throw std::runtime_error("Instruction expects " + std::to_string(_dataTypes.size()) + " sources!"); }
			else
			{
				std::vector<TypedArgument> result;
				result.reserve(_dataTypes.size());

				for(int i = 0; i < (int)_dataTypes.size(); i++)
					result.push_back(TypedArgument(_dataTypes[i], _args[i]));

				return result;
			}
		}
	}
}

