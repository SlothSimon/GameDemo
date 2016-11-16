//
//  Constants.cpp
//  Sunny Doll
//
//  Created by zhangsimon on 16/9/27.
//
//

#include "Constants.h"
#include "GameRole.h"

const map<string, float> PhysicsBodyParameters::CreateMapFriction()
{
    map<string, float> m;
    m["Slope"]  = 2.0f;
    m["Ground"] = 1.0f;
    m["Water"]  = 0.0f;
    return m;
}

const map<string, float> PhysicsBodyParameters::MapFriction = CreateMapFriction();

const float PhysicsBodyParameters::getFriction(const string &k){
    if (MapFriction.find(k) != MapFriction.cend())
        return MapFriction.at(k);
    else
        return 1.0f;
}


const string GameRoleName::Doll = "doll";
const string GameRoleName::Girl = "girl";

const string GameRoleState::convertToEventName(const GameRole* role, const string& s)
{
    return role->getName() + "_" + s;
}

const string GameRoleState::convertToStateName(const string &s)
{
    string state = s.substr(s.find("_") + 1);
    if (state != ""){
        return state;
    }
    return NULL;
}

const string GameRoleState::State::Idle = "idle";
const string GameRoleState::State::Think = "think";
const string GameRoleState::State::Say = "say";
const string GameRoleState::State::Drown = "drown";
const string GameRoleState::State::Walk  = "walk";
const string GameRoleState::State::Cry = "cry";
const string GameRoleState::State::Follow = "follow";
const string GameRoleState::State::Unfollow = "unfollow";
const string GameRoleState::ThinkContent::Drown = "think_drown";
const string GameRoleState::ThinkContent::Walk  = "think_walk";
const string GameRoleState::SayContent::Love = "say_love";
const string GameRoleState::SayContent::Shock = "say_shock";
const string GameRoleState::SayContent::Cry = "say_cry";
const string GameRoleState::SayContent::Ask = "say_ask";
const string GameRoleState::SayContent::Think = "say_think";
const string GameRoleState::SayContent::Rain = "say_rain";
const string GameRoleState::SayContent::Story1 = "say_story1";
const string GameRoleState::SayContent::Story2 = "say_story2";
const string GameRoleState::SayContent::House = "house";


const char* MusicPath::normalBGM   = "music/firstlove_light.mp3";
const char* MusicPath::RainEffect  = "music/rain.wav";

const char* ImagePath::MenuButton  = "button_menu.png";
const char* ImagePath::SunnyButton = "button_sun.png";
const char* ImagePath::RainyButton = "button_rain.png";
const char* ImagePath::Sun         = "sun.png";
const char* ImagePath::Balloon     = "map/balloon.png";
const map<string, const char*> ImagePath::BubbleMap = ImagePath::CreateBubbleMap();

const string ImagePath::getMapPath(const string& mapName) {
    return "map/" + mapName + ".tmx";
}

const char* ImagePath::getRoleFramePath(const string &roleName, const string &animName, const int &frameIndex){
    char* path = new char[50];
    if (frameIndex == (int)NULL)
        sprintf(path, "roles/%s_%s.png", roleName.c_str(), animName.c_str());
    else
        sprintf(path, "roles/%s_%s_%d.png", roleName.c_str(), animName.c_str(), frameIndex);
    return path;
}

const char* ImagePath::getBubblePath(const string& bubbleName){
    auto res = BubbleMap.find(bubbleName);
    if (res != ImagePath::BubbleMap.cend())
        return res->second;
    else
        return nullptr;
}

const map<string, const char*> ImagePath::CreateBubbleMap(){
    map<string, const char*> m;
    m[GameRoleState::ThinkContent::Drown] = "roles/bubble/think_nowalk.png";
    m[GameRoleState::ThinkContent::Walk]  = "roles/bubble/think_walk.png";
    m[GameRoleState::SayContent::Love] = "roles/bubble/say_love.png";
    m[GameRoleState::SayContent::Shock] = "roles/bubble/say_shock.png";
    m[GameRoleState::SayContent::Cry] = "roles/bubble/say_cry.png";
    m[GameRoleState::SayContent::Ask] = "roles/bubble/say_ask.png";
    m[GameRoleState::SayContent::Rain] = "roles/bubble/say_rain.png";
    m[GameRoleState::SayContent::Think] = "roles/bubble/say_think.png";
    m[GameRoleState::SayContent::Story1] = "roles/bubble/say_story1.png";
    m[GameRoleState::SayContent::Story2] = "roles/bubble/say_story2.png";
    m[GameRoleState::SayContent::House] = "roles/bubble/say_house.png";
    return m;
}
