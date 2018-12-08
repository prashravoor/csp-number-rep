#include "floating-point-ops.h"
#include "timer.h"
#include "logger.h"
#include <iostream>
#include <algorithm>
#include "number-rep-ops.h"
#include <sstream>
#include <iomanip>
#include <cmath>

const double FloatingPointOps::log10base2 = log2(10);

void FloatingPointOps::FractionToIeee()
{
    std::cout << "Select precision:" << std::endl;
    std::cout << "1 Single\t2 Double: ";

    int choice = -1;
    std::cin >> choice;
    DLOG << "Choice selected: " << choice;

    if (std::cin.fail() || (choice != 1 && choice != 2))
    {
        ELOG << "Invalid choice, returning";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    switch (choice)
    {
    case 1:
    {
        std::cout << "Enter a fractional value in radix 10, in the acceptable format ( X.Y ): ";
        std::string value;

        std::cin >> value;
        if (std::cin.fail())
        {
            ELOG << "Invalid number format, returning";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return;
        }
        DLOG << "Value entered is " << value;

        if (!validateFraction(value))
        {
            isNanOrInf(value, 8, 23);
            return;
        }

        Timer timer;
        std::string str = singlePrecisionToIeee(value);
        std::cout << "The single precision representation of " << value << " is [" << str << "]" << std::endl;
        ILOG << "The entire operation took " << timer.getElapsedMicroseconds() << "us";
    }
    break;
    case 2:
    {
        std::cout << "Enter a fractional value in radix 10, in the acceptable format ( X.Y ) OR (XeY): ";
        std::string value;

        std::cin >> value;
        if (std::cin.fail())
        {
            ELOG << "Invalid number format, returning";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return;
        }

        DLOG << "Value entered is " << value;
        if (!validateFraction(value))
        {
            // Convert to NaN or INF
            isNanOrInf(value, 11, 52);
            return;
        }

        Timer timer;
        std::string str = doublePrecisionToIeee(value);

        std::cout << "The double precision representation of " << value << " is [" << str << "]" << std::endl;
        ILOG << "The entire operation took " << timer.getElapsedMicroseconds() << "us";
        break;
    }
    }
}

void FloatingPointOps::isNanOrInf(std::string value, int expBits, int mantissaBits)
{
    if (value.empty())
    {
        return;
    }

    bool isNegative = false;
    if (value[0] == '-' || value[0] == '+')
    {
        if (value[0] == '-')
        {
            isNegative = true;
        }
        value = value.substr(1);
        if (value.empty())
        {
            return;
        }
    }

    std::stringstream result;
    result << ((isNegative) ? "1 " : "0 ");

    result << std::setfill('1') << std::setw(expBits) << "1";
    result << " ";

    std::transform(value.begin(), value.end(), value.begin(),
                   [](char c) -> char { return std::tolower(c); });

    if (value != "inf")
    {
        result << std::setfill('0') << std::setw(mantissaBits) << "1";
    }
    else
    {
        result << std::setfill('0') << std::setw(mantissaBits) << "0";
    }

    std::cout << "The provided fraction in floating point notation is: " << result.str() << std::endl;
}

bool FloatingPointOps::validateFraction(std::string value)
{
    DLOG << "Validating string " << value << " for fraction value";
    if (value.empty())
    {
        ELOG << "Invalid value provided!";
        return false;
    }

    unsigned minusCount(0), plusCount(0), expCount(0), dotCount(0);
    // Check if all values are decimal
    for (auto ch : value)
    {
        if ((ch < '0' || ch > '9') && ch != '+' && ch != '-' && ch != 'e' && ch != '.')
        {
            ELOG << "Invalid character found: " << ch;
            return false;
        }

        if (ch == '+')
        {
            ++plusCount;
        }
        else if (ch == '-')
        {
            ++minusCount;
        }
        else if (ch == 'e')
        {
            ++expCount;
        }
        else if (ch == '.')
        {
            ++dotCount;
        }
    }

    // Max 2 occurrences of + and -
    size_t signCount = minusCount + plusCount;
    if (signCount > 2)
    {
        ELOG << "The number is not in a valid format, too many sign bits specified";
        return false;
    }

    // Only 0 or 1 "." can be present
    if (dotCount > 1)
    {
        ELOG << "Only one decimal point can be present";
        return false;
    }

    if (expCount > 1)
    {
        ELOG << "Too many occurances of 'e'";
        return false;
    }

    size_t ePos = value.find('e');
    if (1 == signCount)
    {
        // Only valid positions are at the beginning, or immediately after e
        if (value[0] != '-' && value[1] != '+')
        {
            if (!expCount)
            {
                ELOG << "The sign can only appear at the beginning of the number";
                return false;
            }
            else
            {
                if (value[ePos + 1] != '-' && value[ePos + 1] != '+')
                {
                    ELOG << "The sign can only appear at the beginning of the number";
                    return false;
                }
            }
        }
    }
    else if (2 == signCount)
    {
        // Sign has to be in both places
        if (!expCount)
        {
            ELOG << "Too many sign bits specified!";
            return false;
        }

        if (value[0] != '-' && value[0] != '+')
        {
            ELOG << "The sign can only appear at the beginning of the number";
            return false;
        }

        if (value[ePos + 1] != '-' && value[ePos + 1] != '+')
        {
            ELOG << "The sign can only appear at the beginning of the number";
            return false;
        }
    }

    if (expCount)
    {
        DLOG << "The value contains an e";
        // Ensure valid construction
        if (ePos == 0 || ePos == value.length() - 1)
        {
            ELOG << "'e' can appear only in the format XeY";
            return false;
        }

        // . cannot appear before e
        size_t dotPos = value.find('.');
        if (std::string::npos != dotPos)
        {
            if (dotPos > ePos)
            {
                ELOG << "The '.' cannot appear after 'e'";
                return false;
            }
            DLOG << "The value of the fraction is " << value.substr(0, ePos)
                 << ", exponent = " << value.substr(ePos + 1);
        }
    }

    DLOG << "The fraction " << value << " is valid";
    return true;
}

std::string FloatingPointOps::toInfinity(bool isNegative, int exponentBits, int mantissaBits)
{
    std::stringstream result;
    result << (isNegative ? "1 " : "0 ");

    result << std::setfill('1') << std::setw(exponentBits) << "1";
    result << " " << std::setfill('0') << std::setw(mantissaBits) << "0";

    return result.str();
}

std::string toZero(bool isNegative, int exponentBits, int mantissaBits)
{
    std::stringstream result;
    result << (isNegative ? "1 " : "0 ");

    result << std::setfill('0') << std::setw(exponentBits) << "0";
    result << " " << std::setfill('0') << std::setw(mantissaBits) << "0";

    return result.str();
}

std::string FloatingPointOps::convertToIeee(std::string value, int exponentBits, int mantissaBits)
{
    ILOG << "Converting to IEEE [" << value << "] with exponent ["
         << exponentBits << "], Mantissa [" << mantissaBits << "]";

    // Check for NaN, +INF and -INF

    int bias = (1 << (exponentBits - 1)) - 1;
    DLOG << "The bias is calculated to be " << bias;
    size_t sizeOfResult = 1 + exponentBits + mantissaBits;
    DLOG << "Converting to binary string of size " << sizeOfResult << " bits";

    int maxExponentValue = (1 << (exponentBits)) - bias - 2;
    int minExponentValue = 1 - bias;
    DLOG << "Max possible exponent value is " << maxExponentValue;

    int exp = 0;
    size_t ePos = value.find('e');
    if (std::string::npos != ePos)
    {
        DLOG << "Found exponent at " << ePos;
        std::string expValue = value.substr(ePos + 1);
        exp = std::stoi(expValue);
        value = value.substr(0, ePos);
        DLOG << "After removing exponent, value is now " << value;
    }

    bool negativeValue = false;
    DLOG << "Exponent value in decimal: " << exp;
    exp = (exp * log10base2);
    DLOG << "Exponent value in binary is: " << exp;

    if (value[0] == '+' || value[0] == '-')
    {
        if (value[0] == '-')
        {
            negativeValue = true;
        }
        value = value.substr(1);
    }

    if (value.empty())
    {
        ELOG << "Invalid number entered!";
        return "";
    }

    std::string integer(value), fraction("0");
    size_t dotPos = value.find('.');
    if (std::string::npos != dotPos)
    {
        DLOG << "Found decimal point in the string";
        integer = value.substr(0, dotPos);
        fraction = value.substr(dotPos + 1);
        if (integer.empty())
        {
            integer = "0";
        }
    }
    DLOG << "The integer part is: " << integer << ", fraction is " << fraction;

    NumberRepOps numberRepOps;
    long intVal = 0;
    try
    {
        intVal = std::stol(integer);
    }
    catch (...)
    {
        ELOG << "Failed to covnert number " << integer << " to an integer, the number must be infinity";
        return toInfinity(negativeValue, exponentBits, mantissaBits);
    }
    // convertToBinary returns a signed representation, omit the sign bit
    std::string intValue = numberRepOps.convertToBinary(intVal);
    if (intValue.length() > 1)
    {
        intValue = intValue.substr(1);
    }
    ILOG << "The integer value for the number is " << intValue;
    std::string fractValue = numberRepOps.convertToBinaryFraction(fraction, mantissaBits);
    ILOG << "The fractional value for the number, in binary is " << fractValue;

    DLOG << "The un-normalized number number in binary is " << intValue << "." << fractValue;
    while (intValue.length() > 1)
    {
        fractValue.insert(fractValue.begin(), intValue.back());
        intValue.pop_back();
        // Truncate extra bits
        fractValue.pop_back();
        ++exp;
    }

    if (std::string::npos != fractValue.find('1'))
    {
        while (intValue[0] != '1' && exp > minExponentValue)
        {
            intValue.replace(0, 1, std::string(1, fractValue.front()));
            fractValue.erase(fractValue.begin());
            fractValue = fractValue.append("0");
            --exp;
        }
    }

    ILOG << "The normalized binary string is " << intValue << "." << fractValue << ", exponent: " << exp;

    std::stringstream result;
    // First, the sign bit
    result << ((negativeValue) ? "1 " : "0 ");
    DLOG << "The sign bit is " << result.str();

    // The exponent bits
    if (exp > maxExponentValue)
    {
        // Value is infinity, set exp to all ones
        result << std::setw(exponentBits) << std::setfill('1') << "1";
        DLOG << "Exponent is all 1s!";
        return toInfinity(negativeValue, exponentBits, mantissaBits);
    }
    else if (exp < minExponentValue)
    {
        result << std::setw(exponentBits) << std::setfill('0') << "0";
        DLOG << "Exponent is all 0s!";
        return toZero(negativeValue, exponentBits, mantissaBits);
    }
    {
        exp = (intValue == "0") ? 0 : exp + bias;
        result << std::setfill('0') << std::setw(exponentBits) << numberRepOps.convertToBinary(exp).substr(1);
        DLOG << "The exponent value is " << exp;
    }
    result << " ";

    // The last is the mantissa
    result << fractValue;
    DLOG << "Final value is " << result.str();

    return result.str();
}

std::string FloatingPointOps::singlePrecisionToIeee(std::string value)
{
    return convertToIeee(value, 8, 23);
}

std::string FloatingPointOps::doublePrecisionToIeee(std::string value)
{
    return convertToIeee(value, 11, 52);
}

void FloatingPointOps::IeeeToFraction()
{
    std::cout << "Select precision:" << std::endl;
    std::cout << "1 Single\t2 Double: ";

    int choice = -1;
    std::cin >> choice;
    DLOG << "Choice selected: " << choice;

    if (std::cin.fail() || (choice != 1 && choice != 2))
    {
        ELOG << "Invalid choice, returning";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    switch (choice)
    {
    case 1:
    {
        std::cout << "Enter the IEEE754 representation in binary (32 bits): ";
        std::string value;

        std::cin >> value;
        if (std::cin.fail())
        {
            ELOG << "Invalid number format, returning";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return;
        }
        DLOG << "Value entered is " << value;

        Timer timer;
        float result = ieeeToSinglePrecision(value);
        std::cout << "The single precision value of " << value << " is [" << result << "]" << std::endl;
        ILOG << "The entire operation took " << timer.getElapsedMicroseconds() << "us";
    }
    break;
    case 2:
    {
        std::cout << "Enter the IEEE754 representation in binary (64 bits): ";
        std::string value;

        std::cin >> value;
        if (std::cin.fail())
        {
            ELOG << "Invalid number format, returning";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return;
        }

        DLOG << "Value entered is " << value;

        Timer timer;
        double result = ieeeToDoublePrecision(value);

        std::cout << "The double precision value of " << value << " is [" << result << "]" << std::endl;
        ILOG << "The entire operation took " << timer.getElapsedMicroseconds() << "us";
        break;
    }
    }
}

bool FloatingPointOps::validateBinary(std::string value, unsigned len)
{
    if (value.length() != len)
    {
        ELOG << "Expected the string to be of length: " << len;
        return false;
    }

    if (value.find_first_not_of("01") != std::string::npos)
    {
        ELOG << "There are invalid characcters in the string";
        return false;
    }

    DLOG << "The value " << value << " is valid";
    return true;
}

std::string FloatingPointOps::convertIeeeToFraction(std::string value, unsigned exponentBits, unsigned mantissaBits)
{
    std::stringstream result;
    bool isNegative = false;
    unsigned maxExponentValue = (1 << exponentBits) - 1;
    DLOG << "The max possible exponent value is " << maxExponentValue;

    if (value[0] == '1')
    {
        DLOG << "The provided number is negative";
        isNegative = true;
    }
    value = value.substr(1);

    // Exponent bits
    std::string exponent = value.substr(0, exponentBits), mantissa = value.substr(exponentBits);
    DLOG << "The exponent bits are " << exponent << ", the mantissa is " << mantissa;

    // Convert the exponent bits to a number
    unsigned expValue = std::stoi(exponent, 0, 2);
    DLOG << "Converted the exponent to base 10: " << expValue;

    // Special values
    if (maxExponentValue == expValue)
    {
        // The number is +- INF, or NAN
        if (std::string::npos == mantissa.find_first_not_of("0"))
        {
            DLOG << "Mantissa contains all 0s, the value is INFINITY";
            // Value is INF
            double value = (isNegative ? -INFINITY : INFINITY);
            result << value;
        }
        else
        {
            DLOG << "The mantissa contains non-zero, the value is NAN";
            result << NAN;
        }
    }
    else
    {
        int bias = (1 << (exponentBits - 1)) - 1;
        DLOG << "Calculated the exponent bias as " << bias;
        int addendum = 1;

        int binaryExp = 0;
        if (0 != expValue)
        {
            DLOG << "The exponent is non-0";
            binaryExp = expValue - bias;
        }
        else
        {
            DLOG << "The exponent is 0";
            binaryExp = 1 - bias;
            addendum = 0;
        }

        // Calculate the value of the fraction
        double fp = 0.0;

        for (unsigned i = 0; i < mantissa.length(); ++i)
        {
            double tmp = ((mantissa[i] == '0') ? 0.0 : ((1.0 / ((unsigned long)1 << (i+1)))));
            DLOG << "Calculated value at i = " << i << " to be " << tmp;
            fp += tmp;
            DLOG << "The fp value is now " << fp;
        }

        DLOG << "The fraction value is " << fp;
        fp += addendum;

        ILOG << "The value of [" << value << "] is [" << fp << "x 2^" << binaryExp << "]";

        fp = fp * pow(2.0, binaryExp);
        fp = (isNegative) ? -fp : fp;
        ILOG << "The final value of the fraction is " << fp;
        result << fp;
    }
    return result.str();
}

float FloatingPointOps::ieeeToSinglePrecision(std::string value)
{
    if (!validateBinary(value, 32))
    {
        DLOG << "The value is invalid, returning NaN";
        return NAN;
    }

    return std::stof(convertIeeeToFraction(value, 8, 23));
}

double FloatingPointOps::ieeeToDoublePrecision(std::string value)
{
    if (!validateBinary(value, 64))
    {
        DLOG << "The value is invalid, returning NaN";
        return NAN;
    }

    return std::stod(convertIeeeToFraction(value, 11, 52));
}
