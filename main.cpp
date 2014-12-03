#include <cstdlib>
#include <iostream>
#include <string>
#include "gram.hpp"
#include "lexer.yy.hpp"
#include "lexglobal.h"
#include "token.h"
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
 
int main() {
    string commandLine;
    while (getline(cin, commandLine)) {
        parse(commandLine);
    }
    return 0;
}
