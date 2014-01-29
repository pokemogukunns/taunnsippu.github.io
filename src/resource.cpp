#include "town.h"

ElemRes::ElemRes(){
    tic=0;
    val=0;
    next=0;
}

ElemRes::ElemRes(char t, uint v){
    tic=t;
    val=v;
    next=0;
}

Resource::Resource(){
    head=0;
}

Resource::Resource(char* path){
    ifstream input;
    input.open(path);
    if(!input){
            cout << "Файл не открыт.\n";
            // !!!
    };
    char temp_tic;
    uint temp_val=0;
    ElemRes* curr_elem=NULL;
    bool first=1;
    while (!input.eof()){
        input >> temp_tic >> temp_val;
        // !!! проверка на корректность данных
        ElemRes* tmp=new ElemRes(temp_tic, temp_val);
        if (first==1){
            curr_elem=tmp;
            head=curr_elem;
            first=0;
        }
        else{
            curr_elem->next=tmp;
            curr_elem=tmp;
        }

    }
    input.close();
}

void Resource::getRes(uint* mass){
     ElemRes* curr_elem=head;
    while (curr_elem){
        if (curr_elem->tic=='w') mass[0]=curr_elem->val;
        else if (curr_elem->tic=='s') mass[1]=curr_elem->val;
        else if (curr_elem->tic=='f') mass[2]=curr_elem->val;
        else if (curr_elem->tic=='m') mass[3]=curr_elem->val;
        else if (curr_elem->tic=='h') mass[4]=curr_elem->val;
        else if (curr_elem->tic=='l') mass[5]=curr_elem->val;
        else if (curr_elem->tic=='b') mass[6]=curr_elem->val;
        else if (curr_elem->tic=='i') mass[7]=curr_elem->val;

        curr_elem=curr_elem->next;
    }
}

bool Resource::addRes(char t,uint v){
    ElemRes* curr_elem=head;
    while (curr_elem){
        if (curr_elem->tic==t){
            uint temp=curr_elem->val;
            curr_elem->val=temp+v;
            return 1;
        }
        curr_elem=curr_elem->next;
    }
    return 0;
}

bool Resource::takeRes(char t,uint v){
    ElemRes* curr_elem=head;
    while (curr_elem){
        if (curr_elem->tic==t){
            uint temp=curr_elem->val;
            curr_elem->val=temp-v;
            if (curr_elem->val<0){
                // !!! обработать ситуацию
            }
            return 1;
        }
        curr_elem=curr_elem->next;
    }
    return 0;
}
