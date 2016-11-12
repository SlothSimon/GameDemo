//
//  GameScene_1.cpp
//  Sunny Doll
//
//  Created by zhangsimon on 2016/11/5.
//
//

#include "GameScene_1.hpp"
#include "GameScene_2.hpp"

bool GameScene_1::initSpecfic(){
    
    return true;
}


void GameScene_1::onEnterTransitionDidFinish(){
    // TODO: 第一关新手指导，另外push一个scene，然后再pop
    // TODO: Doll lies down, then stands up.
}

void GameScene_1::enterStage(){
    UserDefault::getInstance()->setIntegerForKey("currentStage", 2);
    Director::getInstance()->replaceScene(TransitionFade::create(2, GameScene_2::createScene()));
}
