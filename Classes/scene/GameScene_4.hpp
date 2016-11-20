//
//  GameScene_4.hpp
//  Sunny Doll
//
//  Created by zhangsimon on 2016/11/19.
//
//

#ifndef GameScene_4_hpp
#define GameScene_4_hpp

#include "GameScene.h"
using namespace std;
USING_NS_CC;

class GameScene_4: public GameScene{
public:
    CREATE_GAMESCENE_SUBCLASS(GameScene_4);
private:
    void updateFire(float dt);
    
    void beRainyGround();

};

#endif /* GameScene_4_hpp */
