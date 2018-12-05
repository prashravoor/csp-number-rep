#include <iostream>
#include <cmath>
#include "logger.h"
#include "matrix-operations.h"
#include <limits>
#include "number-rep-ops.h"

void matrixOperations();

void numRepOperations();

void floatingPointOperations();

void bannerify(const char *text);

int main()
{
    Logger::init();

    bannerify("Starting Up... CSP Assignment #1");

    while (true)
    {
        std::cout << "Select Operation Category:" << std::endl;
        std::cout << "1 Matrix Operations\t2 Number Representation Operations\t"
                     "3 Floating Point Operations\t4 Exit"
                  << std::endl;

        int type = -1;
        std::cin >> type;
        if (std::cin.fail())
        {
            std::cout << "Invalid category, try again" << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (type)
        {
        case 1:
            matrixOperations();
            break;
        case 2:
            numRepOperations();
            break;
        case 3:
            floatingPointOperations();
            break;
        case 4:
            std::cout << "Exiting" << std::endl;
            return 0;
        default:
            ELOG << "Invalid category, try again";
        }
    }
}

void bannerify(const char *text)
{
    std::cout << std::endl
              << "******************** " << text << " ***********************" << std::endl;
}

void matrixOperations()
{
    DLOG << "Beginning Matrix Operations";
    bannerify("Matrix Operations");
    MatrixOperations matrixOperations;

    while (true)
    {
        std::cout << "Select the type of operation:" << std::endl;
        std::cout << "1 CreateMatrix\t2 ReadMatrix\t3 ShowMatrix\t4 FreeMatrix\t5 Previous Menu" << std::endl;

        int choice = -1;
        std::cin >> choice;

        if (std::cin.fail())
        {
            ELOG << "Invalid choice, try again";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice)
        {
        case 1:
            matrixOperations.CreateMatrix();
            break;
        case 2:
            matrixOperations.ReadMatrix();
            break;
        case 3:
            matrixOperations.ShowMatrix();
            break;
        case 4:
            matrixOperations.FreeMatrix();
            break;
        case 5:
            std::cout << "Returning from Matrix Operations" << std::endl;
            return;
        default:
            ELOG << "Invalid choice, try again";
            break;
        }
    }
}

void numRepOperations()
{
    bannerify("Number Representation Operations");
    NumberRepOps numberRepOps;
    while (true)
    {
        std::cout << "Select the type of operation:" << std::endl;
        std::cout << "1 BinToInt\t2 IntToBin\t3 Previous Menu" << std::endl;

        int choice = -1;
        std::cin >> choice;

        if (std::cin.fail())
        {
            ELOG << "Invalid choice, try again";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice)
        {
        case 1:
            numberRepOps.binToInt();
            break;
        case 2:
            numberRepOps.intToBin();
            break;
        case 3:
            return;
        default:
            ELOG << "Invalid choice, try again";
            break;
        }
    }
}

void floatingPointOperations()
{
}
