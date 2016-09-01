//
//  GameRole.h
//  Escape
//
//  Created by zhangsimon on 16/8/26.
//
//

#ifndef GameRole_h
#define GameRole_h
using namespace std;
USING_NS_CC;

class GameRole : public Sprite
{
public:
    static GameRole * create(const string & roleName);
    
    void startWalk(const Vec2 & pos);
    
    void turnAround();
    
    void stopWalk();
    
private:
    Texture2D* origText;
    
    void initAnim();
    
    void initPhysicsBody();
    
    void addAnim(const string &);
};


#endif /* GameRole_h */
