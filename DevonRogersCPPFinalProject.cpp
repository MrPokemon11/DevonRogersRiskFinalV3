// DevonRogersCPPFinalProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

using namespace std;

int numPlayers = 0;

//NOTE TO SELF: use map so that you don't need to make your own graph functions

//forward declaration of the continent and country classes
class Continent;
class Country;

//the following line of code was obtained from here: https://stackoverflow.com/a/50379457
using gameBoard = std::map<string, std::vector<string>>; //unsure about this line, so i shouldn't be afraid to change it if needed

gameBoard GameBoard;
//class for players
class Player {
public:
    Player(int ID);

    void addArmies(int amount);
    int getOwned() const { return numCountriesOwned; };
protected:
    int reserveArmies = 0; //how many armies the player has to reinforce with
    int numCountriesOwned = 0; //how many countries the player controls
    int numExchanges = 0; //how many exchanges the player has done
    string playerName; //the player's name
    int playerID; //the player's ID number
private:
};
//constructor for player class
Player::Player(int ID) {
    playerID = ID;
    cout << "Player " << playerID << ", please enter your name.\n";
    cin >> playerName;
}
//adds (and subtracts) the number of armies a player has
void Player::addArmies(int amount) {
    reserveArmies += amount;
}
//vector of player classes
vector<Player> players;

string removeExtraSpace(string input) {
    //this function removes the first character of a string. it is only called when the first character is a space. function based off of this code: https://stackoverflow.com/a/39546561 
    for (int i = 0;i < input.length() - 1;i++)
    {
        input[i] = input[i + 1]; //move all element to the left except first one
    }
    input.pop_back();
    return input;

}

//class for the overall game board
class Map {
public:
    int numContinents; //how many continents there are on the map; if a player controls a number of continents equal to this number they win
    void readGraph(); //this function will eventually read graphs from text files
    Country findCountryOfName(string name);
protected:
    vector<Continent> mapContinents;
    vector<Country> mapCountries;

};

//class for continents
class Continent : public Map {
public:
    int getControlValue() const { return controlValue; }
    Continent(int contVal, string contName);
protected:
    int controlValue; //how much controlling the whole continent gives
    int numberOfCountries; //how many countries you need to control to get the army bonus
    string continentName; //the name of the continent
};
//constructor
Continent::Continent(int contVal, string contName) {
    controlValue = contVal;
    continentName = contName;
}

//class for countries
class Country : public Map {
public:
    int getOwnedPlayerID() const { return ownedPlayerID; }//returns the ID of the player that controls this country
    void setOwnedPlayerID(int id) { ownedPlayerID = id; }//sets the id of the player that controls this country
    int getArmies() const { return numArmies; } //returns how many armies this country contains
    void modifyArmies(int num); //adds and subtracts armies based on the input
    Country(string name, string parentCont);
    Country(string fileInput);
    string getCountryName() { return countryName; }
    string getParentContinent() { return parentContinent; }
private:
protected:
    int numArmies = 0; //number of armies in this country
    int ownedPlayerID = -1; //-1 means that no player controls this country yet
    string countryName; //the name of the country
    vector<string> connectedCountries; //vector of countries connected to this one; not an array because this can have any amount
    string parentContinent; //which continent this country is a part of
};
//this constructor makes a country not via a file input
Country::Country(string name, string parentCont) {
    countryName = name;
    parentContinent = parentCont;
    
}
//this constructor is used to make a country via a file input
Country::Country(string fileInput) {
    vector<string> tempArray;
    vector<string> tempVector;

    std::istringstream iss(fileInput);
    string s;
    while (std::getline(iss, s, ';')) {
        tempArray.push_back(s);
    }

    //this function  function based off of this code: https://stackoverflow.com/a/39546561 
    if (tempArray[1][0] == ' ') {
        tempArray[1] = removeExtraSpace(tempArray[1]);
    }

    countryName = tempArray[0];
    parentContinent = tempArray[1];

    char trash;

    std::istringstream iss2(tempArray[2]);
    string s2;
    while (std::getline(iss2, s2, ',')) {
        while (s2[0] == ' ' || s2[0] == '{' || s2[0] == '}') {
            s2 = removeExtraSpace(s2);
        }
        if (s2.back() == '}') {
            s2.pop_back();
        }
        tempVector.push_back(s2);
    }

    //debug stuff
    cout << countryName << endl << parentContinent << endl;
    for (int i = 0; i < tempVector.size();i++) {
        cout << tempVector[i] << endl;
    }
}

Country Map::findCountryOfName(string name) {
    
    Country NullIsland("null", "");//this country is not real; it only exists because i can't use "NULL" as the default value

    for (int i = 0; i < mapCountries.size();i++) {
        if (name.length() != mapCountries[i].getCountryName().length()) {
            continue;
        }
        for (int j = 0; j < name.length();j++) {
            if (tolower(name[j]) == tolower(mapCountries[i].getCountryName()[j])) {
                if (j == name.length()) {
                    return mapCountries[i];
                }
                continue;
            }
            else {
                break;
            }
        }

    }
    cout << "No country of the given name found.\n"; //if the given name isn't found in the list of countries, tell the player and return null.
    return NullIsland;
}

void Map::readGraph() {
//put stuff here eventually
    //this holds the file the map is stored in
    ifstream MapData("RiskMapGraph.txt");
    //this holds whatever the stream just pulled
    string fileDat;

    bool doneContinents = false;
    bool doneCountries = false;

    if (MapData.ifstream::is_open()) {
        while (getline(MapData, fileDat)) {
            if (fileDat[0] == '#') {//this allows the insertion of comments by skipping lines that start with the # symbol
                continue;
            }

            if (doneContinents == false) {
                if (fileDat == "Countries") {
                    doneContinents = true;
                    continue;
                }

            }
            else if (doneCountries == false) {
                if (fileDat == "End") {
                    doneCountries = true;
                    continue;
                }
                Country* newCountry;
                newCountry = new Country(fileDat);
                mapCountries.push_back(*newCountry);
            }
            else {
                cout << "Map loaded!\n";
                break;
            }
        }
    }
    else {
        cout << "Map not found! Make sure it has not been renamed and is in the correct folder.\n";
    }
    MapData.ifstream::close();
}

void Country::modifyArmies(int num) {
    numArmies += num;
}


int main()
{

    std::cout << "Please enter the number of players:\n";
    cin >> numPlayers;
    for (int i = 0; i < numPlayers; i++) {
        players.push_back(Player(i + 1));
    }
    

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
