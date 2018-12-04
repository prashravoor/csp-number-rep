#include <iostream>
#include <cmath>
#include "logger.h"
#include "matrix-operations.h"

void matrixOperations();

void numRepOperations();

void floatingPointOperations();

int main()
{
    Logger::init();

    std::cout << "******** Starting Up **************" << std::endl;

    while (true)
    {
        std::cout << "Select Operation Category:" << std::endl;
        std::cout << "1 Matrix Operations\t2 Number Representation Operations\t"
                     "3 Floating Point Operations\t4 Exit"
                  << std::endl;

        int type = -1;
        std::cin >> type;
        if (std::cin.bad())
        {
            std::cout << "Invalid category, try again" << std::endl;
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

void matrixOperations()
{
    DLOG << "Beginning Matrix Operations";
    MatrixOperations matrixOperations;

    while (true)
    {
        std::cout << "Select the type of operation:" << std::endl;
        std::cout << "1 CreateMatrix\t2 ReadMatrix\t3 ShowMatrix\t4FreeMatrix\t5 Previous Menu" << std::endl;

        int choice = -1;
        std::cin >> choice;

        if (std::cin.bad())
        {
            ELOG << "Invalid choice, try again";
            continue;
        }

        switch (choice)
        {
        case 1:
            matrixOperations.CreateMatrix();
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
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
}

void floatingPointOperations()
{
}
