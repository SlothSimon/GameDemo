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

/**
 * Constants about debug, development and system
 */
struct DebugParameters{
    const static int StartStage = 1;
    const static bool DoDebug = true;
    const static int EndStage = 2;
};

/**
 * Constants about Physics
 */
class PhysicsBodyParameters {
public:
    const static map<string, float> MapFriction;
    const static float getFriction(const string &);
    
private:
    const static map<string, float> CreateMapFriction();

};

/**
 * Constants about GameRole
 */
struct GameRoleName {
    const static string Doll;
    const static string Girl;
};

class GameRoleState {
public:
    enum class State {Error, Walk, Idle, Drown, Think};
    
    struct ThinkContent{
        const static string Walk;
        const static string Drown;
    };
    
    const static std::map<string, State> StateMap;
    
    const static string toString(GameRole* role, const State s);
    
    const static State toEnum(const string &s);

private:
    const static char* StateName[];
    const static map<string, State> CreateStateMap();
    
};

#define StateToString(vGameRole, vState) GameRoleState::toString(vGameRole, GameRoleState::State::vState)

/**
 * Constants about filepath
 */
struct MusicPath {
    const static char* normalBGM;
    const static char* RainEffect;
    
};

struct ImagePath {
    const static char* MenuButton;
    const static char* SunnyButton;
    const static char* RainyButton;
    const static char* Sun;
    const static string getMapPath(const string& mapName);
    const static char* getRoleFramePath(const string& roleName, const string& animName, const int& frameIndex);
    const static map<string, const char*> BubbleMap;
private:
    const static map<string, const char*> CreateBubbleMap();
};

#endif /* Constants_h */
