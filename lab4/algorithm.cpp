#include "algorithm.h"

static int calculate_next_floor(int current_floor, int floor_btns_bin, int cabin_btns_bin, bool is_moving, bool up)
{
    int next_floor = 0;
    int dir = up ? 1 : -1;
    bool flag = false;

    for (int floor = current_floor + dir; !flag && 1 <= floor && floor <= NUMBER_OF_FLOORS; floor += dir) {
        if (floor_btns_bin & (1 << (floor - 1)) || cabin_btns_bin & (1 << (floor - 1)))
        {
            next_floor = floor;
            flag = true;
        }
    }

    return next_floor;
}

int calculate_next_target_floor(int current_floor, int floor_btns_bin, int cabin_btns_bin, bool is_moving, bool up)
{
    int next_floor = 0;
    int next_floor_up = 0;
    int next_floor_down = 0;
    
    next_floor_up = calculate_next_floor(current_floor, floor_btns_bin, cabin_btns_bin, is_moving, up);
    next_floor_down = calculate_next_floor(current_floor, floor_btns_bin, cabin_btns_bin, is_moving, !up);

    /* bool flag = false; */

    /* for (int floor = current_floor + 1; !flag && 1 <= floor && floor <= NUMBER_OF_FLOORS; floor += 1) { */
    /*     if (floor_btns_bin & (1 << (floor - 1)) || cabin_btns_bin & (1 << (floor - 1))) */
    /*     { */
    /*         next_floor_up = floor; */
    /*         flag = true; */
    /*     } */
    /* } */

    /* for (int floor = current_floor - 1; !flag && 1 <= floor && floor <= NUMBER_OF_FLOORS; floor -= 1) { */
    /*     if (floor_btns_bin & (1 << (floor - 1)) || cabin_btns_bin & (1 << (floor - 1))) */
    /*     { */
    /*         next_floor_down = floor; */
    /*         flag = true; */
    /*     } */
    /* } */

    // Take next floor on the way up/down if the elevator is moving
    if (is_moving)
    {
        if (up)
        {
            if (next_floor_up)
            {
                next_floor = next_floor_up;
            }
            // Cabin is moving up, but there are no floors above activated
            else
            {
                next_floor = next_floor_down;
            }
        }
        else
        {
            if (next_floor_down)
            {
                next_floor = next_floor_down;
            }
            else
            {
                next_floor = next_floor_up;
            }
        }
        /* next_floor = up ? next_floor_up : next_floor_down; */
    }
    // If elevator is idling/waiting, take the nearest floor
    else
    {
        if (next_floor_up && next_floor_down)
        {
            int cond = (next_floor_up - current_floor) < (current_floor - next_floor_down);

            next_floor = cond ? next_floor_up : next_floor_down;
        }
        else if (next_floor_up)
        {
            next_floor = next_floor_up;
        }
        else if (next_floor_down)
        {
            next_floor = next_floor_down;
        }
        else
        {
            if (floor_btns_bin & (1 << (current_floor - 1)) || cabin_btns_bin & (1 << (current_floor - 1)))
            {
                next_floor = current_floor;
            }
            else
            {
                next_floor = 0;
            }
        }
    }

    return next_floor;
}
