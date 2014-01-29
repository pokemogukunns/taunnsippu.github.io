#include "town.h"

SquareMap::SquareMap(){
    char* path = "Setting/MapInfo.txt";
    ifstream input;
    input.open(path);
    if(!input){
            cout << "Файл не открыт.\n";
            // !!!
    };
    // !!! проверить на нужное число элементов в файле
    input >> num_cells_w >> num_cells_h >> square_w >> square_h;
    input.close();



}
