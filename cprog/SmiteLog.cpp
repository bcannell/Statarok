#include "SmiteLog.h"
#include "PNG.h"

#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

SmiteLogInst::SmiteLogInst(string filename) {
    loadSmiteLog(filename);
}

void SmiteLogInst::loadSmiteLog(string filename){
    line_vector=vector<unordered_map<string,string>>();
    player_names=vector<string>();
    unordered_map<string,int> player_count;
    std::ifstream infile(filename);
    if(infile.is_open()){
        for(string line; getline(infile, line);){
            unordered_map<string,string> line_map;
            int before = 0;
            int equals = 0;
            int after = 0;
            while(1){
                after = line.find('|',before);
                if(after==(int)string::npos)
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
            if(line_map["type"]=="PCET_Spawn"){
                player_names.push_back(line_map["playername"]);
                player_count[line_map["playername"]]=0;
            }
            player_count[line_map["sourceowner"]]++;
        }
    }
    infile.close();
    string common_name = player_names[0];
    for(string pname : player_names)
        if(player_count[common_name]<player_count[pname])
            common_name = pname;
    setCurrentPlayer(common_name);
}

void SmiteLogInst::setCurrentPlayer(string name){
    if(std::find(player_names.begin(), player_names.end(), name) != player_names.end()){
        current_player=name;
    }else{
        cout<<"Not a valid name!";
    }
}

vector<string> SmiteLogInst::getItemsBuilt(){
    vector<string> items;
    for(unordered_map<string,string> line_map : line_vector){
        if(line_map["type"]=="IET_ItemPurchase"){
            items.push_back(line_map["itemname"]);
        }
    }
    return items;
}

vector<pair<int,int>> SmiteLogInst::returnGraphData(string type_name){
    vector<pair<int,int>> VTimeQuantity;
    int total=0;
    int t=0;
    for(unordered_map<string,string> line_map : line_vector){
        if(line_map.find("time")!=line_map.end()){
            if(type_name=="DmgDlt"){
                if((line_map["type"]=="DIT_Damage"||line_map["type"]=="DIT_CritDamage")&&line_map["sourceowner"]==current_player&&std::find(player_names.begin(), player_names.end(), line_map["target"]) != player_names.end()){

                        t=stoi(line_map["time"]);
                        total+=stoi(line_map["value1"]);
                        VTimeQuantity.push_back(pair<int,int>(t,total));
                }else if((line_map["type"]=="IET_CastStart"||line_map["type"]=="IET_CastInterrupt")&&line_map["source"]==current_player){
                    t=stoi(line_map["time"]);
                    total+=stoi(line_map["value1"]);
                    VTimeQuantity.push_back(pair<int,int>(t,total));
                }
            }
            if(type_name=="DmgTkn"){
                if((line_map["type"]=="DIT_Damage"||line_map["type"]=="DIT_CritDamage")&&line_map["sourceowner"]!=current_player){
                    t=stoi(line_map["time"]);
                    total+=stoi(line_map["value1"]);
                    VTimeQuantity.push_back(pair<int,int>(t,total));
                }
            }
            if(type_name=="Heal"){
                if(line_map["type"]=="DIT_Healing"){
                    t=stoi(line_map["time"]);
                    total+=stoi(line_map["value1"]);
                    VTimeQuantity.push_back(pair<int,int>(t,total));
                }
                if(line_map["type"]=="DIT_ManaRestore"){
                    t=stoi(line_map["time"]);
                    total+=stoi(line_map["value1"]);
                    VTimeQuantity.push_back(pair<int,int>(t,total));
                }
            }
            if(type_name=="GoldExp"){
                if(line_map["type"]=="DIT_Currency"){
                    t=stoi(line_map["time"]);
                    total+=stoi(line_map["value1"]);
                    VTimeQuantity.push_back(pair<int,int>(t,total));
                }
            }
        }
    }
    return VTimeQuantity;
}

void SmiteLogInst::makeHeatMap(string type_name){
    bool isEverything=false;
    if(type_name=="Everything")
        isEverything = true;
    cs225::PNG smitemap;
    double x,y,pixel_hue,pixel_sat,pixel_lum;
    bool color_me;
    smitemap.readFromFile("images/smitemap.png");
    for(unordered_map<string,string> line_map : line_vector){
        if(line_map.find("locationx")!=line_map.end()&&line_map.find("locationy")!=line_map.end()){
            color_me=false;
            y=stoi(line_map["locationx"]);
            x=stoi(line_map["locationy"]);
			y=-1*y/19.45+528;
			x=x/19.45+400;
            if(isEverything)
                type_name="DmgDlt";
            if(type_name=="DmgDlt"){
                if((line_map["type"]=="DIT_Damage"||line_map["type"]=="DIT_CritDamage")&&line_map["sourceowner"]==current_player){
                    if(std::find(player_names.begin(), player_names.end(), line_map["target"]) != player_names.end())
                        pixel_hue=0;
                    else
                        pixel_hue=50;
                    pixel_sat=1;
                    pixel_lum=.55;
                    color_me=true;
                }else if((line_map["type"]=="IET_CastStart"||line_map["type"]=="IET_CastInterrupt")&&line_map["source"]==current_player){
                    pixel_hue=0;
                    pixel_sat=0;
                    pixel_lum=.77;
                    color_me=true;
                }
            }
            if(isEverything)
                type_name="DmgTkn";
            if(type_name=="DmgTkn"){
                if((line_map["type"]=="DIT_Damage"||line_map["type"]=="DIT_CritDamage")&&line_map["sourceowner"]!=current_player){
                    if(std::find(player_names.begin(), player_names.end(), line_map["sourceowner"]) != player_names.end())
                        pixel_hue=0;
                    else
                        pixel_hue=60;
                    pixel_sat=1;
                    pixel_lum=.55;
                    color_me=true;
                }
            }
            if(isEverything)
                type_name="Heal";
            if(type_name=="Heal"){
                if(line_map["type"]=="DIT_Healing"){
                    pixel_hue=120;
                    pixel_sat=1;
                    pixel_lum=.55;
                    color_me=true;
                }
                if(line_map["type"]=="DIT_ManaRestore"){
                    pixel_hue=217;
                    pixel_sat=1;
                    pixel_lum=.55;
                    color_me=true;
                }
            }
            if(isEverything)
                type_name="GoldExp";
            if(type_name=="GoldExp"){
                if(line_map["type"]=="DIT_Currency"){
                    pixel_hue=50;
                    pixel_sat=.80;
                    pixel_lum=.55;
                    color_me=true;
                }
            }
            if(color_me){
                for(int i = -2; i<3; i++)
                    for(int j = -2; j<3; j++){
                        smitemap.getPixel(x+i,y+j).h=pixel_hue;
                        smitemap.getPixel(x+i,y+j).s=pixel_sat;
                        smitemap.getPixel(x+i,y+j).l=pixel_lum;
					}
            }
		}
	}
    smitemap.writeToFile("images/smitemap1.png");
}
