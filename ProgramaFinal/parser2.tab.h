/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
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

#ifndef YY_YY_PARSER2_TAB_H_INCLUDED
# define YY_YY_PARSER2_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     ID = 258,
     NUM = 259,
     STRING = 260,
     CHAR = 261,
     START = 262,
     END = 263,
     INT = 264,
     FLOAT = 265,
     DOUBLE = 266,
     VOID = 267,
     STRUCT = 268,
     COLON = 269,
     COMMA = 270,
     WHILE = 271,
     DO = 272,
     PRINT = 273,
     SCAN = 274,
     PRED = 275,
     RETURN = 276,
     SWITCH = 277,
     BREAK = 278,
     CASE = 279,
     DEF = 280,
     DEFAULT = 281,
     SEMICOLON = 282,
     POINT = 283,
     TRUE = 284,
     FALSE = 285,
     ASSIGN = 286,
     OR = 287,
     AND = 288,
     NOTEQUAL = 289,
     EQUAL = 290,
     GT = 291,
     LT = 292,
     LESS = 293,
     GREATER = 294,
     MINUS = 295,
     PLUS = 296,
     MOD = 297,
     DIV = 298,
     MUL = 299,
     NOT = 300,
     RSQBRACK = 301,
     LSQBRACK = 302,
     RPAR = 303,
     LPAR = 304,
     THEN = 305,
     IF = 306,
     SIT = 307,
     ELSE = 308
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 2058 of yacc.c  */
#line 13 "parser2.y"

    int tipo;
    char id[32];
    char cadena[100];
    char caracter[1];
    int num;


/* Line 2058 of yacc.c  */
#line 119 "parser2.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_PARSER2_TAB_H_INCLUDED  */
