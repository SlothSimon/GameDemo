//
//  GameScene_2.cpp
//  Sunny Doll
//
//  Created by zhangsimon on 2016/11/5.
//
//

#include "GameScene_2.hpp"
#include "GameScene_3.hpp"

bool GameScene_2::initSpecfic(){
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = [this](PhysicsContact & contact){
        auto doll = getChildByName(GameRoleName::Doll);
        auto girl = getChildByName(GameRoleName::Girl);
        
        if (doll == nullptr || girl == nullptr)
            return false;
        
        // TODO: contact cinematics
        
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    
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
            
            if (doll->getPosition().distance(touch->getLocation()) <= INTERACTION_RANGE){
                pushCinematic(new Cinematic(doll, GameRoleState::State::Say, -1, GameRoleState::SayContent::Ask));
                pushCinematic(new Cinematic(girl, GameRoleState::State::Say, -1, GameRoleState::SayContent::Think));
                pushCinematic(new Cinematic(girl, GameRoleState::State::Say, -1, GameRoleState::SayContent::Story1));
                pushCinematic(new Cinematic(girl, GameRoleState::State::Say, -1, GameRoleState::SayContent::Rain));
                pushCinematic(new Cinematic(girl, GameRoleState::State::Say, -1, GameRoleState::SayContent::Story2));
                pushCinematic(new Cinematic(girl, GameRoleState::State::Say, -1, GameRoleState::SayContent::Cry));
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

void GameScene_2::onEnterTransitionDidFinish(){
    auto doll = getChildByName(GameRoleName::Doll);
    auto girl = getChildByName(GameRoleName::Girl);
    pushCinematic(new Cinematic(doll, GameRoleState::State::Say, 1, GameRoleState::SayContent::Shock));
    pushCinematic(new Cinematic(girl, GameRoleState::State::Say, -1, GameRoleState::SayContent::Shock));
    pushCinematic(new Cinematic(girl, GameRoleState::State::Say, -1, GameRoleState::SayContent::Cry));
}

void GameScene_2::enterStage(){
    UserDefault::getInstance()->setIntegerForKey("currentStage", 3);
    Director::getInstance()->replaceScene(TransitionFade::create(2, GameScene_3::createScene()));
}
