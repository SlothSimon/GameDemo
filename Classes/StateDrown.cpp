//
//  StateDrown.cpp
//  Sunny Doll
//
//  Created by zhangsimon on 16/9/24.
//
//

#include "StateDrown.h"
#include "StateIdle.h"

void StateDrown::execute(GameRole* role, EnumMsgType enMsgType){
    switch (enMsgType) {
        case en_Msg_Idle:
            role->idle();
            role->getFSM()->changeState(new StateIdle());
            break;
    }
}
