//
//  GameScene_3.cpp
//  Sunny Doll
//
//  Created by zhangsimon on 2016/11/12.
//
//

#include "GameScene_3.hpp"
#include "GameScene_4.hpp"

bool GameScene_3::initSpecfic(){
    auto girl = dynamic_cast<GameRole*>(getChildByName(GameRoleName::Girl));
    auto doll = dynamic_cast<GameRole*>(getChildByName(GameRoleName::Doll));
    
    if (girl == NULL || doll == NULL)
        return false;
    
    girl->follow(doll);
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    
    touchListener->onTouchBegan = [this](Touch * touch, Event * event){
        auto girl = dynamic_cast<GameRole*>(getChildByName(GameRoleName::Girl));
        auto doll = dynamic_cast<GameRole*>(getChildByName(GameRoleName::Doll));
        if (girl == NULL)
            return false;
        
        auto loc = girl->convertTouchToNodeSpace(touch);
        auto rect = girl->getTextureRect();
        
        if (rect.containsPoint(loc)){
            if (doll == NULL)
                return false;
            
            if (doll->getPosition().distance(touch->getLocation()) <= INTERACTION_MESSAGE_RANGE){
                doll->loadGirl();
            }
            else
                doll->doAction(GameRoleState::State::Think, GameRoleState::ThinkContent::Walk);
            return true;
        }
        
        loc = doll->convertTouchToNodeSpace(touch);
        rect = doll->getTextureRect();
        if (rect.containsPoint(loc) && doll->IsMovable()){
            doll->unloadGirl();
            return true;
        }
        
        return false;
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, getChildByName(GameRoleName::Girl));
    
    return true;
}

void GameScene_3::enterStage(){
    GameScene::enterStage();
    saveStage(4);
    Director::getInstance()->replaceScene(TransitionFade::create(2, GameScene_4::createScene()));
}
