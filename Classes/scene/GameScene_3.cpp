//
//  GameScene_3.cpp
//  Sunny Doll
//
//  Created by zhangsimon on 2016/11/12.
//
//

#include "GameScene_3.hpp"
#include "EndScene.h"

bool GameScene_3::initSpecfic(){
    
    
    
    return true;
}


void GameScene_3::onEnterTransitionDidFinish(){
    
}

void GameScene_3::enterStage(){
    UserDefault::getInstance()->setIntegerForKey("currentStage", 4);
    Director::getInstance()->replaceScene(TransitionFade::create(2, EndScene::createScene()));
}
