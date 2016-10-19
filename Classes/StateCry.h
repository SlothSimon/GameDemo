//
//  StateCry.h
//  Sunny Doll
//
//  Created by zhangsimon on 2016/10/19.
//
//

#ifndef StateCry_h
#define StateCry_h

#include "State.h"

class StateCry: public State {
public:
    virtual void execute(GameRole* role, cocos2d::EventCustom* event);
    
};

#endif /* StateCry_h */
