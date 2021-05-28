// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015 Free Software Foundation, Inc.

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


// First part of user declarations.
#line 6 "rcc.ypp" // lalr1.cc:404

#include <iostream>
#include <cctype>
#include <string>
#include "RccGlobal.hpp"
#include "rcc.tab.hpp"
using namespace std;
extern int yylex(yy::rcc::semantic_type *yylval, yy::rcc::location_type *yylloc, RccGlobal &global);

#line 46 "rcc.tab.cpp" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "rcc.tab.hpp"

// User implementation prologue.

#line 60 "rcc.tab.cpp" // lalr1.cc:412


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

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (/*CONSTCOND*/ false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

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
      *yycdebug_ << std::endl;                  \
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
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


namespace yy {
#line 146 "rcc.tab.cpp" // lalr1.cc:479

  /// Build a parser object.
  rcc::rcc (RccGlobal &global_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      global (global_yyarg)
  {}

  rcc::~rcc ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/

  inline
  rcc::syntax_error::syntax_error (const location_type& l, const std::string& m)
    : std::runtime_error (m)
    , location (l)
  {}

  // basic_symbol.
  template <typename Base>
  inline
  rcc::basic_symbol<Base>::basic_symbol ()
    : value ()
  {}

  template <typename Base>
  inline
  rcc::basic_symbol<Base>::basic_symbol (const basic_symbol& other)
    : Base (other)
    , value ()
    , location (other.location)
  {
    value = other.value;
  }


  template <typename Base>
  inline
  rcc::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const semantic_type& v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}


  /// Constructor for valueless symbols.
  template <typename Base>
  inline
  rcc::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  inline
  rcc::basic_symbol<Base>::~basic_symbol ()
  {
    clear ();
  }

  template <typename Base>
  inline
  void
  rcc::basic_symbol<Base>::clear ()
  {
    Base::clear ();
  }

  template <typename Base>
  inline
  bool
  rcc::basic_symbol<Base>::empty () const
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  inline
  void
  rcc::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move(s);
    value = s.value;
    location = s.location;
  }

  // by_type.
  inline
  rcc::by_type::by_type ()
    : type (empty_symbol)
  {}

  inline
  rcc::by_type::by_type (const by_type& other)
    : type (other.type)
  {}

  inline
  rcc::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
  rcc::by_type::clear ()
  {
    type = empty_symbol;
  }

  inline
  void
  rcc::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  inline
  int
  rcc::by_type::type_get () const
  {
    return type;
  }


  // by_state.
  inline
  rcc::by_state::by_state ()
    : state (empty_state)
  {}

  inline
  rcc::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  rcc::by_state::clear ()
  {
    state = empty_state;
  }

  inline
  void
  rcc::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  inline
  rcc::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  rcc::symbol_number_type
  rcc::by_state::type_get () const
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  inline
  rcc::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  rcc::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
    value = that.value;
    // that is emptied.
    that.type = empty_symbol;
  }

  inline
  rcc::stack_symbol_type&
  rcc::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    value = that.value;
    location = that.location;
    return *this;
  }


  template <typename Base>
  inline
  void
  rcc::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);

    // User destructor.
    YYUSE (yysym.type_get ());
  }

#if YYDEBUG
  template <typename Base>
  void
  rcc::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  inline
  void
  rcc::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  rcc::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  rcc::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  rcc::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  rcc::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  rcc::debug_level_type
  rcc::debug_level () const
  {
    return yydebug_;
  }

  void
  rcc::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  inline rcc::state_type
  rcc::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  inline bool
  rcc::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  rcc::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  rcc::parse ()
  {
    // State.
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    // User initialization code.
    #line 17 "rcc.ypp" // lalr1.cc:741
{
	yyla.location.begin.filename=yyla.location.end.filename=new std::string("stdin");
}

#line 486 "rcc.tab.cpp" // lalr1.cc:741

    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            yyla.type = yytranslate_ (yylex (&yyla.value, &yyla.location, global));
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

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
    yypush_ ("Shifting", yyn, yyla);
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
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* If YYLEN is nonzero, implement the default value of the
         action: '$$ = $1'.  Otherwise, use the top of the stack.

         Otherwise, the following line sets YYLHS.VALUE to garbage.
         This behavior is undocumented and Bison users should not rely
         upon it.  */
      if (yylen)
        yylhs.value = yystack_[yylen - 1].value;
      else
        yylhs.value = yystack_[0].value;

      // Compute the default @$.
      {
        slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, slice, yylen);
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
      try
        {
          switch (yyn)
            {
  case 5:
#line 43 "rcc.ypp" // lalr1.cc:859
    {
			cout << endl << "Finished." << endl;
			return 0;
		}
#line 599 "rcc.tab.cpp" // lalr1.cc:859
    break;


#line 603 "rcc.tab.cpp" // lalr1.cc:859
            default:
              break;
            }
        }
      catch (const syntax_error& yyexc)
        {
          error (yyexc);
          YYERROR;
        }
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, yylhs);
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
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
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

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    yyerror_range[1].location = yystack_[yylen - 1].location;
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
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

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
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
  rcc::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
  }

  // Generate an error message.
  std::string
  rcc::yysyntax_error_ (state_type, const symbol_type&) const
  {
    return YY_("syntax error");
  }


  const signed char rcc::yypact_ninf_ = -66;

  const signed char rcc::yytable_ninf_ = -1;

  const short int
  rcc::yypact_[] =
  {
      80,   -66,   -66,   -66,    15,     6,    80,   -66,    54,   -66,
     -66,   -23,   -66,   -66,   -66,   -66,     9,    16,   -66,   -14,
     -15,    73,   -66,    -4,   -66,     9,   255,    30,   -66,   -66,
     -66,   278,   330,   330,   -66,   164,   -66,   -66,   -66,   -66,
     -66,   -66,   -66,   126,   -66,   197,   -66,   -66,    35,   -66,
     -66,   -10,    20,    58,    19,    83,    69,    60,   109,    76,
     112,   -66,   125,   337,    10,   -66,   -66,   -66,   255,   -66,
     -66,    59,   164,   -66,   337,   -66,   -66,   101,    55,   -66,
     217,   -66,   -66,   -66,   337,   337,   337,   337,   337,   337,
     337,   337,   337,   337,   337,   337,   337,   337,   337,   337,
     337,   337,   337,   337,   -66,   -66,   -66,   -66,   -66,   -66,
     -66,   -66,   -66,   -66,   -66,   337,   -66,   -66,   139,   -66,
     -66,   271,   337,   157,   -66,    -5,   -66,   145,   337,   -66,
     -66,   -66,    20,    52,    58,    19,    83,    69,    60,    60,
     109,   109,   109,   109,    76,    76,   112,   112,   -66,   -66,
     -66,   -66,   -66,   -66,   -66,    62,     0,   -66,    21,   -66,
     -66,   -66,   337,   337,   -66,   -66,   -66,   -66,   -66,   -66
  };

  const unsigned char
  rcc::yydefact_[] =
  {
       4,    34,    35,    36,     0,     0,     3,     6,     0,    10,
       1,     0,     2,     7,    16,     8,     0,     0,    11,    13,
      15,     0,     5,     0,     9,     0,     0,     0,   106,   107,
     108,     0,     0,     0,    32,     0,    24,    90,    92,    93,
      91,    94,    95,     0,    31,     0,    28,    30,     0,    37,
      39,    52,    54,    56,    58,    60,    62,    64,    67,    72,
      75,    78,    82,     0,    84,    96,    17,    12,     0,    14,
      19,     0,     0,    88,     0,    85,    86,     0,     0,    26,
       0,    25,    29,    33,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    41,     0,    87,    82,     0,   102,
     103,     0,     0,     0,    22,     0,    18,     0,     0,   109,
      27,    38,    55,     0,    57,    59,    61,    63,    65,    66,
      70,    71,    68,    69,    73,    74,    76,    77,    79,    80,
      81,    40,   101,    98,   104,     0,     0,   100,     0,    20,
      89,    83,     0,     0,    99,    97,    21,    23,    53,   105
  };

  const short int
  rcc::yypgoto_[] =
  {
     -66,   -66,   -66,   -66,   162,    -2,   -66,   -66,   159,   169,
     -66,   -65,   -66,   175,   144,   -38,   -66,   -29,   -34,   -24,
     -66,    26,   -66,   104,   103,   105,   102,   107,    49,    22,
      57,    79,   -47,   -21,   -66,   -66,   -66,   -66
  };

  const short int
  rcc::yydefgoto_[] =
  {
      -1,     4,     5,    12,     6,     7,     8,    17,    18,    19,
      20,    69,   125,    44,    45,    46,    47,     9,    48,    49,
     115,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,   117,    63,    64,   155,    65
  };

  const unsigned char
  rcc::yytable_[] =
  {
      62,    78,    70,   124,    13,    62,    77,    82,    85,    11,
      73,    75,    76,    14,    62,    10,   116,    21,   118,   119,
     120,    26,    62,    27,    62,    28,    29,    30,    31,   158,
      32,    33,    86,    66,    84,    71,   159,    87,    78,   165,
      78,    13,    82,   127,    70,    16,   121,    62,   122,    24,
      25,    62,   133,    62,   148,   149,   150,    35,    14,    62,
     131,    68,   166,    62,    89,    62,   123,    37,    83,    84,
      38,    39,    40,    93,    94,    41,    42,    28,    29,    30,
      31,   161,    32,    33,    91,    92,    84,    15,   156,    84,
      16,   151,   129,   167,    62,   162,   163,   154,   126,   164,
      62,    62,    88,     1,     2,     3,    34,    95,    96,    35,
       1,     2,     3,    21,    36,   140,   141,   142,   143,    37,
      97,    98,    38,    39,    40,    99,   100,    41,    42,    90,
      28,    29,    30,    31,    70,    32,    33,    62,   128,   169,
     138,   139,    62,   152,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   144,   145,     1,     2,     3,    34,
     114,   157,    35,   101,   102,   103,    21,    79,    28,    29,
      30,    31,    37,    32,    33,    38,    39,    40,   146,   147,
      41,    42,   160,    43,    67,    23,    22,    80,   168,   132,
     134,   136,     0,   135,     1,     2,     3,   137,     0,     0,
      35,    28,    29,    30,    31,     0,    32,    33,     0,     0,
      37,     0,     0,    38,    39,    40,     0,     0,    41,    42,
       0,    28,    29,    30,    31,     0,    32,    33,     0,     0,
      34,     0,     0,    35,     0,     0,     0,    21,    81,     0,
       0,     0,     0,    37,     0,     0,    38,    39,    40,     0,
      34,    41,    42,    35,     0,     0,     0,    21,   130,    28,
      29,    30,    31,    37,    32,    33,    38,    39,    40,     0,
       0,    41,    42,     0,     0,    28,    29,    30,    31,     0,
      32,    33,    28,    29,    30,    31,     0,    32,    33,     0,
       0,    35,     0,     0,     0,    68,     0,     0,     0,     0,
       0,    37,     0,     0,    38,    39,    40,    35,   153,    41,
      42,     0,     0,     0,    72,     0,     0,    37,     0,     0,
      38,    39,    40,     0,    37,    41,    42,    38,    39,    40,
       0,     0,    41,    42,    28,    29,    30,    31,     0,    32,
      33,    28,    29,    30,    31,     0,    32,    33,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    74,     0,     0,     0,
       0,     0,     0,    35,     0,     0,    37,     0,     0,    38,
      39,    40,     0,    37,    41,    42,    38,    39,    40,     0,
       0,    41,    42
  };

  const short int
  rcc::yycheck_[] =
  {
      21,    35,    26,    68,     6,    26,    35,    45,    18,     3,
      31,    32,    33,     4,    35,     0,    63,    40,     8,     9,
      10,    35,    43,    38,    45,     4,     5,     6,     7,    34,
       9,    10,    42,    37,    34,     5,    41,    17,    72,    39,
      74,    43,    80,    72,    68,    36,    36,    68,    38,    33,
      34,    72,    86,    74,   101,   102,   103,    36,     4,    80,
      84,    40,    41,    84,    45,    86,    56,    46,    33,    34,
      49,    50,    51,    13,    14,    54,    55,     4,     5,     6,
       7,   128,     9,    10,    15,    16,    34,    33,   122,    34,
      36,   115,    37,   158,   115,    43,    34,   121,    39,    37,
     121,   122,    44,    30,    31,    32,    33,    47,    48,    36,
      30,    31,    32,    40,    41,    93,    94,    95,    96,    46,
      11,    12,    49,    50,    51,    49,    50,    54,    55,    46,
       4,     5,     6,     7,   158,     9,    10,   158,    37,   163,
      91,    92,   163,     4,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    97,    98,    30,    31,    32,    33,
      35,     4,    36,    51,    52,    53,    40,    41,     4,     5,
       6,     7,    46,     9,    10,    49,    50,    51,    99,   100,
      54,    55,    37,    21,    25,    16,    11,    43,   162,    85,
      87,    89,    -1,    88,    30,    31,    32,    90,    -1,    -1,
      36,     4,     5,     6,     7,    -1,     9,    10,    -1,    -1,
      46,    -1,    -1,    49,    50,    51,    -1,    -1,    54,    55,
      -1,     4,     5,     6,     7,    -1,     9,    10,    -1,    -1,
      33,    -1,    -1,    36,    -1,    -1,    -1,    40,    41,    -1,
      -1,    -1,    -1,    46,    -1,    -1,    49,    50,    51,    -1,
      33,    54,    55,    36,    -1,    -1,    -1,    40,    41,     4,
       5,     6,     7,    46,     9,    10,    49,    50,    51,    -1,
      -1,    54,    55,    -1,    -1,     4,     5,     6,     7,    -1,
       9,    10,     4,     5,     6,     7,    -1,     9,    10,    -1,
      -1,    36,    -1,    -1,    -1,    40,    -1,    -1,    -1,    -1,
      -1,    46,    -1,    -1,    49,    50,    51,    36,    37,    54,
      55,    -1,    -1,    -1,    36,    -1,    -1,    46,    -1,    -1,
      49,    50,    51,    -1,    46,    54,    55,    49,    50,    51,
      -1,    -1,    54,    55,     4,     5,     6,     7,    -1,     9,
      10,     4,     5,     6,     7,    -1,     9,    10,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    36,    -1,    -1,    -1,
      -1,    -1,    -1,    36,    -1,    -1,    46,    -1,    -1,    49,
      50,    51,    -1,    46,    54,    55,    49,    50,    51,    -1,
      -1,    54,    55
  };

  const unsigned char
  rcc::yystos_[] =
  {
       0,    30,    31,    32,    58,    59,    61,    62,    63,    74,
       0,     3,    60,    62,     4,    33,    36,    64,    65,    66,
      67,    40,    70,    66,    33,    34,    35,    38,     4,     5,
       6,     7,     9,    10,    33,    36,    41,    46,    49,    50,
      51,    54,    55,    61,    70,    71,    72,    73,    75,    76,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    94,    37,    65,    40,    68,
      76,     5,    36,    90,    36,    90,    90,    74,    75,    41,
      71,    41,    72,    33,    34,    18,    42,    17,    44,    45,
      46,    15,    16,    13,    14,    47,    48,    11,    12,    49,
      50,    51,    52,    53,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    35,    77,    89,    90,     8,     9,
      10,    36,    38,    56,    68,    69,    39,    74,    37,    37,
      41,    76,    80,    75,    81,    82,    83,    84,    85,    85,
      86,    86,    86,    86,    87,    87,    88,    88,    89,    89,
      89,    76,     4,    37,    76,    93,    75,     4,    34,    41,
      37,    89,    43,    34,    37,    39,    41,    68,    78,    76
  };

  const unsigned char
  rcc::yyr1_[] =
  {
       0,    57,    58,    59,    59,    60,    61,    61,    62,    62,
      63,    64,    64,    65,    65,    66,    67,    67,    67,    68,
      68,    68,    69,    69,    70,    70,    70,    70,    71,    71,
      72,    72,    73,    73,    74,    74,    74,    75,    75,    76,
      76,    77,    77,    77,    77,    77,    77,    77,    77,    77,
      77,    77,    78,    78,    79,    79,    80,    80,    81,    81,
      82,    82,    83,    83,    84,    84,    84,    85,    85,    85,
      85,    85,    86,    86,    86,    87,    87,    87,    88,    88,
      88,    88,    89,    89,    90,    90,    90,    90,    90,    90,
      91,    91,    91,    91,    91,    91,    92,    92,    92,    92,
      92,    92,    92,    92,    93,    93,    94,    94,    94,    94
  };

  const unsigned char
  rcc::yyr2_[] =
  {
       0,     2,     2,     1,     0,     2,     1,     2,     2,     3,
       1,     1,     3,     1,     3,     1,     1,     3,     4,     1,
       3,     4,     1,     3,     2,     3,     3,     4,     1,     2,
       1,     1,     1,     2,     1,     1,     1,     1,     3,     1,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     5,     1,     3,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     3,     1,     3,     3,
       3,     3,     1,     3,     3,     1,     3,     3,     1,     3,
       3,     3,     1,     4,     1,     2,     2,     2,     2,     4,
       1,     1,     1,     1,     1,     1,     1,     4,     3,     4,
       3,     3,     2,     2,     1,     3,     1,     1,     1,     3
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const rcc::yytname_[] =
  {
  "$end", "error", "$undefined", "MAIN", "IDENTIFIER", "CONSTANT",
  "STRING_LITERAL", "SIZEOF", "PTR_OP", "INC_OP", "DEC_OP", "LEFT_OP",
  "RIGHT_OP", "LE_OP", "GE_OP", "EQ_OP", "NE_OP", "AND_OP", "OR_OP",
  "MUL_ASSIGN", "DIV_ASSIGN", "MOD_ASSIGN", "ADD_ASSIGN", "SUB_ASSIGN",
  "LEFT_ASSIGN", "RIGHT_ASSIGN", "AND_ASSIGN", "XOR_ASSIGN", "OR_ASSIGN",
  "TYPE_NAME", "CHAR", "INT", "DOUBLE", "';'", "','", "'='", "'('", "')'",
  "'['", "']'", "'{'", "'}'", "'?'", "':'", "'|'", "'^'", "'&'", "'<'",
  "'>'", "'+'", "'-'", "'*'", "'/'", "'%'", "'~'", "'!'", "'.'", "$accept",
  "program", "program_head", "program_body", "declaration_list",
  "declaration", "declaration_specifiers", "init_declarator_list",
  "init_declarator", "declarator", "direct_declarator", "initializer",
  "initializer_list", "compound_statement", "statement_list", "statement",
  "expr_statement", "type_specifier", "expr", "assign_expr", "assign_op",
  "cond_expr", "logical_or_expr", "logical_and_expr", "inclusive_or_expr",
  "exclusive_or_expr", "and_expr", "equality_expr", "relational_expr",
  "shift_expr", "additive_expr", "multiplicative_expr", "cast_expr",
  "unary_expr", "unary_op", "postfix_expr", "argument_expr_list",
  "primary_expr", YY_NULLPTR
  };


  const unsigned short int
  rcc::yyrline_[] =
  {
       0,    33,    33,    37,    38,    42,    54,    55,    59,    60,
      64,    68,    69,    73,    74,    78,    82,    83,    84,    88,
      89,    90,    94,    95,   102,   103,   104,   105,   109,   110,
     114,   115,   119,   120,   127,   128,   129,   136,   137,   141,
     142,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   160,   161,   165,   166,   170,   171,   175,   176,
     180,   181,   185,   186,   190,   191,   192,   196,   197,   198,
     199,   200,   204,   205,   206,   210,   211,   212,   216,   217,
     218,   219,   223,   224,   228,   229,   230,   231,   232,   233,
     237,   238,   239,   240,   241,   242,   246,   247,   248,   249,
     250,   251,   252,   253,   257,   258,   262,   263,   264,   265
  };

  // Print the state stack on the debug stream.
  void
  rcc::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  rcc::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG

  // Symbol number corresponding to token number t.
  inline
  rcc::token_number_type
  rcc::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
     0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    55,     2,     2,     2,    53,    46,     2,
      36,    37,    51,    49,    34,    50,    56,    52,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    43,    33,
      47,    35,    48,    42,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    38,     2,    39,    45,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    40,    44,    41,    54,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32
    };
    const unsigned int user_token_number_max_ = 287;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }


} // yy
#line 1104 "rcc.tab.cpp" // lalr1.cc:1167
#line 268 "rcc.ypp" // lalr1.cc:1168

using namespace std;
extern char yytext[];
extern int column;

/* void yyerror(s)
char *s;
{
	fflush(stdout);
	printf("\n%*s\n%*s\n", column, "^", column, s);
} */

int main(int argc, char **argv)
{
	RccGlobal global;
	if(argc == 2)
		global.in = argv[1];
	else if(argc != 1)
		cout << "Usage: rcc [FILE_NAME]\nor\nrcc" << endl;
	yy::rcc parser(global);
	int v = parser.parse();
	return v;
}

namespace yy
{
	void rcc::error(location const &loc, const string &s){
		cerr << "error at " << loc << ": " << s << endl;	
	}
}
