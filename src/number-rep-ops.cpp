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

    Timer timer;
    long result = convertToInteger(binaryNumber);

    std::cout << "The decimal representation for the number is [" << result << "]" << std::endl;
    std::cout << "The time taken to convert from binary to integer is "
              << timer.getElapsedMicroseconds() << "us" << std::endl;
}

long NumberRepOps::convertToInteger(std::string binaryNumber)
{
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

    // Bit 1 is negative
    long negativeValue = ((long)(binaryNumber[0] - '0') << (MAX_LENGTH - 1));
    DLOG << "Initial negative value: " << -negativeValue;
    long result = -negativeValue;

    for (unsigned i = 1; i < MAX_LENGTH; ++i)
    {
        DLOG << "Shifting " << binaryNumber[i] << " by " << MAX_LENGTH - i << " bits";
        long tmp = ((long)(binaryNumber[i] - '0') << (MAX_LENGTH - i - 1));
        result += tmp;
        DLOG << "At [" << i << "], , adding value [" << tmp << "], result [" << result << "]";
    }

    return result;
}

void NumberRepOps::intToBin()
{
    std::cout << "Enter an integer in the range (" << std::numeric_limits<long>::min()
              << " - " << std::numeric_limits<long>::max() << "): ";

    long value = -1;
    std::cin >> value;
    DLOG << "Read in value " << value;

    if (std::cin.fail())
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        ELOG << "Invalid input entered, returning";
        return;
    }

    Timer timer;
    std::string binaryNumber = convertToBinary(value);

    std::cout << "The equivalent two's complement binary number is: " << binaryNumber << std::endl;

    std::cout << "The time taken to convert from integer to binary is "
              << timer.getElapsedMicroseconds() << "us" << std::endl;
}

std::string NumberRepOps::convertToBinary(long tmp)
{
    bool negative = false;

    if (tmp < 0)
    {
        negative = true;
        tmp = -tmp;
        DLOG << "Setting sign bit to true for a negative number, number is now " << tmp;
    }

    std::string binaryNumber;
    while (tmp > 0)
    {
        binaryNumber.insert(binaryNumber.begin(), '0' + (tmp & 1));
        tmp = tmp >> 1;
        DLOG << "Appending number " << (tmp & 1) << ", tmp is now " << tmp;
    }

    DLOG << "The binary number is " << binaryNumber;
    if (!negative)
    {
        DLOG << "Adding a 0 to the beginning, since it's a positive number";
        binaryNumber.insert(binaryNumber.begin(), '0');
    }
    else
    {
        DLOG << "The number is negative, take 2's complement of the result";
        std::string tempStr;
        for (auto ch : binaryNumber)
        {
            tempStr.push_back((ch == '0') ? '1' : '0');
        }

        binaryNumber = tempStr;
        DLOG << "1s complement of the number is " << binaryNumber;
        for (unsigned i = binaryNumber.length() - 1; i >= 0; --i)
        {
            DLOG << "Bit " << i << " is " << binaryNumber[i];
            if (binaryNumber[i] == '0')
            {
                DLOG << "Setting bit " << i;
                binaryNumber[i] = '1';
                break;
            }
            else
            {
                DLOG << "Resetting bit " << i;
                binaryNumber[i] = '0';
            }
        }

        // Add sign bit
        binaryNumber.insert(binaryNumber.begin(), '1');
    }

    return binaryNumber;
}