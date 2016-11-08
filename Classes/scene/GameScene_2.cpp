//
//  GameScene_2.cpp
//  Sunny Doll
//
//  Created by zhangsimon on 2016/11/5.
//
//

#include "GameScene_2.hpp"

bool GameScene_2::initSpecfic(){
    
    
    
    return true;
}

void GameScene_2::onEnterTransitionDidFinish(){
    auto doll = getChildByName(GameRoleName::Doll);
    auto girl = getChildByName(GameRoleName::Girl);
    pushCinematic(new Cinematic(doll, GameRoleState::State::Say, 1, GameRoleState::SayContent::Shock));
    pushCinematic(new Cinematic(girl, GameRoleState::State::Say, -1, GameRoleState::SayContent::Shock));
    pushCinematic(new Cinematic(girl, GameRoleState::State::Say, -1, GameRoleState::SayContent::Cry));
}
