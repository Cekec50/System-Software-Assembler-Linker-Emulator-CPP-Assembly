/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_BISON_TAB_H_INCLUDED
# define YY_YY_BISON_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    COMMA = 258,
    OPEN_BRACKET = 259,
    CLOSED_BRACKET = 260,
    PLUS = 261,
    HALT = 262,
    INT2 = 263,
    CALL = 264,
    IRET = 265,
    RET = 266,
    JMP = 267,
    BEQ = 268,
    BNE = 269,
    BGT = 270,
    PUSH = 271,
    POP = 272,
    XCHG = 273,
    ADD = 274,
    SUB = 275,
    MUL = 276,
    DIV = 277,
    NOT = 278,
    AND = 279,
    OR = 280,
    XOR = 281,
    SHL = 282,
    SHR = 283,
    LD = 284,
    ST = 285,
    CSRRD = 286,
    CSRWR = 287,
    LABEL = 288,
    LITERAL_VAL = 289,
    LITERAL_MEM = 290,
    SYMBOL_VAL = 291,
    SYMBOL_MEM = 292,
    LITERAL_VAL_HEX = 293,
    gprX = 294,
    csrX = 295,
    STRING = 296,
    HEX = 297,
    GLOBAL = 298,
    EXTERN = 299,
    SECTION = 300,
    WORD = 301,
    SKIP = 302,
    ASCII = 303,
    EQU = 304,
    END = 305
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 15 "bison.y"

  int ival;
  float fval;
  char *sval;

#line 114 "bison.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_BISON_TAB_H_INCLUDED  */
