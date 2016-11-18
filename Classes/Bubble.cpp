//
//  Bubble.cpp
//  Sunny Doll
//
//  Created by zhangsimon on 2016/11/16.
//
//

#include "Bubble.hpp"
#include "Constants.h"

Bubble * Bubble::create(const string &bubbleName){
    Bubble *bb = new (nothrow) Bubble();
    auto path = ImagePath::getBubblePath(bubbleName);
    if (path == nullptr)
        return nullptr;
    if (bb && bb->initWithFile(path))
    {
        bb->autorelease();
        return bb;
    }
    CC_SAFE_DELETE(bb);
    return nullptr;
}

void Bubble::show(){
    setVisible(false);
    stopAllActions();
    auto scale = ScaleBy::create(0.1f, 4.0f);
    auto sh = Show::create();
    auto hd = Hide::create();
    
    setScale(0.25);
    runAction(Sequence::create(sh,
                                scale,
                                DelayTime::create(2.0f),
                                scale->reverse(),
                                hd,
                                NULL));
}

void Bubble::hide(){
    setVisible(false);
    stopAllActions();
}
