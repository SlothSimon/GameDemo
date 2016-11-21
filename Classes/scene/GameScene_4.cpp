//
//  GameScene_4.cpp
//  Sunny Doll
//
//  Created by zhangsimon on 2016/11/19.
//
//

#include "GameScene_4.hpp"
#include "GameScene_5.hpp"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

bool GameScene_4::initSpecfic(){
    auto girl = dynamic_cast<GameRole*>(getChildByName(GameRoleName::Girl));
    auto doll = dynamic_cast<GameRole*>(getChildByName(GameRoleName::Doll));
    
    if (girl == NULL || doll == NULL)
        return false;
    
    girl->follow(doll);
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    
    touchListener->onTouchBegan = [this](Touch * touch, Event * event){
        auto doll = dynamic_cast<GameRole*>(getChildByName(GameRoleName::Doll));
        auto girl = dynamic_cast<GameRole*>(getChildByName(GameRoleName::Girl));
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
        if (rect.containsPoint(loc)){
            doll->unloadGirl();
        }
        
        return false;
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, getChildByName(GameRoleName::Girl));
    
    
    
    
    
    return true;
}


void GameScene_4::enterStage(){
    GameScene::enterStage();
    saveStage(5);
    Director::getInstance()->replaceScene(TransitionFade::create(2, GameScene_5::createScene()));
}

void GameScene_4::updateFire(float dt){
    log("I am updating fire~");
    auto firenodes = utils::findChildren(*this, "Fire");
    auto leavesnodes = utils::findChildren(*this, "Leaves");
    if (weather == WEATHER_SUNNY){
        SimpleAudioEngine::getInstance()->resumeEffect(effects["fire"]);
        for (auto n : firenodes){
            auto pf = dynamic_cast<ParticleSystemQuad*>(n);
            if (pf == NULL)
                continue;
            pf->setLife(1);
            pf->setSpeed(pf->getSpeed() + 10);
        }
        for (auto n : leavesnodes){
            auto pf = dynamic_cast<ParticleSystemQuad*>(n);
            if (pf == NULL)
                continue;
            pf->setPositionY(pf->getPositionY() + 10);
        }
    }
    else {
        SimpleAudioEngine::getInstance()->pauseEffect(effects["fire"]);
        for (auto n : firenodes){
            auto pf = dynamic_cast<ParticleSystem*>(n);
            if (pf == NULL)
                continue;
            pf->setLife(0.5);
            pf->setSpeed(pf->getSpeed() - 10);
        }
        for (auto n : leavesnodes){
            auto pf = dynamic_cast<ParticleSystemQuad*>(n);
            if (pf == NULL)
                continue;
            pf->setPositionY(pf->getPositionY() - 10);
        }
    }
}

void GameScene_4::beRainyGround(){
    GameScene::beRainyGround();
    schedule(schedule_selector(GameScene_4::updateFire), 1.0f, 3, 2.0f);
}
