#include "SmiteLog.h"
#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

void read_smite_log(string filename) {
  vector<unordered_map<string,string>> line_vector;
	std::ifstream infile(filename);
	if (infile.is_open()) {
		for(string line; getline(infile, line);){
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
  unordered_map<string,string> player_name;
  vector<string> abilities;
  unordered_map<string,pair<int,int>> minion_damage;
  unordered_map<string,pair<int,int>> god_damage;
  for(unordered_map<string,string> line_map : line_vector){
    if(line_map["type"]=="PCET_Spawn"){
      player_name[line_map["playername"]]=line_map["godname"];
    }else if(line_map["type"]=="DIT_Damage"&&line_map["sourceowner"]=="FireZerg"){
      string ability_name = line_map["itemname"];
      for(int i = 0; i<ability_name.size(); i++)
        ability_name[i] = toupper(ability_name[i]);
      int before_name = line_map["text"].find("hit");
      int str_for = line_map["text"].find("for");
      int after_damage = line_map["text"].find("damage");
      int damage = stoi(line_map["text"].substr(str_for+4,after_damage-str_for-4));
      string hit_name = line_map["text"].substr(before_name+4,str_for-before_name-5);
      if(minion_damage.find(ability_name)==minion_damage.end()&&god_damage.find(ability_name)==god_damage.end()){
        if(player_name.find(hit_name)!=player_name.end()){
          god_damage[ability_name]=pair<int,int>(1,damage);
          minion_damage[ability_name]=pair<int,int>(0,0);
        }else{
          god_damage[ability_name]=pair<int,int>(0,0);
          minion_damage[ability_name]=pair<int,int>(1,damage);
        }
        abilities.push_back(ability_name);
      }else{
        if(player_name.find(hit_name)!=player_name.end()){
          god_damage[ability_name].first++;
          god_damage[ability_name].second+=damage;
        }else{
          minion_damage[ability_name].first++;
          minion_damage[ability_name].second+=damage;
        }
      }
    }
  }
  for(string names : abilities){
    cout<<names<<endl;
    cout<<"God's hit "<<god_damage[names].first<<" times for a total of "<<god_damage[names].second<<" damage."<<endl;
    cout<<"Minions's hit "<<minion_damage[names].first<<" times for a total of "<<minion_damage[names].second<<" damage."<<endl<<endl;
  }
};
