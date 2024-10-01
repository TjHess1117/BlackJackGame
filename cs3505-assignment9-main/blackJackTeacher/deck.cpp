#include "deck.h"

#include <algorithm>
#include <random>
#include <iostream>
#include <chrono>
#include <vector>
#include <QUrl>


Card::Card(Rank rank, Suit suit, int value, QImage& image): rank(rank), suit(suit), value(value), image(image){}

Deck::Deck(){
    shuffle();
}

void Deck::shuffle(){
    cards.clear();
    for(int i=0; i<4; i++) {
        for(int j=2; j<=14; j++) {
            Rank rank;
            Suit suit;
            int value;
            QImage image;
            QString filename = "";

            if(i == 0){
                suit = heart;
                filename += "heart";
            }
            else if(i == 1){
                suit = diamond;
                filename += "diamond";
            }
            else if(i == 2){
                suit = club;
                filename += "club";
            }
            else{
                suit = spade;
                filename += "spade";
            }

            if(j == 2){
                rank = two;
                filename += "Two";
                value = 2;
            }
            else if(j == 3){
                rank = three;
                filename += "Three";
                value = 3;
            }
            else if(j == 4){
                rank = four;
                filename += "Four";
                value = 4;
            }
            else if(j == 5){
                rank = five;
                filename += "Five";
                value = 5;
            }
            else if(j == 6){
                rank = six;
                filename += "Six";
                value = 6;
            }
            else if(j == 7){
                rank = seven;
                filename += "Seven";
                value = 7;
            }
            else if(j == 8){
                rank = eight;
                filename += "Eight";
                value = 8;
            }
            else if(j == 9){
                rank = nine;
                filename += "Nine";
                value = 9;
            }
            else if(j == 10){
                rank = ten;
                filename += "Ten";
                value = 10;
            }
            else if(j == 11){
                rank = jack;
                filename += "Jack";
                value = 10;
            }
            else if(j == 12){
                rank = queen;
                filename += "Queen";
                value = 10;
            }
            else if(j == 13){
                rank = king;
                filename += "King";
                value = 10;
            }
            else{
                rank = ace;
                filename += "Ace";
                value = 11;
            }

            image.load(":/images/cardImages/" + filename + ".png");

            cards.push_back(Card(rank, suit, value, image));
        }
    }

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rd(seed);

    std::shuffle(std::begin(cards), std::end(cards), rd);
}

Card Deck::draw(){
    if(cards.size() > 0){
        Card ret = cards.back();
        cards.pop_back();
        return ret;
    }
    else{
        throw std::invalid_argument("no more cards");
    }
}

Card Deck::draw(Rank rank, Suit suit){
    std::vector<Card>::iterator CardPtr;
    for(CardPtr = cards.begin(); CardPtr < cards.end(); CardPtr++){
        if(CardPtr->suit == suit && CardPtr->rank == rank){
            Card retCard = *CardPtr;
            cards.erase(CardPtr);
            return retCard;
        }
    }
    throw std::invalid_argument("This card has already been drawn");
}



















