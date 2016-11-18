//
//  GameScene_1.hpp
//  Sunny Doll
//
//  Created by zhangsimon on 2016/11/5.
//
//

#ifndef GameScene_1_hpp
#define GameScene_1_hpp

#include "GameScene.h"

// TODO: maybe I should merge all subclass declarations of chapter 1 into one header file.
class GameScene_1: public GameScene{
public:
    CREATE_GAMESCENE_SUBCLASS(GameScene_1);
private:
    void onEnterTransitionDidFinish();
};
    
#endif /* GameScene_1_hpp */
