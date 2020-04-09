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
}

%code {
    namespace ramvm {
		namespace bison {
			// Return the next token.
			auto yylex(Lexer& _lexer,
						std::vector<Instruction*>& _result,
						Position& _pos,
						std::unordered_map<std::string, int>& _labels) -> Parser::symbol_type
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
                    case TokenType::BYTE_LIT: return Parser::make_TOK_BYTE_LIT((byte)std::stoi(value));
                    case TokenType::INT_LIT: return Parser::make_TOK_INT_LIT(std::stoi(value));
                    case TokenType::FLOAT_LIT: return Parser::make_TOK_FLOAT_LIT(std::stof(value));
                    case TokenType::DOUBLE_LIT: return Parser::make_TOK_DOUBLE_LIT(std::stod(value));
                    case TokenType::LONG_LIT: return Parser::make_TOK_LONG_LIT(std::stoll(value));

                    case TokenType::LSBRACKET: return Parser::make_TOK_LSBRACKET();
                    case TokenType::RSBRACKET: return Parser::make_TOK_RSBRACKET();
                    case TokenType::LCBRACKET: return Parser::make_TOK_LCBRACKET();
                    case TokenType::RCBRACKET: return Parser::make_TOK_RCBRACKET();
                    case TokenType::LPAREN: return Parser::make_TOK_LPAREN();
                    case TokenType::RPAREN: return Parser::make_TOK_RPAREN();
                    case TokenType::SEMICOLON: return Parser::make_TOK_SEMICOLON();
                    case TokenType::COMMA: return Parser::make_TOK_COMMA();
                    case TokenType::PLUS: return Parser::make_TOK_PLUS();
                    case TokenType::MINUS: return Parser::make_TOK_MINUS();
                    
                    case TokenType::SINGLE_TYPE: return Parser::make_TOK_SINGLE_TYPE(CharToDataType(value[0]));
                    case TokenType::DOUBLE_TYPE: return Parser::make_TOK_DOUBLE_TYPE(CharsToDataTypes(value[0], value[1]));
                    case TokenType::TRIPLE_TYPE: return Parser::make_TOK_TRIPLE_TYPE(CharsToDataTypes(value[0], value[1], value[2]));

					case TokenType::LABEL: return Parser::make_TOK_LABEL(value);
					
					case TokenType::REG_SP: return Parser::make_TOK_REG_SP();
					case TokenType::REG_FP: return Parser::make_TOK_REG_FP();
					case TokenType::REG_GP: return Parser::make_TOK_REG_GP();

					case TokenType::KW_HALT: return Parser::make_TOK_HALT();
					case TokenType::KW_COMPARE: return Parser::make_TOK_COMPARE();
					case TokenType::KW_MOV: return Parser::make_TOK_MOV();
					case TokenType::KW_RET: return Parser::make_TOK_RET();
					case TokenType::KW_MALLOC: return Parser::make_TOK_MALLOC();
					case TokenType::KW_FREE: return Parser::make_TOK_FREE();
					case TokenType::KW_PUSH: return Parser::make_TOK_PUSH();
					case TokenType::KW_POP: return Parser::make_TOK_POP();
					case TokenType::KW_PRINT: return Parser::make_TOK_PRINT();
					case TokenType::KW_JUMP: return Parser::make_TOK_JUMP();
					case TokenType::KW_JUMPT: return Parser::make_TOK_JUMPT();
					case TokenType::KW_JUMPF: return Parser::make_TOK_JUMPF();
					case TokenType::KW_CALL: return Parser::make_TOK_CALL();
					case TokenType::KW_STORE: return Parser::make_TOK_STORE();

					case TokenType::KW_ADD: return Parser::make_TOK_ADD();
					case TokenType::KW_SUB: return Parser::make_TOK_SUB();
					case TokenType::KW_MUL: return Parser::make_TOK_MUL();
					case TokenType::KW_DIV: return Parser::make_TOK_DIV();
					case TokenType::KW_MOD: return Parser::make_TOK_MOD();
					case TokenType::KW_POW: return Parser::make_TOK_POW();
					case TokenType::KW_LSHIFT: return Parser::make_TOK_LSHIFT();
					case TokenType::KW_RSHIFT: return Parser::make_TOK_RSHIFT();
					case TokenType::KW_AND: return Parser::make_TOK_AND();
					case TokenType::KW_OR: return Parser::make_TOK_OR();
					case TokenType::KW_XOR: return Parser::make_TOK_XOR();
					case TokenType::KW_LT: return Parser::make_TOK_LT();
					case TokenType::KW_GT: return Parser::make_TOK_GT();
					case TokenType::KW_LTEQ: return Parser::make_TOK_LTEQ();
					case TokenType::KW_GTEQ: return Parser::make_TOK_GTEQ();
					case TokenType::KW_EQ: return Parser::make_TOK_EQ();
					case TokenType::KW_NEQ: return Parser::make_TOK_NEQ();
					case TokenType::KW_NEG: return Parser::make_TOK_NEG();
					case TokenType::KW_NOT: return Parser::make_TOK_NOT();

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
%param { std::unordered_map<std::string, int>& labels }

%token <DataValue> TOK_HEX_LIT "hex"
%token <byte> TOK_BYTE_LIT "byte"
%token <int> TOK_INT_LIT "int"
%token <float> TOK_FLOAT_LIT "float"
%token <double> TOK_DOUBLE_LIT "double"
%token <rLong> TOK_LONG_LIT "long"

%token TOK_LSBRACKET "["
%token TOK_RSBRACKET "]"
%token TOK_LCBRACKET "{"
%token TOK_RCBRACKET "}"
%token TOK_LPAREN "("
%token TOK_RPAREN ")"
%token TOK_SEMICOLON ";"
%token TOK_COMMA ","
%token TOK_PLUS "+"
%token TOK_MINUS "-"

%token <DataType> TOK_SINGLE_TYPE "<T>"
%token <DataTypeDouble> TOK_DOUBLE_TYPE "<TT>"
%token <DataTypeTriple> TOK_TRIPLE_TYPE "<TTT>"

%token <std::string> TOK_LABEL "LABEL"

%token TOK_REG_SP "$SP"
%token TOK_REG_FP "$FP"
%token TOK_REG_GP "$GP"

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
%token TOK_MOV "MOV"
%token TOK_POP "POP"
%token TOK_STORE "STORE"
%token TOK_PUSH "PUSH"

%token TOK_ADD "ADD"
%token TOK_SUB "SUB"
%token TOK_MUL "MUL"
%token TOK_DIV "DIV"
%token TOK_MOD "MOD"
%token TOK_POW "POW"
%token TOK_LSHIFT "LSHIFT"
%token TOK_RSHIFT "RSHIFT"
%token TOK_AND "AND"
%token TOK_OR "OR"
%token TOK_XOR "XOR"
%token TOK_LT "LT"
%token TOK_GT "GT"
%token TOK_LTEQ "LTEQ"
%token TOK_GTEQ "GTEQ"
%token TOK_EQ "EQ"
%token TOK_NEQ "NEQ"
%token TOK_NEG "NEG"
%token TOK_NOT "NOT"

%token TOK_END_OF_FILE 0

%nterm STMTS;
%nterm <std::vector<Argument*>> ARGUMENTS;
%nterm <Instruction*> STMT;
%nterm <Argument*> ARGUMENT;
%nterm <Binop> BINOP;
%nterm <Unop> UNOP;

%%
%start PROGRAM;
PROGRAM: STMTS;

STMTS:
		%empty
	|	STMTS STMT ";" 				{ result.push_back($2); }
	|	STMTS "LABEL" ";"			{ ASSERT_MSG(labels.find($2) == labels.end(), "Duplicate Label: " + $2); labels.insert_or_assign($2, result.size()); }
;

STMT:
		"HALT"																{ $$ = new InstrHalt(); }
	|	"RET" "(" ARGUMENT ")"												{ $$ = new InstrReturn($3); }
	|	"MOV" "<T>" "(" ARGUMENT "," ARGUMENT ")"							{ $$ = new InstrMove($2, $4, $6); }
	|	"MALLOC" "(" ARGUMENT "," ARGUMENT ")"								{ $$ = new InstrMalloc($3, $5); }
	|	"FREE" "(" ARGUMENT	")"												{ $$ = new InstrFree($3); }
	|	"PRINT" "(" ARGUMENT  "," ARGUMENT ")"								{ $$ = new InstrPrint($3, $5); }
	|	"JUMP" "(" "LABEL" ")"												{ $$ = new InstrJump($3); }
	|	"JUMPT" "(" "LABEL" "," ARGUMENT ")"								{ $$ = new InstrCJump($3, $5, false); }
	|	"JUMPF" "(" "LABEL" "," ARGUMENT ")"								{ $$ = new InstrCJump($3, $5, true); }
	|	"CALL" "(" "LABEL" "," ARGUMENT ")"									{ $$ = new InstrCall($3, $5); }
	|	"PUSH" "<T>" "(" ARGUMENTS ")"										{ $$ = new InstrPush($2, $4); }
	|	"POP" "<T>" "(" ARGUMENT ")"										{ $$ = new InstrPop($2, $4); }
	|	"STORE" "<T>" "(" ARGUMENTS ")"		 								{ $$ = new InstrStore($2, std::vector<Argument*>($4.begin(), $4.begin() + ($4.size() - 1)), $4.back()); }
	|	"COMPARE" "(" ARGUMENT "," ARGUMENT "," ARGUMENT "," ARGUMENT ")"	{ $$ = new InstrCompare($3, $5, $7, $9); }
	|	BINOP "<TTT>" "(" ARGUMENT "," ARGUMENT "," ARGUMENT ")"			{ $$ = new InstrBinop($1, $2, $4, $6, $8); }
	|	UNOP "<TT>" "(" ARGUMENT "," ARGUMENT ")"							{ $$ = new InstrUnop($1, $2, $4, $6); }
;

ARGUMENTS:
		ARGUMENT					{ $$ = { $1 }; }
	|	ARGUMENTS "," ARGUMENT 		{ $1.push_back($3); $$ = $1; }
;

ARGUMENT:
		"hex"						{ $$ = new ValueArgument($1); }
	|	"byte"						{ $$ = new ValueArgument($1); }
	|	"int"						{ $$ = new ValueArgument($1); }
	|	"float"						{ $$ = new ValueArgument($1); }
	|	"double"					{ $$ = new ValueArgument($1); }
	|	"long"						{ $$ = new ValueArgument($1); }
	|	"$SP"						{ $$ = new RegisterArgument(RegisterType::SP); }
	|	"$FP"						{ $$ = new RegisterArgument(RegisterType::FP); }
	|	"$GP"						{ $$ = new RegisterArgument(RegisterType::GP); }
	|	"[" ARGUMENT "]"			{ $$ = new StackArgument($2, 0); }
	|	"[" ARGUMENT "+" "int" "]"	{ $$ = new StackArgument($2, $4); }
	|	"[" ARGUMENT "-" "int" "]"	{ $$ = new StackArgument($2, -$4); }
	|	"{" ARGUMENT "}"			{ $$ = new MemoryArgument($2, 0); }
	|	"{" ARGUMENT "+" "int" "}"	{ $$ = new MemoryArgument($2, $4); }
	|	"{" ARGUMENT "-" "int" "}"	{ $$ = new MemoryArgument($2, -$4); }
;

BINOP:
		"ADD"	 { $$ = Binop::ADD; }
	|	"SUB"	 { $$ = Binop::SUB; }
	|	"MUL"	 { $$ = Binop::MUL; }
	|	"DIV"	 { $$ = Binop::DIV; }
	|	"MOD"	 { $$ = Binop::MOD; }
	|	"POW"	 { $$ = Binop::POW; }
	|	"LSHIFT" { $$ = Binop::LSHIFT; }
	|	"RSHIFT" { $$ = Binop::RSHIFT; }
	|	"AND"	 { $$ = Binop::AND; }
	|	"OR"	 { $$ = Binop::OR; }
	|	"XOR"	 { $$ = Binop::XOR; }
	|	"LT"	 { $$ = Binop::LT; }
	|	"GT"	 { $$ = Binop::GT; }
	|	"LTEQ"	 { $$ = Binop::LTEQ; }
	|	"GTEQ"	 { $$ = Binop::GTEQ; }
	|	"EQ"	 { $$ = Binop::EQ; }
	|	"NEQ"	 { $$ = Binop::NEQ; }

UNOP:
		"NEG" { $$ = Unop::NEG; }
	|	"NOT" { $$ = Unop::NOT; }
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

