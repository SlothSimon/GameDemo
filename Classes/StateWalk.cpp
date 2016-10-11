//
//  StateWalk.cpp
//  Sunny Doll
//
//  Created by zhangsimon on 16/9/24.
//
//

#include "StateWalk.h"
#include "StateDrown.h"
#include "StateIdle.h"
#include "GameRole.h"

void StateWalk::execute(GameRole* role, EventCustom* event){
    auto eventName = GameRoleState::convertToStateName(event->getEventName());
    
    if (eventName == GameRoleState::State::Idle){
        role->idle();
        role->changeState(new StateIdle());
    }
    else if (eventName == GameRoleState::State::Drown){
        role->drown();
        role->changeState(new StateDrown());
    }
}
