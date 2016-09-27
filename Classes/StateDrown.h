//
//  StateDrown.h
//  Sunny Doll
//
//  Created by zhangsimon on 16/9/24.
//
//

#ifndef StateDrown_h
#define StateDrown_h

#include "State.h"

class StateDrown: public State {
public:
    virtual void execute(GameRole* role, cocos2d::EventCustom* event);
};

#endif /* StateDrown_h */
