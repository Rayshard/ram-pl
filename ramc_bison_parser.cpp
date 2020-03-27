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
		    case TokenType::IDENTIFIER: { ASTIdentifier const* node = new ASTIdentifier(token.value, token.position); return Parser::make_IDENTIFIER(node); }
		    case TokenType::UNDERSCORE: return Parser::make_UNDERSCORE();
		    case TokenType::STRING_LIT: { ASTStringLit const* node = new ASTStringLit(token.value, token.position); return Parser::make_STRING_LIT(node); }
		    case TokenType::FLOAT_LIT: { ASTFloatLit const* node = new ASTFloatLit(std::stof(token.value), token.position); return Parser::make_FLOAT_LIT(node); }
		    case TokenType::DOUBLE_LIT: { ASTDoubleLit const* node = new ASTDoubleLit(std::stod(token.value), token.position); return Parser::make_DOUBLE_LIT(node); }
		    case TokenType::BYTE_LIT: { ASTByteLit const* node = new ASTByteLit((byte)std::stoll(token.value), token.position); return Parser::make_BYTE_LIT(node); }
		    case TokenType::LONG_LIT: { ASTLongLit const* node = new ASTLongLit(std::stoll(token.value), token.position); return Parser::make_LONG_LIT(node); }

		    case TokenType::KW_TRUE: { ASTBoolLit const* node = new ASTBoolLit(true, token.position); return Parser::make_KW_TRUE(node); }
		    case TokenType::KW_FALSE: { ASTBoolLit const* node = new ASTBoolLit(false, token.position); return Parser::make_KW_FALSE(node); }
		    case TokenType::KW_BYTE: { Type const* type = Type::BYTE; return Parser::make_KW_BYTE(type); }
		    case TokenType::KW_BOOL: { Type const* type = Type::BOOL; return Parser::make_KW_BOOL(type); }
		    case TokenType::KW_INT: { Type const* type = Type::INT; return Parser::make_KW_INT(type); }
		    case TokenType::KW_FLOAT: { Type const* type = Type::FLOAT; return Parser::make_KW_FLOAT(type); }
		    case TokenType::KW_DOUBLE: { Type const* type = Type::DOUBLE; return Parser::make_KW_DOUBLE(type); }
		    case TokenType::KW_LONG: { Type const* type = Type::LONG; return Parser::make_KW_LONG(type); }
		    case TokenType::KW_STRING: { Type const* type = Type::STRING; return Parser::make_KW_STRING(type); }
		    case TokenType::KW_VOID: { Type const* type = Type::VOID; return Parser::make_KW_VOID(type); }

		    case TokenType::KW_LET: return Parser::make_KW_LET(token.position);
          
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

#line 141 "ramc_bison_parser.cpp"


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
#line 214 "ramc_bison_parser.cpp"


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
      case 54: // "true"
      case 55: // "false"
        value.YY_MOVE_OR_COPY< ASTBoolLit* > (YY_MOVE (that.value));
        break;

      case 51: // "BYTE_LIT"
        value.YY_MOVE_OR_COPY< ASTByteLit* > (YY_MOVE (that.value));
        break;

      case 52: // "DOUBLE_LIT"
        value.YY_MOVE_OR_COPY< ASTDoubleLit* > (YY_MOVE (that.value));
        break;

      case 50: // "FLOAT_LIT"
        value.YY_MOVE_OR_COPY< ASTFloatLit* > (YY_MOVE (that.value));
        break;

      case 47: // "ID"
        value.YY_MOVE_OR_COPY< ASTIdentifier* > (YY_MOVE (that.value));
        break;

      case 48: // "INT_LIT"
        value.YY_MOVE_OR_COPY< ASTIntLit* > (YY_MOVE (that.value));
        break;

      case 53: // "LONG_LIT"
        value.YY_MOVE_OR_COPY< ASTLongLit* > (YY_MOVE (that.value));
        break;

      case 66: // PROGRAM
      case 67: // STMT
      case 68: // ASSIGNMENT
      case 69: // VARDECL
      case 70: // EXPR1
      case 71: // EXPR2
      case 72: // EXPR3
      case 73: // EXPR4
      case 74: // EXPR5
      case 75: // EXPR6
      case 76: // EXPR7
      case 77: // EXPR8
      case 78: // EXPR9
      case 79: // EXPR10
      case 80: // EXPR11
      case 81: // EXPR12
      case 82: // ATOM
        value.YY_MOVE_OR_COPY< ASTNode* > (YY_MOVE (that.value));
        break;

      case 49: // "STRING_LIT"
        value.YY_MOVE_OR_COPY< ASTStringLit* > (YY_MOVE (that.value));
        break;

      case 84: // OP_ASSIGN
        value.YY_MOVE_OR_COPY< AssignmentType > (YY_MOVE (that.value));
        break;

      case 64: // "let"
        value.YY_MOVE_OR_COPY< Position > (YY_MOVE (that.value));
        break;

      case 56: // "byte"
      case 57: // "bool"
      case 58: // "int"
      case 59: // "float"
      case 60: // "double"
      case 61: // "long"
      case 62: // "string"
      case 63: // "void"
        value.YY_MOVE_OR_COPY< Type* > (YY_MOVE (that.value));
        break;

      case 83: // STMTS
        value.YY_MOVE_OR_COPY< std::vector<ASTNode*> > (YY_MOVE (that.value));
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
      case 54: // "true"
      case 55: // "false"
        value.move< ASTBoolLit* > (YY_MOVE (that.value));
        break;

      case 51: // "BYTE_LIT"
        value.move< ASTByteLit* > (YY_MOVE (that.value));
        break;

      case 52: // "DOUBLE_LIT"
        value.move< ASTDoubleLit* > (YY_MOVE (that.value));
        break;

      case 50: // "FLOAT_LIT"
        value.move< ASTFloatLit* > (YY_MOVE (that.value));
        break;

      case 47: // "ID"
        value.move< ASTIdentifier* > (YY_MOVE (that.value));
        break;

      case 48: // "INT_LIT"
        value.move< ASTIntLit* > (YY_MOVE (that.value));
        break;

      case 53: // "LONG_LIT"
        value.move< ASTLongLit* > (YY_MOVE (that.value));
        break;

      case 66: // PROGRAM
      case 67: // STMT
      case 68: // ASSIGNMENT
      case 69: // VARDECL
      case 70: // EXPR1
      case 71: // EXPR2
      case 72: // EXPR3
      case 73: // EXPR4
      case 74: // EXPR5
      case 75: // EXPR6
      case 76: // EXPR7
      case 77: // EXPR8
      case 78: // EXPR9
      case 79: // EXPR10
      case 80: // EXPR11
      case 81: // EXPR12
      case 82: // ATOM
        value.move< ASTNode* > (YY_MOVE (that.value));
        break;

      case 49: // "STRING_LIT"
        value.move< ASTStringLit* > (YY_MOVE (that.value));
        break;

      case 84: // OP_ASSIGN
        value.move< AssignmentType > (YY_MOVE (that.value));
        break;

      case 64: // "let"
        value.move< Position > (YY_MOVE (that.value));
        break;

      case 56: // "byte"
      case 57: // "bool"
      case 58: // "int"
      case 59: // "float"
      case 60: // "double"
      case 61: // "long"
      case 62: // "string"
      case 63: // "void"
        value.move< Type* > (YY_MOVE (that.value));
        break;

      case 83: // STMTS
        value.move< std::vector<ASTNode*> > (YY_MOVE (that.value));
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
      case 54: // "true"
      case 55: // "false"
        value.copy< ASTBoolLit* > (that.value);
        break;

      case 51: // "BYTE_LIT"
        value.copy< ASTByteLit* > (that.value);
        break;

      case 52: // "DOUBLE_LIT"
        value.copy< ASTDoubleLit* > (that.value);
        break;

      case 50: // "FLOAT_LIT"
        value.copy< ASTFloatLit* > (that.value);
        break;

      case 47: // "ID"
        value.copy< ASTIdentifier* > (that.value);
        break;

      case 48: // "INT_LIT"
        value.copy< ASTIntLit* > (that.value);
        break;

      case 53: // "LONG_LIT"
        value.copy< ASTLongLit* > (that.value);
        break;

      case 66: // PROGRAM
      case 67: // STMT
      case 68: // ASSIGNMENT
      case 69: // VARDECL
      case 70: // EXPR1
      case 71: // EXPR2
      case 72: // EXPR3
      case 73: // EXPR4
      case 74: // EXPR5
      case 75: // EXPR6
      case 76: // EXPR7
      case 77: // EXPR8
      case 78: // EXPR9
      case 79: // EXPR10
      case 80: // EXPR11
      case 81: // EXPR12
      case 82: // ATOM
        value.copy< ASTNode* > (that.value);
        break;

      case 49: // "STRING_LIT"
        value.copy< ASTStringLit* > (that.value);
        break;

      case 84: // OP_ASSIGN
        value.copy< AssignmentType > (that.value);
        break;

      case 64: // "let"
        value.copy< Position > (that.value);
        break;

      case 56: // "byte"
      case 57: // "bool"
      case 58: // "int"
      case 59: // "float"
      case 60: // "double"
      case 61: // "long"
      case 62: // "string"
      case 63: // "void"
        value.copy< Type* > (that.value);
        break;

      case 83: // STMTS
        value.copy< std::vector<ASTNode*> > (that.value);
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
      case 54: // "true"
      case 55: // "false"
        value.move< ASTBoolLit* > (that.value);
        break;

      case 51: // "BYTE_LIT"
        value.move< ASTByteLit* > (that.value);
        break;

      case 52: // "DOUBLE_LIT"
        value.move< ASTDoubleLit* > (that.value);
        break;

      case 50: // "FLOAT_LIT"
        value.move< ASTFloatLit* > (that.value);
        break;

      case 47: // "ID"
        value.move< ASTIdentifier* > (that.value);
        break;

      case 48: // "INT_LIT"
        value.move< ASTIntLit* > (that.value);
        break;

      case 53: // "LONG_LIT"
        value.move< ASTLongLit* > (that.value);
        break;

      case 66: // PROGRAM
      case 67: // STMT
      case 68: // ASSIGNMENT
      case 69: // VARDECL
      case 70: // EXPR1
      case 71: // EXPR2
      case 72: // EXPR3
      case 73: // EXPR4
      case 74: // EXPR5
      case 75: // EXPR6
      case 76: // EXPR7
      case 77: // EXPR8
      case 78: // EXPR9
      case 79: // EXPR10
      case 80: // EXPR11
      case 81: // EXPR12
      case 82: // ATOM
        value.move< ASTNode* > (that.value);
        break;

      case 49: // "STRING_LIT"
        value.move< ASTStringLit* > (that.value);
        break;

      case 84: // OP_ASSIGN
        value.move< AssignmentType > (that.value);
        break;

      case 64: // "let"
        value.move< Position > (that.value);
        break;

      case 56: // "byte"
      case 57: // "bool"
      case 58: // "int"
      case 59: // "float"
      case 60: // "double"
      case 61: // "long"
      case 62: // "string"
      case 63: // "void"
        value.move< Type* > (that.value);
        break;

      case 83: // STMTS
        value.move< std::vector<ASTNode*> > (that.value);
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
      case 54: // "true"
      case 55: // "false"
        yylhs.value.emplace< ASTBoolLit* > ();
        break;

      case 51: // "BYTE_LIT"
        yylhs.value.emplace< ASTByteLit* > ();
        break;

      case 52: // "DOUBLE_LIT"
        yylhs.value.emplace< ASTDoubleLit* > ();
        break;

      case 50: // "FLOAT_LIT"
        yylhs.value.emplace< ASTFloatLit* > ();
        break;

      case 47: // "ID"
        yylhs.value.emplace< ASTIdentifier* > ();
        break;

      case 48: // "INT_LIT"
        yylhs.value.emplace< ASTIntLit* > ();
        break;

      case 53: // "LONG_LIT"
        yylhs.value.emplace< ASTLongLit* > ();
        break;

      case 66: // PROGRAM
      case 67: // STMT
      case 68: // ASSIGNMENT
      case 69: // VARDECL
      case 70: // EXPR1
      case 71: // EXPR2
      case 72: // EXPR3
      case 73: // EXPR4
      case 74: // EXPR5
      case 75: // EXPR6
      case 76: // EXPR7
      case 77: // EXPR8
      case 78: // EXPR9
      case 79: // EXPR10
      case 80: // EXPR11
      case 81: // EXPR12
      case 82: // ATOM
        yylhs.value.emplace< ASTNode* > ();
        break;

      case 49: // "STRING_LIT"
        yylhs.value.emplace< ASTStringLit* > ();
        break;

      case 84: // OP_ASSIGN
        yylhs.value.emplace< AssignmentType > ();
        break;

      case 64: // "let"
        yylhs.value.emplace< Position > ();
        break;

      case 56: // "byte"
      case 57: // "bool"
      case 58: // "int"
      case 59: // "float"
      case 60: // "double"
      case 61: // "long"
      case 62: // "string"
      case 63: // "void"
        yylhs.value.emplace< Type* > ();
        break;

      case 83: // STMTS
        yylhs.value.emplace< std::vector<ASTNode*> > ();
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
#line 204 "ramc_grammar.yy"
            { result = new ASTProgram("Test File", yystack_[0].value.as < std::vector<ASTNode*> > ()); }
#line 1022 "ramc_bison_parser.cpp"
    break;

  case 3:
#line 208 "ramc_grammar.yy"
                { yylhs.value.as < std::vector<ASTNode*> > () = { }; }
#line 1028 "ramc_bison_parser.cpp"
    break;

  case 4:
#line 209 "ramc_grammar.yy"
                { yystack_[1].value.as < std::vector<ASTNode*> > ().push_back(yystack_[0].value.as < ASTNode* > ()); yylhs.value.as < std::vector<ASTNode*> > () = yystack_[1].value.as < std::vector<ASTNode*> > (); }
#line 1034 "ramc_bison_parser.cpp"
    break;

  case 5:
#line 213 "ramc_grammar.yy"
                   { yylhs.value.as < ASTNode* > () = yystack_[1].value.as < ASTNode* > (); }
#line 1040 "ramc_bison_parser.cpp"
    break;

  case 6:
#line 214 "ramc_grammar.yy"
                   { yylhs.value.as < ASTNode* > () = yystack_[1].value.as < ASTNode* > (); }
#line 1046 "ramc_bison_parser.cpp"
    break;

  case 7:
#line 218 "ramc_grammar.yy"
                            { yylhs.value.as < ASTNode* > () = new ASTVarDecl(yystack_[2].value.as < ASTIdentifier* > (), yystack_[0].value.as < ASTNode* > (), yystack_[3].value.as < Position > ()); }
#line 1052 "ramc_bison_parser.cpp"
    break;

  case 8:
#line 219 "ramc_grammar.yy"
                            { yylhs.value.as < ASTNode* > () = new ASTVarDecl(yystack_[0].value.as < ASTNode* > (), yystack_[3].value.as < Position > ()); }
#line 1058 "ramc_bison_parser.cpp"
    break;

  case 9:
#line 222 "ramc_grammar.yy"
                                 { yylhs.value.as < ASTNode* > () = new ASTAssignment(yystack_[2].value.as < ASTIdentifier* > (), yystack_[0].value.as < ASTNode* > (), yystack_[1].value.as < AssignmentType > ()); }
#line 1064 "ramc_bison_parser.cpp"
    break;

  case 10:
#line 225 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::EQ; }
#line 1070 "ramc_bison_parser.cpp"
    break;

  case 11:
#line 226 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::ADD_EQ; }
#line 1076 "ramc_bison_parser.cpp"
    break;

  case 12:
#line 227 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::SUB_EQ; }
#line 1082 "ramc_bison_parser.cpp"
    break;

  case 13:
#line 228 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::MUL_EQ; }
#line 1088 "ramc_bison_parser.cpp"
    break;

  case 14:
#line 229 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::DIV_EQ; }
#line 1094 "ramc_bison_parser.cpp"
    break;

  case 15:
#line 230 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::MOD_EQ; }
#line 1100 "ramc_bison_parser.cpp"
    break;

  case 16:
#line 231 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::POW_EQ; }
#line 1106 "ramc_bison_parser.cpp"
    break;

  case 17:
#line 232 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::BIN_AND_EQ; }
#line 1112 "ramc_bison_parser.cpp"
    break;

  case 18:
#line 233 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::BIN_OR_EQ; }
#line 1118 "ramc_bison_parser.cpp"
    break;

  case 19:
#line 234 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::BIN_XOR_EQ; }
#line 1124 "ramc_bison_parser.cpp"
    break;

  case 20:
#line 235 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::LSHIFT_EQ; }
#line 1130 "ramc_bison_parser.cpp"
    break;

  case 21:
#line 236 "ramc_grammar.yy"
            { yylhs.value.as < AssignmentType > () = AssignmentType::RSHIFT_EQ; }
#line 1136 "ramc_bison_parser.cpp"
    break;

  case 22:
#line 239 "ramc_grammar.yy"
                      { yylhs.value.as < ASTNode* > () = new ASTBinopExpr(yystack_[2].value.as < ASTNode* > (), yystack_[0].value.as < ASTNode* > (), BinopType::LOG_OR); }
#line 1142 "ramc_bison_parser.cpp"
    break;

  case 23:
#line 240 "ramc_grammar.yy"
                      { yylhs.value.as < ASTNode* > () = yystack_[0].value.as < ASTNode* > (); }
#line 1148 "ramc_bison_parser.cpp"
    break;

  case 24:
#line 244 "ramc_grammar.yy"
                      { yylhs.value.as < ASTNode* > () = new ASTBinopExpr(yystack_[2].value.as < ASTNode* > (), yystack_[0].value.as < ASTNode* > (), BinopType::LOG_AND); }
#line 1154 "ramc_bison_parser.cpp"
    break;

  case 25:
#line 245 "ramc_grammar.yy"
                      { yylhs.value.as < ASTNode* > () = yystack_[0].value.as < ASTNode* > (); }
#line 1160 "ramc_bison_parser.cpp"
    break;

  case 26:
#line 249 "ramc_grammar.yy"
                      { yylhs.value.as < ASTNode* > () = new ASTBinopExpr(yystack_[2].value.as < ASTNode* > (), yystack_[0].value.as < ASTNode* > (), BinopType::BIN_OR); }
#line 1166 "ramc_bison_parser.cpp"
    break;

  case 27:
#line 250 "ramc_grammar.yy"
                      { yylhs.value.as < ASTNode* > () = yystack_[0].value.as < ASTNode* > (); }
#line 1172 "ramc_bison_parser.cpp"
    break;

  case 28:
#line 254 "ramc_grammar.yy"
                      { yylhs.value.as < ASTNode* > () = new ASTBinopExpr(yystack_[2].value.as < ASTNode* > (), yystack_[0].value.as < ASTNode* > (), BinopType::BIN_XOR); }
#line 1178 "ramc_bison_parser.cpp"
    break;

  case 29:
#line 255 "ramc_grammar.yy"
                      { yylhs.value.as < ASTNode* > () = yystack_[0].value.as < ASTNode* > (); }
#line 1184 "ramc_bison_parser.cpp"
    break;

  case 30:
#line 259 "ramc_grammar.yy"
                      { yylhs.value.as < ASTNode* > () = new ASTBinopExpr(yystack_[2].value.as < ASTNode* > (), yystack_[0].value.as < ASTNode* > (), BinopType::BIN_AND); }
#line 1190 "ramc_bison_parser.cpp"
    break;

  case 31:
#line 260 "ramc_grammar.yy"
                      { yylhs.value.as < ASTNode* > () = yystack_[0].value.as < ASTNode* > (); }
#line 1196 "ramc_bison_parser.cpp"
    break;

  case 32:
#line 264 "ramc_grammar.yy"
                       { yylhs.value.as < ASTNode* > () = new ASTBinopExpr(yystack_[2].value.as < ASTNode* > (), yystack_[0].value.as < ASTNode* > (), BinopType::EQ_EQ); }
#line 1202 "ramc_bison_parser.cpp"
    break;

  case 33:
#line 265 "ramc_grammar.yy"
                       { yylhs.value.as < ASTNode* > () = new ASTBinopExpr(yystack_[2].value.as < ASTNode* > (), yystack_[0].value.as < ASTNode* > (), BinopType::NEQ); }
#line 1208 "ramc_bison_parser.cpp"
    break;

  case 34:
#line 266 "ramc_grammar.yy"
                       { yylhs.value.as < ASTNode* > () = yystack_[0].value.as < ASTNode* > (); }
#line 1214 "ramc_bison_parser.cpp"
    break;

  case 35:
#line 270 "ramc_grammar.yy"
                        { yylhs.value.as < ASTNode* > () = new ASTBinopExpr(yystack_[2].value.as < ASTNode* > (), yystack_[0].value.as < ASTNode* > (), BinopType::LT); }
#line 1220 "ramc_bison_parser.cpp"
    break;

  case 36:
#line 271 "ramc_grammar.yy"
                        { yylhs.value.as < ASTNode* > () = new ASTBinopExpr(yystack_[2].value.as < ASTNode* > (), yystack_[0].value.as < ASTNode* > (), BinopType::GT); }
#line 1226 "ramc_bison_parser.cpp"
    break;

  case 37:
#line 272 "ramc_grammar.yy"
                        { yylhs.value.as < ASTNode* > () = new ASTBinopExpr(yystack_[2].value.as < ASTNode* > (), yystack_[0].value.as < ASTNode* > (), BinopType::LT_EQ); }
#line 1232 "ramc_bison_parser.cpp"
    break;

  case 38:
#line 273 "ramc_grammar.yy"
                        { yylhs.value.as < ASTNode* > () = new ASTBinopExpr(yystack_[2].value.as < ASTNode* > (), yystack_[0].value.as < ASTNode* > (), BinopType::GT_EQ); }
#line 1238 "ramc_bison_parser.cpp"
    break;

  case 39:
#line 274 "ramc_grammar.yy"
                        { yylhs.value.as < ASTNode* > () = yystack_[0].value.as < ASTNode* > (); }
#line 1244 "ramc_bison_parser.cpp"
    break;

  case 40:
#line 278 "ramc_grammar.yy"
                       { yylhs.value.as < ASTNode* > () = new ASTBinopExpr(yystack_[2].value.as < ASTNode* > (), yystack_[0].value.as < ASTNode* > (), BinopType::LSHIFT); }
#line 1250 "ramc_bison_parser.cpp"
    break;

  case 41:
#line 279 "ramc_grammar.yy"
                       { yylhs.value.as < ASTNode* > () = new ASTBinopExpr(yystack_[2].value.as < ASTNode* > (), yystack_[0].value.as < ASTNode* > (), BinopType::RSHIFT); }
#line 1256 "ramc_bison_parser.cpp"
    break;

  case 42:
#line 280 "ramc_grammar.yy"
                       { yylhs.value.as < ASTNode* > () = yystack_[0].value.as < ASTNode* > (); }
#line 1262 "ramc_bison_parser.cpp"
    break;

  case 43:
#line 284 "ramc_grammar.yy"
                       { yylhs.value.as < ASTNode* > () = new ASTBinopExpr(yystack_[2].value.as < ASTNode* > (), yystack_[0].value.as < ASTNode* > (), BinopType::ADD); }
#line 1268 "ramc_bison_parser.cpp"
    break;

  case 44:
#line 285 "ramc_grammar.yy"
                       { yylhs.value.as < ASTNode* > () = new ASTBinopExpr(yystack_[2].value.as < ASTNode* > (), yystack_[0].value.as < ASTNode* > (), BinopType::SUB); }
#line 1274 "ramc_bison_parser.cpp"
    break;

  case 45:
#line 286 "ramc_grammar.yy"
                       { yylhs.value.as < ASTNode* > () = yystack_[0].value.as < ASTNode* > (); }
#line 1280 "ramc_bison_parser.cpp"
    break;

  case 46:
#line 290 "ramc_grammar.yy"
                        { yylhs.value.as < ASTNode* > () = new ASTBinopExpr(yystack_[2].value.as < ASTNode* > (), yystack_[0].value.as < ASTNode* > (), BinopType::MUL); }
#line 1286 "ramc_bison_parser.cpp"
    break;

  case 47:
#line 291 "ramc_grammar.yy"
                        { yylhs.value.as < ASTNode* > () = new ASTBinopExpr(yystack_[2].value.as < ASTNode* > (), yystack_[0].value.as < ASTNode* > (), BinopType::DIV); }
#line 1292 "ramc_bison_parser.cpp"
    break;

  case 48:
#line 292 "ramc_grammar.yy"
                        { yylhs.value.as < ASTNode* > () = new ASTBinopExpr(yystack_[2].value.as < ASTNode* > (), yystack_[0].value.as < ASTNode* > (), BinopType::MOD); }
#line 1298 "ramc_bison_parser.cpp"
    break;

  case 49:
#line 293 "ramc_grammar.yy"
                        { yylhs.value.as < ASTNode* > () = yystack_[0].value.as < ASTNode* > (); }
#line 1304 "ramc_bison_parser.cpp"
    break;

  case 50:
#line 297 "ramc_grammar.yy"
                 { yylhs.value.as < ASTNode* > () = new ASTUnopExpr(yystack_[0].value.as < ASTNode* > (), UnopType::NEG); }
#line 1310 "ramc_bison_parser.cpp"
    break;

  case 51:
#line 298 "ramc_grammar.yy"
                 { yylhs.value.as < ASTNode* > () = new ASTUnopExpr(yystack_[0].value.as < ASTNode* > (), UnopType::LOG_NOT); }
#line 1316 "ramc_bison_parser.cpp"
    break;

  case 52:
#line 299 "ramc_grammar.yy"
                 { yylhs.value.as < ASTNode* > () = new ASTUnopExpr(yystack_[0].value.as < ASTNode* > (), UnopType::BIN_NOT); }
#line 1322 "ramc_bison_parser.cpp"
    break;

  case 53:
#line 300 "ramc_grammar.yy"
                 { yylhs.value.as < ASTNode* > () = yystack_[0].value.as < ASTNode* > (); }
#line 1328 "ramc_bison_parser.cpp"
    break;

  case 54:
#line 304 "ramc_grammar.yy"
                       { yylhs.value.as < ASTNode* > () = new ASTBinopExpr(yystack_[2].value.as < ASTNode* > (), yystack_[0].value.as < ASTNode* > (), BinopType::POW); }
#line 1334 "ramc_bison_parser.cpp"
    break;

  case 55:
#line 305 "ramc_grammar.yy"
                       { yylhs.value.as < ASTNode* > () = yystack_[0].value.as < ASTNode* > (); }
#line 1340 "ramc_bison_parser.cpp"
    break;

  case 56:
#line 309 "ramc_grammar.yy"
                    { yylhs.value.as < ASTNode* > () = yystack_[0].value.as < ASTIntLit* > (); }
#line 1346 "ramc_bison_parser.cpp"
    break;

  case 57:
#line 310 "ramc_grammar.yy"
                    { yylhs.value.as < ASTNode* > () = yystack_[0].value.as < ASTFloatLit* > (); }
#line 1352 "ramc_bison_parser.cpp"
    break;

  case 58:
#line 311 "ramc_grammar.yy"
                    { yylhs.value.as < ASTNode* > () = yystack_[0].value.as < ASTByteLit* > (); }
#line 1358 "ramc_bison_parser.cpp"
    break;

  case 59:
#line 312 "ramc_grammar.yy"
                    { yylhs.value.as < ASTNode* > () = yystack_[0].value.as < ASTDoubleLit* > (); }
#line 1364 "ramc_bison_parser.cpp"
    break;

  case 60:
#line 313 "ramc_grammar.yy"
                    { yylhs.value.as < ASTNode* > () = yystack_[0].value.as < ASTLongLit* > (); }
#line 1370 "ramc_bison_parser.cpp"
    break;

  case 61:
#line 314 "ramc_grammar.yy"
                    { yylhs.value.as < ASTNode* > () = yystack_[0].value.as < ASTStringLit* > (); }
#line 1376 "ramc_bison_parser.cpp"
    break;

  case 62:
#line 315 "ramc_grammar.yy"
                    { yylhs.value.as < ASTNode* > () = yystack_[0].value.as < ASTBoolLit* > (); }
#line 1382 "ramc_bison_parser.cpp"
    break;

  case 63:
#line 316 "ramc_grammar.yy"
                    { yylhs.value.as < ASTNode* > () = yystack_[0].value.as < ASTBoolLit* > (); }
#line 1388 "ramc_bison_parser.cpp"
    break;

  case 64:
#line 317 "ramc_grammar.yy"
                    { yylhs.value.as < ASTNode* > () = yystack_[0].value.as < ASTIdentifier* > (); }
#line 1394 "ramc_bison_parser.cpp"
    break;

  case 65:
#line 318 "ramc_grammar.yy"
                    { yylhs.value.as < ASTNode* > () = yystack_[1].value.as < ASTNode* > (); }
#line 1400 "ramc_bison_parser.cpp"
    break;


#line 1404 "ramc_bison_parser.cpp"

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


  const signed char Parser::yypact_ninf_ = -45;

  const signed char Parser::yytable_ninf_ = -1;

  const signed char
  Parser::yypact_[] =
  {
     -45,     2,   -44,   -45,    31,    21,   -45,   -19,     0,   -45,
     -45,   -45,   -45,   -45,   -45,   -45,   -45,   -45,   -45,   -45,
     -45,    -4,   -20,    49,   -45,   -45,    -4,    -4,    -4,    -4,
     -45,   -45,   -45,   -45,   -45,   -45,   -45,   -45,   -45,    60,
       1,    72,    20,    69,   -10,    -3,    57,    68,    10,   -45,
      75,   -45,    -4,    -4,   -45,   -45,   -45,   -11,    -4,    -4,
      -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,    -4,
      -4,    -4,    -4,    -4,    -4,    -4,   -14,    60,    60,   -45,
       1,    72,    20,    69,   -10,    -3,    -3,    57,    57,    57,
      57,    68,    68,    10,    10,   -45,   -45,   -45,   -45
  };

  const signed char
  Parser::yydefact_[] =
  {
       3,     0,     2,     1,     0,     0,     4,     0,     0,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,     0,     0,     0,     5,     6,     0,     0,     0,     0,
      64,    56,    61,    57,    58,    59,    60,    62,    63,     9,
      23,    25,    27,    29,    31,    34,    39,    42,    45,    49,
      53,    55,     0,     0,    50,    51,    52,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     8,     7,    65,
      22,    24,    26,    28,    30,    32,    33,    35,    36,    37,
      38,    40,    41,    43,    44,    46,    47,    48,    54
  };

  const signed char
  Parser::yypgoto_[] =
  {
     -45,   -45,   -45,   -45,   -45,   -28,    26,    27,    25,    28,
      29,    11,   -38,     7,     8,   -21,   -45,    12,   -45,   -45
  };

  const signed char
  Parser::yydefgoto_[] =
  {
      -1,     1,     6,     7,     8,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,     2,    21
  };

  const signed char
  Parser::yytable_[] =
  {
      26,    57,     3,     4,    52,    54,    55,    56,    63,    64,
      58,    65,    66,    67,    68,    73,    74,    75,    27,    28,
       5,    59,    29,    24,    77,    78,    79,    87,    88,    89,
      90,    61,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    25,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    95,    96,    97,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    18,    19,    20,    22,    23,    69,
      70,    71,    72,    53,    85,    86,    91,    92,    62,    93,
      94,    58,    60,    76,    80,    82,    81,     0,    98,    83,
       0,    84
  };

  const signed char
  Parser::yycheck_[] =
  {
       4,    29,     0,    47,    24,    26,    27,    28,    18,    19,
      21,    14,    15,    16,    17,     5,     6,     7,    22,    23,
      64,    20,    36,    42,    52,    53,    37,    65,    66,    67,
      68,    11,    36,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    42,    47,    48,    49,    50,    51,    52,    53,
      54,    55,    73,    74,    75,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    46,    47,    12,
      13,     3,     4,    24,    63,    64,    69,    70,     9,    71,
      72,    21,    10,     8,    58,    60,    59,    -1,    76,    61,
      -1,    62
  };

  const signed char
  Parser::yystos_[] =
  {
       0,    66,    83,     0,    47,    64,    67,    68,    69,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    84,    46,    47,    42,    42,     4,    22,    23,    36,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    82,    24,    24,    80,    80,    80,    70,    21,    20,
      10,    11,     9,    18,    19,    14,    15,    16,    17,    12,
      13,     3,     4,     5,     6,     7,     8,    70,    70,    37,
      71,    72,    73,    74,    75,    76,    76,    77,    77,    77,
      77,    78,    78,    79,    79,    80,    80,    80,    82
  };

  const signed char
  Parser::yyr1_[] =
  {
       0,    65,    66,    83,    83,    67,    67,    69,    69,    68,
      84,    84,    84,    84,    84,    84,    84,    84,    84,    84,
      84,    84,    70,    70,    71,    71,    72,    72,    73,    73,
      74,    74,    75,    75,    75,    76,    76,    76,    76,    76,
      77,    77,    77,    78,    78,    78,    79,    79,    79,    79,
      80,    80,    80,    80,    81,    81,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82
  };

  const signed char
  Parser::yyr2_[] =
  {
       0,     2,     1,     0,     2,     2,     2,     4,     4,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     3,     3,     1,     3,     3,     3,     3,     1,
       3,     3,     1,     3,     3,     1,     3,     3,     3,     1,
       2,     2,     2,     1,     3,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3
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
  "\"<<=\"", "\">>=\"", "\"(\"", "\")\"", "\"}\"", "\"{\"", "\"[\"",
  "\"]\"", "\";\"", "\".\"", "\":\"", "\",\"", "\"_\"", "\"ID\"",
  "\"INT_LIT\"", "\"STRING_LIT\"", "\"FLOAT_LIT\"", "\"BYTE_LIT\"",
  "\"DOUBLE_LIT\"", "\"LONG_LIT\"", "\"true\"", "\"false\"", "\"byte\"",
  "\"bool\"", "\"int\"", "\"float\"", "\"double\"", "\"long\"",
  "\"string\"", "\"void\"", "\"let\"", "$accept", "PROGRAM", "STMT",
  "ASSIGNMENT", "VARDECL", "EXPR1", "EXPR2", "EXPR3", "EXPR4", "EXPR5",
  "EXPR6", "EXPR7", "EXPR8", "EXPR9", "EXPR10", "EXPR11", "EXPR12", "ATOM",
  "STMTS", "OP_ASSIGN", YY_NULLPTR
  };

#if YYDEBUG
  const short
  Parser::yyrline_[] =
  {
       0,   204,   204,   208,   209,   213,   214,   218,   219,   222,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     235,   236,   239,   240,   244,   245,   249,   250,   254,   255,
     259,   260,   264,   265,   266,   270,   271,   272,   273,   274,
     278,   279,   280,   284,   285,   286,   290,   291,   292,   293,
     297,   298,   299,   300,   304,   305,   309,   310,   311,   312,
     313,   314,   315,   316,   317,   318
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
#line 1994 "ramc_bison_parser.cpp"

#line 321 "ramc_grammar.yy"

namespace ramc {
	namespace bison {
		// Report an error to the user.
		auto Parser::error(const std::string& _msg) -> void
		{
			throw std::runtime_error(_msg);
		}
	}
}

