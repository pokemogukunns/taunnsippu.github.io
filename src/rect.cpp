#include "town.h"

void Rect::mouseMoveEvent(QMouseEvent *move_ev){
    emit mouse_move_rect(move_ev->x(), move_ev->y());
}

void Rect::mousePressEvent(QMouseEvent *event_press){
    emit mouse_press_rect();
}
