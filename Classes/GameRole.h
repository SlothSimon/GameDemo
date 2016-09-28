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
    
    void turnAround();
    
    void idle();
    
    void walk(const Vec2 &);
    
    void drown();
    
    void think(const string &);
    
    GameRoleFSM* getFSM() const;
    
    bool IsMovable() const;
    
private:
    Texture2D* origText;
    GameRoleFSM* mFSM;
    bool _isMovable = true;
    
    void initAnim();
    
    void initPhysicsBody();
    
    void initListener();
    
    void addAnim(const string &);
};


#endif /* GameRole_h */
