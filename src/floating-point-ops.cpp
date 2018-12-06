#include "floating-point-ops.h"
#include "timer.h"
#include "logger.h"
#include <iostream>
#include <algorithm>

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
        ILOG << "The entire operation took [" << timer.getElapsedMicroseconds() << "us";
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
        ILOG << "The entire operation took [" << timer.getElapsedMicroseconds() << "us";
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

    // Check if all values are decimal
    for (auto ch : value)
    {
        if ((ch < '0' || ch > '9') && ch != '+' && ch != '-' && ch != 'e' && ch != '.')
        {
            ELOG << "Invalid character found: " << ch;
            return false;
        }
    }

    // Only 0 or 1 "." can be present
    auto count = std::count(value.begin(), value.end(), '.');
    if (count > 1)
    {
        ELOG << "Only one decimal point can be present";
        return false;
    }

    // At max 1 e
    size_t countE = std::count(value.begin(), value.end(), 'e');
    if (countE > 1)
    {
        ELOG << "Only one 'e' can be present";
        return false;
    }

    if (1 == countE)
    {
        DLOG << "The value contains an e";
        // Ensure valid construction
        size_t pos = value.find('e');
        if (pos == 0 || pos == value.length() - 1)
        {
            ELOG << "'e' can appear only in the format XeY";
            return false;
        }

        // . cannot appear before e
        size_t dotPos = value.find('.');
        if (std::string::npos != dotPos)
        {
            if (dotPos > pos)
            {
                ELOG << "The '.' cannot appear after 'e'";
                return false;
            }
            DLOG << "The value of the fraction is " << value.substr(0, pos)
                 << ", exponent = " << value.substr(pos + 1);
        }
    }

    DLOG << "The fraction " << value << " is valid";
    return true;
}

std::string convertToIeee(std::string value, int exponentBits, int mantissaBits)
{
    ILOG << "Converting to IEEE [" << value << "] with exponent ["
         << exponentBits << "], Mantissa [" << mantissaBits << "]";
    int bias = (2 << (exponentBits - 1)) - 1;
    DLOG << "The bias is calculated to be " << bias;
    size_t sizeOfResult = 1 + exponentBits + mantissaBits;
    DLOG << "Converting to binary string of size " << sizeOfResult << " bits";

    std::string result;

    return result;
}

std::string FloatingPointOps::singlePrecisionToIeee(std::string value)
{
    std::string result;

    // Find exponent if exists
    //size_t pos = value.pos()
    return result;
}

std::string FloatingPointOps::doublePrecisionToIeee(std::string value)
{
    std::string result;
    return result;
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
