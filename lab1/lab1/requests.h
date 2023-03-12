#ifndef REQUESTS_H
#define REQUESTS_H

#include "errors.h"
#include "qt/draw.h" // -> canvas_t
#include "points.h"
#include "figure.h" // -> filename_t, change_t

enum actions
{
    LOAD,
    SAVE,
    DRAW,
    TRANSLATE,
    ROTATE,
    SCALE,
    QUIT,
};
using action_t = enum actions;


struct request
{
    action_t action;
    filename_t filename;
    canvas_t canvas;
    change_t change;
};
using request_t = struct request;


err_t process_request(const request_t &request);

#endif // REQUESTS_H
