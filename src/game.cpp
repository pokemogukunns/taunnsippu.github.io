#include "town.h"

Game::Game(uint first_cash, QString player_name, uint profile_image_arg, QString difficultly, bool test_game_arg){
    test_game=test_game_arg;
    head_obj=0;
    trader=0;
    first_cast=1;
    market_exist=0;
    cast_exist=0;
    paused=0;
    cash=first_cash;
    player=player_name;
    profile_image=profile_image_arg;
    is_ok=0;
    is_ok_create_dia=0;
    clock=QTime(0,0,0,0);
    curr_x=0;
    curr_y=0;
    time_in_game=QString("000000");
    clock.start();
    build_mode=0;

    start_difficulty=difficultly;
    days=0;
    max_cash=first_cash;
    min_cash=first_cash;
    storages=0;
    settlements=0;
    hut_objects=0;
    total_employers=0;

    scroll = new QScrollArea;
    field = new Field;

    QPalette pal;
    pal.setBrush(field->backgroundRole(), QBrush(QPixmap(":/images/field.jpg")));
    field->setPalette(pal);
    field->setMinimumHeight(690);
    field->setMinimumWidth(1350);
    field->setMouseTracking(true); // даем возможность отправлять событие MouseMove при каждом перемещении мыши

    rect = new Rect(scroll);
    rect->setScaledContents(true);
    rect->setMouseTracking(true); // даем возможность отправлять событие MouseMove при каждом перемещении мыши

    scroll->setWidget(field);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scroll->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scroll->setWidgetResizable(true);

    QMouseEvent mouse_cl(QEvent::MouseButtonPress, QPoint(), Qt::LeftButton , Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(field, &mouse_cl);

    QMouseEvent mouse_move(QEvent::MouseMove, QPoint(),Qt::LeftButton , Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(field, &mouse_move);

    QMouseEvent mouse_move_rect(QEvent::MouseMove, QPoint(),Qt::LeftButton , Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(rect, &mouse_move_rect);

    QMouseEvent mouse_click_rect(QEvent::MouseButtonPress, QPoint(), Qt::LeftButton , Qt::LeftButton, Qt::NoModifier);
    QApplication::sendEvent(rect, &mouse_click_rect);

    connect(field, SIGNAL(click_to_field(int, int)),this ,SLOT(create(int, int)));
    //connect(this, SIGNAL(reject_find_place()), this, SLOT(buildModeOff()));

    inform_wgt = new QWidget(scroll);
    inf_wgt.setupUi(inform_wgt);
    inform_wgt->setGeometry(0,0, 928, 41);
    speed_k = inf_wgt.horizontalSlider->value();
    inf_wgt.lcdNumber->display(int(cash));

    connect(inf_wgt.horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(speedChanged(int)));
    connect(inf_wgt.pushButton, SIGNAL(clicked()), this, SLOT(nextDay()));
    connect(inf_wgt.pushButton_2, SIGNAL(clicked()), this, SLOT(traderFunc()));

    connect(inf_wgt.pushButton_3, SIGNAL(clicked()), this, SLOT(pause()));
    connect(inf_wgt.pushButton_4, SIGNAL(clicked()), this, SLOT(createMenu()));
    connect(inf_wgt.pushButton_5, SIGNAL(clicked()), this, SLOT(help()));


    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(countDay()));
    timer->start(2000); // timer 2 sec.
    scroll->showMaximized();
    scroll->setAttribute(Qt::WA_DeleteOnClose);
}

Game::~Game(){

}

void Game::drawRect(int x, int y){
    curr_x = x;
    curr_y = y;
    rect->setGeometry(curr_x, curr_y ,150, 150);
}

void Game::drawRectMoveRight(int x, int y){
    curr_x = curr_x + x;
    curr_y = curr_y + y;
    rect->setGeometry(curr_x, curr_y ,150,150);
}

void Game::okIsClicked(){
    is_ok = 1;
}
void Game::closeScroll(){
    scroll->close();
}

void Game::createMenu(){
    warn_dia = new QDialog();
    warn_exit_d.setupUi(warn_dia);
    if (test_game == 1){
        connect(warn_exit_d.pushButton, SIGNAL(clicked()), this, SLOT(newMenuTest()));
    } else {
        connect(warn_exit_d.pushButton, SIGNAL(clicked()), this, SLOT(saveStatistic()));
    }
    connect(warn_exit_d.pushButton_2, SIGNAL(clicked()), this, SLOT(deleteWarnDialog()));
    warn_dia->exec();
}

void Game::deleteWarnDialog(){
    warn_dia->close();
    delete warn_dia;
}

void Game::newMenu(){
    MainWindow* m_win = new MainWindow(1, profile_image, player);
    m_win->showMaximized();
    closeScroll();
    this->~Game();
}

void Game::newMenuTest(){
    MainWindow* m_win = new MainWindow(0, profile_image, player);
    m_win->showMaximized();
    closeScroll();
    this->~Game();
}

void Game::help(){
    QDialog* help_dia = new QDialog();
    help_dia->setAttribute(Qt::WA_DeleteOnClose);
    help_d.setupUi(help_dia);
    help_dia->exec();
}

void Game::pause(){
    if (paused)
        paused=0;
    else
        paused=1;
}

void Game::speedChanged(int val){
    speed_k = val;
}

void Game::traderFunc(){
    if (trader){
        Market* market=new Market();
        QDialog* mark_dia = new QDialog();
        mark_dia->setAttribute(Qt::WA_DeleteOnClose);
        mark_f.setupUi(mark_dia);
        mark_f.label->setText(QString("Trader"));
        mark_f.lcdNumber_2->display(int(market->getPrice('w')));
        mark_f.lcdNumber_3->display(int(market->getPrice('s')));
        mark_f.lcdNumber_4->display(int(market->getPrice('f')));
        mark_f.lcdNumber_5->display(int(market->getPrice('m')));
        mark_f.lcdNumber_6->display(int(market->getPrice('h')));
        mark_f.lcdNumber_7->display(int(market->getPrice('l')));
        mark_f.lcdNumber_8->display(int(market->getPrice('b')));
        mark_f.lcdNumber_9->display(int(market->getPrice('i')));

        uint all_res[8]={0,0,0,0,0,0,0,0};
        getResInStore(all_res);
        mark_f.lcdNumber_10->display(int(all_res[0]));
        mark_f.lcdNumber_11->display(int(all_res[1]));
        mark_f.lcdNumber_12->display(int(all_res[2]));
        mark_f.lcdNumber_13->display(int(all_res[3]));
        mark_f.lcdNumber_14->display(int(all_res[4]));
        mark_f.lcdNumber_15->display(int(all_res[5]));
        mark_f.lcdNumber_16->display(int(all_res[6]));
        mark_f.lcdNumber_17->display(int(all_res[7]));


        connect(mark_f.pushButton, SIGNAL(clicked()), this, SLOT(buySlw()));
        connect(mark_f.pushButton_3, SIGNAL(clicked()),this, SLOT(buySls()));
        connect(mark_f.pushButton_4, SIGNAL(clicked()),this, SLOT(buySlf()));
        connect(mark_f.pushButton_5, SIGNAL(clicked()),this, SLOT(buySlm()));
        connect(mark_f.pushButton_6, SIGNAL(clicked()),this, SLOT(buySlh()));
        connect(mark_f.pushButton_7, SIGNAL(clicked()),this, SLOT(buySll()));
        connect(mark_f.pushButton_8, SIGNAL(clicked()),this, SLOT(buySlb()));
        connect(mark_f.pushButton_9, SIGNAL(clicked()),this, SLOT(buySli()));


        connect(mark_f.pushButton_2, SIGNAL(clicked()), this, SLOT(sellSlw()));
        connect(mark_f.pushButton_10, SIGNAL(clicked()),this, SLOT(sellSls()));
        connect(mark_f.pushButton_11, SIGNAL(clicked()),this, SLOT(sellSlf()));
        connect(mark_f.pushButton_12, SIGNAL(clicked()),this, SLOT(sellSlm()));
        connect(mark_f.pushButton_13, SIGNAL(clicked()),this, SLOT(sellSlh()));
        connect(mark_f.pushButton_14, SIGNAL(clicked()),this, SLOT(sellSll()));
        connect(mark_f.pushButton_15, SIGNAL(clicked()),this, SLOT(sellSlb()));
        connect(mark_f.pushButton_16, SIGNAL(clicked()),this, SLOT(sellSli()));

        connect(mark_f.pushButton, SIGNAL(clicked()), this, SLOT(updt_mark_dia()));
        connect(mark_f.pushButton_3, SIGNAL(clicked()), this, SLOT(updt_mark_dia()));
        connect(mark_f.pushButton_4, SIGNAL(clicked()), this, SLOT(updt_mark_dia()));
        connect(mark_f.pushButton_5, SIGNAL(clicked()), this, SLOT(updt_mark_dia()));
        connect(mark_f.pushButton_6, SIGNAL(clicked()), this, SLOT(updt_mark_dia()));
        connect(mark_f.pushButton_7, SIGNAL(clicked()), this, SLOT(updt_mark_dia()));
        connect(mark_f.pushButton_8, SIGNAL(clicked()), this, SLOT(updt_mark_dia()));
        connect(mark_f.pushButton_9, SIGNAL(clicked()), this, SLOT(updt_mark_dia()));

        connect(mark_f.pushButton_2, SIGNAL(clicked()), this, SLOT(updt_mark_dia()));
        connect(mark_f.pushButton_10, SIGNAL(clicked()), this, SLOT(updt_mark_dia()));
        connect(mark_f.pushButton_11, SIGNAL(clicked()), this, SLOT(updt_mark_dia()));
        connect(mark_f.pushButton_12, SIGNAL(clicked()), this, SLOT(updt_mark_dia()));
        connect(mark_f.pushButton_13, SIGNAL(clicked()), this, SLOT(updt_mark_dia()));
        connect(mark_f.pushButton_14, SIGNAL(clicked()), this, SLOT(updt_mark_dia()));
        connect(mark_f.pushButton_15, SIGNAL(clicked()), this, SLOT(updt_mark_dia()));
        connect(mark_f.pushButton_16, SIGNAL(clicked()), this, SLOT(updt_mark_dia()));

        mark_dia->exec();
        delete market;
    } else {
        QMessageBox::warning(this, tr("Error"), tr("No trader"));

    }
}

void Game::nextDay(){
    inf_wgt.progressBar->setValue(0);
    int temp_day = int(inf_wgt.lcdNumber_2->value());
    temp_day++;
    days++;// увеличиваем общее количество дней
    inf_wgt.lcdNumber_2->display(temp_day);
    recalc();
    if (temp_day % 3 == 0){
        traderCame();
        inf_wgt.label_5->setText(QString("Yes"));
    }
    else{
        traderLeft();
        inf_wgt.label_5->setText(QString("No"));
    }
}

void Game::countDay(){
    if (paused==0){
        int temp_val = inf_wgt.progressBar->value();
        if (temp_val >= 60){
            inf_wgt.progressBar->setValue(0);
            int temp_day = int(inf_wgt.lcdNumber_2->value());
            temp_day++;
            days++;// увеличиваем общее количество дней
            inf_wgt.lcdNumber_2->display(temp_day);
            recalc();
            if (temp_day % 3 == 0){ // контролирует торговца
                traderCame();
                inf_wgt.label_5->setText(QString("Yes"));
            }
            else{
                traderLeft();
                inf_wgt.label_5->setText(QString("No"));
            }
        }
        else{
            if(temp_val+(1*speed_k)>60){ // если ползунок передвинули когда день еще идет
                inf_wgt.progressBar->setValue(60);
                countDay();
            } else
            inf_wgt.progressBar->setValue(temp_val+(1*speed_k));
        }
    }
}

void Game::okIsClickedCreateDia(){
    is_ok_create_dia = 1;
}

void Game::create(int x, int y){
    QDialog* create_dia = new QDialog();
    //create_dia->setAttribute(Qt::WA_DeleteOnClose);
    create_d.setupUi(create_dia);
    connect(create_d.ok, SIGNAL(clicked()), this, SLOT(okIsClickedCreateDia()));
    create_dia->exec();
    if (is_ok_create_dia == 1){
        is_ok_create_dia = 0;
        uint level;
        if (create_d.level1->isChecked()) level=1;
        else if (create_d.level2->isChecked()) level=2;
        else if (create_d.level3->isChecked()) level=3;

        if (create_d.item1->isChecked()){
            build_tp = 0;
            build_hut_tp = 0;
            build_level = 1;
        }
        else if (create_d.item1_2->isChecked()){
            build_tp = 1;
            build_hut_tp = 0;
            build_level = level;
        }
        else if (create_d.item1_3->isChecked()){
            build_tp = 2;
            build_hut_tp = 0;
            build_level = 1;
        }
        else if (create_d.item1_4->isChecked()){
            build_tp = 3;
            build_hut_tp = 1;
            build_level = level;
        }
        else if (create_d.item1_5->isChecked()){
            build_tp = 3;
            build_hut_tp = 2;
            build_level = level;
        }
        else if (create_d.item1_6->isChecked()){
            build_tp = 3;
            build_hut_tp = 3;
            build_level = level;
        }
        else if (create_d.item1_7->isChecked()){
            build_tp = 3;
            build_hut_tp = 4;
            build_level = level;
        }
        else if (create_d.item1_8->isChecked()){
            build_tp = 3;
            build_hut_tp = 5;
            build_level = level;
        }
        else if (create_d.item1_9->isChecked()){
            build_tp = 3;
            build_hut_tp = 6;
            build_level = level;
        }
        else if (create_d.item1_10->isChecked()){
            build_tp = 3;
            build_hut_tp = 7;
            build_level = level;
        }
        else if (create_d.item1_11->isChecked()){
            build_tp = 3;
            build_hut_tp = 8;
            build_level = level;
        }
        else if (create_d.item1_12->isChecked()){
            build_tp = 3;
            build_hut_tp = 9;
            build_level = level;
        }
        buildModeOn(); // включаем строительный режим
    }
}

void Game::keyPressEvent(QKeyEvent * event_press){
    qDebug() << "In KEYPRESSEVENT   Key: " << event_press->key();
    if(event_press->key() == Qt::Key_Escape){
        qDebug() << "ESC is pressed";
        emit reject_find_place();
    }
}

void Game::keyReleasedEvent(QKeyEvent * event_released){
    qDebug() << "Key RELEASED";
    event_released->accept();
}

void Game::buildModeOff(){
    build_mode = 0;
    rect->setGeometry(0,0,0,0); // убираем с поля прямоугольник
    disconnect(field, SIGNAL(mouse_move(int, int)), this, SLOT(drawRect(int, int)));
    disconnect(rect, SIGNAL(mouse_move_rect(int,int)), this, SLOT(drawRectMoveRight(int,int)));
    disconnect(rect, SIGNAL(mouse_press_rect()), this ,SLOT(createObj()));
}

void Game::buildModeOn(){
    build_mode = 1;
    rect->setPixmap(QPixmap(":/images/rect.png"));
    connect(field, SIGNAL(mouse_move(int, int)), this, SLOT(drawRect(int, int)));
    connect(rect, SIGNAL(mouse_move_rect(int,int)), this, SLOT(drawRectMoveRight(int,int)));
    connect(rect, SIGNAL(mouse_press_rect()), this ,SLOT(createObj()));
}

void Game::recalcStatistic(uint tp, uint hut_tp, uint lv){
    uint empl = 0;
    if (tp == 0 || (tp == 3 && hut_tp == 1)){
        ifstream input;
        input.open("Setting/Hut/LevAndEmpl.txt");//путь к файлу с к-вом рабочих на уровнях
        uint temp_lev = 0, temp_empl = 0;
        while(!input.eof()){
            input >> temp_lev >> temp_empl;
            if (temp_lev == lv){
                empl = temp_empl;
                break;
            }
            //!!! проверка на несуществующую запись в файле
        }
        input.close();
    }

    switch(tp){
    case 0:{
        total_employers = total_employers + empl;
        break;
    }
    case 1:{
        storages++;
        break;
    }
    case 3:{
        if (hut_tp == 1){
            settlements++;
            total_employers = total_employers + empl;
        } else {
            hut_objects++;
        }
        break;
    }
    default :{
        break;
    }
    }
}

void Game::createObj(){
    buildModeOff();
    bool is_removed=0;
    if((build_tp==0 && cast_exist) || (build_tp==2 && market_exist)){ // случай, когда замок или рынок уже есть, тогда не снимаем ресурсы
        is_removed=1;

    } else {
        // строим временный объект такой же как и постоянный(нужен для получения информации о нужных для строительства ресурсах)
        Object* tmp_obj;
        switch(build_tp){
        case 0:{
            tmp_obj=new Castle();
            break;
        }
        case 1:{
            tmp_obj=new Storage(build_level);
            break;
        }
        case 2:{
            tmp_obj=new Market();
            break;
        }
        case 3:{
            tmp_obj=new Hut(build_hut_tp, build_level);
            break;
        }
        }

        // получаем нужные ресурсы
        uint nr_mass[8];
        Resource* nr=tmp_obj->getNeedRes();
        nr->getRes(nr_mass);
        delete tmp_obj; // удаляем временный объект

        if (build_tp != 0){ // если не замок
            // отображаем нужные ресурсы
            QDialog* req_dia = new QDialog;
            req_dia->setAttribute(Qt::WA_DeleteOnClose);
            req_r.setupUi(req_dia);
            req_r.lcdNumber->display(int(nr_mass[0]));
            req_r.lcdNumber_2->display(int(nr_mass[1]));
            req_r.lcdNumber_3->display(int(nr_mass[7]));
            connect(req_r.ok, SIGNAL(clicked()), this, SLOT(okIsClicked()));
            req_dia->exec();
        } else { // если замок
            okIsClicked();
        }

        if (is_ok == 1){
            //ищем откуда снять нужные ресурсы
            ObjElem* temp_curr_elem=head_obj;
            uint temp_type;
            while (temp_curr_elem){
                temp_type=temp_curr_elem->getType();
                switch(temp_type){
                case 0:{ // замок
                    Castle* castle=(Castle*) temp_curr_elem->obj;
                    uint sr_mass[8];
                    castle->getCStoreRes(sr_mass);
                    if (sr_mass[0]>=nr_mass[0] && sr_mass[1]>=nr_mass[1] && sr_mass[7]>=nr_mass[7]){
                        castle->takeCStoreRes('w',nr_mass[0]);
                        castle->takeCStoreRes('s',nr_mass[1]);
                        castle->takeCStoreRes('i',nr_mass[7]);
                        is_removed=1;
                    }
                    break;
                }
                case 1:{ // склад
                    Storage* store=(Storage*) temp_curr_elem->obj;
                    uint sr_mass[8];
                    store->getStoreRes(sr_mass);
                    if (sr_mass[0]>=nr_mass[0] && sr_mass[1]>=nr_mass[1] && sr_mass[7]>=nr_mass[7]){
                        store->takeStoreRes('w',nr_mass[0]);
                        store->takeStoreRes('s',nr_mass[1]);
                        store->takeStoreRes('i',nr_mass[7]);
                        is_removed=1;
                    }
                    break;
                }
                }


                temp_curr_elem=temp_curr_elem->next;
            }

        }
    }

    if (is_ok == 1){
        // создание объекта
        if (is_removed || (first_cast && build_tp==0)){ // если ресурсы сняты, то создаем объект

            ObjElem* add_obj;
            if (head_obj==0){ // если список объектов пуст
                if (build_tp==0){ // если замок
                    if (cast_exist==0){ // если замка пока нет
                        add_obj=new ObjElem(build_tp, build_hut_tp, build_level, 1, curr_x, curr_y, field);
                        recalcStatistic(build_tp, build_hut_tp, build_level);
                        head_obj=add_obj;
                        first_cast=0;
                        cast_exist=1;
                        connect(add_obj, SIGNAL(click_to_obj2(uint)), this, SLOT(funcObj(uint)));

                    } else { // если замок уже есть
                       QMessageBox::warning(this, tr("Error"), tr("Castle already exists"));
                    }

                } else {
                    add_obj=new ObjElem(build_tp, build_hut_tp, build_level, 1, curr_x, curr_y, field);
                    recalcStatistic(build_tp, build_hut_tp, build_level);
                    head_obj=add_obj;
                    connect(add_obj, SIGNAL(click_to_obj2(uint)), this, SLOT(funcObj(uint)));
                }

            }
            else{ // если список объектов не пустой
                if (build_tp==0){ // если замок
                    if (cast_exist==0){
                        ObjElem* curr_elem=head_obj;
                        uint i=2;
                        while(curr_elem->next){
                            curr_elem=curr_elem->next;
                            i++;
                        }
                        add_obj=new ObjElem(build_tp, build_hut_tp, build_level, i, curr_x, curr_y, field);
                        recalcStatistic(build_tp, build_hut_tp, build_level);
                        curr_elem->next=add_obj;
                        cast_exist=1;
                        connect(add_obj, SIGNAL(click_to_obj2(uint)), this, SLOT(funcObj(uint)));
                    } else {
                        QMessageBox::warning(this, tr("Error"), tr("Castle already exists"));
                    }


                } else {
                    if (build_tp==2 && market_exist){ // если рынок уже построен
                        QMessageBox::warning(this, tr("Error"), tr("Market already exists"));
                    }
                    else {
                        if (build_tp==2)
                            market_exist=1;
                        ObjElem* curr_elem=head_obj;
                        uint i=2;
                        while(curr_elem->next){
                            curr_elem=curr_elem->next;
                            i++;
                        }
                        add_obj=new ObjElem(build_tp, build_hut_tp, build_level, i, curr_x, curr_y, field);
                        recalcStatistic(build_tp, build_hut_tp, build_level);
                        curr_elem->next=add_obj;
                        connect(add_obj, SIGNAL(click_to_obj2(uint)), this, SLOT(funcObj(uint)));
                    }
                }

            }



        } else
            QMessageBox::warning(this, tr("Error"), tr("Not enough resources in storages"));
        is_ok = 0;
    }
}

bool Game::buy(char tc, uint val){
    // !!! проверить метку

    bool finded=0;
    // ищем рынок или тогровца
    ObjElem* curr_obj_mark=head_obj;
    while (curr_obj_mark){ // ищем рынок
        if (curr_obj_mark->type==2){
            finded=1;
            break;
        }
    curr_obj_mark=curr_obj_mark->next;
    }
    if (finded==0 && trader==0){
        QMessageBox::warning(this, tr("Error"), tr("Market is not found and no trader <br> You can build a market for buying and selling resources or wait for the trader"));
        return 0;
    }
    else{

        Market* market;
        bool temp_mark=0; // нужно для определения, когда есть рынок, и когда его надо создать временно(торговец приехал)
        if(trader==1 && finded==0){ // нашли торговца
            temp_mark=1;
            market=new Market();
        }
        else // нашли рынок
            market=(Market*) curr_obj_mark->obj;
        ObjElem* curr_obj=head_obj;
        finded=0;
        while (curr_obj){
            switch(curr_obj->type){ // определяем объект: замок или склад
            case 0:{ // если замок
                Castle* castle=(Castle*)curr_obj->obj;
                uint cost=market->getPrice(tc);
                if (cash>=cost*val && castle->addCStoreRes(tc, val)){
                    cash=cash-cost*val; // списание денежных средств

                    //вычисление новых max_cash и min_cash
                    if (cash > max_cash){
                        max_cash = cash;
                    } else {
                        if (cash < min_cash){
                            min_cash = cash;
                        }
                    }

                    inf_wgt.lcdNumber->display(int(cash)); // обновляем индикатор
                    if (temp_mark==1) delete market; // очищаем память от временного рынка
                    return 1;
                }
                else{

                    if (cash<cost*val){
                        QMessageBox::warning(this, tr("Error"), tr("Not enough money"));
                        if (temp_mark==1) delete market; // очищаем память от временного рынка
                        return 0;
                    }
                }

                break;
            }
            case 1:{ // если склад
                Storage* store=(Storage*)curr_obj->obj;
                uint cost=market->getPrice(tc);
                if (cash>=cost*val && store->addStoreRes(tc, val)){
                    cash=cash-cost*val; // списание денежных средств

                    //вычисление новых max_cash и min_cash
                    if (cash > max_cash){
                        max_cash = cash;
                    } else {
                        if (cash < min_cash){
                            min_cash = cash;
                        }
                    }

                    inf_wgt.lcdNumber->display(int(cash)); // обновляем индикатор
                    if (temp_mark==1) delete market; // очищаем память от временного рынка
                    return 1;
                }
                    else{
                    if (cash<cost*val){
                        QMessageBox::warning(this, tr("Error"), tr("Not enough money"));
                        if (temp_mark==1) delete market; // очищаем память от временного рынка
                        return 0;
                    }
                }

                break;
            }
            }

            curr_obj=curr_obj->next;
        }

        if (finded==0){ // если ничего не найдено: ни склада, ни замка
            QMessageBox::warning(this, tr("Error"), tr("Storage or castle is not found <br> You can build this building <br> or <br> Storages or castle is full <br> You can free them, selling or using resources"));
            if (temp_mark==1) delete market; // очищаем память от временного рынка
            return 0;
        }
    }
}

bool Game::sell(char tc, uint val){
    // !!! проверить метку
    bool finded=0;
    ObjElem* curr_obj_mark=head_obj;
    while (curr_obj_mark){ // ищем рынок
        if (curr_obj_mark->type==2){
            finded=1;
            break;
        }
    curr_obj_mark=curr_obj_mark->next;
    }
    if (finded==0 && trader==0){
        QMessageBox::warning(this, tr("Error"), tr("Market is not found and no trader <br> You can build a market for buying and selling resources or wait for the trader"));
        return 0;
    }
    else{
        Market* market;
        bool temp_mark=0; // нужно для определения, когда есть рынок, и когда его надо создать временно(торговец приехал)
        if(trader==1 && finded==0){
            temp_mark=1;
            market=new Market(); // временный рынок (праобраз торговца)
        }
        else
            market=(Market*) curr_obj_mark->obj;
        ObjElem* curr_obj=head_obj;
        finded=0;
        while (curr_obj){ // ищем склад или замок
            switch(curr_obj->type){
            case 0:{ // мы имеем замок
                Castle* castle=(Castle*) curr_obj->obj;
                uint cost=market->getPrice(tc);
                if (castle->takeCStoreRes(tc, val)){
                    cash=cash+val*cost; // зачисление денежных средств

                    //вычисление новых max_cash и min_cash
                    if (cash > max_cash){
                        max_cash = cash;
                    } else {
                        if (cash < min_cash){
                            min_cash = cash;
                        }
                    }

                    inf_wgt.lcdNumber->display(int(cash)); // обновляем индикатор
                    if (temp_mark==1) delete market;
                    return 1;
                }

                break;
            }
            case 1:{ // мы имеем склад
                Storage* store=(Storage*) curr_obj->obj;
                uint cost=market->getPrice(tc);
                if (store->takeStoreRes(tc, val)){
                    cash=cash+val*cost;

                    //вычисление новых max_cash и min_cash
                    if (cash > max_cash){
                        max_cash = cash;
                    } else {
                        if (cash < min_cash){
                            min_cash = cash;
                        }
                    }

                    inf_wgt.lcdNumber->display(int(cash)); // обновляем индикатор
                    if (temp_mark==1) delete market;
                    return 1;
                }

                break;
            }
            }


            curr_obj=curr_obj->next;
        }
        if (finded==0){
            QMessageBox::warning(this, tr("Error"), tr("Storage or Castle is not found <br> You can build a Storage or a Castle for storage resources <br> or <br> Not enough resources in storages <br> You can make or buy resources "));
            if (temp_mark==1) delete market; // очищаем память от временного рынка
            return 0;
        }
    }
}

void Game::traderCame(){
    trader=1;
}

void Game::traderLeft(){
    trader=0;
}

uint Game::getCash(){
    return cash;
}

void Game::getResInStore(uint *res){
    ObjElem* curr_obj = head_obj;
    uint temp_type;
    uint temp_res[8]={0,0,0,0,0,0,0,0};
    while(curr_obj){
        temp_type = curr_obj->getType();
        switch(temp_type){
        case 0:{ // замок
            Object* object = curr_obj->obj;
            Castle* castle = (Castle*) object;

            castle->getCStoreRes(temp_res);
            res[0]=res[0]+temp_res[0];
            res[1]=res[1]+temp_res[1];
            res[2]=res[2]+temp_res[2];
            res[3]=res[3]+temp_res[3];
            res[4]=res[4]+temp_res[4];
            res[5]=res[5]+temp_res[5];
            res[6]=res[6]+temp_res[6];
            res[7]=res[7]+temp_res[7];
            break;
        }
        case 1:{ // склад
            Object* object = curr_obj->obj;
            Storage* storage = (Storage*) object;
            storage->getStoreRes(temp_res);
            res[0]=res[0]+temp_res[0];
            res[1]=res[1]+temp_res[1];
            res[2]=res[2]+temp_res[2];
            res[3]=res[3]+temp_res[3];
            res[4]=res[4]+temp_res[4];
            res[5]=res[5]+temp_res[5];
            res[6]=res[6]+temp_res[6];
            res[7]=res[7]+temp_res[7];
            break;
        }
        }
        curr_obj = curr_obj->next;
    }


}

void Game::buySl(char tic, uint val){
    buy(tic, val);
        //QMessageBox::about(this, tr("Control message"), tr("Buy"));
}

void Game::sellSl(char tic, uint val){
    sell(tic, val);
        //QMessageBox::about(this, tr("Control message"), tr("Sell"));
}

void Game::updt_empl_dia(Hut* t_hut){
    uint empl[3];
    t_hut->getEmpl(empl);
    hut_f.lcdNumber_11->display(int(empl[1]));
    hut_f.lcdNumber_10->display(int(empl[2]));
    hut_f.progressBar->setMaximum(empl[0]);
    hut_f.progressBar->setValue(empl[1]);
}

void Game::updt_mark_dia(){
    uint all_res[8]={0,0,0,0,0,0,0,0};
    getResInStore(all_res);
    mark_f.lcdNumber_10->display(int(all_res[0]));
    mark_f.lcdNumber_11->display(int(all_res[1]));
    mark_f.lcdNumber_12->display(int(all_res[2]));
    mark_f.lcdNumber_13->display(int(all_res[3]));
    mark_f.lcdNumber_14->display(int(all_res[4]));
    mark_f.lcdNumber_15->display(int(all_res[5]));
    mark_f.lcdNumber_16->display(int(all_res[6]));
    mark_f.lcdNumber_17->display(int(all_res[7]));
}

void Game::funcObj(uint num){
    ObjElem* curr_obj=head_obj;
    uint temp_type;
    if (curr_obj==0) QMessageBox::warning(this, tr("Error"), tr("The list of building is empty"));
    else{
        uint i=1;
        while (i<num){
            curr_obj=curr_obj->next;
            i++;
        }
        temp_type=curr_obj->getType();
        switch(temp_type){
        case 0:{
            Object* object=curr_obj->obj;
            Castle* castle=(Castle*) object; // приведение объекта баз. кл. к производному
            QDialog* cast_dia = new QDialog();
            cast_dia->setAttribute(Qt::WA_DeleteOnClose);
            cast_f.setupUi(cast_dia);
            cast_f.lcdNumber->display(int(castle->getStoreSize()));
            uint mas[8];
            castle->getCStoreRes(mas);
            cast_f.lcdNumber_2->display(int(mas[0]));
            cast_f.lcdNumber_3->display(int(mas[1]));
            cast_f.lcdNumber_4->display(int(mas[2]));
            cast_f.lcdNumber_5->display(int(mas[3]));
            cast_f.lcdNumber_6->display(int(mas[4]));
            cast_f.lcdNumber_7->display(int(mas[5]));
            cast_f.lcdNumber_8->display(int(mas[6]));
            cast_f.lcdNumber_9->display(int(mas[7]));
            uint empl[3];
            castle->getCEmpl(empl);
            cast_f.lcdNumber_11->display(int(empl[1]));
            cast_f.lcdNumber_10->display(int(empl[2]));
            cast_f.progressBar->setMaximum(empl[0]);
            cast_f.progressBar->setValue(empl[1]);
            cast_dia->exec();
            break;
        }
        case 1:{
            Object* object=curr_obj->obj;
            Storage* storage=(Storage*) object; // приведение объекта баз. кл. к производному

            QDialog* store_dia = new QDialog();
            store_dia->setAttribute(Qt::WA_DeleteOnClose);
            store_f.setupUi(store_dia);
            store_f.lcdNumber_10->display(int(storage->getLevel()));
            store_f.lcdNumber->display(int(storage->getSize()));
            uint mas[8];
            storage->getStoreRes(mas);
            store_f.lcdNumber_2->display(int(mas[0]));
            store_f.lcdNumber_3->display(int(mas[1]));
            store_f.lcdNumber_4->display(int(mas[2]));
            store_f.lcdNumber_5->display(int(mas[3]));
            store_f.lcdNumber_6->display(int(mas[4]));
            store_f.lcdNumber_7->display(int(mas[5]));
            store_f.lcdNumber_8->display(int(mas[6]));
            store_f.lcdNumber_9->display(int(mas[7]));
            store_dia->exec();
            break;
        }
        case 2:{
            Object* object=curr_obj->obj;
            Market* market=(Market*) object; // приведение объекта баз. кл. к производному
            QDialog* mark_dia = new QDialog();
            mark_dia->setAttribute(Qt::WA_DeleteOnClose);
            mark_f.setupUi(mark_dia);
            mark_f.lcdNumber_2->display(int(market->getPrice('w')));
            mark_f.lcdNumber_3->display(int(market->getPrice('s')));
            mark_f.lcdNumber_4->display(int(market->getPrice('f')));
            mark_f.lcdNumber_5->display(int(market->getPrice('m')));
            mark_f.lcdNumber_6->display(int(market->getPrice('h')));
            mark_f.lcdNumber_7->display(int(market->getPrice('l')));
            mark_f.lcdNumber_8->display(int(market->getPrice('b')));
            mark_f.lcdNumber_9->display(int(market->getPrice('i')));

            uint all_res[8]={0,0,0,0,0,0,0,0};
            getResInStore(all_res);
            mark_f.lcdNumber_10->display(int(all_res[0]));
            mark_f.lcdNumber_11->display(int(all_res[1]));
            mark_f.lcdNumber_12->display(int(all_res[2]));
            mark_f.lcdNumber_13->display(int(all_res[3]));
            mark_f.lcdNumber_14->display(int(all_res[4]));
            mark_f.lcdNumber_15->display(int(all_res[5]));
            mark_f.lcdNumber_16->display(int(all_res[6]));
            mark_f.lcdNumber_17->display(int(all_res[7]));


            connect(mark_f.pushButton, SIGNAL(clicked()), this, SLOT(buySlw()));
            connect(mark_f.pushButton_3, SIGNAL(clicked()),this, SLOT(buySls()));
            connect(mark_f.pushButton_4, SIGNAL(clicked()),this, SLOT(buySlf()));
            connect(mark_f.pushButton_5, SIGNAL(clicked()),this, SLOT(buySlm()));
            connect(mark_f.pushButton_6, SIGNAL(clicked()),this, SLOT(buySlh()));
            connect(mark_f.pushButton_7, SIGNAL(clicked()),this, SLOT(buySll()));
            connect(mark_f.pushButton_8, SIGNAL(clicked()),this, SLOT(buySlb()));
            connect(mark_f.pushButton_9, SIGNAL(clicked()),this, SLOT(buySli()));


            connect(mark_f.pushButton_2, SIGNAL(clicked()), this, SLOT(sellSlw()));
            connect(mark_f.pushButton_10, SIGNAL(clicked()),this, SLOT(sellSls()));
            connect(mark_f.pushButton_11, SIGNAL(clicked()),this, SLOT(sellSlf()));
            connect(mark_f.pushButton_12, SIGNAL(clicked()),this, SLOT(sellSlm()));
            connect(mark_f.pushButton_13, SIGNAL(clicked()),this, SLOT(sellSlh()));
            connect(mark_f.pushButton_14, SIGNAL(clicked()),this, SLOT(sellSll()));
            connect(mark_f.pushButton_15, SIGNAL(clicked()),this, SLOT(sellSlb()));
            connect(mark_f.pushButton_16, SIGNAL(clicked()),this, SLOT(sellSli()));

            connect(mark_f.pushButton, SIGNAL(clicked()), this, SLOT(updt_mark_dia()));
            connect(mark_f.pushButton_3, SIGNAL(clicked()), this, SLOT(updt_mark_dia()));
            connect(mark_f.pushButton_4, SIGNAL(clicked()), this, SLOT(updt_mark_dia()));
            connect(mark_f.pushButton_5, SIGNAL(clicked()), this, SLOT(updt_mark_dia()));
            connect(mark_f.pushButton_6, SIGNAL(clicked()), this, SLOT(updt_mark_dia()));
            connect(mark_f.pushButton_7, SIGNAL(clicked()), this, SLOT(updt_mark_dia()));
            connect(mark_f.pushButton_8, SIGNAL(clicked()), this, SLOT(updt_mark_dia()));
            connect(mark_f.pushButton_9, SIGNAL(clicked()), this, SLOT(updt_mark_dia()));

            connect(mark_f.pushButton_2, SIGNAL(clicked()), this, SLOT(updt_mark_dia()));
            connect(mark_f.pushButton_10, SIGNAL(clicked()), this, SLOT(updt_mark_dia()));
            connect(mark_f.pushButton_11, SIGNAL(clicked()), this, SLOT(updt_mark_dia()));
            connect(mark_f.pushButton_12, SIGNAL(clicked()), this, SLOT(updt_mark_dia()));
            connect(mark_f.pushButton_13, SIGNAL(clicked()), this, SLOT(updt_mark_dia()));
            connect(mark_f.pushButton_14, SIGNAL(clicked()), this, SLOT(updt_mark_dia()));
            connect(mark_f.pushButton_15, SIGNAL(clicked()), this, SLOT(updt_mark_dia()));
            connect(mark_f.pushButton_16, SIGNAL(clicked()), this, SLOT(updt_mark_dia()));


            mark_dia->exec();
            break;
        }
        case 3:{
            Object* object=curr_obj->obj;
            Hut* hut=(Hut*) object; // приведение объекта баз. кл. к производному
            uint temp_htype=hut->getType();


            if (temp_htype==1){ // если поселение
                QDialog* settl_dia = new QDialog();
                settl_dia->setAttribute(Qt::WA_DeleteOnClose);
                settl_f.setupUi(settl_dia);
                uint empl[3];
                hut->getEmpl(empl);
                settl_f.lcdNumber_11->display(int(empl[1]));
                settl_f.lcdNumber_10->display(int(empl[2]));
                settl_f.progressBar->setMaximum(empl[0]);
                settl_f.progressBar->setValue(empl[1]);
                settl_dia->exec();
            }
            else { //если действующие объекты
            QDialog* hut_dia = new QDialog();
            hut_dia->setAttribute(Qt::WA_DeleteOnClose);
            hut_f.setupUi(hut_dia);
            switch(temp_htype){
            case 2:{
                hut_f.label->setText(QString("Cutting Hut"));
                hut_f.label_3->setPixmap(QPixmap(":/images/wood.png"));
                break;
            }
            case 3:{
                hut_f.label->setText(QString("Mine"));
                hut_f.label_3->setPixmap(QPixmap(":/images/steel.png"));
                break;
            }
            case 4:{
                hut_f.label->setText(QString("Fishing Hut"));
                hut_f.label_3->setPixmap(QPixmap(":/images/fish.png"));
                break;
            }
            case 5:{
                hut_f.label->setText(QString("Mill"));
                hut_f.label_3->setPixmap(QPixmap(":/images/flour.png"));
                hut_f.pushButton_3->setEnabled(true);
                break;
            }
            case 6:{
                hut_f.label->setText(QString("Bakery"));
                hut_f.label_3->setPixmap(QPixmap(":/images/bread.png"));
                hut_f.pushButton_3->setEnabled(true);
                break;
            }
            case 7:{
                hut_f.label->setText(QString("Smithy"));
                hut_f.label_3->setPixmap(QPixmap(":/images/instrument.png"));
                hut_f.pushButton_3->setEnabled(true);
                break;
            }
            case 8:{
                hut_f.label->setText(QString("Field"));
                hut_f.label_3->setPixmap(QPixmap(":/images/wheat.png"));
                break;
            }
            case 9:{
                hut_f.label->setText(QString("Piggery"));
                hut_f.label_3->setPixmap(QPixmap(":/images/meat.png"));
                break;
            }
            }

            uint empl[3];
            hut->getEmpl(empl);
            hut_f.lcdNumber_11->display(int(empl[1]));
            hut_f.lcdNumber_10->display(int(empl[2]));
            hut_f.progressBar->setMaximum(empl[0]);
            hut_f.progressBar->setValue(empl[1]);

            temp_hut=hut;

            connect(hut_f.pushButton, SIGNAL(clicked()), this, SLOT(addEmplSl()));
            connect(hut_f.pushButton_2, SIGNAL(clicked()), this, SLOT(remEmplSl()));


            connect(hut_f.pushButton, SIGNAL(clicked()), hut, SLOT(upd_empl_sl()));
            connect(hut_f.pushButton_2, SIGNAL(clicked()), hut, SLOT(upd_empl_sl()));
            connect(hut, SIGNAL(upd_empl_sign(Hut*)), this, SLOT(updt_empl_dia(Hut*)));

            connect(hut_f.pushButton_3, SIGNAL(clicked()), this, SLOT(viewProduction()));

            hut_dia->exec();



            }
            break;
        }
        default :{//!!! обработать ситуацию  на всякий случай
        }
        }
    }

}

void Game::viewProduction(){
    uint h_type = temp_hut->getType();
    switch(h_type){
    case 5:{ // Mill
        QDialog* production1_dia = new QDialog();
        production1_dia->setAttribute(Qt::WA_DeleteOnClose);
        product1_d.setupUi(production1_dia);
        product1_d.label->setText(QString("Mill"));
        product1_d.label_3->setPixmap(QPixmap(":/images/wheat.png"));

        uint prod_num = 0;
        ifstream input;
        input.open("Setting/Hut/Mill/Production.txt");// path to file with resources for production

        input >> prod_num; // wheat
        input.close();
        product1_d.lcdNumber->display(int(prod_num));
        production1_dia->exec();

        break;
    }
    case 6:{ // Bakery
        QDialog* production2_dia = new QDialog();
        production2_dia->setAttribute(Qt::WA_DeleteOnClose);
        product2_d.setupUi(production2_dia);
        product2_d.label->setText(QString("Bakery"));
        product2_d.label_3->setPixmap(QPixmap(":/images/wood.png"));
        product2_d.label_5->setPixmap(QPixmap(":/images/flour.png"));

        ifstream input;
        input.open("Setting/Hut/Bakery/Production.txt");// path to file with resources for production
        uint prod_num1 = 0, prod_num2 = 0;
        input >> prod_num1; // wood
        input >> prod_num2; // flour
        input.close();

        product2_d.lcdNumber->display(int(prod_num1));
        product2_d.lcdNumber_2->display(int(prod_num2));
        production2_dia->exec();
        break;
    }
    case 7:{ // Smithy
        QDialog* production2_dia = new QDialog();
        production2_dia->setAttribute(Qt::WA_DeleteOnClose);
        product2_d.setupUi(production2_dia);
        product2_d.label->setText(QString("Smithy"));
        product2_d.label_3->setPixmap(QPixmap(":/images/wood.png"));
        product2_d.label_5->setPixmap(QPixmap(":/images/steel.png"));


        ifstream input;
        input.open("Setting/Hut/Smithy/Production.txt");// path to file with resources for production
        uint prod_num1 = 0, prod_num2 = 0;
        input >> prod_num1; // wood
        input >> prod_num2; // steel
        input.close();

        product2_d.lcdNumber->display(int(prod_num1));
        product2_d.lcdNumber_2->display(int(prod_num2));
        production2_dia->exec();
        break;
    }
    }
}

void Game::addEmpl(Hut* hut){
    ObjElem* curr_settl=head_obj;
    while(curr_settl){
        if(curr_settl->getType()==3){ // если hut
            Object* settl=curr_settl->obj;
            Hut* set=(Hut*) settl; // приведение
            if(set->getType()==1){ // если поселение
                uint empl_mas[3];
                set->getEmpl(empl_mas); // получаем данные о работниках
                if(empl_mas[2]>0){ //выходим из цикла, если свободных людей нет
                    if(hut->incEmpl()){ // добавляем рабочего в hut и в поселение
                        set->incEmpl();// работник добавлен
                        break;
                    }
                    //else
                         //cout << "Работник не добавлен" << endl;
                    break;
                }
            }
        }
        if(curr_settl->getType()==0){
            Object* castle=curr_settl->obj;
            Castle* cast=(Castle*) castle; // приведение
            uint empl_mas[3];
            cast->getCEmpl(empl_mas); // получаем данные
            if(empl_mas[2]>0){ //выходим из цикла, если свободных людей нет
                if(hut->incEmpl()){ // добавляем рабочего в hut и в замок
                    cast->incCEmpl(); // работник добавлен
                    break;
                }
                //else
                  //cout << "Работник не добавлен" << endl;
                break;
            }
        }
        curr_settl=curr_settl->next;
    }
}

void Game::remEmpl(Hut* hut){
    ObjElem* curr_settl=head_obj;
    while(curr_settl){
        if(curr_settl->getType()==3){
            Object* settl=curr_settl->obj;
            Hut* set=(Hut*) settl; // приведение
            if(set->getType()==1){
                uint empl_mas[3];
                set->getEmpl(empl_mas); // получаем данные о работниках
                if(empl_mas[0]-empl_mas[2]>0){ //выходим из цикла, если места в поселении нет
                    if(hut->decEmpl()){ // снимаем рабочего с hut  и с поселения
                        set->decEmpl(); // работник снят
                        break;
                    }
                    //else
                      //   cout << "Работник не снят" << endl;
                    break;
                }
            }
        }
        if(curr_settl->getType()==0){
            Object* castle=curr_settl->obj;
            Castle* cast=(Castle*) castle; // приведение
            uint empl_mas[3];
            cast->getCEmpl(empl_mas); // получаем данные
            if(empl_mas[0]-empl_mas[2]>0){ //выходим из цикла, если места в поселении нет
                if(hut->decEmpl()){ // снимаем рабочего с hut и с замка
                    cast->decCEmpl(); // работник снят
                    break;
                }
                //else
                     //cout << "Работник не снят" << endl;
                break;
            }
        }
        curr_settl=curr_settl->next;
    }
}

void Game::recalc(){
    ObjElem* curr_elem=head_obj;
    uint temp_type;
    uint store_full=0, all_store=0, num_full=0;
    while (curr_elem){
        store_full=0;
        all_store=0;
        temp_type=curr_elem->getType();
        switch(temp_type){
        case 0:{
            break;
        }
        case 1:{
            break;
        }
        case 2:{
            break;
        }
        case 3:{ // обрабатываем все пересчеты с hut-объектами
            Object* object=curr_elem->obj;
            Hut* hut=(Hut*) object; // приведение объекта баз. кл. к производному
            uint temp_htype=hut->getType();
            char temp_tic=NULL, temp_tic_take=NULL, temp_tic_take1=NULL;
            uint take=0, take1=0;
            uint index, take_index, take_index1;
            switch(temp_htype){
            case 1:{
                break;
            }
            case 2:{
                temp_tic='w';
                index=0;
                break;
            }
            case 3:{
                temp_tic='s';
                index=1;
                break;
            }
            case 4:{
                temp_tic='f';
                index=2;
                break;
            }
            case 5:{
                temp_tic='l';
                temp_tic_take='h';
                take_index=4;

                ifstream input;
                input.open("Setting/Hut/Mill/Production.txt");// path to file with resources for production
                input >> take; // wheat
                input.close();

                index=5;
                break;
            }
            case 6:{
                temp_tic='b';
                temp_tic_take='w';
                temp_tic_take1='l';
                take_index=0;
                take_index1=5;


                ifstream input;
                input.open("Setting/Hut/Bakery/Production.txt");// path to file with resources for production
                input >> take; // wood
                input >> take1; // flour
                input.close();

                index=6;
                break;
            }
            case 7:{
                temp_tic='i';
                temp_tic_take='w';
                temp_tic_take1='s';
                take_index=0;
                take_index1=1;

                ifstream input;
                input.open("Setting/Hut/Smithy/Production.txt");// path to file with resources for production
                input >> take; // wood
                input >> take1; // steel
                input.close();

                index=7;
                break;
            }
            case 8:{
                temp_tic='h';
                index=4;
                break;
            }
            case 9:{
                temp_tic='m';
                index=3;
                break;
            }
            }
            // ищем склады
            if(temp_tic!=NULL){ // если выбрано не поселение
                ObjElem* curr_elem1=head_obj;
                while(curr_elem1){
                    if(curr_elem1->getType()==1){ // если склад
                        all_store++;
                        Object* object1=curr_elem1->obj;
                        Storage* store=(Storage*) object1; // приведение объекта баз. кл. к производному
                        uint res[8];
                        store->getStoreRes(res);
                        uint size=store->getSize();// размер склада
                        uint empl[3];
                        hut->getEmpl(empl);
                        // 1 ресурс расчитывается по 1 работнику за день
                        if (temp_tic_take!=NULL && temp_tic_take1!=NULL){ // второй случай
                            if (res[take_index]>=take && res[take_index1]>=take1){// если ресурсы для производства можно снять

                                if (res[index]+empl[1]<=size){ // если ресурсы влазят на склад
                                    store->takeStoreRes(temp_tic_take, take*empl[1]);
                                    store->takeStoreRes(temp_tic_take1, take1*empl[1]);
                                    store->addStoreRes(temp_tic, empl[1]);
                                    break;
                                }
                                else {
                                    store_full++;
                                    //QMessageBox::warning(this, tr("Error"), tr("In stores there is no place for produced resources <br> You can build a new store or use/sell part of the resources from the existing storages"));
                                }

                            } else {
                                QMessageBox::warning(this, tr("Error"), tr("Not enough resourses for manufacturing"));
                            }
                        }

                        else if(temp_tic_take!=NULL && temp_tic_take1==NULL){ // первый случай

                            if(res[take_index]>=take){ // если есть ресурсы для производства
                                if (res[index]+empl[1]<=size){ // если ресурсы влазят на склад
                                    store->takeStoreRes(temp_tic_take, take*empl[1]);
                                    store->addStoreRes(temp_tic, empl[1]);
                                    break;
                                }
                                else {
                                    store_full++;
                                    //QMessageBox::warning(this, tr("Error"), tr("In stores there is no place for produced resources <br> You can build a new store or use/sell part of the resources from the existing storages"));
                                }
                            } else {
                                QMessageBox::warning(this, tr("Error"), tr("Not enough resourses for manufacturing"));
                            }

                        } else {
                            if (res[index]+empl[1]<=size){ // если ресурсы влазят на склад
                                store->addStoreRes(temp_tic, empl[1]);
                                break;
                            }
                            else {
                                store_full++;
                                //QMessageBox::warning(this, tr("Error"), tr("In stores there is no place for produced resources <br> You can build a new store or use/sell part of the resources from the existing storages"));
                            }

                        }



                    }
                    if(curr_elem1->getType()==0){ // если замок
                        all_store++;
                        Object* object1=curr_elem1->obj;
                        Castle* castle=(Castle*) object1; // приведение объекта баз. кл. к производному
                        uint res[8];
                        castle->getCStoreRes(res);
                        uint size=castle->getStoreSize();// размер склада
                        uint empl[3];
                        hut->getEmpl(empl);
                        // 1 ресурс расчитывается по 1 работнику за день
                        if (temp_tic_take!=NULL && temp_tic_take1!=NULL){ // второй случай
                            if (res[take_index]>=take && res[take_index1]>=take1){// если ресурсы для производства можно снять

                                if (res[index]+empl[1]<=size){ // если ресурсы влазят на склад
                                    castle->takeCStoreRes(temp_tic_take, take*empl[1]);
                                    castle->takeCStoreRes(temp_tic_take1, take1*empl[1]);
                                    castle->addCStoreRes(temp_tic, empl[1]);
                                    break;
                                }
                                else {
                                    store_full++;
                                    //QMessageBox::warning(this, tr("Error"), tr("There is no place for resources in the internal storage of the castle <br> You can build a new store or use/sell part of the resources from the existing internal storage of the castle"));
                                }

                            } else {
                                QMessageBox::warning(this, tr("Error"), tr("Not enough resourses for manufacturing"));
                            }
                        }

                        else if(temp_tic_take!=NULL && temp_tic_take1==NULL){ // первый случай

                            if(res[take_index]>=take){ // если есть ресурсы для производства
                                if (res[index]+empl[1]<=size){ // если ресурсы влазят на склад
                                    castle->takeCStoreRes(temp_tic_take, take*empl[1]);
                                    castle->addCStoreRes(temp_tic, empl[1]);
                                    break;
                                }
                                else {
                                    store_full++;
                                    //QMessageBox::warning(this, tr("Error"), tr("There is no place for resources in the internal storage of the castle <br> You can build a new store or use/sell part of the resources from the existing internal storage of the castle"));
                                }
                            } else {
                                QMessageBox::warning(this, tr("Error"), tr("Not enough resourses for manufacturing"));
                            }

                        } else {
                            if (res[index]+empl[1]<=size){ // если ресурсы влазят на склад
                                castle->addCStoreRes(temp_tic, empl[1]);
                                break;
                            }
                            else {
                                store_full++;
                               // QMessageBox::warning(this, tr("Error"), tr("There is no place for resources in the internal storage of the castle <br> You can build a new store or use/sell part of the resources from the existing internal storage of the castle"));
                            }

                        }

                    }
                    curr_elem1=curr_elem1->next;
                }
            }
            break;
        }
        }

        if (all_store!=0 && all_store==store_full){
            num_full++;
        }
        curr_elem=curr_elem->next;

}
    if (num_full>0)
        QMessageBox::warning(this, tr("Error"), tr("There is no place for resources in the internal storage of the castle or in storages <br> You can build a new store or use/sell part of the resources"));
}

void Game::saveStatistic(){
    QString url = "http://township.p.ht//save_statistic.php?login=";
    url.append(player);
    url.append("&start_difficultly=");
    url.append(start_difficulty);
    url.append("&days=");
    QString days_str = QString::number(int(days));
    url.append(days_str);
    url.append("&max_cash=");
    QString max_cash_str = QString::number(int(max_cash));
    url.append(max_cash_str);
    url.append("&min_cash=");
    QString min_cash_str = QString::number(int(min_cash));
    url.append(min_cash_str);
    url.append("&storages=");
    QString storages_str = QString::number(int(storages));
    url.append(storages_str);
    url.append("&settlements=");
    QString settlements_str = QString::number(int(settlements));
    url.append(settlements_str);
    url.append("&hut_objects=");
    QString hut_str = QString::number(int(hut_objects));
    url.append(hut_str);
    url.append("&total_employers=");
    QString empl_str = QString::number(int(total_employers));
    url.append(empl_str);

    QTime tmp_time;
    int ms = clock.elapsed();
    clock.setHMS (0, 0, 0, 0);
    tmp_time = clock.addMSecs(ms);
    time_in_game = tmp_time.toString("hhmmss");

    url.append("&time_in_game=");
    url.append(time_in_game);

    QUrl apiUrl = QUrl(url);
    QNetworkRequest request(apiUrl);
    manager = new QNetworkAccessManager();
    reply = manager->get(request);
    connect(reply, SIGNAL(readyRead()), this, SLOT(readyReplySaveStat()));
}

void Game::readyReplySaveStat(){
    QString answer = QString(reply->readAll());
    if(answer == "ok"){
        QMessageBox::information(this, tr("Debug"), tr("Data saving to database"));
        newMenu();
    } else {
        QMessageBox::warning(this, tr("Debug"), tr("Not saving"));
    }
}

