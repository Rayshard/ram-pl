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
#line 13 "ramvm_grammar.yy"

	#include "pch.h"
	#include "ramvm_lexer.h"

#line 43 "ramvm_bison_parser.cpp"




#include "ramvm_bison_parser.hpp"


// Unqualified %code blocks.
#line 30 "ramvm_grammar.yy"

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

#line 128 "ramvm_bison_parser.cpp"


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
#line 201 "ramvm_bison_parser.cpp"


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
  Parser::Parser (Lexer& lexer_yyarg, std::vector<Instruction*>& result_yyarg, Position& position_yyarg, std::map<std::string, int>& labels_yyarg, std::map<Instruction*, std::pair<std::string, Position>>& ctrlInstrs_yyarg)
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr),
#else
    :
#endif
      yy_lac_established_ (false),
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
      case 51: // ARGUMENT
      case 52: // DEST_ARG
        value.YY_MOVE_OR_COPY< Argument > (YY_MOVE (that.value));
        break;

      case 20: // "MOV"
      case 21: // "POP"
        value.YY_MOVE_OR_COPY< DataType > (YY_MOVE (that.value));
        break;

      case 44: // "NEG"
      case 45: // "LNOT"
      case 46: // "BNOT"
        value.YY_MOVE_OR_COPY< DataTypeDouble > (YY_MOVE (that.value));
        break;

      case 25: // "ADD"
      case 26: // "SUB"
      case 27: // "MUL"
      case 28: // "DIV"
      case 29: // "MOD"
      case 30: // "POW"
      case 31: // "LSHIFT"
      case 32: // "RSHIFT"
      case 33: // "BAND"
      case 34: // "BOR"
      case 35: // "BXOR"
      case 36: // "LAND"
      case 37: // "LOR"
      case 38: // "LT"
      case 39: // "GT"
      case 40: // "LTEQ"
      case 41: // "GTEQ"
      case 42: // "EQ"
      case 43: // "NEQ"
        value.YY_MOVE_OR_COPY< DataTypeTriple > (YY_MOVE (that.value));
        break;

      case 3: // "hex"
        value.YY_MOVE_OR_COPY< DataValue > (YY_MOVE (that.value));
        break;

      case 50: // STMT
        value.YY_MOVE_OR_COPY< Instruction* > (YY_MOVE (that.value));
        break;

      case 4: // "reg"
      case 5: // "mreg"
      case 6: // "sreg"
      case 7: // "spoff"
      case 9: // "ipOff"
        value.YY_MOVE_OR_COPY< int > (YY_MOVE (that.value));
        break;

      case 53: // BINOP
        value.YY_MOVE_OR_COPY< std::pair<Binop, DataTypeTriple> > (YY_MOVE (that.value));
        break;

      case 54: // UNOP
        value.YY_MOVE_OR_COPY< std::pair<Unop, DataTypeDouble> > (YY_MOVE (that.value));
        break;

      case 8: // "LABEL"
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      case 49: // ARGUMENTS
        value.YY_MOVE_OR_COPY< std::vector<Argument> > (YY_MOVE (that.value));
        break;

      case 22: // "STORE"
      case 23: // "PUSH"
      case 24: // "CALL"
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
      case 51: // ARGUMENT
      case 52: // DEST_ARG
        value.move< Argument > (YY_MOVE (that.value));
        break;

      case 20: // "MOV"
      case 21: // "POP"
        value.move< DataType > (YY_MOVE (that.value));
        break;

      case 44: // "NEG"
      case 45: // "LNOT"
      case 46: // "BNOT"
        value.move< DataTypeDouble > (YY_MOVE (that.value));
        break;

      case 25: // "ADD"
      case 26: // "SUB"
      case 27: // "MUL"
      case 28: // "DIV"
      case 29: // "MOD"
      case 30: // "POW"
      case 31: // "LSHIFT"
      case 32: // "RSHIFT"
      case 33: // "BAND"
      case 34: // "BOR"
      case 35: // "BXOR"
      case 36: // "LAND"
      case 37: // "LOR"
      case 38: // "LT"
      case 39: // "GT"
      case 40: // "LTEQ"
      case 41: // "GTEQ"
      case 42: // "EQ"
      case 43: // "NEQ"
        value.move< DataTypeTriple > (YY_MOVE (that.value));
        break;

      case 3: // "hex"
        value.move< DataValue > (YY_MOVE (that.value));
        break;

      case 50: // STMT
        value.move< Instruction* > (YY_MOVE (that.value));
        break;

      case 4: // "reg"
      case 5: // "mreg"
      case 6: // "sreg"
      case 7: // "spoff"
      case 9: // "ipOff"
        value.move< int > (YY_MOVE (that.value));
        break;

      case 53: // BINOP
        value.move< std::pair<Binop, DataTypeTriple> > (YY_MOVE (that.value));
        break;

      case 54: // UNOP
        value.move< std::pair<Unop, DataTypeDouble> > (YY_MOVE (that.value));
        break;

      case 8: // "LABEL"
        value.move< std::string > (YY_MOVE (that.value));
        break;

      case 49: // ARGUMENTS
        value.move< std::vector<Argument> > (YY_MOVE (that.value));
        break;

      case 22: // "STORE"
      case 23: // "PUSH"
      case 24: // "CALL"
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
      case 51: // ARGUMENT
      case 52: // DEST_ARG
        value.copy< Argument > (that.value);
        break;

      case 20: // "MOV"
      case 21: // "POP"
        value.copy< DataType > (that.value);
        break;

      case 44: // "NEG"
      case 45: // "LNOT"
      case 46: // "BNOT"
        value.copy< DataTypeDouble > (that.value);
        break;

      case 25: // "ADD"
      case 26: // "SUB"
      case 27: // "MUL"
      case 28: // "DIV"
      case 29: // "MOD"
      case 30: // "POW"
      case 31: // "LSHIFT"
      case 32: // "RSHIFT"
      case 33: // "BAND"
      case 34: // "BOR"
      case 35: // "BXOR"
      case 36: // "LAND"
      case 37: // "LOR"
      case 38: // "LT"
      case 39: // "GT"
      case 40: // "LTEQ"
      case 41: // "GTEQ"
      case 42: // "EQ"
      case 43: // "NEQ"
        value.copy< DataTypeTriple > (that.value);
        break;

      case 3: // "hex"
        value.copy< DataValue > (that.value);
        break;

      case 50: // STMT
        value.copy< Instruction* > (that.value);
        break;

      case 4: // "reg"
      case 5: // "mreg"
      case 6: // "sreg"
      case 7: // "spoff"
      case 9: // "ipOff"
        value.copy< int > (that.value);
        break;

      case 53: // BINOP
        value.copy< std::pair<Binop, DataTypeTriple> > (that.value);
        break;

      case 54: // UNOP
        value.copy< std::pair<Unop, DataTypeDouble> > (that.value);
        break;

      case 8: // "LABEL"
        value.copy< std::string > (that.value);
        break;

      case 49: // ARGUMENTS
        value.copy< std::vector<Argument> > (that.value);
        break;

      case 22: // "STORE"
      case 23: // "PUSH"
      case 24: // "CALL"
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
      case 51: // ARGUMENT
      case 52: // DEST_ARG
        value.move< Argument > (that.value);
        break;

      case 20: // "MOV"
      case 21: // "POP"
        value.move< DataType > (that.value);
        break;

      case 44: // "NEG"
      case 45: // "LNOT"
      case 46: // "BNOT"
        value.move< DataTypeDouble > (that.value);
        break;

      case 25: // "ADD"
      case 26: // "SUB"
      case 27: // "MUL"
      case 28: // "DIV"
      case 29: // "MOD"
      case 30: // "POW"
      case 31: // "LSHIFT"
      case 32: // "RSHIFT"
      case 33: // "BAND"
      case 34: // "BOR"
      case 35: // "BXOR"
      case 36: // "LAND"
      case 37: // "LOR"
      case 38: // "LT"
      case 39: // "GT"
      case 40: // "LTEQ"
      case 41: // "GTEQ"
      case 42: // "EQ"
      case 43: // "NEQ"
        value.move< DataTypeTriple > (that.value);
        break;

      case 3: // "hex"
        value.move< DataValue > (that.value);
        break;

      case 50: // STMT
        value.move< Instruction* > (that.value);
        break;

      case 4: // "reg"
      case 5: // "mreg"
      case 6: // "sreg"
      case 7: // "spoff"
      case 9: // "ipOff"
        value.move< int > (that.value);
        break;

      case 53: // BINOP
        value.move< std::pair<Binop, DataTypeTriple> > (that.value);
        break;

      case 54: // UNOP
        value.move< std::pair<Unop, DataTypeDouble> > (that.value);
        break;

      case 8: // "LABEL"
        value.move< std::string > (that.value);
        break;

      case 49: // ARGUMENTS
        value.move< std::vector<Argument> > (that.value);
        break;

      case 22: // "STORE"
      case 23: // "PUSH"
      case 24: // "CALL"
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
      case 51: // ARGUMENT
      case 52: // DEST_ARG
        yylhs.value.emplace< Argument > ();
        break;

      case 20: // "MOV"
      case 21: // "POP"
        yylhs.value.emplace< DataType > ();
        break;

      case 44: // "NEG"
      case 45: // "LNOT"
      case 46: // "BNOT"
        yylhs.value.emplace< DataTypeDouble > ();
        break;

      case 25: // "ADD"
      case 26: // "SUB"
      case 27: // "MUL"
      case 28: // "DIV"
      case 29: // "MOD"
      case 30: // "POW"
      case 31: // "LSHIFT"
      case 32: // "RSHIFT"
      case 33: // "BAND"
      case 34: // "BOR"
      case 35: // "BXOR"
      case 36: // "LAND"
      case 37: // "LOR"
      case 38: // "LT"
      case 39: // "GT"
      case 40: // "LTEQ"
      case 41: // "GTEQ"
      case 42: // "EQ"
      case 43: // "NEQ"
        yylhs.value.emplace< DataTypeTriple > ();
        break;

      case 3: // "hex"
        yylhs.value.emplace< DataValue > ();
        break;

      case 50: // STMT
        yylhs.value.emplace< Instruction* > ();
        break;

      case 4: // "reg"
      case 5: // "mreg"
      case 6: // "sreg"
      case 7: // "spoff"
      case 9: // "ipOff"
        yylhs.value.emplace< int > ();
        break;

      case 53: // BINOP
        yylhs.value.emplace< std::pair<Binop, DataTypeTriple> > ();
        break;

      case 54: // UNOP
        yylhs.value.emplace< std::pair<Unop, DataTypeDouble> > ();
        break;

      case 8: // "LABEL"
        yylhs.value.emplace< std::string > ();
        break;

      case 49: // ARGUMENTS
        yylhs.value.emplace< std::vector<Argument> > ();
        break;

      case 22: // "STORE"
      case 23: // "PUSH"
      case 24: // "CALL"
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
#line 172 "ramvm_grammar.yy"
                                        { result.push_back(yystack_[0].value.as < Instruction* > ()); }
#line 1011 "ramvm_bison_parser.cpp"
    break;

  case 5:
#line 173 "ramvm_grammar.yy"
                                { labels.find(yystack_[0].value.as < std::string > ()) == labels.end() ? labels.insert_or_assign(yystack_[0].value.as < std::string > (), result.size()) : throw std::runtime_error("Duplicate Label: " + yystack_[0].value.as < std::string > ()); }
#line 1017 "ramvm_bison_parser.cpp"
    break;

  case 6:
#line 177 "ramvm_grammar.yy"
                                                                                                        { yylhs.value.as < Instruction* > () = new InstrHalt(); }
#line 1023 "ramvm_bison_parser.cpp"
    break;

  case 7:
#line 178 "ramvm_grammar.yy"
                                                                                                { yylhs.value.as < Instruction* > () = new InstrReturn(yystack_[0].value.as < Argument > ()); }
#line 1029 "ramvm_bison_parser.cpp"
    break;

  case 8:
#line 179 "ramvm_grammar.yy"
                                                                                        { yylhs.value.as < Instruction* > () = new InstrMove(yystack_[2].value.as < DataType > (), yystack_[1].value.as < Argument > (), yystack_[0].value.as < Argument > ()); }
#line 1035 "ramvm_bison_parser.cpp"
    break;

  case 9:
#line 180 "ramvm_grammar.yy"
                                                                                        { yylhs.value.as < Instruction* > () = new InstrMalloc(yystack_[1].value.as < Argument > (), yystack_[0].value.as < Argument > ()); }
#line 1041 "ramvm_bison_parser.cpp"
    break;

  case 10:
#line 181 "ramvm_grammar.yy"
                                                                                                { yylhs.value.as < Instruction* > () = new InstrFree(yystack_[0].value.as < Argument > ()); }
#line 1047 "ramvm_bison_parser.cpp"
    break;

  case 11:
#line 182 "ramvm_grammar.yy"
                                                                                        { yylhs.value.as < Instruction* > () = new InstrPrint(yystack_[1].value.as < Argument > (), yystack_[0].value.as < Argument > ()); }
#line 1053 "ramvm_bison_parser.cpp"
    break;

  case 12:
#line 183 "ramvm_grammar.yy"
                                                                                                { yylhs.value.as < Instruction* > () = new InstrJump(-1); ctrlInstrs.insert_or_assign(yylhs.value.as < Instruction* > (), std::make_pair(yystack_[0].value.as < std::string > (), position)); }
#line 1059 "ramvm_bison_parser.cpp"
    break;

  case 13:
#line 184 "ramvm_grammar.yy"
                                                                                                { yylhs.value.as < Instruction* > () = new InstrJump(result.size() + yystack_[0].value.as < int > ()); }
#line 1065 "ramvm_bison_parser.cpp"
    break;

  case 14:
#line 185 "ramvm_grammar.yy"
                                                                                        { yylhs.value.as < Instruction* > () = new InstrCJump(-1, yystack_[0].value.as < Argument > (), false); ctrlInstrs.insert_or_assign(yylhs.value.as < Instruction* > (), std::make_pair(yystack_[1].value.as < std::string > (), position)); }
#line 1071 "ramvm_bison_parser.cpp"
    break;

  case 15:
#line 186 "ramvm_grammar.yy"
                                                                                        { yylhs.value.as < Instruction* > () = new InstrCJump(result.size() + yystack_[1].value.as < int > (), yystack_[0].value.as < Argument > (), false); }
#line 1077 "ramvm_bison_parser.cpp"
    break;

  case 16:
#line 187 "ramvm_grammar.yy"
                                                                                        { yylhs.value.as < Instruction* > () = new InstrCJump(-1, yystack_[0].value.as < Argument > (), true); ctrlInstrs.insert_or_assign(yylhs.value.as < Instruction* > (), std::make_pair(yystack_[1].value.as < std::string > (), position)); }
#line 1083 "ramvm_bison_parser.cpp"
    break;

  case 17:
#line 188 "ramvm_grammar.yy"
                                                                                        { yylhs.value.as < Instruction* > () = new InstrCJump(result.size() + yystack_[1].value.as < int > (), yystack_[0].value.as < Argument > (), true); }
#line 1089 "ramvm_bison_parser.cpp"
    break;

  case 18:
#line 189 "ramvm_grammar.yy"
                                                                                { yylhs.value.as < Instruction* > () = new InstrCall(-1, yystack_[1].value.as < DataValue > ().i, BindArgDataTypes(yystack_[3].value.as < std::vector<DataType> > (), yystack_[0].value.as < std::vector<Argument> > ())); ctrlInstrs.insert_or_assign(yylhs.value.as < Instruction* > (), std::make_pair(yystack_[2].value.as < std::string > (), position)); }
#line 1095 "ramvm_bison_parser.cpp"
    break;

  case 19:
#line 190 "ramvm_grammar.yy"
                                                                                { yylhs.value.as < Instruction* > () = new InstrCall(result.size() + yystack_[2].value.as < DataValue > ().i, yystack_[1].value.as < DataValue > ().i, BindArgDataTypes(yystack_[3].value.as < std::vector<DataType> > (), yystack_[0].value.as < std::vector<Argument> > ())); }
#line 1101 "ramvm_bison_parser.cpp"
    break;

  case 20:
#line 191 "ramvm_grammar.yy"
                                                                                                { yylhs.value.as < Instruction* > () = new InstrPush(BindArgDataTypes(yystack_[1].value.as < std::vector<DataType> > (), yystack_[0].value.as < std::vector<Argument> > ())); }
#line 1107 "ramvm_bison_parser.cpp"
    break;

  case 21:
#line 192 "ramvm_grammar.yy"
                                                                                                { yylhs.value.as < Instruction* > () = new InstrPop(yystack_[1].value.as < DataType > (), yystack_[0].value.as < Argument > ()); }
#line 1113 "ramvm_bison_parser.cpp"
    break;

  case 22:
#line 193 "ramvm_grammar.yy"
                                                                                        { yylhs.value.as < Instruction* > () = yystack_[2].value.as < std::vector<DataType> > ().size() != 0 ? new InstrStore(BindArgDataTypes(yystack_[2].value.as < std::vector<DataType> > (), yystack_[1].value.as < std::vector<Argument> > ()), yystack_[0].value.as < Argument > ()) : throw std::runtime_error("'STORE' expects at least one source argument!"); }
#line 1119 "ramvm_bison_parser.cpp"
    break;

  case 23:
#line 194 "ramvm_grammar.yy"
                                                                { yylhs.value.as < Instruction* > () = new InstrCompare(yystack_[3].value.as < Argument > (), yystack_[2].value.as < Argument > (), yystack_[1].value.as < Argument > (), yystack_[0].value.as < Argument > ()); }
#line 1125 "ramvm_bison_parser.cpp"
    break;

  case 24:
#line 195 "ramvm_grammar.yy"
                                                                                { yylhs.value.as < Instruction* > () = new InstrBinop(yystack_[3].value.as < std::pair<Binop, DataTypeTriple> > ().first, TypedArgument(std::get<0>(yystack_[3].value.as < std::pair<Binop, DataTypeTriple> > ().second), yystack_[2].value.as < Argument > ()), TypedArgument(std::get<1>(yystack_[3].value.as < std::pair<Binop, DataTypeTriple> > ().second), yystack_[1].value.as < Argument > ()), TypedArgument(std::get<2>(yystack_[3].value.as < std::pair<Binop, DataTypeTriple> > ().second), yystack_[0].value.as < Argument > ())); }
#line 1131 "ramvm_bison_parser.cpp"
    break;

  case 25:
#line 196 "ramvm_grammar.yy"
                                                                                        { yylhs.value.as < Instruction* > () = new InstrUnop(yystack_[2].value.as < std::pair<Unop, DataTypeDouble> > ().first, TypedArgument(std::get<0>(yystack_[2].value.as < std::pair<Unop, DataTypeDouble> > ().second), yystack_[1].value.as < Argument > ()), TypedArgument(std::get<1>(yystack_[2].value.as < std::pair<Unop, DataTypeDouble> > ().second), yystack_[0].value.as < Argument > ())); }
#line 1137 "ramvm_bison_parser.cpp"
    break;

  case 26:
#line 200 "ramvm_grammar.yy"
                                                { yylhs.value.as < std::vector<Argument> > () = { }; }
#line 1143 "ramvm_bison_parser.cpp"
    break;

  case 27:
#line 201 "ramvm_grammar.yy"
                                    { yystack_[1].value.as < std::vector<Argument> > ().push_back(yystack_[0].value.as < Argument > ()); yylhs.value.as < std::vector<Argument> > () = yystack_[1].value.as < std::vector<Argument> > (); }
#line 1149 "ramvm_bison_parser.cpp"
    break;

  case 28:
#line 205 "ramvm_grammar.yy"
                                { yylhs.value.as < Argument > () = Argument(ArgType::VALUE, yystack_[0].value.as < DataValue > ()); }
#line 1155 "ramvm_bison_parser.cpp"
    break;

  case 29:
#line 206 "ramvm_grammar.yy"
                                { yylhs.value.as < Argument > () = Argument(ArgType::REGISTER, yystack_[0].value.as < int > ()); }
#line 1161 "ramvm_bison_parser.cpp"
    break;

  case 30:
#line 207 "ramvm_grammar.yy"
                                { yylhs.value.as < Argument > () = Argument(ArgType::MEM_REG, yystack_[0].value.as < int > ()); }
#line 1167 "ramvm_bison_parser.cpp"
    break;

  case 31:
#line 208 "ramvm_grammar.yy"
                                { yylhs.value.as < Argument > () = Argument(ArgType::STACK_REG, yystack_[0].value.as < int > ()); }
#line 1173 "ramvm_bison_parser.cpp"
    break;

  case 32:
#line 209 "ramvm_grammar.yy"
                                { yylhs.value.as < Argument > () = Argument(ArgType::STACK_PTR, 0); }
#line 1179 "ramvm_bison_parser.cpp"
    break;

  case 33:
#line 210 "ramvm_grammar.yy"
                                { yylhs.value.as < Argument > () = Argument(ArgType::SP_OFFSET, yystack_[0].value.as < int > ()); }
#line 1185 "ramvm_bison_parser.cpp"
    break;

  case 34:
#line 214 "ramvm_grammar.yy"
                                { yylhs.value.as < Argument > () = Argument(ArgType::REGISTER, yystack_[0].value.as < int > ()); }
#line 1191 "ramvm_bison_parser.cpp"
    break;

  case 35:
#line 215 "ramvm_grammar.yy"
                                { yylhs.value.as < Argument > () = Argument(ArgType::MEM_REG, yystack_[0].value.as < int > ()); }
#line 1197 "ramvm_bison_parser.cpp"
    break;

  case 36:
#line 216 "ramvm_grammar.yy"
                                { yylhs.value.as < Argument > () = Argument(ArgType::STACK_REG, yystack_[0].value.as < int > ()); }
#line 1203 "ramvm_bison_parser.cpp"
    break;

  case 37:
#line 217 "ramvm_grammar.yy"
                                { yylhs.value.as < Argument > () = Argument(ArgType::SP_OFFSET, yystack_[0].value.as < int > ()); }
#line 1209 "ramvm_bison_parser.cpp"
    break;

  case 38:
#line 221 "ramvm_grammar.yy"
                                { yylhs.value.as < std::pair<Binop, DataTypeTriple> > () = { Binop::ADD, yystack_[0].value.as < DataTypeTriple > () }; }
#line 1215 "ramvm_bison_parser.cpp"
    break;

  case 39:
#line 222 "ramvm_grammar.yy"
                                { yylhs.value.as < std::pair<Binop, DataTypeTriple> > () = { Binop::SUB, yystack_[0].value.as < DataTypeTriple > () }; }
#line 1221 "ramvm_bison_parser.cpp"
    break;

  case 40:
#line 223 "ramvm_grammar.yy"
                                { yylhs.value.as < std::pair<Binop, DataTypeTriple> > () = { Binop::MUL, yystack_[0].value.as < DataTypeTriple > () }; }
#line 1227 "ramvm_bison_parser.cpp"
    break;

  case 41:
#line 224 "ramvm_grammar.yy"
                                { yylhs.value.as < std::pair<Binop, DataTypeTriple> > () = { Binop::DIV, yystack_[0].value.as < DataTypeTriple > () }; }
#line 1233 "ramvm_bison_parser.cpp"
    break;

  case 42:
#line 225 "ramvm_grammar.yy"
                                { yylhs.value.as < std::pair<Binop, DataTypeTriple> > () = { Binop::MOD, yystack_[0].value.as < DataTypeTriple > () }; }
#line 1239 "ramvm_bison_parser.cpp"
    break;

  case 43:
#line 226 "ramvm_grammar.yy"
                                { yylhs.value.as < std::pair<Binop, DataTypeTriple> > () = { Binop::POW, yystack_[0].value.as < DataTypeTriple > () }; }
#line 1245 "ramvm_bison_parser.cpp"
    break;

  case 44:
#line 227 "ramvm_grammar.yy"
                                { yylhs.value.as < std::pair<Binop, DataTypeTriple> > () = { Binop::LSHIFT, yystack_[0].value.as < DataTypeTriple > () }; }
#line 1251 "ramvm_bison_parser.cpp"
    break;

  case 45:
#line 228 "ramvm_grammar.yy"
                                { yylhs.value.as < std::pair<Binop, DataTypeTriple> > () = { Binop::RSHIFT, yystack_[0].value.as < DataTypeTriple > () }; }
#line 1257 "ramvm_bison_parser.cpp"
    break;

  case 46:
#line 229 "ramvm_grammar.yy"
                                { yylhs.value.as < std::pair<Binop, DataTypeTriple> > () = { Binop::BIT_AND, yystack_[0].value.as < DataTypeTriple > () }; }
#line 1263 "ramvm_bison_parser.cpp"
    break;

  case 47:
#line 230 "ramvm_grammar.yy"
                                { yylhs.value.as < std::pair<Binop, DataTypeTriple> > () = { Binop::BIT_OR, yystack_[0].value.as < DataTypeTriple > () }; }
#line 1269 "ramvm_bison_parser.cpp"
    break;

  case 48:
#line 231 "ramvm_grammar.yy"
                                { yylhs.value.as < std::pair<Binop, DataTypeTriple> > () = { Binop::BIT_XOR, yystack_[0].value.as < DataTypeTriple > () }; }
#line 1275 "ramvm_bison_parser.cpp"
    break;

  case 49:
#line 232 "ramvm_grammar.yy"
                                { yylhs.value.as < std::pair<Binop, DataTypeTriple> > () = { Binop::LOG_AND, yystack_[0].value.as < DataTypeTriple > () }; }
#line 1281 "ramvm_bison_parser.cpp"
    break;

  case 50:
#line 233 "ramvm_grammar.yy"
                                { yylhs.value.as < std::pair<Binop, DataTypeTriple> > () = { Binop::LOG_OR, yystack_[0].value.as < DataTypeTriple > () }; }
#line 1287 "ramvm_bison_parser.cpp"
    break;

  case 51:
#line 234 "ramvm_grammar.yy"
                                { yylhs.value.as < std::pair<Binop, DataTypeTriple> > () = { Binop::LT, yystack_[0].value.as < DataTypeTriple > () }; }
#line 1293 "ramvm_bison_parser.cpp"
    break;

  case 52:
#line 235 "ramvm_grammar.yy"
                                { yylhs.value.as < std::pair<Binop, DataTypeTriple> > () = { Binop::GT, yystack_[0].value.as < DataTypeTriple > () }; }
#line 1299 "ramvm_bison_parser.cpp"
    break;

  case 53:
#line 236 "ramvm_grammar.yy"
                                { yylhs.value.as < std::pair<Binop, DataTypeTriple> > () = { Binop::LTEQ, yystack_[0].value.as < DataTypeTriple > () }; }
#line 1305 "ramvm_bison_parser.cpp"
    break;

  case 54:
#line 237 "ramvm_grammar.yy"
                                { yylhs.value.as < std::pair<Binop, DataTypeTriple> > () = { Binop::GTEQ, yystack_[0].value.as < DataTypeTriple > () }; }
#line 1311 "ramvm_bison_parser.cpp"
    break;

  case 55:
#line 238 "ramvm_grammar.yy"
                                { yylhs.value.as < std::pair<Binop, DataTypeTriple> > () = { Binop::EQ, yystack_[0].value.as < DataTypeTriple > () }; }
#line 1317 "ramvm_bison_parser.cpp"
    break;

  case 56:
#line 239 "ramvm_grammar.yy"
                                { yylhs.value.as < std::pair<Binop, DataTypeTriple> > () = { Binop::NEQ, yystack_[0].value.as < DataTypeTriple > () }; }
#line 1323 "ramvm_bison_parser.cpp"
    break;

  case 57:
#line 242 "ramvm_grammar.yy"
                        { yylhs.value.as < std::pair<Unop, DataTypeDouble> > () = std::make_pair(Unop::NEG, yystack_[0].value.as < DataTypeDouble > ()); }
#line 1329 "ramvm_bison_parser.cpp"
    break;

  case 58:
#line 243 "ramvm_grammar.yy"
                        { yylhs.value.as < std::pair<Unop, DataTypeDouble> > () = std::make_pair(Unop::LOG_NOT, yystack_[0].value.as < DataTypeDouble > ()); }
#line 1335 "ramvm_bison_parser.cpp"
    break;

  case 59:
#line 244 "ramvm_grammar.yy"
                        { yylhs.value.as < std::pair<Unop, DataTypeDouble> > () = std::make_pair(Unop::BIN_NOT, yystack_[0].value.as < DataTypeDouble > ()); }
#line 1341 "ramvm_bison_parser.cpp"
    break;


#line 1345 "ramvm_bison_parser.cpp"

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


  const signed char Parser::yypact_ninf_ = -47;

  const signed char Parser::yytable_ninf_ = -34;

  const signed char
  Parser::yypact_[] =
  {
     -47,    65,     6,   -47,   -47,    10,    10,    10,    18,    10,
      27,    31,    33,    10,    10,   -47,   -47,     4,   -47,   -47,
     -47,   -47,   -47,   -47,   -47,   -47,   -47,   -47,   -47,   -47,
     -47,   -47,   -47,   -47,   -47,   -47,   -47,   -47,   -47,   -47,
     -47,    10,    10,   -47,   -47,   -47,   -47,   -47,   -47,   -47,
      18,   -47,   -47,   -47,   -47,   -47,   -47,    18,    10,   -47,
     -47,    10,    10,    10,    10,    18,   -47,    24,    10,     5,
      15,    10,    18,   -47,    10,   -47,   -47,   -47,   -47,   -47,
     -47,    43,    58,   109,   117,   -47,   -47,   -47,   -47,    18,
     -47,    18,    10,    10,   -47,   -47
  };

  const signed char
  Parser::yydefact_[] =
  {
       3,     2,     0,     5,     6,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    26,    26,     0,    38,    39,
      40,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
       4,     0,     0,     1,    28,    29,    30,    31,    33,    32,
       0,    10,     7,    34,    35,    36,    37,     0,     0,    12,
      13,     0,     0,     0,     0,     0,    21,     0,    20,     0,
       0,     0,     0,     9,     0,    11,    14,    15,    16,    17,
       8,    34,    35,    36,    37,    27,    22,    26,    26,     0,
      25,     0,    19,    18,    24,    23
  };

  const signed char
  Parser::yypgoto_[] =
  {
     -47,   -47,   -16,   -47,    -4,   -46,   -47,   -47,   -47
  };

  const signed char
  Parser::yydefgoto_[] =
  {
      -1,     1,    67,    40,    85,    57,    41,    42,     2
  };

  const signed char
  Parser::yytable_[] =
  {
      68,    50,    51,    52,    73,    58,    43,    69,    87,    65,
      66,    74,    70,    44,    45,    46,    47,    48,    88,    80,
      49,    86,    53,    54,    55,    56,    90,    44,    81,    82,
      83,    84,     0,     0,    49,    59,    60,    71,    72,    61,
      62,    63,    64,    94,     0,    95,   -29,   -29,   -29,   -29,
     -29,     0,     0,   -29,    75,     0,     0,    76,    77,    78,
      79,   -30,   -30,   -30,   -30,   -30,     0,    89,   -30,     0,
      91,    92,    93,     3,     0,     0,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,   -31,   -31,   -31,   -31,   -31,     0,     0,   -31,
     -33,   -33,   -33,   -33,   -33,     0,     0,   -33
  };

  const signed char
  Parser::yycheck_[] =
  {
      16,     5,     6,     7,    50,     9,     0,     3,     3,    13,
      14,    57,     8,     3,     4,     5,     6,     7,     3,    65,
      10,    67,     4,     5,     6,     7,    72,     3,     4,     5,
       6,     7,    -1,    -1,    10,     8,     9,    41,    42,     8,
       9,     8,     9,    89,    -1,    91,     3,     4,     5,     6,
       7,    -1,    -1,    10,    58,    -1,    -1,    61,    62,    63,
      64,     3,     4,     5,     6,     7,    -1,    71,    10,    -1,
      74,    87,    88,     8,    -1,    -1,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,     3,     4,     5,     6,     7,    -1,    -1,    10,
       3,     4,     5,     6,     7,    -1,    -1,    10
  };

  const signed char
  Parser::yystos_[] =
  {
       0,    48,    55,     8,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      50,    53,    54,     0,     3,     4,     5,     6,     7,    10,
      51,    51,    51,     4,     5,     6,     7,    52,    51,     8,
       9,     8,     9,     8,     9,    51,    51,    49,    49,     3,
       8,    51,    51,    52,    52,    51,    51,    51,    51,    51,
      52,     4,     5,     6,     7,    51,    52,     3,     3,    51,
      52,    51,    49,    49,    52,    52
  };

  const signed char
  Parser::yyr1_[] =
  {
       0,    47,    55,    48,    48,    48,    50,    50,    50,    50,
      50,    50,    50,    50,    50,    50,    50,    50,    50,    50,
      50,    50,    50,    50,    50,    50,    49,    49,    51,    51,
      51,    51,    51,    51,    52,    52,    52,    52,    53,    53,
      53,    53,    53,    53,    53,    53,    53,    53,    53,    53,
      53,    53,    53,    53,    53,    53,    53,    54,    54,    54
  };

  const signed char
  Parser::yyr2_[] =
  {
       0,     2,     1,     0,     2,     2,     1,     2,     3,     3,
       2,     3,     2,     2,     3,     3,     3,     3,     4,     4,
       2,     2,     3,     5,     4,     3,     0,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const Parser::yytname_[] =
  {
  "TOK_END_OF_FILE", "error", "$undefined", "\"hex\"", "\"reg\"",
  "\"mreg\"", "\"sreg\"", "\"spoff\"", "\"LABEL\"", "\"ipOff\"", "\"SP\"",
  "\"HALT\"", "\"MALLOC\"", "\"FREE\"", "\"RET\"", "\"COMPARE\"",
  "\"PRINT\"", "\"JUMP\"", "\"JUMPT\"", "\"JUMPF\"", "\"MOV\"", "\"POP\"",
  "\"STORE\"", "\"PUSH\"", "\"CALL\"", "\"ADD\"", "\"SUB\"", "\"MUL\"",
  "\"DIV\"", "\"MOD\"", "\"POW\"", "\"LSHIFT\"", "\"RSHIFT\"", "\"BAND\"",
  "\"BOR\"", "\"BXOR\"", "\"LAND\"", "\"LOR\"", "\"LT\"", "\"GT\"",
  "\"LTEQ\"", "\"GTEQ\"", "\"EQ\"", "\"NEQ\"", "\"NEG\"", "\"LNOT\"",
  "\"BNOT\"", "$accept", "STMTS", "ARGUMENTS", "STMT", "ARGUMENT",
  "DEST_ARG", "BINOP", "UNOP", "PROGRAM", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned char
  Parser::yyrline_[] =
  {
       0,   168,   168,   171,   172,   173,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,   192,   193,   194,   195,   196,   200,   201,   205,   206,
     207,   208,   209,   210,   214,   215,   216,   217,   221,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   235,   236,   237,   238,   239,   242,   243,   244
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
#line 1932 "ramvm_bison_parser.cpp"

#line 247 "ramvm_grammar.yy"

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

