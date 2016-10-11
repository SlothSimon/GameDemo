//
//  State.h
//  Sunny Doll
//
//  Created by zhangsimon on 16/9/24.
//
//

#ifndef State_h
#define State_h

#include "Constants.h"
USING_NS_CC;

class GameRole;

class State : public cocos2d::Ref{
public:
    virtual void execute(GameRole* role, cocos2d::EventCustom* event) = 0;
};

#endif /* State_h */
