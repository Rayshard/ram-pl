// A Bison parser, made by GNU Bison 3.5.

// Skeleton interface for Bison LALR(1) parsers in C++

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


/**
 ** \file ramvm_bison_parser.hpp
 ** Define the ramvm::bison::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

// Undocumented macros, especially those whose name start with YY_,
// are private implementation details.  Do not rely on them.

#ifndef YY_YY_RAMVM_BISON_PARSER_HPP_INCLUDED
# define YY_YY_RAMVM_BISON_PARSER_HPP_INCLUDED
// "%code requires" blocks.
#line 18 "ramvm_grammar.yy"

  #include "ramvm_instruction.h"

  using namespace ramvm;

#line 54 "ramvm_bison_parser.hpp"


# include <cstdlib> // std::abort
# include <iostream>
# include <stdexcept>
# include <string>
# include <vector>

#if defined __cplusplus
# define YY_CPLUSPLUS __cplusplus
#else
# define YY_CPLUSPLUS 199711L
#endif

// Support move semantics when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_MOVE           std::move
# define YY_MOVE_OR_COPY   move
# define YY_MOVE_REF(Type) Type&&
# define YY_RVREF(Type)    Type&&
# define YY_COPY(Type)     Type
#else
# define YY_MOVE
# define YY_MOVE_OR_COPY   copy
# define YY_MOVE_REF(Type) Type&
# define YY_RVREF(Type)    const Type&
# define YY_COPY(Type)     const Type&
#endif

// Support noexcept when possible.
#if 201103L <= YY_CPLUSPLUS
# define YY_NOEXCEPT noexcept
# define YY_NOTHROW
#else
# define YY_NOEXCEPT
# define YY_NOTHROW throw ()
#endif

// Support constexpr when possible.
#if 201703 <= YY_CPLUSPLUS
# define YY_CONSTEXPR constexpr
#else
# define YY_CONSTEXPR
#endif


#ifndef YY_ASSERT
# include <cassert>
# define YY_ASSERT assert
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

#line 8 "ramvm_grammar.yy"
namespace ramvm { namespace bison {
#line 189 "ramvm_bison_parser.hpp"




  /// A Bison parser.
  class Parser
  {
  public:
#ifndef YYSTYPE
  /// A buffer to store and retrieve objects.
  ///
  /// Sort of a variant, but does not keep track of the nature
  /// of the stored data, since that knowledge is available
  /// via the current parser state.
  class semantic_type
  {
  public:
    /// Type of *this.
    typedef semantic_type self_type;

    /// Empty construction.
    semantic_type () YY_NOEXCEPT
      : yybuffer_ ()
    {}

    /// Construct and fill.
    template <typename T>
    semantic_type (YY_RVREF (T) t)
    {
      YY_ASSERT (sizeof (T) <= size);
      new (yyas_<T> ()) T (YY_MOVE (t));
    }

    /// Destruction, allowed only if empty.
    ~semantic_type () YY_NOEXCEPT
    {}

# if 201103L <= YY_CPLUSPLUS
    /// Instantiate a \a T in here from \a t.
    template <typename T, typename... U>
    T&
    emplace (U&&... u)
    {
      return *new (yyas_<T> ()) T (std::forward <U>(u)...);
    }
# else
    /// Instantiate an empty \a T in here.
    template <typename T>
    T&
    emplace ()
    {
      return *new (yyas_<T> ()) T ();
    }

    /// Instantiate a \a T in here from \a t.
    template <typename T>
    T&
    emplace (const T& t)
    {
      return *new (yyas_<T> ()) T (t);
    }
# endif

    /// Instantiate an empty \a T in here.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build ()
    {
      return emplace<T> ();
    }

    /// Instantiate a \a T in here from \a t.
    /// Obsolete, use emplace.
    template <typename T>
    T&
    build (const T& t)
    {
      return emplace<T> (t);
    }

    /// Accessor to a built \a T.
    template <typename T>
    T&
    as () YY_NOEXCEPT
    {
      return *yyas_<T> ();
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    const T&
    as () const YY_NOEXCEPT
    {
      return *yyas_<T> ();
    }

    /// Swap the content with \a that, of same type.
    ///
    /// Both variants must be built beforehand, because swapping the actual
    /// data requires reading it (with as()), and this is not possible on
    /// unconstructed variants: it would require some dynamic testing, which
    /// should not be the variant's responsibility.
    /// Swapping between built and (possibly) non-built is done with
    /// self_type::move ().
    template <typename T>
    void
    swap (self_type& that) YY_NOEXCEPT
    {
      std::swap (as<T> (), that.as<T> ());
    }

    /// Move the content of \a that to this.
    ///
    /// Destroys \a that.
    template <typename T>
    void
    move (self_type& that)
    {
# if 201103L <= YY_CPLUSPLUS
      emplace<T> (std::move (that.as<T> ()));
# else
      emplace<T> ();
      swap<T> (that);
# endif
      that.destroy<T> ();
    }

# if 201103L <= YY_CPLUSPLUS
    /// Move the content of \a that to this.
    template <typename T>
    void
    move (self_type&& that)
    {
      emplace<T> (std::move (that.as<T> ()));
      that.destroy<T> ();
    }
#endif

    /// Copy the content of \a that to this.
    template <typename T>
    void
    copy (const self_type& that)
    {
      emplace<T> (that.as<T> ());
    }

    /// Destroy the stored \a T.
    template <typename T>
    void
    destroy ()
    {
      as<T> ().~T ();
    }

  private:
    /// Prohibit blind copies.
    self_type& operator= (const self_type&);
    semantic_type (const self_type&);

    /// Accessor to raw memory as \a T.
    template <typename T>
    T*
    yyas_ () YY_NOEXCEPT
    {
      void *yyp = yybuffer_.yyraw;
      return static_cast<T*> (yyp);
     }

    /// Const accessor to raw memory as \a T.
    template <typename T>
    const T*
    yyas_ () const YY_NOEXCEPT
    {
      const void *yyp = yybuffer_.yyraw;
      return static_cast<const T*> (yyp);
     }

    /// An auxiliary type to compute the largest semantic type.
    union union_type
    {
      // ARGUMENT
      char dummy1[sizeof (Argument*)];

      // BINOP
      char dummy2[sizeof (Binop)];

      // "<T>"
      char dummy3[sizeof (DataType)];

      // "<TT>"
      char dummy4[sizeof (DataTypeDouble)];

      // "<TTT>"
      char dummy5[sizeof (DataTypeTriple)];

      // "hex"
      char dummy6[sizeof (DataValue)];

      // STMT
      char dummy7[sizeof (Instruction*)];

      // UNOP
      char dummy8[sizeof (Unop)];

      // "byte"
      char dummy9[sizeof (byte)];

      // "double"
      char dummy10[sizeof (double)];

      // "float"
      char dummy11[sizeof (float)];

      // "int"
      char dummy12[sizeof (int)];

      // "long"
      char dummy13[sizeof (rLong)];

      // "LABEL"
      char dummy14[sizeof (std::string)];

      // ARGUMENTS
      char dummy15[sizeof (std::vector<Argument*>)];
    };

    /// The size of the largest semantic type.
    enum { size = sizeof (union_type) };

    /// A buffer to store semantic values.
    union
    {
      /// Strongest alignment constraints.
      long double yyalign_me;
      /// A buffer large enough to store any of the semantic values.
      char yyraw[size];
    } yybuffer_;
  };

#else
    typedef YYSTYPE semantic_type;
#endif

    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const std::string& m)
        : std::runtime_error (m)
      {}

      syntax_error (const syntax_error& s)
        : std::runtime_error (s.what ())
      {}

      ~syntax_error () YY_NOEXCEPT YY_NOTHROW;
    };

    /// Tokens.
    struct token
    {
      enum yytokentype
      {
        TOK_END_OF_FILE = 0,
        TOK_HEX_LIT = 258,
        TOK_BYTE_LIT = 259,
        TOK_INT_LIT = 260,
        TOK_FLOAT_LIT = 261,
        TOK_DOUBLE_LIT = 262,
        TOK_LONG_LIT = 263,
        TOK_LSBRACKET = 264,
        TOK_RSBRACKET = 265,
        TOK_LCBRACKET = 266,
        TOK_RCBRACKET = 267,
        TOK_LPAREN = 268,
        TOK_RPAREN = 269,
        TOK_SEMICOLON = 270,
        TOK_COMMA = 271,
        TOK_PLUS = 272,
        TOK_MINUS = 273,
        TOK_SINGLE_TYPE = 274,
        TOK_DOUBLE_TYPE = 275,
        TOK_TRIPLE_TYPE = 276,
        TOK_LABEL = 277,
        TOK_REG_SP = 278,
        TOK_REG_FP = 279,
        TOK_REG_GP = 280,
        TOK_HALT = 281,
        TOK_MALLOC = 282,
        TOK_FREE = 283,
        TOK_CALL = 284,
        TOK_RET = 285,
        TOK_COMPARE = 286,
        TOK_PRINT = 287,
        TOK_JUMP = 288,
        TOK_JUMPT = 289,
        TOK_JUMPF = 290,
        TOK_MOV = 291,
        TOK_POP = 292,
        TOK_STORE = 293,
        TOK_PUSH = 294,
        TOK_ADD = 295,
        TOK_SUB = 296,
        TOK_MUL = 297,
        TOK_DIV = 298,
        TOK_MOD = 299,
        TOK_POW = 300,
        TOK_LSHIFT = 301,
        TOK_RSHIFT = 302,
        TOK_AND = 303,
        TOK_OR = 304,
        TOK_XOR = 305,
        TOK_LT = 306,
        TOK_GT = 307,
        TOK_LTEQ = 308,
        TOK_GTEQ = 309,
        TOK_EQ = 310,
        TOK_NEQ = 311,
        TOK_NEG = 312,
        TOK_NOT = 313
      };
    };

    /// (External) token type, as returned by yylex.
    typedef token::yytokentype token_type;

    /// Symbol type: an internal symbol number.
    typedef int symbol_number_type;

    /// The symbol type number to denote an empty symbol.
    enum { empty_symbol = -2 };

    /// Internal symbol number for tokens (subsumed by symbol_number_type).
    typedef signed char token_number_type;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol type
    /// via type_get ().
    ///
    /// Provide access to semantic value.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol ()
        : value ()
      {}

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      basic_symbol (basic_symbol&& that);
#endif

      /// Copy constructor.
      basic_symbol (const basic_symbol& that);

      /// Constructor for valueless symbols, and symbols from each type.
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t)
        : Base (t)
      {}
#else
      basic_symbol (typename Base::kind_type t)
        : Base (t)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, Argument*&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const Argument*& v)
        : Base (t)
        , value (v)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, Binop&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const Binop& v)
        : Base (t)
        , value (v)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, DataType&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const DataType& v)
        : Base (t)
        , value (v)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, DataTypeDouble&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const DataTypeDouble& v)
        : Base (t)
        , value (v)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, DataTypeTriple&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const DataTypeTriple& v)
        : Base (t)
        , value (v)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, DataValue&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const DataValue& v)
        : Base (t)
        , value (v)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, Instruction*&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const Instruction*& v)
        : Base (t)
        , value (v)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, Unop&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const Unop& v)
        : Base (t)
        , value (v)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, byte&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const byte& v)
        : Base (t)
        , value (v)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, double&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const double& v)
        : Base (t)
        , value (v)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, float&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const float& v)
        : Base (t)
        , value (v)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, int&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const int& v)
        : Base (t)
        , value (v)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, rLong&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const rLong& v)
        : Base (t)
        , value (v)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::string&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::string& v)
        : Base (t)
        , value (v)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::vector<Argument*>&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::vector<Argument*>& v)
        : Base (t)
        , value (v)
      {}
#endif

      /// Destroy the symbol.
      ~basic_symbol ()
      {
        clear ();
      }

      /// Destroy contents, and record that is empty.
      void clear ()
      {
        // User destructor.
        symbol_number_type yytype = this->type_get ();
        basic_symbol<Base>& yysym = *this;
        (void) yysym;
        switch (yytype)
        {
       default:
          break;
        }

        // Type destructor.
switch (yytype)
    {
      case 63: // ARGUMENT
        value.template destroy< Argument* > ();
        break;

      case 64: // BINOP
        value.template destroy< Binop > ();
        break;

      case 19: // "<T>"
        value.template destroy< DataType > ();
        break;

      case 20: // "<TT>"
        value.template destroy< DataTypeDouble > ();
        break;

      case 21: // "<TTT>"
        value.template destroy< DataTypeTriple > ();
        break;

      case 3: // "hex"
        value.template destroy< DataValue > ();
        break;

      case 62: // STMT
        value.template destroy< Instruction* > ();
        break;

      case 65: // UNOP
        value.template destroy< Unop > ();
        break;

      case 4: // "byte"
        value.template destroy< byte > ();
        break;

      case 7: // "double"
        value.template destroy< double > ();
        break;

      case 6: // "float"
        value.template destroy< float > ();
        break;

      case 5: // "int"
        value.template destroy< int > ();
        break;

      case 8: // "long"
        value.template destroy< rLong > ();
        break;

      case 22: // "LABEL"
        value.template destroy< std::string > ();
        break;

      case 61: // ARGUMENTS
        value.template destroy< std::vector<Argument*> > ();
        break;

      default:
        break;
    }

        Base::clear ();
      }

      /// Whether empty.
      bool empty () const YY_NOEXCEPT;

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      semantic_type value;

    private:
#if YY_CPLUSPLUS < 201103L
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& that);
#endif
    };

    /// Type access provider for token (enum) based symbols.
    struct by_type
    {
      /// Default constructor.
      by_type ();

#if 201103L <= YY_CPLUSPLUS
      /// Move constructor.
      by_type (by_type&& that);
#endif

      /// Copy constructor.
      by_type (const by_type& that);

      /// The symbol type as needed by the constructor.
      typedef token_type kind_type;

      /// Constructor from (external) token numbers.
      by_type (kind_type t);

      /// Record that this symbol is empty.
      void clear ();

      /// Steal the symbol type from \a that.
      void move (by_type& that);

      /// The (internal) type number (corresponding to \a type).
      /// \a empty when empty.
      symbol_number_type type_get () const YY_NOEXCEPT;

      /// The symbol type.
      /// \a empty_symbol when empty.
      /// An int, not token_number_type, to be able to store empty_symbol.
      int type;
    };

    /// "External" symbols: returned by the scanner.
    struct symbol_type : basic_symbol<by_type>
    {
      /// Superclass.
      typedef basic_symbol<by_type> super_type;

      /// Empty symbol.
      symbol_type () {}

      /// Constructor for valueless symbols, and symbols from each type.
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok)
        : super_type(token_type (tok))
      {
        YY_ASSERT (tok == token::TOK_END_OF_FILE || tok == token::TOK_LSBRACKET || tok == token::TOK_RSBRACKET || tok == token::TOK_LCBRACKET || tok == token::TOK_RCBRACKET || tok == token::TOK_LPAREN || tok == token::TOK_RPAREN || tok == token::TOK_SEMICOLON || tok == token::TOK_COMMA || tok == token::TOK_PLUS || tok == token::TOK_MINUS || tok == token::TOK_REG_SP || tok == token::TOK_REG_FP || tok == token::TOK_REG_GP || tok == token::TOK_HALT || tok == token::TOK_MALLOC || tok == token::TOK_FREE || tok == token::TOK_CALL || tok == token::TOK_RET || tok == token::TOK_COMPARE || tok == token::TOK_PRINT || tok == token::TOK_JUMP || tok == token::TOK_JUMPT || tok == token::TOK_JUMPF || tok == token::TOK_MOV || tok == token::TOK_POP || tok == token::TOK_STORE || tok == token::TOK_PUSH || tok == token::TOK_ADD || tok == token::TOK_SUB || tok == token::TOK_MUL || tok == token::TOK_DIV || tok == token::TOK_MOD || tok == token::TOK_POW || tok == token::TOK_LSHIFT || tok == token::TOK_RSHIFT || tok == token::TOK_AND || tok == token::TOK_OR || tok == token::TOK_XOR || tok == token::TOK_LT || tok == token::TOK_GT || tok == token::TOK_LTEQ || tok == token::TOK_GTEQ || tok == token::TOK_EQ || tok == token::TOK_NEQ || tok == token::TOK_NEG || tok == token::TOK_NOT);
      }
#else
      symbol_type (int tok)
        : super_type(token_type (tok))
      {
        YY_ASSERT (tok == token::TOK_END_OF_FILE || tok == token::TOK_LSBRACKET || tok == token::TOK_RSBRACKET || tok == token::TOK_LCBRACKET || tok == token::TOK_RCBRACKET || tok == token::TOK_LPAREN || tok == token::TOK_RPAREN || tok == token::TOK_SEMICOLON || tok == token::TOK_COMMA || tok == token::TOK_PLUS || tok == token::TOK_MINUS || tok == token::TOK_REG_SP || tok == token::TOK_REG_FP || tok == token::TOK_REG_GP || tok == token::TOK_HALT || tok == token::TOK_MALLOC || tok == token::TOK_FREE || tok == token::TOK_CALL || tok == token::TOK_RET || tok == token::TOK_COMPARE || tok == token::TOK_PRINT || tok == token::TOK_JUMP || tok == token::TOK_JUMPT || tok == token::TOK_JUMPF || tok == token::TOK_MOV || tok == token::TOK_POP || tok == token::TOK_STORE || tok == token::TOK_PUSH || tok == token::TOK_ADD || tok == token::TOK_SUB || tok == token::TOK_MUL || tok == token::TOK_DIV || tok == token::TOK_MOD || tok == token::TOK_POW || tok == token::TOK_LSHIFT || tok == token::TOK_RSHIFT || tok == token::TOK_AND || tok == token::TOK_OR || tok == token::TOK_XOR || tok == token::TOK_LT || tok == token::TOK_GT || tok == token::TOK_LTEQ || tok == token::TOK_GTEQ || tok == token::TOK_EQ || tok == token::TOK_NEQ || tok == token::TOK_NEG || tok == token::TOK_NOT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, DataType v)
        : super_type(token_type (tok), std::move (v))
      {
        YY_ASSERT (tok == token::TOK_SINGLE_TYPE);
      }
#else
      symbol_type (int tok, const DataType& v)
        : super_type(token_type (tok), v)
      {
        YY_ASSERT (tok == token::TOK_SINGLE_TYPE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, DataTypeDouble v)
        : super_type(token_type (tok), std::move (v))
      {
        YY_ASSERT (tok == token::TOK_DOUBLE_TYPE);
      }
#else
      symbol_type (int tok, const DataTypeDouble& v)
        : super_type(token_type (tok), v)
      {
        YY_ASSERT (tok == token::TOK_DOUBLE_TYPE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, DataTypeTriple v)
        : super_type(token_type (tok), std::move (v))
      {
        YY_ASSERT (tok == token::TOK_TRIPLE_TYPE);
      }
#else
      symbol_type (int tok, const DataTypeTriple& v)
        : super_type(token_type (tok), v)
      {
        YY_ASSERT (tok == token::TOK_TRIPLE_TYPE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, DataValue v)
        : super_type(token_type (tok), std::move (v))
      {
        YY_ASSERT (tok == token::TOK_HEX_LIT);
      }
#else
      symbol_type (int tok, const DataValue& v)
        : super_type(token_type (tok), v)
      {
        YY_ASSERT (tok == token::TOK_HEX_LIT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, byte v)
        : super_type(token_type (tok), std::move (v))
      {
        YY_ASSERT (tok == token::TOK_BYTE_LIT);
      }
#else
      symbol_type (int tok, const byte& v)
        : super_type(token_type (tok), v)
      {
        YY_ASSERT (tok == token::TOK_BYTE_LIT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, double v)
        : super_type(token_type (tok), std::move (v))
      {
        YY_ASSERT (tok == token::TOK_DOUBLE_LIT);
      }
#else
      symbol_type (int tok, const double& v)
        : super_type(token_type (tok), v)
      {
        YY_ASSERT (tok == token::TOK_DOUBLE_LIT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, float v)
        : super_type(token_type (tok), std::move (v))
      {
        YY_ASSERT (tok == token::TOK_FLOAT_LIT);
      }
#else
      symbol_type (int tok, const float& v)
        : super_type(token_type (tok), v)
      {
        YY_ASSERT (tok == token::TOK_FLOAT_LIT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, int v)
        : super_type(token_type (tok), std::move (v))
      {
        YY_ASSERT (tok == token::TOK_INT_LIT);
      }
#else
      symbol_type (int tok, const int& v)
        : super_type(token_type (tok), v)
      {
        YY_ASSERT (tok == token::TOK_INT_LIT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, rLong v)
        : super_type(token_type (tok), std::move (v))
      {
        YY_ASSERT (tok == token::TOK_LONG_LIT);
      }
#else
      symbol_type (int tok, const rLong& v)
        : super_type(token_type (tok), v)
      {
        YY_ASSERT (tok == token::TOK_LONG_LIT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, std::string v)
        : super_type(token_type (tok), std::move (v))
      {
        YY_ASSERT (tok == token::TOK_LABEL);
      }
#else
      symbol_type (int tok, const std::string& v)
        : super_type(token_type (tok), v)
      {
        YY_ASSERT (tok == token::TOK_LABEL);
      }
#endif
    };

    /// Build a parser object.
    Parser (Lexer& lexer_yyarg, std::vector<Instruction*>& result_yyarg, Position& position_yyarg, std::unordered_map<std::string, int>& labels_yyarg);
    virtual ~Parser ();

    /// Parse.  An alias for parse ().
    /// \returns  0 iff parsing succeeded.
    int operator() ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.
    /// \param msg    a description of the syntax error.
    virtual void error (const std::string& msg);

    /// Report a syntax error.
    void error (const syntax_error& err);

    // Implementation of make_symbol for each symbol type.
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_END_OF_FILE ()
      {
        return symbol_type (token::TOK_END_OF_FILE);
      }
#else
      static
      symbol_type
      make_TOK_END_OF_FILE ()
      {
        return symbol_type (token::TOK_END_OF_FILE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_HEX_LIT (DataValue v)
      {
        return symbol_type (token::TOK_HEX_LIT, std::move (v));
      }
#else
      static
      symbol_type
      make_TOK_HEX_LIT (const DataValue& v)
      {
        return symbol_type (token::TOK_HEX_LIT, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_BYTE_LIT (byte v)
      {
        return symbol_type (token::TOK_BYTE_LIT, std::move (v));
      }
#else
      static
      symbol_type
      make_TOK_BYTE_LIT (const byte& v)
      {
        return symbol_type (token::TOK_BYTE_LIT, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_INT_LIT (int v)
      {
        return symbol_type (token::TOK_INT_LIT, std::move (v));
      }
#else
      static
      symbol_type
      make_TOK_INT_LIT (const int& v)
      {
        return symbol_type (token::TOK_INT_LIT, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_FLOAT_LIT (float v)
      {
        return symbol_type (token::TOK_FLOAT_LIT, std::move (v));
      }
#else
      static
      symbol_type
      make_TOK_FLOAT_LIT (const float& v)
      {
        return symbol_type (token::TOK_FLOAT_LIT, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_DOUBLE_LIT (double v)
      {
        return symbol_type (token::TOK_DOUBLE_LIT, std::move (v));
      }
#else
      static
      symbol_type
      make_TOK_DOUBLE_LIT (const double& v)
      {
        return symbol_type (token::TOK_DOUBLE_LIT, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_LONG_LIT (rLong v)
      {
        return symbol_type (token::TOK_LONG_LIT, std::move (v));
      }
#else
      static
      symbol_type
      make_TOK_LONG_LIT (const rLong& v)
      {
        return symbol_type (token::TOK_LONG_LIT, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_LSBRACKET ()
      {
        return symbol_type (token::TOK_LSBRACKET);
      }
#else
      static
      symbol_type
      make_TOK_LSBRACKET ()
      {
        return symbol_type (token::TOK_LSBRACKET);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_RSBRACKET ()
      {
        return symbol_type (token::TOK_RSBRACKET);
      }
#else
      static
      symbol_type
      make_TOK_RSBRACKET ()
      {
        return symbol_type (token::TOK_RSBRACKET);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_LCBRACKET ()
      {
        return symbol_type (token::TOK_LCBRACKET);
      }
#else
      static
      symbol_type
      make_TOK_LCBRACKET ()
      {
        return symbol_type (token::TOK_LCBRACKET);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_RCBRACKET ()
      {
        return symbol_type (token::TOK_RCBRACKET);
      }
#else
      static
      symbol_type
      make_TOK_RCBRACKET ()
      {
        return symbol_type (token::TOK_RCBRACKET);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_LPAREN ()
      {
        return symbol_type (token::TOK_LPAREN);
      }
#else
      static
      symbol_type
      make_TOK_LPAREN ()
      {
        return symbol_type (token::TOK_LPAREN);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_RPAREN ()
      {
        return symbol_type (token::TOK_RPAREN);
      }
#else
      static
      symbol_type
      make_TOK_RPAREN ()
      {
        return symbol_type (token::TOK_RPAREN);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_SEMICOLON ()
      {
        return symbol_type (token::TOK_SEMICOLON);
      }
#else
      static
      symbol_type
      make_TOK_SEMICOLON ()
      {
        return symbol_type (token::TOK_SEMICOLON);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_COMMA ()
      {
        return symbol_type (token::TOK_COMMA);
      }
#else
      static
      symbol_type
      make_TOK_COMMA ()
      {
        return symbol_type (token::TOK_COMMA);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_PLUS ()
      {
        return symbol_type (token::TOK_PLUS);
      }
#else
      static
      symbol_type
      make_TOK_PLUS ()
      {
        return symbol_type (token::TOK_PLUS);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_MINUS ()
      {
        return symbol_type (token::TOK_MINUS);
      }
#else
      static
      symbol_type
      make_TOK_MINUS ()
      {
        return symbol_type (token::TOK_MINUS);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_SINGLE_TYPE (DataType v)
      {
        return symbol_type (token::TOK_SINGLE_TYPE, std::move (v));
      }
#else
      static
      symbol_type
      make_TOK_SINGLE_TYPE (const DataType& v)
      {
        return symbol_type (token::TOK_SINGLE_TYPE, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_DOUBLE_TYPE (DataTypeDouble v)
      {
        return symbol_type (token::TOK_DOUBLE_TYPE, std::move (v));
      }
#else
      static
      symbol_type
      make_TOK_DOUBLE_TYPE (const DataTypeDouble& v)
      {
        return symbol_type (token::TOK_DOUBLE_TYPE, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_TRIPLE_TYPE (DataTypeTriple v)
      {
        return symbol_type (token::TOK_TRIPLE_TYPE, std::move (v));
      }
#else
      static
      symbol_type
      make_TOK_TRIPLE_TYPE (const DataTypeTriple& v)
      {
        return symbol_type (token::TOK_TRIPLE_TYPE, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_LABEL (std::string v)
      {
        return symbol_type (token::TOK_LABEL, std::move (v));
      }
#else
      static
      symbol_type
      make_TOK_LABEL (const std::string& v)
      {
        return symbol_type (token::TOK_LABEL, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_REG_SP ()
      {
        return symbol_type (token::TOK_REG_SP);
      }
#else
      static
      symbol_type
      make_TOK_REG_SP ()
      {
        return symbol_type (token::TOK_REG_SP);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_REG_FP ()
      {
        return symbol_type (token::TOK_REG_FP);
      }
#else
      static
      symbol_type
      make_TOK_REG_FP ()
      {
        return symbol_type (token::TOK_REG_FP);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_REG_GP ()
      {
        return symbol_type (token::TOK_REG_GP);
      }
#else
      static
      symbol_type
      make_TOK_REG_GP ()
      {
        return symbol_type (token::TOK_REG_GP);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_HALT ()
      {
        return symbol_type (token::TOK_HALT);
      }
#else
      static
      symbol_type
      make_TOK_HALT ()
      {
        return symbol_type (token::TOK_HALT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_MALLOC ()
      {
        return symbol_type (token::TOK_MALLOC);
      }
#else
      static
      symbol_type
      make_TOK_MALLOC ()
      {
        return symbol_type (token::TOK_MALLOC);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_FREE ()
      {
        return symbol_type (token::TOK_FREE);
      }
#else
      static
      symbol_type
      make_TOK_FREE ()
      {
        return symbol_type (token::TOK_FREE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_CALL ()
      {
        return symbol_type (token::TOK_CALL);
      }
#else
      static
      symbol_type
      make_TOK_CALL ()
      {
        return symbol_type (token::TOK_CALL);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_RET ()
      {
        return symbol_type (token::TOK_RET);
      }
#else
      static
      symbol_type
      make_TOK_RET ()
      {
        return symbol_type (token::TOK_RET);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_COMPARE ()
      {
        return symbol_type (token::TOK_COMPARE);
      }
#else
      static
      symbol_type
      make_TOK_COMPARE ()
      {
        return symbol_type (token::TOK_COMPARE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_PRINT ()
      {
        return symbol_type (token::TOK_PRINT);
      }
#else
      static
      symbol_type
      make_TOK_PRINT ()
      {
        return symbol_type (token::TOK_PRINT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_JUMP ()
      {
        return symbol_type (token::TOK_JUMP);
      }
#else
      static
      symbol_type
      make_TOK_JUMP ()
      {
        return symbol_type (token::TOK_JUMP);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_JUMPT ()
      {
        return symbol_type (token::TOK_JUMPT);
      }
#else
      static
      symbol_type
      make_TOK_JUMPT ()
      {
        return symbol_type (token::TOK_JUMPT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_JUMPF ()
      {
        return symbol_type (token::TOK_JUMPF);
      }
#else
      static
      symbol_type
      make_TOK_JUMPF ()
      {
        return symbol_type (token::TOK_JUMPF);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_MOV ()
      {
        return symbol_type (token::TOK_MOV);
      }
#else
      static
      symbol_type
      make_TOK_MOV ()
      {
        return symbol_type (token::TOK_MOV);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_POP ()
      {
        return symbol_type (token::TOK_POP);
      }
#else
      static
      symbol_type
      make_TOK_POP ()
      {
        return symbol_type (token::TOK_POP);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_STORE ()
      {
        return symbol_type (token::TOK_STORE);
      }
#else
      static
      symbol_type
      make_TOK_STORE ()
      {
        return symbol_type (token::TOK_STORE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_PUSH ()
      {
        return symbol_type (token::TOK_PUSH);
      }
#else
      static
      symbol_type
      make_TOK_PUSH ()
      {
        return symbol_type (token::TOK_PUSH);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_ADD ()
      {
        return symbol_type (token::TOK_ADD);
      }
#else
      static
      symbol_type
      make_TOK_ADD ()
      {
        return symbol_type (token::TOK_ADD);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_SUB ()
      {
        return symbol_type (token::TOK_SUB);
      }
#else
      static
      symbol_type
      make_TOK_SUB ()
      {
        return symbol_type (token::TOK_SUB);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_MUL ()
      {
        return symbol_type (token::TOK_MUL);
      }
#else
      static
      symbol_type
      make_TOK_MUL ()
      {
        return symbol_type (token::TOK_MUL);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_DIV ()
      {
        return symbol_type (token::TOK_DIV);
      }
#else
      static
      symbol_type
      make_TOK_DIV ()
      {
        return symbol_type (token::TOK_DIV);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_MOD ()
      {
        return symbol_type (token::TOK_MOD);
      }
#else
      static
      symbol_type
      make_TOK_MOD ()
      {
        return symbol_type (token::TOK_MOD);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_POW ()
      {
        return symbol_type (token::TOK_POW);
      }
#else
      static
      symbol_type
      make_TOK_POW ()
      {
        return symbol_type (token::TOK_POW);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_LSHIFT ()
      {
        return symbol_type (token::TOK_LSHIFT);
      }
#else
      static
      symbol_type
      make_TOK_LSHIFT ()
      {
        return symbol_type (token::TOK_LSHIFT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_RSHIFT ()
      {
        return symbol_type (token::TOK_RSHIFT);
      }
#else
      static
      symbol_type
      make_TOK_RSHIFT ()
      {
        return symbol_type (token::TOK_RSHIFT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_AND ()
      {
        return symbol_type (token::TOK_AND);
      }
#else
      static
      symbol_type
      make_TOK_AND ()
      {
        return symbol_type (token::TOK_AND);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_OR ()
      {
        return symbol_type (token::TOK_OR);
      }
#else
      static
      symbol_type
      make_TOK_OR ()
      {
        return symbol_type (token::TOK_OR);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_XOR ()
      {
        return symbol_type (token::TOK_XOR);
      }
#else
      static
      symbol_type
      make_TOK_XOR ()
      {
        return symbol_type (token::TOK_XOR);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_LT ()
      {
        return symbol_type (token::TOK_LT);
      }
#else
      static
      symbol_type
      make_TOK_LT ()
      {
        return symbol_type (token::TOK_LT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_GT ()
      {
        return symbol_type (token::TOK_GT);
      }
#else
      static
      symbol_type
      make_TOK_GT ()
      {
        return symbol_type (token::TOK_GT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_LTEQ ()
      {
        return symbol_type (token::TOK_LTEQ);
      }
#else
      static
      symbol_type
      make_TOK_LTEQ ()
      {
        return symbol_type (token::TOK_LTEQ);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_GTEQ ()
      {
        return symbol_type (token::TOK_GTEQ);
      }
#else
      static
      symbol_type
      make_TOK_GTEQ ()
      {
        return symbol_type (token::TOK_GTEQ);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_EQ ()
      {
        return symbol_type (token::TOK_EQ);
      }
#else
      static
      symbol_type
      make_TOK_EQ ()
      {
        return symbol_type (token::TOK_EQ);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_NEQ ()
      {
        return symbol_type (token::TOK_NEQ);
      }
#else
      static
      symbol_type
      make_TOK_NEQ ()
      {
        return symbol_type (token::TOK_NEQ);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_NEG ()
      {
        return symbol_type (token::TOK_NEG);
      }
#else
      static
      symbol_type
      make_TOK_NEG ()
      {
        return symbol_type (token::TOK_NEG);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TOK_NOT ()
      {
        return symbol_type (token::TOK_NOT);
      }
#else
      static
      symbol_type
      make_TOK_NOT ()
      {
        return symbol_type (token::TOK_NOT);
      }
#endif


  private:
    /// This class is not copyable.
    Parser (const Parser&);
    Parser& operator= (const Parser&);

    /// Check the lookahead yytoken.
    /// \returns  true iff the token will be eventually shifted.
    bool yy_lac_check_ (int yytoken) const;
    /// Establish the initial context if no initial context currently exists.
    /// \returns  true iff the token will be eventually shifted.
    bool yy_lac_establish_ (int yytoken);
    /// Discard any previous initial lookahead context because of event.
    /// \param event  the event which caused the lookahead to be discarded.
    ///               Only used for debbuging output.
    void yy_lac_discard_ (const char* event);

    /// Stored state numbers (used for stacks).
    typedef unsigned char state_type;

    /// Generate an error message.
    /// \param yystate   the state where the error occurred.
    /// \param yyla      the lookahead token.
    virtual std::string yysyntax_error_ (state_type yystate,
                                         const symbol_type& yyla) const;

    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yysym     the nonterminal to push on the stack
    static state_type yy_lr_goto_state_ (state_type yystate, int yysym);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue);

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue);

    static const signed char yypact_ninf_;
    static const signed char yytable_ninf_;

    /// Convert a scanner token number \a t to a symbol number.
    /// In theory \a t should be a token_type, but character literals
    /// are valid, yet not members of the token_type enum.
    static token_number_type yytranslate_ (int t);

    // Tables.
    // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
    // STATE-NUM.
    static const short yypact_[];

    // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
    // Performed when YYTABLE does not specify something else to do.  Zero
    // means the default is an error.
    static const signed char yydefact_[];

    // YYPGOTO[NTERM-NUM].
    static const signed char yypgoto_[];

    // YYDEFGOTO[NTERM-NUM].
    static const signed char yydefgoto_[];

    // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
    // positive, shift that token.  If negative, reduce the rule whose
    // number is the opposite.  If YYTABLE_NINF, syntax error.
    static const unsigned char yytable_[];

    static const short yycheck_[];

    // YYSTOS[STATE-NUM] -- The (internal number of the) accessing
    // symbol of state STATE-NUM.
    static const signed char yystos_[];

    // YYR1[YYN] -- Symbol number of symbol that rule YYN derives.
    static const signed char yyr1_[];

    // YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.
    static const signed char yyr2_[];


    /// Convert the symbol name \a n to a form suitable for a diagnostic.
    static std::string yytnamerr_ (const char *n);


    /// For a symbol, its name in clear.
    static const char* const yytname_[];
#if YYDEBUG
    // YYRLINE[YYN] -- Source line where rule number YYN was defined.
    static const short yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r);
    /// Print the state stack on the debug stream.
    virtual void yystack_print_ ();

    /// Debugging level.
    int yydebug_;
    /// Debug stream.
    std::ostream* yycdebug_;

    /// \brief Display a symbol type, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Base>
    void yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param yysym     The symbol.
    template <typename Base>
    void yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const;

  private:
    /// Type access provider for state based symbols.
    struct by_state
    {
      /// Default constructor.
      by_state () YY_NOEXCEPT;

      /// The symbol type as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s) YY_NOEXCEPT;

      /// Copy constructor.
      by_state (const by_state& that) YY_NOEXCEPT;

      /// Record that this symbol is empty.
      void clear () YY_NOEXCEPT;

      /// Steal the symbol type from \a that.
      void move (by_state& that);

      /// The (internal) type number (corresponding to \a state).
      /// \a empty_symbol when empty.
      symbol_number_type type_get () const YY_NOEXCEPT;

      /// The state number used to denote an empty symbol.
      /// We use the initial state, as it does not have a value.
      enum { empty_state = 0 };

      /// The state.
      /// \a empty when empty.
      state_type state;
    };

    /// "Internal" symbol: element of the stack.
    struct stack_symbol_type : basic_symbol<by_state>
    {
      /// Superclass.
      typedef basic_symbol<by_state> super_type;
      /// Construct an empty symbol.
      stack_symbol_type ();
      /// Move or copy construction.
      stack_symbol_type (YY_RVREF (stack_symbol_type) that);
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) sym);
#if YY_CPLUSPLUS < 201103L
      /// Assignment, needed by push_back by some old implementations.
      /// Moves the contents of that.
      stack_symbol_type& operator= (stack_symbol_type& that);

      /// Assignment, needed by push_back by other implementations.
      /// Needed by some other old implementations.
      stack_symbol_type& operator= (const stack_symbol_type& that);
#endif
    };

    /// A stack with random access from its top.
    template <typename T, typename S = std::vector<T> >
    class stack
    {
    public:
      // Hide our reversed order.
      typedef typename S::reverse_iterator iterator;
      typedef typename S::const_reverse_iterator const_iterator;
      typedef typename S::size_type size_type;
      typedef typename std::ptrdiff_t index_type;

      stack (size_type n = 200)
        : seq_ (n)
      {}

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      const T&
      operator[] (index_type i) const
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Random access.
      ///
      /// Index 0 returns the topmost element.
      T&
      operator[] (index_type i)
      {
        return seq_[size_type (size () - 1 - i)];
      }

      /// Steal the contents of \a t.
      ///
      /// Close to move-semantics.
      void
      push (YY_MOVE_REF (T) t)
      {
        seq_.push_back (T ());
        operator[] (0).move (t);
      }

      /// Pop elements from the stack.
      void
      pop (std::ptrdiff_t n = 1) YY_NOEXCEPT
      {
        for (; 0 < n; --n)
          seq_.pop_back ();
      }

      /// Pop all elements from the stack.
      void
      clear () YY_NOEXCEPT
      {
        seq_.clear ();
      }

      /// Number of elements on the stack.
      index_type
      size () const YY_NOEXCEPT
      {
        return index_type (seq_.size ());
      }

      std::ptrdiff_t
      ssize () const YY_NOEXCEPT
      {
        return std::ptrdiff_t (size ());
      }

      /// Iterator on top of the stack (going downwards).
      const_iterator
      begin () const YY_NOEXCEPT
      {
        return seq_.rbegin ();
      }

      /// Bottom of the stack.
      const_iterator
      end () const YY_NOEXCEPT
      {
        return seq_.rend ();
      }

      /// Present a slice of the top of a stack.
      class slice
      {
      public:
        slice (const stack& stack, index_type range)
          : stack_ (stack)
          , range_ (range)
        {}

        const T&
        operator[] (index_type i) const
        {
          return stack_[range_ - i];
        }

      private:
        const stack& stack_;
        index_type range_;
      };

    private:
      stack (const stack&);
      stack& operator= (const stack&);
      /// The wrapped container.
      S seq_;
    };


    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;
    /// The stack for LAC.
    /// Logically, the yy_lac_stack's lifetime is confined to the function
    /// yy_lac_check_. We just store it as a member of this class to hold
    /// on to the memory and to avoid frequent reallocations.
    /// Since yy_lac_check_ is const, this member must be mutable.
    mutable std::vector<state_type> yylac_stack_;
    /// Whether an initial LAC context was established.
    bool yy_lac_established_;


    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param sym  the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a sym.value is stolen.
    void yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym);

    /// Pop \a n symbols from the stack.
    void yypop_ (int n = 1);

    /// Some specific tokens.
    static const token_number_type yy_error_token_ = 1;
    static const token_number_type yy_undef_token_ = 2;

    /// Constants.
    enum
    {
      yyeof_ = 0,
      yylast_ = 142,     ///< Last index in yytable_.
      yynnts_ = 8,  ///< Number of nonterminal symbols.
      yyfinal_ = 40, ///< Termination state number.
      yyntokens_ = 59  ///< Number of tokens.
    };


    // User arguments.
    Lexer& lexer;
    std::vector<Instruction*>& result;
    Position& position;
    std::unordered_map<std::string, int>& labels;
  };

  inline
  Parser::token_number_type
  Parser::yytranslate_ (int t)
  {
    // YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to
    // TOKEN-NUM as returned by yylex.
    static
    const token_number_type
    translate_table[] =
    {
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58
    };
    const int user_token_number_max_ = 313;

    if (t <= 0)
      return yyeof_;
    else if (t <= user_token_number_max_)
      return translate_table[t];
    else
      return yy_undef_token_;
  }

  // basic_symbol.
#if 201103L <= YY_CPLUSPLUS
  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (basic_symbol&& that)
    : Base (std::move (that))
    , value ()
  {
    switch (this->type_get ())
    {
      case 63: // ARGUMENT
        value.move< Argument* > (std::move (that.value));
        break;

      case 64: // BINOP
        value.move< Binop > (std::move (that.value));
        break;

      case 19: // "<T>"
        value.move< DataType > (std::move (that.value));
        break;

      case 20: // "<TT>"
        value.move< DataTypeDouble > (std::move (that.value));
        break;

      case 21: // "<TTT>"
        value.move< DataTypeTriple > (std::move (that.value));
        break;

      case 3: // "hex"
        value.move< DataValue > (std::move (that.value));
        break;

      case 62: // STMT
        value.move< Instruction* > (std::move (that.value));
        break;

      case 65: // UNOP
        value.move< Unop > (std::move (that.value));
        break;

      case 4: // "byte"
        value.move< byte > (std::move (that.value));
        break;

      case 7: // "double"
        value.move< double > (std::move (that.value));
        break;

      case 6: // "float"
        value.move< float > (std::move (that.value));
        break;

      case 5: // "int"
        value.move< int > (std::move (that.value));
        break;

      case 8: // "long"
        value.move< rLong > (std::move (that.value));
        break;

      case 22: // "LABEL"
        value.move< std::string > (std::move (that.value));
        break;

      case 61: // ARGUMENTS
        value.move< std::vector<Argument*> > (std::move (that.value));
        break;

      default:
        break;
    }

  }
#endif

  template <typename Base>
  Parser::basic_symbol<Base>::basic_symbol (const basic_symbol& that)
    : Base (that)
    , value ()
  {
    switch (this->type_get ())
    {
      case 63: // ARGUMENT
        value.copy< Argument* > (YY_MOVE (that.value));
        break;

      case 64: // BINOP
        value.copy< Binop > (YY_MOVE (that.value));
        break;

      case 19: // "<T>"
        value.copy< DataType > (YY_MOVE (that.value));
        break;

      case 20: // "<TT>"
        value.copy< DataTypeDouble > (YY_MOVE (that.value));
        break;

      case 21: // "<TTT>"
        value.copy< DataTypeTriple > (YY_MOVE (that.value));
        break;

      case 3: // "hex"
        value.copy< DataValue > (YY_MOVE (that.value));
        break;

      case 62: // STMT
        value.copy< Instruction* > (YY_MOVE (that.value));
        break;

      case 65: // UNOP
        value.copy< Unop > (YY_MOVE (that.value));
        break;

      case 4: // "byte"
        value.copy< byte > (YY_MOVE (that.value));
        break;

      case 7: // "double"
        value.copy< double > (YY_MOVE (that.value));
        break;

      case 6: // "float"
        value.copy< float > (YY_MOVE (that.value));
        break;

      case 5: // "int"
        value.copy< int > (YY_MOVE (that.value));
        break;

      case 8: // "long"
        value.copy< rLong > (YY_MOVE (that.value));
        break;

      case 22: // "LABEL"
        value.copy< std::string > (YY_MOVE (that.value));
        break;

      case 61: // ARGUMENTS
        value.copy< std::vector<Argument*> > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

  }



  template <typename Base>
  bool
  Parser::basic_symbol<Base>::empty () const YY_NOEXCEPT
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  void
  Parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move (s);
    switch (this->type_get ())
    {
      case 63: // ARGUMENT
        value.move< Argument* > (YY_MOVE (s.value));
        break;

      case 64: // BINOP
        value.move< Binop > (YY_MOVE (s.value));
        break;

      case 19: // "<T>"
        value.move< DataType > (YY_MOVE (s.value));
        break;

      case 20: // "<TT>"
        value.move< DataTypeDouble > (YY_MOVE (s.value));
        break;

      case 21: // "<TTT>"
        value.move< DataTypeTriple > (YY_MOVE (s.value));
        break;

      case 3: // "hex"
        value.move< DataValue > (YY_MOVE (s.value));
        break;

      case 62: // STMT
        value.move< Instruction* > (YY_MOVE (s.value));
        break;

      case 65: // UNOP
        value.move< Unop > (YY_MOVE (s.value));
        break;

      case 4: // "byte"
        value.move< byte > (YY_MOVE (s.value));
        break;

      case 7: // "double"
        value.move< double > (YY_MOVE (s.value));
        break;

      case 6: // "float"
        value.move< float > (YY_MOVE (s.value));
        break;

      case 5: // "int"
        value.move< int > (YY_MOVE (s.value));
        break;

      case 8: // "long"
        value.move< rLong > (YY_MOVE (s.value));
        break;

      case 22: // "LABEL"
        value.move< std::string > (YY_MOVE (s.value));
        break;

      case 61: // ARGUMENTS
        value.move< std::vector<Argument*> > (YY_MOVE (s.value));
        break;

      default:
        break;
    }

  }

  // by_type.
  inline
  Parser::by_type::by_type ()
    : type (empty_symbol)
  {}

#if 201103L <= YY_CPLUSPLUS
  inline
  Parser::by_type::by_type (by_type&& that)
    : type (that.type)
  {
    that.clear ();
  }
#endif

  inline
  Parser::by_type::by_type (const by_type& that)
    : type (that.type)
  {}

  inline
  Parser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
  Parser::by_type::clear ()
  {
    type = empty_symbol;
  }

  inline
  void
  Parser::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  inline
  int
  Parser::by_type::type_get () const YY_NOEXCEPT
  {
    return type;
  }

#line 8 "ramvm_grammar.yy"
} } // ramvm::bison
#line 2584 "ramvm_bison_parser.hpp"





#endif // !YY_YY_RAMVM_BISON_PARSER_HPP_INCLUDED
