#include "town.h"

void Field::mousePressEvent(QMouseEvent *ev){
    if (ev->x()!=0 && ev->y()!=0){
        emit click_to_field(ev->x(), ev->y());
    }
}

void Field::mouseMoveEvent(QMouseEvent *move_ev){
    if (move_ev->x()!=0 && move_ev->y()!=0){
        emit mouse_move(move_ev->x(), move_ev->y());
    }
}

