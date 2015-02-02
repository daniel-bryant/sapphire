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
#include "sapphire.h"
#include "internal.h"

using namespace std;
 
void* ParseAlloc(void* (*allocProc)(size_t));
void Parse(void* parser, int token, const char* tokenptr, bool* valid);
void ParseFree(void* parser, void(*freeProc)(void*));
YYSTYPE yylval;
 
void parse(istream& stream, int console) {
    // Set up the scanner
    yyscan_t scanner;
    yylex_init(&scanner);
    YY_BUFFER_STATE bufferState;
 
    // Set up the parser
    void* gramParser = ParseAlloc(malloc);
 
    int lexCode;
    //struct Token tokenInfo;
    bool validParse = true;
    string str;

    if (console) cout << "sapphire :" << console << " > ";
    while ( getline(stream, str) ) {
      bufferState = yy_scan_string(str.c_str(), scanner);

      while ( validParse && (lexCode = yylex(scanner)) > 0 ) {
          Parse(gramParser, lexCode, yylval.sval, &validParse);
      }

      Parse(gramParser, NEWLINE, "\n", &validParse);
      if (console) cout << "sapphire :" << ++console << " > ";

      if (-1 == lexCode) {
          cerr << "The scanner encountered an error.\n";
      }

      if (!validParse) {
          cerr << "The parser encountered an error.\n";
      }
    }
 

    // Cleanup the scanner and parser
    yy_delete_buffer(bufferState, scanner);
    yylex_destroy(scanner);
    Parse(gramParser, 0, "End of Input", &validParse);
    ParseFree(gramParser, free);
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
  cout << "Sapphire Test Suite" << endl;

  DIR *dpdf;
  struct dirent *epdf;
  dpdf = opendir("./spec");

  if (dpdf != NULL){
    while ( (epdf = readdir(dpdf)) ) {
      string filename(epdf->d_name);

      if ( is_valid_filename(filename) ) {
        string spec_path = "spec/" + filename;
        //string spec_contents = get_file_contents(spec_path.c_str());
        ifstream file(spec_path.c_str());
        parse(file, 0);
      }
    }

    closedir(dpdf);
  } else {
    cout << "Cannot run test suite. No /spec directory found." << endl;
  }
}

void run_sapphire_console() {
  parse(cin, 1);
}

int main(int argc, char *argv[]) {
  alpha_ruby_init();

  /*VALUE fv = rb_float_new(3.14);
  double dv = rb_float_value(fv);
  cout << "original value should be 3.14 : " << dv << endl;*/
  cout << "Qtrue                 : " << (Qtrue) << endl;
  cout << "nil.nil? (should eq ^): " << rb_funcall(Qnil, rb_intern("nil?"), 0) << endl;
  cout << endl;

  VALUE flt = rb_funcall(rb_mMath, rb_intern("sin"), 1, 2);
  cout << "Math.sin(2) => 0.909...: " << rb_float_value_inline(flt) << endl;
  flt = rb_funcall(rb_mMath, rb_intern("cos"), 1, 2);
  cout << "Math.cos(2) => -0.416..: " << rb_float_value_inline(flt) << endl;
  flt = rb_funcall(rb_mMath, rb_intern("tan"), 1, 2);
  cout << "Math.tan(2) => -2.185..: " << rb_float_value_inline(flt) << endl;

  VALUE foobar = rb_const_get(rb_mMath, rb_intern("THIRTYSEVEN"));
  printf("Math.THIRTYSEVEN: %d\n", (int)foobar);

  /*VALUE v;
  v = rb_funcall(rb_mMath, "sin", 1, (VALUE)4);
  cout << "result: " << (int)v << endl;
  v = rb_funcall(rb_mMath, "cos", 1, (VALUE)4);
  cout << "result: " << (int)v << endl;
  v = rb_funcall(rb_mMath, "tan", 1, (VALUE)4);
  cout << "result: " << (int)v << endl;*/

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
