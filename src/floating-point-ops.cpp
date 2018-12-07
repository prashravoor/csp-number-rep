#include "floating-point-ops.h"
#include "timer.h"
#include "logger.h"
#include <iostream>
#include <algorithm>
#include "number-rep-ops.h"
#include <sstream>
#include <iomanip>

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
            if (expCount)
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

std::string convertToIeee(std::string value, int exponentBits, int mantissaBits)
{
    ILOG << "Converting to IEEE [" << value << "] with exponent ["
         << exponentBits << "], Mantissa [" << mantissaBits << "]";
    int bias = (1 << (exponentBits - 1)) - 1;
    DLOG << "The bias is calculated to be " << bias;
    size_t sizeOfResult = 1 + exponentBits + mantissaBits;
    DLOG << "Converting to binary string of size " << sizeOfResult << " bits";

    int maxExponentValue = (1 << (exponentBits)) - bias - 2;
    DLOG << "Max possible exponent value is " << maxExponentValue;

    int exp = 0;
    size_t ePos = value.find('e');
    // Normalize first
    if (std::string::npos != ePos)
    {
        DLOG << "Found exponent at " << ePos;
        std::string expValue = value.substr(ePos + 1);
        exp = std::stoi(expValue);
        value = value.substr(0, ePos);
        DLOG << "After removing exponent, value is now " << value;
    }

    bool negativeValue = false;
    DLOG << "Exponent value: " << exp;
    if (value[0] == '+' || value[0] == '-')
    {
        if (value[0] == '-')
        {
            negativeValue = true;
        }
        value = value.substr(1);
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
    long intVal = std::stol(integer);
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
        ++exp;
    }

    ILOG << "The normalized binary string is " << intValue << "." << fractValue << ", exponent: " << exp;
    // Truncate it back to max number of mantissa bits
    if (fractValue.length() > unsigned(mantissaBits))
    {
        fractValue = fractValue.substr(0, mantissaBits);
    }

    std::stringstream result;
    // First, the sign bit
    result << ((negativeValue) ? "1 " : "0 ");
    DLOG << "The sign bit is " << result.str();

    // The exponent bits
    if (exp > maxExponentValue)
    {
        // Value is infinity, set exp to all ones
        for (int i = 0; i < exponentBits; ++i)
        {
            result << "1";
        }
        DLOG << "Exponent is all 1s!";
    }
    else
    {
        exp = exp + bias;
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
        ILOG << "The entire operation took [" << timer.getElapsedMicroseconds() << "us";
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
        ILOG << "The entire operation took [" << timer.getElapsedMicroseconds() << "us";
        break;
    }
    }
}

float FloatingPointOps::ieeeToSinglePrecision(std::string value)
{
    float result = 0.0f;
    return result;
}

double FloatingPointOps::ieeeToDoublePrecision(std::string value)
{
    double result = 0.0;
    return result;
}
