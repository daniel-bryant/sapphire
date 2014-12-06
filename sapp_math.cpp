#include "sapp_math.h"

#include <math.h>
#include <vector>
#include "symtabs.h"

double SappMath::sapp_sin(double num)
{
  return sin(num);
}

double SappMath::sapp_cos(double num)
{
  return cos(num);
}

double SappMath::sapp_tan(double num)
{
  return tan(num);
}

classrec SappMath::get_class_record()
{
  std::vector<symrec> math_functions;
  
  symrec math_sin = {"sin", SappMath::sapp_sin};
  symrec math_cos = {"cos", SappMath::sapp_cos};
  symrec math_tan = {"tan", SappMath::sapp_tan};

  math_functions.push_back(math_sin);
  math_functions.push_back(math_cos);
  math_functions.push_back(math_tan);

  classrec mathrec = {"Math", math_functions};
  return mathrec;
}
