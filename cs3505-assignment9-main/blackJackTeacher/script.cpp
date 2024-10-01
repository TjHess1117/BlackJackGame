#include "script.h"
#include <vector>
#include <sstream>

Script::Script()
{}
Script::~Script()
{}

void Script::setScript(std::string filename)
{
    lines.clear();
    //Try to open the provided file name
    QFile file(QString::fromStdString(filename));
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    //Check to see if it opened. If not throw error
    while(!file.atEnd())
    {
        QString line = file.readLine();
        lines.push_back(line);
    }
}

QString Script::nextCommand(QString* outputDetails)
{
    //Read the next line from the file
    QString nextLine = lines[lineNum];
    lineNum++;
    if(!nextLine.isEmpty())
    {
        // By the contract we made for the scripts, starting with a forward slash represents a message to display to the view
        if(nextLine.at(0) == '/')
        {
            nextLine = nextLine.removeFirst();
            *outputDetails = nextLine.removeFirst();
            return "message";
        }
        // Otherwise, all other commands should start with an asterisk.
        else if(nextLine.at(0) == '*')
        {
            nextLine = nextLine.removeFirst();
            std::vector<QString> tokens = tokenize(nextLine.removeFirst());
            if(tokens[0] == "deal")
            {
                *outputDetails = tokens[1] +" " + tokens[2];
                return "deal";
            }
            if(tokens[0] == "shuffle\n" || tokens[0] == "shuffle")
            {
                *outputDetails = "";
                //Pull all cards back into the deck and prepare for the next deal
                return "shuffle";
            }
            if(tokens[0] == "reveal")
            {
                *outputDetails = "";
                //flip the display of the dealers hole card, so the user can see
                return "reveal";
            }
            if(tokens[0] == "lock")
            {
                *outputDetails = tokens[1];
                //flip the display of the dealers hole card, so the user can see
                return "lock";
            }
        }
        else if (nextLine.at(0) == '+')
        {
            nextLine = nextLine.removeFirst();
            *outputDetails = nextLine.removeFirst();
            return "addCards";
        }
        else
        {
            //We should be throwing at this point. If we are not starting with a '/' or '*'
            //We are not following the contract. If it is empty, we should close this script otherwise they tried to open something that they shouldn't
            if(nextLine.isEmpty())
            {
                *outputDetails = "";
                return "finish";
            }
            return "error";
        }

    }
    else
    {
        *outputDetails = "";
        //We made it to the end of the file. End the Module, close the file, and return to home screen
        return "finish";
    }
    return "error";
}

std::vector<QString> Script::tokenize(QString providedString)
{
    std::vector<QString> tokens;
    QStringList list1 = providedString.split(u' ');
    for(QString s:list1)
        tokens.push_back(s);
    return tokens;
}

Script& Script::operator=(Script& other)
{
    std::swap(lines, other.lines);
    std::swap(lineNum, other.lineNum);
    return *this;
}
