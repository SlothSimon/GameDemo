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
    auto enMsgType = (int)GameRoleState::toEnum(event->getEventName());
    
    switch (enMsgType) {
        case GameRoleState::State::Idle:
            role->idle();
            role->getFSM()->changeState(new StateIdle());
            break;
        case GameRoleState::State::Drown:
            role->drown();
            role->getFSM()->changeState(new StateDrown());
            break;
        default:
            break;
    }
}
