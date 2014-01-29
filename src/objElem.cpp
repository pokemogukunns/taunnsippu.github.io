#include "town.h"

ObjElem::ObjElem(uint tp, uint hut_tp, uint lv, uint num, int x, int y, Field* pic_par){
    // !!! проверка типов на корректность
    type=tp;
    next=0;
    number=num;
    switch(tp){
    case 0:{
        obj=new Castle();
        obj->drawPict(pic_par, x, y,0, num);
        break;
    }
    case 1:{
        obj=new Storage(lv);
        obj->drawPict(pic_par, x, y,0, num);
        break;
    }
    case 2:{
        obj=new Market();
        obj->drawPict(pic_par, x, y,0, num);
        break;
    }
    case 3:{
        obj=new Hut(hut_tp, lv);
        obj->drawPict(pic_par, x, y, hut_tp, num);
        break;
    }
    }
    connect(obj,SIGNAL(click_to_obj1(uint)), this, SIGNAL(click_to_obj2(uint)));
}

uint ObjElem::getType(){
    return type;
}

