#include "town.h"

Hut::Hut(uint tp, uint lv){
    type=tp;
    //!!! проверка правильности типа
    level=lv;
    //!!! проверка lv<=0, lv>3
    empl_act=0;

    ifstream input;
    input.open("Setting/Hut/LevAndEmpl.txt");//!!! путь к файлу с к-вом рабочих на уровн€х
    uint temp_lev=0, temp_empl=0;
    while(!input.eof()){
        input >> temp_lev >> temp_empl;
        if (level==temp_lev){
            empl=temp_empl;
            break;
        }
        //!!! проверка на несуществующую запись в файле
    }
    input.close();
    char* path;
    switch(type){ // выбираем нужный путь к файлу с ресурсами по уровн€м и типу
    case 1:{ // поселение
        switch(level){
        case 1:{
            path="Setting/Hut/Settlement/NR1.txt";
            break;
        }
        case 2:{
            path="Setting/Hut/Settlement/NR2.txt";
            break;
        }
        case 3:{
            path="Setting/Hut/Settlement/NR3.txt";
            break;
        }
        }
        break;
    }
    case 2:{
        switch(level){
        case 1:{
            path="Setting/Hut/Cutter/NR1.txt";
            break;
        }
        case 2:{
            path="Setting/Hut/Cutter/NR2.txt";
            break;
        }
        case 3:{
            path="Setting/Hut/Cutter/NR3.txt";
            break;
        }
        }
        break;
    }
    case 3:{
        switch(level){
        case 1:{
            path="Setting/Hut/Mine/NR1.txt";
            break;
        }
        case 2:{
            path="Setting/Hut/Mine/NR2.txt";
            break;
        }
        case 3:{
            path="Setting/Hut/Mine/NR3.txt";
            break;
        }
        }
        break;
    }
    case 4:{
        switch(level){
        case 1:{
            path="Setting/Hut/Fishing/NR1.txt";
            break;
        }
        case 2:{
            path="Setting/Hut/Fishing/NR2.txt";
            break;
        }
        case 3:{
            path="Setting/Hut/Fishing/NR3.txt";
            break;
        }
        }
        break;
    }
    case 5:{
        switch(level){
        case 1:{
            path="Setting/Hut/Mill/NR1.txt";
            break;
        }
        case 2:{
            path="Setting/Hut/Mill/NR2.txt";
            break;
        }
        case 3:{
            path="Setting/Hut/Mill/NR3.txt";
            break;
        }
        }
        break;
    }
    case 6:{
        switch(level){
        case 1:{
            path="Setting/Hut/Bakery/NR1.txt";
            break;
        }
        case 2:{
            path="Setting/Hut/Bakery/NR2.txt";
            break;
        }
        case 3:{
            path="Setting/Hut/Bakery/NR3.txt";
            break;
        }
        }
        break;
    }
    case 7:{
        switch(level){
        case 1:{
            path="Setting/Hut/Smithy/NR1.txt";
            break;
        }
        case 2:{
            path="Setting/Hut/Smithy/NR2.txt";
            break;
        }
        case 3:{
            path="Setting/Hut/Smithy/NR3.txt";
            break;
        }
        }
        break;
    }
    case 8:{
        switch(level){
        case 1:{
            path="Setting/Hut/Field/NR1.txt";
            break;
        }
        case 2:{
            path="Setting/Hut/Field/NR2.txt";
            break;
        }
        case 3:{
            path="Setting/Hut/Field/NR3.txt";
            break;
        }
        }
        break;
    }
    case 9:{
        switch(level){
        case 1:{
            path="Setting/Hut/Piggery/NR1.txt";
            break;
        }
        case 2:{
            path="Setting/Hut/Piggery/NR2.txt";
            break;
        }
        case 3:{
            path="Setting/Hut/Piggery/NR3.txt";
            break;
        }
        }
        break;
    }
    }


    need_res=new Resource(path);
}

void Hut::drawPict(Field* parrent, int x, int y, uint hut_t, uint num){
    ObjectPic* lbl = new ObjectPic(parrent, num);
    lbl->setScaledContents(true);
    int pic_size_x = 50, pic_size_y = 50; // индивидуальные размеры изображений строений
    switch(hut_t){ // выбираем изображени€ к объекту по типу и уровню
    case 1:{ // поселение
        switch(level){
        case 1:{
            lbl->setPixmap(QPixmap(":/images/level_1/settlement_1.png"));
            pic_size_x = 120;
            pic_size_y = 86;
            break;
        }
        case 2:{
            lbl->setPixmap(QPixmap(":/images/level_2/settlement_2.png"));
            pic_size_x = 120;
            pic_size_y = 86;
            break;
        }
        case 3:{
            lbl->setPixmap(QPixmap(":/images/level_3/settlement_3.png"));
            pic_size_x = 120;
            pic_size_y = 86;
            break;
        }
        }
        break;
    }
    case 2:{ // хижина дровосека
        switch(level){
        case 1:{
            lbl->setPixmap(QPixmap(":/images/level_1/cutting_hut_1.png"));
            pic_size_x = 100;
            pic_size_y = 72;
            break;
        }
        case 2:{
            lbl->setPixmap(QPixmap(":/images/level_2/cutting_hut_2.png"));
            pic_size_x = 100;
            pic_size_y = 72;
            break;
        }
        case 3:{
            lbl->setPixmap(QPixmap(":/images/level_3/cutting_hut_3.png"));
            pic_size_x = 100;
            pic_size_y = 72;
            break;
        }
        }
        break;
    }
    case 3:{ // каменоломн€
        switch(level){
        case 1:{
            lbl->setPixmap(QPixmap(":/images/level_1/mine_1_final.png"));
            pic_size_x = 110;
            pic_size_y = 72;
            break;
        }
        case 2:{
            lbl->setPixmap(QPixmap(":/images/level_2/mine_2_final.png"));
            pic_size_x = 110;
            pic_size_y = 72;
            break;
        }
        case 3:{
            lbl->setPixmap(QPixmap(":/images/level_3/mine_3_final.png"));
            pic_size_x = 110;
            pic_size_y = 72;
            break;
        }
        }
        break;
    }
    case 4:{ // рыбацка€ хижина
        switch(level){
        case 1:{
            lbl->setPixmap(QPixmap(":/images/level_1/fishing_hut_1.png"));
            pic_size_x = 80;
            pic_size_y = 57;
            break;
        }
        case 2:{
            lbl->setPixmap(QPixmap(":/images/level_2/fishing_hut_2.png"));
            pic_size_x = 100;
            pic_size_y = 72;
            break;
        }
        case 3:{
            lbl->setPixmap(QPixmap(":/images/level_3/fishing_hut_3.png"));
            pic_size_x = 110;
            pic_size_y = 79;
            break;
        }
        }
        break;
    }
    case 5:{ // мельница
        switch(level){
        case 1:{
            lbl->setPixmap(QPixmap(":/images/level_1/mill_1.png"));
            pic_size_x = 60;
            pic_size_y = 72;
            break;
        }
        case 2:{
            lbl->setPixmap(QPixmap(":/images/level_2/mill_2.png"));
            pic_size_x = 95;
            pic_size_y = 80;
            break;
        }
        case 3:{
            lbl->setPixmap(QPixmap(":/images/level_3/mill_3.png"));
            pic_size_x = 100;
            pic_size_y = 100;
            break;
        }
        }
        break;
    }
    case 6:{ // пекарн€
        switch(level){
        case 1:{
            lbl->setPixmap(QPixmap(":/images/level_1/bakery_1.png"));
            pic_size_x = 80;
            pic_size_y = 57;
            break;
        }
        case 2:{
            lbl->setPixmap(QPixmap(":/images/level_2/bakery_2.png"));
            pic_size_x = 100;
            pic_size_y = 72;
            break;
        }
        case 3:{
            lbl->setPixmap(QPixmap(":/images/level_3/bakery_3.png"));
            pic_size_x = 120;
            pic_size_y = 86;
            break;
        }
        }
        break;
    }
    case 7:{ // кузница
        switch(level){
        case 1:{
            lbl->setPixmap(QPixmap(":/images/level_1/smithy_1.png"));
            pic_size_x = 80;
            pic_size_y = 57;
            break;
        }
        case 2:{
            lbl->setPixmap(QPixmap(":/images/level_2/smithy_2.png"));
            pic_size_x = 100;
            pic_size_y = 72;
            break;
        }
        case 3:{
            lbl->setPixmap(QPixmap(":/images/level_3/smithy_3.png"));
            pic_size_x = 110;
            pic_size_y = 79;
            break;
        }
        }
        break;
    }
    case 8:{ // поле
        switch(level){
        case 1:{
            lbl->setPixmap(QPixmap(":/images/level_1/field_1.png"));
            pic_size_x = 120;
            pic_size_y = 86;
            break;
        }
        case 2:{
            lbl->setPixmap(QPixmap(":/images/level_2/field_2.png"));
            pic_size_x = 130;
            pic_size_y = 108;
            break;
        }
        case 3:{
            lbl->setPixmap(QPixmap(":/images/level_3/field_3.png"));
            pic_size_x = 170;
            pic_size_y = 122;
            break;
        }
        }
        break;
    }
    case 9:{ // свинарник
        switch(level){
        case 1:{
            lbl->setPixmap(QPixmap(":/images/level_1/piggery_1.png"));
            pic_size_x = 120;
            pic_size_y = 86;
            break;
        }
        case 2:{
            lbl->setPixmap(QPixmap(":/images/level_2/piggery_2.png"));
            pic_size_x = 130;
            pic_size_y = 108;
            break;
        }
        case 3:{
            lbl->setPixmap(QPixmap(":/images/level_3/piggery_3.png"));
            pic_size_x = 170;
            pic_size_y = 122;
            break;
        }
        }
        break;
    }
    }
    lbl->setGeometry(x, y, pic_size_x, pic_size_y);
    lbl->show();
    connect(lbl, SIGNAL(click_to_obj(uint)), this, SIGNAL(click_to_obj1(uint)));
}

Resource* Hut::getNeedRes(){
    return need_res;
}

uint Hut::getType(){
    return type;
}

void Hut::getEmpl(uint *empl_mas){
    empl_mas[0]=empl;
    empl_mas[1]=empl_act;
    empl_mas[2]=empl-empl_act;
}

bool Hut::incEmpl(){
    if (empl_act+1<=empl){
        empl_act++;
        return 1;
    }
    else{
        //cout << "ѕри попытке добавлени€ было превышено предельное значение рабочих" << endl;
        return 0;
    }
}

bool Hut::decEmpl(){
    if (empl_act>=1){
        empl_act--;
        return 1;
    }
    else{
        //cout << "–абочих нет" << endl;
        return 0;
    }
}

void Hut::upd_empl_sl(){
    emit upd_empl_sign(this);
}
