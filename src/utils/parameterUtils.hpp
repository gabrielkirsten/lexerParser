#include <string.h>
#include <iostream>

using namespace std;

class ParameterUtils
{
public:
    static bool hasParameters(int arg)
    {
        return arg > 1;
    }
};
