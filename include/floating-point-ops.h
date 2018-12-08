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
  void isNanOrInf(std::string value, int expBits, int mantissaBits);
  std::string toInfinity(bool isNegative, int exponentBits, int mantissaBits);
  std::string convertToIeee(std::string value, int exponentBits, int mantissaBits);
};

#endif