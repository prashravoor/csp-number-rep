#ifndef FLOATING_POINT_OPS_H
#define FLOATING_POINT_OPS_H

#include <string>

class FloatingPointOps
{
  public:
    void IeeeToFraction();

    void FractionToIeee();

    float ieeeToSinglePrecision(std::string number);

    std::string singlePrecisionToIeee(std::string value);

    double ieeeToDoublePrecision(std::string number);

    std::string doublePrecisionToIeee(std::string value);

  private:
  bool validateFraction(std::string value);
};

#endif