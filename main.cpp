#include "main.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include "gram.hpp"
#include "lexer.yy.hpp"
#include "lexglobal.h"
#include "token.h"
#include "sapp_math.h"
#include "symtabs.h"
using namespace std;
 
void* ParseAlloc(void* (*allocProc)(size_t));
void Parse(void* parser, int token, Token* tokenInfo, bool* valid);
void ParseFree(void* parser, void(*freeProc)(void*));
YYSTYPE yylval;
 
void parse(const string& commandLine) {
    // Set up the scanner
    yyscan_t scanner;
    yylex_init(&scanner);
    YY_BUFFER_STATE bufferState = yy_scan_string(commandLine.c_str(), scanner);
 
    // Set up the parser
    void* gramParser = ParseAlloc(malloc);
 
    int lexCode;
    struct Token tokenInfo;
    bool validParse = true;
    do {
        lexCode = yylex(scanner);
        //cout << "lexCode: " << lexCode << ", yylval: " << yylval.dval << endl;
        tokenInfo.num = yylval.dval;
        tokenInfo.str = yylval.sval;
        Parse(gramParser, lexCode, &tokenInfo, &validParse);
    }
    while (lexCode > 0 && validParse);
 
    if (-1 == lexCode) {
        cerr << "The scanner encountered an error.\n";
    }

    if (!validParse) {
        cerr << "The parser encountered an error.\n";
    }
 
    // Cleanup the scanner and parser
    yy_delete_buffer(bufferState, scanner);
    yylex_destroy(scanner);
    ParseFree(gramParser, free);
}

/* declare the Class Symbol Table */
vector<classrec> class_table;

void init_symbol_table()
{
  class_table.push_back( SappMath::get_class_record() );
}
 
int main() {
  init_symbol_table();
  string commandLine;
  while (getline(cin, commandLine)) {
      parse(commandLine);
  }
  return 0;
}

double call_class_method(string class_name, string method_name, double arg)
{
  classrec class_record;
  symrec function_record;

  /* search for a class record with the given name */
  for(auto &c_rec : class_table) {
    if (c_rec.name == class_name) {
      // record found, stop searching
      class_record = c_rec;
      break;
    }
  }

  /* if we found a class with the given name */
  if (class_record.name == class_name) {
    // search for the function record
    for(auto &f_rec : class_record.fncttab) {
      if ( f_rec.name == method_name) {
        // function found, stop searching
        function_record = f_rec;
        break;
      }
    }
  } else {
    // class not found, return an error message?
    return 0;
  }

  /* if we found a function with the given name */
  if (function_record.name == method_name)
    return function_record.fnctptr(arg);
  else
    // function not found, return an error message?
    return 0;
}
