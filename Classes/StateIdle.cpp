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
    auto enMsgType = GameRoleState::toEnum(event->getEventName());
    
    switch (enMsgType) {
        case GameRoleState::State::Walk:{
            auto pos = static_cast<Vec2*>(event->getUserData());
            role->walk(*pos);
            role->getFSM()->changeState(new StateWalk());
            break;
        }
        case GameRoleState::State::Drown:{
            role->drown();
            role->getFSM()->changeState(new StateDrown());
            break;
        }
        default:
            break;
    }
}