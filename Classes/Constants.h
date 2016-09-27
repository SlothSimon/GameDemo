//
//  Constants.h
//  Sunny Doll
//
//  Created by zhangsimon on 16/9/27.
//
//

#ifndef Constants_h
#define Constants_h

#include <map>
class GameRole;
using namespace std;

#define StateToString(vGameRole, vState) GameRoleState::toString(vGameRole, GameRoleState::State::vState)

class PhysicsBodyParameters {
public:
    const static map<string, float> MapFriction;
    const static float getFriction(const string &);
    
private:
    const static map<string, float> CreateMapFriction();

};

struct DebugParameters{
    const static int StartStage = 2;
    const static bool DoDebug = true;
    const static int EndStage = 2;
};

struct ThinkContent{
    const static string Walk;
    const static string Drown;
};

class GameRoleState {
public:
    enum State {Error, Walk, Idle, Drown, Think};
    
    const static std::map<string, State> StateMap;
    
    const static string toString(GameRole* role, const State s);
    
    const static State toEnum(const string &s);

private:
    const static char* StateName[];
    const static map<string, State> CreateStateMap();
    
};


#endif /* Constants_h */
