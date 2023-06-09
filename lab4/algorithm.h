#pragma once

#ifndef NUMBER_OF_FLOORS
#define NUMBER_OF_FLOORS 10
#endif

int calculate_next_target_floor(int current_floor, int floor_btns_bin, int cabin_btns_bin, bool is_moving, bool up);
