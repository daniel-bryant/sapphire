%include
{
  #include <cassert>
  #include <iostream>
  #include <math.h>
  #include "main.h"
  #include "token.h"
}

%syntax_error
{
  *valid = false;
}

%token_type {const char*}
%type expr {Token}
%type proper_identity {Token}
%type general_identity {Token}
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

start ::= expr(A) .                  { if (A.str)
                                         std::cout << "string: " << A.str << std::endl; }

expr ::= expr PLUS expr .    { std::cout << "adding two expressions" << std::endl; }
expr ::= expr MINUS expr .   { std::cout << "subtracting two expressions" << std::endl; }
expr ::= expr TIMES expr .   { std::cout << "multiplying two expressions" << std::endl; }
expr ::= expr DIVIDE expr .  { std::cout << "dividing two expressions" << std::endl; }

expr ::= expr EXP expr .     { std::cout << "exponent of two expressions" << std::endl; }
expr ::= MINUS expr . [EXP]  { std::cout << "negating an expression" << std::endl; }

expr ::= LPAREN expr RPAREN . [POP_PARENS]  { std::cout << "popping parenthases" << std::endl; }

/* method calling */

/* with a list of args */
expr ::= expr PERIOD general_identity(C) LPAREN expr_list RPAREN .  { std::cout << "method call with list of args in paren: " << C.str << std::endl; }
expr ::= expr PERIOD general_identity(C) expr_list . [METHOD_CALL]  { std::cout << "method call with list of args: " << C.str << std::endl; }
/* with a single arg */
expr ::= expr PERIOD general_identity(C) LPAREN expr RPAREN .       { std::cout << "method call with single arg in paren: " << C.str << std::endl; }
expr ::= expr PERIOD general_identity(C) expr . [METHOD_CALL]       { std::cout << "method call with single arg: " << C.str << std::endl; }
/* with no args */
expr ::= expr PERIOD general_identity(C) LPAREN RPAREN .            { std::cout << "method call with no args in paren: " << C.str << std::endl; }
expr ::= expr PERIOD general_identity(C) .                          { std::cout << "method call with no args: " << C.str << std::endl; }

/* arg list */
expr_list ::= expr_list COMMA expr .   { std::cout << "expression list continued" << std::endl; }
expr_list ::= expr COMMA expr .        { std::cout << "expression list started" << std::endl; }

/* end method calling */

expr ::= NUM(TOK_PTR) .                       { std::cout << "found number " << TOK_PTR << std::endl; }
expr ::= STRING(TOK_PTR) .                    { std::cout << "found string " << TOK_PTR << std::endl; }
expr ::= PROPER_IDENTIFIER(TOK_PTR) .         { std::cout << "found pronid " << TOK_PTR << std::endl; }
general_identity(A) ::= IDENTIFIER(TOK_PTR) . { std::cout << "found genrid " << TOK_PTR << std::endl; A.str = strdup(TOK_PTR); }
