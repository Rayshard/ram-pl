%skeleton "lalr1.cc"
%require "3.5"
%language "c++"

%defines
%define api.value.type variant
%define api.token.constructor
%define api.namespace {ramc::bison}
%define api.parser.class {Parser}
%define parse.lac
%define parse.error verbose

%code top {
  #include "pch.h"
  #include "ramc_lexer.h"
}

%code requires {
  #include "ramc_ast.h"
  #include "ramc_typesystem.h"
}

%code {
    namespace ramc {
      namespace bison {
		bool IsInLoop, InTopLvl;

      // Return the next token.
		auto yylex(Lexer& _lexer, ASTNode*& _result, Position& _pos) -> Parser::symbol_type
		{
			LexerResult readRes = _lexer.GetNextToken();

			if (!readRes.IsSuccess())
				ASSERT_MSG(false, readRes.ToString(false));

			Token token = readRes.GetValue();
			_pos = token.position;

			switch (token.type)
			{
				case TokenType::INT_LIT: { ASTIntLit const* node = new ASTIntLit(std::stoi(token.value), token.position); return Parser::make_INT_LIT(node); }
				case TokenType::IDENTIFIER: { return Parser::make_ID(std::make_pair(token.value, token.position)); }
				case TokenType::UNDERSCORE: return Parser::make_UNDERSCORE();
				case TokenType::STRING_LIT: { ASTStringLit const* node = new ASTStringLit(token.value, token.position); return Parser::make_STRING_LIT(node); }
				case TokenType::FLOAT_LIT: { ASTFloatLit const* node = new ASTFloatLit(std::stof(token.value), token.position); return Parser::make_FLOAT_LIT(node); }
				case TokenType::DOUBLE_LIT: { ASTDoubleLit const* node = new ASTDoubleLit(std::stod(token.value), token.position); return Parser::make_DOUBLE_LIT(node); }
				case TokenType::BYTE_LIT: { ASTByteLit const* node = new ASTByteLit((byte)std::stoll(token.value), token.position); return Parser::make_BYTE_LIT(node); }
				case TokenType::LONG_LIT: { ASTLongLit const* node = new ASTLongLit(std::stoll(token.value), token.position); return Parser::make_LONG_LIT(node); }

				case TokenType::KW_TRUE: { ASTBoolLit const* node = new ASTBoolLit(true, token.position); return Parser::make_KW_TRUE(node); }
				case TokenType::KW_FALSE: { ASTBoolLit const* node = new ASTBoolLit(false, token.position); return Parser::make_KW_FALSE(node); }

				case TokenType::KW_BYTE: { const Type* type = Type::BYTE(); return Parser::make_KW_BYTE(type); }
				case TokenType::KW_BOOL: { const Type* type = Type::BOOL(); return Parser::make_KW_BOOL(type); }
				case TokenType::KW_INT: { const Type* type = Type::INT(); return Parser::make_KW_INT(type); }
				case TokenType::KW_FLOAT: { const Type* type = Type::FLOAT(); return Parser::make_KW_FLOAT(type); }
				case TokenType::KW_DOUBLE: {const  Type* type = Type::DOUBLE(); return Parser::make_KW_DOUBLE(type); }
				case TokenType::KW_LONG: { const Type* type = Type::LONG(); return Parser::make_KW_LONG(type); }
				case TokenType::KW_STRING: { const Type* type = Type::STRING(); return Parser::make_KW_STRING(type); }

				case TokenType::KW_LET: return Parser::make_KW_LET(token.position);
				case TokenType::KW_FUNC: return Parser::make_KW_FUNC(token.position);
				case TokenType::KW_IF: return Parser::make_KW_IF(token.position);
				case TokenType::KW_THEN: return Parser::make_KW_THEN(token.position);
				case TokenType::KW_ELSE: return Parser::make_KW_ELSE(token.position);
				case TokenType::KW_WHILE: return Parser::make_KW_WHILE(token.position);
				case TokenType::KW_FOR: return Parser::make_KW_FOR(token.position);
				case TokenType::KW_DO: return Parser::make_KW_DO(token.position);
				case TokenType::KW_CONTINUE: return Parser::make_KW_CONTINUE(token.position);
				case TokenType::KW_BREAK: return Parser::make_KW_BREAK(token.position);
				case TokenType::KW_RETURN: return Parser::make_KW_RETURN(token.position);

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
				case TokenType::BIN_NOT: return Parser::make_BIN_NOT();
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
				case TokenType::LPAREN: return Parser::make_LPAREN();
				case TokenType::RPAREN: return Parser::make_RPAREN();
				case TokenType::LCBRACKET: return Parser::make_LCBRACKET(token.position);
				case TokenType::RCBRACKET: return Parser::make_RCBRACKET();
				case TokenType::LSBRACKET: return Parser::make_LSBRACKET(token.position);
				case TokenType::RSBRACKET: return Parser::make_RSBRACKET();
				case TokenType::SEMICOLON: return Parser::make_SEMICOLON();
				case TokenType::PERIOD: return Parser::make_PERIOD();
				case TokenType::COLON: return Parser::make_COLON();
				case TokenType::COMMA: return Parser::make_COMMA();
				case TokenType::GOES_TO: return Parser::make_GOES_TO();
				case TokenType::END_OF_FILE: return Parser::make_END_OF_FILE();
				default: ASSERT_MSG(false, token.ToString(true) + " is not parasble!");
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
%token BIN_NOT "~"
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
%token <Position> LCBRACKET "{"
%token RCBRACKET "}"
%token <Position> LSBRACKET "["
%token RSBRACKET "]"
%token GOES_TO "->"
%token SEMICOLON ";"
%token PERIOD "."
%token COLON ":"
%token COMMA ","
%token UNDERSCORE "_"
%token <std::pair<std::string, Position>> ID "ID"
%token <ASTIntLit*> INT_LIT "INT_LIT"
%token <ASTStringLit*> STRING_LIT "STRING_LIT"
%token <ASTFloatLit*> FLOAT_LIT "FLOAT_LIT"
%token <ASTByteLit*> BYTE_LIT "BYTE_LIT"
%token <ASTDoubleLit*> DOUBLE_LIT "DOUBLE_LIT"
%token <ASTLongLit*> LONG_LIT "LONG_LIT"
%token <ASTBoolLit*> KW_TRUE "true"
%token <ASTBoolLit*> KW_FALSE "false"
%token <Type*> KW_BYTE "byte"
%token <Type*> KW_BOOL "bool"
%token <Type*> KW_INT "int"
%token <Type*> KW_FLOAT "float"
%token <Type*> KW_DOUBLE "double"
%token <Type*> KW_LONG "long"
%token <Type*> KW_STRING "string"
%token <Position> KW_LET "let"
%token <Position> KW_IF "if"
%token <Position> KW_THEN "then"
%token <Position> KW_ELSE "else"
%token <Position> KW_WHILE "while"
%token <Position> KW_FOR "for"
%token <Position> KW_DO "do"
%token <Position> KW_FUNC "func"
%token <Position> KW_BREAK "break"
%token <Position> KW_CONTINUE "continue"
%token <Position> KW_RETURN "return"
%token END_OF_FILE 0

%nterm <ASTProgram*> PROGRAM;
%nterm <ASTStmt*> STMT;
%nterm <ASTStmt*> OPEN_STMT;
%nterm <ASTStmt*> CLOSED_STMT;
%nterm <ASTAssignment*> ASSIGNMENT;
%nterm <ASTVarDecl*> VARDECL;
%nterm <ASTBlock*> BLOCK;
%nterm <ASTFuncDecl*> FUNCDECL;
%nterm <ASTStmt*> WHILE_STMT;
%nterm <ASTStmt*> FOR_STMT;
%nterm <ASTExpr*> EXPR;
%nterm <ASTExpr*> EXPR1;
%nterm <ASTExpr*> EXPR2;
%nterm <ASTExpr*> EXPR3;
%nterm <ASTExpr*> EXPR4;
%nterm <ASTExpr*> EXPR5;
%nterm <ASTExpr*> EXPR6;
%nterm <ASTExpr*> EXPR7;
%nterm <ASTExpr*> EXPR8;
%nterm <ASTExpr*> EXPR9;
%nterm <ASTExpr*> EXPR10;
%nterm <ASTExpr*> EXPR11;
%nterm <ASTExpr*> EXPR12;
%nterm <ASTExpr*> ATOM;
%nterm <ASTIdentifier*> IDENTIFIER;
%nterm <ASTFuncCallExpr*> FUNC_CALL;
%nterm <ASTArrayInit*> ARRAY_INIT;

%nterm <std::vector<ASTVarDecl*>> VARDECLS;
%nterm <std::vector<ASTFuncDecl*>> TL_FUNCDECLS;
%nterm <std::vector<ASTStmt*>> STMTS;
%nterm <std::vector<ASTExpr*>> EXPR_STAR;
%nterm <std::vector<ASTExpr*>> EXPR_PLUS;
%nterm <AssignmentType> OP_ASSIGN;
%nterm <std::vector<Type*>> TYPE_PLUS;
%nterm <Type*> TYPE;
%nterm <std::vector<Param>> PARAM_STAR;
%nterm <std::vector<Param>> PARAM_PLUS;
%nterm <Param> PARAM;

%%
%start PROGRAM;
PROGRAM: { InTopLvl = true; } VARDECLS { InTopLvl = false; } TL_FUNCDECLS { result = new ASTProgram("Test File", $2, $4); };

VARDECLS: %empty			   { $$ = { }; }
		| VARDECL ";" VARDECLS { $3.insert($3.begin(), $1); $$ = $3; }
;

TL_FUNCDECLS: %empty				{ $$ = { }; }
		    | FUNCDECL TL_FUNCDECLS	{ $2.insert($2.begin(), $1); $$ = $2; }
;

FUNCDECL: "func" "ID" ":" "(" PARAM_STAR ")" "->" TYPE_PLUS "=" BLOCK { $$ = new ASTFuncDecl($2.first, $5, $8, $10, $1); }
		| "func" "ID" ":" "(" PARAM_STAR ")" "=" BLOCK				  { $$ = new ASTFuncDecl($2.first, $5, { }, $8, $1); }

PARAM_STAR: %empty	   { $$ = { }; }
		  | PARAM_PLUS { $$ = $1; }
;

PARAM_PLUS: PARAM			     { $$ = { $1 }; }
		  | PARAM "," PARAM_PLUS { $3.insert($3.begin(), $1); $$ = $3; }
;

PARAM: "ID" ":" TYPE { $$ = { $1.first, $3, $1.second }; }

STMTS: %empty		  { $$ = { }; }
	 | STMT ";" STMTS { $3.insert($3.begin(), $1); $$ = $3; }
;

STMT: OPEN_STMT   { $$ = $1; }
    | CLOSED_STMT { $$ = $1; }   
;

OPEN_STMT: "if" EXPR1 "then" STMT					        { $$ = new ASTIfStmt($2, $4, nullptr, $1); }
		 | "if" EXPR1 "then" CLOSED_STMT "else" OPEN_STMT	{ $$ = new ASTIfStmt($2, $4, $6, $1); }
		 | WHILE_STMT										{ $$ = $1; }
;

CLOSED_STMT: ASSIGNMENT											{ $$ = $1; }
		   | VARDECL											{ $$ = $1; }
		   | BLOCK												{ $$ = $1; }
		   | "if" EXPR1 "then" CLOSED_STMT "else" CLOSED_STMT	{ $$ = new ASTIfStmt($2, $4, $6, $1); }
		   | FOR_STMT											{ $$ = $1; }
		   | "break"											{ ASSERT(IsInLoop, "Cannot have 'break' outside of a loop!"); $$ = new ASTBreakContinueStmt(true, $1); }
		   | "continue"											{ ASSERT(IsInLoop, "Cannot have 'continue' outside of a loop!"); $$ = new ASTBreakContinueStmt(false, $1); }
		   | "return" EXPR_STAR									{ $$ = new ASTReturnStmt($2, $1); }
;

BLOCK: "{" STMT ";" STMTS "}" { $4.insert($4.begin(), $2); $$ = new ASTBlock($4, $1); }

WHILE_STMT: "while" EXPR1 "do" { IsInLoop = true; } STMT { IsInLoop = false; $$ = new ASTWhileStmt($2, $5, $1); }

FOR_STMT: "for" "ID" ":" TYPE "=" EXPR ":" "while" EXPR1 "do" { IsInLoop = true; } STMT { IsInLoop = false; } "then" CLOSED_STMT { $$ = new ASTForStmt(new ASTVarDecl($2.first, $4, $6, false, $1), $9, $12, $15, $1); }
		| "for" "ID" "=" EXPR ":" "while" EXPR1 "do" { IsInLoop = true; } STMT { IsInLoop = false; } "then" CLOSED_STMT { $$ = new ASTForStmt(new ASTVarDecl($2.first, $4, false, $1), $7, $10, $13, $1); }

VARDECL: "let" "ID" "=" EXPR            { $$ = new ASTVarDecl($2.first, $4, InTopLvl, $1); }
	   | "let" "ID" ":" TYPE "=" EXPR   { $$ = new ASTVarDecl($2.first, $4, $6, InTopLvl, $1); }
	   | "let" "_" "=" EXPR				{ ASSERT_MSG(!InTopLvl, "Cannot declare throw away in top level!"); $$ = new ASTVarDecl($4, $1); }
;

TYPE_PLUS: TYPE			      { $$ = { $1 }; }
		 | TYPE "," TYPE_PLUS { $3.insert($3.begin(), $1); $$ = $3; }
;

TYPE:
    "byte"					{ $$ = $1; }
  | "bool"					{ $$ = $1; }
  | "int"					{ $$ = $1; }
  | "float"					{ $$ = $1; }
  | "double"				{ $$ = $1; }
  | "long"					{ $$ = $1; }
  | "string"				{ $$ = $1; }
  | TYPE "[" "INT_LIT" "]"  { $$ = new ArrayType($1, $3->GetValue()); delete $3; }
;

ASSIGNMENT: IDENTIFIER OP_ASSIGN EXPR { $$ = new ASTAssignment($1, $3, $2); };

OP_ASSIGN:
    "="     { $$ = AssignmentType::EQ; }
  | "+="    { $$ = AssignmentType::ADD_EQ; }
  | "-="    { $$ = AssignmentType::SUB_EQ; }
  | "*="    { $$ = AssignmentType::MUL_EQ; }
  | "/="    { $$ = AssignmentType::DIV_EQ; }
  | "%="    { $$ = AssignmentType::MOD_EQ; }
  | "**="   { $$ = AssignmentType::POW_EQ; }
  | "&="    { $$ = AssignmentType::BIN_AND_EQ; }
  | "|="    { $$ = AssignmentType::BIN_OR_EQ; }
  | "^="    { $$ = AssignmentType::BIN_XOR_EQ; }
  | "<<="   { $$ = AssignmentType::LSHIFT_EQ; }
  | ">>="   { $$ = AssignmentType::RSHIFT_EQ; }

EXPR_STAR: %empty		  { $$ = { }; }
		 | EXPR_PLUS	  { $$ = $1; }
;

EXPR_PLUS: EXPR			      { $$ = { $1 }; }
		 | EXPR "," EXPR_PLUS { $3.insert($3.begin(), $1); $$ = $3; }
;

EXPR:
        EXPR1                               { $$ = $1; }
    |   "if" EXPR1 "then" EXPR "else" EXPR  { $$ = new ASTIfExpr($2, $4, $6, $1); }
;        

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
  | "~" EXPR11   { $$ = new ASTUnopExpr($2, UnopType::BIN_NOT); }
  | EXPR12       { $$ = $1; }
;

EXPR12:
    EXPR12 "**" ATOM   { $$ = new ASTBinopExpr($1, $3, BinopType::POW); }
  | ATOM               { $$ = $1; }
;

ATOM:
    "INT_LIT"			{ $$ = $1; }
  | "FLOAT_LIT"			{ $$ = $1; }
  | "BYTE_LIT"			{ $$ = $1; }
  | "DOUBLE_LIT"		{ $$ = $1; }
  | "LONG_LIT"			{ $$ = $1; }
  | "STRING_LIT"		{ $$ = $1; }
  | "true"				{ $$ = $1; }
  | "false"				{ $$ = $1; }
  | IDENTIFIER			{ $$ = $1; }
  | FUNC_CALL			{ $$ = $1; }
  | "(" EXPR ")"		{ $$ = $2; }
  | ARRAY_INIT			{ $$ = $1; }
  | ATOM "[" EXPR "]"	{ $$ = new ASTBinopExpr($1, $3, BinopType::INDEX); }
;

IDENTIFIER: "ID"					{ $$ = new ASTIdentifier($1.first, $1.second); }
FUNC_CALL: "ID" "(" EXPR_STAR ")"	{ $$ = new ASTFuncCallExpr($1.first, $3, $1.second); }
ARRAY_INIT: "[" EXPR_PLUS "]"		{ $$ = new ASTArrayInit($2, $1); }

%%
namespace ramc {
	namespace bison {
		// Report an error to the user.
		auto Parser::error(const std::string& _msg) -> void
		{
			ASSERT_MSG(false, _msg);
		}
	}
}

