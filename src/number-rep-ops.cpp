#include "number-rep-ops.h"
#include <string>
#include <iostream>
#include "logger.h"
#include "timer.h"
#include <limits>

#define MAX_LENGTH 64

void NumberRepOps::binToInt()
{
    std::string binaryNumber;
    std::cout << "Enter a number in 2's Complement Binary notation (minimum 2 bits): ";
    std::cin >> binaryNumber;

    if (std::cin.fail())
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        ELOG << "Invalid input entered, returning";
        return;
    }

    if (binaryNumber.length() < 2)
    {
        ELOG << "Invalid number entered, returning";
        return;
    }

    for (auto ch : binaryNumber)
    {
        if (ch != '0' && ch != '1')
        {
            ELOG << "Invalid character '" << ch << "', returning";
            return;
        }
    }

    DLOG << "The length of the number is " << binaryNumber.length();
    DLOG << "The number read is: " << binaryNumber;
    if (binaryNumber.length() > MAX_LENGTH)
    {
        ELOG << "The entered string is too long, truncating to " << MAX_LENGTH << " bits";
        binaryNumber = binaryNumber.substr(0, MAX_LENGTH);
    }
    else
    {
        DLOG << "Padding the string with " << (MAX_LENGTH - binaryNumber.length())
             << " " << binaryNumber[0] << "s";
        while (MAX_LENGTH > binaryNumber.length())
        {
            binaryNumber.insert(binaryNumber.begin(), binaryNumber[0]);
        }
    }

    Timer timer;
    // Bit 1 is negative
    long negativeValue = ((long)(binaryNumber[0] - '0') << (MAX_LENGTH - 1));
    DLOG << "Initial negative value: " << -negativeValue;
    long result = -negativeValue;

    for (unsigned i = 1; i < MAX_LENGTH; ++i)
    {
        DLOG << "Shifting " << binaryNumber[i] << " by " << MAX_LENGTH - i << " bits";
        long tmp = ((long)(binaryNumber[i] - '0') << (MAX_LENGTH - i -1));
        result += tmp;
        DLOG << "At [" << i << "], , adding value [" << tmp << "], result [" << result << "]";
    }

    std::cout << "The decimal representation for the number is [" << result << "]" << std::endl;
    std::cout << "The time taken to convert from binary to integer is "
              << timer.getElapsedMicroseconds() << "us" << std::endl;
}

void NumberRepOps::intToBin()
{
}