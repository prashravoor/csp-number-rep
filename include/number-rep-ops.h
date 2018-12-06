#ifndef NUMBER_REP_OPS_H
#define NUMBER_REP_OPS_H

#include <string>

class NumberRepOps
{
    public:
    void intToBin();

    void binToInt();

    std::string convertToBinary(long value);

    long convertToInteger(std::string value);
};

#endif