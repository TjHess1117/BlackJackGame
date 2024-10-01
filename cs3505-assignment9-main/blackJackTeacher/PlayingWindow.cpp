#include "PlayingWindow.h"
#include "ui_PlayingWindow.h"
#include "model.h"
#include "animation.h"

PlayingWindow::PlayingWindow(Model &model,QMainWindow* menu, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PlayingWindow)
{
    ui->setupUi(this);
    mainMenu = menu;
    SetUpConnections(model);
    ui->splitButton->setEnabled(false);
    ui->dealCards->setEnabled(false);
    ui->winButton->setVisible(false);
    ui->splitLabel->setVisible(false);
    ui->splitTotal->setVisible(false);
    ui->splitHand->setVisible(false);
    ui->mainHand->setVisible(false);

    // Connect timer signal to updateWorld slot
    //connect(timer, &QTimer::timeout, this, &PlayingWindow::updateWorld);
    //timer->start(1000 / 60); // 60Hz

    QPalette palette = this->palette();
    QColor bg = QColor(61, 59, 59);
    palette.setColor(QPalette::Window, bg);
    this->setAutoFillBackground(true);
    this->setPalette(palette);
    ui->ProbOfDealerBustLabel->setStyleSheet("color: rgb(255, 255, 255);");
}

PlayingWindow::~PlayingWindow()
{
    delete ui;
}

void PlayingWindow::receivedProb(double probOfDealerBust, double probOfDealerWin){
    ui->ProbOfDealerBustLabel->setText("Probability of Dealer Bust: " + QString::number(probOfDealerBust) + "%");
    ui->ProbOfDealerExceedLabel->setText("Probability of Dealer Exceeding Current Hand: " + QString::number(probOfDealerWin*100) + "%");
}


void PlayingWindow::addCardToPlayerHand(Card card, bool isSplit){
    QLabel* newCard = new QLabel( ui->handArea);
    cards.push_back(newCard);
    newCard->setFixedSize(125, 175);
    newCard->setStyleSheet("QPushButton {background-color: rgb(224,224,224);}");
    if(isSplit){
        splitLayout->addWidget(newCard,Qt::AlignLeft);
        updateCardImage(card.image);
    }
    else{
        QHBoxLayout* layout = (QHBoxLayout*)ui->handArea->widget()->layout();
        layout->addWidget(newCard, Qt::AlignLeft);
        updateCardImage(card.image);
    }

}


void PlayingWindow::addCardToDealerHand(Card card, bool isFirstCard){
    QLabel* newCard = new QLabel( ui->dealerArea);
    newCard->setFixedSize(125, 175);
    cards.push_back(newCard);
    newCard->setStyleSheet("QPushButton {background-color: rgb(224,224,224);}");

    QHBoxLayout* layout = (QHBoxLayout*)ui->dealerArea->widget()->layout();
    layout->addWidget(newCard, Qt::AlignLeft);
    if(isFirstCard){
        QImage cardBack;
        cardBack.load(":/images/cardImages/cardBack.png");
        updateCardImage(cardBack);
    }
    else{updateCardImage(card.image);}
}


void PlayingWindow::updateCardImage(QImage image){
    QLabel* cardToUpdate = cards.back();
    QPixmap pixmap = QPixmap::fromImage(image);
    cardToUpdate->setPixmap(pixmap);
    cardToUpdate->setScaledContents(true);
}
void PlayingWindow::flipDealerCard(QImage image){
    QLabel* cardToUpdate = cards.at(1);
    QPixmap pixmap = QPixmap::fromImage(image);
    cardToUpdate->setPixmap(pixmap);
    cardToUpdate->setScaledContents(true);
}

void PlayingWindow::mainMenuClicked()
{
    mainMenu->show();
    delete this;
}

void PlayingWindow::clearOldImages(){
    ui->splitLabel->setVisible(false);
    ui->splitTotal->setVisible(false);
    ui->handLayout->setStretch(0,1);
    ui->splitHand->setVisible(false);
    ui->mainHand->setVisible(false);
    for(int i = 0; i < cards.size(); i++){
        delete cards.at(i);
    }
    cards.clear();
}

void PlayingWindow::canSplit(bool enableSplit){
    if(enableSplit){
        ui->splitButton->setEnabled(true);
    }
    else{
        ui->splitButton->setEnabled(false);
    }
}

void PlayingWindow::messageRecieved(QString message)
{
    //Setlabel to the message recieved
    ui->textDisplay->setText(message);
}

void PlayingWindow::recievedLock(QString allBut)
{
    if(readingScript)
    {
        if(allBut == "stand\n")
        {
            ui->hitButton->setEnabled(false);
            ui->standButton->setEnabled(true);
            ui->nextButton->setEnabled(false);
            ui->splitButton->setEnabled(false);
        }
        if(allBut == "hit\n")
        {
            ui->hitButton->setEnabled(true);
            ui->standButton->setEnabled(false);
            ui->nextButton->setEnabled(false);
            ui->splitButton->setEnabled(false);
        }
        if(allBut == "next\n")
        {
            ui->hitButton->setEnabled(false);
            ui->standButton->setEnabled(false);
            ui->nextButton->setEnabled(true);
            ui->splitButton->setEnabled(false);
        }
        if(allBut == "split\n")
        {
            ui->hitButton->setEnabled(false);
            ui->standButton->setEnabled(false);
            ui->nextButton->setEnabled(false);
            ui->splitButton->setEnabled(true);
        }
    }
}
void PlayingWindow::unlockStand()
{
    if(readingScript)
    {
        ui->hitButton->setEnabled(false);
        ui->standButton->setEnabled(false);
        ui->nextButton->setEnabled(true);
        ui->splitButton->setEnabled(false);
    }
}

void PlayingWindow::unlockSplit()
{
    if(readingScript)
    {
        ui->hitButton->setEnabled(false);
        ui->standButton->setEnabled(false);
        ui->nextButton->setEnabled(true);
        ui->splitButton->setEnabled(false);
    }
}

void PlayingWindow::showWhichHand(bool makeVisible){
    ui->mainHand->setVisible(makeVisible);
    ui->splitHand->setVisible(!makeVisible);
}

void PlayingWindow::split(){

    // Create a layout for the new scroll area
    splitLayout = (QHBoxLayout*)ui->splitArea->widget()->layout();
    ui->handLayout->setStretch(0,0);
    ui->splitHand->setVisible(true);


    // get the card to move from the the player hand to the split hand
    QLabel* moveCard = cards.at(2);
    QHBoxLayout* moveFrom = qobject_cast<QHBoxLayout*>(ui->handArea->widget()->layout());

    // Remove the QLabel from its current layout
    moveFrom->removeWidget(moveCard);

    // Add the QLabel to the layout of the new scroll area
    splitLayout->addWidget(moveCard);

    // Disable the split button
    ui->splitButton->setEnabled(false);

    ui->splitLabel->setVisible(true);
    ui->splitTotal->setVisible(true);


}
void PlayingWindow::unlockHit()
{
    if(readingScript)
    {
        ui->hitButton->setEnabled(false);
        ui->standButton->setEnabled(false);
        ui->nextButton->setEnabled(true);
        ui->splitButton->setEnabled(false);
    }
}

void PlayingWindow::setReadingScript(bool reading)
{
    readingScript = reading;
    if(!readingScript)
        ui->nextButton->setEnabled(false);
}
//=========================== CONECTIONS =========================

void PlayingWindow::SetUpConnections(Model& model){
    //============ hit connections
    //connect(this,&PlayingWindow::hit,&model,&Model::hitSlot);
    connect(ui->hitButton,&QPushButton::clicked,&model, &Model::hitSlot);
    connect(ui->mainMenu, &QPushButton::clicked, &model, &Model::mainMenuSlot);
    connect(ui->mainMenu, &QPushButton::clicked,this,&PlayingWindow::mainMenuClicked);
    //connect(&model,&Model::SendCardImage,this,&PlayingWindow::updateCardImage);

    // ============ stand connections

    connect(&model,&Model::SendCardImage,this,&PlayingWindow::flipDealerCard);
    connect(ui->standButton, &QPushButton::clicked, &model, &Model::standSlot);
    connect(&model, &Model::disableButtons, ui->hitButton, &QPushButton::setEnabled);
    connect(&model, &Model::disableButtons, ui->standButton, &QPushButton::setEnabled);
    connect(&model, &Model::disableButtons, ui->mainMenu, &QPushButton::setEnabled);
    connect(&model, &Model::disableButtons, ui->dealCards, &QPushButton::setEnabled);
    connect(&model, &Model::disableButtons, ui->splitButton, &QPushButton::setEnabled);
    connect(&model, &Model::blackJackButtons, this, &PlayingWindow::blackJack);
    connect(&model, &Model::displayEndGameMessage, this, &PlayingWindow::endGamePopup);
    connect(&model, &Model::changeEndGameMessage, this, &PlayingWindow::changePopupText);
    connect(&model, &Model::enableMainMenu, ui->mainMenu, &QPushButton::setEnabled);

    //============= Reset game connections

    //connect(ui->mainMenu, &QPushButton::clicked, &model, &Model::resetGame);

    //============= initalDeal connections

    connect(&model,&Model::addCardToDealerHand,this,&PlayingWindow::addCardToDealerHand);
    connect(&model,&Model::addCardToPlayerHand,this,&PlayingWindow::addCardToPlayerHand);

    //============= dealCards

    connect(ui->dealCards,&QPushButton::clicked,this,&PlayingWindow::clearOldImages);
    connect(&model, &Model::enableDealCards, ui->dealCards, &QPushButton::setEnabled);
    connect(ui->dealCards, &QPushButton::clicked, &model, &Model::dealCards);

    //============= update Counts

    connect(&model, &Model::updateDealerCount, ui->dealerTotal, &QLabel::setText);
    connect(&model, &Model::updatePlayerCount, ui->playerTotal, &QLabel::setText);
    connect(&model, &Model::updateSplitPlayerCount, ui->splitTotal, &QLabel::setText);


    //============= script Handling connections
    connect(&model,&Model::sendMessage,this,&PlayingWindow::messageRecieved);
    connect(this, &PlayingWindow::nextLine,&model,&Model::readyForNextLine);
    connect(&model,&Model::endLevel,this,&PlayingWindow::endLevel);
    connect(ui->splitButton,&QPushButton::clicked,this,&PlayingWindow::unlockSplit);
    connect(ui->standButton,&QPushButton::clicked,this,&PlayingWindow::unlockStand);
    connect(ui->hitButton,&QPushButton::clicked,this,&PlayingWindow::unlockHit);
    connect(&model,&Model::revealHole,this,&PlayingWindow::flipDealerCard);
    connect(&model,&Model::sendLock,this,&PlayingWindow::recievedLock);
    connect(&model,&Model::sendClear,this,&PlayingWindow::clearOldImages);
    connect(&model,&Model::sendReadingScript,this,&PlayingWindow::setReadingScript);
    connect(&model,&Model::sendProbabilities,this,&PlayingWindow::receivedProb);

    //============= split connect
    connect(ui->splitButton,&QPushButton::clicked,this,&PlayingWindow::split);
    connect(ui->splitButton,&QPushButton::clicked,&model,&Model::splitSlot);
    connect(&model,&Model::enableSplit,this,&PlayingWindow::canSplit);
    connect(&model,&Model::showCurrentHand,this,&PlayingWindow::showWhichHand);
}


void PlayingWindow::on_nextButton_clicked()
{
    ui->ProbOfDealerBustLabel->setStyleSheet("color: rgb(255, 255, 255);");

    emit nextLine();
}
void PlayingWindow::endLevel(bool errorState)
{
    if(!errorState)
    {
        //Play animation
        //Change text box to display a "hooray" message?
    }
    //Disable next
    if(readingScript)
    {
        recievedLock("next");
        ui->nextButton->setEnabled(false);
    }

}
void PlayingWindow::endGamePopup(bool isVisible)
{
    ui->winButton->setVisible(isVisible);
}
void PlayingWindow::changePopupText(QString text){
    ui->winButton->setText(text);
    if (ui->winButton->text() == "You won" || ui->winButton->text() == "You got a blackjack") {
        //qDebug() << "Entering endGamePopup function"; // Debug line
        Animation *animation = new Animation(this);
        // Show the new window
        QTimer::singleShot(5000, [=]{animation->hide();});
        animation->show();

        //connect(animation, &Animation::finished, animation, &Animation::close);
    }
}
void PlayingWindow::blackJack(bool setState){
    ui->splitButton->setEnabled(setState);
    ui->hitButton->setEnabled(setState);
    ui->standButton->setEnabled(setState);
    ui->mainMenu->setEnabled(!setState);
    ui->dealCards->setEnabled(!setState);
}
