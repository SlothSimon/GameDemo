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
#include "StateCry.h"

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
    else if (eventName == GameRoleState::State::Think || eventName == GameRoleState::State::Say){
        auto m = static_cast<map<string, void*>*>(event->getUserData());
        auto cont = static_cast<string*>(m->at("Data"));
        if (m->find("Callback") != m->cend()){
            auto callback = static_cast<CallFunc*>(m->at("Callback"));
            role->think(*cont, callback);
        }else
        role->think(*cont);
    }
    else if (eventName == GameRoleState::State::Drown){
        role->drown();
        role->changeState(new StateDrown());
    }else if (eventName == GameRoleState::State::Cry){
        role->cry();
        role->changeState(new StateCry());
    }
}
