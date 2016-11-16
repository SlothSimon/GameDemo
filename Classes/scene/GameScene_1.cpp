//
//  GameScene_1.cpp
//  Sunny Doll
//
//  Created by zhangsimon on 2016/11/5.
//
//

#include "GameScene_1.hpp"
#include "GameScene_2.hpp"

bool GameScene_1::initSpecfic(){
    auto doll = dynamic_cast<GameRole*>(getChildByName(GameRoleName::Doll));
//
//    auto bl = Sprite::create(ImagePath::Balloon);
//    if (bl){
//        bl->setScale(1/doll->getScale());
//        bl->setFlippedX(true);
//        bl->setRotation(-30 + 15 * 0);
//        doll->addChild(bl);
//        bl->setPosition(0, doll->getContentSize().height*0.9);
//        bl->setLocalZOrder(-1);
//    }
    doll->addItem("balloon");
    
    
    return true;
}


void GameScene_1::onEnterTransitionDidFinish(){
    // TODO: 第一关新手指导，另外push一个scene，然后再pop
    // TODO: Doll lies down, then stands up.
}

void GameScene_1::enterStage(){
    UserDefault::getInstance()->setIntegerForKey("currentStage", 2);
    Director::getInstance()->replaceScene(TransitionFade::create(2, GameScene_2::createScene()));
}
