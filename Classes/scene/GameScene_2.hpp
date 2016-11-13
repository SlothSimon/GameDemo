//
//  GameScene_2.hpp
//  Sunny Doll
//
//  Created by zhangsimon on 2016/11/5.
//
//

#ifndef GameScene_2_hpp
#define GameScene_2_hpp

#include "GameScene.h"

class GameScene_2: public GameScene{
public:
    CREATE_GAMESCENE_SUBCLASS(GameScene_2);
private:
    bool isTogether = false;
    bool isFirstTalk = true;
};

#endif /* GameScene_2_hpp */
