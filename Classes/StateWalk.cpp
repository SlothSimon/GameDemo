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

void StateWalk::execute(GameRole* role, EventCustom* event){
    auto enMsgType = stoi(event->getEventName());
    
    switch (enMsgType) {
        case en_Msg_Idle:
            role->idle();
            role->getFSM()->changeState(new StateIdle());
            break;
        case en_Msg_Drown:
            role->drown();
            role->getFSM()->changeState(new StateDrown());
            break;
        default:
            break;
    }
}
