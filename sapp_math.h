#ifndef SAPP_MATH_H
#define SAPP_MATH_H

#include "symtabs.h"
 
class SappMath
{
public:
    static double sapp_sin(double num);
    static double sapp_cos(double num);
    static double sapp_tan(double num);

    static classrec get_class_record();
};
 
#endif
