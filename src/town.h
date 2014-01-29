#ifndef TOWN_H
#define TOWN_H

#include <QApplication>
#include <iostream>
#include <QStyle>
#include <QMouseEvent>
#include <fstream>
#include <vector>
#include <QtNetwork>

#include <QWidget>
#include <QtGui>
#include <QAction>
#include <QTimer>
#include <vector>
#include "ui_dialog.h"
#include "ui_reqRes.h"
#include "ui_castleFunc.h"
#include "ui_hutFunc.h"
#include "ui_marketFunc.h"
#include "ui_settlementFunc.h"
#include "ui_storageFunc.h"
#include "ui_inform.h"
#include "ui_gameSet.h"
#include "ui_help.h"
#include "ui_production1.h"
#include "ui_production2.h"
#include "ui_noticeExit.h"
#include "ui_newProfile.h"
#include "ui_profiles.h"
#include "ui_myProfile.h"

typedef unsigned int uint;
using namespace Ui;
using namespace std;
//---------------------------------------------
class Field: public QWidget{ // field for game
    Q_OBJECT
protected:
    virtual void mousePressEvent(QMouseEvent *);// обработчик события нажатия на поле
    virtual void mouseMoveEvent(QMouseEvent *);// обработчик перемещения указателя мыши
signals:
    void click_to_field(int, int);// сигнал с координатами нажатия на поле
    void mouse_move(int, int);// сигнал с текущими координатами мыши на поле
};
//--------------------------------------------
class Rect: public QLabel{ // прямоугольник выбора места на поле
    Q_OBJECT
public:
    Rect(QWidget* parrent):QLabel(parrent){
    };
protected:
    virtual void mouseMoveEvent(QMouseEvent *);// обработчик перемещения указателя мыши
    virtual void mousePressEvent(QMouseEvent *);// обработчик нажатия на прямоугольник
signals:
    void mouse_move_rect(int, int);// сигнал с текущими координатами мыши на прямоугольнике
    void mouse_press_rect(); // сигнал нажатия на прямоугольник, начало строительства
};
/*---------------------------------------------
class SquareMap{
    uint square_w, square_h;
    uint num_cells_w, num_cells_h;
    Square* head;
public:
    SquareMap();
    //~SquareMap();
};
//---------------------------------------------
class Square{
    uint x, y;
    uint center_x, center_y;
    uint number;
    uint status;
    Square* next;
    friend class SquareMap;
public:
    Square();
    Square(uint, uint, uint, uint, uint, uint);
    void getInfo(uint* );
    ~Square();
};
//---------------------------------------------
*/
class ObjectPic: public QLabel{
    Q_OBJECT
    uint number;
public:
    ObjectPic(Field* parrent, uint num):QLabel(parrent){
        number=num;
    }
    uint getNum(); // return number of object in list
protected:
    virtual void mousePressEvent(QMouseEvent*); // обработчик события нажатия на объект
signals:
     void click_to_obj(uint); // сигнал нажатия на объект (1-й в цепочке)
};

class ElemRes{
    char tic;
    uint val;
    ElemRes* next;
    friend class Resource;
public:
    ElemRes();
    ElemRes(char, uint);
};

class Resource{
    ElemRes* head;

public:
    Resource();
    Resource(char*);
    void getRes(uint* mass/*[8]*/); // получить ресурсы
    bool addRes(char ,uint); // добавить определенное количество единиц ресурса
    bool takeRes(char ,uint); // взять определенное количество единиц ресурса
    //~Resource();
};
//---------------------------------------------------
class Object: public QObject{
    Q_OBJECT
protected:
    Resource* need_res;
    ObjectPic* pict;
public:
     virtual Resource* getNeedRes()=0;
     virtual void drawPict(Field* parrent,int x, int y, uint hut_t, uint num)=0;
signals:
     void click_to_obj1(uint); // сигнал нажатия на объект (2-й в цепочке)
};
//---------------------------------------------------
class Storage: public Object{
    Q_OBJECT
    uint size;
    Resource* store_res;
    uint level;
public:
    Storage(uint);
    void drawPict(Field* parrent,int x, int y, uint hut_t, uint num);
    uint getSize();
    uint getLevel();
    Resource* getNeedRes();
    void getStoreRes(uint* res/*[8]*/);
    bool addStoreRes(char ,uint); // добавить определенное количество ресурса
    bool takeStoreRes(char ,uint); // изъять определенное количество ресурса
    //~Storage();
};
//---------------------------------------
class Market: public Object{
    Q_OBJECT
    vector <char> tic;
    vector <uint> price;
public:
    Market();
    void drawPict(Field* parrent,int x, int y, uint hut_t, uint num);
    Resource* getNeedRes();
    uint getPrice(char);// получить цену товара по метке
    //~Market();

};
//----------------------------------------
class Hut: public Object{
    Q_OBJECT
protected:
    uint empl;
    uint empl_act;
    uint level;
    uint type;
    //~Hut();
public:
    Hut(uint tp,uint lv);
    void drawPict(Field* parent,int x, int y, uint hut_t, uint num);
    Resource* getNeedRes();
    uint getType(); // тип объекта
    void getEmpl(uint* empl_mas/*[3]*/); // получить данные : все работники, активные, пассивные
    bool incEmpl(); // добавить одного активного работника
    bool decEmpl(); // убрать одного активного работника
public slots:
    void upd_empl_sl();
signals:
    void upd_empl_sign(Hut*);

};
//---------------------------------------------------
class Castle: public Object{
    Q_OBJECT
    Storage* store;
    Hut* settl;
public:
    Castle();
    uint getStoreSize();
    void drawPict(Field* parrent,int x, int y, uint hut_t, uint num);
    Resource* getNeedRes();
    void getCStoreRes(uint* res/*[8]*/);
    void getCEmpl(uint* empl_mas/*[3]*/);
    bool incCEmpl();
    bool decCEmpl();
    bool addCStoreRes(char ,uint); // добавить определенное количество ресурса
    bool takeCStoreRes(char ,uint); // изъять определенное количество ресурса
    //~Castle();
};
//---------------------------------------
class ObjElem: public QObject{
     Q_OBJECT
    Object* obj;
    uint type;
    ObjElem* next;
    uint number;
    friend class Game;
public:
    ObjElem(uint tp, uint hut_tp, uint lv, uint num, int x, int y, Field* pic_par);
    uint getType();
    //~ObjElem();
signals:
    void click_to_obj2(uint);// сигнал нажатия на объект (3-й в цепочке)
};
//-----------------------------------------
class MainWindow: public QWidget{
    Q_OBJECT

    Ui::GameSet game_s;
    Ui::Help help_d;
    Ui::NewProfile new_prof_d;
    Ui::Profiles prof_d;
    Ui::MyProfile my_prof_d;

    QHBoxLayout* hlayout;
    QVBoxLayout* vlayout;
    QPushButton* new_butt;
    QPushButton* prof_butt;
    QPushButton* exit_butt;
    QPushButton* help_butt;
    QPushButton* about_butt;
    QWidget* backgr_wgt;
    QDialog* prof_dia;
    QDialog* new_dia;
    bool test_game; // тестовый вход
    bool is_ok_registration; // нажатие клавиши регистрации

    uint temp_cash; // for start difficultly in new game
    QString player;
    QString difficultly;
    bool is_logged_in;
    uint profile_image;
    uint cash_tab[3];

    QNetworkAccessManager manager;
    QNetworkReply *reply;

public:
    MainWindow(bool, uint , QString);
    MainWindow(const MainWindow&);
    ~MainWindow();
private slots:
    void newGame();
    void gameSett();
    void newProf();
    void profiles();
    void auth();
    void registration();
    void out();
    void about();
    void help();
    void exit();
    void test_game_on(); // устанавливает тестовый вход
    void test_game_off(); // сброс тестового входа
    void readyReplyAuth();
    void readyReplyOut();
    void readyReplyReg();
    void readyReplyExit();
    void okRegistration();
};

//--------------------------------------------

class Game: public QWidget{
    Q_OBJECT
protected:
    virtual void keyPressEvent(QKeyEvent* );
    virtual void keyReleasedEvent(QKeyEvent* );
public:
    Game(uint, QString, uint, QString, bool);
    uint getCash();
    void recalc(); // пересчет результатов
    bool buy(char, uint); // купить товар то метке и количеству
    bool sell(char, uint); // продать товар по метке и количеству
    void traderCame(); // торговец приехал
    void traderLeft(); // торговец уехал
    void getResInStore(uint* res); // получить все имеющиеся ресурсы
    void closeScroll();

    ~Game();


private:
    //forms
    Ui::CreateDialog create_d;
    Ui::reqRes req_r;
    Ui::CastleFunc cast_f;
    Ui::HutFunc hut_f;
    Ui::MarketFunc mark_f;
    Ui::SettlementFunc settl_f;
    Ui::StorageFunc store_f;
    Ui::InformWgt inf_wgt;
    Ui::Help help_d;
    Ui::Production1 product1_d;
    Ui::Production2 product2_d;
    Ui::NoticeExit warn_exit_d;

    QTimer* timer;
    QScrollArea* scroll;
    QWidget* inform_wgt;
    Field* field;
    Rect* rect;
    Hut* temp_hut;
    QString player; // имя игрока
    QTime clock; // игровые часы
    QString time_in_game; // время в игре
    bool test_game; // тестовый вход
    QNetworkAccessManager* manager;
    QNetworkReply *reply;
    int curr_x; // текущие координаты курсора в строительном режиме
    int curr_y; // текущие координаты курсора в строительном режиме
    bool build_mode; // режим выбора места для строительства

    QDialog* warn_dia; // диалог предупреждения о выходе
    bool is_ok; // показывает когда кнопка Ok нажата в диалоге нужных ресурсов
    bool is_ok_create_dia; // показывает когда кнопка Ok нажата в диалоге создания
    QString start_difficulty; // начальная сложность игры
    uint days; // общее количество дней в игре
    uint max_cash; // максимум наличности за игру
    uint min_cash; // минимум наличности за игру
    uint storages; // к-во складов
    uint settlements; // к-во поселений
    uint hut_objects; // к-во действующих объектов
    uint total_employers; // общее к-во рабочих

    uint profile_image; // картинка из профиля игрока
    uint cash; // счет
    ObjElem* head_obj; // первый объект в списке
    bool trader; // торговец
    bool first_cast; // первый замок
    bool market_exist;
    bool cast_exist; // существование замка
    bool paused;
    int speed_k; // коэффициент скорости времени игры

    // для строительства
    uint build_tp;
    uint build_hut_tp;
    uint build_level;

    void recalcStatistic(uint, uint, uint); // пересчет данных для статистики
private slots:
    void createObj(); // создать объект
    void buildModeOn(); // включение режима строительства
    void buildModeOff(); // выключение режима строительства
    void viewProduction();
    void okIsClicked();
    void okIsClickedCreateDia();
    void newMenu(); // выход в начальное меню
    void newMenuTest(); // выход в начальное меню с тестовым входом
    void help();
    void pause();
    void createMenu();
    void deleteWarnDialog(); // удаление диалога предупреждения
    void saveStatistic(); // сохранить статистику в базу данных
    void readyReplySaveStat(); // сохранение готово
    void traderFunc();
    void create(int, int);
    void drawRect(int, int); // отрисовка прямоугольника
    void drawRectMoveRight(int, int); // отрисовка прямоугольника, когда указатель движется вправо
    void funcObj(uint); // возможные действия с объектом по его номеру в списке(с 1)
    void countDay(); // пересчет индикаторов текущего времени
    void nextDay(); // обработка кнопки следующий день
    void speedChanged(int);
    void buySlw(){
        buySl('w', 1);
    }
    void buySls(){
        buySl('s', 1);
    }
    void buySlf(){
        buySl('f', 1);
    }
    void buySlm(){
        buySl('m', 1);
    }
    void buySlh(){
        buySl('h', 1);
    }
    void buySll(){
        buySl('l', 1);
    }
    void buySlb(){
        buySl('b', 1);
    }
    void buySli(){
        buySl('i', 1);
    }void sellSlw(){
        sellSl('w', 1);
    }
    void sellSls(){
        sellSl('s', 1);
    }
    void sellSlf(){
        sellSl('f', 1);
    }
    void sellSlm(){
        sellSl('m', 1);
    }
    void sellSlh(){
        sellSl('h', 1);
    }
    void sellSll(){
        sellSl('l', 1);
    }
    void sellSlb(){
        sellSl('b', 1);
    }
    void sellSli(){
        sellSl('i', 1);
    }
    void buySl(char, uint);
    void sellSl(char, uint);
    void addEmpl(Hut*);
    void remEmpl(Hut*);
    void addEmplSl(){
        addEmpl(temp_hut);
    }
    void remEmplSl(){
        remEmpl(temp_hut);
    }

    void updt_mark_dia();
public slots:
    void updt_empl_dia(Hut*);
signals:
    void reject_find_place();
};

#endif // TOWN_H
