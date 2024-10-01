#ifndef SCRIPT_H
#define SCRIPT_H
#include <string>

#include <vector>
#include <QObject>
#include <QFile>
class Script
{
public:
    ///
    /// \brief Script default constructor
    ///
    Script();

    ///
    /// \brief Destructor
    ///
    ~Script();
    ///
    /// \brief nextCommand Parses the next line of the script and returns a string representing the type of command
    /// \param outputDetails a string that holds the details of the output. Will be replaced with a blank string, a string with a display message or a string that can be parsed into pieces to deal a card
    /// \return a string representing the next command's type
    ///
    QString nextCommand(QString* outputDetails);

    ///
    /// \brief operator = assignment operator
    /// \param other the other script
    /// \return the script that was provided on the rhs
    ///
    Script& operator=(Script& other);

    ///
    /// \brief setScript clears the previous script and starts the provided file script
    /// \param filename the script to be read
    ///
    void setScript(std::string filename);

    ///
    /// \brief tokenize breaks a given string into a vector of tokens seperated by a ' ' as a delimiter
    /// \param providedString a line to be tokenized
    /// \return a vector inn which each entry is a word in the original string.
    ///
    std::vector<QString> tokenize(QString providedString);

    ///
    /// \brief index
    ///
    int lineNum = 0;

private:
    ///
    /// \brief script a file stream reading the script
    ///
    std::vector<QString> lines;
    
};

#endif // SCRIPT_H
