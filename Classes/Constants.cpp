//
//  Constants.cpp
//  Sunny Doll
//
//  Created by zhangsimon on 16/9/27.
//
//

#include "Constants.h"
#include "GameRole.h"

#define VtoString(x) #x

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

const string GameRoleState::toString(GameRole* role, const State s)
{
    return role->getName() + "_" + StateName[(int)s];
}

const GameRoleState::State GameRoleState::toEnum(const string &s)
{
    string state = s.substr(s.find("_") + 1);
    if (state != ""){
        if (StateMap.find(state) != StateMap.cend())
            return StateMap.at(state);
            else
                return State::Error;
    }
    return State::Error;
}

const map<string, GameRoleState::State> GameRoleState::CreateStateMap()
{
    map<string, State> m;
    for (int i = (int)State::Walk ; i <= (int)State::Think; i = i + 1){
        State s = (State)i;
        m[StateName[i]] = s;
    }
    return m;
}

const char* GameRoleState::StateName[] = {
    VtoString(Error),
    VtoString(Walk),
    VtoString(Idle),
    VtoString(Drown),
    VtoString(Think)
};

const string GameRoleState::ThinkContent::Drown = "think_drown";
const string GameRoleState::ThinkContent::Walk  = "think_walk";
const std::map<string, GameRoleState::State> GameRoleState::StateMap = CreateStateMap();

const char* MusicPath::normalBGM   = "music/firstlove_light.mp3";
const char* MusicPath::RainEffect  = "music/rain.wav";

const char* ImagePath::MenuButton  = "button_menu.png";
const char* ImagePath::SunnyButton = "button_sun.png";
const char* ImagePath::RainyButton = "button_rain.png";
const char* ImagePath::Sun         = "sun.png";
const map<string, const char*> ImagePath::BubbleMap = ImagePath::CreateBubbleMap();

const string ImagePath::getMapPath(const string& mapName) {
    return "map/" + mapName + ".tmx";
}

const char* ImagePath::getRoleFramePath(const string &roleName, const string &animName, const int &frameIndex){
    char* path = new char[50];
    sprintf(path, "roles/%s_%s_%d.png", roleName.c_str(), animName.c_str(), frameIndex);
    return path;
}

const map<string, const char*> ImagePath::CreateBubbleMap(){
    map<string, const char*> m;
    m[GameRoleState::ThinkContent::Drown] = "roles/bubble/think_drown.png";
    m[GameRoleState::ThinkContent::Walk]  = "roles/bubble/think_walk.png";
    return m;
}
