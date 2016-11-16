//
//  GameRole.h
//  Sunny Doll
//
//  Created by zhangsimon on 16/8/26.
//
//

#ifndef GameRole_h
#define GameRole_h

#include "GameRoleFSM.h"
#include "Constants.h"

using namespace std;
USING_NS_CC;

class GameRole : public Sprite
{
public:
    static GameRole * create(const string & roleName);
    
    GameRoleFSM* getFSM() const;
    
    void doAction(const string& actionName, map<string, void*>& userdata) const;
    void doAction(const string& actionName, string userdata = "") const;
    void doAction(const string& actionName, Vec2 pos) const;
    void doAction(const string& actionName, GameRole* role) const;
    void addItem(const string& itemName, int count = 1);
    
    bool IsMovable() const;
    
    friend class StateDrown;
    friend class StateWalk;
    friend class StateIdle;
    friend class StateCry;
    
private:
    Texture2D* origText;
    GameRoleFSM* mFSM;
    bool _isMovable = true;
    GameRole* roleFollowed;
    map<string, int> itemList;
    
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
    
    void cry();
    
    void think(const string &, CallFunc* callback = nullptr);
    
    void follow(GameRole* role);
    
    void unfollow();
    
    // scheduler
    void updateFollow(float dt);
};


#endif /* GameRole_h */
