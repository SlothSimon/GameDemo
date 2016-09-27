//
//  GameRoleFSM.h
//  Sunny Doll
//
//  Created by zhangsimon on 16/9/24.
//
//

#ifndef GameRoleFSM_h
#define GameRoleFSM_h

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
    void onRecvWalk(EventCustom* event);
    void onRecvIdle(EventCustom* event);
    void onRecvDrown(EventCustom* event);
    
    State* curState;
    
    GameRole* role;
};

#endif /* GameRoleFSM_h */
