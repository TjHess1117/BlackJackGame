#include "model.h"
#include "game.h"
#include <QTimer>
#include <iostream>
#include <ostream>
Model::Model(QObject *parent)
    : QObject{parent}
    , deck()
    , playerOne(false)
    , dealer(true)
    , game(deck, playerOne, dealer, false)
{
    probability = Probability();
}
void Model::standSlot(){
    // if a 1 is returned then the player has stood on their split hand, this makes it so the players
    // hits now only work on the left hand (represented by a 0)
    int num = game.stand(playerOne);
    if(num == 0){ // if the player has stayed on the initial hand or the hand on the left of the split
        emit SendCardImage(dealer.getCardArray().begin()->image); // flips dealer card
        emit updateDealerCount(QString(QString::number(game.getDealerCount()))); // updates the count
        emit disableButtons(false); // disables buttons so that the player cannot hit or stand or split while the dealer deals
        if(isRigged){
            emit enableMainMenu(true);
        }
        if(game.getDealerCount() > 16){ // if the dealers hand is above a 16 with its given two cards end the round
            endGame();
            return;
        }
        int waitTime = 1000;
        int count = 1;
        while(true){
            waitTime = abs(count * waitTime);
            if(isRigged){
                get<0>(game.hit(dealer, riggedCards[nextCard]));
                nextCard++;
            }
            else{
                get<0>(game.hit(dealer));
            }
            if(game.getDealerCount() > 16){ // after hitting, if the dealer is above a 16, end the game and display the card
                QTimer::singleShot(waitTime, this,[=]{ emit addCardToDealerHand(dealer.getCardArray().at(count + 1),false);});
                emit updateDealerCount(QString(QString::number(game.getDealerCount())));
                endGame();
                break;
            }
            // display card and hit again
            QTimer::singleShot(waitTime, this,[=]{ emit addCardToDealerHand(dealer.getCardArray().at(count + 1),false);});
            QTimer::singleShot(waitTime, this,[=]{ emit updateDealerCount(QString(QString::number(game.getDealerCount())));});
            count++;
        }
    }
    else{ // if the split hand is dealt a 21 forcibly stand it
        emit showCurrentHand(true);
        std::tuple<bool, int> stateCheck = game.checkState(playerOne); // checks if the main hand is a 21
        if(get<1>(stateCheck) == 3){
            standSlot();
        }
    }
}
void Model::endGame(){
    if(!isRigged){ // if the game isnt rigged, when the game ends wait 3 seconds to renable the main menu and deal cards button
        QTimer::singleShot(3500, this,[=]{ emit enableDealCards(true);});
        QTimer::singleShot(3500, this,[=]{ emit enableMainMenu(true);});
    }
    std::tuple endResult = game.endResult();
    if(get<0>(endResult) == "tie"){
        if(get<1>(endResult) == "splitTie"){
            emit changeEndGameMessage("You tied on both hands");
        }
        else if(get<1>(endResult) == "splitWin"){
            emit changeEndGameMessage("You tied on your main hand and won your split hand");
        }
        else if(get<1>(endResult) == "splitLoss"){
            emit displayEndGameMessage(true);
            emit changeEndGameMessage("You tied on your main hand and lost your split hand");
        }
        else{
            emit changeEndGameMessage("You tied");
        }
    }
    else if(get<0>(endResult) == "loss"){
        if(get<1>(endResult) == "splitTie"){
            emit changeEndGameMessage("You tied on your main hand and tied on your split hand");
        }
        else if(get<1>(endResult) == "splitWin"){
            emit changeEndGameMessage("You lost on your main hand and won your split hand");
        }
        else if(get<1>(endResult) == "splitLoss"){
            emit changeEndGameMessage("You lost on both hands");
        }
        else{
            emit changeEndGameMessage("You lost");
        }
    }
    else if(get<0>(endResult) == "win"){
        if(get<1>(endResult) == "splitTie"){
            emit changeEndGameMessage("You won on your main hand and tied on your split hand");
        }
        else if(get<1>(endResult) == "splitWin"){
            emit displayEndGameMessage(true);
            emit changeEndGameMessage("You won on both hands");
        }
        else if(get<1>(endResult) == "splitLoss"){
            emit displayEndGameMessage(true);
            emit changeEndGameMessage("You won on your main hand and lost your split hand");
        }
        else{
            emit changeEndGameMessage("You won");
        }
    }
    emit displayEndGameMessage(true);
}
void Model::hitSlot(){
    std::tuple<bool, int> gameTuple;
    if(isRigged){
        gameTuple = game.hit(playerOne,riggedCards[nextCard]);
        emit updatePlayerCount(QString(QString::number(game.getPersonCount())));
        nextCard++;
        readyForNextLine();
        emit sendProbabilities(probability.probabilityOfDealerBust(dealer.getCardArray(), playerOne.getCardArray(), true),
                               probability.probabilityOfDealerExceeding(dealer.getCardArray(), playerOne.getCardArray()));
    }
    else{
        gameTuple = game.hit(playerOne);
        std::cout << game.getPersonCount() << std::endl;
        std::cout << game.getPersonSplitCount() << std::endl;
    }
    if(get<0>(gameTuple) && get<1>(gameTuple) == 0){ // if the main hand didn't bust
        emit addCardToPlayerHand(playerOne.getCardArray().back(), false); // display main hand card
        emit updatePlayerCount(QString(QString::number(game.getPersonCount()))); // update count
    }
    else if(get<0>(gameTuple) && get<1>(gameTuple) == 1){ // if the split hand didn't bust
        emit addCardToPlayerHand(playerOne.getSplitArray().back(), true); // display split hand card
        emit updateSplitPlayerCount(QString(QString::number(game.getPersonSplitCount()))); // update count
    }
    else if(!get<0>(gameTuple) && get<1>(gameTuple) == 1){ // if the split hand bust
        emit addCardToPlayerHand(playerOne.getSplitArray().back(), true);
        emit updateSplitPlayerCount(QString(QString::number(game.getPersonSplitCount())));
        standSlot();
    }
    else if(get<0>(gameTuple) && get<1>(gameTuple) == 3){ //player gets a 21 on main hand
        emit addCardToPlayerHand(playerOne.getCardArray().back(), false);
        emit updatePlayerCount(QString(QString::number(game.getPersonCount())));
        if(!isRigged)
            emit disableButtons(false);
        standSlot();
    }
    else if(get<0>(gameTuple) && get<1>(gameTuple) == 4){ //players split hand gets a 21 on split hand
        emit addCardToPlayerHand(playerOne.getSplitArray().back(), true);
        emit updateSplitPlayerCount(QString(QString::number(game.getPersonSplitCount())));
        standSlot();
    }
    else{ // the main hand bust
        emit addCardToPlayerHand(playerOne.getCardArray().back(), false);
        emit updatePlayerCount(QString(QString::number(game.getPersonCount())));
        if(get<0>(gameTuple) && get<1>(gameTuple) == 5){
            standSlot();
        }
        else{
            endGame();
            if(!isRigged){
                emit disableButtons(false);
                emit enableDealCards(true);
            }
        }
    }

}
void Model::dealCards(){
    if(currentLevel < 4)
        isRigged = true;
    else
        isRigged = false;
    emit disableButtons(true);
    initialDeal();
}
void Model::SetLevel(int level){
    currentLevel = level;

    if(currentLevel < 4)
    {
        isRigged = true;
        QString levelNum;
        switch(level){
            case(1):
            {
                levelNum = "One";
                break;
            }
            case(2):
            {
                levelNum = "Two";
                break;
            }
            default:
            {
                levelNum = "Three";
                break;
            }
        }
        emit sendReadingScript(isRigged);
        QString scriptName =":/scripts/levelScripts/level";
        scriptName.append(levelNum);
        levelScript.setScript(scriptName.append("Script.txt").toStdString());
        QString messagetype = levelScript.nextCommand(&scriptOutputDetails);
        interpretCommand(messagetype);
        emit sendLock("next\n");
    }
    else
    {
        isRigged = false;
        emit sendReadingScript(isRigged);
    }

    initialDeal();
    //riggedCards.clear();
}
void Model::mainMenuSlot(){
    riggedCards.clear();
    deck.shuffle();
    levelScript.lineNum = 0;
    nextCard = 0;
    emit endLevel(true);
}
void Model::resetGame(){
    playerOne.resetPlayer();
    dealer.resetPlayer();
    deck.shuffle();
}
void Model::initialDeal(){
    emit displayEndGameMessage(false);
    emit enableSplit(false);
    if(isRigged){
        resetGame();
        Game riggedGame(deck, playerOne, dealer, true);
        game = riggedGame; // creates a new game and passes in shuffled + reset information
        for(int i = 0; i<4; i++)
        {
            if(i%2==0)
                game.hit(playerOne, riggedCards[nextCard]);
            else
                game.hit(dealer, riggedCards[nextCard]);
            nextCard++;
        }
        emit updatePlayerCount(QString(QString::number(game.getPersonCount())));
        int temp = game.getDealerCount();
        temp = temp - dealer.getCardArray().at(0).value;
        emit updateDealerCount(QString(QString::number(temp)));
    }
    else{
        resetGame();
        Game newGame(deck, playerOne, dealer, false);
        game = newGame; // creates a new game and passes in shuffled + reset information
        emit updatePlayerCount(QString(QString::number(game.getPersonCount())));
        int temp = game.getDealerCount();
        temp = temp - dealer.getCardArray().at(0).value;
        emit updateDealerCount(QString(QString::number(temp)));
    }
    emit enableDealCards(false);
    emit addCardToPlayerHand(playerOne.getCardArray().at(0), false);
    emit addCardToDealerHand(dealer.getCardArray().at(0), true);
    emit addCardToPlayerHand(playerOne.getCardArray().at(1), false);
    emit addCardToDealerHand(dealer.getCardArray().at(1), false);
    int checkBlackJack = game.checkBlackJack(playerOne,dealer);
    if(checkBlackJack == 1){
        if(!isRigged)
            emit blackJackButtons(false);
        emit displayEndGameMessage(true);
        emit changeEndGameMessage("You got a blackjack");
        emit SendCardImage(dealer.getCardArray().begin()->image);
        emit updateDealerCount(QString(QString::number(game.getDealerCount())));
    }
    else if(checkBlackJack == 2){
        if(!isRigged)
            emit blackJackButtons(false);
        emit displayEndGameMessage(true);
        emit changeEndGameMessage("The dealer got a blackjack");
        emit SendCardImage(dealer.getCardArray().begin()->image);
        emit updateDealerCount(QString(QString::number(game.getDealerCount())));
    }
    else if(checkBlackJack == 3){
        if(!isRigged)
            emit blackJackButtons(false);
        emit displayEndGameMessage(true);
        emit changeEndGameMessage("You and the dealer both got blackjack");
        emit SendCardImage(dealer.getCardArray().begin()->image);
        emit updateDealerCount(QString(QString::number(game.getDealerCount())));
    }
    else if(playerOne.getCardArray().at(0).rank == playerOne.getCardArray().at(1).rank && isRigged == false && playerOne.getCardArray().size() == 2){ emit enableSplit(true); }
    else{emit enableSplit(false);}
    emit sendProbabilities(probability.probabilityOfDealerBust(dealer.getCardArray(), playerOne.getCardArray(), false),
                           probability.probabilityOfDealerExceeding(dealer.getCardArray(), playerOne.getCardArray()));
}
void Model::splitSlot(){
    if(isRigged){
        game.split(playerOne, riggedCards[nextCard + 1], riggedCards[nextCard]);
        nextCard += 2;
    }
    else
        game.split(playerOne);
    std::tuple<bool, int> stateCheck = game.checkState(playerOne); // checks if the split hand is a 21
    if(get<1>(stateCheck) == 4){
        standSlot();
    }
    emit updatePlayerCount(QString(QString::number(game.getPersonCount())));
    emit updateSplitPlayerCount(QString(QString::number(game.getPersonSplitCount())));
    emit addCardToPlayerHand(playerOne.getSplitArray().back(), true);
    emit addCardToPlayerHand(playerOne.getCardArray().back(), false);
}
void Model::readyForNextLine()
{
    QString messageType = levelScript.nextCommand(&scriptOutputDetails);
    interpretCommand(messageType);
}
void Model::interpretCommand(QString messageType)
{
    //TODO: Switch case here?
    if(messageType == "error")
    {
        //EndLevel
        emit endLevel(true);
    }
    else if (messageType == "message")
    {
        std::vector<QString> tokens =levelScript.tokenize(scriptOutputDetails);
        if(tokens[tokens.size()-2] == "lock"){
            emit sendLock(tokens[tokens.size()-1]);
            scriptOutputDetails.clear();
            for(int i =0; i<tokens.size()-2; i++)
            {
                scriptOutputDetails.append(tokens[i]);
                scriptOutputDetails.append(" ");
            }
        }
        //emit message to the view (it will show the popup)
        emit sendMessage(scriptOutputDetails);
    }
    else if (messageType == "deal")
    {
        std::vector<QString> tokens = levelScript.tokenize(scriptOutputDetails);
        if(tokens[1] == "dealer")
        {
            //deal the next card in the card draw queue to dealer primary hand
            game.hit(dealer, riggedCards[nextCard]);
        }
        else if(tokens[2]== "hand")
        {
            //deal the next card in the card draw queue to the players primary hand
            game.hit(playerOne, riggedCards[nextCard]);

        }
        else if(tokens[2]== "split")
        {
            //deal the next card in the card draw queue to the players split hand
            game.hit(playerOne, riggedCards[nextCard]);
        }
        nextCard++;
    }
    else if (messageType == "reveal")
    {
        //reveal hole
        emit revealHole(dealer.getCardArray()[0].image);
    }
    else if (messageType == "addCards")
    {
        std::vector<QString> tokens = levelScript.tokenize(scriptOutputDetails);
        for(QString token:tokens)
        {
            Suit suit;
            if(token.at(0) == 'h')
            {
                suit = heart;
            }
            else if(token.at(0) == 'd')
            {
                suit = diamond;
            }
            else if(token.at(0) == 'c')
            {
                suit = club;
            }
            else
            {
                suit = spade;
            }
            QString rank = token.removeFirst();

            riggedCards.push_back(deck.draw(stringToRank(rank), suit));
        }

    }
    else if (messageType == "finish")
    {
        riggedCards.clear();
        //emit end level signal
        emit endLevel(false);
    }
    else if (messageType == "shuffle")
    {
        //Pull all cards back into the deck and shuffle
        emit sendClear();
        initialDeal();
    }
    else if (messageType == "lock")
    {
        emit sendLock(scriptOutputDetails);
    }
    else
    {
        //This is an error state. We should be throwing here, or something
        emit endLevel(true);
    }
}

Rank Model::stringToRank(QString text)
{
    if(text == "Ace")
        return ace;
    if(text == "Two")
        return two;
    if(text == "Three")
        return three;
    if(text == "Four")
        return four;
    if(text == "Five")
        return five;
    if(text == "Six")
        return six;
    if(text == "Seven")
        return seven;
    if(text == "Eight")
        return eight;
    if(text == "Nine")
        return nine;
    if(text == "Ten")
        return ten;
    if(text == "Jack")
        return jack;
    if(text == "Queen")
        return queen;
    if(text == "King"){
        return king;
    }
    else{
        text = text.removeLast();
        return stringToRank(text);
    }
}
