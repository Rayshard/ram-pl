// A Bison parser, made by GNU Bison 3.5.0.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2019 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// Undocumented macros, especially those whose name start with YY_,
// are private implementation details.  Do not rely on them.

// "%code top" blocks.
#line 13 "ramc_grammar.yy"

  #include "pch.h"
  #include "ramc_lexer.h"

#line 43 "ramc_bison_parser.cpp"




#include "ramc_bison_parser.hpp"


// Unqualified %code blocks.
#line 23 "ramc_grammar.yy"

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

#line 154 "ramc_bison_parser.cpp"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif



// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

#line 8 "ramc_grammar.yy"
namespace ramc { namespace bison {
#line 227 "ramc_bison_parser.cpp"


  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  Parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              else
                goto append;

            append:
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
  Parser::Parser (Lexer& lexer_yyarg, ASTNode*& result_yyarg, Position& position_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      yy_lac_established_ (false),
      lexer (lexer_yyarg),
      result (result_yyarg),
      position (position_yyarg)
  {}

  Parser::~Parser ()
  {}

  Parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  Parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  Parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  Parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  Parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  Parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  Parser::symbol_number_type
  Parser::by_state::type_get () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  Parser::stack_symbol_type::stack_symbol_type ()
  {}

  Parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state))
  {
    switch (that.type_get ())
    {
      case 102: // ARRAY_INIT
        value.YY_MOVE_OR_COPY< ASTArrayInit* > (YY_MOVE (that.value));
        break;

      case 80: // ASSIGNMENT
        value.YY_MOVE_OR_COPY< ASTAssignment* > (YY_MOVE (that.value));
        break;

      case 82: // BLOCK
        value.YY_MOVE_OR_COPY< ASTBlock* > (YY_MOVE (that.value));
        break;

      case 55: // "true"
      case 56: // "false"
        value.YY_MOVE_OR_COPY< ASTBoolLit* > (YY_MOVE (that.value));
        break;

      case 52: // "BYTE_LIT"
        value.YY_MOVE_OR_COPY< ASTByteLit* > (YY_MOVE (that.value));
        break;

      case 53: // "DOUBLE_LIT"
        value.YY_MOVE_OR_COPY< ASTDoubleLit* > (YY_MOVE (that.value));
        break;

      case 86: // EXPR
      case 87: // EXPR1
      case 88: // EXPR2
      case 89: // EXPR3
      case 90: // EXPR4
      case 91: // EXPR5
      case 92: // EXPR6
      case 93: // EXPR7
      case 94: // EXPR8
      case 95: // EXPR9
      case 96: // EXPR10
      case 97: // EXPR11
      case 98: // EXPR12
      case 99: // ATOM
        value.YY_MOVE_OR_COPY< ASTExpr* > (YY_MOVE (that.value));
        break;

      case 51: // "FLOAT_LIT"
        value.YY_MOVE_OR_COPY< ASTFloatLit* > (YY_MOVE (that.value));
        break;

      case 101: // FUNC_CALL
        value.YY_MOVE_OR_COPY< ASTFuncCallExpr* > (YY_MOVE (that.value));
        break;

      case 83: // FUNCDECL
        value.YY_MOVE_OR_COPY< ASTFuncDecl* > (YY_MOVE (that.value));
        break;

      case 100: // IDENTIFIER
        value.YY_MOVE_OR_COPY< ASTIdentifier* > (YY_MOVE (that.value));
        break;

      case 49: // "INT_LIT"
        value.YY_MOVE_OR_COPY< ASTIntLit* > (YY_MOVE (that.value));
        break;

      case 54: // "LONG_LIT"
        value.YY_MOVE_OR_COPY< ASTLongLit* > (YY_MOVE (that.value));
        break;

      case 76: // PROGRAM
        value.YY_MOVE_OR_COPY< ASTProgram* > (YY_MOVE (that.value));
        break;

      case 77: // STMT
      case 78: // OPEN_STMT
      case 79: // CLOSED_STMT
      case 84: // WHILE_STMT
      case 85: // FOR_STMT
        value.YY_MOVE_OR_COPY< ASTStmt* > (YY_MOVE (that.value));
        break;

      case 50: // "STRING_LIT"
        value.YY_MOVE_OR_COPY< ASTStringLit* > (YY_MOVE (that.value));
        break;

      case 81: // VARDECL
        value.YY_MOVE_OR_COPY< ASTVarDecl* > (YY_MOVE (that.value));
        break;

      case 108: // OP_ASSIGN
        value.YY_MOVE_OR_COPY< AssignmentType > (YY_MOVE (that.value));
        break;

      case 113: // PARAM
        value.YY_MOVE_OR_COPY< Param > (YY_MOVE (that.value));
        break;

      case 38: // "{"
      case 40: // "["
      case 64: // "let"
      case 65: // "if"
      case 66: // "then"
      case 67: // "else"
      case 68: // "while"
      case 69: // "for"
      case 70: // "do"
      case 71: // "func"
      case 72: // "break"
      case 73: // "continue"
      case 74: // "return"
        value.YY_MOVE_OR_COPY< Position > (YY_MOVE (that.value));
        break;

      case 57: // "byte"
      case 58: // "bool"
      case 59: // "int"
      case 60: // "float"
      case 61: // "double"
      case 62: // "long"
      case 63: // "string"
      case 110: // TYPE
        value.YY_MOVE_OR_COPY< Type* > (YY_MOVE (that.value));
        break;

      case 48: // "ID"
        value.YY_MOVE_OR_COPY< std::pair<std::string, Position> > (YY_MOVE (that.value));
        break;

      case 106: // EXPR_STAR
      case 107: // EXPR_PLUS
        value.YY_MOVE_OR_COPY< std::vector<ASTExpr*> > (YY_MOVE (that.value));
        break;

      case 104: // TL_FUNCDECLS
        value.YY_MOVE_OR_COPY< std::vector<ASTFuncDecl*> > (YY_MOVE (that.value));
        break;

      case 105: // STMTS
        value.YY_MOVE_OR_COPY< std::vector<ASTStmt*> > (YY_MOVE (that.value));
        break;

      case 103: // VARDECLS
        value.YY_MOVE_OR_COPY< std::vector<ASTVarDecl*> > (YY_MOVE (that.value));
        break;

      case 111: // PARAM_STAR
      case 112: // PARAM_PLUS
        value.YY_MOVE_OR_COPY< std::vector<Param> > (YY_MOVE (that.value));
        break;

      case 109: // TYPE_PLUS
        value.YY_MOVE_OR_COPY< std::vector<Type*> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  Parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s)
  {
    switch (that.type_get ())
    {
      case 102: // ARRAY_INIT
        value.move< ASTArrayInit* > (YY_MOVE (that.value));
        break;

      case 80: // ASSIGNMENT
        value.move< ASTAssignment* > (YY_MOVE (that.value));
        break;

      case 82: // BLOCK
        value.move< ASTBlock* > (YY_MOVE (that.value));
        break;

      case 55: // "true"
      case 56: // "false"
        value.move< ASTBoolLit* > (YY_MOVE (that.value));
        break;

      case 52: // "BYTE_LIT"
        value.move< ASTByteLit* > (YY_MOVE (that.value));
        break;

      case 53: // "DOUBLE_LIT"
        value.move< ASTDoubleLit* > (YY_MOVE (that.value));
        break;

      case 86: // EXPR
      case 87: // EXPR1
      case 88: // EXPR2
      case 89: // EXPR3
      case 90: // EXPR4
      case 91: // EXPR5
      case 92: // EXPR6
      case 93: // EXPR7
      case 94: // EXPR8
      case 95: // EXPR9
      case 96: // EXPR10
      case 97: // EXPR11
      case 98: // EXPR12
      case 99: // ATOM
        value.move< ASTExpr* > (YY_MOVE (that.value));
        break;

      case 51: // "FLOAT_LIT"
        value.move< ASTFloatLit* > (YY_MOVE (that.value));
        break;

      case 101: // FUNC_CALL
        value.move< ASTFuncCallExpr* > (YY_MOVE (that.value));
        break;

      case 83: // FUNCDECL
        value.move< ASTFuncDecl* > (YY_MOVE (that.value));
        break;

      case 100: // IDENTIFIER
        value.move< ASTIdentifier* > (YY_MOVE (that.value));
        break;

      case 49: // "INT_LIT"
        value.move< ASTIntLit* > (YY_MOVE (that.value));
        break;

      case 54: // "LONG_LIT"
        value.move< ASTLongLit* > (YY_MOVE (that.value));
        break;

      case 76: // PROGRAM
        value.move< ASTProgram* > (YY_MOVE (that.value));
        break;

      case 77: // STMT
      case 78: // OPEN_STMT
      case 79: // CLOSED_STMT
      case 84: // WHILE_STMT
      case 85: // FOR_STMT
        value.move< ASTStmt* > (YY_MOVE (that.value));
        break;

      case 50: // "STRING_LIT"
        value.move< ASTStringLit* > (YY_MOVE (that.value));
        break;

      case 81: // VARDECL
        value.move< ASTVarDecl* > (YY_MOVE (that.value));
        break;

      case 108: // OP_ASSIGN
        value.move< AssignmentType > (YY_MOVE (that.value));
        break;

      case 113: // PARAM
        value.move< Param > (YY_MOVE (that.value));
        break;

      case 38: // "{"
      case 40: // "["
      case 64: // "let"
      case 65: // "if"
      case 66: // "then"
      case 67: // "else"
      case 68: // "while"
      case 69: // "for"
      case 70: // "do"
      case 71: // "func"
      case 72: // "break"
      case 73: // "continue"
      case 74: // "return"
        value.move< Position > (YY_MOVE (that.value));
        break;

      case 57: // "byte"
      case 58: // "bool"
      case 59: // "int"
      case 60: // "float"
      case 61: // "double"
      case 62: // "long"
      case 63: // "string"
      case 110: // TYPE
        value.move< Type* > (YY_MOVE (that.value));
        break;

      case 48: // "ID"
        value.move< std::pair<std::string, Position> > (YY_MOVE (that.value));
        break;

      case 106: // EXPR_STAR
      case 107: // EXPR_PLUS
        value.move< std::vector<ASTExpr*> > (YY_MOVE (that.value));
        break;

      case 104: // TL_FUNCDECLS
        value.move< std::vector<ASTFuncDecl*> > (YY_MOVE (that.value));
        break;

      case 105: // STMTS
        value.move< std::vector<ASTStmt*> > (YY_MOVE (that.value));
        break;

      case 103: // VARDECLS
        value.move< std::vector<ASTVarDecl*> > (YY_MOVE (that.value));
        break;

      case 111: // PARAM_STAR
      case 112: // PARAM_PLUS
        value.move< std::vector<Param> > (YY_MOVE (that.value));
        break;

      case 109: // TYPE_PLUS
        value.move< std::vector<Type*> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

#if YY_CPLUSPLUS < 201103L
  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 102: // ARRAY_INIT
        value.copy< ASTArrayInit* > (that.value);
        break;

      case 80: // ASSIGNMENT
        value.copy< ASTAssignment* > (that.value);
        break;

      case 82: // BLOCK
        value.copy< ASTBlock* > (that.value);
        break;

      case 55: // "true"
      case 56: // "false"
        value.copy< ASTBoolLit* > (that.value);
        break;

      case 52: // "BYTE_LIT"
        value.copy< ASTByteLit* > (that.value);
        break;

      case 53: // "DOUBLE_LIT"
        value.copy< ASTDoubleLit* > (that.value);
        break;

      case 86: // EXPR
      case 87: // EXPR1
      case 88: // EXPR2
      case 89: // EXPR3
      case 90: // EXPR4
      case 91: // EXPR5
      case 92: // EXPR6
      case 93: // EXPR7
      case 94: // EXPR8
      case 95: // EXPR9
      case 96: // EXPR10
      case 97: // EXPR11
      case 98: // EXPR12
      case 99: // ATOM
        value.copy< ASTExpr* > (that.value);
        break;

      case 51: // "FLOAT_LIT"
        value.copy< ASTFloatLit* > (that.value);
        break;

      case 101: // FUNC_CALL
        value.copy< ASTFuncCallExpr* > (that.value);
        break;

      case 83: // FUNCDECL
        value.copy< ASTFuncDecl* > (that.value);
        break;

      case 100: // IDENTIFIER
        value.copy< ASTIdentifier* > (that.value);
        break;

      case 49: // "INT_LIT"
        value.copy< ASTIntLit* > (that.value);
        break;

      case 54: // "LONG_LIT"
        value.copy< ASTLongLit* > (that.value);
        break;

      case 76: // PROGRAM
        value.copy< ASTProgram* > (that.value);
        break;

      case 77: // STMT
      case 78: // OPEN_STMT
      case 79: // CLOSED_STMT
      case 84: // WHILE_STMT
      case 85: // FOR_STMT
        value.copy< ASTStmt* > (that.value);
        break;

      case 50: // "STRING_LIT"
        value.copy< ASTStringLit* > (that.value);
        break;

      case 81: // VARDECL
        value.copy< ASTVarDecl* > (that.value);
        break;

      case 108: // OP_ASSIGN
        value.copy< AssignmentType > (that.value);
        break;

      case 113: // PARAM
        value.copy< Param > (that.value);
        break;

      case 38: // "{"
      case 40: // "["
      case 64: // "let"
      case 65: // "if"
      case 66: // "then"
      case 67: // "else"
      case 68: // "while"
      case 69: // "for"
      case 70: // "do"
      case 71: // "func"
      case 72: // "break"
      case 73: // "continue"
      case 74: // "return"
        value.copy< Position > (that.value);
        break;

      case 57: // "byte"
      case 58: // "bool"
      case 59: // "int"
      case 60: // "float"
      case 61: // "double"
      case 62: // "long"
      case 63: // "string"
      case 110: // TYPE
        value.copy< Type* > (that.value);
        break;

      case 48: // "ID"
        value.copy< std::pair<std::string, Position> > (that.value);
        break;

      case 106: // EXPR_STAR
      case 107: // EXPR_PLUS
        value.copy< std::vector<ASTExpr*> > (that.value);
        break;

      case 104: // TL_FUNCDECLS
        value.copy< std::vector<ASTFuncDecl*> > (that.value);
        break;

      case 105: // STMTS
        value.copy< std::vector<ASTStmt*> > (that.value);
        break;

      case 103: // VARDECLS
        value.copy< std::vector<ASTVarDecl*> > (that.value);
        break;

      case 111: // PARAM_STAR
      case 112: // PARAM_PLUS
        value.copy< std::vector<Param> > (that.value);
        break;

      case 109: // TYPE_PLUS
        value.copy< std::vector<Type*> > (that.value);
        break;

      default:
        break;
    }

    return *this;
  }

  Parser::stack_symbol_type&
  Parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 102: // ARRAY_INIT
        value.move< ASTArrayInit* > (that.value);
        break;

      case 80: // ASSIGNMENT
        value.move< ASTAssignment* > (that.value);
        break;

      case 82: // BLOCK
        value.move< ASTBlock* > (that.value);
        break;

      case 55: // "true"
      case 56: // "false"
        value.move< ASTBoolLit* > (that.value);
        break;

      case 52: // "BYTE_LIT"
        value.move< ASTByteLit* > (that.value);
        break;

      case 53: // "DOUBLE_LIT"
        value.move< ASTDoubleLit* > (that.value);
        break;

      case 86: // EXPR
      case 87: // EXPR1
      case 88: // EXPR2
      case 89: // EXPR3
      case 90: // EXPR4
      case 91: // EXPR5
      case 92: // EXPR6
      case 93: // EXPR7
      case 94: // EXPR8
      case 95: // EXPR9
      case 96: // EXPR10
      case 97: // EXPR11
      case 98: // EXPR12
      case 99: // ATOM
        value.move< ASTExpr* > (that.value);
        break;

      case 51: // "FLOAT_LIT"
        value.move< ASTFloatLit* > (that.value);
        break;

      case 101: // FUNC_CALL
        value.move< ASTFuncCallExpr* > (that.value);
        break;

      case 83: // FUNCDECL
        value.move< ASTFuncDecl* > (that.value);
        break;

      case 100: // IDENTIFIER
        value.move< ASTIdentifier* > (that.value);
        break;

      case 49: // "INT_LIT"
        value.move< ASTIntLit* > (that.value);
        break;

      case 54: // "LONG_LIT"
        value.move< ASTLongLit* > (that.value);
        break;

      case 76: // PROGRAM
        value.move< ASTProgram* > (that.value);
        break;

      case 77: // STMT
      case 78: // OPEN_STMT
      case 79: // CLOSED_STMT
      case 84: // WHILE_STMT
      case 85: // FOR_STMT
        value.move< ASTStmt* > (that.value);
        break;

      case 50: // "STRING_LIT"
        value.move< ASTStringLit* > (that.value);
        break;

      case 81: // VARDECL
        value.move< ASTVarDecl* > (that.value);
        break;

      case 108: // OP_ASSIGN
        value.move< AssignmentType > (that.value);
        break;

      case 113: // PARAM
        value.move< Param > (that.value);
        break;

      case 38: // "{"
      case 40: // "["
      case 64: // "let"
      case 65: // "if"
      case 66: // "then"
      case 67: // "else"
      case 68: // "while"
      case 69: // "for"
      case 70: // "do"
      case 71: // "func"
      case 72: // "break"
      case 73: // "continue"
      case 74: // "return"
        value.move< Position > (that.value);
        break;

      case 57: // "byte"
      case 58: // "bool"
      case 59: // "int"
      case 60: // "float"
      case 61: // "double"
      case 62: // "long"
      case 63: // "string"
      case 110: // TYPE
        value.move< Type* > (that.value);
        break;

      case 48: // "ID"
        value.move< std::pair<std::string, Position> > (that.value);
        break;

      case 106: // EXPR_STAR
      case 107: // EXPR_PLUS
        value.move< std::vector<ASTExpr*> > (that.value);
        break;

      case 104: // TL_FUNCDECLS
        value.move< std::vector<ASTFuncDecl*> > (that.value);
        break;

      case 105: // STMTS
        value.move< std::vector<ASTStmt*> > (that.value);
        break;

      case 103: // VARDECLS
        value.move< std::vector<ASTVarDecl*> > (that.value);
        break;

      case 111: // PARAM_STAR
      case 112: // PARAM_PLUS
        value.move< std::vector<Param> > (that.value);
        break;

      case 109: // TYPE_PLUS
        value.move< std::vector<Type*> > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  Parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  Parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
#if defined __GNUC__ && ! defined __clang__ && ! defined __ICC && __GNUC__ * 100 + __GNUC_MINOR__ <= 408
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
#endif
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " (";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  void
  Parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  Parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  Parser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  Parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Parser::debug_level_type
  Parser::debug_level () const
  {
    return yydebug_;
  }

  void
  Parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  Parser::state_type
  Parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  bool
  Parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  Parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Parser::operator() ()
  {
    return parse ();
  }

  int
  Parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The return value of parse ().
    int yyresult;

    /// Discard the LAC context in case there still is one left from a
    /// previous invocation.
    yy_lac_discard_ ("init");

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            symbol_type yylookahead (yylex (lexer, result, position));
            yyla.move (yylookahead);
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      {
        if (!yy_lac_establish_ (yyla.type_get ()))
           goto yyerrlab;
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        if (!yy_lac_establish_ (yyla.type_get ()))
           goto yyerrlab;

        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", static_cast<state_type> (yyn), YY_MOVE (yyla));
    yy_lac_discard_ ("shift");
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case 102: // ARRAY_INIT
        yylhs.value.emplace< ASTArrayInit* > ();
        break;

      case 80: // ASSIGNMENT
        yylhs.value.emplace< ASTAssignment* > ();
        break;

      case 82: // BLOCK
        yylhs.value.emplace< ASTBlock* > ();
        break;

      case 55: // "true"
      case 56: // "false"
        yylhs.value.emplace< ASTBoolLit* > ();
        break;

      case 52: // "BYTE_LIT"
        yylhs.value.emplace< ASTByteLit* > ();
        break;

      case 53: // "DOUBLE_LIT"
        yylhs.value.emplace< ASTDoubleLit* > ();
        break;

      case 86: // EXPR
      case 87: // EXPR1
      case 88: // EXPR2
      case 89: // EXPR3
      case 90: // EXPR4
      case 91: // EXPR5
      case 92: // EXPR6
      case 93: // EXPR7
      case 94: // EXPR8
      case 95: // EXPR9
      case 96: // EXPR10
      case 97: // EXPR11
      case 98: // EXPR12
      case 99: // ATOM
        yylhs.value.emplace< ASTExpr* > ();
        break;

      case 51: // "FLOAT_LIT"
        yylhs.value.emplace< ASTFloatLit* > ();
        break;

      case 101: // FUNC_CALL
        yylhs.value.emplace< ASTFuncCallExpr* > ();
        break;

      case 83: // FUNCDECL
        yylhs.value.emplace< ASTFuncDecl* > ();
        break;

      case 100: // IDENTIFIER
        yylhs.value.emplace< ASTIdentifier* > ();
        break;

      case 49: // "INT_LIT"
        yylhs.value.emplace< ASTIntLit* > ();
        break;

      case 54: // "LONG_LIT"
        yylhs.value.emplace< ASTLongLit* > ();
        break;

      case 76: // PROGRAM
        yylhs.value.emplace< ASTProgram* > ();
        break;

      case 77: // STMT
      case 78: // OPEN_STMT
      case 79: // CLOSED_STMT
      case 84: // WHILE_STMT
      case 85: // FOR_STMT
        yylhs.value.emplace< ASTStmt* > ();
        break;

      case 50: // "STRING_LIT"
        yylhs.value.emplace< ASTStringLit* > ();
        break;

      case 81: // VARDECL
        yylhs.value.emplace< ASTVarDecl* > ();
        break;

      case 108: // OP_ASSIGN
        yylhs.value.emplace< AssignmentType > ();
        break;

      case 113: // PARAM
        yylhs.value.emplace< Param > ();
        break;

      case 38: // "{"
      case 40: // "["
      case 64: // "let"
      case 65: // "if"
      case 66: // "then"
      case 67: // "else"
      case 68: // "while"
      case 69: // "for"
      case 70: // "do"
      case 71: // "func"
      case 72: // "break"
      case 73: // "continue"
      case 74: // "return"
        yylhs.value.emplace< Position > ();
        break;

      case 57: // "byte"
      case 58: // "bool"
      case 59: // "int"
      case 60: // "float"
      case 61: // "double"
      case 62: // "long"
      case 63: // "string"
      case 110: // TYPE
        yylhs.value.emplace< Type* > ();
        break;

      case 48: // "ID"
        yylhs.value.emplace< std::pair<std::string, Position> > ();
        break;

      case 106: // EXPR_STAR
      case 107: // EXPR_PLUS
        yylhs.value.emplace< std::vector<ASTExpr*> > ();
        break;

      case 104: // TL_FUNCDECLS
        yylhs.value.emplace< std::vector<ASTFuncDecl*> > ();
        break;

      case 105: // STMTS
        yylhs.value.emplace< std::vector<ASTStmt*> > ();
        break;

      case 103: // VARDECLS
        yylhs.value.emplace< std::vector<ASTVarDecl*> > ();
        break;

      case 111: // PARAM_STAR
      case 112: // PARAM_PLUS
        yylhs.value.emplace< std::vector<Param> > ();
        break;

      case 109: // TYPE_PLUS
        yylhs.value.emplace< std::vector<Type*> > ();
        break;

      default:
        break;
    }



      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2:
#line 245 "ramc_grammar.yy"
         { InTopLvl = true; }
#line 1410 "ramc_bison_parser.cpp"
    break;

  case 3:
#line 245 "ramc_grammar.yy"
                                       { InTopLvl = false; }
#line 1416 "ramc_bison_parser.cpp"
    break;

  case 4:
#line 245 "ramc_grammar.yy"
                                                                          { result = new ASTProgram("Test File", yystack_[2].value.as < std::vector<ASTVarDecl*> > (), yystack_[0].value.as < std::vector<ASTFuncDecl*> > ()); }
#line 1422 "ramc_bison_parser.cpp"
    break;

  case 5:
#line 247 "ramc_grammar.yy"
                                           { yylhs.value.as < std::vector<ASTVarDecl*> > () = { }; }
#line 1428 "ramc_bison_parser.cpp"
    break;

  case 6:
#line 248 "ramc_grammar.yy"
                                       { yystack_[0].value.as < std::vector<ASTVarDecl*> > ().insert(yystack_[0].value.as < std::vector<ASTVarDecl*> > ().begin(), yystack_[2].value.as < ASTVarDecl* > ()); yylhs.value.as < std::vector<ASTVarDecl*> > () = yystack_[0].value.as < std::vector<ASTVarDecl*> > (); }
#line 1434 "ramc_bison_parser.cpp"
    break;

  case 7:
#line 251 "ramc_grammar.yy"
                                                { yylhs.value.as < std::vector<ASTFuncDecl*> > () = { }; }
#line 1440 "ramc_bison_parser.cpp"
    break;

  case 8:
#line 252 "ramc_grammar.yy"
                                                { yystack_[0].value.as < std::vector<ASTFuncDecl*> > ().insert(yystack_[0].value.as < std::vector<ASTFuncDecl*> > ().begin(), yystack_[1].value.as < ASTFuncDecl* > ()); yylhs.value.as < std::vector<ASTFuncDecl*> > () = yystack_[0].value.as < std::vector<ASTFuncDecl*> > (); }
#line 1446 "ramc_bison_parser.cpp"
    break;

  case 9:
#line 255 "ramc_grammar.yy"
                                                                      { yylhs.value.as < ASTFuncDecl* > () = new ASTFuncDecl(yystack_[8].value.as < std::pair<std::string, Position> > ().first, yystack_[5].value.as < std::vector<Param> > (), yystack_[2].value.as < std::vector<Type*> > (), yystack_[0].value.as < ASTBlock* > (), yystack_[9].value.as < Position > ()); }
#line 1452 "ramc_bison_parser.cpp"
    break;

  case 10:
#line 256 "ramc_grammar.yy"
                                                                                          { yylhs.value.as < ASTFuncDecl* > () = new ASTFuncDecl(yystack_[6].value.as < std::pair<std::string, Position> > ().first, yystack_[3].value.as < std::vector<Param> > (), { }, yystack_[0].value.as < ASTBlock* > (), yystack_[7].value.as < Position > ()); }
#line 1458 "ramc_bison_parser.cpp"
    break;

  case 11:
#line 258 "ramc_grammar.yy"
                           { yylhs.value.as < std::vector<Param> > () = { }; }
#line 1464 "ramc_bison_parser.cpp"
    break;

  case 12:
#line 259 "ramc_grammar.yy"
                               { yylhs.value.as < std::vector<Param> > () = yystack_[0].value.as < std::vector<Param> > (); }
#line 1470 "ramc_bison_parser.cpp"
    break;

  case 13:
#line 262 "ramc_grammar.yy"
                                             { yylhs.value.as < std::vector<Param> > () = { yystack_[0].value.as < Param > () }; }
#line 1476 "ramc_bison_parser.cpp"
    break;

  case 14:
#line 263 "ramc_grammar.yy"
                                         { yystack_[0].value.as < std::vector<Param> > ().insert(yystack_[0].value.as < std::vector<Param> > ().begin(), yystack_[2].value.as < Param > ()); yylhs.value.as < std::vector<Param> > () = yystack_[0].value.as < std::vector<Param> > (); }
#line 1482 "ramc_bison_parser.cpp"
    break;

  case 15:
#line 266 "ramc_grammar.yy"
                     { yylhs.value.as < Param > () = { yystack_[2].value.as < std::pair<std::string, Position> > ().first, yystack_[0].value.as < Type* > (), yystack_[2].value.as < std::pair<std::string, Position> > ().second }; }
#line 1488 "ramc_bison_parser.cpp"
    break;

  case 16:
#line 268 "ramc_grammar.yy"
                          { yylhs.value.as < std::vector<ASTStmt*> > () = { }; }
#line 1494 "ramc_bison_parser.cpp"
    break;

  case 17:
#line 269 "ramc_grammar.yy"
                          { yystack_[0].value.as < std::vector<ASTStmt*> > ().insert(yystack_[0].value.as < std::vector<ASTStmt*> > ().begin(), yystack_[2].value.as < ASTStmt* > ()); yylhs.value.as < std::vector<ASTStmt*> > () = yystack_[0].value.as < std::vector<ASTStmt*> > (); }
#line 1500 "ramc_bison_parser.cpp"
    break;

  case 18:
#line 272 "ramc_grammar.yy"
                  { yylhs.value.as < ASTStmt* > () = yystack_[0].value.as < ASTStmt* > (); }
#line 1506 "ramc_bison_parser.cpp"
    break;

  case 19:
#line 273 "ramc_grammar.yy"
                  { yylhs.value.as < ASTStmt* > () = yystack_[0].value.as < ASTStmt* > (); }
#line 1512 "ramc_bison_parser.cpp"
    break;

  case 20:
#line 276 "ramc_grammar.yy"
                                                                                { yylhs.value.as < ASTStmt* > () = new ASTIfStmt(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTStmt* > (), nullptr, yystack_[3].value.as < Position > ()); }
#line 1518 "ramc_bison_parser.cpp"
    break;

  case 21:
#line 277 "ramc_grammar.yy"
                                                                        { yylhs.value.as < ASTStmt* > () = new ASTIfStmt(yystack_[4].value.as < ASTExpr* > (), yystack_[2].value.as < ASTStmt* > (), yystack_[0].value.as < ASTStmt* > (), yystack_[5].value.as < Position > ()); }
#line 1524 "ramc_bison_parser.cpp"
    break;

  case 22:
#line 278 "ramc_grammar.yy"
                                                                                                        { yylhs.value.as < ASTStmt* > () = yystack_[0].value.as < ASTStmt* > (); }
#line 1530 "ramc_bison_parser.cpp"
    break;

  case 23:
#line 281 "ramc_grammar.yy"
                                                                                                        { yylhs.value.as < ASTStmt* > () = yystack_[0].value.as < ASTAssignment* > (); }
#line 1536 "ramc_bison_parser.cpp"
    break;

  case 24:
#line 282 "ramc_grammar.yy"
                                                                                                                { yylhs.value.as < ASTStmt* > () = yystack_[0].value.as < ASTVarDecl* > (); }
#line 1542 "ramc_bison_parser.cpp"
    break;

  case 25:
#line 283 "ramc_grammar.yy"
                                                                                                                        { yylhs.value.as < ASTStmt* > () = yystack_[0].value.as < ASTBlock* > (); }
#line 1548 "ramc_bison_parser.cpp"
    break;

  case 26:
#line 284 "ramc_grammar.yy"
                                                                        { yylhs.value.as < ASTStmt* > () = new ASTIfStmt(yystack_[4].value.as < ASTExpr* > (), yystack_[2].value.as < ASTStmt* > (), yystack_[0].value.as < ASTStmt* > (), yystack_[5].value.as < Position > ()); }
#line 1554 "ramc_bison_parser.cpp"
    break;

  case 27:
#line 285 "ramc_grammar.yy"
                                                                                                                { yylhs.value.as < ASTStmt* > () = yystack_[0].value.as < ASTStmt* > (); }
#line 1560 "ramc_bison_parser.cpp"
    break;

  case 28:
#line 286 "ramc_grammar.yy"
                                                                                                                { ASSERT_MSG(IsInLoop, "Cannot have 'break' outside of a loop!"); yylhs.value.as < ASTStmt* > () = new ASTBreakContinueStmt(true, yystack_[0].value.as < Position > ()); }
#line 1566 "ramc_bison_parser.cpp"
    break;

  case 29:
#line 287 "ramc_grammar.yy"
                                                                                                                { ASSERT_MSG(IsInLoop, "Cannot have 'continue' outside of a loop!"); yylhs.value.as < ASTStmt* > () = new ASTBreakContinueStmt(false, yystack_[0].value.as < Position > ()); }
#line 1572 "ramc_bison_parser.cpp"
    break;

  case 30:
#line 288 "ramc_grammar.yy"
                                                                                                        { yylhs.value.as < ASTStmt* > () = new ASTReturnStmt(yystack_[0].value.as < std::vector<ASTExpr*> > (), yystack_[1].value.as < Position > ()); }
#line 1578 "ramc_bison_parser.cpp"
    break;

  case 31:
#line 291 "ramc_grammar.yy"
                              { yystack_[1].value.as < std::vector<ASTStmt*> > ().insert(yystack_[1].value.as < std::vector<ASTStmt*> > ().begin(), yystack_[3].value.as < ASTStmt* > ()); yylhs.value.as < ASTBlock* > () = new ASTBlock(yystack_[1].value.as < std::vector<ASTStmt*> > (), yystack_[4].value.as < Position > ()); }
#line 1584 "ramc_bison_parser.cpp"
    break;

  case 32:
#line 293 "ramc_grammar.yy"
                               { IsInLoop = true; }
#line 1590 "ramc_bison_parser.cpp"
    break;

  case 33:
#line 293 "ramc_grammar.yy"
                                                         { IsInLoop = false; yylhs.value.as < ASTStmt* > () = new ASTWhileStmt(yystack_[3].value.as < ASTExpr* > (), yystack_[0].value.as < ASTStmt* > (), yystack_[4].value.as < Position > ()); }
#line 1596 "ramc_bison_parser.cpp"
    break;

  case 34:
#line 295 "ramc_grammar.yy"
                                                              { IsInLoop = true; }
#line 1602 "ramc_bison_parser.cpp"
    break;

  case 35:
#line 295 "ramc_grammar.yy"
                                                                                        { IsInLoop = false; }
#line 1608 "ramc_bison_parser.cpp"
    break;

  case 36:
#line 295 "ramc_grammar.yy"
                                                                                                                                 { yylhs.value.as < ASTStmt* > () = new ASTForStmt(new ASTVarDecl(yystack_[13].value.as < std::pair<std::string, Position> > ().first, yystack_[11].value.as < Type* > (), yystack_[9].value.as < ASTExpr* > (), false, yystack_[14].value.as < Position > ()), yystack_[6].value.as < ASTExpr* > (), yystack_[3].value.as < ASTStmt* > (), yystack_[0].value.as < ASTStmt* > (), yystack_[14].value.as < Position > ()); }
#line 1614 "ramc_bison_parser.cpp"
    break;

  case 37:
#line 296 "ramc_grammar.yy"
                                                             { IsInLoop = true; }
#line 1620 "ramc_bison_parser.cpp"
    break;

  case 38:
#line 296 "ramc_grammar.yy"
                                                                                       { IsInLoop = false; }
#line 1626 "ramc_bison_parser.cpp"
    break;

  case 39:
#line 296 "ramc_grammar.yy"
                                                                                                                                { yylhs.value.as < ASTStmt* > () = new ASTForStmt(new ASTVarDecl(yystack_[11].value.as < std::pair<std::string, Position> > ().first, yystack_[9].value.as < ASTExpr* > (), false, yystack_[12].value.as < Position > ()), yystack_[6].value.as < ASTExpr* > (), yystack_[3].value.as < ASTStmt* > (), yystack_[0].value.as < ASTStmt* > (), yystack_[12].value.as < Position > ()); }
#line 1632 "ramc_bison_parser.cpp"
    break;

  case 40:
#line 298 "ramc_grammar.yy"
                                        { yylhs.value.as < ASTVarDecl* > () = new ASTVarDecl(yystack_[2].value.as < std::pair<std::string, Position> > ().first, yystack_[0].value.as < ASTExpr* > (), InTopLvl, yystack_[3].value.as < Position > ()); }
#line 1638 "ramc_bison_parser.cpp"
    break;

  case 41:
#line 299 "ramc_grammar.yy"
                                            { yylhs.value.as < ASTVarDecl* > () = new ASTVarDecl(yystack_[4].value.as < std::pair<std::string, Position> > ().first, yystack_[2].value.as < Type* > (), yystack_[0].value.as < ASTExpr* > (), InTopLvl, yystack_[5].value.as < Position > ()); }
#line 1644 "ramc_bison_parser.cpp"
    break;

  case 42:
#line 300 "ramc_grammar.yy"
                                                        { ASSERT_MSG(!InTopLvl, "Cannot declare throw away in top level!"); yylhs.value.as < ASTVarDecl* > () = new ASTVarDecl(yystack_[0].value.as < ASTExpr* > (), yystack_[3].value.as < Position > ()); }
#line 1650 "ramc_bison_parser.cpp"
    break;

  case 43:
#line 303 "ramc_grammar.yy"
                                      { yylhs.value.as < std::vector<Type*> > () = { yystack_[0].value.as < Type* > () }; }
#line 1656 "ramc_bison_parser.cpp"
    break;

  case 44:
#line 304 "ramc_grammar.yy"
                                      { yystack_[0].value.as < std::vector<Type*> > ().insert(yystack_[0].value.as < std::vector<Type*> > ().begin(), yystack_[2].value.as < Type* > ()); yylhs.value.as < std::vector<Type*> > () = yystack_[0].value.as < std::vector<Type*> > (); }
#line 1662 "ramc_bison_parser.cpp"
    break;

  case 45:
#line 308 "ramc_grammar.yy"
                                                { yylhs.value.as < Type* > () = yystack_[0].value.as < Type* > (); }
#line 1668 "ramc_bison_parser.cpp"
    break;

  case 46:
#line 309 "ramc_grammar.yy"
                                                { yylhs.value.as < Type* > () = yystack_[0].value.as < Type* > (); }
#line 1674 "ramc_bison_parser.cpp"
    break;

  case 47:
#line 310 "ramc_grammar.yy"
                                                { yylhs.value.as < Type* > () = yystack_[0].value.as < Type* > (); }
#line 1680 "ramc_bison_parser.cpp"
    break;

  case 48:
#line 311 "ramc_grammar.yy"
                                                { yylhs.value.as < Type* > () = yystack_[0].value.as < Type* > (); }
#line 1686 "ramc_bison_parser.cpp"
    break;

  case 49:
#line 312 "ramc_grammar.yy"
                                        { yylhs.value.as < Type* > () = yystack_[0].value.as < Type* > (); }
#line 1692 "ramc_bison_parser.cpp"
    break;

  case 50:
#line 313 "ramc_grammar.yy"
                                                { yylhs.value.as < Type* > () = yystack_[0].value.as < Type* > (); }
#line 1698 "ramc_bison_parser.cpp"
    break;

  case 51:
#line 314 "ramc_grammar.yy"
                                        { yylhs.value.as < Type* > () = yystack_[0].value.as < Type* > (); }
#line 1704 "ramc_bison_parser.cpp"
    break;

  case 52:
#line 315 "ramc_grammar.yy"
                            { yylhs.value.as < Type* > () = new ArrayType(yystack_[3].value.as < Type* > (), yystack_[1].value.as < ASTIntLit* > ()->GetValue()); delete yystack_[1].value.as < ASTIntLit* > (); }
#line 1710 "ramc_bison_parser.cpp"
    break;

  case 53:
#line 318 "ramc_grammar.yy"
                                      { yylhs.value.as < ASTAssignment* > () = new ASTAssignment(yystack_[2].value.as < ASTIdentifier* > (), yystack_[0].value.as < ASTExpr* > (), yystack_[1].value.as < AssignmentType > ()); }
#line 1716 "ramc_bison_parser.cpp"
    break;

  case 54:
#line 321 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::EQ; }
#line 1722 "ramc_bison_parser.cpp"
    break;

  case 55:
#line 322 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::ADD_EQ; }
#line 1728 "ramc_bison_parser.cpp"
    break;

  case 56:
#line 323 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::SUB_EQ; }
#line 1734 "ramc_bison_parser.cpp"
    break;

  case 57:
#line 324 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::MUL_EQ; }
#line 1740 "ramc_bison_parser.cpp"
    break;

  case 58:
#line 325 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::DIV_EQ; }
#line 1746 "ramc_bison_parser.cpp"
    break;

  case 59:
#line 326 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::MOD_EQ; }
#line 1752 "ramc_bison_parser.cpp"
    break;

  case 60:
#line 327 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::POW_EQ; }
#line 1758 "ramc_bison_parser.cpp"
    break;

  case 61:
#line 328 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::BIN_AND_EQ; }
#line 1764 "ramc_bison_parser.cpp"
    break;

  case 62:
#line 329 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::BIN_OR_EQ; }
#line 1770 "ramc_bison_parser.cpp"
    break;

  case 63:
#line 330 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::BIN_XOR_EQ; }
#line 1776 "ramc_bison_parser.cpp"
    break;

  case 64:
#line 331 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::LSHIFT_EQ; }
#line 1782 "ramc_bison_parser.cpp"
    break;

  case 65:
#line 332 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::RSHIFT_EQ; }
#line 1788 "ramc_bison_parser.cpp"
    break;

  case 66:
#line 334 "ramc_grammar.yy"
                                  { yylhs.value.as < std::vector<ASTExpr*> > () = { }; }
#line 1794 "ramc_bison_parser.cpp"
    break;

  case 67:
#line 335 "ramc_grammar.yy"
                                  { yylhs.value.as < std::vector<ASTExpr*> > () = yystack_[0].value.as < std::vector<ASTExpr*> > (); }
#line 1800 "ramc_bison_parser.cpp"
    break;

  case 68:
#line 338 "ramc_grammar.yy"
                                      { yylhs.value.as < std::vector<ASTExpr*> > () = { yystack_[0].value.as < ASTExpr* > () }; }
#line 1806 "ramc_bison_parser.cpp"
    break;

  case 69:
#line 339 "ramc_grammar.yy"
                                      { yystack_[0].value.as < std::vector<ASTExpr*> > ().insert(yystack_[0].value.as < std::vector<ASTExpr*> > ().begin(), yystack_[2].value.as < ASTExpr* > ()); yylhs.value.as < std::vector<ASTExpr*> > () = yystack_[0].value.as < std::vector<ASTExpr*> > (); }
#line 1812 "ramc_bison_parser.cpp"
    break;

  case 70:
#line 343 "ramc_grammar.yy"
                                            { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 1818 "ramc_bison_parser.cpp"
    break;

  case 71:
#line 344 "ramc_grammar.yy"
                                            { yylhs.value.as < ASTExpr* > () = new ASTIfExpr(yystack_[4].value.as < ASTExpr* > (), yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), yystack_[5].value.as < Position > ()); }
#line 1824 "ramc_bison_parser.cpp"
    break;

  case 72:
#line 348 "ramc_grammar.yy"
                      { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::LOG_OR); }
#line 1830 "ramc_bison_parser.cpp"
    break;

  case 73:
#line 349 "ramc_grammar.yy"
                      { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 1836 "ramc_bison_parser.cpp"
    break;

  case 74:
#line 353 "ramc_grammar.yy"
                      { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::LOG_AND); }
#line 1842 "ramc_bison_parser.cpp"
    break;

  case 75:
#line 354 "ramc_grammar.yy"
                      { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 1848 "ramc_bison_parser.cpp"
    break;

  case 76:
#line 358 "ramc_grammar.yy"
                      { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::BIN_OR); }
#line 1854 "ramc_bison_parser.cpp"
    break;

  case 77:
#line 359 "ramc_grammar.yy"
                      { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 1860 "ramc_bison_parser.cpp"
    break;

  case 78:
#line 363 "ramc_grammar.yy"
                      { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::BIN_XOR); }
#line 1866 "ramc_bison_parser.cpp"
    break;

  case 79:
#line 364 "ramc_grammar.yy"
                      { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 1872 "ramc_bison_parser.cpp"
    break;

  case 80:
#line 368 "ramc_grammar.yy"
                      { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::BIN_AND); }
#line 1878 "ramc_bison_parser.cpp"
    break;

  case 81:
#line 369 "ramc_grammar.yy"
                      { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 1884 "ramc_bison_parser.cpp"
    break;

  case 82:
#line 373 "ramc_grammar.yy"
                       { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::EQ_EQ); }
#line 1890 "ramc_bison_parser.cpp"
    break;

  case 83:
#line 374 "ramc_grammar.yy"
                       { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::NEQ); }
#line 1896 "ramc_bison_parser.cpp"
    break;

  case 84:
#line 375 "ramc_grammar.yy"
                       { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 1902 "ramc_bison_parser.cpp"
    break;

  case 85:
#line 379 "ramc_grammar.yy"
                        { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::LT); }
#line 1908 "ramc_bison_parser.cpp"
    break;

  case 86:
#line 380 "ramc_grammar.yy"
                        { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::GT); }
#line 1914 "ramc_bison_parser.cpp"
    break;

  case 87:
#line 381 "ramc_grammar.yy"
                        { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::LT_EQ); }
#line 1920 "ramc_bison_parser.cpp"
    break;

  case 88:
#line 382 "ramc_grammar.yy"
                        { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::GT_EQ); }
#line 1926 "ramc_bison_parser.cpp"
    break;

  case 89:
#line 383 "ramc_grammar.yy"
                        { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 1932 "ramc_bison_parser.cpp"
    break;

  case 90:
#line 387 "ramc_grammar.yy"
                       { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::LSHIFT); }
#line 1938 "ramc_bison_parser.cpp"
    break;

  case 91:
#line 388 "ramc_grammar.yy"
                       { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::RSHIFT); }
#line 1944 "ramc_bison_parser.cpp"
    break;

  case 92:
#line 389 "ramc_grammar.yy"
                       { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 1950 "ramc_bison_parser.cpp"
    break;

  case 93:
#line 393 "ramc_grammar.yy"
                       { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::ADD); }
#line 1956 "ramc_bison_parser.cpp"
    break;

  case 94:
#line 394 "ramc_grammar.yy"
                       { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::SUB); }
#line 1962 "ramc_bison_parser.cpp"
    break;

  case 95:
#line 395 "ramc_grammar.yy"
                       { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 1968 "ramc_bison_parser.cpp"
    break;

  case 96:
#line 399 "ramc_grammar.yy"
                        { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::MUL); }
#line 1974 "ramc_bison_parser.cpp"
    break;

  case 97:
#line 400 "ramc_grammar.yy"
                        { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::DIV); }
#line 1980 "ramc_bison_parser.cpp"
    break;

  case 98:
#line 401 "ramc_grammar.yy"
                        { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::MOD); }
#line 1986 "ramc_bison_parser.cpp"
    break;

  case 99:
#line 402 "ramc_grammar.yy"
                        { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 1992 "ramc_bison_parser.cpp"
    break;

  case 100:
#line 406 "ramc_grammar.yy"
                 { yylhs.value.as < ASTExpr* > () = new ASTUnopExpr(yystack_[0].value.as < ASTExpr* > (), UnopType::NEG); }
#line 1998 "ramc_bison_parser.cpp"
    break;

  case 101:
#line 407 "ramc_grammar.yy"
                 { yylhs.value.as < ASTExpr* > () = new ASTUnopExpr(yystack_[0].value.as < ASTExpr* > (), UnopType::LOG_NOT); }
#line 2004 "ramc_bison_parser.cpp"
    break;

  case 102:
#line 408 "ramc_grammar.yy"
                 { yylhs.value.as < ASTExpr* > () = new ASTUnopExpr(yystack_[0].value.as < ASTExpr* > (), UnopType::BIN_NOT); }
#line 2010 "ramc_bison_parser.cpp"
    break;

  case 103:
#line 409 "ramc_grammar.yy"
                 { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 2016 "ramc_bison_parser.cpp"
    break;

  case 104:
#line 413 "ramc_grammar.yy"
                       { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::POW); }
#line 2022 "ramc_bison_parser.cpp"
    break;

  case 105:
#line 414 "ramc_grammar.yy"
                       { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 2028 "ramc_bison_parser.cpp"
    break;

  case 106:
#line 418 "ramc_grammar.yy"
                                { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTIntLit* > (); }
#line 2034 "ramc_bison_parser.cpp"
    break;

  case 107:
#line 419 "ramc_grammar.yy"
                                { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTFloatLit* > (); }
#line 2040 "ramc_bison_parser.cpp"
    break;

  case 108:
#line 420 "ramc_grammar.yy"
                                { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTByteLit* > (); }
#line 2046 "ramc_bison_parser.cpp"
    break;

  case 109:
#line 421 "ramc_grammar.yy"
                                { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTDoubleLit* > (); }
#line 2052 "ramc_bison_parser.cpp"
    break;

  case 110:
#line 422 "ramc_grammar.yy"
                                { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTLongLit* > (); }
#line 2058 "ramc_bison_parser.cpp"
    break;

  case 111:
#line 423 "ramc_grammar.yy"
                                { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTStringLit* > (); }
#line 2064 "ramc_bison_parser.cpp"
    break;

  case 112:
#line 424 "ramc_grammar.yy"
                                        { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTBoolLit* > (); }
#line 2070 "ramc_bison_parser.cpp"
    break;

  case 113:
#line 425 "ramc_grammar.yy"
                                        { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTBoolLit* > (); }
#line 2076 "ramc_bison_parser.cpp"
    break;

  case 114:
#line 426 "ramc_grammar.yy"
                                { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTIdentifier* > (); }
#line 2082 "ramc_bison_parser.cpp"
    break;

  case 115:
#line 427 "ramc_grammar.yy"
                                { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTFuncCallExpr* > (); }
#line 2088 "ramc_bison_parser.cpp"
    break;

  case 116:
#line 428 "ramc_grammar.yy"
                                { yylhs.value.as < ASTExpr* > () = yystack_[1].value.as < ASTExpr* > (); }
#line 2094 "ramc_bison_parser.cpp"
    break;

  case 117:
#line 429 "ramc_grammar.yy"
                                { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTArrayInit* > (); }
#line 2100 "ramc_bison_parser.cpp"
    break;

  case 118:
#line 430 "ramc_grammar.yy"
                        { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[3].value.as < ASTExpr* > (), yystack_[1].value.as < ASTExpr* > (), BinopType::INDEX); }
#line 2106 "ramc_bison_parser.cpp"
    break;

  case 119:
#line 433 "ramc_grammar.yy"
                                                        { yylhs.value.as < ASTIdentifier* > () = new ASTIdentifier(yystack_[0].value.as < std::pair<std::string, Position> > ().first, yystack_[0].value.as < std::pair<std::string, Position> > ().second); }
#line 2112 "ramc_bison_parser.cpp"
    break;

  case 120:
#line 434 "ramc_grammar.yy"
                                        { yylhs.value.as < ASTFuncCallExpr* > () = new ASTFuncCallExpr(yystack_[3].value.as < std::pair<std::string, Position> > ().first, yystack_[1].value.as < std::vector<ASTExpr*> > (), yystack_[3].value.as < std::pair<std::string, Position> > ().second); }
#line 2118 "ramc_bison_parser.cpp"
    break;

  case 121:
#line 435 "ramc_grammar.yy"
                                        { yylhs.value.as < ASTArrayInit* > () = new ASTArrayInit(yystack_[1].value.as < std::vector<ASTExpr*> > (), yystack_[2].value.as < Position > ()); }
#line 2124 "ramc_bison_parser.cpp"
    break;


#line 2128 "ramc_bison_parser.cpp"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yysyntax_error_ (yystack_[0].state, yyla));
      }


    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yy_error_token_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yy_error_token_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }


      // Shift the error token.
      yy_lac_discard_ ("error recovery");
      error_token.state = static_cast<state_type> (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  Parser::error (const syntax_error& yyexc)
  {
    error (yyexc.what ());
  }

  bool
  Parser::yy_lac_check_ (int yytoken) const
  {
    // Logically, the yylac_stack's lifetime is confined to this function.
    // Clear it, to get rid of potential left-overs from previous call.
    yylac_stack_.clear ();
    // Reduce until we encounter a shift and thereby accept the token.
#if YYDEBUG
    YYCDEBUG << "LAC: checking lookahead " << yytname_[yytoken] << ':';
#endif
    std::ptrdiff_t lac_top = 0;
    while (true)
      {
        state_type top_state = (yylac_stack_.empty ()
                                ? yystack_[lac_top].state
                                : yylac_stack_.back ());
        int yyrule = yypact_[top_state];
        if (yy_pact_value_is_default_ (yyrule)
            || (yyrule += yytoken) < 0 || yylast_ < yyrule
            || yycheck_[yyrule] != yytoken)
          {
            // Use the default action.
            yyrule = yydefact_[top_state];
            if (yyrule == 0)
              {
                YYCDEBUG << " Err\n";
                return false;
              }
          }
        else
          {
            // Use the action from yytable.
            yyrule = yytable_[yyrule];
            if (yy_table_value_is_error_ (yyrule))
              {
                YYCDEBUG << " Err\n";
                return false;
              }
            if (0 < yyrule)
              {
                YYCDEBUG << " S" << yyrule << '\n';
                return true;
              }
            yyrule = -yyrule;
          }
        // By now we know we have to simulate a reduce.
        YYCDEBUG << " R" << yyrule - 1;
        // Pop the corresponding number of values from the stack.
        {
          std::ptrdiff_t yylen = yyr2_[yyrule];
          // First pop from the LAC stack as many tokens as possible.
          std::ptrdiff_t lac_size = std::ptrdiff_t (yylac_stack_.size ());
          if (yylen < lac_size)
            {
              yylac_stack_.resize (std::size_t (lac_size - yylen));
              yylen = 0;
            }
          else if (lac_size)
            {
              yylac_stack_.clear ();
              yylen -= lac_size;
            }
          // Only afterwards look at the main stack.
          // We simulate popping elements by incrementing lac_top.
          lac_top += yylen;
        }
        // Keep top_state in sync with the updated stack.
        top_state = (yylac_stack_.empty ()
                     ? yystack_[lac_top].state
                     : yylac_stack_.back ());
        // Push the resulting state of the reduction.
        state_type state = yy_lr_goto_state_ (top_state, yyr1_[yyrule]);
        YYCDEBUG << " G" << state;
        yylac_stack_.push_back (state);
      }
  }

  // Establish the initial context if no initial context currently exists.
  bool
  Parser::yy_lac_establish_ (int yytoken)
  {
    /* Establish the initial context for the current lookahead if no initial
       context is currently established.

       We define a context as a snapshot of the parser stacks.  We define
       the initial context for a lookahead as the context in which the
       parser initially examines that lookahead in order to select a
       syntactic action.  Thus, if the lookahead eventually proves
       syntactically unacceptable (possibly in a later context reached via a
       series of reductions), the initial context can be used to determine
       the exact set of tokens that would be syntactically acceptable in the
       lookahead's place.  Moreover, it is the context after which any
       further semantic actions would be erroneous because they would be
       determined by a syntactically unacceptable token.

       yy_lac_establish_ should be invoked when a reduction is about to be
       performed in an inconsistent state (which, for the purposes of LAC,
       includes consistent states that don't know they're consistent because
       their default reductions have been disabled).

       For parse.lac=full, the implementation of yy_lac_establish_ is as
       follows.  If no initial context is currently established for the
       current lookahead, then check if that lookahead can eventually be
       shifted if syntactic actions continue from the current context.  */
    if (!yy_lac_established_)
      {
#if YYDEBUG
        YYCDEBUG << "LAC: initial context established for "
                 << yytname_[yytoken] << '\n';
#endif
        yy_lac_established_ = true;
        return yy_lac_check_ (yytoken);
      }
    return true;
  }

  // Discard any previous initial lookahead context.
  void
  Parser::yy_lac_discard_ (const char* evt)
  {
   /* Discard any previous initial lookahead context because of Event,
      which may be a lookahead change or an invalidation of the currently
      established initial context for the current lookahead.

      The most common example of a lookahead change is a shift.  An example
      of both cases is syntax error recovery.  That is, a syntax error
      occurs when the lookahead is syntactically erroneous for the
      currently established initial context, so error recovery manipulates
      the parser stacks to try to find a new initial context in which the
      current lookahead is syntactically acceptable.  If it fails to find
      such a context, it discards the lookahead.  */
    if (yy_lac_established_)
      {
        YYCDEBUG << "LAC: initial context discarded due to "
                 << evt << '\n';
        yy_lac_established_ = false;
      }
  }

  // Generate an error message.
  std::string
  Parser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    std::ptrdiff_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
         In the first two cases, it might appear that the current syntax
         error should have been detected in the previous state when
         yy_lac_check was invoked.  However, at that time, there might
         have been a different syntax error that discarded a different
         initial context during error recovery, leaving behind the
         current lookahead.
    */
    if (!yyla.empty ())
      {
        symbol_number_type yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];

#if YYDEBUG
        // Execute LAC once. We don't care if it is succesful, we
        // only do it for the sake of debugging output.
        if (!yy_lac_established_)
          yy_lac_check_ (yytoken);
#endif

        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            for (int yyx = 0; yyx < yyntokens_; ++yyx)
              if (yyx != yy_error_token_ && yyx != yy_undef_token_
                  && yy_lac_check_ (yyx))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const short Parser::yypact_ninf_ = -138;

  const signed char Parser::yytable_ninf_ = -1;

  const short
  Parser::yypact_[] =
  {
    -138,    13,   -40,  -138,   -10,    -4,  -138,    46,    -5,   -40,
       1,    85,    85,   153,  -138,    25,     1,  -138,   106,   106,
     106,    85,    85,    49,  -138,  -138,  -138,  -138,  -138,  -138,
    -138,  -138,   106,  -138,    69,    74,    89,    95,   100,    36,
      97,    51,    64,    28,  -138,   110,    75,  -138,  -138,  -138,
    -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,     3,    77,
    -138,  -138,  -138,  -138,    83,    78,    86,    85,    -7,   106,
     106,   106,   106,   106,   106,   106,   106,   106,   106,   106,
     106,   106,   106,   106,   106,   106,   106,   141,    85,    85,
      82,    90,  -138,    85,  -138,   108,  -138,    85,    74,    89,
      95,   100,    36,    97,    97,    51,    51,    51,    51,    64,
      64,    28,    28,  -138,  -138,  -138,    75,    91,  -138,   102,
      99,  -138,  -138,    81,  -138,  -138,   104,   114,  -138,   118,
      85,   153,   -13,    99,  -138,   112,   115,   153,  -138,   -23,
    -138,   142,   -18,  -138,   106,   106,   117,  -138,  -138,    85,
     125,  -138,  -138,  -138,  -138,  -138,  -138,  -138,   174,   115,
     153,    -1,   -14,     2,  -138,   -23,  -138,  -138,  -138,  -138,
    -138,  -138,  -138,  -138,  -138,  -138,  -138,  -138,    85,  -138,
    -138,   -23,  -138,    85,   153,   128,   136,  -138,  -138,   111,
     -23,   135,    12,   -23,  -138,   -23,  -138,   149,    85,  -138,
    -138,  -138,   106,   137,   -12,   150,  -138,   106,   -23,    -9,
    -138,  -138,   154,   -23,    31,  -138,   106,  -138,   155,     0,
      31,    31,  -138,   152,    31
  };

  const signed char
  Parser::yydefact_[] =
  {
       2,     0,     5,     1,     0,     0,     3,     0,     0,     5,
       7,     0,     0,     0,     6,     0,     7,     4,     0,     0,
       0,     0,     0,   119,   106,   111,   107,   108,   109,   110,
     112,   113,     0,    42,    70,    73,    75,    77,    79,    81,
      84,    89,    92,    95,    99,   103,   105,   114,   115,   117,
      40,    45,    46,    47,    48,    49,    50,    51,     0,     0,
       8,   100,   101,   102,     0,    68,     0,    66,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   116,     0,   121,     0,    67,     0,    72,    74,
      76,    78,    80,    82,    83,    85,    86,    87,    88,    90,
      91,    93,    94,    96,    97,    98,   104,     0,    41,     0,
      11,    69,   120,     0,   118,    52,     0,     0,    12,    13,
       0,     0,     0,     0,    71,    15,     0,     0,    14,     0,
      10,     0,    43,   119,     0,     0,     0,    28,    29,    66,
       0,    18,    19,    23,    24,    25,    22,    27,     0,     0,
       0,     0,     0,     0,    30,    16,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,     0,     9,
      44,     0,    32,     0,     0,     0,     0,    53,    20,    19,
       0,     0,     0,    16,    31,     0,    33,     0,     0,    17,
      21,    26,     0,     0,     0,     0,    37,     0,     0,     0,
      38,    34,     0,     0,     0,    35,     0,    39,     0,     0,
       0,     0,    36,     0,     0
  };

  const short
  Parser::yypgoto_[] =
  {
    -138,  -138,  -137,    27,   -51,  -138,    21,  -128,  -138,  -138,
    -138,   -11,   -28,   156,   157,   158,   151,   159,    18,   107,
      17,    19,    -2,  -138,   139,  -133,  -138,  -138,   215,   212,
      37,    84,   -19,  -138,    71,    -8,  -138,   101,  -138,  -138,
    -138,  -138,  -138,  -138,  -138,  -138
  };

  const short
  Parser::yydefgoto_[] =
  {
      -1,     1,   185,   151,   152,   153,   154,   155,    16,   156,
     157,    65,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,     6,    17,
     186,    95,    96,   178,   141,   142,   127,   128,   129,     2,
      10,   190,   213,   218,   208,   212
  };

  const unsigned char
  Parser::yytable_[] =
  {
      33,    50,   150,    66,    68,    58,   158,    69,   140,    69,
      64,   136,    69,     3,    69,   139,    61,    62,    63,    12,
      69,    69,    90,     5,     4,   143,   183,    89,   160,   137,
       5,   179,   158,    84,    85,    86,   198,     7,     8,     9,
      13,     4,   144,    90,   188,   145,   146,   184,   158,   147,
     148,   149,    90,   196,    74,    75,   182,   158,   206,    97,
     158,   211,   158,    80,    81,   181,   221,    82,    83,   139,
      11,   210,    15,    59,   121,   158,   215,   117,   118,   143,
     158,   158,   113,   114,   115,    67,   123,   158,   158,    18,
      69,   158,   103,   104,    70,     4,   216,   109,   110,    71,
     146,   111,   112,   147,   148,   149,    72,    19,    20,    73,
      18,    76,    77,    78,    79,    88,   161,   162,    87,   134,
      92,    21,    91,   135,    93,    22,   120,    94,    19,    20,
     189,   119,   124,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    21,   125,   201,   122,    22,   126,   130,   131,
      32,   132,    90,   139,    23,    24,    25,    26,    27,    28,
      29,    30,    31,   217,   133,   163,   159,   187,   165,   222,
     223,   193,   191,   201,   204,   194,   192,    21,   195,   209,
     197,    22,   205,   105,   106,   107,   108,   203,   219,    23,
      24,    25,    26,    27,    28,    29,    30,    31,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
      51,    52,    53,    54,    55,    56,    57,   202,   207,   224,
     214,   220,   200,   101,    14,    98,   116,    99,    60,   100,
     199,   180,   102,   164,   138
  };

  const unsigned char
  Parser::yycheck_[] =
  {
      11,    12,   139,    22,    32,    13,   139,    21,   136,    21,
      21,    24,    21,     0,    21,    38,    18,    19,    20,    24,
      21,    21,    40,     2,    64,    48,    24,    24,    46,    42,
       9,   159,   165,     5,     6,     7,    24,    47,    48,    43,
      45,    64,    65,    40,   181,    68,    69,    45,   181,    72,
      73,    74,    40,   190,    18,    19,    70,   190,    70,    66,
     193,    70,   195,    12,    13,    66,    66,     3,     4,    38,
      24,   208,    71,    48,    93,   208,   213,    88,    89,    48,
     213,   214,    84,    85,    86,    36,    97,   220,   221,     4,
      21,   224,    74,    75,    20,    64,    65,    80,    81,    10,
      69,    82,    83,    72,    73,    74,    11,    22,    23,     9,
       4,    14,    15,    16,    17,    40,   144,   145,     8,   130,
      37,    36,    45,   131,    46,    40,    36,    41,    22,    23,
     181,    49,    41,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    36,    41,   195,    37,    40,    48,    67,    45,
      65,    37,    40,    38,    48,    49,    50,    51,    52,    53,
      54,    55,    56,   214,    46,    48,    24,   178,    43,   220,
     221,    43,   183,   224,   202,    39,   184,    36,    67,   207,
      45,    40,    45,    76,    77,    78,    79,   198,   216,    48,
      49,    50,    51,    52,    53,    54,    55,    56,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      57,    58,    59,    60,    61,    62,    63,    68,    68,    67,
      66,    66,   195,    72,     9,    69,    87,    70,    16,    71,
     193,   160,    73,   149,   133
  };

  const signed char
  Parser::yystos_[] =
  {
       0,    76,   114,     0,    64,    81,   103,    47,    48,    43,
     115,    24,    24,    45,   103,    71,    83,   104,     4,    22,
      23,    36,    40,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    65,    86,    87,    88,    89,    90,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,   101,   102,
      86,    57,    58,    59,    60,    61,    62,    63,   110,    48,
     104,    97,    97,    97,    86,    86,   107,    36,    87,    21,
      20,    10,    11,     9,    18,    19,    14,    15,    16,    17,
      12,    13,     3,     4,     5,     6,     7,     8,    40,    24,
      40,    45,    37,    46,    41,   106,   107,    66,    88,    89,
      90,    91,    92,    93,    93,    94,    94,    94,    94,    95,
      95,    96,    96,    97,    97,    97,    99,    86,    86,    49,
      36,   107,    37,    86,    41,    41,    48,   111,   112,   113,
      67,    45,    37,    46,    86,   110,    24,    42,   112,    38,
      82,   109,   110,    48,    65,    68,    69,    72,    73,    74,
      77,    78,    79,    80,    81,    82,    84,    85,   100,    24,
      46,    87,    87,    48,   106,    43,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,   108,    82,
     109,    66,    70,    24,    45,    77,   105,    86,    77,    79,
     116,    86,   110,    43,    39,    67,    77,    45,    24,   105,
      78,    79,    68,    86,    87,    45,    70,    68,   119,    87,
      77,    70,   120,   117,    66,    77,    65,    79,   118,    87,
      66,    66,    79,    79,    67
  };

  const signed char
  Parser::yyr1_[] =
  {
       0,    75,   114,   115,    76,   103,   103,   104,   104,    83,
      83,   111,   111,   112,   112,   113,   105,   105,    77,    77,
      78,    78,    78,    79,    79,    79,    79,    79,    79,    79,
      79,    82,   116,    84,   117,   118,    85,   119,   120,    85,
      81,    81,    81,   109,   109,   110,   110,   110,   110,   110,
     110,   110,   110,    80,   108,   108,   108,   108,   108,   108,
     108,   108,   108,   108,   108,   108,   106,   106,   107,   107,
      86,    86,    87,    87,    88,    88,    89,    89,    90,    90,
      91,    91,    92,    92,    92,    93,    93,    93,    93,    93,
      94,    94,    94,    95,    95,    95,    96,    96,    96,    96,
      97,    97,    97,    97,    98,    98,    99,    99,    99,    99,
      99,    99,    99,    99,    99,    99,    99,    99,    99,   100,
     101,   102
  };

  const signed char
  Parser::yyr2_[] =
  {
       0,     2,     0,     0,     4,     0,     3,     0,     2,    10,
       8,     0,     1,     1,     3,     3,     0,     3,     1,     1,
       4,     6,     1,     1,     1,     1,     6,     1,     1,     1,
       2,     5,     0,     5,     0,     0,    15,     0,     0,    13,
       4,     6,     4,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     4,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     1,     1,     3,
       1,     6,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     3,     1,     3,     3,     3,     3,     1,
       3,     3,     1,     3,     3,     1,     3,     3,     3,     1,
       2,     2,     2,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     3,     1,     4,     1,
       4,     3
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const Parser::yytname_[] =
  {
  "END_OF_FILE", "error", "$undefined", "\"+\"", "\"-\"", "\"*\"",
  "\"/\"", "\"%\"", "\"**\"", "\"&\"", "\"|\"", "\"^\"", "\"<<\"",
  "\">>\"", "\"<\"", "\">\"", "\"<=\"", "\">=\"", "\"==\"", "\"!=\"",
  "\"&&\"", "\"||\"", "\"!\"", "\"~\"", "\"=\"", "\"+=\"", "\"-=\"",
  "\"*=\"", "\"/=\"", "\"%=\"", "\"**=\"", "\"&=\"", "\"|=\"", "\"^=\"",
  "\"<<=\"", "\">>=\"", "\"(\"", "\")\"", "\"{\"", "\"}\"", "\"[\"",
  "\"]\"", "\"->\"", "\";\"", "\".\"", "\":\"", "\",\"", "\"_\"", "\"ID\"",
  "\"INT_LIT\"", "\"STRING_LIT\"", "\"FLOAT_LIT\"", "\"BYTE_LIT\"",
  "\"DOUBLE_LIT\"", "\"LONG_LIT\"", "\"true\"", "\"false\"", "\"byte\"",
  "\"bool\"", "\"int\"", "\"float\"", "\"double\"", "\"long\"",
  "\"string\"", "\"let\"", "\"if\"", "\"then\"", "\"else\"", "\"while\"",
  "\"for\"", "\"do\"", "\"func\"", "\"break\"", "\"continue\"",
  "\"return\"", "$accept", "PROGRAM", "STMT", "OPEN_STMT", "CLOSED_STMT",
  "ASSIGNMENT", "VARDECL", "BLOCK", "FUNCDECL", "WHILE_STMT", "FOR_STMT",
  "EXPR", "EXPR1", "EXPR2", "EXPR3", "EXPR4", "EXPR5", "EXPR6", "EXPR7",
  "EXPR8", "EXPR9", "EXPR10", "EXPR11", "EXPR12", "ATOM", "IDENTIFIER",
  "FUNC_CALL", "ARRAY_INIT", "VARDECLS", "TL_FUNCDECLS", "STMTS",
  "EXPR_STAR", "EXPR_PLUS", "OP_ASSIGN", "TYPE_PLUS", "TYPE", "PARAM_STAR",
  "PARAM_PLUS", "PARAM", "$@1", "$@2", "$@3", "$@4", "$@5", "$@6", "$@7", YY_NULLPTR
  };

#if YYDEBUG
  const short
  Parser::yyrline_[] =
  {
       0,   245,   245,   245,   245,   247,   248,   251,   252,   255,
     256,   258,   259,   262,   263,   266,   268,   269,   272,   273,
     276,   277,   278,   281,   282,   283,   284,   285,   286,   287,
     288,   291,   293,   293,   295,   295,   295,   296,   296,   296,
     298,   299,   300,   303,   304,   308,   309,   310,   311,   312,
     313,   314,   315,   318,   321,   322,   323,   324,   325,   326,
     327,   328,   329,   330,   331,   332,   334,   335,   338,   339,
     343,   344,   348,   349,   353,   354,   358,   359,   363,   364,
     368,   369,   373,   374,   375,   379,   380,   381,   382,   383,
     387,   388,   389,   393,   394,   395,   399,   400,   401,   402,
     406,   407,   408,   409,   413,   414,   418,   419,   420,   421,
     422,   423,   424,   425,   426,   427,   428,   429,   430,   433,
     434,   435
  };

  // Print the state stack on the debug stream.
  void
  Parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  Parser::yy_reduce_print_ (int yyrule)
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


#line 8 "ramc_grammar.yy"
} } // ramc::bison
#line 2814 "ramc_bison_parser.cpp"

#line 437 "ramc_grammar.yy"

namespace ramc {
	namespace bison {
		// Report an error to the user.
		auto Parser::error(const std::string& _msg) -> void
		{
			ASSERT_MSG(false, _msg);
		}
	}
}

