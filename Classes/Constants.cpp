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
    m["Slope"] = 2.0f;
    m["Ground"] = 1.0f;
    m["Water"] = 0.0f;
    return m;
}

const map<string, float> PhysicsBodyParameters::MapFriction = CreateMapFriction();

const float PhysicsBodyParameters::getFriction(const string &k){
    if (MapFriction.find(k) != MapFriction.cend())
        return MapFriction.at(k);
    else
        return 1.0f;
}

const string GameRoleState::toString(GameRole* role, const State s)
{
    return role->getName() + "_" + StateName[s];
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
    for (int i = Walk ; i <= Think; i = i + 1){
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

const std::map<string, GameRoleState::State> GameRoleState::StateMap = CreateStateMap();
