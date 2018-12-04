#ifndef ACCESS_TYPE_H
#define ACCESS_TYPE_H

#include <ostream>

enum AccessType
{
    ROW_MAJOR = 1,
    COLUMN_MAJOR = 2
};

inline std::ostream &operator<<(std::ostream &os, const AccessType &type)
{
    switch (type)
    {
    case ROW_MAJOR:
        os << "ROW_MAJOR";
        break;
    case COLUMN_MAJOR:
        os << "COLUMN_MAJOR";
        break;
    }

    return os;
}

#endif