//
//  StateIdle.h
//  Sunny Doll
//
//  Created by zhangsimon on 16/9/24.
//
//

#ifndef StateIdle_h
#define StateIdle_h

#include "State.h"

class StateIdle: public State {
public:
    virtual void execute(GameRole* role, cocos2d::EventCustom* event);
    
};

#endif /* StateIdle_h */
