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
        role->changeState(new StateWalk());
    }
    else if (eventName == GameRoleState::State::Think){
        auto m = static_cast<map<string, void*>*>(event->getUserData());
        auto cont = static_cast<string*>(m->at("Data"));
        auto callback = static_cast<CallFunc*>(m->at("Callback"));
        role->think(*cont, callback);
    }
    else if (eventName == GameRoleState::State::Drown){
        role->drown();
        role->changeState(new StateDrown());
    }

}
