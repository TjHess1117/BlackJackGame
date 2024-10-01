#include "levelone.h"
#include "qboxlayout.h"
#include "ui_levelone.h"

LevelOne::LevelOne(QWidget *parent,QMainWindow* menu)
    : QMainWindow(parent)
    , ui(new Ui::LevelOne)
{
    ui->setupUi(this);
    mainMenu = menu;
}

LevelOne::~LevelOne()
{
    delete ui;
}

void LevelOne::on_hitButton_clicked()
{
    QPushButton* newCard = new QPushButton( ui->handArea);
    newCard->setFixedSize(125, 175);
    newCard->setStyleSheet("QPushButton {background-color: rgb(224,224,224);}");

    QHBoxLayout* layout = (QHBoxLayout*)ui->handArea->widget()->layout();
    layout->addWidget(newCard, Qt::AlignLeft);
}

void LevelOne::on_mainMenu_clicked()
{
    mainMenu->show();
    delete this;
}

