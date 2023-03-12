#ifndef ERRORS_H
#define ERRORS_H

enum errors
{
    OK,
    ERR_OPEN_FILE,
    ERR_NULL_FILE,
    ERR_FGETS,

    ERR_READ_POINT_INVALID_TYPE,
    ERR_READ_POINT_INVALID_DATA,

    ERR_READ_EDGE_INVALID_TYPE,
    ERR_READ_EDGE_INVALID_DATA,

    ERR_INVALID_ACTION_IN_MW_SETUP_CHANGE,

    ERR_UNKNOWN,
};
using err_t = enum errors;

#endif // ERRORS_H
