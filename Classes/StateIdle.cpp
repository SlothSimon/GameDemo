//
//  StateIdle.cpp
//  Sunny Doll
//
//  Created by zhangsimon on 16/9/24.
//
//

#include "StateIdle.h"
#include "StateWalk.h"
#include "StateDrown.h"
#include "GameRole.h"

void StateIdle::execute(GameRole* role, EventCustom* event){
    auto eventName = GameRoleState::convertToStateName(event->getEventName());
    
    if (eventName == GameRoleState::State::Walk){
        auto pos = static_cast<Vec2*>(event->getUserData());
        role->walk(*pos);
        role->getFSM()->changeState(new StateWalk());
    }
    else if (eventName == GameRoleState::State::Think){
        string* cont = static_cast<string*>(event->getUserData());
        role->think(*cont);
    }
    else if (eventName == GameRoleState::State::Drown){
        role->drown();
        role->getFSM()->changeState(new StateDrown());
    }

}
