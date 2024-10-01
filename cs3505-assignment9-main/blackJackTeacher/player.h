#ifndef PLAYER_H
#define PLAYER_H

#include "deck.h"

///
/// \brief The player holds the information of the current players states, this includes their cards, and their current count
/// (Ace + 4 = 5 or 15)
///
class Player
{
private:
    bool isDealer; // if the player is a dealer the rules will be different
    bool choseStand[2] = {false, false}; // if theres a split lets game know to check that hand too
    std::vector<Card> cardArray; //holds the players hand
    std::vector<Card> splitArray; //if theres a split the second card goes here
    int currentHand = 0; // 0 is left hand, 1 is right hand
public:
    Player(bool isDealer);
    /// adds card to players hand
    void addCard(Card topCard);
    /// if a player chooses to split, creates another hand
    void addHand(Card topCard, Card cardAfter);
    /// getter and setter for state of the current hand
    bool getState(QString hand);
    void setState(bool state);
    /// resets the player back to how they are at the start
    void resetPlayer();
    /// getter for whether or not the player is a dealer
    bool getIsDealer();
    ///
    /// \brief getCurrentHand returns the players current hand
    /// \return
    ///
    int getCurrentHand();
    ///
    /// \brief setCurrentHand sets the players current hand
    /// \param setter
    ///
    void setCurrentHand(int setter);
    ///
    /// \brief getCardArray gets the cardArray Vector
    /// \return
    ///
    std::vector<Card> getCardArray();
    ///
    /// \brief getSplitArray gets the splitArray Vector
    /// \return
    ///
    std::vector<Card> getSplitArray();

};

#endif // PLAYER_H
