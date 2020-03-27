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
 ** \file ramc_bison_parser.hpp
 ** Define the ramc::bison::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

// Undocumented macros, especially those whose name start with YY_,
// are private implementation details.  Do not rely on them.

#ifndef YY_YY_RAMC_BISON_PARSER_HPP_INCLUDED
# define YY_YY_RAMC_BISON_PARSER_HPP_INCLUDED
// "%code requires" blocks.
#line 18 "ramc_grammar.yy"

  #include "ramc_ast.h"
  #include "ramc_typesystem.h"

#line 53 "ramc_bison_parser.hpp"


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

#line 8 "ramc_grammar.yy"
namespace ramc { namespace bison {
#line 188 "ramc_bison_parser.hpp"




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
      // "true"
      // "false"
      char dummy1[sizeof (ASTBoolLit*)];

      // "BYTE_LIT"
      char dummy2[sizeof (ASTByteLit*)];

      // "DOUBLE_LIT"
      char dummy3[sizeof (ASTDoubleLit*)];

      // EXPR
      // EXPR1
      // EXPR2
      // EXPR3
      // EXPR4
      // EXPR5
      // EXPR6
      // EXPR7
      // EXPR8
      // EXPR9
      // EXPR10
      // EXPR11
      // EXPR12
      // ATOM
      char dummy4[sizeof (ASTExpr*)];

      // "FLOAT_LIT"
      char dummy5[sizeof (ASTFloatLit*)];

      // "ID"
      char dummy6[sizeof (ASTIdentifier*)];

      // "INT_LIT"
      char dummy7[sizeof (ASTIntLit*)];

      // "LONG_LIT"
      char dummy8[sizeof (ASTLongLit*)];

      // PROGRAM
      char dummy9[sizeof (ASTProgram*)];

      // STMT
      // ASSIGNMENT
      // VARDECL
      char dummy10[sizeof (ASTStmt*)];

      // "STRING_LIT"
      char dummy11[sizeof (ASTStringLit*)];

      // OP_ASSIGN
      char dummy12[sizeof (AssignmentType)];

      // "let"
      // "if"
      // "then"
      // "else"
      // "while"
      // "for"
      // "break"
      // "continue"
      // "return"
      char dummy13[sizeof (Position)];

      // "byte"
      // "bool"
      // "int"
      // "float"
      // "double"
      // "long"
      // "string"
      // "void"
      // TYPE
      char dummy14[sizeof (TypePtr)];

      // STMTS
      char dummy15[sizeof (std::vector<ASTStmt*>)];
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
        END_OF_FILE = 0,
        PLUS = 258,
        MINUS = 259,
        TIMES = 260,
        DIVIDE = 261,
        MOD = 262,
        POW = 263,
        BIN_AND = 264,
        BIN_OR = 265,
        BIN_XOR = 266,
        LSHIFT = 267,
        RSHIFT = 268,
        LT = 269,
        GT = 270,
        LT_EQ = 271,
        GT_EQ = 272,
        EQ_EQ = 273,
        NEQ = 274,
        LOG_AND = 275,
        LOG_OR = 276,
        LOG_NOT = 277,
        BIN_NOT = 278,
        EQ = 279,
        ADD_EQ = 280,
        SUB_EQ = 281,
        MUL_EQ = 282,
        DIV_EQ = 283,
        MOD_EQ = 284,
        POW_EQ = 285,
        BIN_AND_EQ = 286,
        BIN_OR_EQ = 287,
        BIN_XOR_EQ = 288,
        LSHIFT_EQ = 289,
        RSHIFT_EQ = 290,
        LPAREN = 291,
        RPAREN = 292,
        LCBRACKET = 293,
        RCBRACKET = 294,
        LSBRACKET = 295,
        RSBRACKET = 296,
        SEMICOLON = 297,
        PERIOD = 298,
        COLON = 299,
        COMMA = 300,
        UNDERSCORE = 301,
        IDENTIFIER = 302,
        INT_LIT = 303,
        STRING_LIT = 304,
        FLOAT_LIT = 305,
        BYTE_LIT = 306,
        DOUBLE_LIT = 307,
        LONG_LIT = 308,
        KW_TRUE = 309,
        KW_FALSE = 310,
        KW_BYTE = 311,
        KW_BOOL = 312,
        KW_INT = 313,
        KW_FLOAT = 314,
        KW_DOUBLE = 315,
        KW_LONG = 316,
        KW_STRING = 317,
        KW_VOID = 318,
        KW_LET = 319,
        KW_IF = 320,
        KW_THEN = 321,
        KW_ELSE = 322,
        KW_WHILE = 323,
        KW_FOR = 324,
        KW_BREAK = 325,
        KW_CONTINUE = 326,
        KW_RETURN = 327
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
      basic_symbol (typename Base::kind_type t, ASTBoolLit*&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const ASTBoolLit*& v)
        : Base (t)
        , value (v)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, ASTByteLit*&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const ASTByteLit*& v)
        : Base (t)
        , value (v)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, ASTDoubleLit*&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const ASTDoubleLit*& v)
        : Base (t)
        , value (v)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, ASTExpr*&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const ASTExpr*& v)
        : Base (t)
        , value (v)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, ASTFloatLit*&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const ASTFloatLit*& v)
        : Base (t)
        , value (v)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, ASTIdentifier*&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const ASTIdentifier*& v)
        : Base (t)
        , value (v)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, ASTIntLit*&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const ASTIntLit*& v)
        : Base (t)
        , value (v)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, ASTLongLit*&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const ASTLongLit*& v)
        : Base (t)
        , value (v)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, ASTProgram*&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const ASTProgram*& v)
        : Base (t)
        , value (v)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, ASTStmt*&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const ASTStmt*& v)
        : Base (t)
        , value (v)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, ASTStringLit*&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const ASTStringLit*& v)
        : Base (t)
        , value (v)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, AssignmentType&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const AssignmentType& v)
        : Base (t)
        , value (v)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, Position&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const Position& v)
        : Base (t)
        , value (v)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, TypePtr&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const TypePtr& v)
        : Base (t)
        , value (v)
      {}
#endif
#if 201103L <= YY_CPLUSPLUS
      basic_symbol (typename Base::kind_type t, std::vector<ASTStmt*>&& v)
        : Base (t)
        , value (std::move (v))
      {}
#else
      basic_symbol (typename Base::kind_type t, const std::vector<ASTStmt*>& v)
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
      case 54: // "true"
      case 55: // "false"
        value.template destroy< ASTBoolLit* > ();
        break;

      case 51: // "BYTE_LIT"
        value.template destroy< ASTByteLit* > ();
        break;

      case 52: // "DOUBLE_LIT"
        value.template destroy< ASTDoubleLit* > ();
        break;

      case 79: // EXPR
      case 80: // EXPR1
      case 81: // EXPR2
      case 82: // EXPR3
      case 83: // EXPR4
      case 84: // EXPR5
      case 85: // EXPR6
      case 86: // EXPR7
      case 87: // EXPR8
      case 88: // EXPR9
      case 89: // EXPR10
      case 90: // EXPR11
      case 91: // EXPR12
      case 92: // ATOM
        value.template destroy< ASTExpr* > ();
        break;

      case 50: // "FLOAT_LIT"
        value.template destroy< ASTFloatLit* > ();
        break;

      case 47: // "ID"
        value.template destroy< ASTIdentifier* > ();
        break;

      case 48: // "INT_LIT"
        value.template destroy< ASTIntLit* > ();
        break;

      case 53: // "LONG_LIT"
        value.template destroy< ASTLongLit* > ();
        break;

      case 74: // PROGRAM
        value.template destroy< ASTProgram* > ();
        break;

      case 75: // STMT
      case 76: // ASSIGNMENT
      case 77: // VARDECL
        value.template destroy< ASTStmt* > ();
        break;

      case 49: // "STRING_LIT"
        value.template destroy< ASTStringLit* > ();
        break;

      case 94: // OP_ASSIGN
        value.template destroy< AssignmentType > ();
        break;

      case 64: // "let"
      case 65: // "if"
      case 66: // "then"
      case 67: // "else"
      case 68: // "while"
      case 69: // "for"
      case 70: // "break"
      case 71: // "continue"
      case 72: // "return"
        value.template destroy< Position > ();
        break;

      case 56: // "byte"
      case 57: // "bool"
      case 58: // "int"
      case 59: // "float"
      case 60: // "double"
      case 61: // "long"
      case 62: // "string"
      case 63: // "void"
      case 78: // TYPE
        value.template destroy< TypePtr > ();
        break;

      case 93: // STMTS
        value.template destroy< std::vector<ASTStmt*> > ();
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
        YY_ASSERT (tok == token::END_OF_FILE || tok == token::PLUS || tok == token::MINUS || tok == token::TIMES || tok == token::DIVIDE || tok == token::MOD || tok == token::POW || tok == token::BIN_AND || tok == token::BIN_OR || tok == token::BIN_XOR || tok == token::LSHIFT || tok == token::RSHIFT || tok == token::LT || tok == token::GT || tok == token::LT_EQ || tok == token::GT_EQ || tok == token::EQ_EQ || tok == token::NEQ || tok == token::LOG_AND || tok == token::LOG_OR || tok == token::LOG_NOT || tok == token::BIN_NOT || tok == token::EQ || tok == token::ADD_EQ || tok == token::SUB_EQ || tok == token::MUL_EQ || tok == token::DIV_EQ || tok == token::MOD_EQ || tok == token::POW_EQ || tok == token::BIN_AND_EQ || tok == token::BIN_OR_EQ || tok == token::BIN_XOR_EQ || tok == token::LSHIFT_EQ || tok == token::RSHIFT_EQ || tok == token::LPAREN || tok == token::RPAREN || tok == token::LCBRACKET || tok == token::RCBRACKET || tok == token::LSBRACKET || tok == token::RSBRACKET || tok == token::SEMICOLON || tok == token::PERIOD || tok == token::COLON || tok == token::COMMA || tok == token::UNDERSCORE);
      }
#else
      symbol_type (int tok)
        : super_type(token_type (tok))
      {
        YY_ASSERT (tok == token::END_OF_FILE || tok == token::PLUS || tok == token::MINUS || tok == token::TIMES || tok == token::DIVIDE || tok == token::MOD || tok == token::POW || tok == token::BIN_AND || tok == token::BIN_OR || tok == token::BIN_XOR || tok == token::LSHIFT || tok == token::RSHIFT || tok == token::LT || tok == token::GT || tok == token::LT_EQ || tok == token::GT_EQ || tok == token::EQ_EQ || tok == token::NEQ || tok == token::LOG_AND || tok == token::LOG_OR || tok == token::LOG_NOT || tok == token::BIN_NOT || tok == token::EQ || tok == token::ADD_EQ || tok == token::SUB_EQ || tok == token::MUL_EQ || tok == token::DIV_EQ || tok == token::MOD_EQ || tok == token::POW_EQ || tok == token::BIN_AND_EQ || tok == token::BIN_OR_EQ || tok == token::BIN_XOR_EQ || tok == token::LSHIFT_EQ || tok == token::RSHIFT_EQ || tok == token::LPAREN || tok == token::RPAREN || tok == token::LCBRACKET || tok == token::RCBRACKET || tok == token::LSBRACKET || tok == token::RSBRACKET || tok == token::SEMICOLON || tok == token::PERIOD || tok == token::COLON || tok == token::COMMA || tok == token::UNDERSCORE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, ASTBoolLit* v)
        : super_type(token_type (tok), std::move (v))
      {
        YY_ASSERT (tok == token::KW_TRUE || tok == token::KW_FALSE);
      }
#else
      symbol_type (int tok, const ASTBoolLit*& v)
        : super_type(token_type (tok), v)
      {
        YY_ASSERT (tok == token::KW_TRUE || tok == token::KW_FALSE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, ASTByteLit* v)
        : super_type(token_type (tok), std::move (v))
      {
        YY_ASSERT (tok == token::BYTE_LIT);
      }
#else
      symbol_type (int tok, const ASTByteLit*& v)
        : super_type(token_type (tok), v)
      {
        YY_ASSERT (tok == token::BYTE_LIT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, ASTDoubleLit* v)
        : super_type(token_type (tok), std::move (v))
      {
        YY_ASSERT (tok == token::DOUBLE_LIT);
      }
#else
      symbol_type (int tok, const ASTDoubleLit*& v)
        : super_type(token_type (tok), v)
      {
        YY_ASSERT (tok == token::DOUBLE_LIT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, ASTFloatLit* v)
        : super_type(token_type (tok), std::move (v))
      {
        YY_ASSERT (tok == token::FLOAT_LIT);
      }
#else
      symbol_type (int tok, const ASTFloatLit*& v)
        : super_type(token_type (tok), v)
      {
        YY_ASSERT (tok == token::FLOAT_LIT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, ASTIdentifier* v)
        : super_type(token_type (tok), std::move (v))
      {
        YY_ASSERT (tok == token::IDENTIFIER);
      }
#else
      symbol_type (int tok, const ASTIdentifier*& v)
        : super_type(token_type (tok), v)
      {
        YY_ASSERT (tok == token::IDENTIFIER);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, ASTIntLit* v)
        : super_type(token_type (tok), std::move (v))
      {
        YY_ASSERT (tok == token::INT_LIT);
      }
#else
      symbol_type (int tok, const ASTIntLit*& v)
        : super_type(token_type (tok), v)
      {
        YY_ASSERT (tok == token::INT_LIT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, ASTLongLit* v)
        : super_type(token_type (tok), std::move (v))
      {
        YY_ASSERT (tok == token::LONG_LIT);
      }
#else
      symbol_type (int tok, const ASTLongLit*& v)
        : super_type(token_type (tok), v)
      {
        YY_ASSERT (tok == token::LONG_LIT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, ASTStringLit* v)
        : super_type(token_type (tok), std::move (v))
      {
        YY_ASSERT (tok == token::STRING_LIT);
      }
#else
      symbol_type (int tok, const ASTStringLit*& v)
        : super_type(token_type (tok), v)
      {
        YY_ASSERT (tok == token::STRING_LIT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, Position v)
        : super_type(token_type (tok), std::move (v))
      {
        YY_ASSERT (tok == token::KW_LET || tok == token::KW_IF || tok == token::KW_THEN || tok == token::KW_ELSE || tok == token::KW_WHILE || tok == token::KW_FOR || tok == token::KW_BREAK || tok == token::KW_CONTINUE || tok == token::KW_RETURN);
      }
#else
      symbol_type (int tok, const Position& v)
        : super_type(token_type (tok), v)
      {
        YY_ASSERT (tok == token::KW_LET || tok == token::KW_IF || tok == token::KW_THEN || tok == token::KW_ELSE || tok == token::KW_WHILE || tok == token::KW_FOR || tok == token::KW_BREAK || tok == token::KW_CONTINUE || tok == token::KW_RETURN);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      symbol_type (int tok, TypePtr v)
        : super_type(token_type (tok), std::move (v))
      {
        YY_ASSERT (tok == token::KW_BYTE || tok == token::KW_BOOL || tok == token::KW_INT || tok == token::KW_FLOAT || tok == token::KW_DOUBLE || tok == token::KW_LONG || tok == token::KW_STRING || tok == token::KW_VOID);
      }
#else
      symbol_type (int tok, const TypePtr& v)
        : super_type(token_type (tok), v)
      {
        YY_ASSERT (tok == token::KW_BYTE || tok == token::KW_BOOL || tok == token::KW_INT || tok == token::KW_FLOAT || tok == token::KW_DOUBLE || tok == token::KW_LONG || tok == token::KW_STRING || tok == token::KW_VOID);
      }
#endif
    };

    /// Build a parser object.
    Parser (Lexer& lexer_yyarg, ASTNode*& result_yyarg, Position& position_yyarg);
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
      make_END_OF_FILE ()
      {
        return symbol_type (token::END_OF_FILE);
      }
#else
      static
      symbol_type
      make_END_OF_FILE ()
      {
        return symbol_type (token::END_OF_FILE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PLUS ()
      {
        return symbol_type (token::PLUS);
      }
#else
      static
      symbol_type
      make_PLUS ()
      {
        return symbol_type (token::PLUS);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MINUS ()
      {
        return symbol_type (token::MINUS);
      }
#else
      static
      symbol_type
      make_MINUS ()
      {
        return symbol_type (token::MINUS);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_TIMES ()
      {
        return symbol_type (token::TIMES);
      }
#else
      static
      symbol_type
      make_TIMES ()
      {
        return symbol_type (token::TIMES);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIVIDE ()
      {
        return symbol_type (token::DIVIDE);
      }
#else
      static
      symbol_type
      make_DIVIDE ()
      {
        return symbol_type (token::DIVIDE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MOD ()
      {
        return symbol_type (token::MOD);
      }
#else
      static
      symbol_type
      make_MOD ()
      {
        return symbol_type (token::MOD);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_POW ()
      {
        return symbol_type (token::POW);
      }
#else
      static
      symbol_type
      make_POW ()
      {
        return symbol_type (token::POW);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BIN_AND ()
      {
        return symbol_type (token::BIN_AND);
      }
#else
      static
      symbol_type
      make_BIN_AND ()
      {
        return symbol_type (token::BIN_AND);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BIN_OR ()
      {
        return symbol_type (token::BIN_OR);
      }
#else
      static
      symbol_type
      make_BIN_OR ()
      {
        return symbol_type (token::BIN_OR);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BIN_XOR ()
      {
        return symbol_type (token::BIN_XOR);
      }
#else
      static
      symbol_type
      make_BIN_XOR ()
      {
        return symbol_type (token::BIN_XOR);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LSHIFT ()
      {
        return symbol_type (token::LSHIFT);
      }
#else
      static
      symbol_type
      make_LSHIFT ()
      {
        return symbol_type (token::LSHIFT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RSHIFT ()
      {
        return symbol_type (token::RSHIFT);
      }
#else
      static
      symbol_type
      make_RSHIFT ()
      {
        return symbol_type (token::RSHIFT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LT ()
      {
        return symbol_type (token::LT);
      }
#else
      static
      symbol_type
      make_LT ()
      {
        return symbol_type (token::LT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_GT ()
      {
        return symbol_type (token::GT);
      }
#else
      static
      symbol_type
      make_GT ()
      {
        return symbol_type (token::GT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LT_EQ ()
      {
        return symbol_type (token::LT_EQ);
      }
#else
      static
      symbol_type
      make_LT_EQ ()
      {
        return symbol_type (token::LT_EQ);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_GT_EQ ()
      {
        return symbol_type (token::GT_EQ);
      }
#else
      static
      symbol_type
      make_GT_EQ ()
      {
        return symbol_type (token::GT_EQ);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_EQ_EQ ()
      {
        return symbol_type (token::EQ_EQ);
      }
#else
      static
      symbol_type
      make_EQ_EQ ()
      {
        return symbol_type (token::EQ_EQ);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_NEQ ()
      {
        return symbol_type (token::NEQ);
      }
#else
      static
      symbol_type
      make_NEQ ()
      {
        return symbol_type (token::NEQ);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LOG_AND ()
      {
        return symbol_type (token::LOG_AND);
      }
#else
      static
      symbol_type
      make_LOG_AND ()
      {
        return symbol_type (token::LOG_AND);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LOG_OR ()
      {
        return symbol_type (token::LOG_OR);
      }
#else
      static
      symbol_type
      make_LOG_OR ()
      {
        return symbol_type (token::LOG_OR);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LOG_NOT ()
      {
        return symbol_type (token::LOG_NOT);
      }
#else
      static
      symbol_type
      make_LOG_NOT ()
      {
        return symbol_type (token::LOG_NOT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BIN_NOT ()
      {
        return symbol_type (token::BIN_NOT);
      }
#else
      static
      symbol_type
      make_BIN_NOT ()
      {
        return symbol_type (token::BIN_NOT);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_EQ ()
      {
        return symbol_type (token::EQ);
      }
#else
      static
      symbol_type
      make_EQ ()
      {
        return symbol_type (token::EQ);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_ADD_EQ ()
      {
        return symbol_type (token::ADD_EQ);
      }
#else
      static
      symbol_type
      make_ADD_EQ ()
      {
        return symbol_type (token::ADD_EQ);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SUB_EQ ()
      {
        return symbol_type (token::SUB_EQ);
      }
#else
      static
      symbol_type
      make_SUB_EQ ()
      {
        return symbol_type (token::SUB_EQ);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MUL_EQ ()
      {
        return symbol_type (token::MUL_EQ);
      }
#else
      static
      symbol_type
      make_MUL_EQ ()
      {
        return symbol_type (token::MUL_EQ);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DIV_EQ ()
      {
        return symbol_type (token::DIV_EQ);
      }
#else
      static
      symbol_type
      make_DIV_EQ ()
      {
        return symbol_type (token::DIV_EQ);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_MOD_EQ ()
      {
        return symbol_type (token::MOD_EQ);
      }
#else
      static
      symbol_type
      make_MOD_EQ ()
      {
        return symbol_type (token::MOD_EQ);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_POW_EQ ()
      {
        return symbol_type (token::POW_EQ);
      }
#else
      static
      symbol_type
      make_POW_EQ ()
      {
        return symbol_type (token::POW_EQ);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BIN_AND_EQ ()
      {
        return symbol_type (token::BIN_AND_EQ);
      }
#else
      static
      symbol_type
      make_BIN_AND_EQ ()
      {
        return symbol_type (token::BIN_AND_EQ);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BIN_OR_EQ ()
      {
        return symbol_type (token::BIN_OR_EQ);
      }
#else
      static
      symbol_type
      make_BIN_OR_EQ ()
      {
        return symbol_type (token::BIN_OR_EQ);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BIN_XOR_EQ ()
      {
        return symbol_type (token::BIN_XOR_EQ);
      }
#else
      static
      symbol_type
      make_BIN_XOR_EQ ()
      {
        return symbol_type (token::BIN_XOR_EQ);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LSHIFT_EQ ()
      {
        return symbol_type (token::LSHIFT_EQ);
      }
#else
      static
      symbol_type
      make_LSHIFT_EQ ()
      {
        return symbol_type (token::LSHIFT_EQ);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RSHIFT_EQ ()
      {
        return symbol_type (token::RSHIFT_EQ);
      }
#else
      static
      symbol_type
      make_RSHIFT_EQ ()
      {
        return symbol_type (token::RSHIFT_EQ);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LPAREN ()
      {
        return symbol_type (token::LPAREN);
      }
#else
      static
      symbol_type
      make_LPAREN ()
      {
        return symbol_type (token::LPAREN);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RPAREN ()
      {
        return symbol_type (token::RPAREN);
      }
#else
      static
      symbol_type
      make_RPAREN ()
      {
        return symbol_type (token::RPAREN);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LCBRACKET ()
      {
        return symbol_type (token::LCBRACKET);
      }
#else
      static
      symbol_type
      make_LCBRACKET ()
      {
        return symbol_type (token::LCBRACKET);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RCBRACKET ()
      {
        return symbol_type (token::RCBRACKET);
      }
#else
      static
      symbol_type
      make_RCBRACKET ()
      {
        return symbol_type (token::RCBRACKET);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LSBRACKET ()
      {
        return symbol_type (token::LSBRACKET);
      }
#else
      static
      symbol_type
      make_LSBRACKET ()
      {
        return symbol_type (token::LSBRACKET);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_RSBRACKET ()
      {
        return symbol_type (token::RSBRACKET);
      }
#else
      static
      symbol_type
      make_RSBRACKET ()
      {
        return symbol_type (token::RSBRACKET);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_SEMICOLON ()
      {
        return symbol_type (token::SEMICOLON);
      }
#else
      static
      symbol_type
      make_SEMICOLON ()
      {
        return symbol_type (token::SEMICOLON);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_PERIOD ()
      {
        return symbol_type (token::PERIOD);
      }
#else
      static
      symbol_type
      make_PERIOD ()
      {
        return symbol_type (token::PERIOD);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_COLON ()
      {
        return symbol_type (token::COLON);
      }
#else
      static
      symbol_type
      make_COLON ()
      {
        return symbol_type (token::COLON);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_COMMA ()
      {
        return symbol_type (token::COMMA);
      }
#else
      static
      symbol_type
      make_COMMA ()
      {
        return symbol_type (token::COMMA);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_UNDERSCORE ()
      {
        return symbol_type (token::UNDERSCORE);
      }
#else
      static
      symbol_type
      make_UNDERSCORE ()
      {
        return symbol_type (token::UNDERSCORE);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_IDENTIFIER (ASTIdentifier* v)
      {
        return symbol_type (token::IDENTIFIER, std::move (v));
      }
#else
      static
      symbol_type
      make_IDENTIFIER (const ASTIdentifier*& v)
      {
        return symbol_type (token::IDENTIFIER, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_INT_LIT (ASTIntLit* v)
      {
        return symbol_type (token::INT_LIT, std::move (v));
      }
#else
      static
      symbol_type
      make_INT_LIT (const ASTIntLit*& v)
      {
        return symbol_type (token::INT_LIT, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_STRING_LIT (ASTStringLit* v)
      {
        return symbol_type (token::STRING_LIT, std::move (v));
      }
#else
      static
      symbol_type
      make_STRING_LIT (const ASTStringLit*& v)
      {
        return symbol_type (token::STRING_LIT, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_FLOAT_LIT (ASTFloatLit* v)
      {
        return symbol_type (token::FLOAT_LIT, std::move (v));
      }
#else
      static
      symbol_type
      make_FLOAT_LIT (const ASTFloatLit*& v)
      {
        return symbol_type (token::FLOAT_LIT, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_BYTE_LIT (ASTByteLit* v)
      {
        return symbol_type (token::BYTE_LIT, std::move (v));
      }
#else
      static
      symbol_type
      make_BYTE_LIT (const ASTByteLit*& v)
      {
        return symbol_type (token::BYTE_LIT, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_DOUBLE_LIT (ASTDoubleLit* v)
      {
        return symbol_type (token::DOUBLE_LIT, std::move (v));
      }
#else
      static
      symbol_type
      make_DOUBLE_LIT (const ASTDoubleLit*& v)
      {
        return symbol_type (token::DOUBLE_LIT, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_LONG_LIT (ASTLongLit* v)
      {
        return symbol_type (token::LONG_LIT, std::move (v));
      }
#else
      static
      symbol_type
      make_LONG_LIT (const ASTLongLit*& v)
      {
        return symbol_type (token::LONG_LIT, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_KW_TRUE (ASTBoolLit* v)
      {
        return symbol_type (token::KW_TRUE, std::move (v));
      }
#else
      static
      symbol_type
      make_KW_TRUE (const ASTBoolLit*& v)
      {
        return symbol_type (token::KW_TRUE, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_KW_FALSE (ASTBoolLit* v)
      {
        return symbol_type (token::KW_FALSE, std::move (v));
      }
#else
      static
      symbol_type
      make_KW_FALSE (const ASTBoolLit*& v)
      {
        return symbol_type (token::KW_FALSE, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_KW_BYTE (TypePtr v)
      {
        return symbol_type (token::KW_BYTE, std::move (v));
      }
#else
      static
      symbol_type
      make_KW_BYTE (const TypePtr& v)
      {
        return symbol_type (token::KW_BYTE, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_KW_BOOL (TypePtr v)
      {
        return symbol_type (token::KW_BOOL, std::move (v));
      }
#else
      static
      symbol_type
      make_KW_BOOL (const TypePtr& v)
      {
        return symbol_type (token::KW_BOOL, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_KW_INT (TypePtr v)
      {
        return symbol_type (token::KW_INT, std::move (v));
      }
#else
      static
      symbol_type
      make_KW_INT (const TypePtr& v)
      {
        return symbol_type (token::KW_INT, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_KW_FLOAT (TypePtr v)
      {
        return symbol_type (token::KW_FLOAT, std::move (v));
      }
#else
      static
      symbol_type
      make_KW_FLOAT (const TypePtr& v)
      {
        return symbol_type (token::KW_FLOAT, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_KW_DOUBLE (TypePtr v)
      {
        return symbol_type (token::KW_DOUBLE, std::move (v));
      }
#else
      static
      symbol_type
      make_KW_DOUBLE (const TypePtr& v)
      {
        return symbol_type (token::KW_DOUBLE, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_KW_LONG (TypePtr v)
      {
        return symbol_type (token::KW_LONG, std::move (v));
      }
#else
      static
      symbol_type
      make_KW_LONG (const TypePtr& v)
      {
        return symbol_type (token::KW_LONG, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_KW_STRING (TypePtr v)
      {
        return symbol_type (token::KW_STRING, std::move (v));
      }
#else
      static
      symbol_type
      make_KW_STRING (const TypePtr& v)
      {
        return symbol_type (token::KW_STRING, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_KW_VOID (TypePtr v)
      {
        return symbol_type (token::KW_VOID, std::move (v));
      }
#else
      static
      symbol_type
      make_KW_VOID (const TypePtr& v)
      {
        return symbol_type (token::KW_VOID, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_KW_LET (Position v)
      {
        return symbol_type (token::KW_LET, std::move (v));
      }
#else
      static
      symbol_type
      make_KW_LET (const Position& v)
      {
        return symbol_type (token::KW_LET, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_KW_IF (Position v)
      {
        return symbol_type (token::KW_IF, std::move (v));
      }
#else
      static
      symbol_type
      make_KW_IF (const Position& v)
      {
        return symbol_type (token::KW_IF, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_KW_THEN (Position v)
      {
        return symbol_type (token::KW_THEN, std::move (v));
      }
#else
      static
      symbol_type
      make_KW_THEN (const Position& v)
      {
        return symbol_type (token::KW_THEN, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_KW_ELSE (Position v)
      {
        return symbol_type (token::KW_ELSE, std::move (v));
      }
#else
      static
      symbol_type
      make_KW_ELSE (const Position& v)
      {
        return symbol_type (token::KW_ELSE, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_KW_WHILE (Position v)
      {
        return symbol_type (token::KW_WHILE, std::move (v));
      }
#else
      static
      symbol_type
      make_KW_WHILE (const Position& v)
      {
        return symbol_type (token::KW_WHILE, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_KW_FOR (Position v)
      {
        return symbol_type (token::KW_FOR, std::move (v));
      }
#else
      static
      symbol_type
      make_KW_FOR (const Position& v)
      {
        return symbol_type (token::KW_FOR, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_KW_BREAK (Position v)
      {
        return symbol_type (token::KW_BREAK, std::move (v));
      }
#else
      static
      symbol_type
      make_KW_BREAK (const Position& v)
      {
        return symbol_type (token::KW_BREAK, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_KW_CONTINUE (Position v)
      {
        return symbol_type (token::KW_CONTINUE, std::move (v));
      }
#else
      static
      symbol_type
      make_KW_CONTINUE (const Position& v)
      {
        return symbol_type (token::KW_CONTINUE, v);
      }
#endif
#if 201103L <= YY_CPLUSPLUS
      static
      symbol_type
      make_KW_RETURN (Position v)
      {
        return symbol_type (token::KW_RETURN, std::move (v));
      }
#else
      static
      symbol_type
      make_KW_RETURN (const Position& v)
      {
        return symbol_type (token::KW_RETURN, v);
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
    typedef signed char state_type;

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
    static const signed char yypact_[];

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
    static const signed char yytable_[];

    static const signed char yycheck_[];

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
      yylast_ = 120,     ///< Last index in yytable_.
      yynnts_ = 22,  ///< Number of nonterminal symbols.
      yyfinal_ = 3, ///< Termination state number.
      yyntokens_ = 73  ///< Number of tokens.
    };


    // User arguments.
    Lexer& lexer;
    ASTNode*& result;
    Position& position;
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
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72
    };
    const int user_token_number_max_ = 327;

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
      case 54: // "true"
      case 55: // "false"
        value.move< ASTBoolLit* > (std::move (that.value));
        break;

      case 51: // "BYTE_LIT"
        value.move< ASTByteLit* > (std::move (that.value));
        break;

      case 52: // "DOUBLE_LIT"
        value.move< ASTDoubleLit* > (std::move (that.value));
        break;

      case 79: // EXPR
      case 80: // EXPR1
      case 81: // EXPR2
      case 82: // EXPR3
      case 83: // EXPR4
      case 84: // EXPR5
      case 85: // EXPR6
      case 86: // EXPR7
      case 87: // EXPR8
      case 88: // EXPR9
      case 89: // EXPR10
      case 90: // EXPR11
      case 91: // EXPR12
      case 92: // ATOM
        value.move< ASTExpr* > (std::move (that.value));
        break;

      case 50: // "FLOAT_LIT"
        value.move< ASTFloatLit* > (std::move (that.value));
        break;

      case 47: // "ID"
        value.move< ASTIdentifier* > (std::move (that.value));
        break;

      case 48: // "INT_LIT"
        value.move< ASTIntLit* > (std::move (that.value));
        break;

      case 53: // "LONG_LIT"
        value.move< ASTLongLit* > (std::move (that.value));
        break;

      case 74: // PROGRAM
        value.move< ASTProgram* > (std::move (that.value));
        break;

      case 75: // STMT
      case 76: // ASSIGNMENT
      case 77: // VARDECL
        value.move< ASTStmt* > (std::move (that.value));
        break;

      case 49: // "STRING_LIT"
        value.move< ASTStringLit* > (std::move (that.value));
        break;

      case 94: // OP_ASSIGN
        value.move< AssignmentType > (std::move (that.value));
        break;

      case 64: // "let"
      case 65: // "if"
      case 66: // "then"
      case 67: // "else"
      case 68: // "while"
      case 69: // "for"
      case 70: // "break"
      case 71: // "continue"
      case 72: // "return"
        value.move< Position > (std::move (that.value));
        break;

      case 56: // "byte"
      case 57: // "bool"
      case 58: // "int"
      case 59: // "float"
      case 60: // "double"
      case 61: // "long"
      case 62: // "string"
      case 63: // "void"
      case 78: // TYPE
        value.move< TypePtr > (std::move (that.value));
        break;

      case 93: // STMTS
        value.move< std::vector<ASTStmt*> > (std::move (that.value));
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
      case 54: // "true"
      case 55: // "false"
        value.copy< ASTBoolLit* > (YY_MOVE (that.value));
        break;

      case 51: // "BYTE_LIT"
        value.copy< ASTByteLit* > (YY_MOVE (that.value));
        break;

      case 52: // "DOUBLE_LIT"
        value.copy< ASTDoubleLit* > (YY_MOVE (that.value));
        break;

      case 79: // EXPR
      case 80: // EXPR1
      case 81: // EXPR2
      case 82: // EXPR3
      case 83: // EXPR4
      case 84: // EXPR5
      case 85: // EXPR6
      case 86: // EXPR7
      case 87: // EXPR8
      case 88: // EXPR9
      case 89: // EXPR10
      case 90: // EXPR11
      case 91: // EXPR12
      case 92: // ATOM
        value.copy< ASTExpr* > (YY_MOVE (that.value));
        break;

      case 50: // "FLOAT_LIT"
        value.copy< ASTFloatLit* > (YY_MOVE (that.value));
        break;

      case 47: // "ID"
        value.copy< ASTIdentifier* > (YY_MOVE (that.value));
        break;

      case 48: // "INT_LIT"
        value.copy< ASTIntLit* > (YY_MOVE (that.value));
        break;

      case 53: // "LONG_LIT"
        value.copy< ASTLongLit* > (YY_MOVE (that.value));
        break;

      case 74: // PROGRAM
        value.copy< ASTProgram* > (YY_MOVE (that.value));
        break;

      case 75: // STMT
      case 76: // ASSIGNMENT
      case 77: // VARDECL
        value.copy< ASTStmt* > (YY_MOVE (that.value));
        break;

      case 49: // "STRING_LIT"
        value.copy< ASTStringLit* > (YY_MOVE (that.value));
        break;

      case 94: // OP_ASSIGN
        value.copy< AssignmentType > (YY_MOVE (that.value));
        break;

      case 64: // "let"
      case 65: // "if"
      case 66: // "then"
      case 67: // "else"
      case 68: // "while"
      case 69: // "for"
      case 70: // "break"
      case 71: // "continue"
      case 72: // "return"
        value.copy< Position > (YY_MOVE (that.value));
        break;

      case 56: // "byte"
      case 57: // "bool"
      case 58: // "int"
      case 59: // "float"
      case 60: // "double"
      case 61: // "long"
      case 62: // "string"
      case 63: // "void"
      case 78: // TYPE
        value.copy< TypePtr > (YY_MOVE (that.value));
        break;

      case 93: // STMTS
        value.copy< std::vector<ASTStmt*> > (YY_MOVE (that.value));
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
      case 54: // "true"
      case 55: // "false"
        value.move< ASTBoolLit* > (YY_MOVE (s.value));
        break;

      case 51: // "BYTE_LIT"
        value.move< ASTByteLit* > (YY_MOVE (s.value));
        break;

      case 52: // "DOUBLE_LIT"
        value.move< ASTDoubleLit* > (YY_MOVE (s.value));
        break;

      case 79: // EXPR
      case 80: // EXPR1
      case 81: // EXPR2
      case 82: // EXPR3
      case 83: // EXPR4
      case 84: // EXPR5
      case 85: // EXPR6
      case 86: // EXPR7
      case 87: // EXPR8
      case 88: // EXPR9
      case 89: // EXPR10
      case 90: // EXPR11
      case 91: // EXPR12
      case 92: // ATOM
        value.move< ASTExpr* > (YY_MOVE (s.value));
        break;

      case 50: // "FLOAT_LIT"
        value.move< ASTFloatLit* > (YY_MOVE (s.value));
        break;

      case 47: // "ID"
        value.move< ASTIdentifier* > (YY_MOVE (s.value));
        break;

      case 48: // "INT_LIT"
        value.move< ASTIntLit* > (YY_MOVE (s.value));
        break;

      case 53: // "LONG_LIT"
        value.move< ASTLongLit* > (YY_MOVE (s.value));
        break;

      case 74: // PROGRAM
        value.move< ASTProgram* > (YY_MOVE (s.value));
        break;

      case 75: // STMT
      case 76: // ASSIGNMENT
      case 77: // VARDECL
        value.move< ASTStmt* > (YY_MOVE (s.value));
        break;

      case 49: // "STRING_LIT"
        value.move< ASTStringLit* > (YY_MOVE (s.value));
        break;

      case 94: // OP_ASSIGN
        value.move< AssignmentType > (YY_MOVE (s.value));
        break;

      case 64: // "let"
      case 65: // "if"
      case 66: // "then"
      case 67: // "else"
      case 68: // "while"
      case 69: // "for"
      case 70: // "break"
      case 71: // "continue"
      case 72: // "return"
        value.move< Position > (YY_MOVE (s.value));
        break;

      case 56: // "byte"
      case 57: // "bool"
      case 58: // "int"
      case 59: // "float"
      case 60: // "double"
      case 61: // "long"
      case 62: // "string"
      case 63: // "void"
      case 78: // TYPE
        value.move< TypePtr > (YY_MOVE (s.value));
        break;

      case 93: // STMTS
        value.move< std::vector<ASTStmt*> > (YY_MOVE (s.value));
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

#line 8 "ramc_grammar.yy"
} } // ramc::bison
#line 2967 "ramc_bison_parser.hpp"





#endif // !YY_YY_RAMC_BISON_PARSER_HPP_INCLUDED
