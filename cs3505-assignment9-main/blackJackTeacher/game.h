#ifndef GAME_H
#define GAME_H

#include "deck.h"
#include "player.h"

class Game
{

private:
    Deck gameDeck; // the given deck
    bool isRigged;
    int personCount; // count that the left hand (or center if no split) totals to
    int dealerCount; // dealers count
    int personSplitCount = 0; // count that the right hand totals to
public:
    Game(Deck& deck, Player& person, Player& dealer, bool isRigged);

    Game(const Game& other);
    //destructor
    ~Game();
    //equal method
    Game& operator=(Game other);
    /// checks whether the dealer or the player has lost, false is player bust, true means that the player did not bust or the dealer bust
    std::tuple<bool,int> checkState(Player currentPlayer);
    /// adds card to player
    std::tuple<bool,int> hit(Player& person);
    /// splits a persons hand
    bool split(Player& person);
    bool split(Player& person, Card card, Card secondCard);
    /// disables further actions on players current hand
    int stand(Player& person);
    /// decides who won or loss, bool determines whether the dealer won or the player won, and the int determines which hand won
    std::tuple<QString,QString> endResult();
    /// rigs Game
    std::tuple<bool,int> hit(Player& person, Card card);

    /// initial checker for whether the dealer or the player or both got a blackjack
    int checkBlackJack(Player& person, Player& dealer);
    ///
    /// \brief getDeck returns the deck
    /// \return
    ///
    Deck getDeck();
    ///
    /// \brief getPersonCount returns the main hand count
    /// \return
    ///
    int getPersonCount();
    ///
    /// \brief getPersonSplitCount returns the count of the split hand
    /// \return
    ///
    int getPersonSplitCount();
    ///
    /// \brief getDealerCount returns the dealer count
    /// \return
    ///
    int getDealerCount();

};

#endif // GAME_H
