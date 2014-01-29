#include "town.h"

Market::Market(){
    ifstream input;
    input.open("Setting/Market/Exchange.txt");//!!! путь к файлу с обменным курсом
    char temp_tic=NULL;
    uint temp_price=0;
    while(!input.eof()){
        input >> temp_tic >> temp_price;
        tic.push_back(temp_tic);
        price.push_back(temp_price);
        //!!! проверка на несуществующую запись в файле
    }
    input.close();
    need_res=new Resource("Setting/Market/NR1.txt");
}

void Market::drawPict(Field* parent, int x, int y, uint hut_t, uint num){
    ObjectPic* lbl = new ObjectPic(parent, num);
    lbl->setScaledContents(true);
    lbl->setPixmap(QPixmap(":/images/market.png"));
    lbl->setGeometry(x, y ,200,120);
    lbl->show();
    connect(lbl, SIGNAL(click_to_obj(uint)), this, SIGNAL(click_to_obj1(uint)));
}

Resource* Market::getNeedRes(){
    return need_res;
}

uint Market::getPrice(char tc){
    uint size=tic.size(), i=0;
    while (i<size){
        if (tic[i]==tc)
            return price[i];
        i++;
    }

}
