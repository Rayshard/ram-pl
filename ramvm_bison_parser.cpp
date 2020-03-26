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
#line 11 "ramvm_grammar.yy"

  #include "pch.h"
  #include "ramvm_lexer.h"

#line 43 "ramvm_bison_parser.cpp"




#include "ramvm_bison_parser.hpp"


// Unqualified %code blocks.
#line 28 "ramvm_grammar.yy"

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

				Token token = readRes.GetToken();
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
					case TokenType::KW_SP: return Parser::make_TOK_SP();
					case TokenType::KW_HALT: return Parser::make_TOK_HALT();
					case TokenType::KW_MOV: return Parser::make_TOK_MOV(CharToDataType(value[0]));
					case TokenType::KW_RET: return Parser::make_TOK_RET(CharsToDataTypes(value));
					case TokenType::KW_MALLOC: return Parser::make_TOK_MALLOC();
					case TokenType::KW_FREE: return Parser::make_TOK_FREE();
					case TokenType::KW_PUSH: return Parser::make_TOK_PUSH(CharsToDataTypes(value));
					case TokenType::KW_POP: return Parser::make_TOK_POP(CharToDataType(value[0]));
					case TokenType::KW_PRINT: return Parser::make_TOK_PRINT();
					case TokenType::KW_JUMP: return Parser::make_TOK_JUMP();
					case TokenType::KW_CJUMP: return Parser::make_TOK_CJUMP();
					case TokenType::KW_CALL: return Parser::make_TOK_CALL(CharsToDataTypes(value));
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

#line 125 "ramvm_bison_parser.cpp"


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

#line 8 "ramvm_grammar.yy"
namespace ramvm { namespace bison {
#line 198 "ramvm_bison_parser.cpp"


  /// Build a parser object.
  Parser::Parser (Lexer& lexer_yyarg, std::vector<Instruction*>& result_yyarg, Position& position_yyarg, std::map<std::string, int>& labels_yyarg, std::map<Instruction*, std::pair<std::string, Position>>& ctrlInstrs_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      lexer (lexer_yyarg),
      result (result_yyarg),
      position (position_yyarg),
      labels (labels_yyarg),
      ctrlInstrs (ctrlInstrs_yyarg)
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
      case 48: // ARGUMENT
      case 49: // DEST_ARG
        value.YY_MOVE_OR_COPY< Argument > (YY_MOVE (that.value));
        break;

      case 11: // "MOV"
      case 17: // "POP"
        value.YY_MOVE_OR_COPY< DataType > (YY_MOVE (that.value));
        break;

      case 41: // "NEG"
      case 42: // "LNOT"
      case 43: // "BNOT"
        value.YY_MOVE_OR_COPY< DataTypeDouble > (YY_MOVE (that.value));
        break;

      case 22: // "ADD"
      case 23: // "SUB"
      case 24: // "MUL"
      case 25: // "DIV"
      case 26: // "MOD"
      case 27: // "POW"
      case 28: // "LSHIFT"
      case 29: // "RSHIFT"
      case 30: // "BAND"
      case 31: // "BOR"
      case 32: // "BXOR"
      case 33: // "LAND"
      case 34: // "LOR"
      case 35: // "LT"
      case 36: // "GT"
      case 37: // "LTEQ"
      case 38: // "GTEQ"
      case 39: // "EQ"
      case 40: // "NEQ"
        value.YY_MOVE_OR_COPY< DataTypeTriple > (YY_MOVE (that.value));
        break;

      case 3: // "hex"
        value.YY_MOVE_OR_COPY< DataValue > (YY_MOVE (that.value));
        break;

      case 47: // STMT
        value.YY_MOVE_OR_COPY< Instruction* > (YY_MOVE (that.value));
        break;

      case 4: // "reg"
      case 5: // "mreg"
      case 6: // "sreg"
      case 7: // "spoff"
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case 50: // BINOP
        value.YY_MOVE_OR_COPY< std::pair<Binop, DataTypeTriple> > (YY_MOVE (that.value));
        break;

      case 51: // UNOP
        value.YY_MOVE_OR_COPY< std::pair<Unop, DataTypeDouble> > (YY_MOVE (that.value));
        break;

      case 8: // "LABEL"
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case 46: // ARGUMENTS
        value.YY_MOVE_OR_COPY< std::vector<Argument> > (YY_MOVE (that.value));
        break;

      case 12: // "RET"
      case 15: // "STORE"
      case 16: // "PUSH"
      case 21: // "CALL"
        value.YY_MOVE_OR_COPY< std::vector<DataType> > (YY_MOVE (that.value));
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
      case 48: // ARGUMENT
      case 49: // DEST_ARG
        value.move< Argument > (YY_MOVE (that.value));
        break;

      case 11: // "MOV"
      case 17: // "POP"
        value.move< DataType > (YY_MOVE (that.value));
        break;

      case 41: // "NEG"
      case 42: // "LNOT"
      case 43: // "BNOT"
        value.move< DataTypeDouble > (YY_MOVE (that.value));
        break;

      case 22: // "ADD"
      case 23: // "SUB"
      case 24: // "MUL"
      case 25: // "DIV"
      case 26: // "MOD"
      case 27: // "POW"
      case 28: // "LSHIFT"
      case 29: // "RSHIFT"
      case 30: // "BAND"
      case 31: // "BOR"
      case 32: // "BXOR"
      case 33: // "LAND"
      case 34: // "LOR"
      case 35: // "LT"
      case 36: // "GT"
      case 37: // "LTEQ"
      case 38: // "GTEQ"
      case 39: // "EQ"
      case 40: // "NEQ"
        value.move< DataTypeTriple > (YY_MOVE (that.value));
        break;

      case 3: // "hex"
        value.move< DataValue > (YY_MOVE (that.value));
        break;

      case 47: // STMT
        value.move< Instruction* > (YY_MOVE (that.value));
        break;

      case 4: // "reg"
      case 5: // "mreg"
      case 6: // "sreg"
      case 7: // "spoff"
        value.move< int > (YY_MOVE (that.value));
        break;

      case 50: // BINOP
        value.move< std::pair<Binop, DataTypeTriple> > (YY_MOVE (that.value));
        break;

      case 51: // UNOP
        value.move< std::pair<Unop, DataTypeDouble> > (YY_MOVE (that.value));
        break;

      case 8: // "LABEL"
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case 46: // ARGUMENTS
        value.move< std::vector<Argument> > (YY_MOVE (that.value));
        break;

      case 12: // "RET"
      case 15: // "STORE"
      case 16: // "PUSH"
      case 21: // "CALL"
        value.move< std::vector<DataType> > (YY_MOVE (that.value));
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
      case 48: // ARGUMENT
      case 49: // DEST_ARG
        value.copy< Argument > (that.value);
        break;

      case 11: // "MOV"
      case 17: // "POP"
        value.copy< DataType > (that.value);
        break;

      case 41: // "NEG"
      case 42: // "LNOT"
      case 43: // "BNOT"
        value.copy< DataTypeDouble > (that.value);
        break;

      case 22: // "ADD"
      case 23: // "SUB"
      case 24: // "MUL"
      case 25: // "DIV"
      case 26: // "MOD"
      case 27: // "POW"
      case 28: // "LSHIFT"
      case 29: // "RSHIFT"
      case 30: // "BAND"
      case 31: // "BOR"
      case 32: // "BXOR"
      case 33: // "LAND"
      case 34: // "LOR"
      case 35: // "LT"
      case 36: // "GT"
      case 37: // "LTEQ"
      case 38: // "GTEQ"
      case 39: // "EQ"
      case 40: // "NEQ"
        value.copy< DataTypeTriple > (that.value);
        break;

      case 3: // "hex"
        value.copy< DataValue > (that.value);
        break;

      case 47: // STMT
        value.copy< Instruction* > (that.value);
        break;

      case 4: // "reg"
      case 5: // "mreg"
      case 6: // "sreg"
      case 7: // "spoff"
        value.copy< int > (that.value);
        break;

      case 50: // BINOP
        value.copy< std::pair<Binop, DataTypeTriple> > (that.value);
        break;

      case 51: // UNOP
        value.copy< std::pair<Unop, DataTypeDouble> > (that.value);
        break;

      case 8: // "LABEL"
        value.copy< std::string > (that.value);
        break;

      case 46: // ARGUMENTS
        value.copy< std::vector<Argument> > (that.value);
        break;

      case 12: // "RET"
      case 15: // "STORE"
      case 16: // "PUSH"
      case 21: // "CALL"
        value.copy< std::vector<DataType> > (that.value);
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
      case 48: // ARGUMENT
      case 49: // DEST_ARG
        value.move< Argument > (that.value);
        break;

      case 11: // "MOV"
      case 17: // "POP"
        value.move< DataType > (that.value);
        break;

      case 41: // "NEG"
      case 42: // "LNOT"
      case 43: // "BNOT"
        value.move< DataTypeDouble > (that.value);
        break;

      case 22: // "ADD"
      case 23: // "SUB"
      case 24: // "MUL"
      case 25: // "DIV"
      case 26: // "MOD"
      case 27: // "POW"
      case 28: // "LSHIFT"
      case 29: // "RSHIFT"
      case 30: // "BAND"
      case 31: // "BOR"
      case 32: // "BXOR"
      case 33: // "LAND"
      case 34: // "LOR"
      case 35: // "LT"
      case 36: // "GT"
      case 37: // "LTEQ"
      case 38: // "GTEQ"
      case 39: // "EQ"
      case 40: // "NEQ"
        value.move< DataTypeTriple > (that.value);
        break;

      case 3: // "hex"
        value.move< DataValue > (that.value);
        break;

      case 47: // STMT
        value.move< Instruction* > (that.value);
        break;

      case 4: // "reg"
      case 5: // "mreg"
      case 6: // "sreg"
      case 7: // "spoff"
        value.move< int > (that.value);
        break;

      case 50: // BINOP
        value.move< std::pair<Binop, DataTypeTriple> > (that.value);
        break;

      case 51: // UNOP
        value.move< std::pair<Unop, DataTypeDouble> > (that.value);
        break;

      case 8: // "LABEL"
        value.move< std::string > (that.value);
        break;

      case 46: // ARGUMENTS
        value.move< std::vector<Argument> > (that.value);
        break;

      case 12: // "RET"
      case 15: // "STORE"
      case 16: // "PUSH"
      case 21: // "CALL"
        value.move< std::vector<DataType> > (that.value);
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
            symbol_type yylookahead (yylex (lexer, result, position, labels, ctrlInstrs));
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
      case 48: // ARGUMENT
      case 49: // DEST_ARG
        yylhs.value.emplace< Argument > ();
        break;

      case 11: // "MOV"
      case 17: // "POP"
        yylhs.value.emplace< DataType > ();
        break;

      case 41: // "NEG"
      case 42: // "LNOT"
      case 43: // "BNOT"
        yylhs.value.emplace< DataTypeDouble > ();
        break;

      case 22: // "ADD"
      case 23: // "SUB"
      case 24: // "MUL"
      case 25: // "DIV"
      case 26: // "MOD"
      case 27: // "POW"
      case 28: // "LSHIFT"
      case 29: // "RSHIFT"
      case 30: // "BAND"
      case 31: // "BOR"
      case 32: // "BXOR"
      case 33: // "LAND"
      case 34: // "LOR"
      case 35: // "LT"
      case 36: // "GT"
      case 37: // "LTEQ"
      case 38: // "GTEQ"
      case 39: // "EQ"
      case 40: // "NEQ"
        yylhs.value.emplace< DataTypeTriple > ();
        break;

      case 3: // "hex"
        yylhs.value.emplace< DataValue > ();
        break;

      case 47: // STMT
        yylhs.value.emplace< Instruction* > ();
        break;

      case 4: // "reg"
      case 5: // "mreg"
      case 6: // "sreg"
      case 7: // "spoff"
        yylhs.value.emplace< int > ();
        break;

      case 50: // BINOP
        yylhs.value.emplace< std::pair<Binop, DataTypeTriple> > ();
        break;

      case 51: // UNOP
        yylhs.value.emplace< std::pair<Unop, DataTypeDouble> > ();
        break;

      case 8: // "LABEL"
        yylhs.value.emplace< std::string > ();
        break;

      case 46: // ARGUMENTS
        yylhs.value.emplace< std::vector<Argument> > ();
        break;

      case 12: // "RET"
      case 15: // "STORE"
      case 16: // "PUSH"
      case 21: // "CALL"
        yylhs.value.emplace< std::vector<DataType> > ();
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
  case 4:
#line 164 "ramvm_grammar.yy"
                                        { result.push_back(yystack_[0].value.as < Instruction* > ()); }
#line 956 "ramvm_bison_parser.cpp"
    break;

  case 5:
#line 165 "ramvm_grammar.yy"
                                { labels.find(yystack_[0].value.as < std::string > ()) == labels.end() ? labels.insert_or_assign(yystack_[0].value.as < std::string > (), result.size()) : throw std::runtime_error("Duplicate Label: " + yystack_[0].value.as < std::string > ()); }
#line 962 "ramvm_bison_parser.cpp"
    break;

  case 6:
#line 169 "ramvm_grammar.yy"
                                                                                { yylhs.value.as < Instruction* > () = new InstrHalt(); }
#line 968 "ramvm_bison_parser.cpp"
    break;

  case 7:
#line 170 "ramvm_grammar.yy"
                                                                        { yylhs.value.as < Instruction* > () = new InstrReturn(BindArgDataTypes(yystack_[1].value.as < std::vector<DataType> > (), yystack_[0].value.as < std::vector<Argument> > ())); }
#line 974 "ramvm_bison_parser.cpp"
    break;

  case 8:
#line 171 "ramvm_grammar.yy"
                                                                { yylhs.value.as < Instruction* > () = new InstrMove(TypedArgument(yystack_[2].value.as < DataType > (), yystack_[1].value.as < Argument > ()), yystack_[0].value.as < Argument > ()); }
#line 980 "ramvm_bison_parser.cpp"
    break;

  case 9:
#line 172 "ramvm_grammar.yy"
                                                                { yylhs.value.as < Instruction* > () = new InstrMalloc(yystack_[1].value.as < Argument > (), yystack_[0].value.as < Argument > ()); }
#line 986 "ramvm_bison_parser.cpp"
    break;

  case 10:
#line 173 "ramvm_grammar.yy"
                                                                        { yylhs.value.as < Instruction* > () = new InstrFree(yystack_[0].value.as < Argument > ()); }
#line 992 "ramvm_bison_parser.cpp"
    break;

  case 11:
#line 174 "ramvm_grammar.yy"
                                                                { yylhs.value.as < Instruction* > () = new InstrPrint(yystack_[1].value.as < Argument > (), yystack_[0].value.as < Argument > ()); }
#line 998 "ramvm_bison_parser.cpp"
    break;

  case 12:
#line 175 "ramvm_grammar.yy"
                                                                        { yylhs.value.as < Instruction* > () = new InstrJump(-1); ctrlInstrs.insert_or_assign(yylhs.value.as < Instruction* > (), std::make_pair(yystack_[0].value.as < std::string > (), position)); }
#line 1004 "ramvm_bison_parser.cpp"
    break;

  case 13:
#line 176 "ramvm_grammar.yy"
                                                                { yylhs.value.as < Instruction* > () = new InstrCJump(-1, yystack_[0].value.as < Argument > ()); ctrlInstrs.insert_or_assign(yylhs.value.as < Instruction* > (), std::make_pair(yystack_[1].value.as < std::string > (), position)); }
#line 1010 "ramvm_bison_parser.cpp"
    break;

  case 14:
#line 177 "ramvm_grammar.yy"
                                                        { yylhs.value.as < Instruction* > () = new InstrCall(-1, yystack_[1].value.as < DataValue > ().i, BindArgDataTypes(yystack_[3].value.as < std::vector<DataType> > (), yystack_[0].value.as < std::vector<Argument> > ())); ctrlInstrs.insert_or_assign(yylhs.value.as < Instruction* > (), std::make_pair(yystack_[2].value.as < std::string > (), position)); }
#line 1016 "ramvm_bison_parser.cpp"
    break;

  case 15:
#line 178 "ramvm_grammar.yy"
                                                                        { yylhs.value.as < Instruction* > () = new InstrPush(BindArgDataTypes(yystack_[1].value.as < std::vector<DataType> > (), yystack_[0].value.as < std::vector<Argument> > ())); }
#line 1022 "ramvm_bison_parser.cpp"
    break;

  case 16:
#line 179 "ramvm_grammar.yy"
                                                                        { yylhs.value.as < Instruction* > () = new InstrPop(yystack_[1].value.as < DataType > (), yystack_[0].value.as < Argument > ()); }
#line 1028 "ramvm_bison_parser.cpp"
    break;

  case 17:
#line 180 "ramvm_grammar.yy"
                                                                { yylhs.value.as < Instruction* > () = yystack_[2].value.as < std::vector<DataType> > ().size() != 0 ? new InstrStore(BindArgDataTypes(yystack_[2].value.as < std::vector<DataType> > (), yystack_[1].value.as < std::vector<Argument> > ()), yystack_[0].value.as < Argument > ()) : throw std::runtime_error("'STORE' expects at least one source argument!"); }
#line 1034 "ramvm_bison_parser.cpp"
    break;

  case 18:
#line 181 "ramvm_grammar.yy"
                                                        { yylhs.value.as < Instruction* > () = new InstrBinop(yystack_[3].value.as < std::pair<Binop, DataTypeTriple> > ().first, TypedArgument(std::get<0>(yystack_[3].value.as < std::pair<Binop, DataTypeTriple> > ().second), yystack_[2].value.as < Argument > ()), TypedArgument(std::get<1>(yystack_[3].value.as < std::pair<Binop, DataTypeTriple> > ().second), yystack_[1].value.as < Argument > ()), TypedArgument(std::get<2>(yystack_[3].value.as < std::pair<Binop, DataTypeTriple> > ().second), yystack_[0].value.as < Argument > ())); }
#line 1040 "ramvm_bison_parser.cpp"
    break;

  case 19:
#line 182 "ramvm_grammar.yy"
                                                                { yylhs.value.as < Instruction* > () = new InstrUnop(yystack_[2].value.as < std::pair<Unop, DataTypeDouble> > ().first, TypedArgument(yystack_[2].value.as < std::pair<Unop, DataTypeDouble> > ().second.first, yystack_[1].value.as < Argument > ()), TypedArgument(yystack_[2].value.as < std::pair<Unop, DataTypeDouble> > ().second.second, yystack_[0].value.as < Argument > ())); }
#line 1046 "ramvm_bison_parser.cpp"
    break;

  case 20:
#line 186 "ramvm_grammar.yy"
                                                { yylhs.value.as < std::vector<Argument> > () = { }; }
#line 1052 "ramvm_bison_parser.cpp"
    break;

  case 21:
#line 187 "ramvm_grammar.yy"
                                    { yystack_[1].value.as < std::vector<Argument> > ().push_back(yystack_[0].value.as < Argument > ()); yylhs.value.as < std::vector<Argument> > () = yystack_[1].value.as < std::vector<Argument> > (); }
#line 1058 "ramvm_bison_parser.cpp"
    break;

  case 22:
#line 191 "ramvm_grammar.yy"
                                { yylhs.value.as < Argument > () = Argument(ArgType::VALUE, yystack_[0].value.as < DataValue > ()); }
#line 1064 "ramvm_bison_parser.cpp"
    break;

  case 23:
#line 192 "ramvm_grammar.yy"
                                { yylhs.value.as < Argument > () = Argument(ArgType::REGISTER, yystack_[0].value.as < int > ()); }
#line 1070 "ramvm_bison_parser.cpp"
    break;

  case 24:
#line 193 "ramvm_grammar.yy"
                                { yylhs.value.as < Argument > () = Argument(ArgType::MEM_REG, yystack_[0].value.as < int > ()); }
#line 1076 "ramvm_bison_parser.cpp"
    break;

  case 25:
#line 194 "ramvm_grammar.yy"
                                { yylhs.value.as < Argument > () = Argument(ArgType::STACK_REG, yystack_[0].value.as < int > ()); }
#line 1082 "ramvm_bison_parser.cpp"
    break;

  case 26:
#line 195 "ramvm_grammar.yy"
                                { yylhs.value.as < Argument > () = Argument(ArgType::STACK_PTR, 0); }
#line 1088 "ramvm_bison_parser.cpp"
    break;

  case 27:
#line 196 "ramvm_grammar.yy"
                                { yylhs.value.as < Argument > () = Argument(ArgType::SP_OFFSET, yystack_[0].value.as < int > ()); }
#line 1094 "ramvm_bison_parser.cpp"
    break;

  case 28:
#line 200 "ramvm_grammar.yy"
                                { yylhs.value.as < Argument > () = Argument(ArgType::REGISTER, yystack_[0].value.as < int > ()); }
#line 1100 "ramvm_bison_parser.cpp"
    break;

  case 29:
#line 201 "ramvm_grammar.yy"
                                { yylhs.value.as < Argument > () = Argument(ArgType::MEM_REG, yystack_[0].value.as < int > ()); }
#line 1106 "ramvm_bison_parser.cpp"
    break;

  case 30:
#line 202 "ramvm_grammar.yy"
                                { yylhs.value.as < Argument > () = Argument(ArgType::STACK_REG, yystack_[0].value.as < int > ()); }
#line 1112 "ramvm_bison_parser.cpp"
    break;

  case 31:
#line 203 "ramvm_grammar.yy"
                                { yylhs.value.as < Argument > () = Argument(ArgType::SP_OFFSET, yystack_[0].value.as < int > ()); }
#line 1118 "ramvm_bison_parser.cpp"
    break;

  case 32:
#line 207 "ramvm_grammar.yy"
                                { yylhs.value.as < std::pair<Binop, DataTypeTriple> > () = { Binop::ADD, yystack_[0].value.as < DataTypeTriple > () }; }
#line 1124 "ramvm_bison_parser.cpp"
    break;

  case 33:
#line 208 "ramvm_grammar.yy"
                                { yylhs.value.as < std::pair<Binop, DataTypeTriple> > () = { Binop::SUB, yystack_[0].value.as < DataTypeTriple > () }; }
#line 1130 "ramvm_bison_parser.cpp"
    break;

  case 34:
#line 209 "ramvm_grammar.yy"
                                { yylhs.value.as < std::pair<Binop, DataTypeTriple> > () = { Binop::MUL, yystack_[0].value.as < DataTypeTriple > () }; }
#line 1136 "ramvm_bison_parser.cpp"
    break;

  case 35:
#line 210 "ramvm_grammar.yy"
                                { yylhs.value.as < std::pair<Binop, DataTypeTriple> > () = { Binop::DIV, yystack_[0].value.as < DataTypeTriple > () }; }
#line 1142 "ramvm_bison_parser.cpp"
    break;

  case 36:
#line 211 "ramvm_grammar.yy"
                                { yylhs.value.as < std::pair<Binop, DataTypeTriple> > () = { Binop::MOD, yystack_[0].value.as < DataTypeTriple > () }; }
#line 1148 "ramvm_bison_parser.cpp"
    break;

  case 37:
#line 212 "ramvm_grammar.yy"
                                { yylhs.value.as < std::pair<Binop, DataTypeTriple> > () = { Binop::POW, yystack_[0].value.as < DataTypeTriple > () }; }
#line 1154 "ramvm_bison_parser.cpp"
    break;

  case 38:
#line 213 "ramvm_grammar.yy"
                                { yylhs.value.as < std::pair<Binop, DataTypeTriple> > () = { Binop::LSHIFT, yystack_[0].value.as < DataTypeTriple > () }; }
#line 1160 "ramvm_bison_parser.cpp"
    break;

  case 39:
#line 214 "ramvm_grammar.yy"
                                { yylhs.value.as < std::pair<Binop, DataTypeTriple> > () = { Binop::RSHIFT, yystack_[0].value.as < DataTypeTriple > () }; }
#line 1166 "ramvm_bison_parser.cpp"
    break;

  case 40:
#line 215 "ramvm_grammar.yy"
                                { yylhs.value.as < std::pair<Binop, DataTypeTriple> > () = { Binop::BIT_AND, yystack_[0].value.as < DataTypeTriple > () }; }
#line 1172 "ramvm_bison_parser.cpp"
    break;

  case 41:
#line 216 "ramvm_grammar.yy"
                                { yylhs.value.as < std::pair<Binop, DataTypeTriple> > () = { Binop::BIT_OR, yystack_[0].value.as < DataTypeTriple > () }; }
#line 1178 "ramvm_bison_parser.cpp"
    break;

  case 42:
#line 217 "ramvm_grammar.yy"
                                { yylhs.value.as < std::pair<Binop, DataTypeTriple> > () = { Binop::BIT_XOR, yystack_[0].value.as < DataTypeTriple > () }; }
#line 1184 "ramvm_bison_parser.cpp"
    break;

  case 43:
#line 218 "ramvm_grammar.yy"
                                { yylhs.value.as < std::pair<Binop, DataTypeTriple> > () = { Binop::LOG_AND, yystack_[0].value.as < DataTypeTriple > () }; }
#line 1190 "ramvm_bison_parser.cpp"
    break;

  case 44:
#line 219 "ramvm_grammar.yy"
                                { yylhs.value.as < std::pair<Binop, DataTypeTriple> > () = { Binop::LOG_OR, yystack_[0].value.as < DataTypeTriple > () }; }
#line 1196 "ramvm_bison_parser.cpp"
    break;

  case 45:
#line 220 "ramvm_grammar.yy"
                                { yylhs.value.as < std::pair<Binop, DataTypeTriple> > () = { Binop::LT, yystack_[0].value.as < DataTypeTriple > () }; }
#line 1202 "ramvm_bison_parser.cpp"
    break;

  case 46:
#line 221 "ramvm_grammar.yy"
                                { yylhs.value.as < std::pair<Binop, DataTypeTriple> > () = { Binop::GT, yystack_[0].value.as < DataTypeTriple > () }; }
#line 1208 "ramvm_bison_parser.cpp"
    break;

  case 47:
#line 222 "ramvm_grammar.yy"
                                { yylhs.value.as < std::pair<Binop, DataTypeTriple> > () = { Binop::LTEQ, yystack_[0].value.as < DataTypeTriple > () }; }
#line 1214 "ramvm_bison_parser.cpp"
    break;

  case 48:
#line 223 "ramvm_grammar.yy"
                                { yylhs.value.as < std::pair<Binop, DataTypeTriple> > () = { Binop::GTEQ, yystack_[0].value.as < DataTypeTriple > () }; }
#line 1220 "ramvm_bison_parser.cpp"
    break;

  case 49:
#line 224 "ramvm_grammar.yy"
                                { yylhs.value.as < std::pair<Binop, DataTypeTriple> > () = { Binop::EQ, yystack_[0].value.as < DataTypeTriple > () }; }
#line 1226 "ramvm_bison_parser.cpp"
    break;

  case 50:
#line 225 "ramvm_grammar.yy"
                                { yylhs.value.as < std::pair<Binop, DataTypeTriple> > () = { Binop::NEQ, yystack_[0].value.as < DataTypeTriple > () }; }
#line 1232 "ramvm_bison_parser.cpp"
    break;

  case 51:
#line 228 "ramvm_grammar.yy"
                        { yylhs.value.as < std::pair<Unop, DataTypeDouble> > () = { Unop::NEG, yystack_[0].value.as < DataTypeDouble > () }; }
#line 1238 "ramvm_bison_parser.cpp"
    break;

  case 52:
#line 229 "ramvm_grammar.yy"
                        { yylhs.value.as < std::pair<Unop, DataTypeDouble> > () = { Unop::LOG_NOT, yystack_[0].value.as < DataTypeDouble > () }; }
#line 1244 "ramvm_bison_parser.cpp"
    break;

  case 53:
#line 230 "ramvm_grammar.yy"
                        { yylhs.value.as < std::pair<Unop, DataTypeDouble> > () = { Unop::BIN_NOT, yystack_[0].value.as < DataTypeDouble > () }; }
#line 1250 "ramvm_bison_parser.cpp"
    break;


#line 1254 "ramvm_bison_parser.cpp"

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
  Parser::error (const syntax_error& yyexc)
  {
    error (yyexc.what ());
  }

  // Generate an error message.
  std::string
  Parser::yysyntax_error_ (state_type, const symbol_type&) const
  {
    return YY_("syntax error");
  }


  const signed char Parser::yypact_ninf_ = -39;

  const signed char Parser::yytable_ninf_ = -28;

  const signed char
  Parser::yypact_[] =
  {
     -39,    57,     3,   -39,   -39,    12,   -39,    12,    12,   -39,
     -39,    12,    12,    -2,    -1,     2,   -39,   -39,   -39,   -39,
     -39,   -39,   -39,   -39,   -39,   -39,   -39,   -39,   -39,   -39,
     -39,   -39,   -39,   -39,   -39,   -39,   -39,   -39,   -39,    12,
      12,   -39,   -39,   -39,   -39,   -39,   -39,   -39,    26,    12,
      26,   -39,    20,    12,   -39,    12,   -39,    12,     8,    12,
      26,   -39,   -39,   -39,   -39,   -39,   -39,   -39,    36,    44,
      54,    98,   -39,   -39,   -39,   -39,    26,   -39,    12,   -39
  };

  const signed char
  Parser::yydefact_[] =
  {
       3,     2,     0,     5,     6,     0,    20,     0,     0,    20,
      20,     0,     0,     0,     0,     0,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,     4,     0,
       0,     1,    22,    23,    24,    25,    27,    26,     0,     7,
       0,    10,     0,    15,    16,     0,    12,     0,     0,     0,
       0,    28,    29,    30,    31,     8,    21,     9,    28,    29,
      30,    31,    17,    11,    13,    20,     0,    19,    14,    18
  };

  const signed char
  Parser::yypgoto_[] =
  {
     -39,   -39,    -9,   -39,    -3,   -38,   -39,   -39,   -39
  };

  const signed char
  Parser::yydefgoto_[] =
  {
      -1,     1,    49,    38,    66,    65,    39,    40,     2
  };

  const signed char
  Parser::yytable_[] =
  {
      52,    53,    48,    41,    50,    51,    56,    57,    54,    55,
      58,    75,    67,     0,    72,    42,    43,    44,    45,    46,
       0,    47,    77,    42,    68,    69,    70,    71,     0,    47,
      61,    62,    63,    64,     0,     0,    59,    60,    79,   -23,
     -23,   -23,   -23,   -23,     0,   -23,     0,   -24,   -24,   -24,
     -24,   -24,    73,   -24,    74,     0,    76,   -25,   -25,   -25,
     -25,   -25,     0,   -25,     0,     3,    78,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,   -27,   -27,   -27,   -27,   -27,     0,   -27
  };

  const signed char
  Parser::yycheck_[] =
  {
       9,    10,     5,     0,     7,     8,     8,     8,    11,    12,
       8,     3,    50,    -1,    52,     3,     4,     5,     6,     7,
      -1,     9,    60,     3,     4,     5,     6,     7,    -1,     9,
       4,     5,     6,     7,    -1,    -1,    39,    40,    76,     3,
       4,     5,     6,     7,    -1,     9,    -1,     3,     4,     5,
       6,     7,    55,     9,    57,    -1,    59,     3,     4,     5,
       6,     7,    -1,     9,    -1,     8,    75,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,     3,     4,     5,     6,     7,    -1,     9
  };

  const signed char
  Parser::yystos_[] =
  {
       0,    45,    52,     8,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    47,    50,
      51,     0,     3,     4,     5,     6,     7,     9,    48,    46,
      48,    48,    46,    46,    48,    48,     8,     8,     8,    48,
      48,     4,     5,     6,     7,    49,    48,    49,     4,     5,
       6,     7,    49,    48,    48,     3,    48,    49,    46,    49
  };

  const signed char
  Parser::yyr1_[] =
  {
       0,    44,    52,    45,    45,    45,    47,    47,    47,    47,
      47,    47,    47,    47,    47,    47,    47,    47,    47,    47,
      46,    46,    48,    48,    48,    48,    48,    48,    49,    49,
      49,    49,    50,    50,    50,    50,    50,    50,    50,    50,
      50,    50,    50,    50,    50,    50,    50,    50,    50,    50,
      50,    51,    51,    51
  };

  const signed char
  Parser::yyr2_[] =
  {
       0,     2,     1,     0,     2,     2,     1,     2,     3,     3,
       2,     3,     2,     3,     4,     2,     2,     3,     4,     3,
       0,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const Parser::yytname_[] =
  {
  "TOK_END_OF_FILE", "error", "$undefined", "\"hex\"", "\"reg\"",
  "\"mreg\"", "\"sreg\"", "\"spoff\"", "\"LABEL\"", "\"SP\"", "\"HALT\"",
  "\"MOV\"", "\"RET\"", "\"MALLOC\"", "\"FREE\"", "\"STORE\"", "\"PUSH\"",
  "\"POP\"", "\"PRINT\"", "\"JUMP\"", "\"CJUMP\"", "\"CALL\"", "\"ADD\"",
  "\"SUB\"", "\"MUL\"", "\"DIV\"", "\"MOD\"", "\"POW\"", "\"LSHIFT\"",
  "\"RSHIFT\"", "\"BAND\"", "\"BOR\"", "\"BXOR\"", "\"LAND\"", "\"LOR\"",
  "\"LT\"", "\"GT\"", "\"LTEQ\"", "\"GTEQ\"", "\"EQ\"", "\"NEQ\"",
  "\"NEG\"", "\"LNOT\"", "\"BNOT\"", "$accept", "STMTS", "ARGUMENTS",
  "STMT", "ARGUMENT", "DEST_ARG", "BINOP", "UNOP", "PROGRAM", YY_NULLPTR
  };


  const unsigned char
  Parser::yyrline_[] =
  {
       0,   160,   160,   163,   164,   165,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     186,   187,   191,   192,   193,   194,   195,   196,   200,   201,
     202,   203,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218,   219,   220,   221,   222,   223,   224,
     225,   228,   229,   230
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


#line 8 "ramvm_grammar.yy"
} } // ramvm::bison
#line 1602 "ramvm_bison_parser.cpp"

#line 233 "ramvm_grammar.yy"

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

