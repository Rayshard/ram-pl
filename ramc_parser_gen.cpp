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
#line 9 "ramc_grammar.yy"

  #include "pch.h"
  #include "ramc_lexer.h"
  #include "ramc_position.h"

  using namespace ramc;

#line 46 "ramc_parser_gen.cpp"




#include "ramc_parser_gen.hpp"


// Unqualified %code blocks.
#line 21 "ramc_grammar.yy"

  namespace yy
  {
    // Return the next token.
    auto yylex(Lexer& _lexer, ASTNode*& _result, Position& _pos) -> parser::symbol_type
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
          return parser::make_INT_LIT(node);
        }
        case TokenType::IDENTIFIER: {
            ASTIdentifier const* node = new ASTIdentifier(token.value, token.position);
            return parser::make_IDENTIFIER(node);
        }
        case TokenType::UNDERSCORE: return parser::make_UNDERSCORE();
        case TokenType::STRING_LIT: {
            ASTStringLit const* node = new ASTStringLit(token.value, token.position);
            return parser::make_STRING_LIT(node);
        }
        case TokenType::FLOAT_LIT: {
            ASTFloatLit const* node = new ASTFloatLit(std::stof(token.value), token.position);
            return parser::make_FLOAT_LIT(node);
        }
        case TokenType::KW_TRUE: {
            ASTBoolLit const* node = new ASTBoolLit(true, token.position);
            return parser::make_KW_TRUE(node);
        }
        case TokenType::KW_FALSE: {
            ASTBoolLit const* node = new ASTBoolLit(false, token.position);
            return parser::make_KW_FALSE(node);
        }
        case TokenType::PLUS: return parser::make_PLUS();
        case TokenType::MINUS: return parser::make_MINUS();
        case TokenType::TIMES: return parser::make_TIMES();
        case TokenType::DIVIDE: return parser::make_DIVIDE();
        case TokenType::MOD: return parser::make_MOD();
        case TokenType::POW: return parser::make_POW();
        case TokenType::BIN_AND: return parser::make_BIN_AND();
        case TokenType::BIN_OR: return parser::make_BIN_OR();
        case TokenType::BIN_XOR: return parser::make_BIN_XOR();
        case TokenType::LSHIFT: return parser::make_LSHIFT();
        case TokenType::RSHIFT: return parser::make_RSHIFT();
        case TokenType::LT: return parser::make_LT();
        case TokenType::GT: return parser::make_GT();
        case TokenType::LT_EQ: return parser::make_LT_EQ();
        case TokenType::GT_EQ: return parser::make_GT_EQ();
        case TokenType::EQ_EQ: return parser::make_EQ_EQ();
        case TokenType::NEQ: return parser::make_NEQ();
        case TokenType::LOG_AND: return parser::make_LOG_AND();
        case TokenType::LOG_OR: return parser::make_LOG_OR();
        case TokenType::LOG_NOT: return parser::make_LOG_NOT();
        case TokenType::EQ: return parser::make_EQ();
        case TokenType::PLUS_EQ: return parser::make_ADD_EQ();
        case TokenType::MINUS_EQ: return parser::make_SUB_EQ();
        case TokenType::TIMES_EQ: return parser::make_MUL_EQ();
        case TokenType::DIVIDE_EQ: return parser::make_DIV_EQ();
        case TokenType::MOD_EQ: return parser::make_MOD_EQ();
        case TokenType::POW_EQ: return parser::make_POW_EQ();
        case TokenType::BIN_AND_EQ: return parser::make_BIN_AND_EQ();
        case TokenType::BIN_OR_EQ: return parser::make_BIN_OR_EQ();
        case TokenType::BIN_XOR_EQ: return parser::make_BIN_XOR_EQ();
        case TokenType::LSHIFT_EQ: return parser::make_LSHIFT_EQ();
        case TokenType::RSHIFT_EQ: return parser::make_RSHIFT_EQ();
        case TokenType::INC: return parser::make_INC();
        case TokenType::DEC: return parser::make_DEC();
        case TokenType::LPAREN: return parser::make_LPAREN();
        case TokenType::RPAREN: return parser::make_RPAREN();
        case TokenType::LCBRACKET: return parser::make_LCBRACKET();
        case TokenType::RCBRACKET: return parser::make_RCBRACKET();
        case TokenType::LSBRACKET: return parser::make_LSBRACKET();
        case TokenType::RSBRACKET: return parser::make_RSBRACKET();
        case TokenType::SEMICOLON: return parser::make_SEMICOLON();
        case TokenType::PERIOD: return parser::make_PERIOD();
        case TokenType::COLON: return parser::make_COLON();
        case TokenType::COMMA: return parser::make_COMMA();
        case TokenType::END_OF_FILE: return parser::make_END_OF_FILE();
        default: throw std::runtime_error(token.ToString(true) + " is not parasble!");
      }
    }
  }

#line 147 "ramc_parser_gen.cpp"


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

namespace yy {
#line 219 "ramc_parser_gen.cpp"


  /// Build a parser object.
  parser::parser (ramc::Lexer& lexer_yyarg, ramc::ASTNode*& result_yyarg, ramc::Position& position_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      lexer (lexer_yyarg),
      result (result_yyarg),
      position (position_yyarg)
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_number_type
  parser::by_state::type_get () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state))
  {
    switch (that.type_get ())
    {
      case 52: // "true"
      case 53: // "false"
        value.YY_MOVE_OR_COPY< ramc::ASTBoolLit* > (YY_MOVE (that.value));
        break;

      case 51: // "FLOAT_LIT"
        value.YY_MOVE_OR_COPY< ramc::ASTFloatLit* > (YY_MOVE (that.value));
        break;

      case 48: // "ID"
        value.YY_MOVE_OR_COPY< ramc::ASTIdentifier* > (YY_MOVE (that.value));
        break;

      case 49: // "INT_LIT"
        value.YY_MOVE_OR_COPY< ramc::ASTIntLit* > (YY_MOVE (that.value));
        break;

      case 55: // PROGRAM
      case 56: // STMT
      case 57: // ASSIGNMENT
      case 58: // EXPR1
      case 59: // EXPR2
      case 60: // EXPR3
      case 61: // EXPR4
      case 62: // EXPR5
      case 63: // EXPR6
      case 64: // EXPR7
      case 65: // EXPR8
      case 66: // EXPR9
      case 67: // EXPR10
      case 68: // EXPR11
      case 69: // EXPR12
      case 70: // EXPR13
      case 71: // EXPR14
      case 72: // ATOM
        value.YY_MOVE_OR_COPY< ramc::ASTNode* > (YY_MOVE (that.value));
        break;

      case 50: // "STRING_LIT"
        value.YY_MOVE_OR_COPY< ramc::ASTStringLit* > (YY_MOVE (that.value));
        break;

      case 74: // OP_ASSIGN
        value.YY_MOVE_OR_COPY< ramc::BinopType > (YY_MOVE (that.value));
        break;

      case 73: // STMTS
        value.YY_MOVE_OR_COPY< std::vector<ramc::ASTNode*> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s)
  {
    switch (that.type_get ())
    {
      case 52: // "true"
      case 53: // "false"
        value.move< ramc::ASTBoolLit* > (YY_MOVE (that.value));
        break;

      case 51: // "FLOAT_LIT"
        value.move< ramc::ASTFloatLit* > (YY_MOVE (that.value));
        break;

      case 48: // "ID"
        value.move< ramc::ASTIdentifier* > (YY_MOVE (that.value));
        break;

      case 49: // "INT_LIT"
        value.move< ramc::ASTIntLit* > (YY_MOVE (that.value));
        break;

      case 55: // PROGRAM
      case 56: // STMT
      case 57: // ASSIGNMENT
      case 58: // EXPR1
      case 59: // EXPR2
      case 60: // EXPR3
      case 61: // EXPR4
      case 62: // EXPR5
      case 63: // EXPR6
      case 64: // EXPR7
      case 65: // EXPR8
      case 66: // EXPR9
      case 67: // EXPR10
      case 68: // EXPR11
      case 69: // EXPR12
      case 70: // EXPR13
      case 71: // EXPR14
      case 72: // ATOM
        value.move< ramc::ASTNode* > (YY_MOVE (that.value));
        break;

      case 50: // "STRING_LIT"
        value.move< ramc::ASTStringLit* > (YY_MOVE (that.value));
        break;

      case 74: // OP_ASSIGN
        value.move< ramc::BinopType > (YY_MOVE (that.value));
        break;

      case 73: // STMTS
        value.move< std::vector<ramc::ASTNode*> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 52: // "true"
      case 53: // "false"
        value.copy< ramc::ASTBoolLit* > (that.value);
        break;

      case 51: // "FLOAT_LIT"
        value.copy< ramc::ASTFloatLit* > (that.value);
        break;

      case 48: // "ID"
        value.copy< ramc::ASTIdentifier* > (that.value);
        break;

      case 49: // "INT_LIT"
        value.copy< ramc::ASTIntLit* > (that.value);
        break;

      case 55: // PROGRAM
      case 56: // STMT
      case 57: // ASSIGNMENT
      case 58: // EXPR1
      case 59: // EXPR2
      case 60: // EXPR3
      case 61: // EXPR4
      case 62: // EXPR5
      case 63: // EXPR6
      case 64: // EXPR7
      case 65: // EXPR8
      case 66: // EXPR9
      case 67: // EXPR10
      case 68: // EXPR11
      case 69: // EXPR12
      case 70: // EXPR13
      case 71: // EXPR14
      case 72: // ATOM
        value.copy< ramc::ASTNode* > (that.value);
        break;

      case 50: // "STRING_LIT"
        value.copy< ramc::ASTStringLit* > (that.value);
        break;

      case 74: // OP_ASSIGN
        value.copy< ramc::BinopType > (that.value);
        break;

      case 73: // STMTS
        value.copy< std::vector<ramc::ASTNode*> > (that.value);
        break;

      default:
        break;
    }

    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      case 52: // "true"
      case 53: // "false"
        value.move< ramc::ASTBoolLit* > (that.value);
        break;

      case 51: // "FLOAT_LIT"
        value.move< ramc::ASTFloatLit* > (that.value);
        break;

      case 48: // "ID"
        value.move< ramc::ASTIdentifier* > (that.value);
        break;

      case 49: // "INT_LIT"
        value.move< ramc::ASTIntLit* > (that.value);
        break;

      case 55: // PROGRAM
      case 56: // STMT
      case 57: // ASSIGNMENT
      case 58: // EXPR1
      case 59: // EXPR2
      case 60: // EXPR3
      case 61: // EXPR4
      case 62: // EXPR5
      case 63: // EXPR6
      case 64: // EXPR7
      case 65: // EXPR8
      case 66: // EXPR9
      case 67: // EXPR10
      case 68: // EXPR11
      case 69: // EXPR12
      case 70: // EXPR13
      case 71: // EXPR14
      case 72: // ATOM
        value.move< ramc::ASTNode* > (that.value);
        break;

      case 50: // "STRING_LIT"
        value.move< ramc::ASTStringLit* > (that.value);
        break;

      case 74: // OP_ASSIGN
        value.move< ramc::BinopType > (that.value);
        break;

      case 73: // STMTS
        value.move< std::vector<ramc::ASTNode*> > (that.value);
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
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo,
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
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
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
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", static_cast<state_type> (yyn), YY_MOVE (yyla));
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
      case 52: // "true"
      case 53: // "false"
        yylhs.value.emplace< ramc::ASTBoolLit* > ();
        break;

      case 51: // "FLOAT_LIT"
        yylhs.value.emplace< ramc::ASTFloatLit* > ();
        break;

      case 48: // "ID"
        yylhs.value.emplace< ramc::ASTIdentifier* > ();
        break;

      case 49: // "INT_LIT"
        yylhs.value.emplace< ramc::ASTIntLit* > ();
        break;

      case 55: // PROGRAM
      case 56: // STMT
      case 57: // ASSIGNMENT
      case 58: // EXPR1
      case 59: // EXPR2
      case 60: // EXPR3
      case 61: // EXPR4
      case 62: // EXPR5
      case 63: // EXPR6
      case 64: // EXPR7
      case 65: // EXPR8
      case 66: // EXPR9
      case 67: // EXPR10
      case 68: // EXPR11
      case 69: // EXPR12
      case 70: // EXPR13
      case 71: // EXPR14
      case 72: // ATOM
        yylhs.value.emplace< ramc::ASTNode* > ();
        break;

      case 50: // "STRING_LIT"
        yylhs.value.emplace< ramc::ASTStringLit* > ();
        break;

      case 74: // OP_ASSIGN
        yylhs.value.emplace< ramc::BinopType > ();
        break;

      case 73: // STMTS
        yylhs.value.emplace< std::vector<ramc::ASTNode*> > ();
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
#line 195 "ramc_grammar.yy"
            { result = new ramc::ASTProgram("Test File", yystack_[0].value.as < std::vector<ramc::ASTNode*> > ()); }
#line 845 "ramc_parser_gen.cpp"
    break;

  case 3:
#line 199 "ramc_grammar.yy"
                { yylhs.value.as < std::vector<ramc::ASTNode*> > () = { }; }
#line 851 "ramc_parser_gen.cpp"
    break;

  case 4:
#line 200 "ramc_grammar.yy"
                { yystack_[1].value.as < std::vector<ramc::ASTNode*> > ().push_back(yystack_[0].value.as < ramc::ASTNode* > ()); yylhs.value.as < std::vector<ramc::ASTNode*> > () = yystack_[1].value.as < std::vector<ramc::ASTNode*> > (); }
#line 857 "ramc_parser_gen.cpp"
    break;

  case 5:
#line 204 "ramc_grammar.yy"
                   { yylhs.value.as < ramc::ASTNode* > () = yystack_[1].value.as < ramc::ASTNode* > (); }
#line 863 "ramc_parser_gen.cpp"
    break;

  case 6:
#line 205 "ramc_grammar.yy"
                   { yylhs.value.as < ramc::ASTNode* > () = yystack_[1].value.as < ramc::ASTNode* > (); }
#line 869 "ramc_parser_gen.cpp"
    break;

  case 7:
#line 209 "ramc_grammar.yy"
                            { yylhs.value.as < ramc::ASTNode* > () = new ramc::ASTBinopExpr(yystack_[2].value.as < ramc::ASTIdentifier* > (), yystack_[0].value.as < ramc::ASTNode* > (), yystack_[1].value.as < ramc::BinopType > ()); }
#line 875 "ramc_parser_gen.cpp"
    break;

  case 8:
#line 210 "ramc_grammar.yy"
                            { yylhs.value.as < ramc::ASTNode* > () = yystack_[0].value.as < ramc::ASTNode* > (); }
#line 881 "ramc_parser_gen.cpp"
    break;

  case 9:
#line 214 "ramc_grammar.yy"
            { yylhs.value.as < ramc::BinopType > () = ramc::BinopType::EQ; }
#line 887 "ramc_parser_gen.cpp"
    break;

  case 10:
#line 215 "ramc_grammar.yy"
            { yylhs.value.as < ramc::BinopType > () = ramc::BinopType::ADD_EQ; }
#line 893 "ramc_parser_gen.cpp"
    break;

  case 11:
#line 216 "ramc_grammar.yy"
            { yylhs.value.as < ramc::BinopType > () = ramc::BinopType::SUB_EQ; }
#line 899 "ramc_parser_gen.cpp"
    break;

  case 12:
#line 217 "ramc_grammar.yy"
            { yylhs.value.as < ramc::BinopType > () = ramc::BinopType::MUL_EQ; }
#line 905 "ramc_parser_gen.cpp"
    break;

  case 13:
#line 218 "ramc_grammar.yy"
            { yylhs.value.as < ramc::BinopType > () = ramc::BinopType::DIV_EQ; }
#line 911 "ramc_parser_gen.cpp"
    break;

  case 14:
#line 219 "ramc_grammar.yy"
            { yylhs.value.as < ramc::BinopType > () = ramc::BinopType::MOD_EQ; }
#line 917 "ramc_parser_gen.cpp"
    break;

  case 15:
#line 220 "ramc_grammar.yy"
            { yylhs.value.as < ramc::BinopType > () = ramc::BinopType::POW_EQ; }
#line 923 "ramc_parser_gen.cpp"
    break;

  case 16:
#line 221 "ramc_grammar.yy"
            { yylhs.value.as < ramc::BinopType > () = ramc::BinopType::BIN_AND_EQ; }
#line 929 "ramc_parser_gen.cpp"
    break;

  case 17:
#line 222 "ramc_grammar.yy"
            { yylhs.value.as < ramc::BinopType > () = ramc::BinopType::BIN_OR_EQ; }
#line 935 "ramc_parser_gen.cpp"
    break;

  case 18:
#line 223 "ramc_grammar.yy"
            { yylhs.value.as < ramc::BinopType > () = ramc::BinopType::BIN_XOR_EQ; }
#line 941 "ramc_parser_gen.cpp"
    break;

  case 19:
#line 224 "ramc_grammar.yy"
            { yylhs.value.as < ramc::BinopType > () = ramc::BinopType::LSHIFT_EQ; }
#line 947 "ramc_parser_gen.cpp"
    break;

  case 20:
#line 225 "ramc_grammar.yy"
            { yylhs.value.as < ramc::BinopType > () = ramc::BinopType::RSHIFT_EQ; }
#line 953 "ramc_parser_gen.cpp"
    break;

  case 21:
#line 228 "ramc_grammar.yy"
                      { yylhs.value.as < ramc::ASTNode* > () = new ramc::ASTBinopExpr(yystack_[2].value.as < ramc::ASTNode* > (), yystack_[0].value.as < ramc::ASTNode* > (), ramc::BinopType::LOG_OR); }
#line 959 "ramc_parser_gen.cpp"
    break;

  case 22:
#line 229 "ramc_grammar.yy"
                      { yylhs.value.as < ramc::ASTNode* > () = yystack_[0].value.as < ramc::ASTNode* > (); }
#line 965 "ramc_parser_gen.cpp"
    break;

  case 23:
#line 233 "ramc_grammar.yy"
                      { yylhs.value.as < ramc::ASTNode* > () = new ramc::ASTBinopExpr(yystack_[2].value.as < ramc::ASTNode* > (), yystack_[0].value.as < ramc::ASTNode* > (), ramc::BinopType::LOG_AND); }
#line 971 "ramc_parser_gen.cpp"
    break;

  case 24:
#line 234 "ramc_grammar.yy"
                      { yylhs.value.as < ramc::ASTNode* > () = yystack_[0].value.as < ramc::ASTNode* > (); }
#line 977 "ramc_parser_gen.cpp"
    break;

  case 25:
#line 238 "ramc_grammar.yy"
                      { yylhs.value.as < ramc::ASTNode* > () = new ramc::ASTBinopExpr(yystack_[2].value.as < ramc::ASTNode* > (), yystack_[0].value.as < ramc::ASTNode* > (), ramc::BinopType::BIN_OR); }
#line 983 "ramc_parser_gen.cpp"
    break;

  case 26:
#line 239 "ramc_grammar.yy"
                      { yylhs.value.as < ramc::ASTNode* > () = yystack_[0].value.as < ramc::ASTNode* > (); }
#line 989 "ramc_parser_gen.cpp"
    break;

  case 27:
#line 243 "ramc_grammar.yy"
                      { yylhs.value.as < ramc::ASTNode* > () = new ramc::ASTBinopExpr(yystack_[2].value.as < ramc::ASTNode* > (), yystack_[0].value.as < ramc::ASTNode* > (), ramc::BinopType::BIN_XOR); }
#line 995 "ramc_parser_gen.cpp"
    break;

  case 28:
#line 244 "ramc_grammar.yy"
                      { yylhs.value.as < ramc::ASTNode* > () = yystack_[0].value.as < ramc::ASTNode* > (); }
#line 1001 "ramc_parser_gen.cpp"
    break;

  case 29:
#line 248 "ramc_grammar.yy"
                      { yylhs.value.as < ramc::ASTNode* > () = new ramc::ASTBinopExpr(yystack_[2].value.as < ramc::ASTNode* > (), yystack_[0].value.as < ramc::ASTNode* > (), ramc::BinopType::BIN_AND); }
#line 1007 "ramc_parser_gen.cpp"
    break;

  case 30:
#line 249 "ramc_grammar.yy"
                      { yylhs.value.as < ramc::ASTNode* > () = yystack_[0].value.as < ramc::ASTNode* > (); }
#line 1013 "ramc_parser_gen.cpp"
    break;

  case 31:
#line 253 "ramc_grammar.yy"
                       { yylhs.value.as < ramc::ASTNode* > () = new ramc::ASTBinopExpr(yystack_[2].value.as < ramc::ASTNode* > (), yystack_[0].value.as < ramc::ASTNode* > (), ramc::BinopType::EQ_EQ); }
#line 1019 "ramc_parser_gen.cpp"
    break;

  case 32:
#line 254 "ramc_grammar.yy"
                       { yylhs.value.as < ramc::ASTNode* > () = new ramc::ASTBinopExpr(yystack_[2].value.as < ramc::ASTNode* > (), yystack_[0].value.as < ramc::ASTNode* > (), ramc::BinopType::NEQ); }
#line 1025 "ramc_parser_gen.cpp"
    break;

  case 33:
#line 255 "ramc_grammar.yy"
                       { yylhs.value.as < ramc::ASTNode* > () = yystack_[0].value.as < ramc::ASTNode* > (); }
#line 1031 "ramc_parser_gen.cpp"
    break;

  case 34:
#line 259 "ramc_grammar.yy"
                        { yylhs.value.as < ramc::ASTNode* > () = new ramc::ASTBinopExpr(yystack_[2].value.as < ramc::ASTNode* > (), yystack_[0].value.as < ramc::ASTNode* > (), ramc::BinopType::LT); }
#line 1037 "ramc_parser_gen.cpp"
    break;

  case 35:
#line 260 "ramc_grammar.yy"
                        { yylhs.value.as < ramc::ASTNode* > () = new ramc::ASTBinopExpr(yystack_[2].value.as < ramc::ASTNode* > (), yystack_[0].value.as < ramc::ASTNode* > (), ramc::BinopType::GT); }
#line 1043 "ramc_parser_gen.cpp"
    break;

  case 36:
#line 261 "ramc_grammar.yy"
                        { yylhs.value.as < ramc::ASTNode* > () = new ramc::ASTBinopExpr(yystack_[2].value.as < ramc::ASTNode* > (), yystack_[0].value.as < ramc::ASTNode* > (), ramc::BinopType::LT_EQ); }
#line 1049 "ramc_parser_gen.cpp"
    break;

  case 37:
#line 262 "ramc_grammar.yy"
                        { yylhs.value.as < ramc::ASTNode* > () = new ramc::ASTBinopExpr(yystack_[2].value.as < ramc::ASTNode* > (), yystack_[0].value.as < ramc::ASTNode* > (), ramc::BinopType::GT_EQ); }
#line 1055 "ramc_parser_gen.cpp"
    break;

  case 38:
#line 263 "ramc_grammar.yy"
                        { yylhs.value.as < ramc::ASTNode* > () = yystack_[0].value.as < ramc::ASTNode* > (); }
#line 1061 "ramc_parser_gen.cpp"
    break;

  case 39:
#line 267 "ramc_grammar.yy"
                       { yylhs.value.as < ramc::ASTNode* > () = new ramc::ASTBinopExpr(yystack_[2].value.as < ramc::ASTNode* > (), yystack_[0].value.as < ramc::ASTNode* > (), ramc::BinopType::LSHIFT); }
#line 1067 "ramc_parser_gen.cpp"
    break;

  case 40:
#line 268 "ramc_grammar.yy"
                       { yylhs.value.as < ramc::ASTNode* > () = new ramc::ASTBinopExpr(yystack_[2].value.as < ramc::ASTNode* > (), yystack_[0].value.as < ramc::ASTNode* > (), ramc::BinopType::RSHIFT); }
#line 1073 "ramc_parser_gen.cpp"
    break;

  case 41:
#line 269 "ramc_grammar.yy"
                       { yylhs.value.as < ramc::ASTNode* > () = yystack_[0].value.as < ramc::ASTNode* > (); }
#line 1079 "ramc_parser_gen.cpp"
    break;

  case 42:
#line 273 "ramc_grammar.yy"
                       { yylhs.value.as < ramc::ASTNode* > () = new ramc::ASTBinopExpr(yystack_[2].value.as < ramc::ASTNode* > (), yystack_[0].value.as < ramc::ASTNode* > (), ramc::BinopType::ADD); }
#line 1085 "ramc_parser_gen.cpp"
    break;

  case 43:
#line 274 "ramc_grammar.yy"
                       { yylhs.value.as < ramc::ASTNode* > () = new ramc::ASTBinopExpr(yystack_[2].value.as < ramc::ASTNode* > (), yystack_[0].value.as < ramc::ASTNode* > (), ramc::BinopType::SUB); }
#line 1091 "ramc_parser_gen.cpp"
    break;

  case 44:
#line 275 "ramc_grammar.yy"
                       { yylhs.value.as < ramc::ASTNode* > () = yystack_[0].value.as < ramc::ASTNode* > (); }
#line 1097 "ramc_parser_gen.cpp"
    break;

  case 45:
#line 279 "ramc_grammar.yy"
                        { yylhs.value.as < ramc::ASTNode* > () = new ramc::ASTBinopExpr(yystack_[2].value.as < ramc::ASTNode* > (), yystack_[0].value.as < ramc::ASTNode* > (), ramc::BinopType::MUL); }
#line 1103 "ramc_parser_gen.cpp"
    break;

  case 46:
#line 280 "ramc_grammar.yy"
                        { yylhs.value.as < ramc::ASTNode* > () = new ramc::ASTBinopExpr(yystack_[2].value.as < ramc::ASTNode* > (), yystack_[0].value.as < ramc::ASTNode* > (), ramc::BinopType::DIV); }
#line 1109 "ramc_parser_gen.cpp"
    break;

  case 47:
#line 281 "ramc_grammar.yy"
                        { yylhs.value.as < ramc::ASTNode* > () = new ramc::ASTBinopExpr(yystack_[2].value.as < ramc::ASTNode* > (), yystack_[0].value.as < ramc::ASTNode* > (), ramc::BinopType::MOD); }
#line 1115 "ramc_parser_gen.cpp"
    break;

  case 48:
#line 282 "ramc_grammar.yy"
                        { yylhs.value.as < ramc::ASTNode* > () = yystack_[0].value.as < ramc::ASTNode* > (); }
#line 1121 "ramc_parser_gen.cpp"
    break;

  case 49:
#line 286 "ramc_grammar.yy"
                 { yylhs.value.as < ramc::ASTNode* > () = new ramc::ASTUnopExpr(yystack_[0].value.as < ramc::ASTNode* > (), ramc::UnopType::NEG); }
#line 1127 "ramc_parser_gen.cpp"
    break;

  case 50:
#line 287 "ramc_grammar.yy"
                 { yylhs.value.as < ramc::ASTNode* > () = new ramc::ASTUnopExpr(yystack_[0].value.as < ramc::ASTNode* > (), ramc::UnopType::LOG_NOT); }
#line 1133 "ramc_parser_gen.cpp"
    break;

  case 51:
#line 288 "ramc_grammar.yy"
                 { yylhs.value.as < ramc::ASTNode* > () = yystack_[0].value.as < ramc::ASTNode* > (); }
#line 1139 "ramc_parser_gen.cpp"
    break;

  case 52:
#line 292 "ramc_grammar.yy"
                         { yylhs.value.as < ramc::ASTNode* > () = new ramc::ASTBinopExpr(yystack_[2].value.as < ramc::ASTNode* > (), yystack_[0].value.as < ramc::ASTNode* > (), ramc::BinopType::POW); }
#line 1145 "ramc_parser_gen.cpp"
    break;

  case 53:
#line 293 "ramc_grammar.yy"
                         { yylhs.value.as < ramc::ASTNode* > () = yystack_[0].value.as < ramc::ASTNode* > (); }
#line 1151 "ramc_parser_gen.cpp"
    break;

  case 54:
#line 297 "ramc_grammar.yy"
                  { yylhs.value.as < ramc::ASTNode* > () = new ramc::ASTUnopExpr(yystack_[0].value.as < ramc::ASTNode* > (), ramc::UnopType::PRE_INC); }
#line 1157 "ramc_parser_gen.cpp"
    break;

  case 55:
#line 298 "ramc_grammar.yy"
                  { yylhs.value.as < ramc::ASTNode* > () = new ramc::ASTUnopExpr(yystack_[0].value.as < ramc::ASTNode* > (), ramc::UnopType::PRE_DEC); }
#line 1163 "ramc_parser_gen.cpp"
    break;

  case 56:
#line 299 "ramc_grammar.yy"
                  { yylhs.value.as < ramc::ASTNode* > () = yystack_[0].value.as < ramc::ASTNode* > (); }
#line 1169 "ramc_parser_gen.cpp"
    break;

  case 57:
#line 303 "ramc_grammar.yy"
                  { yylhs.value.as < ramc::ASTNode* > () = new ramc::ASTUnopExpr(yystack_[1].value.as < ramc::ASTNode* > (), ramc::UnopType::POST_INC); }
#line 1175 "ramc_parser_gen.cpp"
    break;

  case 58:
#line 304 "ramc_grammar.yy"
                  { yylhs.value.as < ramc::ASTNode* > () = new ramc::ASTUnopExpr(yystack_[1].value.as < ramc::ASTNode* > (), ramc::UnopType::POST_DEC); }
#line 1181 "ramc_parser_gen.cpp"
    break;

  case 59:
#line 305 "ramc_grammar.yy"
                  { yylhs.value.as < ramc::ASTNode* > () = yystack_[0].value.as < ramc::ASTNode* > (); }
#line 1187 "ramc_parser_gen.cpp"
    break;

  case 60:
#line 309 "ramc_grammar.yy"
                    { yylhs.value.as < ramc::ASTNode* > () = yystack_[0].value.as < ramc::ASTIntLit* > (); }
#line 1193 "ramc_parser_gen.cpp"
    break;

  case 61:
#line 310 "ramc_grammar.yy"
                    { yylhs.value.as < ramc::ASTNode* > () = yystack_[0].value.as < ramc::ASTFloatLit* > (); }
#line 1199 "ramc_parser_gen.cpp"
    break;

  case 62:
#line 311 "ramc_grammar.yy"
                    { yylhs.value.as < ramc::ASTNode* > () = yystack_[0].value.as < ramc::ASTStringLit* > (); }
#line 1205 "ramc_parser_gen.cpp"
    break;

  case 63:
#line 312 "ramc_grammar.yy"
                    { yylhs.value.as < ramc::ASTNode* > () = yystack_[0].value.as < ramc::ASTBoolLit* > (); }
#line 1211 "ramc_parser_gen.cpp"
    break;

  case 64:
#line 313 "ramc_grammar.yy"
                    { yylhs.value.as < ramc::ASTNode* > () = yystack_[0].value.as < ramc::ASTBoolLit* > (); }
#line 1217 "ramc_parser_gen.cpp"
    break;

  case 65:
#line 314 "ramc_grammar.yy"
                    { yylhs.value.as < ramc::ASTNode* > () = yystack_[0].value.as < ramc::ASTIdentifier* > (); }
#line 1223 "ramc_parser_gen.cpp"
    break;

  case 66:
#line 315 "ramc_grammar.yy"
                    { yylhs.value.as < ramc::ASTNode* > () = yystack_[1].value.as < ramc::ASTNode* > (); }
#line 1229 "ramc_parser_gen.cpp"
    break;


#line 1233 "ramc_parser_gen.cpp"

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
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.what ());
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (state_type, const symbol_type&) const
  {
    return YY_("syntax error");
  }


  const signed char parser::yypact_ninf_ = -50;

  const signed char parser::yytable_ninf_ = -1;

  const signed char
  parser::yypact_[] =
  {
     -50,     7,    -2,   -50,    30,    30,    30,    49,    49,     1,
      80,   -50,   -50,   -50,   -50,   -50,   -50,    10,   -18,    24,
      22,    44,    47,     4,    -5,    14,    25,    12,   -50,    50,
     -50,   -11,   -50,   -50,   -50,   -50,   -15,   -50,   -50,    30,
     -50,   -50,   -50,   -50,   -50,   -50,   -50,   -50,   -50,   -50,
     -50,   -50,    30,   -50,    30,   -50,    30,    30,    30,    30,
      30,    30,    30,    30,    30,    30,    30,    30,    30,    30,
      30,    30,    30,    49,   -50,   -50,   -50,    36,    36,    24,
      22,    44,    47,     4,    -5,    -5,    14,    14,    14,    14,
      25,    25,    12,    12,   -50,   -50,   -50,   -50
  };

  const signed char
  parser::yydefact_[] =
  {
       3,     0,     2,     1,     0,     0,     0,     0,     0,     0,
      65,    60,    62,    61,    63,    64,     4,     0,     0,    22,
      24,    26,    28,    30,    33,    38,    41,    44,    48,    51,
      53,    56,    59,    65,    49,    50,     0,    54,    55,     0,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,     0,     5,     0,     6,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    58,    66,     8,     7,    21,
      23,    25,    27,    29,    31,    32,    34,    35,    36,    37,
      39,    40,    42,    43,    45,    46,    47,    52
  };

  const signed char
  parser::yypgoto_[] =
  {
     -50,   -50,   -50,   -50,     2,     5,     6,     3,    11,    15,
     -25,   -49,   -29,   -26,    -4,   -50,    -3,   -50,   -50,   -50,
     -50
  };

  const signed char
  parser::yydefgoto_[] =
  {
      -1,     1,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,     2,
      52
  };

  const signed char
  parser::yytable_[] =
  {
      34,    35,     4,    54,    37,    38,    54,     3,    36,    62,
      63,    64,    65,    86,    87,    88,    89,    70,    71,    72,
       5,    76,    60,    61,    39,    55,    66,    67,    68,    69,
      74,    75,    57,     6,     4,    84,    85,    90,    91,     7,
       8,    77,    92,    93,    56,     9,    10,    11,    12,    13,
      14,    15,     5,    53,    78,    58,    59,    54,    73,    79,
      81,     0,    80,     0,     0,     6,    94,    95,    96,    82,
      97,     7,     8,     0,    83,     0,     0,     0,    33,    11,
      12,    13,    14,    15,     6,     0,     0,     0,     0,     0,
       7,     8,     0,     0,     0,     0,     0,    33,    11,    12,
      13,    14,    15,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,    50,    51
  };

  const signed char
  parser::yycheck_[] =
  {
       4,     5,     4,    21,     7,     8,    21,     0,     6,    14,
      15,    16,    17,    62,    63,    64,    65,     5,     6,     7,
      22,    36,    18,    19,    23,    43,    12,    13,     3,     4,
      41,    42,    10,    35,     4,    60,    61,    66,    67,    41,
      42,    39,    68,    69,    20,    47,    48,    49,    50,    51,
      52,    53,    22,    43,    52,    11,     9,    21,     8,    54,
      57,    -1,    56,    -1,    -1,    35,    70,    71,    72,    58,
      73,    41,    42,    -1,    59,    -1,    -1,    -1,    48,    49,
      50,    51,    52,    53,    35,    -1,    -1,    -1,    -1,    -1,
      41,    42,    -1,    -1,    -1,    -1,    -1,    48,    49,    50,
      51,    52,    53,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34
  };

  const signed char
  parser::yystos_[] =
  {
       0,    55,    73,     0,     4,    22,    35,    41,    42,    47,
      48,    49,    50,    51,    52,    53,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    48,    68,    68,    58,    70,    70,    23,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    74,    43,    21,    43,    20,    10,    11,     9,
      18,    19,    14,    15,    16,    17,    12,    13,     3,     4,
       5,     6,     7,     8,    41,    42,    36,    58,    58,    59,
      60,    61,    62,    63,    64,    64,    65,    65,    65,    65,
      66,    66,    67,    67,    68,    68,    68,    70
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    54,    55,    73,    73,    56,    56,    57,    57,    74,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      74,    58,    58,    59,    59,    60,    60,    61,    61,    62,
      62,    63,    63,    63,    64,    64,    64,    64,    64,    65,
      65,    65,    66,    66,    66,    67,    67,    67,    67,    68,
      68,    68,    69,    69,    70,    70,    70,    71,    71,    71,
      72,    72,    72,    72,    72,    72,    72
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     1,     0,     2,     2,     2,     3,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     3,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     3,     3,     1,     3,     3,     3,     3,     1,     3,
       3,     1,     3,     3,     1,     3,     3,     3,     1,     2,
       2,     1,     3,     1,     2,     2,     1,     2,     2,     1,
       1,     1,     1,     1,     1,     1,     3
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "END_OF_FILE", "error", "$undefined", "\"+\"", "\"-\"", "\"*\"",
  "\"/\"", "\"%\"", "\"**\"", "\"&\"", "\"|\"", "\"^\"", "\"<<\"",
  "\">>\"", "\"<\"", "\">\"", "\"<=\"", "\">=\"", "\"==\"", "\"!=\"",
  "\"&&\"", "\"||\"", "\"!\"", "\"=\"", "\"+=\"", "\"-=\"", "\"*=\"",
  "\"/=\"", "\"%=\"", "\"**=\"", "\"&=\"", "\"|=\"", "\"^=\"", "\"<<=\"",
  "\">>=\"", "\"(\"", "\")\"", "\"}\"", "\"{\"", "\"[\"", "\"]\"",
  "\"++\"", "\"--\"", "\";\"", "\".\"", "\":\"", "\",\"", "\"_\"",
  "\"ID\"", "\"INT_LIT\"", "\"STRING_LIT\"", "\"FLOAT_LIT\"", "\"true\"",
  "\"false\"", "$accept", "PROGRAM", "STMT", "ASSIGNMENT", "EXPR1",
  "EXPR2", "EXPR3", "EXPR4", "EXPR5", "EXPR6", "EXPR7", "EXPR8", "EXPR9",
  "EXPR10", "EXPR11", "EXPR12", "EXPR13", "EXPR14", "ATOM", "STMTS",
  "OP_ASSIGN", YY_NULLPTR
  };


  const short
  parser::yyrline_[] =
  {
       0,   195,   195,   199,   200,   204,   205,   209,   210,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   228,   229,   233,   234,   238,   239,   243,   244,   248,
     249,   253,   254,   255,   259,   260,   261,   262,   263,   267,
     268,   269,   273,   274,   275,   279,   280,   281,   282,   286,
     287,   288,   292,   293,   297,   298,   299,   303,   304,   305,
     309,   310,   311,   312,   313,   314,   315
  };

  // Print the state stack on the debug stream.
  void
  parser::yystack_print_ ()
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
  parser::yy_reduce_print_ (int yyrule)
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


} // yy
#line 1598 "ramc_parser_gen.cpp"

#line 318 "ramc_grammar.yy"

namespace yy
{
  // Report an error to the user.
  auto parser::error (const std::string& _msg) -> void
  {
    throw std::runtime_error(_msg);
  }
}

