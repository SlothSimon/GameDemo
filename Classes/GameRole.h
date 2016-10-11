//
//  GameRole.h
//  Escape
//
//  Created by zhangsimon on 16/8/26.
//
//

#ifndef GameRole_h
#define GameRole_h

#include "GameRoleFSM.h"

using namespace std;
USING_NS_CC;

class GameRole : public Sprite
{
public:
    static GameRole * create(const string & roleName);
    
    GameRoleFSM* getFSM() const;
    
    void doAction(const string& , void* userdata = nullptr) const;
    
    bool IsMovable() const;
    
    friend class StateDrown;
    friend class StateWalk;
    friend class StateIdle;
    
private:
    Texture2D* origText;
    GameRoleFSM* mFSM;
    bool _isMovable = true;
    
    void initAnim();
    
    void initPhysicsBody();
    
    void initListener();
    
    void addAnim(const string &);
    
    
    // State Machine
    void changeState(State* state) const;
    
    void turnAround();
    
    void idle();
    
    void walk(const Vec2 &);
    
    void drown();
    
    void think(const string &, CallFunc* callback = nullptr);
};


#endif /* GameRole_h */
