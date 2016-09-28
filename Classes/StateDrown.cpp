//
//  StateDrown.cpp
//  Sunny Doll
//
//  Created by zhangsimon on 16/9/24.
//
//

#include "StateDrown.h"
#include "StateIdle.h"
#include "GameRole.h"

void StateDrown::execute(GameRole* role, EventCustom* event){
    auto enMsgType = GameRoleState::toEnum(event->getEventName());
    
    switch (enMsgType) {
        case GameRoleState::State::Idle:
            role->idle();
            role->getFSM()->changeState(new StateIdle());
            break;
        default:
            break;
    }
}
