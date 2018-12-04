#include "matrix-operations.h"
#include <iostream>
#include "timer.h"
#include <random>

MatrixOperations::MatrixOperations() : matrixType(NA),
                                       intMatrix(NULL),
                                       longMatrix(NULL),
                                       floatMatrix(NULL),
                                       doubleMatrix(NULL)
{
}

MatrixOperations::~MatrixOperations()
{
    FreeMatrix();
}

void MatrixOperations::FreeMatrix()
{
    DLOG << "Deleting matrix of type " << matrixType;
    if (intMatrix)
    {
        delete intMatrix;
        intMatrix = NULL;
    }

    if (longMatrix)
    {
        delete longMatrix;
        longMatrix = NULL;
    }

    if (floatMatrix)
    {
        delete floatMatrix;
        floatMatrix = NULL;
    }

    if (doubleMatrix)
    {
        delete doubleMatrix;
        doubleMatrix = NULL;
    }

    this->matrixType = NA;
}

void MatrixOperations::CreateMatrix()
{
    int rows = -1, columns = -1, matrixType = -1, accessType = ROW_MAJOR;
    std::cout << "Creating Matrix" << std::endl;
    std::cout << "Number of Rows (1 - " << MAX_ROW_SIZE << "): ";
    std::cin >> rows;

    if (std::cin.fail() || rows <= 0 || rows > MAX_ROW_SIZE)
    {
        ELOG << "Invalid number of rows input: " << rows << ", stopping";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
    DLOG << "Got number of rows = " << rows;

    std::cout << "Number of Columns (1 - " << MAX_COL_SIZE << "): ";
    std::cin >> columns;

    if (std::cin.fail() || columns <= 0 || columns > MAX_COL_SIZE)
    {
        ELOG << "Invalid number of columns input: " << columns << ", stopping";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
    DLOG << "Got number of columns = " << columns;

    std::cout << "Select Type of Matrix:" << std::endl
              << "1 Int\t2 Long\t3 Float\t4 Double: ";
    std::cin >> matrixType;

    if (std::cin.fail() || matrixType < 0 || matrixType > 4)
    {
        ELOG << "Invalid Matrix type selected, stopping";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
    DLOG << "Got Matrix Type = " << matrixType;

    /*std::cout << "Enter Access type: 1 Row Major\t2 Column Major: ";
    std::cin >> accessType;
    if (std::cin.fail() || (accessType != 1 && accessType != 2))
    {
        ELOG << "Invalid Access Type specified, stopping";std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
    DLOG << "Setting accesstype to " << accessType;
    */

    if (this->matrixType != NA)
    {
        ILOG << "Matrix of type [" << this->matrixType << "] already exists, it will be deleted";
        FreeMatrix();
    }

    this->matrixType = MatrixType(matrixType);
    Timer timer;
    switch (this->matrixType)
    {
    case INT:
        intMatrix = new IntMatrix(unsigned(rows), unsigned(columns), AccessType(accessType));
        break;
    case LONG:
        longMatrix = new LongMatrix(unsigned(rows), unsigned(columns), AccessType(accessType));
        break;
    case FLOAT:
        floatMatrix = new FloatMatrix(unsigned(rows), unsigned(columns), AccessType(accessType));
        break;
    case DOUBLE:
        doubleMatrix = new DoubleMatrix(unsigned(rows), unsigned(columns), AccessType(accessType));
        break;
    case NA:
        ELOG << "Invalid state!";
        return;
        break;
    }
    std::cout << "Creating matrix completed in " << timer.getElapsedMilliseconds() << "ms" << std::endl;

    ILOG << "Created a matrix of type [" << MatrixType(matrixType) << "], with size ["
         << rows << " x " << columns << "], access type [" << AccessType(accessType) << "]";
}

void MatrixOperations::ReadMatrix()
{
    std::cout << "Attempting to read in matrix of type " << matrixType << std::endl;
    if (NA == matrixType)
    {
        ELOG << "The matrix has not been initialized yet, call CreateMatrix first";
        return;
    }

    std::cout
        << "1 Enter elements manually\t2 Enter random elements" << std::endl;
    int random = -1;
    std::cin >> random;

    if (std::cin.fail())
    {
        ELOG << "Invalid choice entered, stopping";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    if (random != 1 && random != 2)
    {
        ELOG << "Invalid choice entered, stopping";
        return;
    }

    bool manualInput = (1 == random);
    switch (matrixType)
    {
    case INT:
        readIntMatrix(manualInput);
        break;
    case LONG:
        readLongMatrix(manualInput);
        break;
    case FLOAT:
        readFloatMatrix(manualInput);
        break;
    case DOUBLE:
        readDoubleMatrix(manualInput);
        break;
    case NA:
        ELOG << "Invalid state!";
        return;
        break;
    }
}

bool recheckManual(bool manual, unsigned rows, unsigned cols)
{
    if (manual && rows * cols > 16)
    {
        std::cout << "Are you sure you want to input [" << rows * cols
                  << "] elements manually? (Y/[N]): ";
        char yes = 'N';
        std::cin >> yes;

        if (!('Y' == yes) && !('y' == yes))
        {
            manual = false;
        }
    }
    return manual;
}

void MatrixOperations::readIntMatrix(bool manual)
{
    unsigned rows = intMatrix->getRows(), cols = intMatrix->getCols();
    manual = recheckManual(manual, rows, cols);
    Timer timer;
    if (manual)
    {
        ILOG << "Reading " << rows * cols << " elements from the user manually";
        for (unsigned i = 0; i < rows; ++i)
        {
            for (unsigned j = 0; j < cols; ++j)
            {
                std::cout << "Value [" << i << ", " << j << "]: ";
                int value = 0;
                std::cin >> value;

                if (std::cin.fail())
                {
                    ELOG << "Invalid integer entered, assuming 0";
                    value = 0;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }

                DLOG << "Read value " << value;
                intMatrix->set(i, j, value);
            }
        }
    }
    else
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution<int> dist;
        ILOG << "Reading " << rows * cols << " elements randomly";
        for (unsigned i = 0; i < rows; ++i)
        {
            for (unsigned j = 0; j < cols; ++j)
            {
                int value = dist(mt);
                DLOG << "Putting value " << value;
                intMatrix->set(i, j, value);
            }
        }
    }
    std::cout << "Total time to read the matrix: " << timer.getElapsedMilliseconds() << "ms" << std::endl;
}

void MatrixOperations::readLongMatrix(bool manual)
{
    unsigned rows = longMatrix->getRows(), cols = longMatrix->getCols();
    manual = recheckManual(manual, rows, cols);
    Timer timer;
    if (manual)
    {
        ILOG << "Reading " << rows * cols << " elements from the user manually";
        for (unsigned i = 0; i < rows; ++i)
        {
            for (unsigned j = 0; j < cols; ++j)
            {
                std::cout << "Value [" << i << ", " << j << "]: ";
                long value = 0;
                std::cin >> value;

                if (std::cin.fail())
                {
                    ELOG << "Invalid long entered, assuming 0";
                    value = 0;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }

                DLOG << "Read value " << value;
                longMatrix->set(i, j, value);
            }
        }
    }
    else
    {
        std::random_device rd;
        std::mt19937_64 mt(rd());
        std::uniform_int_distribution<long> dist;
        ILOG << "Reading " << rows * cols << " elements randomly";
        for (unsigned i = 0; i < rows; ++i)
        {
            for (unsigned j = 0; j < cols; ++j)
            {
                long value = dist(mt);
                DLOG << "Putting value " << value;
                longMatrix->set(i, j, value);
            }
        }
    }
    std::cout << "Total time to read the matrix: " << timer.getElapsedMilliseconds() << "ms" << std::endl;
}

void MatrixOperations::readFloatMatrix(bool manual)
{
    unsigned rows = floatMatrix->getRows(), cols = floatMatrix->getCols();
    manual = recheckManual(manual, rows, cols);
    Timer timer;
    if (manual)
    {
        ILOG << "Reading " << rows * cols << " elements from the user manually";
        for (unsigned i = 0; i < rows; ++i)
        {
            for (unsigned j = 0; j < cols; ++j)
            {
                std::cout << "Value [" << i << ", " << j << "]: ";
                float value = 0.0f;
                std::cin >> value;

                if (std::cin.fail())
                {
                    ELOG << "Invalid long entered, assuming 0";
                    value = 0;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }

                DLOG << "Read value " << value;
                floatMatrix->set(i, j, value);
            }
        }
    }
    else
    {
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_real_distribution<float> dist;
        ILOG << "Reading " << rows * cols << " elements randomly";
        for (unsigned i = 0; i < rows; ++i)
        {
            for (unsigned j = 0; j < cols; ++j)
            {
                float value = dist(mt);
                DLOG << "Putting value " << value;
                floatMatrix->set(i, j, value);
            }
        }
    }
    std::cout << "Total time to read the matrix: " << timer.getElapsedMilliseconds() << "ms" << std::endl;
}

void MatrixOperations::readDoubleMatrix(bool manual)
{
    unsigned rows = doubleMatrix->getRows(), cols = doubleMatrix->getCols();
    manual = recheckManual(manual, rows, cols);
    Timer timer;
    if (manual)
    {
        ILOG << "Reading " << rows * cols << " elements from the user manually";
        for (unsigned i = 0; i < rows; ++i)
        {
            for (unsigned j = 0; j < cols; ++j)
            {
                std::cout << "Value [" << i << ", " << j << "]: ";
                double value = 0.0;
                std::cin >> value;

                if (std::cin.fail())
                {
                    ELOG << "Invalid long entered, assuming 0";
                    value = 0.0;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }

                DLOG << "Read value " << value;
                doubleMatrix->set(i, j, value);
            }
        }
    }
    else
    {
        std::random_device rd;
        std::mt19937_64 mt(rd());
        std::uniform_real_distribution<double> dist;
        ILOG << "Reading " << rows * cols << " elements randomly";
        for (unsigned i = 0; i < rows; ++i)
        {
            for (unsigned j = 0; j < cols; ++j)
            {
                double value = dist(mt);
                DLOG << "Putting value " << value;
                doubleMatrix->set(i, j, value);
            }
        }
    }
    std::cout << "Total time to read the matrix: " << timer.getElapsedMilliseconds() << "ms" << std::endl;
}

void MatrixOperations::ShowMatrix()
{
    if (NA == matrixType)
    {
        ELOG << "The matrix has not been initialized yet, run CreateMatrix first";
        return;
    }

    int row = -1, col = -1;
    std::cout << "Enter start Row value [0]: ";
    std::cin >> row;

    if (std::cin.fail() || row < 0 || row > MAX_ROW_SIZE)
    {
        ELOG << "Invalid value entered for row, assuming 0";
        row = 0;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::cout << "Enter start Column value [0]: ";
    std::cin >> col;

    if (std::cin.fail() || col < 0 || col > MAX_COL_SIZE)
    {
        ELOG << "Invalid value entered for row, assuming 0";
        col = 0;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    int accessType = -1;
    std::cout << "Enter Access type: 1 Row Major\t2 Column Major: ";
    std::cin >> accessType;
    if (std::cin.fail() || (accessType != 1 && accessType != 2))
    {
        ELOG << "Invalid Access Type specified, stopping";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
    DLOG << "Setting accesstype to " << accessType;

    Timer timer;
    switch (matrixType)
    {
    case NA:
        break;
    case INT:
        std::cout << std::endl
                  << "Displaying matrix from row [" << row
                  << "], column [" << col << "] in order: " << AccessType(accessType)
                  << std::endl;
        this->intMatrix->display(unsigned(row), unsigned(col), AccessType(accessType));
        break;
    case LONG:
        std::cout << std::endl
                  << "Displaying matrix from row [" << row
                  << "], column [" << col << "] in order: " << AccessType(accessType)
                  << std::endl
                  << std::endl;
        this->longMatrix->display(unsigned(row), unsigned(col), AccessType(accessType));
        break;
    case FLOAT:
        std::cout << std::endl
                  << "Displaying matrix from row [" << row
                  << "], column [" << col << "] in order: " << AccessType(accessType)
                  << std::endl
                  << std::endl;
        this->floatMatrix->display(unsigned(row), unsigned(col), AccessType(accessType));
        break;
    case DOUBLE:
        std::cout << std::endl
                  << "Displaying matrix from row [" << row
                  << "], column [" << col << "] in order: " << AccessType(accessType)
                  << std::endl
                  << std::endl;
        this->doubleMatrix->display(unsigned(row), unsigned(col), AccessType(accessType));
        break;
    }
    std::cout << std::endl
              << "Total time to display matrix: "
              << timer.getElapsedMilliseconds() << "ms" << std::endl;
}