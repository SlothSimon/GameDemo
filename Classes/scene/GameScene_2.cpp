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
        if (isTogether)
            return true;
        
        auto r1 = contact.getShapeA()->getBody()->getNode();
        auto r2 = contact.getShapeB()->getBody()->getNode();
        
        GameRole* doll = nullptr;
        GameRole* girl = nullptr;
        
        if (r1->getName() == GameRoleName::Doll)
            doll = dynamic_cast<GameRole*>(r1);
        else if (r2->getName() == GameRoleName::Doll)
            doll = dynamic_cast<GameRole*>(r2);
        
        if (r2->getName() == GameRoleName::Girl)
            girl = dynamic_cast<GameRole*>(r2);
        else if (r1->getName() == GameRoleName::Girl)
            girl = dynamic_cast<GameRole*>(r1);
        
        if (doll == NULL || girl == NULL)
            return true;
        
        doll->doAction(GameRoleState::State::Idle);
        pushCinematic(new Cinematic(girl, GameRoleState::State::Say, -1, GameRoleState::SayContent::Love));
        girl->doAction(GameRoleState::State::Follow, doll);
        isTogether = true;
        
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
            
            if (doll->getPosition().distance(touch->getLocation()) <= INTERACTION_MESSAGE_RANGE){
                if (isFirstTalk){
                    pushCinematic(new Cinematic(doll, GameRoleState::State::Say, -1, GameRoleState::SayContent::Ask));
                    pushCinematic(new Cinematic(girl, GameRoleState::State::Say, -1, GameRoleState::SayContent::Think));
                    pushCinematic(new Cinematic(girl, GameRoleState::State::Say, -1, GameRoleState::SayContent::Story1));
                    pushCinematic(new Cinematic(girl, GameRoleState::State::Say, -1, GameRoleState::SayContent::Rain));
                    pushCinematic(new Cinematic(girl, GameRoleState::State::Say, -1, GameRoleState::SayContent::Story2));
                    pushCinematic(new Cinematic(girl, GameRoleState::State::Say, -1, GameRoleState::SayContent::Cry));
                    isFirstTalk = false;
                }else{
                    pushCinematic(new Cinematic(girl, GameRoleState::State::Say, -1, GameRoleState::SayContent::Love));
                    pushCinematic(new Cinematic(girl, GameRoleState::State::Say, -1, GameRoleState::SayContent::Story1));
                }
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

