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
    auto eventName = GameRoleState::convertToStateName(event->getEventName());
    
    if (eventName == GameRoleState::State::Idle){
        role->idle();
        role->changeState(new StateIdle());
    }
    else if (eventName == GameRoleState::State::Think || eventName == GameRoleState::State::Say){
        auto m = static_cast<map<string, void*>*>(event->getUserData());
        auto cont = static_cast<string*>(m->at("Data"));
        if (m->find("Callback") != m->cend()){
            auto callback = static_cast<CallFunc*>(m->at("Callback"));
            role->think(*cont, callback);
        }else
            role->think(*cont);
    }
}
