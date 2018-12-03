#include <iostream>
#include "int-matrix.h"
#include <cmath>

int main()
{
    std::cout << "Hello, World!" << std::endl;

    MatrixBase<int> *matrix = new IntMatrix(2, 2, COLUMN_MAJOR);

    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 2; ++j) {
            int t;
            scanf("%d", &t);
            matrix->set(i, j, t);
        }

    matrix->display();

    return 0;
}