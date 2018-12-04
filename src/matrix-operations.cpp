#include "matrix-operations.h"
#include <iostream>

MatrixOperations::MatrixOperations() : matrixType(INT),
                                       intMatrix(NULL),
                                       longMatrix(NULL),
                                       floatMatrix(NULL),
                                       doubleMatrix(NULL)
{
}

MatrixOperations::~MatrixOperations()
{
    if (intMatrix)
    {
        delete intMatrix;
    }

    if (longMatrix)
    {
        delete longMatrix;
    }

    if (floatMatrix)
    {
        delete floatMatrix;
    }

    if (doubleMatrix)
    {
        delete doubleMatrix;
    }
}

void MatrixOperations::CreateMatrix()
{
    int rows = -1, columns = -1, matrixType = -1, accessType = -1;
    std::cout << "Creating Matrix" << std::endl;
    std::cout << "Number of Rows (1 - 4096): ";
    std::cin >> rows;

    if (std::cin.bad() || rows <= 0 || rows > 4096)
    {
        ELOG << "Invalid number of rows input: " << rows << ", stopping";
        return;
    }
    DLOG << "Got number of rows = " << rows;

    std::cout << "Number of Columns (1 - 4096): ";
    std::cin >> columns;

    if (std::cin.bad() || columns <= 0 || columns > 4096)
    {
        ELOG << "Invalid number of columns input: " << columns << ", stopping";
        return;
    }
    DLOG << "Got number of columns = " << columns;

    std::cout << "Select Type of Matrix:" << std::endl
              << "1 Int\t2 Long\t3 Float\t4 Double: ";
    std::cin >> matrixType;

    if (std::cin.bad() || matrixType < 0 || matrixType > 3)
    {
        ELOG << "Invalid Matrix type selected, stopping";
        return;
    }
    DLOG << "Got Matrix Type = " << matrixType;

    std::cout << "Enter Access type: 1 Row Major\t2 Column Major: ";
    std::cin >> accessType;
    if (std::cin.bad() || (accessType != 1 && accessType != 2))
    {
        ELOG << "Invalid Access Type specified, stopping";
        return;
    }
    DLOG << "Setting accesstype to " << accessType;

    ILOG << "Create a matrix of type [" << MatrixType(matrixType) << "], with size ["
         << rows << " x " << columns << "], access type [" << AccessType(accessType) << "]";
}