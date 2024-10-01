#ifndef MODEL_H
#define MODEL_H

#include "game.h"
#include <QObject>
#include "script.h"
#include "probability.h"

class Model : public QObject
{
    Q_OBJECT
public:
    explicit Model(QObject *parent = nullptr);

signals:
    void showCurrentHand(bool);
    void blackJackButtons(bool);
    ///
    /// \brief enableSplit send a signal to the view to enable to split button
    /// \param canSplit
    ///
    void enableSplit(bool canSplit);
    ///
    /// \brief sendLock
    /// \param allBut
    ///
    void sendLock(QString allBut);
    ///
    /// \brief SendCardImage sends a specefic image to the view to display
    ///
    void SendCardImage(QImage);
    ///
    /// \brief disableButtons disables or enables the split, hit, and stand button
    ///
    void disableButtons(bool);
    ///
    /// \brief addCardToPlayerHand displays the most recent card for the player
    ///        the bool determines which hand
    ///
    void addCardToPlayerHand(Card, bool);
    ///
    /// \brief addCardToDealerHand displays the dealers most recent card
    ///
    void addCardToDealerHand(Card, bool);
    ///
    /// \brief enableDealCards enables or disables the ability to click the deal cards button
    ///
    void enableDealCards(bool);
    ///
    /// \brief winMessage displays upon the end of a game
    ///
    void displayEndGameMessage(bool);
    void changeEndGameMessage(QString);
    ///
    /// \brief updatePlayerCount displays the main hand count for a player
    ///
    void updatePlayerCount(QString);
    ///
    /// \brief updatePlayerCount displays the split count for a player
    ///
    void updateSplitPlayerCount(QString);
    ///
    /// \brief updateDealerCount displayes the count for a dealer
    ///
    void updateDealerCount(QString);
    ///
    /// \brief enableMainMenu enables or disables the main menu button
    ///
    void enableMainMenu(bool);
    ///
    /// \brief sendReadingScript
    ///
    void sendReadingScript(bool);
    ///
    /// \brief sendProbabilities signal that sends relevant probabilities to
    /// \param probOfDealerBust
    /// \param probOfDealerWin
    ///
    void sendProbabilities(double probOfDealerBust, double probOfDealerWin);
    ///
    /// \brief endLevel send a signal to the view to display game end information
    /// \param errorState whether the game was terminated due to an error or naturally
    ///
    void endLevel(bool errorState);
    ///
    /// \brief sendMessage sends a message to be displayed to the user
    /// \param message a QString containing text to display to the user
    ///
    void sendMessage(QString message);
    ///
    /// \brief revealHole signals the view to display the hole card to the user
    ///
    void revealHole(QImage cardFront);

    void sendClear();

public slots:
    ///
    /// \brief hitSlot handles logic for when a player hits the hit button
    ///
    void hitSlot();
    ///
    /// \brief dealCards handles logic for when the player hits deal cards
    ///
    void dealCards();
    void SetLevel(int level);
    ///
    /// \brief standSlot handles logic for when the player hits the stand button
    ///
    void standSlot();
    ///
    /// \brief splitSlot handles logic for when the player hits the split button
    ///
    void splitSlot();
    ///
    /// \brief readyForNextLine parse the next line and prepare to send it to the view
    ///
    void readyForNextLine();
    ///
    /// \brief mainMenuSlot handles logic for when the player hits the main menu button
    ///
    void mainMenuSlot();

private:
    ///
    /// \brief deck the deck that is passed into the game
    ///
    Deck deck;
    ///
    /// \brief playerOne the player object that is passed into the game and represnts the person playing
    ///
    Player playerOne;
    ///
    /// \brief dealer the player object that is passed into the game and represents the dealer
    ///
    Player dealer;
    ///
    /// \brief game the game object which handles the entirety of black jacks logic
    ///
    Game game;
    ///
    /// \brief isRigged boolean which determines whether or not the game is rigged
    ///
    bool isRigged = false;
    ///
    /// \brief currentLevel
    ///
    int currentLevel;
    ///
    /// \brief endGame helper message that decides to display end game logic
    ///
    void endGame();
    std::vector<Card> riggedCards;
    int nextCard = 0;
    ///
    /// \brief scriptOutputDetails a string used as an additional output by the script object
    ///
    QString scriptOutputDetails;
    ///
    /// \brief levelScript a script file that is read in by a QFile object. represents the current level's script
    ///
    Script levelScript;
    Probability probability;
    ///
    /// \brief helper method that handles all of the logic for when a person trys to reset a game (deal new cards)
    ///
    void resetGame();

    ///
    /// \brief initialDeal method that is used to create a new game, is called whenever a level is selected or deal cards is pressed
    ///
    void initialDeal();
    ///
    /// \brief interpretCommand parses the output of the line provided by levelScript's nextLine and handles it in the context of the game
    /// \param messagetype the string returned by levelScript. Represents a command type
    ///
    void interpretCommand(QString messagetype);
    Rank stringToRank(QString text);
};

#endif // MODEL_H
