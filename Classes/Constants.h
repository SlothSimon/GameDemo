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
    const static int StartStage = 2;
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
    struct State {
        const static string Error;
        const static string Walk;
        const static string Idle;
        const static string Drown;
        const static string Think;
        const static string Say;
        const static string Cry;
    };
    
    struct ThinkContent{
        const static string Walk;
        const static string Drown;
    };
    
    struct SayContent{
        const static string Love;
        const static string Shock;
        const static string Cry;
        const static string Ask;
        const static string Think;
        const static string Rain;
        const static string Story1;
        const static string Story2;
    };
    
    const static string convertToEventName(const GameRole* role, const string& s);

    const static string convertToStateName(const string &s);
    
};

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
    const static char* getRoleFramePath(const string& roleName, const string& animName, const int& frameIndex = NULL);
    const static map<string, const char*> BubbleMap;
private:
    const static map<string, const char*> CreateBubbleMap();
};

#endif /* Constants_h */
