#include "SmiteLog.h"
#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

void read_smite_log(string filename) {
  vector<unordered_map<string,string>> line_vector;
	std::ifstream infile(filename);
	if (infile.is_open()) {
		for(std::string line; getline(infile, line);){
      unordered_map<string,string> line_map;
      int before = 0;
      int equals = 0;
      int after = 0;
      while(1){
        after = line.find('|',before);
        if(after==(int)std::string::npos)
          break;
        equals = line.find('=',before);
        if(equals>after)
          line_map.insert({line.substr(before,after),""});
        else
          line_map.insert({line.substr(before,equals-before),line.substr(equals+1,after-equals-1)});
        before=after+1;
      }
      before = line.find_last_of('|');
      equals = line.find_last_of('=');
      if(before>=0){
        if(equals<before)
          line_map.insert({line.substr(before+1,line.length()-1),""});
        else
          line_map.insert({line.substr(before+1,equals-before-1),line.substr(equals+1,line.length()-1-equals)});
        line_vector.push_back(line_map);
      }
    }
	}
	infile.close();
  int total_damage = 0;
  for(unordered_map<string,string> line_map : line_vector){
    if(line_map["type"]=="DIT_Damage"&&line_map["sourceowner"]=="FireZerg"){
      int before = line_map["text"].find("for");
      int after = line_map["text"].find("damage");
      total_damage += stoi(line_map["text"].substr(before+3,after));
    }
  }
  cout<<total_damage<<endl;
};
