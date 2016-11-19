//
//  GameScene_5.cpp
//  Sunny Doll
//
//  Created by zhangsimon on 2016/11/19.
//
//

#include "GameScene_5.hpp"
#include "EndScene.h"

bool GameScene_5::initSpecfic(){
    auto girl = dynamic_cast<GameRole*>(getChildByName(GameRoleName::Girl));
    auto doll = dynamic_cast<GameRole*>(getChildByName(GameRoleName::Doll));
    
    if (girl == NULL || doll == NULL)
        return false;
    
    girl->follow(doll);
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    
    touchListener->onTouchBegan = [this](Touch * touch, Event * event){
        auto girl = dynamic_cast<GameRole*>(getChildByName(GameRoleName::Girl));
        if (girl == NULL)
            return false;
        
        auto loc = girl->convertTouchToNodeSpace(touch);
        auto rect = girl->getTextureRect();
        
        if (rect.containsPoint(loc)){
            auto doll = dynamic_cast<GameRole*>(getChildByName(GameRoleName::Doll));
            if (doll == NULL)
                return false;
            
            if (doll->getPosition().distance(touch->getLocation()) <= INTERACTION_MESSAGE_RANGE){
                pushCinematic(new Cinematic(girl, GameRoleState::State::Say, -1, GameRoleState::SayContent::Love));
                pushCinematic(new Cinematic(girl, GameRoleState::State::Say, -1, GameRoleState::SayContent::Story1));
            }
            else
                doll->doAction(GameRoleState::State::Think, GameRoleState::ThinkContent::Walk);
            return true;
        }
        
        return false;
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, getChildByName(GameRoleName::Girl));
    
    return true;
}


void GameScene_5::enterStage(){
    GameScene::enterStage();
    saveStage(6);
    Director::getInstance()->replaceScene(TransitionFade::create(2, EndScene::createScene()));
}
