%skeleton "lalr1.cc"
%require "3.5"
%language "c++"

%defines
%define api.value.type variant
%define api.token.constructor
%define api.namespace {ramc::bison}
%define api.parser.class {Parser}

%code top {
  #include "pch.h"
  #include "ramc_lexer.h"
}

%code requires {
  #include "ramc_ast.h"
}

%code {
    namespace ramc {
      namespace bison {
      // Return the next token.
      auto yylex(Lexer& _lexer, ASTNode*& _result, Position& _pos) -> Parser::symbol_type
      {
        LexerResult readRes = _lexer.GetNextToken(true, true);

        if (!readRes.IsSuccess())
          throw std::runtime_error(readRes.ToString());

        Token token = readRes.GetToken();
        _pos = token.position;

        switch (token.type)
        {
          case TokenType::INT_LIT: {
            ASTIntLit const* node = new ASTIntLit(std::stoi(token.value), token.position);
            return Parser::make_INT_LIT(node);
          }
          case TokenType::IDENTIFIER: {
            ASTIdentifier const* node = new ASTIdentifier(token.value, token.position);
            return Parser::make_IDENTIFIER(node);
          }
          case TokenType::UNDERSCORE: return Parser::make_UNDERSCORE();
          case TokenType::STRING_LIT: {
            ASTStringLit const* node = new ASTStringLit(token.value, token.position);
            return Parser::make_STRING_LIT(node);
          }
          case TokenType::FLOAT_LIT: {
            ASTFloatLit const* node = new ASTFloatLit(std::stof(token.value), token.position);
            return Parser::make_FLOAT_LIT(node);
          }
          case TokenType::KW_TRUE: {
            ASTBoolLit const* node = new ASTBoolLit(true, token.position);
            return Parser::make_KW_TRUE(node);
          }
          case TokenType::KW_FALSE: {
            ASTBoolLit const* node = new ASTBoolLit(false, token.position);
            return Parser::make_KW_FALSE(node);
          }
          case TokenType::PLUS: return Parser::make_PLUS();
          case TokenType::MINUS: return Parser::make_MINUS();
          case TokenType::TIMES: return Parser::make_TIMES();
          case TokenType::DIVIDE: return Parser::make_DIVIDE();
          case TokenType::MOD: return Parser::make_MOD();
          case TokenType::POW: return Parser::make_POW();
          case TokenType::BIN_AND: return Parser::make_BIN_AND();
          case TokenType::BIN_OR: return Parser::make_BIN_OR();
          case TokenType::BIN_XOR: return Parser::make_BIN_XOR();
          case TokenType::LSHIFT: return Parser::make_LSHIFT();
          case TokenType::RSHIFT: return Parser::make_RSHIFT();
          case TokenType::LT: return Parser::make_LT();
          case TokenType::GT: return Parser::make_GT();
          case TokenType::LT_EQ: return Parser::make_LT_EQ();
          case TokenType::GT_EQ: return Parser::make_GT_EQ();
          case TokenType::EQ_EQ: return Parser::make_EQ_EQ();
          case TokenType::NEQ: return Parser::make_NEQ();
          case TokenType::LOG_AND: return Parser::make_LOG_AND();
          case TokenType::LOG_OR: return Parser::make_LOG_OR();
          case TokenType::LOG_NOT: return Parser::make_LOG_NOT();
          case TokenType::EQ: return Parser::make_EQ();
          case TokenType::PLUS_EQ: return Parser::make_ADD_EQ();
          case TokenType::MINUS_EQ: return Parser::make_SUB_EQ();
          case TokenType::TIMES_EQ: return Parser::make_MUL_EQ();
          case TokenType::DIVIDE_EQ: return Parser::make_DIV_EQ();
          case TokenType::MOD_EQ: return Parser::make_MOD_EQ();
          case TokenType::POW_EQ: return Parser::make_POW_EQ();
          case TokenType::BIN_AND_EQ: return Parser::make_BIN_AND_EQ();
          case TokenType::BIN_OR_EQ: return Parser::make_BIN_OR_EQ();
          case TokenType::BIN_XOR_EQ: return Parser::make_BIN_XOR_EQ();
          case TokenType::LSHIFT_EQ: return Parser::make_LSHIFT_EQ();
          case TokenType::RSHIFT_EQ: return Parser::make_RSHIFT_EQ();
          case TokenType::INC: return Parser::make_INC();
          case TokenType::DEC: return Parser::make_DEC();
          case TokenType::LPAREN: return Parser::make_LPAREN();
          case TokenType::RPAREN: return Parser::make_RPAREN();
          case TokenType::LCBRACKET: return Parser::make_LCBRACKET();
          case TokenType::RCBRACKET: return Parser::make_RCBRACKET();
          case TokenType::LSBRACKET: return Parser::make_LSBRACKET();
          case TokenType::RSBRACKET: return Parser::make_RSBRACKET();
          case TokenType::SEMICOLON: return Parser::make_SEMICOLON();
          case TokenType::PERIOD: return Parser::make_PERIOD();
          case TokenType::COLON: return Parser::make_COLON();
          case TokenType::COMMA: return Parser::make_COMMA();
          case TokenType::END_OF_FILE: return Parser::make_END_OF_FILE();
          default: throw std::runtime_error(token.ToString(true) + " is not parasble!");
        }
      }
    }
  }
}

%param { Lexer& lexer }
%param { ASTNode*& result }
%param { Position& position }

%token PLUS "+"
%token MINUS "-"
%token TIMES "*"
%token DIVIDE "/"
%token MOD "%"
%token POW "**"
%token BIN_AND "&"
%token BIN_OR "|"
%token BIN_XOR "^"
%token LSHIFT "<<"
%token RSHIFT ">>"
%token LT "<"
%token GT ">"
%token LT_EQ "<="
%token GT_EQ ">="
%token EQ_EQ "=="
%token NEQ "!="
%token LOG_AND "&&"
%token LOG_OR "||"
%token LOG_NOT "!"
%token EQ "="
%token ADD_EQ "+="
%token SUB_EQ "-="
%token MUL_EQ "*="
%token DIV_EQ "/="
%token MOD_EQ "%="
%token POW_EQ "**="
%token BIN_AND_EQ "&="
%token BIN_OR_EQ "|="
%token BIN_XOR_EQ "^="
%token LSHIFT_EQ "<<="
%token RSHIFT_EQ ">>="
%token LPAREN "("
%token RPAREN ")"
%token LCBRACKET "}"
%token RCBRACKET "{"
%token LSBRACKET "["
%token RSBRACKET "]"
%token INC "++"
%token DEC "--"
%token SEMICOLON ";"
%token PERIOD "."
%token COLON ":"
%token COMMA ","
%token UNDERSCORE "_"
%token <ASTIdentifier*> IDENTIFIER "ID"
%token <ASTIntLit*> INT_LIT "INT_LIT"
%token <ASTStringLit*> STRING_LIT "STRING_LIT"
%token <ASTFloatLit*> FLOAT_LIT "FLOAT_LIT"
%token <ASTBoolLit*> KW_TRUE "true"
%token <ASTBoolLit*> KW_FALSE "false"
%token END_OF_FILE 0

%nterm <ASTNode*> PROGRAM;
%nterm <ASTNode*> STMT;
%nterm <ASTNode*> ASSIGNMENT;
%nterm <ASTNode*> EXPR1;
%nterm <ASTNode*> EXPR2;
%nterm <ASTNode*> EXPR3;
%nterm <ASTNode*> EXPR4;
%nterm <ASTNode*> EXPR5;
%nterm <ASTNode*> EXPR6;
%nterm <ASTNode*> EXPR7;
%nterm <ASTNode*> EXPR8;
%nterm <ASTNode*> EXPR9;
%nterm <ASTNode*> EXPR10;
%nterm <ASTNode*> EXPR11;
%nterm <ASTNode*> EXPR12;
%nterm <ASTNode*> EXPR13;
%nterm <ASTNode*> EXPR14;
%nterm <ASTNode*> ATOM;

%nterm <std::vector<ASTNode*>> STMTS;
%nterm <BinopType> OP_ASSIGN;

%%
%start PROGRAM;
PROGRAM: 
    STMTS   { result = new ASTProgram("Test File", $1); }
;

STMTS:
    %empty      { $$ = { }; }
  | STMTS STMT  { $1.push_back($2); $$ = $1; }
;

STMT:
    ASSIGNMENT ";" { $$ = $1; }
  | EXPR1 ";"      { $$ = $1; }
;

ASSIGNMENT:
    "ID" OP_ASSIGN EXPR1    { $$ = new ASTBinopExpr($1, $3, $2); }
  | "_" "=" EXPR1           { $$ = $3; }
;

OP_ASSIGN:
    "="     { $$ = BinopType::EQ; }
  | "+="    { $$ = BinopType::ADD_EQ; }
  | "-="    { $$ = BinopType::SUB_EQ; }
  | "*="    { $$ = BinopType::MUL_EQ; }
  | "/="    { $$ = BinopType::DIV_EQ; }
  | "%="    { $$ = BinopType::MOD_EQ; }
  | "**="   { $$ = BinopType::POW_EQ; }
  | "&="    { $$ = BinopType::BIN_AND_EQ; }
  | "|="    { $$ = BinopType::BIN_OR_EQ; }
  | "^="    { $$ = BinopType::BIN_XOR_EQ; }
  | "<<="   { $$ = BinopType::LSHIFT_EQ; }
  | ">>="   { $$ = BinopType::RSHIFT_EQ; }

EXPR1:
    EXPR1 "||" EXPR2  { $$ = new ASTBinopExpr($1, $3, BinopType::LOG_OR); }
  | EXPR2             { $$ = $1; }
;

EXPR2:
    EXPR2 "&&" EXPR3  { $$ = new ASTBinopExpr($1, $3, BinopType::LOG_AND); }
  | EXPR3             { $$ = $1; }
;

EXPR3:
    EXPR3 "|" EXPR4   { $$ = new ASTBinopExpr($1, $3, BinopType::BIN_OR); }
  | EXPR4             { $$ = $1; }
;

EXPR4:
    EXPR4 "^" EXPR5   { $$ = new ASTBinopExpr($1, $3, BinopType::BIN_XOR); }
  | EXPR5             { $$ = $1; }
;

EXPR5:
    EXPR5 "&" EXPR6   { $$ = new ASTBinopExpr($1, $3, BinopType::BIN_AND); }
  | EXPR6             { $$ = $1; }
;

EXPR6:
    EXPR6 "==" EXPR7   { $$ = new ASTBinopExpr($1, $3, BinopType::EQ_EQ); }
  | EXPR6 "!=" EXPR7   { $$ = new ASTBinopExpr($1, $3, BinopType::NEQ); }
  | EXPR7              { $$ = $1; }
;

EXPR7:
    EXPR7 "<" EXPR8     { $$ = new ASTBinopExpr($1, $3, BinopType::LT); }
  | EXPR7 ">" EXPR8     { $$ = new ASTBinopExpr($1, $3, BinopType::GT); }
  | EXPR7 "<=" EXPR8    { $$ = new ASTBinopExpr($1, $3, BinopType::LT_EQ); }
  | EXPR7 ">=" EXPR8    { $$ = new ASTBinopExpr($1, $3, BinopType::GT_EQ); }
  | EXPR8               { $$ = $1; }
;

EXPR8:
    EXPR8 "<<" EXPR9   { $$ = new ASTBinopExpr($1, $3, BinopType::LSHIFT); }
  | EXPR8 ">>" EXPR9   { $$ = new ASTBinopExpr($1, $3, BinopType::RSHIFT); }
  | EXPR9              { $$ = $1; }
;

EXPR9:
    EXPR9 "+" EXPR10   { $$ = new ASTBinopExpr($1, $3, BinopType::ADD); }
  | EXPR9 "-" EXPR10   { $$ = new ASTBinopExpr($1, $3, BinopType::SUB); }
  | EXPR10             { $$ = $1; }
;

EXPR10:
    EXPR10 "*" EXPR11   { $$ = new ASTBinopExpr($1, $3, BinopType::MUL); }
  | EXPR10 "/" EXPR11   { $$ = new ASTBinopExpr($1, $3, BinopType::DIV); }
  | EXPR10 "%" EXPR11   { $$ = new ASTBinopExpr($1, $3, BinopType::MOD); }
  | EXPR11              { $$ = $1; }
;

EXPR11:
    "-" EXPR11   { $$ = new ASTUnopExpr($2, UnopType::NEG); }
  | "!" EXPR11   { $$ = new ASTUnopExpr($2, UnopType::LOG_NOT); }
  | EXPR12       { $$ = $1; }
;

EXPR12:
    EXPR12 "**" EXPR13   { $$ = new ASTBinopExpr($1, $3, BinopType::POW); }
  | EXPR13               { $$ = $1; }
;

EXPR13:
    "++" EXPR13   { $$ = new ASTUnopExpr($2, UnopType::PRE_INC); }
  | "--" EXPR13   { $$ = new ASTUnopExpr($2, UnopType::PRE_DEC); }
  | EXPR14        { $$ = $1; }
;

EXPR14:
    EXPR14 "++"   { $$ = new ASTUnopExpr($1, UnopType::POST_INC); }
  | EXPR14 "--"   { $$ = new ASTUnopExpr($1, UnopType::POST_DEC); }
  | ATOM          { $$ = $1; }
;

ATOM:
    "INT_LIT"       { $$ = $1; }
  | "FLOAT_LIT"     { $$ = $1; }
  | "STRING_LIT"    { $$ = $1; }
  | "true"          { $$ = $1; }
  | "false"         { $$ = $1; }
  | "ID"            { $$ = $1; }
  | "(" EXPR1 ")"   { $$ = $2; }
;

%%
namespace ramc {
	namespace bison {
		// Report an error to the user.
		auto Parser::error(const std::string& _msg) -> void
		{
			throw std::runtime_error(_msg);
		}
	}
}

