#include "probability.h"

Probability::Probability() {
}

double Probability::probabilityOfDealerBust(std::vector<Card> dealersHand, std::vector<Card> playersHand,bool holeRevealed)
{
    return probabilityOfBust(dealersHand, playersHand)*100;
}

double Probability::probabilityOfDealerBust(std::vector<Card> dealersHand, std::vector<Card> playersHand1, std::vector<Card>playersHand2)
{
    return probabilityOfBustSplit(dealersHand, playersHand1, playersHand2)*100;
}

double Probability::probabilityOfPlayerBust(std::vector<Card> dealersHand, std::vector<Card> playersHand)
{
    return probabilityOfBust(playersHand, dealersHand)*100;
}

double Probability::probabilityOfPlayerBust(std::vector<Card> dealersHand, std::vector<Card> playersHand1, std::vector<Card>playersHand2)
{
    return probabilityOfBustSplit(playersHand1, playersHand2, dealersHand)*100;
}

double Probability::probabilityOfDealerExceeding(std::vector<Card> dealersHand, std::vector<Card> playersHand)
{
    //Track the number of cards remaining in the deck
    int cardsRemaining = 52;
    //Begin by assuming all cards are in the deck
    int inDeck[13];
    for(int i = 0; i<13;i++)
    {
        inDeck[i] = 4;
    }
    //Hold a tally of the player's cards
    int pHandSum = 0;
    for(int i = 0; i<playersHand.size(); i++)
    {
        Card card = playersHand[i];
        if(card.rank == ace)
        {
            inDeck[0] -= 1;
            pHandSum += 1;
        }
        else if(card.rank == jack)
        {
            inDeck[10] -= 1;
            pHandSum += 10;
        }
        else if(card.rank == queen)
        {
            inDeck[11] -= 1;
            pHandSum += 10;
        }
        else if(card.rank == king)
        {
            inDeck[12] -= 1;
            pHandSum += 10;
        }
        else
        {
            inDeck[card.rank-1] -= 1;//Have to subtract one because we start at 0
            pHandSum += card.rank;
        }
        cardsRemaining --;
    }
    //Hold a tally of the player's cards
    int dHandSum = 0;
    for(int i = 0; i<dealersHand.size(); i++)
    {
        Card card = dealersHand[i];
        if(card.rank == ace)
        {
            inDeck[0] -= 1;
            dHandSum += 1;
        }
        else if(card.rank == jack)
        {
            inDeck[10] -= 1;
            dHandSum += 10;
        }
        else if(card.rank == queen)
        {
            inDeck[11] -= 1;
            dHandSum += 10;
        }
        else if(card.rank == king)
        {
            inDeck[12] -= 1;
            dHandSum += 10;
        }
        else
        {
            inDeck[card.rank-1] -= 1;//Have to subtract one because we start at 0
            dHandSum += card.rank;
        }
        cardsRemaining --;
    }
    //Take the difference between the sums
    int minimumToWin = pHandSum - dHandSum ;
    //If the dealers hand is already higher, the dealer already exceeded
    if(minimumToWin < 0)
        return 1.0;//Return 1
    int maxNoBust = 21-dHandSum;
    int cardsThatBeat = 0;
    for(int i = minimumToWin; i< maxNoBust; i++)
        cardsThatBeat += inDeck[i];

    return (double)cardsThatBeat/cardsRemaining;
}


///BACKERS------------------------------------------

double Probability::probabilityOfBust(std::vector<Card> handToAnalyze, std::vector<Card> otherHand)
{
    //Track the number of cards remaining in the deck
    int cardsRemaining = 52;
    //Begin by assuming all cards are in the deck
    int inDeck[13];
    for(int i = 0; i<13;i++)
    {
        inDeck[i] = 4;
    }
    //Hold a tally of the handToAnalyze's cards
    int handSum = 0;
    for(int i = 0; i<handToAnalyze.size(); i++)
    {
        Card card = handToAnalyze[i];
        if(card.rank == ace)
        {
            inDeck[0] -= 1;
            handSum += 1;
        }
        else if(card.rank == jack)
        {
            inDeck[10] -= 1;
            handSum += 10;
        }
        else if(card.rank == queen)
        {
            inDeck[11] -= 1;
            handSum += 10;
        }
        else if(card.rank == king)
        {
            inDeck[12] -= 1;
            handSum += 10;
        }
        else
        {
            inDeck[card.rank-1] -= 1;//Have to subtract one because we start at 0
            handSum += card.rank;
        }
        cardsRemaining --;
    }

    //Remove the cards in the other hand from the pool of possible cards
    for(int i = 0; i<otherHand.size(); i++)
    {
        Card card = otherHand[i];
        if(card.rank == ace)
            inDeck[0] -= 1;
        else if(card.rank == jack)
            inDeck[10] -= 1;
        else if(card.rank == queen)
            inDeck[11] -= 1;
        else if(card.rank == king)
            inDeck[12] -= 1;
        else
            inDeck[card.rank-1] -= 1;//Have to subtract one because we start at 0

        cardsRemaining --;
    }

    int minimumToBust = 22- handSum;
    int cardsOverMin =0;
    for(int i = (minimumToBust-1);i<13;i++)//Have to subtract one because we start at 0
        cardsOverMin += inDeck[i];
    return (double)cardsOverMin/cardsRemaining;
}


double Probability::probabilityOfBustSplit(std::vector<Card> handToAnalyze, std::vector<Card> otherHand, std::vector<Card> thirdHand)
{
    //Track the number of cards remaining in the deck
    int cardsRemaining = 52;
    //Begin by assuming all cards are in the deck
    int inDeck[13];
    for(int i = 0; i<13;i++)
    {
        inDeck[i] = 4;
    }
    //Hold a tally of the handToAnalyze's cards
    int handSum = 0;
    for(Card card:handToAnalyze)
    {
        if(card.rank == ace)
        {
            inDeck[0] -= 1;
            handSum += 1;
        }
        else if(card.rank == jack)
        {
            inDeck[10] -= 1;
            handSum += 10;
        }
        else if(card.rank == queen)
        {
            inDeck[11] -= 1;
            handSum += 10;
        }
        else if(card.rank == king)
        {
            inDeck[12] -= 1;
            handSum += 10;
        }
        else
        {
            inDeck[card.rank-1] -= 1;//Have to subtract one because we start at 0
            handSum += card.rank;
        }
        cardsRemaining --;
    }

    //Remove the cards in the other hand from the pool of possible cards
    for(Card card:otherHand)
    {
        if(card.rank == ace)
            inDeck[0] -= 1;
        else if(card.rank == jack)
            inDeck[10] -= 1;
        else if(card.rank == queen)
            inDeck[11] -= 1;
        else if(card.rank == king)
            inDeck[12] -= 1;
        else
            inDeck[card.rank-1] -= 1;//Have to subtract one because we start at 0

        cardsRemaining --;
    }

    //Remove the cards in the dealers hand from the pool of possible cards
    for(Card card:thirdHand)
    {
        if(card.rank == ace)
            inDeck[0] -= 1;
        else if(card.rank == jack)
            inDeck[10] -= 1;
        else if(card.rank == queen)
            inDeck[11] -= 1;
        else if(card.rank == king)
            inDeck[12] -= 1;
        else
            inDeck[card.rank-1] -= 1;//Have to subtract one because we start at 0

        cardsRemaining --;
    }

    int minimumToBust = 22- handSum;
    int cardsOverMin =0;
    for(int i = (minimumToBust-1);i<13;i++)//Have to subtract one because we start at 0
        cardsOverMin += inDeck[i];
    return (double)cardsOverMin/cardsRemaining;
}


double Probability::probabilityOfBust3OrMoreHands(std::vector<std::vector<Card>> hands)
{
    //Track the number of cards remaining in the deck
    int cardsRemaining = 52;
    //Begin by assuming all cards are in the deck
    int inDeck[13];
    for(int i = 0; i<13;i++)
    {
        inDeck[i] = 4;
    }
    //Hold a tally of the handToAnalyze's cards
    int handSum = 0;
    for(Card card:hands.at(0))
    {
        if(card.rank == ace)
            handSum += 1;
        else if(card.rank == jack)
            handSum += 10;
        else if(card.rank == queen)
            handSum += 10;
        else if(card.rank == king)
            handSum += 10;
        else
            handSum += card.rank;
        cardsRemaining --;
    }
    for(std::vector<Card> hand:hands)
    {
        for(Card card:hand)
        {
            if(card.rank == ace)
                inDeck[0] -= 1;
            else if(card.rank == jack)
                inDeck[10] -= 1;
            else if(card.rank == queen)
                inDeck[11] -= 1;
            else if(card.rank == king)
                inDeck[12] -= 1;
            else
                inDeck[card.rank-1] -= 1;//Have to subtract one because we start at 0

            cardsRemaining --;
        }
    }
    int minimumToBust = 22- handSum;
    int cardsOverMin =0;
    for(int i = (minimumToBust-1);i<13;i++)//Have to subtract one because we start at 0
        cardsOverMin += inDeck[i];
    return (double)cardsOverMin/cardsRemaining;
}


