#ifndef SYMTABS_H
#define SYMTABS_H

#include <string>
#include <vector>

/* Symbol Records - struct to contain a function symbol */
struct symrec
{
  std::string name;
  double (*fnctptr)(double);
};

typedef struct symrec symrec;

/* Class Records - struct to contain a vector of class methods */
struct classrec
{
  std::string name;
  std::vector<symrec> fncttab;
};

typedef struct classrec classrec;

/* The symbol table: a vector of `struct symrec'.     */
extern std::vector<classrec> class_table;

/*symrec *putsym (char *sym_name, int sym_type);
symrec *getsym (char *sym_name);*/

#endif
