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

    int accessType = ROW_MAJOR;
    std::cout << "Enter Access type: 1 Row Major\t2 Column Major: ";
    std::cin >> accessType;
    if (std::cin.fail() || (accessType != 1 && accessType != 2))
    {
        ELOG << "Invalid Access Type specified, assuming ROW_MAJOR";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
    DLOG << "Setting accesstype to " << AccessType(accessType);

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
        readIntMatrix(manualInput, AccessType(accessType));
        break;
    case LONG:
        readLongMatrix(manualInput, AccessType(accessType));
        break;
    case FLOAT:
        readFloatMatrix(manualInput, AccessType(accessType));
        break;
    case DOUBLE:
        readDoubleMatrix(manualInput, AccessType(accessType));
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

void MatrixOperations::readIntMatrix(bool manual, AccessType accessType)
{
    unsigned rows = intMatrix->getRows(), cols = intMatrix->getCols();
    manual = recheckManual(manual, rows, cols);

    if (manual)
    {
        ILOG << "Reading " << rows * cols << " elements from the user manually";
    }
    else
    {
        ILOG << "Reading " << rows * cols << " elements randomly";
    }

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist;
    Timer timer;
    if (ROW_MAJOR == accessType)
    {
        for (unsigned i = 0; i < rows; ++i)
        {
            for (unsigned j = 0; j < cols; ++j)
            {
                int value = 0;
                if (manual)
                {
                    std::cout << "Value [" << i << ", " << j << "]: ";
                    std::cin >> value;

                    if (std::cin.fail())
                    {
                        ELOG << "Invalid integer entered, assuming 0";
                        value = 0;
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                }
                else
                {
                    value = dist(mt);
                }

                DLOG << "Read value " << value;
                intMatrix->set(i, j, value);
            }
        }
    }
    else
    {
        for (unsigned i = 0; i < cols; ++i)
        {
            for (unsigned j = 0; j < rows; ++j)
            {
                int value = 0;
                if (manual)
                {
                    std::cout << "Value [" << j << ", " << i << "]: ";
                    std::cin >> value;

                    if (std::cin.fail())
                    {
                        ELOG << "Invalid integer entered, assuming 0";
                        value = 0;
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                }
                else
                {
                    value = dist(mt);
                }

                DLOG << "Read value " << value;
                intMatrix->set(j, i, value);
            }
        }
    }
    std::cout << "Total time to read the matrix: " << timer.getElapsedMilliseconds() << "ms" << std::endl;
}

void MatrixOperations::readLongMatrix(bool manual, AccessType accessType)
{
    unsigned rows = longMatrix->getRows(), cols = longMatrix->getCols();
    manual = recheckManual(manual, rows, cols);

    if (manual)
    {
        ILOG << "Reading " << rows * cols << " elements from the user manually";
    }
    else
    {
        ILOG << "Reading " << rows * cols << " elements randomly";
    }

    std::random_device rd;
    std::mt19937_64 mt(rd());
    std::uniform_int_distribution<long> dist;
    Timer timer;
    if (ROW_MAJOR == accessType)
    {
        for (unsigned i = 0; i < rows; ++i)
        {
            for (unsigned j = 0; j < cols; ++j)
            {
                long value = 0;
                if (manual)
                {
                    std::cout << "Value [" << i << ", " << j << "]: ";
                    std::cin >> value;

                    if (std::cin.fail())
                    {
                        ELOG << "Invalid integer entered, assuming 0";
                        value = 0;
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                }
                else
                {
                    value = dist(mt);
                }

                DLOG << "Read value " << value;
                longMatrix->set(i, j, value);
            }
        }
    }
    else
    {
        for (unsigned i = 0; i < cols; ++i)
        {
            for (unsigned j = 0; j < rows; ++j)
            {
                long value = 0;
                if (manual)
                {
                    std::cout << "Value [" << j << ", " << i << "]: ";
                    std::cin >> value;

                    if (std::cin.fail())
                    {
                        ELOG << "Invalid integer entered, assuming 0";
                        value = 0;
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                }
                else
                {
                    value = dist(mt);
                }

                DLOG << "Read value " << value;
                longMatrix->set(j, i, value);
            }
        }
    }
    std::cout << "Total time to read the matrix: " << timer.getElapsedMilliseconds() << "ms" << std::endl;
}

void MatrixOperations::readFloatMatrix(bool manual, AccessType accessType)
{
    unsigned rows = floatMatrix->getRows(), cols = floatMatrix->getCols();
    manual = recheckManual(manual, rows, cols);

    if (manual)
    {
        ILOG << "Reading " << rows * cols << " elements from the user manually";
    }
    else
    {
        ILOG << "Reading " << rows * cols << " elements randomly";
    }

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(-100.0, 100.0);
    Timer timer;
    if (ROW_MAJOR == accessType)
    {
        for (unsigned i = 0; i < rows; ++i)
        {
            for (unsigned j = 0; j < cols; ++j)
            {
                float value = 0;
                if (manual)
                {
                    std::cout << "Value [" << i << ", " << j << "]: ";
                    std::cin >> value;

                    if (std::cin.fail())
                    {
                        ELOG << "Invalid integer entered, assuming 0";
                        value = 0;
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                }
                else
                {
                    value = dist(mt);
                }

                DLOG << "Read value " << value;
                floatMatrix->set(i, j, value);
            }
        }
    }
    else
    {
        for (unsigned i = 0; i < cols; ++i)
        {
            for (unsigned j = 0; j < rows; ++j)
            {
                float value = 0;
                if (manual)
                {
                    std::cout << "Value [" << j << ", " << i << "]: ";
                    std::cin >> value;

                    if (std::cin.fail())
                    {
                        ELOG << "Invalid integer entered, assuming 0";
                        value = 0;
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                }
                else
                {
                    value = dist(mt);
                }

                DLOG << "Read value " << value;
                floatMatrix->set(j, i, value);
            }
        }
    }
    std::cout << "Total time to read the matrix: " << timer.getElapsedMilliseconds() << "ms" << std::endl;
}

void MatrixOperations::readDoubleMatrix(bool manual, AccessType accessType)
{
    unsigned rows = doubleMatrix->getRows(), cols = doubleMatrix->getCols();
    manual = recheckManual(manual, rows, cols);

    if (manual)
    {
        ILOG << "Reading " << rows * cols << " elements from the user manually";
    }
    else
    {
        ILOG << "Reading " << rows * cols << " elements randomly";
    }

    std::random_device rd;
    std::mt19937_64 mt(rd());
    std::uniform_real_distribution<double> dist(-100.0, 100.0);
    Timer timer;
    if (ROW_MAJOR == accessType)
    {
        for (unsigned i = 0; i < rows; ++i)
        {
            for (unsigned j = 0; j < cols; ++j)
            {
                double value = 0;
                if (manual)
                {
                    std::cout << "Value [" << i << ", " << j << "]: ";
                    std::cin >> value;

                    if (std::cin.fail())
                    {
                        ELOG << "Invalid integer entered, assuming 0";
                        value = 0;
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                }
                else
                {
                    value = dist(mt);
                }

                DLOG << "Read value " << value;
                doubleMatrix->set(i, j, value);
            }
        }
    }
    else
    {
        for (unsigned i = 0; i < cols; ++i)
        {
            for (unsigned j = 0; j < rows; ++j)
            {
                double value = 0;
                if (manual)
                {
                    std::cout << "Value [" << j << ", " << i << "]: ";
                    std::cin >> value;

                    if (std::cin.fail())
                    {
                        ELOG << "Invalid integer entered, assuming 0";
                        value = 0;
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    }
                }
                else
                {
                    value = dist(mt);
                }

                DLOG << "Read value " << value;
                doubleMatrix->set(j, i, value);
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