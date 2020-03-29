// A Bison parser, made by GNU Bison 3.5.

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
		bool IsInLoop;

      // Return the next token.
		auto yylex(Lexer& _lexer, ASTNode*& _result, Position& _pos) -> Parser::symbol_type
		{
			LexerResult readRes = _lexer.GetNextToken();

			if (!readRes.IsSuccess())
				throw std::runtime_error(readRes.ToString(false));

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

				case TokenType::KW_BYTE: { TypePtr type = Type::BYTE; return Parser::make_KW_BYTE(type); }
				case TokenType::KW_BOOL: { TypePtr type = Type::BOOL; return Parser::make_KW_BOOL(type); }
				case TokenType::KW_INT: { TypePtr type = Type::INT; return Parser::make_KW_INT(type); }
				case TokenType::KW_FLOAT: { TypePtr type = Type::FLOAT; return Parser::make_KW_FLOAT(type); }
				case TokenType::KW_DOUBLE: { TypePtr type = Type::DOUBLE; return Parser::make_KW_DOUBLE(type); }
				case TokenType::KW_LONG: { TypePtr type = Type::LONG; return Parser::make_KW_LONG(type); }
				case TokenType::KW_STRING: { TypePtr type = Type::STRING; return Parser::make_KW_STRING(type); }
				case TokenType::KW_VOID: { TypePtr type = Type::VOID; return Parser::make_KW_VOID(type); }

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
				case TokenType::LSBRACKET: return Parser::make_LSBRACKET();
				case TokenType::RSBRACKET: return Parser::make_RSBRACKET();
				case TokenType::SEMICOLON: return Parser::make_SEMICOLON();
				case TokenType::PERIOD: return Parser::make_PERIOD();
				case TokenType::COLON: return Parser::make_COLON();
				case TokenType::COMMA: return Parser::make_COMMA();
				case TokenType::GOES_TO: return Parser::make_GOES_TO();
				case TokenType::END_OF_FILE: return Parser::make_END_OF_FILE();
				default: throw std::runtime_error(token.ToString(true) + " is not parasble!");
			}
		}
    }
  }

#line 155 "ramc_bison_parser.cpp"


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
#line 228 "ramc_bison_parser.cpp"


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
      case 81: // ASSIGNMENT
        value.YY_MOVE_OR_COPY< ASTAssignment* > (YY_MOVE (that.value));
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

      case 77: // PROGRAM
        value.YY_MOVE_OR_COPY< ASTProgram* > (YY_MOVE (that.value));
        break;

      case 78: // STMT
      case 79: // OPEN_STMT
      case 80: // CLOSED_STMT
      case 84: // WHILE_STMT
      case 85: // FOR_STMT
        value.YY_MOVE_OR_COPY< ASTStmt* > (YY_MOVE (that.value));
        break;

      case 50: // "STRING_LIT"
        value.YY_MOVE_OR_COPY< ASTStringLit* > (YY_MOVE (that.value));
        break;

      case 82: // VARDECL
        value.YY_MOVE_OR_COPY< ASTVarDecl* > (YY_MOVE (that.value));
        break;

      case 106: // OP_ASSIGN
        value.YY_MOVE_OR_COPY< AssignmentType > (YY_MOVE (that.value));
        break;

      case 111: // PARAM
        value.YY_MOVE_OR_COPY< Param > (YY_MOVE (that.value));
        break;

      case 38: // "{"
      case 65: // "let"
      case 66: // "if"
      case 67: // "then"
      case 68: // "else"
      case 69: // "while"
      case 70: // "for"
      case 71: // "do"
      case 72: // "func"
      case 73: // "break"
      case 74: // "continue"
      case 75: // "return"
        value.YY_MOVE_OR_COPY< Position > (YY_MOVE (that.value));
        break;

      case 57: // "byte"
      case 58: // "bool"
      case 59: // "int"
      case 60: // "float"
      case 61: // "double"
      case 62: // "long"
      case 63: // "string"
      case 64: // "void"
      case 108: // TYPE
        value.YY_MOVE_OR_COPY< TypePtr > (YY_MOVE (that.value));
        break;

      case 48: // "ID"
        value.YY_MOVE_OR_COPY< std::pair<std::string, Position> > (YY_MOVE (that.value));
        break;

      case 104: // EXPR_STAR
      case 105: // EXPR_PLUS
        value.YY_MOVE_OR_COPY< std::vector<ASTExpr*> > (YY_MOVE (that.value));
        break;

      case 102: // TL_FUNCDECLS
        value.YY_MOVE_OR_COPY< std::vector<ASTFuncDecl*> > (YY_MOVE (that.value));
        break;

      case 103: // STMTS
        value.YY_MOVE_OR_COPY< std::vector<ASTStmt*> > (YY_MOVE (that.value));
        break;

      case 101: // TL_VARDECLS
        value.YY_MOVE_OR_COPY< std::vector<ASTVarDecl*> > (YY_MOVE (that.value));
        break;

      case 109: // PARAM_STAR
      case 110: // PARAM_PLUS
        value.YY_MOVE_OR_COPY< std::vector<Param> > (YY_MOVE (that.value));
        break;

      case 107: // TYPE_PLUS
        value.YY_MOVE_OR_COPY< std::vector<TypePtr> > (YY_MOVE (that.value));
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
      case 81: // ASSIGNMENT
        value.move< ASTAssignment* > (YY_MOVE (that.value));
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

      case 77: // PROGRAM
        value.move< ASTProgram* > (YY_MOVE (that.value));
        break;

      case 78: // STMT
      case 79: // OPEN_STMT
      case 80: // CLOSED_STMT
      case 84: // WHILE_STMT
      case 85: // FOR_STMT
        value.move< ASTStmt* > (YY_MOVE (that.value));
        break;

      case 50: // "STRING_LIT"
        value.move< ASTStringLit* > (YY_MOVE (that.value));
        break;

      case 82: // VARDECL
        value.move< ASTVarDecl* > (YY_MOVE (that.value));
        break;

      case 106: // OP_ASSIGN
        value.move< AssignmentType > (YY_MOVE (that.value));
        break;

      case 111: // PARAM
        value.move< Param > (YY_MOVE (that.value));
        break;

      case 38: // "{"
      case 65: // "let"
      case 66: // "if"
      case 67: // "then"
      case 68: // "else"
      case 69: // "while"
      case 70: // "for"
      case 71: // "do"
      case 72: // "func"
      case 73: // "break"
      case 74: // "continue"
      case 75: // "return"
        value.move< Position > (YY_MOVE (that.value));
        break;

      case 57: // "byte"
      case 58: // "bool"
      case 59: // "int"
      case 60: // "float"
      case 61: // "double"
      case 62: // "long"
      case 63: // "string"
      case 64: // "void"
      case 108: // TYPE
        value.move< TypePtr > (YY_MOVE (that.value));
        break;

      case 48: // "ID"
        value.move< std::pair<std::string, Position> > (YY_MOVE (that.value));
        break;

      case 104: // EXPR_STAR
      case 105: // EXPR_PLUS
        value.move< std::vector<ASTExpr*> > (YY_MOVE (that.value));
        break;

      case 102: // TL_FUNCDECLS
        value.move< std::vector<ASTFuncDecl*> > (YY_MOVE (that.value));
        break;

      case 103: // STMTS
        value.move< std::vector<ASTStmt*> > (YY_MOVE (that.value));
        break;

      case 101: // TL_VARDECLS
        value.move< std::vector<ASTVarDecl*> > (YY_MOVE (that.value));
        break;

      case 109: // PARAM_STAR
      case 110: // PARAM_PLUS
        value.move< std::vector<Param> > (YY_MOVE (that.value));
        break;

      case 107: // TYPE_PLUS
        value.move< std::vector<TypePtr> > (YY_MOVE (that.value));
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
      case 81: // ASSIGNMENT
        value.copy< ASTAssignment* > (that.value);
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

      case 77: // PROGRAM
        value.copy< ASTProgram* > (that.value);
        break;

      case 78: // STMT
      case 79: // OPEN_STMT
      case 80: // CLOSED_STMT
      case 84: // WHILE_STMT
      case 85: // FOR_STMT
        value.copy< ASTStmt* > (that.value);
        break;

      case 50: // "STRING_LIT"
        value.copy< ASTStringLit* > (that.value);
        break;

      case 82: // VARDECL
        value.copy< ASTVarDecl* > (that.value);
        break;

      case 106: // OP_ASSIGN
        value.copy< AssignmentType > (that.value);
        break;

      case 111: // PARAM
        value.copy< Param > (that.value);
        break;

      case 38: // "{"
      case 65: // "let"
      case 66: // "if"
      case 67: // "then"
      case 68: // "else"
      case 69: // "while"
      case 70: // "for"
      case 71: // "do"
      case 72: // "func"
      case 73: // "break"
      case 74: // "continue"
      case 75: // "return"
        value.copy< Position > (that.value);
        break;

      case 57: // "byte"
      case 58: // "bool"
      case 59: // "int"
      case 60: // "float"
      case 61: // "double"
      case 62: // "long"
      case 63: // "string"
      case 64: // "void"
      case 108: // TYPE
        value.copy< TypePtr > (that.value);
        break;

      case 48: // "ID"
        value.copy< std::pair<std::string, Position> > (that.value);
        break;

      case 104: // EXPR_STAR
      case 105: // EXPR_PLUS
        value.copy< std::vector<ASTExpr*> > (that.value);
        break;

      case 102: // TL_FUNCDECLS
        value.copy< std::vector<ASTFuncDecl*> > (that.value);
        break;

      case 103: // STMTS
        value.copy< std::vector<ASTStmt*> > (that.value);
        break;

      case 101: // TL_VARDECLS
        value.copy< std::vector<ASTVarDecl*> > (that.value);
        break;

      case 109: // PARAM_STAR
      case 110: // PARAM_PLUS
        value.copy< std::vector<Param> > (that.value);
        break;

      case 107: // TYPE_PLUS
        value.copy< std::vector<TypePtr> > (that.value);
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
      case 81: // ASSIGNMENT
        value.move< ASTAssignment* > (that.value);
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

      case 77: // PROGRAM
        value.move< ASTProgram* > (that.value);
        break;

      case 78: // STMT
      case 79: // OPEN_STMT
      case 80: // CLOSED_STMT
      case 84: // WHILE_STMT
      case 85: // FOR_STMT
        value.move< ASTStmt* > (that.value);
        break;

      case 50: // "STRING_LIT"
        value.move< ASTStringLit* > (that.value);
        break;

      case 82: // VARDECL
        value.move< ASTVarDecl* > (that.value);
        break;

      case 106: // OP_ASSIGN
        value.move< AssignmentType > (that.value);
        break;

      case 111: // PARAM
        value.move< Param > (that.value);
        break;

      case 38: // "{"
      case 65: // "let"
      case 66: // "if"
      case 67: // "then"
      case 68: // "else"
      case 69: // "while"
      case 70: // "for"
      case 71: // "do"
      case 72: // "func"
      case 73: // "break"
      case 74: // "continue"
      case 75: // "return"
        value.move< Position > (that.value);
        break;

      case 57: // "byte"
      case 58: // "bool"
      case 59: // "int"
      case 60: // "float"
      case 61: // "double"
      case 62: // "long"
      case 63: // "string"
      case 64: // "void"
      case 108: // TYPE
        value.move< TypePtr > (that.value);
        break;

      case 48: // "ID"
        value.move< std::pair<std::string, Position> > (that.value);
        break;

      case 104: // EXPR_STAR
      case 105: // EXPR_PLUS
        value.move< std::vector<ASTExpr*> > (that.value);
        break;

      case 102: // TL_FUNCDECLS
        value.move< std::vector<ASTFuncDecl*> > (that.value);
        break;

      case 103: // STMTS
        value.move< std::vector<ASTStmt*> > (that.value);
        break;

      case 101: // TL_VARDECLS
        value.move< std::vector<ASTVarDecl*> > (that.value);
        break;

      case 109: // PARAM_STAR
      case 110: // PARAM_PLUS
        value.move< std::vector<Param> > (that.value);
        break;

      case 107: // TYPE_PLUS
        value.move< std::vector<TypePtr> > (that.value);
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
      case 81: // ASSIGNMENT
        yylhs.value.emplace< ASTAssignment* > ();
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

      case 77: // PROGRAM
        yylhs.value.emplace< ASTProgram* > ();
        break;

      case 78: // STMT
      case 79: // OPEN_STMT
      case 80: // CLOSED_STMT
      case 84: // WHILE_STMT
      case 85: // FOR_STMT
        yylhs.value.emplace< ASTStmt* > ();
        break;

      case 50: // "STRING_LIT"
        yylhs.value.emplace< ASTStringLit* > ();
        break;

      case 82: // VARDECL
        yylhs.value.emplace< ASTVarDecl* > ();
        break;

      case 106: // OP_ASSIGN
        yylhs.value.emplace< AssignmentType > ();
        break;

      case 111: // PARAM
        yylhs.value.emplace< Param > ();
        break;

      case 38: // "{"
      case 65: // "let"
      case 66: // "if"
      case 67: // "then"
      case 68: // "else"
      case 69: // "while"
      case 70: // "for"
      case 71: // "do"
      case 72: // "func"
      case 73: // "break"
      case 74: // "continue"
      case 75: // "return"
        yylhs.value.emplace< Position > ();
        break;

      case 57: // "byte"
      case 58: // "bool"
      case 59: // "int"
      case 60: // "float"
      case 61: // "double"
      case 62: // "long"
      case 63: // "string"
      case 64: // "void"
      case 108: // TYPE
        yylhs.value.emplace< TypePtr > ();
        break;

      case 48: // "ID"
        yylhs.value.emplace< std::pair<std::string, Position> > ();
        break;

      case 104: // EXPR_STAR
      case 105: // EXPR_PLUS
        yylhs.value.emplace< std::vector<ASTExpr*> > ();
        break;

      case 102: // TL_FUNCDECLS
        yylhs.value.emplace< std::vector<ASTFuncDecl*> > ();
        break;

      case 103: // STMTS
        yylhs.value.emplace< std::vector<ASTStmt*> > ();
        break;

      case 101: // TL_VARDECLS
        yylhs.value.emplace< std::vector<ASTVarDecl*> > ();
        break;

      case 109: // PARAM_STAR
      case 110: // PARAM_PLUS
        yylhs.value.emplace< std::vector<Param> > ();
        break;

      case 107: // TYPE_PLUS
        yylhs.value.emplace< std::vector<TypePtr> > ();
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
#line 244 "ramc_grammar.yy"
                                  { result = new ASTProgram("Test File", yystack_[1].value.as < std::vector<ASTVarDecl*> > (), yystack_[0].value.as < std::vector<ASTFuncDecl*> > ()); }
#line 1351 "ramc_bison_parser.cpp"
    break;

  case 3:
#line 246 "ramc_grammar.yy"
                                                                                                { yylhs.value.as < std::vector<ASTVarDecl*> > () = { }; }
#line 1357 "ramc_bison_parser.cpp"
    break;

  case 4:
#line 247 "ramc_grammar.yy"
                                                                        { yystack_[5].value.as < std::vector<ASTVarDecl*> > ().push_back(new ASTVarDecl(yystack_[3].value.as < ASTIdentifier* > (), yystack_[1].value.as < ASTExpr* > (), yystack_[4].value.as < Position > ())); yylhs.value.as < std::vector<ASTVarDecl*> > () = yystack_[5].value.as < std::vector<ASTVarDecl*> > (); }
#line 1363 "ramc_bison_parser.cpp"
    break;

  case 5:
#line 248 "ramc_grammar.yy"
                                                                        { yystack_[7].value.as < std::vector<ASTVarDecl*> > ().push_back(new ASTVarDecl(yystack_[5].value.as < ASTIdentifier* > (), yystack_[3].value.as < TypePtr > (), yystack_[1].value.as < ASTExpr* > (), yystack_[6].value.as < Position > ())); yylhs.value.as < std::vector<ASTVarDecl*> > () = yystack_[7].value.as < std::vector<ASTVarDecl*> > (); }
#line 1369 "ramc_bison_parser.cpp"
    break;

  case 6:
#line 251 "ramc_grammar.yy"
                                                { yylhs.value.as < std::vector<ASTFuncDecl*> > () = { }; }
#line 1375 "ramc_bison_parser.cpp"
    break;

  case 7:
#line 252 "ramc_grammar.yy"
                                                { yystack_[1].value.as < std::vector<ASTFuncDecl*> > ().push_back(yystack_[0].value.as < ASTFuncDecl* > ()); yylhs.value.as < std::vector<ASTFuncDecl*> > () = yystack_[1].value.as < std::vector<ASTFuncDecl*> > (); }
#line 1381 "ramc_bison_parser.cpp"
    break;

  case 8:
#line 255 "ramc_grammar.yy"
                                                                     { yylhs.value.as < ASTFuncDecl* > () = new ASTFuncDecl(yystack_[8].value.as < std::pair<std::string, Position> > ().first, yystack_[5].value.as < std::vector<Param> > (), yystack_[2].value.as < std::vector<TypePtr> > (), yystack_[0].value.as < ASTStmt* > (), yystack_[9].value.as < Position > ()); }
#line 1387 "ramc_bison_parser.cpp"
    break;

  case 9:
#line 256 "ramc_grammar.yy"
                                                                                         { yylhs.value.as < ASTFuncDecl* > () = new ASTFuncDecl(yystack_[6].value.as < std::pair<std::string, Position> > ().first, yystack_[3].value.as < std::vector<Param> > (), { }, yystack_[0].value.as < ASTStmt* > (), yystack_[7].value.as < Position > ()); }
#line 1393 "ramc_bison_parser.cpp"
    break;

  case 10:
#line 258 "ramc_grammar.yy"
                           { yylhs.value.as < std::vector<Param> > () = { }; }
#line 1399 "ramc_bison_parser.cpp"
    break;

  case 11:
#line 259 "ramc_grammar.yy"
                               { yylhs.value.as < std::vector<Param> > () = yystack_[0].value.as < std::vector<Param> > (); }
#line 1405 "ramc_bison_parser.cpp"
    break;

  case 12:
#line 262 "ramc_grammar.yy"
                                             { yylhs.value.as < std::vector<Param> > () = { yystack_[0].value.as < Param > () }; }
#line 1411 "ramc_bison_parser.cpp"
    break;

  case 13:
#line 263 "ramc_grammar.yy"
                                         { yystack_[2].value.as < std::vector<Param> > ().push_back(yystack_[0].value.as < Param > ()); yylhs.value.as < std::vector<Param> > () = yystack_[2].value.as < std::vector<Param> > (); }
#line 1417 "ramc_bison_parser.cpp"
    break;

  case 14:
#line 266 "ramc_grammar.yy"
                     { yylhs.value.as < Param > () = { yystack_[2].value.as < std::pair<std::string, Position> > ().first, yystack_[0].value.as < TypePtr > (), yystack_[2].value.as < std::pair<std::string, Position> > ().second }; }
#line 1423 "ramc_bison_parser.cpp"
    break;

  case 15:
#line 268 "ramc_grammar.yy"
                          { yylhs.value.as < std::vector<ASTStmt*> > () = { }; }
#line 1429 "ramc_bison_parser.cpp"
    break;

  case 16:
#line 269 "ramc_grammar.yy"
                          { yystack_[2].value.as < std::vector<ASTStmt*> > ().push_back(yystack_[1].value.as < ASTStmt* > ()); yylhs.value.as < std::vector<ASTStmt*> > () = yystack_[2].value.as < std::vector<ASTStmt*> > (); }
#line 1435 "ramc_bison_parser.cpp"
    break;

  case 17:
#line 272 "ramc_grammar.yy"
                  { yylhs.value.as < ASTStmt* > () = yystack_[0].value.as < ASTStmt* > (); }
#line 1441 "ramc_bison_parser.cpp"
    break;

  case 18:
#line 273 "ramc_grammar.yy"
                  { yylhs.value.as < ASTStmt* > () = yystack_[0].value.as < ASTStmt* > (); }
#line 1447 "ramc_bison_parser.cpp"
    break;

  case 19:
#line 276 "ramc_grammar.yy"
                                                                                { yylhs.value.as < ASTStmt* > () = new ASTIfStmt(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTStmt* > (), nullptr, yystack_[3].value.as < Position > ()); }
#line 1453 "ramc_bison_parser.cpp"
    break;

  case 20:
#line 277 "ramc_grammar.yy"
                                                                        { yylhs.value.as < ASTStmt* > () = new ASTIfStmt(yystack_[4].value.as < ASTExpr* > (), yystack_[2].value.as < ASTStmt* > (), yystack_[0].value.as < ASTStmt* > (), yystack_[5].value.as < Position > ()); }
#line 1459 "ramc_bison_parser.cpp"
    break;

  case 21:
#line 278 "ramc_grammar.yy"
                                                                                                        { yylhs.value.as < ASTStmt* > () = yystack_[0].value.as < ASTStmt* > (); }
#line 1465 "ramc_bison_parser.cpp"
    break;

  case 22:
#line 281 "ramc_grammar.yy"
                                                                                                        { yylhs.value.as < ASTStmt* > () = yystack_[0].value.as < ASTAssignment* > (); }
#line 1471 "ramc_bison_parser.cpp"
    break;

  case 23:
#line 282 "ramc_grammar.yy"
                                                                                                                { yylhs.value.as < ASTStmt* > () = yystack_[0].value.as < ASTVarDecl* > (); }
#line 1477 "ramc_bison_parser.cpp"
    break;

  case 24:
#line 283 "ramc_grammar.yy"
                                                                                                        { std::vector<ASTStmt*> instrs = { yystack_[3].value.as < ASTStmt* > () }; instrs.insert(instrs.end(), yystack_[1].value.as < std::vector<ASTStmt*> > ().begin(), yystack_[1].value.as < std::vector<ASTStmt*> > ().end()); yylhs.value.as < ASTStmt* > () = new ASTBlock(instrs, yystack_[4].value.as < Position > ());}
#line 1483 "ramc_bison_parser.cpp"
    break;

  case 25:
#line 284 "ramc_grammar.yy"
                                                                        { yylhs.value.as < ASTStmt* > () = new ASTIfStmt(yystack_[4].value.as < ASTExpr* > (), yystack_[2].value.as < ASTStmt* > (), yystack_[0].value.as < ASTStmt* > (), yystack_[5].value.as < Position > ()); }
#line 1489 "ramc_bison_parser.cpp"
    break;

  case 26:
#line 285 "ramc_grammar.yy"
                                                                                                                { yylhs.value.as < ASTStmt* > () = yystack_[0].value.as < ASTStmt* > (); }
#line 1495 "ramc_bison_parser.cpp"
    break;

  case 27:
#line 286 "ramc_grammar.yy"
                                                                                                                { IsInLoop ? yylhs.value.as < ASTStmt* > () = new ASTBreakContinueStmt(true, yystack_[0].value.as < Position > ()) : throw std::runtime_error("Cannot have 'break' outside of a loop!"); }
#line 1501 "ramc_bison_parser.cpp"
    break;

  case 28:
#line 287 "ramc_grammar.yy"
                                                                                                                { IsInLoop ? yylhs.value.as < ASTStmt* > () = new ASTBreakContinueStmt(false, yystack_[0].value.as < Position > ()) : throw std::runtime_error("Cannot have 'continue' outside of a loop!"); }
#line 1507 "ramc_bison_parser.cpp"
    break;

  case 29:
#line 288 "ramc_grammar.yy"
                                                                                                        { yylhs.value.as < ASTStmt* > () = new ASTReturnStmt(yystack_[0].value.as < std::vector<ASTExpr*> > (), yystack_[1].value.as < Position > ()); }
#line 1513 "ramc_bison_parser.cpp"
    break;

  case 30:
#line 291 "ramc_grammar.yy"
                               { IsInLoop = true; }
#line 1519 "ramc_bison_parser.cpp"
    break;

  case 31:
#line 291 "ramc_grammar.yy"
                                                         { IsInLoop = false; yylhs.value.as < ASTStmt* > () = new ASTWhileStmt(yystack_[3].value.as < ASTExpr* > (), yystack_[0].value.as < ASTStmt* > (), yystack_[4].value.as < Position > ()); }
#line 1525 "ramc_bison_parser.cpp"
    break;

  case 32:
#line 293 "ramc_grammar.yy"
                                                                    { IsInLoop = true; }
#line 1531 "ramc_bison_parser.cpp"
    break;

  case 33:
#line 293 "ramc_grammar.yy"
                                                                                              { IsInLoop = false; }
#line 1537 "ramc_bison_parser.cpp"
    break;

  case 34:
#line 293 "ramc_grammar.yy"
                                                                                                                                       { yylhs.value.as < ASTStmt* > () = new ASTForStmt(new ASTVarDecl(yystack_[13].value.as < ASTIdentifier* > (), yystack_[11].value.as < TypePtr > (), yystack_[9].value.as < ASTExpr* > (), yystack_[14].value.as < Position > ()), yystack_[6].value.as < ASTExpr* > (), yystack_[3].value.as < ASTStmt* > (), yystack_[0].value.as < ASTStmt* > (), yystack_[14].value.as < Position > ()); }
#line 1543 "ramc_bison_parser.cpp"
    break;

  case 35:
#line 294 "ramc_grammar.yy"
                                                                   { IsInLoop = true; }
#line 1549 "ramc_bison_parser.cpp"
    break;

  case 36:
#line 294 "ramc_grammar.yy"
                                                                                             { IsInLoop = false; }
#line 1555 "ramc_bison_parser.cpp"
    break;

  case 37:
#line 294 "ramc_grammar.yy"
                                                                                                                                      { yylhs.value.as < ASTStmt* > () = new ASTForStmt(new ASTVarDecl(yystack_[11].value.as < ASTIdentifier* > (), yystack_[9].value.as < ASTExpr* > (), yystack_[12].value.as < Position > ()), yystack_[6].value.as < ASTExpr* > (), yystack_[3].value.as < ASTStmt* > (), yystack_[0].value.as < ASTStmt* > (), yystack_[12].value.as < Position > ()); }
#line 1561 "ramc_bison_parser.cpp"
    break;

  case 38:
#line 297 "ramc_grammar.yy"
                                          { yylhs.value.as < ASTVarDecl* > () = new ASTVarDecl(yystack_[2].value.as < ASTIdentifier* > (), yystack_[0].value.as < ASTExpr* > (), yystack_[3].value.as < Position > ()); }
#line 1567 "ramc_bison_parser.cpp"
    break;

  case 39:
#line 298 "ramc_grammar.yy"
                                          { yylhs.value.as < ASTVarDecl* > () = new ASTVarDecl(yystack_[4].value.as < ASTIdentifier* > (), yystack_[2].value.as < TypePtr > (), yystack_[0].value.as < ASTExpr* > (), yystack_[5].value.as < Position > ()); }
#line 1573 "ramc_bison_parser.cpp"
    break;

  case 40:
#line 299 "ramc_grammar.yy"
                                    { yylhs.value.as < ASTVarDecl* > () = new ASTVarDecl(yystack_[0].value.as < ASTExpr* > (), yystack_[3].value.as < Position > ()); }
#line 1579 "ramc_bison_parser.cpp"
    break;

  case 41:
#line 302 "ramc_grammar.yy"
                                      { yylhs.value.as < std::vector<TypePtr> > () = { yystack_[0].value.as < TypePtr > () }; }
#line 1585 "ramc_bison_parser.cpp"
    break;

  case 42:
#line 303 "ramc_grammar.yy"
                                      { yystack_[2].value.as < std::vector<TypePtr> > ().push_back(yystack_[0].value.as < TypePtr > ()); yylhs.value.as < std::vector<TypePtr> > () = yystack_[2].value.as < std::vector<TypePtr> > (); }
#line 1591 "ramc_bison_parser.cpp"
    break;

  case 43:
#line 307 "ramc_grammar.yy"
               { yylhs.value.as < TypePtr > () = yystack_[0].value.as < TypePtr > (); }
#line 1597 "ramc_bison_parser.cpp"
    break;

  case 44:
#line 308 "ramc_grammar.yy"
               { yylhs.value.as < TypePtr > () = yystack_[0].value.as < TypePtr > (); }
#line 1603 "ramc_bison_parser.cpp"
    break;

  case 45:
#line 309 "ramc_grammar.yy"
               { yylhs.value.as < TypePtr > () = yystack_[0].value.as < TypePtr > (); }
#line 1609 "ramc_bison_parser.cpp"
    break;

  case 46:
#line 310 "ramc_grammar.yy"
               { yylhs.value.as < TypePtr > () = yystack_[0].value.as < TypePtr > (); }
#line 1615 "ramc_bison_parser.cpp"
    break;

  case 47:
#line 311 "ramc_grammar.yy"
               { yylhs.value.as < TypePtr > () = yystack_[0].value.as < TypePtr > (); }
#line 1621 "ramc_bison_parser.cpp"
    break;

  case 48:
#line 312 "ramc_grammar.yy"
               { yylhs.value.as < TypePtr > () = yystack_[0].value.as < TypePtr > (); }
#line 1627 "ramc_bison_parser.cpp"
    break;

  case 49:
#line 313 "ramc_grammar.yy"
               { yylhs.value.as < TypePtr > () = yystack_[0].value.as < TypePtr > (); }
#line 1633 "ramc_bison_parser.cpp"
    break;

  case 50:
#line 314 "ramc_grammar.yy"
               { yylhs.value.as < TypePtr > () = yystack_[0].value.as < TypePtr > (); }
#line 1639 "ramc_bison_parser.cpp"
    break;

  case 51:
#line 317 "ramc_grammar.yy"
                                      { yylhs.value.as < ASTAssignment* > () = new ASTAssignment(yystack_[2].value.as < ASTIdentifier* > (), yystack_[0].value.as < ASTExpr* > (), yystack_[1].value.as < AssignmentType > ()); }
#line 1645 "ramc_bison_parser.cpp"
    break;

  case 52:
#line 320 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::EQ; }
#line 1651 "ramc_bison_parser.cpp"
    break;

  case 53:
#line 321 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::ADD_EQ; }
#line 1657 "ramc_bison_parser.cpp"
    break;

  case 54:
#line 322 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::SUB_EQ; }
#line 1663 "ramc_bison_parser.cpp"
    break;

  case 55:
#line 323 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::MUL_EQ; }
#line 1669 "ramc_bison_parser.cpp"
    break;

  case 56:
#line 324 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::DIV_EQ; }
#line 1675 "ramc_bison_parser.cpp"
    break;

  case 57:
#line 325 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::MOD_EQ; }
#line 1681 "ramc_bison_parser.cpp"
    break;

  case 58:
#line 326 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::POW_EQ; }
#line 1687 "ramc_bison_parser.cpp"
    break;

  case 59:
#line 327 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::BIN_AND_EQ; }
#line 1693 "ramc_bison_parser.cpp"
    break;

  case 60:
#line 328 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::BIN_OR_EQ; }
#line 1699 "ramc_bison_parser.cpp"
    break;

  case 61:
#line 329 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::BIN_XOR_EQ; }
#line 1705 "ramc_bison_parser.cpp"
    break;

  case 62:
#line 330 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::LSHIFT_EQ; }
#line 1711 "ramc_bison_parser.cpp"
    break;

  case 63:
#line 331 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::RSHIFT_EQ; }
#line 1717 "ramc_bison_parser.cpp"
    break;

  case 64:
#line 333 "ramc_grammar.yy"
                                  { yylhs.value.as < std::vector<ASTExpr*> > () = { }; }
#line 1723 "ramc_bison_parser.cpp"
    break;

  case 65:
#line 334 "ramc_grammar.yy"
                                  { yylhs.value.as < std::vector<ASTExpr*> > () = yystack_[0].value.as < std::vector<ASTExpr*> > (); }
#line 1729 "ramc_bison_parser.cpp"
    break;

  case 66:
#line 337 "ramc_grammar.yy"
                                      { yylhs.value.as < std::vector<ASTExpr*> > () = { yystack_[0].value.as < ASTExpr* > () }; }
#line 1735 "ramc_bison_parser.cpp"
    break;

  case 67:
#line 338 "ramc_grammar.yy"
                                      { yystack_[2].value.as < std::vector<ASTExpr*> > ().push_back(yystack_[0].value.as < ASTExpr* > ()); yylhs.value.as < std::vector<ASTExpr*> > () = yystack_[2].value.as < std::vector<ASTExpr*> > (); }
#line 1741 "ramc_bison_parser.cpp"
    break;

  case 68:
#line 342 "ramc_grammar.yy"
                                              { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 1747 "ramc_bison_parser.cpp"
    break;

  case 69:
#line 343 "ramc_grammar.yy"
                                              { yylhs.value.as < ASTExpr* > () = new ASTIfExpr(yystack_[4].value.as < ASTExpr* > (), yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), yystack_[5].value.as < Position > ()); }
#line 1753 "ramc_bison_parser.cpp"
    break;

  case 70:
#line 347 "ramc_grammar.yy"
                      { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::LOG_OR); }
#line 1759 "ramc_bison_parser.cpp"
    break;

  case 71:
#line 348 "ramc_grammar.yy"
                      { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 1765 "ramc_bison_parser.cpp"
    break;

  case 72:
#line 352 "ramc_grammar.yy"
                      { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::LOG_AND); }
#line 1771 "ramc_bison_parser.cpp"
    break;

  case 73:
#line 353 "ramc_grammar.yy"
                      { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 1777 "ramc_bison_parser.cpp"
    break;

  case 74:
#line 357 "ramc_grammar.yy"
                      { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::BIN_OR); }
#line 1783 "ramc_bison_parser.cpp"
    break;

  case 75:
#line 358 "ramc_grammar.yy"
                      { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 1789 "ramc_bison_parser.cpp"
    break;

  case 76:
#line 362 "ramc_grammar.yy"
                      { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::BIN_XOR); }
#line 1795 "ramc_bison_parser.cpp"
    break;

  case 77:
#line 363 "ramc_grammar.yy"
                      { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 1801 "ramc_bison_parser.cpp"
    break;

  case 78:
#line 367 "ramc_grammar.yy"
                      { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::BIN_AND); }
#line 1807 "ramc_bison_parser.cpp"
    break;

  case 79:
#line 368 "ramc_grammar.yy"
                      { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 1813 "ramc_bison_parser.cpp"
    break;

  case 80:
#line 372 "ramc_grammar.yy"
                       { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::EQ_EQ); }
#line 1819 "ramc_bison_parser.cpp"
    break;

  case 81:
#line 373 "ramc_grammar.yy"
                       { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::NEQ); }
#line 1825 "ramc_bison_parser.cpp"
    break;

  case 82:
#line 374 "ramc_grammar.yy"
                       { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 1831 "ramc_bison_parser.cpp"
    break;

  case 83:
#line 378 "ramc_grammar.yy"
                        { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::LT); }
#line 1837 "ramc_bison_parser.cpp"
    break;

  case 84:
#line 379 "ramc_grammar.yy"
                        { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::GT); }
#line 1843 "ramc_bison_parser.cpp"
    break;

  case 85:
#line 380 "ramc_grammar.yy"
                        { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::LT_EQ); }
#line 1849 "ramc_bison_parser.cpp"
    break;

  case 86:
#line 381 "ramc_grammar.yy"
                        { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::GT_EQ); }
#line 1855 "ramc_bison_parser.cpp"
    break;

  case 87:
#line 382 "ramc_grammar.yy"
                        { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 1861 "ramc_bison_parser.cpp"
    break;

  case 88:
#line 386 "ramc_grammar.yy"
                       { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::LSHIFT); }
#line 1867 "ramc_bison_parser.cpp"
    break;

  case 89:
#line 387 "ramc_grammar.yy"
                       { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::RSHIFT); }
#line 1873 "ramc_bison_parser.cpp"
    break;

  case 90:
#line 388 "ramc_grammar.yy"
                       { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 1879 "ramc_bison_parser.cpp"
    break;

  case 91:
#line 392 "ramc_grammar.yy"
                       { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::ADD); }
#line 1885 "ramc_bison_parser.cpp"
    break;

  case 92:
#line 393 "ramc_grammar.yy"
                       { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::SUB); }
#line 1891 "ramc_bison_parser.cpp"
    break;

  case 93:
#line 394 "ramc_grammar.yy"
                       { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 1897 "ramc_bison_parser.cpp"
    break;

  case 94:
#line 398 "ramc_grammar.yy"
                        { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::MUL); }
#line 1903 "ramc_bison_parser.cpp"
    break;

  case 95:
#line 399 "ramc_grammar.yy"
                        { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::DIV); }
#line 1909 "ramc_bison_parser.cpp"
    break;

  case 96:
#line 400 "ramc_grammar.yy"
                        { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::MOD); }
#line 1915 "ramc_bison_parser.cpp"
    break;

  case 97:
#line 401 "ramc_grammar.yy"
                        { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 1921 "ramc_bison_parser.cpp"
    break;

  case 98:
#line 405 "ramc_grammar.yy"
                 { yylhs.value.as < ASTExpr* > () = new ASTUnopExpr(yystack_[0].value.as < ASTExpr* > (), UnopType::NEG); }
#line 1927 "ramc_bison_parser.cpp"
    break;

  case 99:
#line 406 "ramc_grammar.yy"
                 { yylhs.value.as < ASTExpr* > () = new ASTUnopExpr(yystack_[0].value.as < ASTExpr* > (), UnopType::LOG_NOT); }
#line 1933 "ramc_bison_parser.cpp"
    break;

  case 100:
#line 407 "ramc_grammar.yy"
                 { yylhs.value.as < ASTExpr* > () = new ASTUnopExpr(yystack_[0].value.as < ASTExpr* > (), UnopType::BIN_NOT); }
#line 1939 "ramc_bison_parser.cpp"
    break;

  case 101:
#line 408 "ramc_grammar.yy"
                 { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 1945 "ramc_bison_parser.cpp"
    break;

  case 102:
#line 412 "ramc_grammar.yy"
                       { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::POW); }
#line 1951 "ramc_bison_parser.cpp"
    break;

  case 103:
#line 413 "ramc_grammar.yy"
                       { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 1957 "ramc_bison_parser.cpp"
    break;

  case 104:
#line 417 "ramc_grammar.yy"
                    { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTIntLit* > (); }
#line 1963 "ramc_bison_parser.cpp"
    break;

  case 105:
#line 418 "ramc_grammar.yy"
                    { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTFloatLit* > (); }
#line 1969 "ramc_bison_parser.cpp"
    break;

  case 106:
#line 419 "ramc_grammar.yy"
                    { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTByteLit* > (); }
#line 1975 "ramc_bison_parser.cpp"
    break;

  case 107:
#line 420 "ramc_grammar.yy"
                    { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTDoubleLit* > (); }
#line 1981 "ramc_bison_parser.cpp"
    break;

  case 108:
#line 421 "ramc_grammar.yy"
                    { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTLongLit* > (); }
#line 1987 "ramc_bison_parser.cpp"
    break;

  case 109:
#line 422 "ramc_grammar.yy"
                    { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTStringLit* > (); }
#line 1993 "ramc_bison_parser.cpp"
    break;

  case 110:
#line 423 "ramc_grammar.yy"
                    { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTBoolLit* > (); }
#line 1999 "ramc_bison_parser.cpp"
    break;

  case 111:
#line 424 "ramc_grammar.yy"
                    { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTBoolLit* > (); }
#line 2005 "ramc_bison_parser.cpp"
    break;

  case 112:
#line 425 "ramc_grammar.yy"
                        { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTIdentifier* > (); }
#line 2011 "ramc_bison_parser.cpp"
    break;

  case 113:
#line 426 "ramc_grammar.yy"
                        { yylhs.value.as < ASTExpr* > () = yystack_[1].value.as < ASTExpr* > (); }
#line 2017 "ramc_bison_parser.cpp"
    break;

  case 114:
#line 429 "ramc_grammar.yy"
                        { yylhs.value.as < ASTIdentifier* > () = new ASTIdentifier(yystack_[0].value.as < std::pair<std::string, Position> > ().first, yystack_[0].value.as < std::pair<std::string, Position> > ().second); }
#line 2023 "ramc_bison_parser.cpp"
    break;


#line 2027 "ramc_bison_parser.cpp"

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


  const signed char Parser::yypact_ninf_ = -115;

  const signed char Parser::yytable_ninf_ = -1;

  const short
  Parser::yypact_[] =
  {
    -115,     9,   -44,  -115,     0,   -42,  -115,   -14,    38,  -115,
      14,   175,    12,    74,    74,    74,    14,  -115,  -115,  -115,
    -115,  -115,  -115,  -115,  -115,    74,    50,    31,    81,    93,
      94,   122,    -3,   104,     7,    29,   139,  -115,   124,  -115,
    -115,  -115,  -115,  -115,  -115,  -115,  -115,  -115,  -115,   111,
     100,  -115,  -115,  -115,   102,    -9,  -115,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,   163,    14,    92,  -115,    14,
      81,    93,    94,   122,    -3,   104,   104,     7,     7,     7,
       7,    29,    29,   139,   139,  -115,  -115,  -115,  -115,   105,
     107,   110,   109,  -115,    82,  -115,   175,    -2,    92,    14,
    -115,    68,   175,  -115,  -115,    68,    28,    74,    74,     0,
    -115,  -115,    14,  -115,  -115,  -115,  -115,  -115,  -115,  -115,
     196,   -19,  -115,   113,   137,     4,    -8,   -18,    55,  -115,
    -115,   116,  -115,  -115,  -115,  -115,  -115,  -115,  -115,  -115,
    -115,  -115,  -115,  -115,    14,    68,   175,  -115,    14,    14,
     175,    68,  -115,    14,   175,    14,  -115,  -115,  -115,   -31,
    -115,  -115,   140,  -115,    95,    68,   123,   142,  -115,  -115,
     127,    14,    68,  -115,   106,    14,  -115,  -115,  -115,  -115,
      74,   128,   -17,   108,  -115,    74,    68,   -15,  -115,  -115,
     112,    68,   121,  -115,    74,  -115,   115,    -7,   121,   121,
    -115,   117,   121
  };

  const signed char
  Parser::yydefact_[] =
  {
       3,     0,     6,     1,     0,     2,   114,     0,     0,     7,
       0,     0,     0,     0,     0,     0,     0,   104,   109,   105,
     106,   107,   108,   110,   111,     0,     0,    68,    71,    73,
      75,    77,    79,    82,    87,    90,    93,    97,   101,   103,
     112,    43,    44,    45,    46,    47,    48,    49,    50,     0,
       0,    98,    99,   100,     0,     0,     4,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    10,   113,     0,
      70,    72,    74,    76,    78,    80,    81,    83,    84,    85,
      86,    88,    89,    91,    92,    94,    95,    96,   102,     0,
       0,     0,    11,    12,     0,     5,     0,     0,     0,     0,
      14,     0,     0,    13,    69,     0,     0,     0,     0,     0,
      27,    28,    64,     9,    17,    18,    22,    23,    21,    26,
       0,     0,    41,     0,     0,     0,     0,     0,     0,    66,
      29,    65,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,     0,     0,     0,    15,     0,     0,
       0,     0,    30,     0,     0,     0,    51,     8,    42,     0,
      40,    38,     0,    19,    18,     0,     0,     0,    67,    24,
       0,     0,     0,    31,     0,     0,    16,    39,    20,    25,
       0,     0,     0,     0,    35,     0,     0,     0,    36,    32,
       0,     0,     0,    33,     0,    37,     0,     0,     0,     0,
      34,     0,     0
  };

  const short
  Parser::yypgoto_[] =
  {
    -115,  -115,  -114,     1,  -110,  -115,  -115,  -115,  -115,  -115,
      -5,   -23,   131,   126,   130,   114,   129,    46,    24,    45,
     -25,    11,  -115,   118,    -4,  -115,  -115,  -115,  -115,  -115,
    -115,  -115,   -83,  -115,  -115,    98,  -115,  -115,  -115,  -115,
    -115
  };

  const short
  Parser::yydefgoto_[] =
  {
      -1,     1,   123,   124,   125,   126,   127,     9,   128,   129,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,     2,     5,   169,   140,   141,
     154,   131,    49,   101,   102,   103,   175,   201,   206,   196,
     200
  };

  const unsigned char
  Parser::yytable_[] =
  {
       7,   133,    55,    57,    57,   155,    57,   115,   179,     3,
      10,    54,    57,    57,    57,    62,    63,     6,    13,    68,
      69,     4,   111,   110,    51,    52,    53,   156,   159,   132,
       8,    11,    70,    71,   116,   117,    14,    15,   118,   119,
     112,   167,   120,   121,   122,    93,    94,   173,     6,   160,
      16,   174,    57,   162,   194,   180,   199,    50,    79,   161,
     209,   183,     6,    17,    18,    19,    20,    21,    22,    23,
      24,    99,   189,   168,   104,   134,     6,   172,    13,   163,
      25,   177,   198,    95,    96,    97,    12,   203,    87,    88,
      89,    90,   205,    56,   136,   137,    14,    15,   210,   211,
     164,    58,   189,    59,   114,    60,   115,   130,    85,    86,
      16,   130,   135,    91,    92,   138,     6,   139,    64,    65,
      66,    67,     6,    17,    18,    19,    20,    21,    22,    23,
      24,    61,    75,   116,   117,    76,    77,   118,   119,    78,
     100,   120,   121,   122,    72,    73,    74,   107,   105,   166,
     109,   130,   106,   170,   171,   108,   157,   130,   176,   115,
     178,   158,   165,   182,   181,   130,   185,   192,   184,     6,
     186,   130,   197,   193,    83,   190,   187,   195,   130,   202,
     191,   207,   208,   188,    81,   212,   116,   204,    80,    82,
      84,   119,   130,    98,   120,   121,   122,   130,   130,    16,
       0,     0,     0,     0,   130,   130,   113,     0,   130,     0,
       0,     6,    17,    18,    19,    20,    21,    22,    23,    24,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,    41,    42,    43,    44,    45,    46,    47,    48
  };

  const short
  Parser::yycheck_[] =
  {
       4,   115,    25,    21,    21,    24,    21,    38,    39,     0,
      24,    16,    21,    21,    21,    18,    19,    48,     4,    12,
      13,    65,    24,   106,    13,    14,    15,    46,    24,   112,
      72,    45,     3,     4,    65,    66,    22,    23,    69,    70,
      42,   155,    73,    74,    75,    70,    71,   161,    48,    45,
      36,   161,    21,    71,    71,   169,    71,    45,    67,    67,
      67,   175,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    76,   182,   156,    79,    47,    48,   160,     4,    24,
      66,   164,   196,    72,    73,    74,    48,   201,    64,    65,
      66,    67,   202,    43,   117,   118,    22,    23,   208,   209,
      45,    20,   212,    10,   109,    11,    38,   111,    62,    63,
      36,   115,   116,    68,    69,   119,    48,   122,    14,    15,
      16,    17,    48,    49,    50,    51,    52,    53,    54,    55,
      56,     9,     8,    65,    66,    24,    36,    69,    70,    37,
      48,    73,    74,    75,     5,     6,     7,    37,    43,   154,
      68,   155,    45,   158,   159,    46,    43,   161,   163,    38,
     165,    24,    46,    68,    24,   169,    24,   190,    45,    48,
      43,   175,   195,    45,    60,    69,   181,    69,   182,    67,
     185,   204,    67,   182,    58,    68,    65,    66,    57,    59,
      61,    70,   196,    75,    73,    74,    75,   201,   202,    36,
      -1,    -1,    -1,    -1,   208,   209,   108,    -1,   212,    -1,
      -1,    48,    49,    50,    51,    52,    53,    54,    55,    56,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    57,    58,    59,    60,    61,    62,    63,    64
  };

  const signed char
  Parser::yystos_[] =
  {
       0,    77,   101,     0,    65,   102,    48,   100,    72,    83,
      24,    45,    48,     4,    22,    23,    36,    49,    50,    51,
      52,    53,    54,    55,    56,    66,    86,    87,    88,    89,
      90,    91,    92,    93,    94,    95,    96,    97,    98,    99,
     100,    57,    58,    59,    60,    61,    62,    63,    64,   108,
      45,    97,    97,    97,    86,    87,    43,    21,    20,    10,
      11,     9,    18,    19,    14,    15,    16,    17,    12,    13,
       3,     4,     5,     6,     7,     8,    24,    36,    37,    67,
      88,    89,    90,    91,    92,    93,    93,    94,    94,    94,
      94,    95,    95,    96,    96,    97,    97,    97,    99,    86,
      48,   109,   110,   111,    86,    43,    45,    37,    46,    68,
     108,    24,    42,   111,    86,    38,    65,    66,    69,    70,
      73,    74,    75,    78,    79,    80,    81,    82,    84,    85,
     100,   107,   108,    78,    47,   100,    87,    87,   100,    86,
     104,   105,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,   106,    24,    46,    43,    24,    24,
      45,    67,    71,    24,    45,    46,    86,    78,   108,   103,
      86,    86,   108,    78,    80,   112,    86,   108,    86,    39,
      78,    24,    68,    78,    45,    24,    43,    86,    79,    80,
      69,    86,    87,    45,    71,    69,   115,    87,    78,    71,
     116,   113,    67,    78,    66,    80,   114,    87,    67,    67,
      80,    80,    68
  };

  const signed char
  Parser::yyr1_[] =
  {
       0,    76,    77,   101,   101,   101,   102,   102,    83,    83,
     109,   109,   110,   110,   111,   103,   103,    78,    78,    79,
      79,    79,    80,    80,    80,    80,    80,    80,    80,    80,
     112,    84,   113,   114,    85,   115,   116,    85,    82,    82,
      82,   107,   107,   108,   108,   108,   108,   108,   108,   108,
     108,    81,   106,   106,   106,   106,   106,   106,   106,   106,
     106,   106,   106,   106,   104,   104,   105,   105,    86,    86,
      87,    87,    88,    88,    89,    89,    90,    90,    91,    91,
      92,    92,    92,    93,    93,    93,    93,    93,    94,    94,
      94,    95,    95,    95,    96,    96,    96,    96,    97,    97,
      97,    97,    98,    98,    99,    99,    99,    99,    99,    99,
      99,    99,    99,    99,   100
  };

  const signed char
  Parser::yyr2_[] =
  {
       0,     2,     2,     0,     6,     8,     0,     2,    10,     8,
       0,     1,     1,     3,     3,     0,     3,     1,     1,     4,
       6,     1,     1,     1,     5,     6,     1,     1,     1,     2,
       0,     5,     0,     0,    15,     0,     0,    13,     4,     6,
       4,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     1,     1,     3,     1,     6,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     3,     1,     3,     3,     3,     3,     1,     3,     3,
       1,     3,     3,     1,     3,     3,     3,     1,     2,     2,
       2,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     1
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
  "\"string\"", "\"void\"", "\"let\"", "\"if\"", "\"then\"", "\"else\"",
  "\"while\"", "\"for\"", "\"do\"", "\"func\"", "\"break\"",
  "\"continue\"", "\"return\"", "$accept", "PROGRAM", "STMT", "OPEN_STMT",
  "CLOSED_STMT", "ASSIGNMENT", "VARDECL", "FUNCDECL", "WHILE_STMT",
  "FOR_STMT", "EXPR", "EXPR1", "EXPR2", "EXPR3", "EXPR4", "EXPR5", "EXPR6",
  "EXPR7", "EXPR8", "EXPR9", "EXPR10", "EXPR11", "EXPR12", "ATOM",
  "IDENTIFIER", "TL_VARDECLS", "TL_FUNCDECLS", "STMTS", "EXPR_STAR",
  "EXPR_PLUS", "OP_ASSIGN", "TYPE_PLUS", "TYPE", "PARAM_STAR",
  "PARAM_PLUS", "PARAM", "$@1", "$@2", "$@3", "$@4", "$@5", YY_NULLPTR
  };

#if YYDEBUG
  const short
  Parser::yyrline_[] =
  {
       0,   244,   244,   246,   247,   248,   251,   252,   255,   256,
     258,   259,   262,   263,   266,   268,   269,   272,   273,   276,
     277,   278,   281,   282,   283,   284,   285,   286,   287,   288,
     291,   291,   293,   293,   293,   294,   294,   294,   297,   298,
     299,   302,   303,   307,   308,   309,   310,   311,   312,   313,
     314,   317,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,   331,   333,   334,   337,   338,   342,   343,
     347,   348,   352,   353,   357,   358,   362,   363,   367,   368,
     372,   373,   374,   378,   379,   380,   381,   382,   386,   387,
     388,   392,   393,   394,   398,   399,   400,   401,   405,   406,
     407,   408,   412,   413,   417,   418,   419,   420,   421,   422,
     423,   424,   425,   426,   429
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
#line 2707 "ramc_bison_parser.cpp"

#line 431 "ramc_grammar.yy"

namespace ramc {
	namespace bison {
		// Report an error to the user.
		auto Parser::error(const std::string& _msg) -> void
		{
			throw std::runtime_error(_msg);
		}
	}
}

