//
//  GameRoleFSM.cpp
//  Sunny Doll
//
//  Created by zhangsimon on 16/9/24.
//
//

#include "GameRoleFSM.h"
#include "GameRole.h"
#include "StateIdle.h"

#define DISPATCH Director::getInstance()->getEventDispatcher()

GameRoleFSM::~GameRoleFSM(){
    DISPATCH->removeEventListenersForTarget(this);
}

GameRoleFSM* GameRoleFSM::createWithGameRole(GameRole *role){
    GameRoleFSM* fsm = new GameRoleFSM();
    
    if (fsm && fsm->initWithGameRole(role)){
        fsm->autorelease();
    }else{
        CC_SAFE_DELETE(fsm);
        fsm = NULL;
    }
    
    return fsm;
}

bool GameRoleFSM::initWithGameRole(GameRole *role){
    this->curState = new StateIdle();
    this->curState->retain();
    this->role = role;
    role->retain();
    
    
    auto listener = EventListenerCustom::create(StateToString(role, Walk), CC_CALLBACK_1(GameRoleFSM::onRecvWalk, this));
    DISPATCH->addEventListenerWithSceneGraphPriority(listener, this);
    
    listener = EventListenerCustom::create(StateToString(role, Idle), CC_CALLBACK_1(GameRoleFSM::onRecvIdle, this));
    DISPATCH->addEventListenerWithSceneGraphPriority(listener, this);
    
    listener = EventListenerCustom::create(StateToString(role, Drown), CC_CALLBACK_1(GameRoleFSM::onRecvDrown, this));
    DISPATCH->addEventListenerWithSceneGraphPriority(listener, this);
    listener = EventListenerCustom::create(StateToString(role, Think), CC_CALLBACK_1(GameRoleFSM::onRecvDrown, this));
    DISPATCH->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void GameRoleFSM::changeState(State *state){
    CC_SAFE_DELETE(curState);
    
    this->curState = state;
}

void GameRoleFSM::onRecvIdle(cocos2d::EventCustom *event){
    this->curState->execute(role, event);
}

void GameRoleFSM::onRecvWalk(cocos2d::EventCustom *event){
    this->curState->execute(role, event);
}

void GameRoleFSM::onRecvDrown(cocos2d::EventCustom *event){
    this->curState->execute(role, event);
}

const State* GameRoleFSM::getCurrentState() const{
    return curState;
}

#undef DISPATCH
#undef StateToString
