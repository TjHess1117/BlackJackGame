#include "player.h"
#include "deck.h"

Player::Player(bool isPlayerDealer){
    isDealer = isPlayerDealer;
}
void Player::addCard(Card topCard){
    if(currentHand == 0){
        cardArray.push_back(topCard);
    }
    else{
        splitArray.push_back(topCard);
    }
}
void Player::addHand(Card topCard, Card cardAfter){
    splitArray.push_back(cardArray.back()); // becomes hand on the right of player
    cardArray.pop_back();
    splitArray.push_back(topCard); // adds card to hand on the right
    cardArray.push_back(cardAfter); // adds card to hand on the left
    currentHand = 1; // sets current hand to the splitArray
}
void Player::setState(bool state){
    if(currentHand == 0)
         choseStand[0] = state;
    else
        choseStand[1] = state;
}
void Player::resetPlayer(){
    choseStand[0] = false;
    choseStand[1] = false;
    splitArray.clear();
    cardArray.clear();
    currentHand = 0;
}
bool Player::getState(QString hand){
    if(hand == "main")
        return choseStand[0];
    return choseStand[1];
}
bool Player::getIsDealer(){
    return isDealer;
}
std::vector<Card> Player::getCardArray(){
    return cardArray;
}
std::vector<Card> Player::getSplitArray(){
    return splitArray;
}
int Player::getCurrentHand(){
    return currentHand;
}
void Player::setCurrentHand(int setter){
    currentHand = setter;
}
