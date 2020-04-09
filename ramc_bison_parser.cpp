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
		bool IsInLoop, InTopLvl;

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
				default: throw std::runtime_error(token.ToString(true) + " is not parasble!");
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
      case 101: // ARRAY_INIT
        value.YY_MOVE_OR_COPY< ASTArrayInit* > (YY_MOVE (that.value));
        break;

      case 80: // ASSIGNMENT
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

      case 85: // EXPR
      case 86: // EXPR1
      case 87: // EXPR2
      case 88: // EXPR3
      case 89: // EXPR4
      case 90: // EXPR5
      case 91: // EXPR6
      case 92: // EXPR7
      case 93: // EXPR8
      case 94: // EXPR9
      case 95: // EXPR10
      case 96: // EXPR11
      case 97: // EXPR12
      case 98: // ATOM
        value.YY_MOVE_OR_COPY< ASTExpr* > (YY_MOVE (that.value));
        break;

      case 51: // "FLOAT_LIT"
        value.YY_MOVE_OR_COPY< ASTFloatLit* > (YY_MOVE (that.value));
        break;

      case 100: // FUNC_CALL
        value.YY_MOVE_OR_COPY< ASTFuncCallExpr* > (YY_MOVE (that.value));
        break;

      case 82: // FUNCDECL
        value.YY_MOVE_OR_COPY< ASTFuncDecl* > (YY_MOVE (that.value));
        break;

      case 99: // IDENTIFIER
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
      case 83: // WHILE_STMT
      case 84: // FOR_STMT
        value.YY_MOVE_OR_COPY< ASTStmt* > (YY_MOVE (that.value));
        break;

      case 50: // "STRING_LIT"
        value.YY_MOVE_OR_COPY< ASTStringLit* > (YY_MOVE (that.value));
        break;

      case 81: // VARDECL
        value.YY_MOVE_OR_COPY< ASTVarDecl* > (YY_MOVE (that.value));
        break;

      case 107: // OP_ASSIGN
        value.YY_MOVE_OR_COPY< AssignmentType > (YY_MOVE (that.value));
        break;

      case 112: // PARAM
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
      case 109: // TYPE
        value.YY_MOVE_OR_COPY< Type* > (YY_MOVE (that.value));
        break;

      case 48: // "ID"
        value.YY_MOVE_OR_COPY< std::pair<std::string, Position> > (YY_MOVE (that.value));
        break;

      case 105: // EXPR_STAR
      case 106: // EXPR_PLUS
        value.YY_MOVE_OR_COPY< std::vector<ASTExpr*> > (YY_MOVE (that.value));
        break;

      case 103: // TL_FUNCDECLS
        value.YY_MOVE_OR_COPY< std::vector<ASTFuncDecl*> > (YY_MOVE (that.value));
        break;

      case 104: // STMTS
        value.YY_MOVE_OR_COPY< std::vector<ASTStmt*> > (YY_MOVE (that.value));
        break;

      case 102: // VARDECLS
        value.YY_MOVE_OR_COPY< std::vector<ASTVarDecl*> > (YY_MOVE (that.value));
        break;

      case 110: // PARAM_STAR
      case 111: // PARAM_PLUS
        value.YY_MOVE_OR_COPY< std::vector<Param> > (YY_MOVE (that.value));
        break;

      case 108: // TYPE_PLUS
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
      case 101: // ARRAY_INIT
        value.move< ASTArrayInit* > (YY_MOVE (that.value));
        break;

      case 80: // ASSIGNMENT
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

      case 85: // EXPR
      case 86: // EXPR1
      case 87: // EXPR2
      case 88: // EXPR3
      case 89: // EXPR4
      case 90: // EXPR5
      case 91: // EXPR6
      case 92: // EXPR7
      case 93: // EXPR8
      case 94: // EXPR9
      case 95: // EXPR10
      case 96: // EXPR11
      case 97: // EXPR12
      case 98: // ATOM
        value.move< ASTExpr* > (YY_MOVE (that.value));
        break;

      case 51: // "FLOAT_LIT"
        value.move< ASTFloatLit* > (YY_MOVE (that.value));
        break;

      case 100: // FUNC_CALL
        value.move< ASTFuncCallExpr* > (YY_MOVE (that.value));
        break;

      case 82: // FUNCDECL
        value.move< ASTFuncDecl* > (YY_MOVE (that.value));
        break;

      case 99: // IDENTIFIER
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
      case 83: // WHILE_STMT
      case 84: // FOR_STMT
        value.move< ASTStmt* > (YY_MOVE (that.value));
        break;

      case 50: // "STRING_LIT"
        value.move< ASTStringLit* > (YY_MOVE (that.value));
        break;

      case 81: // VARDECL
        value.move< ASTVarDecl* > (YY_MOVE (that.value));
        break;

      case 107: // OP_ASSIGN
        value.move< AssignmentType > (YY_MOVE (that.value));
        break;

      case 112: // PARAM
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
      case 109: // TYPE
        value.move< Type* > (YY_MOVE (that.value));
        break;

      case 48: // "ID"
        value.move< std::pair<std::string, Position> > (YY_MOVE (that.value));
        break;

      case 105: // EXPR_STAR
      case 106: // EXPR_PLUS
        value.move< std::vector<ASTExpr*> > (YY_MOVE (that.value));
        break;

      case 103: // TL_FUNCDECLS
        value.move< std::vector<ASTFuncDecl*> > (YY_MOVE (that.value));
        break;

      case 104: // STMTS
        value.move< std::vector<ASTStmt*> > (YY_MOVE (that.value));
        break;

      case 102: // VARDECLS
        value.move< std::vector<ASTVarDecl*> > (YY_MOVE (that.value));
        break;

      case 110: // PARAM_STAR
      case 111: // PARAM_PLUS
        value.move< std::vector<Param> > (YY_MOVE (that.value));
        break;

      case 108: // TYPE_PLUS
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
      case 101: // ARRAY_INIT
        value.copy< ASTArrayInit* > (that.value);
        break;

      case 80: // ASSIGNMENT
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

      case 85: // EXPR
      case 86: // EXPR1
      case 87: // EXPR2
      case 88: // EXPR3
      case 89: // EXPR4
      case 90: // EXPR5
      case 91: // EXPR6
      case 92: // EXPR7
      case 93: // EXPR8
      case 94: // EXPR9
      case 95: // EXPR10
      case 96: // EXPR11
      case 97: // EXPR12
      case 98: // ATOM
        value.copy< ASTExpr* > (that.value);
        break;

      case 51: // "FLOAT_LIT"
        value.copy< ASTFloatLit* > (that.value);
        break;

      case 100: // FUNC_CALL
        value.copy< ASTFuncCallExpr* > (that.value);
        break;

      case 82: // FUNCDECL
        value.copy< ASTFuncDecl* > (that.value);
        break;

      case 99: // IDENTIFIER
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
      case 83: // WHILE_STMT
      case 84: // FOR_STMT
        value.copy< ASTStmt* > (that.value);
        break;

      case 50: // "STRING_LIT"
        value.copy< ASTStringLit* > (that.value);
        break;

      case 81: // VARDECL
        value.copy< ASTVarDecl* > (that.value);
        break;

      case 107: // OP_ASSIGN
        value.copy< AssignmentType > (that.value);
        break;

      case 112: // PARAM
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
      case 109: // TYPE
        value.copy< Type* > (that.value);
        break;

      case 48: // "ID"
        value.copy< std::pair<std::string, Position> > (that.value);
        break;

      case 105: // EXPR_STAR
      case 106: // EXPR_PLUS
        value.copy< std::vector<ASTExpr*> > (that.value);
        break;

      case 103: // TL_FUNCDECLS
        value.copy< std::vector<ASTFuncDecl*> > (that.value);
        break;

      case 104: // STMTS
        value.copy< std::vector<ASTStmt*> > (that.value);
        break;

      case 102: // VARDECLS
        value.copy< std::vector<ASTVarDecl*> > (that.value);
        break;

      case 110: // PARAM_STAR
      case 111: // PARAM_PLUS
        value.copy< std::vector<Param> > (that.value);
        break;

      case 108: // TYPE_PLUS
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
      case 101: // ARRAY_INIT
        value.move< ASTArrayInit* > (that.value);
        break;

      case 80: // ASSIGNMENT
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

      case 85: // EXPR
      case 86: // EXPR1
      case 87: // EXPR2
      case 88: // EXPR3
      case 89: // EXPR4
      case 90: // EXPR5
      case 91: // EXPR6
      case 92: // EXPR7
      case 93: // EXPR8
      case 94: // EXPR9
      case 95: // EXPR10
      case 96: // EXPR11
      case 97: // EXPR12
      case 98: // ATOM
        value.move< ASTExpr* > (that.value);
        break;

      case 51: // "FLOAT_LIT"
        value.move< ASTFloatLit* > (that.value);
        break;

      case 100: // FUNC_CALL
        value.move< ASTFuncCallExpr* > (that.value);
        break;

      case 82: // FUNCDECL
        value.move< ASTFuncDecl* > (that.value);
        break;

      case 99: // IDENTIFIER
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
      case 83: // WHILE_STMT
      case 84: // FOR_STMT
        value.move< ASTStmt* > (that.value);
        break;

      case 50: // "STRING_LIT"
        value.move< ASTStringLit* > (that.value);
        break;

      case 81: // VARDECL
        value.move< ASTVarDecl* > (that.value);
        break;

      case 107: // OP_ASSIGN
        value.move< AssignmentType > (that.value);
        break;

      case 112: // PARAM
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
      case 109: // TYPE
        value.move< Type* > (that.value);
        break;

      case 48: // "ID"
        value.move< std::pair<std::string, Position> > (that.value);
        break;

      case 105: // EXPR_STAR
      case 106: // EXPR_PLUS
        value.move< std::vector<ASTExpr*> > (that.value);
        break;

      case 103: // TL_FUNCDECLS
        value.move< std::vector<ASTFuncDecl*> > (that.value);
        break;

      case 104: // STMTS
        value.move< std::vector<ASTStmt*> > (that.value);
        break;

      case 102: // VARDECLS
        value.move< std::vector<ASTVarDecl*> > (that.value);
        break;

      case 110: // PARAM_STAR
      case 111: // PARAM_PLUS
        value.move< std::vector<Param> > (that.value);
        break;

      case 108: // TYPE_PLUS
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
      case 101: // ARRAY_INIT
        yylhs.value.emplace< ASTArrayInit* > ();
        break;

      case 80: // ASSIGNMENT
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

      case 85: // EXPR
      case 86: // EXPR1
      case 87: // EXPR2
      case 88: // EXPR3
      case 89: // EXPR4
      case 90: // EXPR5
      case 91: // EXPR6
      case 92: // EXPR7
      case 93: // EXPR8
      case 94: // EXPR9
      case 95: // EXPR10
      case 96: // EXPR11
      case 97: // EXPR12
      case 98: // ATOM
        yylhs.value.emplace< ASTExpr* > ();
        break;

      case 51: // "FLOAT_LIT"
        yylhs.value.emplace< ASTFloatLit* > ();
        break;

      case 100: // FUNC_CALL
        yylhs.value.emplace< ASTFuncCallExpr* > ();
        break;

      case 82: // FUNCDECL
        yylhs.value.emplace< ASTFuncDecl* > ();
        break;

      case 99: // IDENTIFIER
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
      case 83: // WHILE_STMT
      case 84: // FOR_STMT
        yylhs.value.emplace< ASTStmt* > ();
        break;

      case 50: // "STRING_LIT"
        yylhs.value.emplace< ASTStringLit* > ();
        break;

      case 81: // VARDECL
        yylhs.value.emplace< ASTVarDecl* > ();
        break;

      case 107: // OP_ASSIGN
        yylhs.value.emplace< AssignmentType > ();
        break;

      case 112: // PARAM
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
      case 109: // TYPE
        yylhs.value.emplace< Type* > ();
        break;

      case 48: // "ID"
        yylhs.value.emplace< std::pair<std::string, Position> > ();
        break;

      case 105: // EXPR_STAR
      case 106: // EXPR_PLUS
        yylhs.value.emplace< std::vector<ASTExpr*> > ();
        break;

      case 103: // TL_FUNCDECLS
        yylhs.value.emplace< std::vector<ASTFuncDecl*> > ();
        break;

      case 104: // STMTS
        yylhs.value.emplace< std::vector<ASTStmt*> > ();
        break;

      case 102: // VARDECLS
        yylhs.value.emplace< std::vector<ASTVarDecl*> > ();
        break;

      case 110: // PARAM_STAR
      case 111: // PARAM_PLUS
        yylhs.value.emplace< std::vector<Param> > ();
        break;

      case 108: // TYPE_PLUS
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
#line 244 "ramc_grammar.yy"
         { InTopLvl = true; }
#line 1390 "ramc_bison_parser.cpp"
    break;

  case 3:
#line 244 "ramc_grammar.yy"
                                       { InTopLvl = false; }
#line 1396 "ramc_bison_parser.cpp"
    break;

  case 4:
#line 244 "ramc_grammar.yy"
                                                                          { result = new ASTProgram("Test File", yystack_[2].value.as < std::vector<ASTVarDecl*> > (), yystack_[0].value.as < std::vector<ASTFuncDecl*> > ()); }
#line 1402 "ramc_bison_parser.cpp"
    break;

  case 5:
#line 246 "ramc_grammar.yy"
                                           { yylhs.value.as < std::vector<ASTVarDecl*> > () = { }; }
#line 1408 "ramc_bison_parser.cpp"
    break;

  case 6:
#line 247 "ramc_grammar.yy"
                                       { yystack_[0].value.as < std::vector<ASTVarDecl*> > ().insert(yystack_[0].value.as < std::vector<ASTVarDecl*> > ().begin(), yystack_[2].value.as < ASTVarDecl* > ()); yylhs.value.as < std::vector<ASTVarDecl*> > () = yystack_[0].value.as < std::vector<ASTVarDecl*> > (); }
#line 1414 "ramc_bison_parser.cpp"
    break;

  case 7:
#line 250 "ramc_grammar.yy"
                                                { yylhs.value.as < std::vector<ASTFuncDecl*> > () = { }; }
#line 1420 "ramc_bison_parser.cpp"
    break;

  case 8:
#line 251 "ramc_grammar.yy"
                                                { yystack_[0].value.as < std::vector<ASTFuncDecl*> > ().insert(yystack_[0].value.as < std::vector<ASTFuncDecl*> > ().begin(), yystack_[1].value.as < ASTFuncDecl* > ()); yylhs.value.as < std::vector<ASTFuncDecl*> > () = yystack_[0].value.as < std::vector<ASTFuncDecl*> > (); }
#line 1426 "ramc_bison_parser.cpp"
    break;

  case 9:
#line 254 "ramc_grammar.yy"
                                                                     { yylhs.value.as < ASTFuncDecl* > () = new ASTFuncDecl(yystack_[8].value.as < std::pair<std::string, Position> > ().first, yystack_[5].value.as < std::vector<Param> > (), yystack_[2].value.as < std::vector<Type*> > (), yystack_[0].value.as < ASTStmt* > (), yystack_[9].value.as < Position > ()); }
#line 1432 "ramc_bison_parser.cpp"
    break;

  case 10:
#line 255 "ramc_grammar.yy"
                                                                                         { yylhs.value.as < ASTFuncDecl* > () = new ASTFuncDecl(yystack_[6].value.as < std::pair<std::string, Position> > ().first, yystack_[3].value.as < std::vector<Param> > (), { }, yystack_[0].value.as < ASTStmt* > (), yystack_[7].value.as < Position > ()); }
#line 1438 "ramc_bison_parser.cpp"
    break;

  case 11:
#line 257 "ramc_grammar.yy"
                           { yylhs.value.as < std::vector<Param> > () = { }; }
#line 1444 "ramc_bison_parser.cpp"
    break;

  case 12:
#line 258 "ramc_grammar.yy"
                               { yylhs.value.as < std::vector<Param> > () = yystack_[0].value.as < std::vector<Param> > (); }
#line 1450 "ramc_bison_parser.cpp"
    break;

  case 13:
#line 261 "ramc_grammar.yy"
                                             { yylhs.value.as < std::vector<Param> > () = { yystack_[0].value.as < Param > () }; }
#line 1456 "ramc_bison_parser.cpp"
    break;

  case 14:
#line 262 "ramc_grammar.yy"
                                         { yystack_[0].value.as < std::vector<Param> > ().insert(yystack_[0].value.as < std::vector<Param> > ().begin(), yystack_[2].value.as < Param > ()); yylhs.value.as < std::vector<Param> > () = yystack_[0].value.as < std::vector<Param> > (); }
#line 1462 "ramc_bison_parser.cpp"
    break;

  case 15:
#line 265 "ramc_grammar.yy"
                     { yylhs.value.as < Param > () = { yystack_[2].value.as < std::pair<std::string, Position> > ().first, yystack_[0].value.as < Type* > (), yystack_[2].value.as < std::pair<std::string, Position> > ().second }; }
#line 1468 "ramc_bison_parser.cpp"
    break;

  case 16:
#line 267 "ramc_grammar.yy"
                          { yylhs.value.as < std::vector<ASTStmt*> > () = { }; }
#line 1474 "ramc_bison_parser.cpp"
    break;

  case 17:
#line 268 "ramc_grammar.yy"
                          { yystack_[0].value.as < std::vector<ASTStmt*> > ().insert(yystack_[0].value.as < std::vector<ASTStmt*> > ().begin(), yystack_[2].value.as < ASTStmt* > ()); yylhs.value.as < std::vector<ASTStmt*> > () = yystack_[0].value.as < std::vector<ASTStmt*> > (); }
#line 1480 "ramc_bison_parser.cpp"
    break;

  case 18:
#line 271 "ramc_grammar.yy"
                  { yylhs.value.as < ASTStmt* > () = yystack_[0].value.as < ASTStmt* > (); }
#line 1486 "ramc_bison_parser.cpp"
    break;

  case 19:
#line 272 "ramc_grammar.yy"
                  { yylhs.value.as < ASTStmt* > () = yystack_[0].value.as < ASTStmt* > (); }
#line 1492 "ramc_bison_parser.cpp"
    break;

  case 20:
#line 275 "ramc_grammar.yy"
                                                                                { yylhs.value.as < ASTStmt* > () = new ASTIfStmt(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTStmt* > (), nullptr, yystack_[3].value.as < Position > ()); }
#line 1498 "ramc_bison_parser.cpp"
    break;

  case 21:
#line 276 "ramc_grammar.yy"
                                                                        { yylhs.value.as < ASTStmt* > () = new ASTIfStmt(yystack_[4].value.as < ASTExpr* > (), yystack_[2].value.as < ASTStmt* > (), yystack_[0].value.as < ASTStmt* > (), yystack_[5].value.as < Position > ()); }
#line 1504 "ramc_bison_parser.cpp"
    break;

  case 22:
#line 277 "ramc_grammar.yy"
                                                                                                        { yylhs.value.as < ASTStmt* > () = yystack_[0].value.as < ASTStmt* > (); }
#line 1510 "ramc_bison_parser.cpp"
    break;

  case 23:
#line 280 "ramc_grammar.yy"
                                                                                                        { yylhs.value.as < ASTStmt* > () = yystack_[0].value.as < ASTAssignment* > (); }
#line 1516 "ramc_bison_parser.cpp"
    break;

  case 24:
#line 281 "ramc_grammar.yy"
                                                                                                                { yylhs.value.as < ASTStmt* > () = yystack_[0].value.as < ASTVarDecl* > (); }
#line 1522 "ramc_bison_parser.cpp"
    break;

  case 25:
#line 282 "ramc_grammar.yy"
                                                                                                        { yystack_[1].value.as < std::vector<ASTStmt*> > ().insert(yystack_[1].value.as < std::vector<ASTStmt*> > ().begin(), yystack_[3].value.as < ASTStmt* > ()); yylhs.value.as < ASTStmt* > () = new ASTBlock(yystack_[1].value.as < std::vector<ASTStmt*> > (), yystack_[4].value.as < Position > ());}
#line 1528 "ramc_bison_parser.cpp"
    break;

  case 26:
#line 283 "ramc_grammar.yy"
                                                                        { yylhs.value.as < ASTStmt* > () = new ASTIfStmt(yystack_[4].value.as < ASTExpr* > (), yystack_[2].value.as < ASTStmt* > (), yystack_[0].value.as < ASTStmt* > (), yystack_[5].value.as < Position > ()); }
#line 1534 "ramc_bison_parser.cpp"
    break;

  case 27:
#line 284 "ramc_grammar.yy"
                                                                                                                { yylhs.value.as < ASTStmt* > () = yystack_[0].value.as < ASTStmt* > (); }
#line 1540 "ramc_bison_parser.cpp"
    break;

  case 28:
#line 285 "ramc_grammar.yy"
                                                                                                                { IsInLoop ? yylhs.value.as < ASTStmt* > () = new ASTBreakContinueStmt(true, yystack_[0].value.as < Position > ()) : throw std::runtime_error("Cannot have 'break' outside of a loop!"); }
#line 1546 "ramc_bison_parser.cpp"
    break;

  case 29:
#line 286 "ramc_grammar.yy"
                                                                                                                { IsInLoop ? yylhs.value.as < ASTStmt* > () = new ASTBreakContinueStmt(false, yystack_[0].value.as < Position > ()) : throw std::runtime_error("Cannot have 'continue' outside of a loop!"); }
#line 1552 "ramc_bison_parser.cpp"
    break;

  case 30:
#line 287 "ramc_grammar.yy"
                                                                                                        { yylhs.value.as < ASTStmt* > () = new ASTReturnStmt(yystack_[0].value.as < std::vector<ASTExpr*> > (), yystack_[1].value.as < Position > ()); }
#line 1558 "ramc_bison_parser.cpp"
    break;

  case 31:
#line 290 "ramc_grammar.yy"
                               { IsInLoop = true; }
#line 1564 "ramc_bison_parser.cpp"
    break;

  case 32:
#line 290 "ramc_grammar.yy"
                                                         { IsInLoop = false; yylhs.value.as < ASTStmt* > () = new ASTWhileStmt(yystack_[3].value.as < ASTExpr* > (), yystack_[0].value.as < ASTStmt* > (), yystack_[4].value.as < Position > ()); }
#line 1570 "ramc_bison_parser.cpp"
    break;

  case 33:
#line 292 "ramc_grammar.yy"
                                                              { IsInLoop = true; }
#line 1576 "ramc_bison_parser.cpp"
    break;

  case 34:
#line 292 "ramc_grammar.yy"
                                                                                        { IsInLoop = false; }
#line 1582 "ramc_bison_parser.cpp"
    break;

  case 35:
#line 292 "ramc_grammar.yy"
                                                                                                                                 { yylhs.value.as < ASTStmt* > () = new ASTForStmt(new ASTVarDecl(yystack_[13].value.as < std::pair<std::string, Position> > ().first, yystack_[11].value.as < Type* > (), yystack_[9].value.as < ASTExpr* > (), false, yystack_[14].value.as < Position > ()), yystack_[6].value.as < ASTExpr* > (), yystack_[3].value.as < ASTStmt* > (), yystack_[0].value.as < ASTStmt* > (), yystack_[14].value.as < Position > ()); }
#line 1588 "ramc_bison_parser.cpp"
    break;

  case 36:
#line 293 "ramc_grammar.yy"
                                                             { IsInLoop = true; }
#line 1594 "ramc_bison_parser.cpp"
    break;

  case 37:
#line 293 "ramc_grammar.yy"
                                                                                       { IsInLoop = false; }
#line 1600 "ramc_bison_parser.cpp"
    break;

  case 38:
#line 293 "ramc_grammar.yy"
                                                                                                                                { yylhs.value.as < ASTStmt* > () = new ASTForStmt(new ASTVarDecl(yystack_[11].value.as < std::pair<std::string, Position> > ().first, yystack_[9].value.as < ASTExpr* > (), false, yystack_[12].value.as < Position > ()), yystack_[6].value.as < ASTExpr* > (), yystack_[3].value.as < ASTStmt* > (), yystack_[0].value.as < ASTStmt* > (), yystack_[12].value.as < Position > ()); }
#line 1606 "ramc_bison_parser.cpp"
    break;

  case 39:
#line 296 "ramc_grammar.yy"
                                    { yylhs.value.as < ASTVarDecl* > () = new ASTVarDecl(yystack_[2].value.as < std::pair<std::string, Position> > ().first, yystack_[0].value.as < ASTExpr* > (), InTopLvl, yystack_[3].value.as < Position > ()); }
#line 1612 "ramc_bison_parser.cpp"
    break;

  case 40:
#line 297 "ramc_grammar.yy"
                                    { yylhs.value.as < ASTVarDecl* > () = new ASTVarDecl(yystack_[4].value.as < std::pair<std::string, Position> > ().first, yystack_[2].value.as < Type* > (), yystack_[0].value.as < ASTExpr* > (), InTopLvl, yystack_[5].value.as < Position > ()); }
#line 1618 "ramc_bison_parser.cpp"
    break;

  case 41:
#line 298 "ramc_grammar.yy"
                                                          { !InTopLvl ? yylhs.value.as < ASTVarDecl* > () = new ASTVarDecl(yystack_[0].value.as < ASTExpr* > (), yystack_[3].value.as < Position > ()) : throw std::runtime_error("Cannot declare throw away in top level!"); }
#line 1624 "ramc_bison_parser.cpp"
    break;

  case 42:
#line 301 "ramc_grammar.yy"
                                      { yylhs.value.as < std::vector<Type*> > () = { yystack_[0].value.as < Type* > () }; }
#line 1630 "ramc_bison_parser.cpp"
    break;

  case 43:
#line 302 "ramc_grammar.yy"
                                      { yystack_[0].value.as < std::vector<Type*> > ().insert(yystack_[0].value.as < std::vector<Type*> > ().begin(), yystack_[2].value.as < Type* > ()); yylhs.value.as < std::vector<Type*> > () = yystack_[0].value.as < std::vector<Type*> > (); }
#line 1636 "ramc_bison_parser.cpp"
    break;

  case 44:
#line 306 "ramc_grammar.yy"
                                                { yylhs.value.as < Type* > () = yystack_[0].value.as < Type* > (); }
#line 1642 "ramc_bison_parser.cpp"
    break;

  case 45:
#line 307 "ramc_grammar.yy"
                                                { yylhs.value.as < Type* > () = yystack_[0].value.as < Type* > (); }
#line 1648 "ramc_bison_parser.cpp"
    break;

  case 46:
#line 308 "ramc_grammar.yy"
                                                { yylhs.value.as < Type* > () = yystack_[0].value.as < Type* > (); }
#line 1654 "ramc_bison_parser.cpp"
    break;

  case 47:
#line 309 "ramc_grammar.yy"
                                                { yylhs.value.as < Type* > () = yystack_[0].value.as < Type* > (); }
#line 1660 "ramc_bison_parser.cpp"
    break;

  case 48:
#line 310 "ramc_grammar.yy"
                                        { yylhs.value.as < Type* > () = yystack_[0].value.as < Type* > (); }
#line 1666 "ramc_bison_parser.cpp"
    break;

  case 49:
#line 311 "ramc_grammar.yy"
                                                { yylhs.value.as < Type* > () = yystack_[0].value.as < Type* > (); }
#line 1672 "ramc_bison_parser.cpp"
    break;

  case 50:
#line 312 "ramc_grammar.yy"
                                        { yylhs.value.as < Type* > () = yystack_[0].value.as < Type* > (); }
#line 1678 "ramc_bison_parser.cpp"
    break;

  case 51:
#line 313 "ramc_grammar.yy"
                            { yylhs.value.as < Type* > () = new ArrayType(yystack_[3].value.as < Type* > (), yystack_[1].value.as < ASTIntLit* > ()->GetValue()); delete yystack_[1].value.as < ASTIntLit* > (); }
#line 1684 "ramc_bison_parser.cpp"
    break;

  case 52:
#line 316 "ramc_grammar.yy"
                                      { yylhs.value.as < ASTAssignment* > () = new ASTAssignment(yystack_[2].value.as < ASTIdentifier* > (), yystack_[0].value.as < ASTExpr* > (), yystack_[1].value.as < AssignmentType > ()); }
#line 1690 "ramc_bison_parser.cpp"
    break;

  case 53:
#line 319 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::EQ; }
#line 1696 "ramc_bison_parser.cpp"
    break;

  case 54:
#line 320 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::ADD_EQ; }
#line 1702 "ramc_bison_parser.cpp"
    break;

  case 55:
#line 321 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::SUB_EQ; }
#line 1708 "ramc_bison_parser.cpp"
    break;

  case 56:
#line 322 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::MUL_EQ; }
#line 1714 "ramc_bison_parser.cpp"
    break;

  case 57:
#line 323 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::DIV_EQ; }
#line 1720 "ramc_bison_parser.cpp"
    break;

  case 58:
#line 324 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::MOD_EQ; }
#line 1726 "ramc_bison_parser.cpp"
    break;

  case 59:
#line 325 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::POW_EQ; }
#line 1732 "ramc_bison_parser.cpp"
    break;

  case 60:
#line 326 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::BIN_AND_EQ; }
#line 1738 "ramc_bison_parser.cpp"
    break;

  case 61:
#line 327 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::BIN_OR_EQ; }
#line 1744 "ramc_bison_parser.cpp"
    break;

  case 62:
#line 328 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::BIN_XOR_EQ; }
#line 1750 "ramc_bison_parser.cpp"
    break;

  case 63:
#line 329 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::LSHIFT_EQ; }
#line 1756 "ramc_bison_parser.cpp"
    break;

  case 64:
#line 330 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::RSHIFT_EQ; }
#line 1762 "ramc_bison_parser.cpp"
    break;

  case 65:
#line 332 "ramc_grammar.yy"
                                  { yylhs.value.as < std::vector<ASTExpr*> > () = { }; }
#line 1768 "ramc_bison_parser.cpp"
    break;

  case 66:
#line 333 "ramc_grammar.yy"
                                  { yylhs.value.as < std::vector<ASTExpr*> > () = yystack_[0].value.as < std::vector<ASTExpr*> > (); }
#line 1774 "ramc_bison_parser.cpp"
    break;

  case 67:
#line 336 "ramc_grammar.yy"
                                      { yylhs.value.as < std::vector<ASTExpr*> > () = { yystack_[0].value.as < ASTExpr* > () }; }
#line 1780 "ramc_bison_parser.cpp"
    break;

  case 68:
#line 337 "ramc_grammar.yy"
                                      { yystack_[0].value.as < std::vector<ASTExpr*> > ().insert(yystack_[0].value.as < std::vector<ASTExpr*> > ().begin(), yystack_[2].value.as < ASTExpr* > ()); yylhs.value.as < std::vector<ASTExpr*> > () = yystack_[0].value.as < std::vector<ASTExpr*> > (); }
#line 1786 "ramc_bison_parser.cpp"
    break;

  case 69:
#line 341 "ramc_grammar.yy"
                                            { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 1792 "ramc_bison_parser.cpp"
    break;

  case 70:
#line 342 "ramc_grammar.yy"
                                            { yylhs.value.as < ASTExpr* > () = new ASTIfExpr(yystack_[4].value.as < ASTExpr* > (), yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), yystack_[5].value.as < Position > ()); }
#line 1798 "ramc_bison_parser.cpp"
    break;

  case 71:
#line 346 "ramc_grammar.yy"
                      { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::LOG_OR); }
#line 1804 "ramc_bison_parser.cpp"
    break;

  case 72:
#line 347 "ramc_grammar.yy"
                      { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 1810 "ramc_bison_parser.cpp"
    break;

  case 73:
#line 351 "ramc_grammar.yy"
                      { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::LOG_AND); }
#line 1816 "ramc_bison_parser.cpp"
    break;

  case 74:
#line 352 "ramc_grammar.yy"
                      { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 1822 "ramc_bison_parser.cpp"
    break;

  case 75:
#line 356 "ramc_grammar.yy"
                      { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::BIN_OR); }
#line 1828 "ramc_bison_parser.cpp"
    break;

  case 76:
#line 357 "ramc_grammar.yy"
                      { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 1834 "ramc_bison_parser.cpp"
    break;

  case 77:
#line 361 "ramc_grammar.yy"
                      { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::BIN_XOR); }
#line 1840 "ramc_bison_parser.cpp"
    break;

  case 78:
#line 362 "ramc_grammar.yy"
                      { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 1846 "ramc_bison_parser.cpp"
    break;

  case 79:
#line 366 "ramc_grammar.yy"
                      { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::BIN_AND); }
#line 1852 "ramc_bison_parser.cpp"
    break;

  case 80:
#line 367 "ramc_grammar.yy"
                      { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 1858 "ramc_bison_parser.cpp"
    break;

  case 81:
#line 371 "ramc_grammar.yy"
                       { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::EQ_EQ); }
#line 1864 "ramc_bison_parser.cpp"
    break;

  case 82:
#line 372 "ramc_grammar.yy"
                       { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::NEQ); }
#line 1870 "ramc_bison_parser.cpp"
    break;

  case 83:
#line 373 "ramc_grammar.yy"
                       { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 1876 "ramc_bison_parser.cpp"
    break;

  case 84:
#line 377 "ramc_grammar.yy"
                        { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::LT); }
#line 1882 "ramc_bison_parser.cpp"
    break;

  case 85:
#line 378 "ramc_grammar.yy"
                        { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::GT); }
#line 1888 "ramc_bison_parser.cpp"
    break;

  case 86:
#line 379 "ramc_grammar.yy"
                        { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::LT_EQ); }
#line 1894 "ramc_bison_parser.cpp"
    break;

  case 87:
#line 380 "ramc_grammar.yy"
                        { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::GT_EQ); }
#line 1900 "ramc_bison_parser.cpp"
    break;

  case 88:
#line 381 "ramc_grammar.yy"
                        { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 1906 "ramc_bison_parser.cpp"
    break;

  case 89:
#line 385 "ramc_grammar.yy"
                       { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::LSHIFT); }
#line 1912 "ramc_bison_parser.cpp"
    break;

  case 90:
#line 386 "ramc_grammar.yy"
                       { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::RSHIFT); }
#line 1918 "ramc_bison_parser.cpp"
    break;

  case 91:
#line 387 "ramc_grammar.yy"
                       { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 1924 "ramc_bison_parser.cpp"
    break;

  case 92:
#line 391 "ramc_grammar.yy"
                       { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::ADD); }
#line 1930 "ramc_bison_parser.cpp"
    break;

  case 93:
#line 392 "ramc_grammar.yy"
                       { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::SUB); }
#line 1936 "ramc_bison_parser.cpp"
    break;

  case 94:
#line 393 "ramc_grammar.yy"
                       { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 1942 "ramc_bison_parser.cpp"
    break;

  case 95:
#line 397 "ramc_grammar.yy"
                        { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::MUL); }
#line 1948 "ramc_bison_parser.cpp"
    break;

  case 96:
#line 398 "ramc_grammar.yy"
                        { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::DIV); }
#line 1954 "ramc_bison_parser.cpp"
    break;

  case 97:
#line 399 "ramc_grammar.yy"
                        { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::MOD); }
#line 1960 "ramc_bison_parser.cpp"
    break;

  case 98:
#line 400 "ramc_grammar.yy"
                        { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 1966 "ramc_bison_parser.cpp"
    break;

  case 99:
#line 404 "ramc_grammar.yy"
                 { yylhs.value.as < ASTExpr* > () = new ASTUnopExpr(yystack_[0].value.as < ASTExpr* > (), UnopType::NEG); }
#line 1972 "ramc_bison_parser.cpp"
    break;

  case 100:
#line 405 "ramc_grammar.yy"
                 { yylhs.value.as < ASTExpr* > () = new ASTUnopExpr(yystack_[0].value.as < ASTExpr* > (), UnopType::LOG_NOT); }
#line 1978 "ramc_bison_parser.cpp"
    break;

  case 101:
#line 406 "ramc_grammar.yy"
                 { yylhs.value.as < ASTExpr* > () = new ASTUnopExpr(yystack_[0].value.as < ASTExpr* > (), UnopType::BIN_NOT); }
#line 1984 "ramc_bison_parser.cpp"
    break;

  case 102:
#line 407 "ramc_grammar.yy"
                 { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 1990 "ramc_bison_parser.cpp"
    break;

  case 103:
#line 411 "ramc_grammar.yy"
                       { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[2].value.as < ASTExpr* > (), yystack_[0].value.as < ASTExpr* > (), BinopType::POW); }
#line 1996 "ramc_bison_parser.cpp"
    break;

  case 104:
#line 412 "ramc_grammar.yy"
                       { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTExpr* > (); }
#line 2002 "ramc_bison_parser.cpp"
    break;

  case 105:
#line 416 "ramc_grammar.yy"
                                { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTIntLit* > (); }
#line 2008 "ramc_bison_parser.cpp"
    break;

  case 106:
#line 417 "ramc_grammar.yy"
                                { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTFloatLit* > (); }
#line 2014 "ramc_bison_parser.cpp"
    break;

  case 107:
#line 418 "ramc_grammar.yy"
                                { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTByteLit* > (); }
#line 2020 "ramc_bison_parser.cpp"
    break;

  case 108:
#line 419 "ramc_grammar.yy"
                                { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTDoubleLit* > (); }
#line 2026 "ramc_bison_parser.cpp"
    break;

  case 109:
#line 420 "ramc_grammar.yy"
                                { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTLongLit* > (); }
#line 2032 "ramc_bison_parser.cpp"
    break;

  case 110:
#line 421 "ramc_grammar.yy"
                                { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTStringLit* > (); }
#line 2038 "ramc_bison_parser.cpp"
    break;

  case 111:
#line 422 "ramc_grammar.yy"
                                        { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTBoolLit* > (); }
#line 2044 "ramc_bison_parser.cpp"
    break;

  case 112:
#line 423 "ramc_grammar.yy"
                                        { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTBoolLit* > (); }
#line 2050 "ramc_bison_parser.cpp"
    break;

  case 113:
#line 424 "ramc_grammar.yy"
                                { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTIdentifier* > (); }
#line 2056 "ramc_bison_parser.cpp"
    break;

  case 114:
#line 425 "ramc_grammar.yy"
                                { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTFuncCallExpr* > (); }
#line 2062 "ramc_bison_parser.cpp"
    break;

  case 115:
#line 426 "ramc_grammar.yy"
                                { yylhs.value.as < ASTExpr* > () = yystack_[1].value.as < ASTExpr* > (); }
#line 2068 "ramc_bison_parser.cpp"
    break;

  case 116:
#line 427 "ramc_grammar.yy"
                                { yylhs.value.as < ASTExpr* > () = yystack_[0].value.as < ASTArrayInit* > (); }
#line 2074 "ramc_bison_parser.cpp"
    break;

  case 117:
#line 428 "ramc_grammar.yy"
                        { yylhs.value.as < ASTExpr* > () = new ASTBinopExpr(yystack_[3].value.as < ASTExpr* > (), yystack_[1].value.as < ASTExpr* > (), BinopType::INDEX); }
#line 2080 "ramc_bison_parser.cpp"
    break;

  case 118:
#line 431 "ramc_grammar.yy"
                                                        { yylhs.value.as < ASTIdentifier* > () = new ASTIdentifier(yystack_[0].value.as < std::pair<std::string, Position> > ().first, yystack_[0].value.as < std::pair<std::string, Position> > ().second); }
#line 2086 "ramc_bison_parser.cpp"
    break;

  case 119:
#line 432 "ramc_grammar.yy"
                                        { yylhs.value.as < ASTFuncCallExpr* > () = new ASTFuncCallExpr(yystack_[3].value.as < std::pair<std::string, Position> > ().first, yystack_[1].value.as < std::vector<ASTExpr*> > (), yystack_[3].value.as < std::pair<std::string, Position> > ().second); }
#line 2092 "ramc_bison_parser.cpp"
    break;

  case 120:
#line 433 "ramc_grammar.yy"
                                        { yylhs.value.as < ASTArrayInit* > () = new ASTArrayInit(yystack_[1].value.as < std::vector<ASTExpr*> > (), yystack_[2].value.as < Position > ()); }
#line 2098 "ramc_bison_parser.cpp"
    break;


#line 2102 "ramc_bison_parser.cpp"

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


  const short Parser::yypact_ninf_ = -134;

  const signed char Parser::yytable_ninf_ = -1;

  const short
  Parser::yypact_[] =
  {
    -134,    24,   -50,  -134,    49,    44,  -134,    10,   -12,   -50,
     -17,    87,    87,   -31,  -134,    42,   -17,  -134,    17,    17,
      17,    87,    87,    63,  -134,  -134,  -134,  -134,  -134,  -134,
    -134,  -134,    17,  -134,   105,   101,   121,   142,   141,    18,
      99,    90,   114,   100,  -134,   156,   125,  -134,  -134,  -134,
    -134,  -134,  -134,  -134,  -134,  -134,  -134,  -134,    -2,   122,
    -134,  -134,  -134,  -134,   129,   124,   127,    87,   -14,    17,
      17,    17,    17,    17,    17,    17,    17,    17,    17,    17,
      17,    17,    17,    17,    17,    17,    17,   139,    87,    87,
     128,   137,  -134,    87,  -134,   143,  -134,    87,   101,   121,
     142,   141,    18,    99,    99,    90,    90,    90,    90,   114,
     114,   100,   100,  -134,  -134,  -134,   125,   133,  -134,   140,
     130,  -134,  -134,   115,  -134,  -134,   138,   147,  -134,   162,
      87,   -31,     1,   130,  -134,   169,    60,   -31,  -134,    60,
    -134,    17,    17,   163,  -134,  -134,    87,  -134,  -134,  -134,
    -134,  -134,  -134,  -134,   172,   186,     9,   170,    27,   -10,
      -4,  -134,  -134,  -134,  -134,  -134,  -134,  -134,  -134,  -134,
    -134,  -134,  -134,  -134,    87,    60,   -31,    60,    60,  -134,
      87,   -31,  -134,  -134,  -134,   171,   173,  -134,   148,    60,
     174,    11,    60,  -134,    60,  -134,   149,    87,  -134,  -134,
    -134,    17,   175,    -8,   150,  -134,    17,    60,    -6,  -134,
    -134,   155,    60,    82,  -134,    17,  -134,   157,    29,    82,
      82,  -134,   158,    82
  };

  const signed char
  Parser::yydefact_[] =
  {
       2,     0,     5,     1,     0,     0,     3,     0,     0,     5,
       7,     0,     0,     0,     6,     0,     7,     4,     0,     0,
       0,     0,     0,   118,   105,   110,   106,   107,   108,   109,
     111,   112,     0,    41,    69,    72,    74,    76,    78,    80,
      83,    88,    91,    94,    98,   102,   104,   113,   114,   116,
      39,    44,    45,    46,    47,    48,    49,    50,     0,     0,
       8,    99,   100,   101,     0,    67,     0,    65,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   115,     0,   120,     0,    66,     0,    71,    73,
      75,    77,    79,    81,    82,    84,    85,    86,    87,    89,
      90,    92,    93,    95,    96,    97,   103,     0,    40,     0,
      11,    68,   119,     0,   117,    51,     0,     0,    12,    13,
       0,     0,     0,     0,    70,    15,     0,     0,    14,     0,
     118,     0,     0,     0,    28,    29,    65,    10,    18,    19,
      23,    24,    22,    27,     0,     0,    42,     0,     0,     0,
       0,    30,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,     0,     0,     0,    16,     0,    31,
       0,     0,    52,     9,    43,     0,     0,    20,    19,     0,
       0,     0,    16,    25,     0,    32,     0,     0,    17,    21,
      26,     0,     0,     0,     0,    36,     0,     0,     0,    37,
      33,     0,     0,     0,    34,     0,    38,     0,     0,     0,
       0,    35,     0,     0
  };

  const short
  Parser::yypgoto_[] =
  {
    -134,  -134,  -133,    22,  -119,  -134,    14,  -134,  -134,  -134,
     -11,   -30,   153,   154,   159,   160,   161,    70,    81,    68,
      79,    -1,  -134,   144,  -131,  -134,  -134,   217,   211,    36,
      83,   -13,  -134,    57,    -9,  -134,   102,  -134,  -134,  -134,
    -134,  -134,  -134,  -134,  -134
  };

  const short
  Parser::yydefgoto_[] =
  {
      -1,     1,   185,   148,   149,   150,   151,    16,   152,   153,
      65,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,     6,    17,   186,
      95,    96,   174,   155,   156,   127,   128,   129,     2,    10,
     189,   212,   217,   207,   211
  };

  const unsigned char
  Parser::yytable_[] =
  {
      33,    50,    68,   147,    58,   154,   157,    69,   154,    66,
      64,    69,    12,    69,     4,    69,     5,    61,    62,    63,
     180,    18,    89,     5,     3,   136,    51,    52,    53,    54,
      55,    56,    57,    13,    11,   197,    74,    75,    90,    19,
      20,   181,   183,   137,   154,   187,   154,   154,    69,    90,
      69,    90,    97,    21,    15,   176,   195,    22,   154,   188,
     179,   154,   205,   154,   210,    23,    24,    25,    26,    27,
      28,    29,    30,    31,   209,   200,   154,   117,   118,   214,
     121,   154,   154,   113,   114,   115,   123,     9,   154,   154,
      59,    18,   154,   178,   216,   220,     7,     8,   139,    67,
     221,   222,    80,    81,   200,    84,    85,    86,   140,    19,
      20,   158,   159,    76,    77,    78,    79,    82,    83,   134,
     139,    70,   135,    21,     4,   141,    69,    22,   142,   143,
     140,    71,   144,   145,   146,    23,    24,    25,    26,    27,
      28,    29,    30,    31,   103,   104,     4,   215,   109,   110,
      73,   143,    32,    72,   144,   145,   146,   105,   106,   107,
     108,   111,   112,   182,    87,    88,    92,    91,    94,   190,
      93,   203,   191,   120,   124,    21,   208,   119,   126,    22,
     122,   125,   130,   131,   132,   218,   202,    23,    24,    25,
      26,    27,    28,    29,    30,    31,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   133,    90,
     175,   160,   193,   177,   192,   194,   199,   201,   206,   196,
     204,   213,    98,   219,    99,   223,    14,    60,   198,   161,
     100,   116,   101,   184,   102,   138
  };

  const unsigned char
  Parser::yycheck_[] =
  {
      11,    12,    32,   136,    13,   136,   139,    21,   139,    22,
      21,    21,    24,    21,    64,    21,     2,    18,    19,    20,
      24,     4,    24,     9,     0,    24,    57,    58,    59,    60,
      61,    62,    63,    45,    24,    24,    18,    19,    40,    22,
      23,    45,   175,    42,   175,   178,   177,   178,    21,    40,
      21,    40,    66,    36,    71,    46,   189,    40,   189,   178,
      70,   192,    70,   194,    70,    48,    49,    50,    51,    52,
      53,    54,    55,    56,   207,   194,   207,    88,    89,   212,
      93,   212,   213,    84,    85,    86,    97,    43,   219,   220,
      48,     4,   223,    66,   213,    66,    47,    48,    38,    36,
     219,   220,    12,    13,   223,     5,     6,     7,    48,    22,
      23,   141,   142,    14,    15,    16,    17,     3,     4,   130,
      38,    20,   131,    36,    64,    65,    21,    40,    68,    69,
      48,    10,    72,    73,    74,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    74,    75,    64,    65,    80,    81,
       9,    69,    65,    11,    72,    73,    74,    76,    77,    78,
      79,    82,    83,   174,     8,    40,    37,    45,    41,   180,
      46,   201,   181,    36,    41,    36,   206,    49,    48,    40,
      37,    41,    67,    45,    37,   215,   197,    48,    49,    50,
      51,    52,    53,    54,    55,    56,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    46,    40,
      24,    48,    39,    43,    43,    67,   194,    68,    68,    45,
      45,    66,    69,    66,    70,    67,     9,    16,   192,   146,
      71,    87,    72,   176,    73,   133
  };

  const signed char
  Parser::yystos_[] =
  {
       0,    76,   113,     0,    64,    81,   102,    47,    48,    43,
     114,    24,    24,    45,   102,    71,    82,   103,     4,    22,
      23,    36,    40,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    65,    85,    86,    87,    88,    89,    90,    91,
      92,    93,    94,    95,    96,    97,    98,    99,   100,   101,
      85,    57,    58,    59,    60,    61,    62,    63,   109,    48,
     103,    96,    96,    96,    85,    85,   106,    36,    86,    21,
      20,    10,    11,     9,    18,    19,    14,    15,    16,    17,
      12,    13,     3,     4,     5,     6,     7,     8,    40,    24,
      40,    45,    37,    46,    41,   105,   106,    66,    87,    88,
      89,    90,    91,    92,    92,    93,    93,    93,    93,    94,
      94,    95,    95,    96,    96,    96,    98,    85,    85,    49,
      36,   106,    37,    85,    41,    41,    48,   110,   111,   112,
      67,    45,    37,    46,    85,   109,    24,    42,   111,    38,
      48,    65,    68,    69,    72,    73,    74,    77,    78,    79,
      80,    81,    83,    84,    99,   108,   109,    77,    86,    86,
      48,   105,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,   107,    24,    46,    43,    66,    70,
      24,    45,    85,    77,   108,    77,   104,    77,    79,   115,
      85,   109,    43,    39,    67,    77,    45,    24,   104,    78,
      79,    68,    85,    86,    45,    70,    68,   118,    86,    77,
      70,   119,   116,    66,    77,    65,    79,   117,    86,    66,
      66,    79,    79,    67
  };

  const signed char
  Parser::yyr1_[] =
  {
       0,    75,   113,   114,    76,   102,   102,   103,   103,    82,
      82,   110,   110,   111,   111,   112,   104,   104,    77,    77,
      78,    78,    78,    79,    79,    79,    79,    79,    79,    79,
      79,   115,    83,   116,   117,    84,   118,   119,    84,    81,
      81,    81,   108,   108,   109,   109,   109,   109,   109,   109,
     109,   109,    80,   107,   107,   107,   107,   107,   107,   107,
     107,   107,   107,   107,   107,   105,   105,   106,   106,    85,
      85,    86,    86,    87,    87,    88,    88,    89,    89,    90,
      90,    91,    91,    91,    92,    92,    92,    92,    92,    93,
      93,    93,    94,    94,    94,    95,    95,    95,    95,    96,
      96,    96,    96,    97,    97,    98,    98,    98,    98,    98,
      98,    98,    98,    98,    98,    98,    98,    98,    99,   100,
     101
  };

  const signed char
  Parser::yyr2_[] =
  {
       0,     2,     0,     0,     4,     0,     3,     0,     2,    10,
       8,     0,     1,     1,     3,     3,     0,     3,     1,     1,
       4,     6,     1,     1,     1,     5,     6,     1,     1,     1,
       2,     0,     5,     0,     0,    15,     0,     0,    13,     4,
       6,     4,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     4,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     1,     1,     3,     1,
       6,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     3,     3,     1,     3,     3,     3,     3,     1,     3,
       3,     1,     3,     3,     1,     3,     3,     3,     1,     2,
       2,     2,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     1,     4,     1,     4,
       3
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
  "ASSIGNMENT", "VARDECL", "FUNCDECL", "WHILE_STMT", "FOR_STMT", "EXPR",
  "EXPR1", "EXPR2", "EXPR3", "EXPR4", "EXPR5", "EXPR6", "EXPR7", "EXPR8",
  "EXPR9", "EXPR10", "EXPR11", "EXPR12", "ATOM", "IDENTIFIER", "FUNC_CALL",
  "ARRAY_INIT", "VARDECLS", "TL_FUNCDECLS", "STMTS", "EXPR_STAR",
  "EXPR_PLUS", "OP_ASSIGN", "TYPE_PLUS", "TYPE", "PARAM_STAR",
  "PARAM_PLUS", "PARAM", "$@1", "$@2", "$@3", "$@4", "$@5", "$@6", "$@7", YY_NULLPTR
  };

#if YYDEBUG
  const short
  Parser::yyrline_[] =
  {
       0,   244,   244,   244,   244,   246,   247,   250,   251,   254,
     255,   257,   258,   261,   262,   265,   267,   268,   271,   272,
     275,   276,   277,   280,   281,   282,   283,   284,   285,   286,
     287,   290,   290,   292,   292,   292,   293,   293,   293,   296,
     297,   298,   301,   302,   306,   307,   308,   309,   310,   311,
     312,   313,   316,   319,   320,   321,   322,   323,   324,   325,
     326,   327,   328,   329,   330,   332,   333,   336,   337,   341,
     342,   346,   347,   351,   352,   356,   357,   361,   362,   366,
     367,   371,   372,   373,   377,   378,   379,   380,   381,   385,
     386,   387,   391,   392,   393,   397,   398,   399,   400,   404,
     405,   406,   407,   411,   412,   416,   417,   418,   419,   420,
     421,   422,   423,   424,   425,   426,   427,   428,   431,   432,
     433
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
#line 2788 "ramc_bison_parser.cpp"

#line 435 "ramc_grammar.yy"

namespace ramc {
	namespace bison {
		// Report an error to the user.
		auto Parser::error(const std::string& _msg) -> void
		{
			throw std::runtime_error(_msg);
		}
	}
}

