//
//  StateWalk.h
//  Sunny Doll
//
//  Created by zhangsimon on 16/9/24.
//
//

#ifndef StateWalk_h
#define StateWalk_h

#include "State.h"

class StateWalk: public State {
public:
    virtual void execute(GameRole* role, cocos2d::EventCustom* event);
};

#endif /* StateWalk_h */
