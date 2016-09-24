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

void StateIdle::execute(GameRole* role, EnumMsgType enMsgType){
    switch (enMsgType) {
        case en_Msg_Walk:
            role->walk();
            role->getFSM()->changeState(new StateWalk());
            break;
        case en_Msg_Drown:
            role->drown();
            role->getFSM()->changeState(new StateDrown());
            break;
    }
}
