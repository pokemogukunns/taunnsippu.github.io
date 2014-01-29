#include "town.h"

MainWindow::MainWindow(bool is_logged_in_arg, uint image_arg, QString player_arg){
    temp_cash=0;
    profile_image=image_arg;
    is_logged_in=is_logged_in_arg;
    player=player_arg;
    test_game=0;
    difficultly=QString();
    is_ok_registration = 0;
    for(int i=0; i<3; i++){
        cash_tab[i]=0;
    }

    new_butt = new QPushButton(tr("New Game"), this);
    new_butt->setMaximumSize(100,25);
    connect(new_butt, SIGNAL(clicked()),SLOT(gameSett()));
    prof_butt = new QPushButton(tr("Profiles"), this);
    prof_butt->setMaximumSize(100,25);
    connect(prof_butt, SIGNAL(clicked()),SLOT(profiles()));
    exit_butt = new QPushButton(tr("Exit"), this);
    exit_butt->setMaximumSize(100,25);
    connect(exit_butt, SIGNAL(clicked()),SLOT(exit()));
    about_butt = new QPushButton(tr("About"), this);
    about_butt->setMaximumSize(100,25);
    connect(about_butt, SIGNAL(clicked()),SLOT(about()));
    help_butt = new QPushButton(tr("Help"),this);
    help_butt->setMaximumSize(100,25);
    connect(help_butt, SIGNAL(clicked()), SLOT(help()));

    backgr_wgt = new QWidget;
    backgr_wgt->setStyleSheet("background-image: url(:/images/intro.jpg);");

    vlayout = new QVBoxLayout;
    vlayout->addWidget(new_butt);
    vlayout->addWidget(prof_butt);
    vlayout->addWidget(help_butt);
    vlayout->addWidget(about_butt);
    vlayout->addWidget(exit_butt);

    hlayout = new QHBoxLayout;
    hlayout->addLayout(vlayout);
    hlayout->addWidget(backgr_wgt);
    setLayout(hlayout);
}


MainWindow::~MainWindow(){
}

void MainWindow::about(){
    QMessageBox::about(this, tr("About"), tr("Game TOWNSHIP is economic simulation in the Middle Ages<br> Version: 0.9 (beta)<br> Author: Sushkov Vladimir<br> Site with game statistic: www.township.p.ht"));
}

void MainWindow::exit(){
    if (is_logged_in){
        QString url = "http://township.p.ht//out.php?login=";
        url.append(player);
        QUrl apiUrl = QUrl(url);
        QNetworkRequest request(apiUrl);
        reply = manager.get(request);
        connect(reply, SIGNAL(readyRead()), this, SLOT(readyReplyExit()));
    } else {
        this->close();
    }

}

void MainWindow::readyReplyExit(){
    QString answer = QString(reply->readAll());
    this->close();
}

void MainWindow::help(){
    QDialog* help_dia = new QDialog();
    help_dia->setAttribute(Qt::WA_DeleteOnClose);
    help_d.setupUi(help_dia);
    help_dia->exec();
}

void MainWindow::gameSett(){
    if (is_logged_in || test_game){ // если пользователь авторизован

        QDialog* set_dia = new QDialog();
        //set_dia->setAttribute(Qt::WA_DeleteOnClose);
        game_s.setupUi(set_dia);

        // считываем размер счета по сложности
        ifstream input;
        input.open("Setting/Cash.txt");
        int i=0;
        while(!input.eof()){
            input >> cash_tab[i];
            i++;
            if (i>2) break; // сомволы с индексами больше 3 не читаем
            //!!! проверка на несуществующую запись в файле
        }
        input.close();
        game_s.lcdNumber->display(int(cash_tab[0]));
        game_s.lcdNumber_2->display(int(cash_tab[1]));
        game_s.lcdNumber_3->display(int(cash_tab[2]));



        connect(game_s.ok, SIGNAL(clicked()), this, SLOT(newGame()));
        set_dia->exec();

    } else { // если пользователь не авторизован
        QMessageBox::warning(this, tr("Warning"), tr("To start a new game you have to login"));
        profiles();
        gameSett();
    }
}

void MainWindow::okRegistration(){
    is_ok_registration = 1;
}

void MainWindow::newProf(){
    new_dia = new QDialog();
    new_prof_d.setupUi(new_dia);
    connect(new_prof_d.regist, SIGNAL(clicked()), this, SLOT(okRegistration()));
    new_dia->exec();
    if (is_ok_registration == 1){
        is_ok_registration = 0;
        if (new_prof_d.radioButton_1->isChecked()){
            profile_image = 1;
        }
        else if (new_prof_d.radioButton_2->isChecked()){
            profile_image = 2;
        }
        else if (new_prof_d.radioButton_3->isChecked()){
            profile_image = 3;
        }
        else if (new_prof_d.radioButton_4->isChecked()){
            profile_image = 4;
        }
        else if (new_prof_d.radioButton_5->isChecked()){
            profile_image = 5;
        }
        else if (new_prof_d.radioButton_6->isChecked()){
            profile_image = 6;
        }
        else if (new_prof_d.radioButton_7->isChecked()){
            profile_image = 7;
        }
        else if (new_prof_d.radioButton_8->isChecked()){
            profile_image = 8;
        }
        registration();
    }

}

void MainWindow::test_game_on(){
    test_game = 1;
    prof_dia->close();
}

void MainWindow::test_game_off(){
    test_game = 0;
    prof_dia->close();
}

void MainWindow::newGame(){
    // выбираем нужную сложность
    if (game_s.radioButton->isChecked()){
        temp_cash = cash_tab[0];
        difficultly = QString("Easy");
    }
    else if (game_s.radioButton_2->isChecked()){
        temp_cash = cash_tab[1];
        difficultly = QString("Medium");
    }
    else if (game_s.radioButton_3->isChecked()){
        temp_cash = cash_tab[2];
        difficultly = QString("Hard");
    }
    else if (game_s.radioButton_4->isChecked()){
        temp_cash = 0;
        difficultly = QString("Impossible");
    }
    Game* create_game = new Game(temp_cash, player, profile_image, difficultly, test_game);
    close();
    this->~MainWindow();
}

void MainWindow::profiles(){
    prof_dia = new QDialog();
    prof_dia->setAttribute(Qt::WA_DeleteOnClose);
    if (is_logged_in){ // если пользователь авторизован
        my_prof_d.setupUi(prof_dia);
        my_prof_d.username->setText(player);

        switch(profile_image){
        case 1:{
            my_prof_d.image->setPixmap(QPixmap(":/images/profiles_images/m_1.PNG"));
            break;
        }
        case 2:{
            my_prof_d.image->setPixmap(QPixmap(":/images/profiles_images/m_2.PNG"));
            break;
        }
        case 3:{
            my_prof_d.image->setPixmap(QPixmap(":/images/profiles_images/m_3.PNG"));
            break;
        }
        case 4:{
            my_prof_d.image->setPixmap(QPixmap(":/images/profiles_images/m_4.PNG"));
            break;
        }
        case 5:{
            my_prof_d.image->setPixmap(QPixmap(":/images/profiles_images/w_1.PNG"));
            break;
        }
        case 6:{
            my_prof_d.image->setPixmap(QPixmap(":/images/profiles_images/w_2.PNG"));
            break;
        }
        case 7:{
            my_prof_d.image->setPixmap(QPixmap(":/images/profiles_images/w_3.PNG"));
            break;
        }
        case 8:{
            my_prof_d.image->setPixmap(QPixmap(":/images/profiles_images/w_4.PNG"));
            break;
        }
        default:{
            my_prof_d.image->setPixmap(QPixmap(":/images/profiles_images/m_1.PNG"));
            break;
        }
        }
        connect(my_prof_d.logout, SIGNAL(clicked()), this, SLOT(out()));
        prof_dia->exec();
    } else { // если пользователь не авторизован
        prof_d.setupUi(prof_dia);
        connect(prof_d.new_prof_butt, SIGNAL(clicked()), this, SLOT(newProf()));
        connect(prof_d.log_butt, SIGNAL(clicked()), this, SLOT(auth()));
        connect(prof_d.test_game_butt, SIGNAL(clicked()), this, SLOT(test_game_on()));
        prof_dia->exec();
    }
}

void MainWindow::auth(){
    QString login = NULL;
    QString password = NULL;
    login = prof_d.login->text();
    password = prof_d.password->text();
    if (login == NULL || password == NULL){
        QMessageBox::warning(this, tr("Error"), tr("Fields <b>Login</b> and <b>Password</b> can not be empty <br> Please fill them"));
    } else {
        QString url = "http://township.p.ht//auth.php?login=";
        url.append(login);
        url.append("&password=");
        url.append(password);
        QUrl apiUrl = QUrl(url);
        QNetworkRequest request(apiUrl);
        player = login;
        reply = manager.get(request);
        connect(reply, SIGNAL(readyRead()), this, SLOT(readyReplyAuth()));
    }
}

void MainWindow::registration(){
    QString login = NULL;
    QString password = NULL;
    QString conf_password = NULL;
    login = new_prof_d.user->text();
    password = new_prof_d.password->text();
    conf_password = new_prof_d.conf_password->text();



    QString url = "http://township.p.ht//reg.php?login=";
    url.append(login);
    url.append("&password=");
    url.append(password);
    url.append("&password2=");
    url.append(conf_password);
    url.append("&image=");
    QString image_str = QString::number(int(profile_image));
    url.append(image_str);
    QUrl apiUrl = QUrl(url);
    QNetworkRequest request(apiUrl);
    reply = manager.get(request);
    connect(reply, SIGNAL(readyRead()), this, SLOT(readyReplyReg()));
}

void MainWindow::readyReplyReg(){
    QString answer = QString(reply->readAll());
   if(answer == "ok"){
        QMessageBox::information(this, tr("Registration"), tr("You have successfully registered"));
    } else {
        QMessageBox::warning(this, tr("Registration"), answer);
        new_dia->exec();
    }
}

void MainWindow::readyReplyAuth(){
    QString answer = QString(reply->readAll());

    if(answer == "false"){
        QMessageBox::warning(this, tr("Authorization"), tr("You are not logged in <br> Incorrect login/password, try again"));
    } else {
        is_logged_in = 1;
        test_game_off();
        if(answer == "Image:1") profile_image = 1;
        else if(answer == "Image:2") profile_image = 2;
        else if(answer == "Image:3") profile_image = 3;
        else if(answer == "Image:4") profile_image = 4;
        else if(answer == "Image:5") profile_image = 5;
        else if(answer == "Image:6") profile_image = 6;
        else if(answer == "Image:7") profile_image = 7;
        else if(answer == "Image:8") profile_image = 8;
        prof_dia->close();
        QMessageBox::information(this, tr("Authorization"), tr("You are logged in"));

    }
}

void MainWindow::out(){
    QString url = "http://township.p.ht//out.php?login=";
    url.append(player);
    QUrl apiUrl = QUrl(url);
    QNetworkRequest request(apiUrl);
    reply = manager.get(request);
    connect(reply, SIGNAL(readyRead()), this, SLOT(readyReplyOut()));
}

void MainWindow::readyReplyOut(){
    QString answer = QString(reply->readAll());
    if(answer == "true"){
        is_logged_in = 0;
        profile_image = 0;
        player = QString();
        prof_dia->close();
        QMessageBox::information(this, tr("Authorization"), tr("You are logged out"));
    }

}

