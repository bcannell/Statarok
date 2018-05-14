#ifndef _SMITELOG_H
#define _SMITELOG_H

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class SmiteLogInst
{
public:
    SmiteLogInst(string file);
    void makeHeatMap();
    void printDamageDealt();
    void printDamageTaken();
private:
    vector<unordered_map<string,string>> line_vector;
};

#endif
