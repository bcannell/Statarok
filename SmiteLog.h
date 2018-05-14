#ifndef _SMITELOG_H
#define _SMITELOG_H

#include <string>
<<<<<<< HEAD
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
=======

void read_smite_log(std::string file);
>>>>>>> 6a153fa061542863f9113298dfcbf3badc3502ea

#endif
