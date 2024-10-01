#ifndef DECK_H
#define DECK_H

#include <QImage>
#include <vector>

///
/// \brief the 4 suits in a card deck
///
enum Suit{
    heart,
    diamond,
    club,
    spade
};

///
/// \brief the name of a particular card
///
enum Rank{
    two,
    three,
    four,
    five,
    six,
    seven,
    eight,
    nine,
    ten,
    jack,
    queen,
    king,
    ace
};

///
/// \brief The Card class represents an individual card in the 52 card deck
///
class Card{
public:
    Card(Rank rank, Suit suit, int value, QImage& image);
    ~Card(){};
    Rank rank;
    Suit suit;
    int value;
    QImage image;
};


///
/// \brief The Deck class represents a standard deck of 52 cards with suites hearts, diamonds, clubs, and spades.
///
class Deck{
public:
    Deck();
    ~Deck(){};
    /// \brief resets the deck by creating 52 new cards and randomizing their order
    void shuffle();
    /// \brief returns a single Card object from the "top" of the deck
    /// \return card
    Card draw();
    /// \brief draws the specified card from the deck, if that card has already been drawn, an error is thrown
    /// \param rank
    /// \param suit
    /// \return card
    Card draw(Rank rank, Suit suit);
private:
    /// \brief a vector of card objects that represents all the cards that have yet to be drawn
    std::vector<Card> cards;
};

#endif // DECK_H
