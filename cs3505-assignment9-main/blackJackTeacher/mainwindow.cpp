#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "PlayingWindow.h"
#include "model.h"
#include <iostream>
#include <ostream>

MainWindow::MainWindow(Model& model, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->model = &model;
    setupConnections(model,ui);
    setupWindowDisplay();
}



MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::levelClicked(int level){
    this->levelOneWindow = new PlayingWindow(*model,this);
    levelOneWindow->show();
    this->hide();
    emit SetLevel(level);
}
void MainWindow::hitSlot(){
    std::cout << "hit" << std::endl;
}
void MainWindow::setupConnections(Model& model,Ui::MainWindow *ui){
    connect(ui->levelOne,&QPushButton::clicked,this,[=]{this->levelClicked(1);});
    connect(ui->levelTwo,&QPushButton::clicked,this,[=]{this->levelClicked(2);});
    connect(ui->levelThree,&QPushButton::clicked,this,[=]{this->levelClicked(3);});
    connect(ui->levelFour,&QPushButton::clicked,this,[=]{this->levelClicked(4);});
    connect(this,&MainWindow::SetLevel,&model,&Model::SetLevel);
}
void::MainWindow::setupWindowDisplay(){
    QPixmap bkgnd(":/images/BlackjackWelcome.png");
    bkgnd = bkgnd.scaled(this->size(), Qt::KeepAspectRatioByExpanding);
    QPalette palette;
    palette.setBrush(QPalette::Window, bkgnd);
    // this->centralWidget()->setStyleSheet("background-image:url(\":/images/BlackjackWelcome.png\"); background-position: center; ");
    // the line of code above is cool if you run it instead of the palett stuff
    ui->welcomeText->setStyleSheet("QLabel { background-color : rgba(0,0,0,0%) ; color : white; }");
    ui->levelOne->setStyleSheet("QPushButton { background-color : rgba(100,100,100,50%) ; color : white; }");
    ui->levelTwo->setStyleSheet("QPushButton { background-color : rgba(100,100,100,50%) ; color : white; }");
    ui->levelThree->setStyleSheet("QPushButton { background-color : rgba(100,100,100,50%) ; color : white; }");
    ui->levelFour->setStyleSheet("QPushButton { background-color : rgba(100,100,100,50%) ; color : white; }");

    this->setPalette(palette);
}

