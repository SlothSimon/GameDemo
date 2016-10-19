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
    
    
    auto listener = EventListenerCustom::create(GameRoleState::convertToEventName(role, GameRoleState::State::Walk), CC_CALLBACK_1(GameRoleFSM::onRecvState, this));
    DISPATCH->addEventListenerWithSceneGraphPriority(listener, this);
    listener = EventListenerCustom::create(GameRoleState::convertToEventName(role, GameRoleState::State::Idle), CC_CALLBACK_1(GameRoleFSM::onRecvState, this));
    DISPATCH->addEventListenerWithSceneGraphPriority(listener, this);
    listener = EventListenerCustom::create(GameRoleState::convertToEventName(role, GameRoleState::State::Drown), CC_CALLBACK_1(GameRoleFSM::onRecvState, this));
    DISPATCH->addEventListenerWithSceneGraphPriority(listener, this);
    listener = EventListenerCustom::create(GameRoleState::convertToEventName(role, GameRoleState::State::Think), CC_CALLBACK_1(GameRoleFSM::onRecvState, this));
    DISPATCH->addEventListenerWithSceneGraphPriority(listener, this);
    listener = EventListenerCustom::create(GameRoleState::convertToEventName(role, GameRoleState::State::Cry), CC_CALLBACK_1(GameRoleFSM::onRecvState, this));
    DISPATCH->addEventListenerWithSceneGraphPriority(listener, this);
    listener = EventListenerCustom::create(GameRoleState::convertToEventName(role, GameRoleState::State::Say), CC_CALLBACK_1(GameRoleFSM::onRecvState, this));
    DISPATCH->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

void GameRoleFSM::changeState(State *state){
    CC_SAFE_DELETE(curState);
    
    this->curState = state;
}

void GameRoleFSM::onRecvState(cocos2d::EventCustom *event){
    this->curState->execute(role, event);
}

const State* GameRoleFSM::getCurrentState() const{
    return curState;
}

#undef DISPATCH
#undef StateToString
