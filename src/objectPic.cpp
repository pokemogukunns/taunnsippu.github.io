#include "town.h"

uint ObjectPic::getNum(){
    return number;
}

void ObjectPic::mousePressEvent(QMouseEvent * ev){
    uint num = this->getNum();
    emit click_to_obj(num);

}
