#include "town.h"

Square::Square(){
    x, y, center_x, center_y, number, status = 0;
    next = 0;
}

Square::Square(uint x_arg, uint y_arg, uint center_x_arg, uint center_y_arg, uint number_arg, uint status_arg){
    x = x_arg;
    y = y_arg;
    center_x = center_x_arg;
    center_y = center_y_arg;
    number = number_arg;
    status = status_arg;
    next = 0;
}

void Square::getInfo(uint* info){
    uint info_array[6] = {number, x, y, center_x, center_y, status};
    info = info_array;
}
