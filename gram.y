%include
{
  #include <cassert>
  #include <iostream>
  #include <math.h>
  #include "sapphire.h"
  #include "token.h"
}

%syntax_error
{
  *valid = false;
}

%token_type {const char*}
%type expr {VALUE}
%type general_identity {VALUE}
%extra_argument {bool* valid}

%left POP_PARENS .
%left METHOD_CALL .
%left COMMA .
%left PLUS MINUS .
%left TIMES DIVIDE .
%right EXP .
/* PERIOD - Default precedence for method calls, may need to be moved later (for ex when 'def self.foobar' is added).
 * If this is incompatible, we can give method calls an alternative precendence rule in brackets eg [METHOD_CALL]
 */
%left PERIOD .

/* terminate expressions with new line and accept empty lines */
start ::= in .
in ::= .
in ::= in NEWLINE .
in ::= in expression NEWLINE .
/* end terminate expressions */

expression ::= expr(A) .             { if (A)
                                         std::cout << "result: " << value_to_int(A) << std::endl; }

expr(A) ::= expr    PLUS   expr .  { std::cout << "\tadding two expressions" << std::endl;         A = sp_funcall(int_to_value(2), "+", 1, int_to_value(2)); }
/* the above is a TEMP fix. Since we have things like 'Math.sin(3) + 3' in the test suite, but Math.sin not implemented attempting a "+" function call with result in a seg fault */
expr(A) ::= expr(B) MINUS  expr(C) .  { std::cout << "\tsubtracting two expressions" << std::endl; A = sp_funcall(B, "-", 1, C); }
expr(A) ::= expr(B) TIMES  expr(C) .  { std::cout << "\tmultiplying two expressions" << std::endl; A = sp_funcall(B, "*", 1, C); }
expr(A) ::= expr(B) DIVIDE expr(C) .  { std::cout << "\tdividing two expressions" << std::endl;    A = sp_funcall(B, "/", 1, C); }

expr ::= expr EXP expr .     { std::cout << "\texponent of two expressions" << std::endl; }
expr ::= MINUS expr . [EXP]  { std::cout << "\tnegating an expression" << std::endl; }

expr ::= LPAREN expr RPAREN . [POP_PARENS]  { std::cout << "\tpopping parenthases" << std::endl; }

/* method calling */

/* with a list of args */
expr ::= expr PERIOD general_identity(C) LPAREN expr_list RPAREN .  { std::cout << "\tmethod call with list of args in paren: " << C << std::endl; }
expr ::= expr PERIOD general_identity(C) expr_list . [METHOD_CALL]  { std::cout << "\tmethod call with list of args: " << C << std::endl; }
/* with a single arg */
expr ::= expr PERIOD general_identity(C) LPAREN expr RPAREN .       { std::cout << "\tmethod call with single arg in paren: " << C << std::endl; }
expr ::= expr PERIOD general_identity(C) expr . [METHOD_CALL]       { std::cout << "\tmethod call with single arg: " << C << std::endl; }
/* with no args */
expr ::= expr PERIOD general_identity(C) LPAREN RPAREN .            { std::cout << "\tmethod call with no args in paren: " << C << std::endl; }
expr ::= expr PERIOD general_identity(C) .                          { std::cout << "\tmethod call with no args: " << C << std::endl; }

/* arg list */
expr_list ::= expr_list COMMA expr .   { std::cout << "\texpression list continued" << std::endl; }
expr_list ::= expr COMMA expr .        { std::cout << "\texpression list started" << std::endl; }

/* end method calling */

/* As soon as we see a terminal symbol, we will convert it to a non-terminal symbol with a token type of VALUE */
expr(RESULT) ::= NUM(TOK_PTR) .               { std::cout << "\tfound number " << TOK_PTR << std::endl; RESULT = int_to_value(atoi(TOK_PTR)); }
expr ::= STRING(TOK_PTR) .                    { std::cout << "\tfound string " << TOK_PTR << std::endl; }
expr ::= PROPER_IDENTIFIER(TOK_PTR) .         { std::cout << "\tfound pronid " << TOK_PTR << std::endl; }
general_identity(A) ::= IDENTIFIER(TOK_PTR) . { std::cout << "\tfound genrid " << TOK_PTR << std::endl; /*A = strdup(TOK_PTR);*/ }
