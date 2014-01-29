#include "town.h"

Storage::Storage(uint lv){
    store_res=new Resource("Setting/Storage/FR.txt");//!!! ���� � ����� � ���������� ��������� ���������
    //!!! �������� lv<=0, lv>3
    level=lv;
    ifstream input;
    input.open("Setting/Storage/Size.txt");//!!! ���� � ����� � ��������� �������
    uint temp_lev=0, temp_size=0;
    while(!input.eof()){
        input >> temp_lev >> temp_size;
        if (level==temp_lev){
            size=temp_size;
            break;
        }
        //!!! �������� �� �������������� ������ � �����
    }
    input.close();
    switch(level){
    case 1:{
        need_res=new Resource("Setting/Storage/NR1.txt");//!!! ���� � ����� � ������� ���������
        break;
    }
    case 2:{
        need_res=new Resource("Setting/Storage/NR2.txt");//!!! ���� � ����� � ������� ���������
        break;
    }
    case 3:{
        need_res=new Resource("Setting/Storage/NR3.txt");//!!! ���� � ����� � ������� ���������
        break;
    }
    default:{
    // !!! ����������
        break;
    }
    }

}


uint Storage::getSize(){
    return size;
}

uint Storage::getLevel(){
    return level;
}

void Storage::drawPict(Field* parrent, int x, int y, uint hut_t, uint num){
    ObjectPic* lbl = new ObjectPic(parrent, num);
    lbl->setScaledContents(true);
    uint pic_size_x = 50, pic_size_y = 50;
    switch(level){
    case 1:{
        lbl->setPixmap(QPixmap(":/images/level_1/storage_1.png"));
        pic_size_x = 100;
        pic_size_y = 72;
        break;
    }
    case 2:{
        lbl->setPixmap(QPixmap(":/images/level_2/storage_2.png"));
        pic_size_x = 100;
        pic_size_y = 72;
        break;
    }
    case 3:{
        lbl->setPixmap(QPixmap(":/images/level_3/storage_3.png"));
        pic_size_x = 140;
        pic_size_y = 100;
        break;
    }
    }
    lbl->setGeometry(x, y ,pic_size_x, pic_size_y);
    lbl->show();
    connect(lbl, SIGNAL(click_to_obj(uint)), this, SIGNAL(click_to_obj1(uint)));
}

Resource* Storage::getNeedRes(){
    return need_res;
}

void Storage::getStoreRes(uint* res){
    store_res->getRes(res);
}

bool Storage::addStoreRes(char tc,uint vl){
    uint mass[8];
    uint res; // ������� ������
    store_res->getRes(mass); //�������� ��� ������� ������

    // �������� ������ ������ � ���������� ��� ����������
    if (tc=='w') res=mass[0];
    else if (tc=='s') res=mass[1];
    else if (tc=='f') res=mass[2];
    else if (tc=='m') res=mass[3];
    else if (tc=='h') res=mass[4];
    else if (tc=='l') res=mass[5];
    else if (tc=='b') res=mass[6];
    else if (tc=='i') res=mass[7];
    else {
        //cout << "�������� �������� �������� �������" << endl;
        return 0;
    }
    if (res+vl>size){
        //cout << "������������ ������" << endl;
        return 0;
    }

    if (store_res->addRes(tc, vl))
        //cout << "��������� �������" << endl;
        ;
    else{
        //cout << "�� ���������, �.�. �� ������ �� ������������� ����� ��������" << endl;
        return 0;
    }
    return 1;
}

bool Storage::takeStoreRes(char tc,uint vl){
    uint mass[8];
    uint res; // ������� ������
    store_res->getRes(mass); //�������� ��� ������� ������
    // �������� ������ ������ � ���������� ��� ����������
    if (tc=='w') res=mass[0];
    else if (tc=='s') res=mass[1];
    else if (tc=='f') res=mass[2];
    else if (tc=='m') res=mass[3];
    else if (tc=='h') res=mass[4];
    else if (tc=='l') res=mass[5];
    else if (tc=='b') res=mass[6];
    else if (tc=='i') res=mass[7];
    else {
        //cout << "�������� �������� �������� �������" << endl;
        return 0;
    }

    if (res<vl){
        //cout << "�� ������ ��� ������, ������� ����� ������" << endl;
        return 0;
    }

    if (store_res->takeRes(tc, vl))
        //cout << "������ �������" << endl;
        ;
    else{
        //cout << "�� ������, �.�. �� ������ �� ������������� ����� ��������" << endl;
        return 0;
    }
    return 1;

}
