//
//  GameRoleFSM.h
//  Sunny Doll
//
//  Created by zhangsimon on 16/9/24.
//
//

#ifndef GameRoleFSM_h
#define GameRoleFSM_h

#include "cocos2d.h"
USING_NS_CC;

class State;
class GameRole;

class GameRoleFSM: public Node {
public:
    ~GameRoleFSM();
    
    static GameRoleFSM* createWithGameRole(GameRole* role);
    bool initWithGameRole(GameRole* role);
    
    void changeState(State* state);
    
private:
    void onRecvWalk(Ref* obj);
    void onRecvIdle(Ref* obj);
    void onRecvDrown(Ref* obj);
    
    State* curState;
    
    GameRole* role;
};

#endif /* GameRoleFSM_h */
