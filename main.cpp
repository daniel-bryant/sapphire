#include "main.h"

#include <cerrno>
#include <cstdlib>
#include <dirent.h> // for reading directories
#include <fstream> // for file input
#include <iostream>
#include <sstream> // for string stream while reading a file
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

void initialize_sapphire_lang()
{
  // initialize symbol table
  class_table.push_back( SappMath::get_class_record() );
}
 
void print_usage_help()
{
  string help_string = "usage:\n\tsapphire COMMAND";
  help_string += "\n\navailable commands:";
  help_string += "\n\tsapphire help     # show help message";
  help_string += "\n\tsapphire console  # run the sapphire console";
  help_string += "\n\tsapphire test     # run the sapphire test suite";

  cout << help_string << endl;
}

bool is_valid_filename(string &filename) {
  bool valid = true;
  int namelength = filename.length();

  if ( filename == "." || filename == ".." ) {
    valid = false; // cout << "ignoring '.' or '..'" << endl;
  } else if ( namelength < 4 ) {
    valid = false; // cout << "filename too short: " << filename << endl;
  } else if ( filename.substr(namelength - 3) != ".sp" ) {
    valid = false; // cout << "not an .sp file: " << filename << endl;
  }

  return valid;
}

/*
 * get_file_contents - this method reads an entire file and returns a std::string
 * code taken from: http://insanecoding.blogspot.ca/2011/11/how-to-read-in-file-in-c.html
 * this is one of many ways to implement this method
 * we *could* read line by line but for now this suites our needs best
 */
string get_file_contents(const char *filename) {
  ifstream file(filename);
  if (file) {
    ostringstream contents;
    contents << file.rdbuf();
    file.close();
    return(contents.str());
  }
  throw(errno);
}

void run_sapphire_test_suite() {
  initialize_sapphire_lang();
  cout << "Sapphire Test Suite" << endl;

  DIR *dpdf;
  struct dirent *epdf;
  dpdf = opendir("./spec");

  if (dpdf != NULL){
    while ( (epdf = readdir(dpdf)) ) {
      string filename(epdf->d_name);

      if ( is_valid_filename(filename) ) {
        string spec_path = "spec/" + filename;
        string spec_contents = get_file_contents(spec_path.c_str());
        parse(spec_contents);
      }
    }

    closedir(dpdf);
  } else {
    cout << "Cannot run test suite. No /spec directory found." << endl;
  }
}

void run_sapphire_console() {
  initialize_sapphire_lang();

  string commandLine;
  int commandCount = 0;

  cout << "sapphire :" << ++commandCount << " > ";
  while (getline(cin, commandLine)) {
    parse(commandLine);
    cout << "sapphire :" << ++commandCount << " > ";
  }
}

int main(int argc, char *argv[]) {
  if (argc == 2) { // 2 args 'sapphire' and command
    if (strcmp(argv[1], "console") == 0) {
      run_sapphire_console();
    } else if (strcmp(argv[1], "test") == 0 ) {
      run_sapphire_test_suite();
    } else if (strcmp(argv[1], "help") == 0) {
      print_usage_help();
    } else {
      print_usage_help();
    }
  } else {
    print_usage_help();
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
