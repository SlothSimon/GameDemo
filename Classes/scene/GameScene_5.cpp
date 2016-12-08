//
//  GameScene_5.cpp
//  Sunny Doll
//
//  Created by zhangsimon on 2016/11/19.
//
//

#include "GameScene_5.hpp"
#include "EndScene.h"

bool GameScene_5::initSpecfic(){
    auto girl = dynamic_cast<GameRole*>(getChildByName(GameRoleName::Girl));
    auto doll = dynamic_cast<GameRole*>(getChildByName(GameRoleName::Doll));
    
    if (girl == NULL || doll == NULL)
        return false;
    
    girl->follow(doll);
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    
    touchListener->onTouchBegan = [this](Touch * touch, Event * event){
        auto doll = dynamic_cast<GameRole*>(getChildByName(GameRoleName::Doll));
        auto girl = dynamic_cast<GameRole*>(getChildByName(GameRoleName::Girl));
        if (girl == NULL)
            return false;
        
        auto loc = girl->convertTouchToNodeSpace(touch);
        auto rect = girl->getTextureRect();
        
        if (rect.containsPoint(loc)){
            if (doll == NULL)
                return false;
            
            if (doll->getPosition().distance(touch->getLocation()) <= INTERACTION_MESSAGE_RANGE && doll->IsMovable()){
                doll->loadGirl();
            }
            else
                doll->doAction(GameRoleState::State::Think, GameRoleState::ThinkContent::Walk);
            return true;
        }
        
        loc = doll->convertTouchToNodeSpace(touch);
        rect = doll->getTextureRect();
        if (rect.containsPoint(loc) && doll->IsMovable()){
            doll->unloadGirl();
        }
        
        return false;
    };
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, getChildByName(GameRoleName::Girl));
    
    
    return true;
}


void GameScene_5::enterStage(){
//    GameScene::enterStage();
//    saveStage(6);
    UserDefault::getInstance()->setBoolForKey("End1", true);
    Director::getInstance()->replaceScene(TransitionFade::create(2, EndScene::createScene("End 1: Up")));
}

void GameScene_5::doAfterAddItem(const string & itemName){
    if (itemName == "balloon"){
        auto doll = dynamic_cast<GameRole*>(getChildByName(GameRoleName::Doll));
        auto iter = doll->getItemList().find("balloon");
        if (iter != doll->getItemList().cend() && iter->second >=4){
            auto n = Node::create();
            n->setPosition(Vec2(0,doll->getPosition().y));
            n->setContentSize(Size(Director::getInstance()->getVisibleSize().width, doll->getContentSize().height));
            this->addChild(n);
            doll->runAction(Sequence::create(DelayTime::create(5),
                                             CallFunc::create([doll, n, this]{
                                                this->pushCinematic(new Cinematic(doll, GameRoleState::State::Say, -1, GameRoleState::SayContent::Shock));
                                                doll->loadGirl();
                                                doll->fly(n);
                                            }),
                                             NULL));
        }else{
            // TODO: Cinematics about end 2
            pushCinematic(new Cinematic(doll, GameRoleState::State::Say, -1, GameRoleState::SayContent::Think));
            doll->runAction(Sequence::create(DelayTime::create(8),
                                             CallFunc::create([this]{
                                                GameScene::enterStage();
                                                UserDefault::getInstance()->setBoolForKey("End2", true);
                                                Director::getInstance()->replaceScene(TransitionFade::create(2, EndScene::createScene("End 2: Le ballon rouge")));
            }),
                                             NULL));
        }
    }
}
