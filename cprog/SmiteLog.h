#ifndef _SMITELOG_H
#define _SMITELOG_H

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class SmiteLogInst
{
public:
    SmiteLogInst(){};
    SmiteLogInst(string file);
    void loadSmiteLog(string file);
    void setCurrentPlayer(string name);
    string getCurrentPlayer(){return current_player;};
    vector<string> getPlayerNames(){return player_names;};
    vector<pair<int,int>> returnGraphData(string type_name);
    vector<string> getItemsBuilt();
    void makeHeatMap(string type_name);
private:
    vector<unordered_map<string,string>> line_vector;
    string current_player;
    vector<string> player_names;
};

#endif
