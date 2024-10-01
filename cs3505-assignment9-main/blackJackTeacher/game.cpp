#include "game.h"
#include "deck.h"
#include "player.h"
#include <iostream>

Game::Game(Deck& deck, Player& person, Player& dealer, bool rigged) {
    gameDeck = deck;
    isRigged = rigged;
    if(!isRigged){
        isRigged = false;
        person.addCard(gameDeck.draw()); // add cards to player and dealer
        dealer.addCard(gameDeck.draw());
        person.addCard(gameDeck.draw());
        dealer.addCard(gameDeck.draw());
    }
    else{
        isRigged = true;
    }
    checkState(dealer);
    checkState(person);
}
Game::Game(const Game& otherGame){
    gameDeck = otherGame.gameDeck;
    personCount = otherGame.personCount;
    dealerCount = otherGame.dealerCount;
    personSplitCount = otherGame.personSplitCount;
    isRigged = otherGame.isRigged;
}
Game& Game::operator=(Game otherGame){
    std::swap(gameDeck,otherGame.gameDeck);
    std::swap(personCount,otherGame.personCount);
    std::swap(dealerCount,otherGame.dealerCount);
    std::swap(personSplitCount,otherGame.personSplitCount);
    std::swap(isRigged, otherGame.isRigged);
    return *this;
}
Game::~Game(){}
Deck Game::getDeck(){
    return gameDeck;
}
int Game::checkBlackJack(Player& person, Player& dealer){
    personCount = person.getCardArray().at(0).value + person.getCardArray().at(1).value; // get inital personCount
    dealerCount = dealer.getCardArray().at(0).value + dealer.getCardArray().at(1).value; // get inital dealerCount
    if(personCount == 21 && dealerCount != 21){ // returns player win
        return 1;
    }
    else if(dealerCount == 21 && personCount != 21){ // returns player loss
        // emit dealer win
        return 2;
    }
    else if(dealerCount == 21 && personCount == 21){ // returns tie
        // return tie
        return 3;
    }
    return 4; // returns 4 if neither party got a blackJack
}

std::tuple<bool,int> Game::checkState(Player currentPlayer){
    if(currentPlayer.getIsDealer()){ // adds up the dealers count when they hit
        dealerCount = 0;
        int aceCount = 0;
        for(int i = 0; i < int(currentPlayer.getCardArray().size()); i++){
            dealerCount += currentPlayer.getCardArray().at(i).value;
            if(currentPlayer.getCardArray().at(i).rank == ace){
                aceCount += 1;
            }
        }
        while(aceCount > 0){ // if an ace is present, this logic decides whether its a 1 or 11
            if(dealerCount > 21){
                dealerCount -= 10;
                aceCount -= 1;
            }
            else{
                break;
            }
        }
        if(dealerCount > 21){ // returns that the dealer has bust
            return std::tuple<bool,int>(false,0);
        }
        else{ // returns that the dealer has not bust
            return std::tuple<bool,int>(true,0);
        }
    }
    if(!currentPlayer.getIsDealer()){ // adds up the players respective hand's counts
        personCount = 0;
        int aceCount = 0;
        for(int i = 0; i < int(currentPlayer.getCardArray().size()); i++){
            personCount += currentPlayer.getCardArray().at(i).value;
            if(currentPlayer.getCardArray().at(i).rank == ace){
                aceCount += 1;
            }
        }
        while(aceCount > 0){
            if(personCount > 21){
                personCount -= 10;
                aceCount -= 1;
            }
            else{
                break;
            }
        }
        if(personCount > 21 && personSplitCount < 22 && personSplitCount > 0){
            return std::tuple<bool,int>(false,5);
        }
        else if(personCount > 21){
            return std::tuple<bool,int>(false,0);
        }
        else if(personCount == 21 && currentPlayer.getCurrentHand() == 0){ // if the player gets a 21 and they aren't currently dealing with the split hand
            return std::tuple<bool,int>(true,3); // returns that its not a bust and that the main hand is a 21
        }
        if(int(currentPlayer.getSplitArray().size()) != 0 && !currentPlayer.getState("split")){ // same as above but for a split hand
            personSplitCount = 0;
            aceCount = 0;
            for(int i = 0; i < int(currentPlayer.getSplitArray().size()); i++){
                personSplitCount += currentPlayer.getSplitArray().at(i).value;
                if(currentPlayer.getSplitArray().at(i).rank == ace){
                    aceCount += 1;
                }
            }
            while(aceCount > 0){
                if(personSplitCount > 21){
                    personSplitCount -= 10;
                    aceCount -= 1;
                }
                else{
                    break;
                }
            }
            if(personSplitCount > 21){
                return std::tuple<bool,int>(false,1);
            }
            else if(personSplitCount == 21){ // if the player hit a 21 on the split hand
                return std::tuple<bool,int>(true,4); // returns that the player didn't bust and a number that specifies
                // that the split hand got a 21
            }
        }
    }
    return std::tuple<bool,int>(true, currentPlayer.getCurrentHand()); // continue the game as normal
}

std::tuple<QString,QString> Game::endResult(){
    std::tuple<QString,QString> returnTuple("","");
    if(personSplitCount != 0){ // if a split exists
        if(personSplitCount == dealerCount){
            get<1>(returnTuple) = "splitTie";
        }
        if(personSplitCount < dealerCount){
            get<1>(returnTuple) = "splitLoss";
        }
        if(personSplitCount > dealerCount){
            get<1>(returnTuple) = "splitWin";
        }
    }
    if(dealerCount > 21){
        get<0>(returnTuple) = "win";
        if(personSplitCount <= 21 && personSplitCount != 0){
            get<1>(returnTuple) = "splitWin";
        }
        else if(personSplitCount != 0){
            get<1>(returnTuple) = "splitLoss";
        }
    }
    else if(personCount < dealerCount){
        get<0>(returnTuple) = "loss";
    }
    else if(personCount == dealerCount){
        get<0>(returnTuple) = "tie";
    }
    else if(personCount > dealerCount){
        get<0>(returnTuple) = "win";
    }
    if(personSplitCount > 21){
        get<1>(returnTuple) = "splitLoss";
    }
    if(personCount > 21){
        get<0>(returnTuple) = "loss";
    }
    return returnTuple;
}
std::tuple<bool,int> Game::hit(Player& currentPlayer){
    currentPlayer.addCard(gameDeck.draw());
    std::tuple<bool, int> playerState = checkState(currentPlayer);
    if(get<0>(playerState) && get<1>(playerState) == 3){ // if the main hand is a 21
        return std::tuple<bool, int>(true,3); // returns true meaning the hand didn't bust
    }
    if(get<0>(playerState) && get<1>(playerState) == 4){ // if the split hand is a 21
        return std::tuple<bool, int>(true,4);
    }
    if(get<0>(playerState) && get<1>(playerState) == 0){ // if the main hand is less than a 21
        return std::tuple<bool, int>(true,currentPlayer.getCurrentHand());
    }
    else if(!get<0>(playerState) && get<1>(playerState) == 0){ // if the main hand is greater than a 21
        return std::tuple<bool, int>(false, currentPlayer.getCurrentHand()); // returns false meaning the hand busted
    }
    if(get<0>(playerState) && get<1>(playerState) == 1){ // if the split hand is less than a 21
        return std::tuple<bool, int>(true, currentPlayer.getCurrentHand());
    }
    else{ // the split hand is greater than a 21
        int tempVal = currentPlayer.getCurrentHand();
        return std::tuple<bool, int>(false, tempVal);
    }
}

std::tuple<bool,int> Game::hit(Player& currentPlayer, Card card){
    currentPlayer.addCard(gameDeck.draw(card.rank,card.suit)); // takes in specific card
    std::tuple<bool, int> playerState = checkState(currentPlayer);
    if(get<0>(playerState) && get<1>(playerState) == 3){ // if the main hand is a 21
        return std::tuple<bool, int>(true,3); // returns true meaning the hand didn't bust
    }
    if(get<0>(playerState) && get<1>(playerState) == 4){ // if the split hand is a 21
        return std::tuple<bool, int>(true,4);
    }
    if(get<0>(playerState) && get<1>(playerState) == 0){ // if the main hand is less than a 21
        return std::tuple<bool, int>(true,currentPlayer.getCurrentHand());
    }
    else if(!get<0>(playerState) && get<1>(playerState) == 0){ // if the main hand is greater than a 21
        return std::tuple<bool, int>(false, currentPlayer.getCurrentHand()); // returns false meaning the hand busted
    }
    if(get<0>(playerState) && get<1>(playerState) == 1){ // if the split hand is less than a 21
        return std::tuple<bool, int>(true, currentPlayer.getCurrentHand());
    }
    if(!get<0>(playerState) && get<1>(playerState) == 5){
        return std::tuple<bool,int>(false, 5);
    }
    else{ // the split hand is greater than a 21
        int tempVal = currentPlayer.getCurrentHand();
        currentPlayer.setCurrentHand(0);
        return std::tuple<bool, int>(false, tempVal);
    }
}

bool Game::split(Player& person){
    person.addHand(gameDeck.draw(), gameDeck.draw());
    checkState(person);
    return true;
}

bool Game::split(Player& person, Card splitCard, Card mainCard){
    person.addHand(gameDeck.draw(splitCard.rank, splitCard.suit), gameDeck.draw(mainCard.rank, mainCard.suit));
    checkState(person);
    return true;
}

int Game::stand(Player& currentPlayer){
    currentPlayer.setState(true);
    if(currentPlayer.getCurrentHand() == 0) // if the player has stood on the main hand
        return 0;
    currentPlayer.setCurrentHand(currentPlayer.getCurrentHand() - 1); // if the player stood on a split hand, deincriment currenthand by one
    return 1;
}


int Game::getDealerCount(){
    return dealerCount;
}
int Game::getPersonCount(){
    return personCount;
}
int Game::getPersonSplitCount(){
    return personSplitCount;
}


