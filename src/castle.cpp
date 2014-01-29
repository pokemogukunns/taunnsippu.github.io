#include "town.h"

Castle::Castle(){
    need_res=new Resource("Setting/Castle/NR1.txt");
    settl=new Hut(1, 1);
    store=new Storage(1);
}

uint Castle::getStoreSize(){
    return store->getSize();
}

void Castle::drawPict(Field* parrent, int x, int y, uint hut_t, uint num){
    ObjectPic* lbl = new ObjectPic(parrent, num);
    lbl->setPixmap(QPixmap(":/images/castle.png"));
    lbl->setGeometry(x, y ,130,110);
    lbl->show();
    connect(lbl, SIGNAL(click_to_obj(uint)), this, SIGNAL(click_to_obj1(uint)));
}

void Castle::getCStoreRes(uint *res){
    store->getStoreRes(res);
}

void Castle::getCEmpl(uint *empl_mas){
    settl->getEmpl(empl_mas);
}

Resource* Castle::getNeedRes(){
    return need_res;
}

bool Castle::addCStoreRes(char tc,uint vl){
    if (store->addStoreRes(tc, vl)) return 1;
    else return 0;
}

bool Castle::takeCStoreRes(char tc,uint vl){
    if (store->takeStoreRes(tc, vl)) return 1;
    else return 0;
}

bool Castle::incCEmpl(){
    if (settl->incEmpl()) return 1;
    else return 0;

}

bool Castle::decCEmpl(){
    if (settl->decEmpl()) return 1;
    else return 0;

}
