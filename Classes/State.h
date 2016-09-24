//
//  State.h
//  Sunny Doll
//
//  Created by zhangsimon on 16/9/24.
//
//

#ifndef State_h
#define State_h

#include "EnumMsgType.h"

class GameRole;

class State {
public:
    virtual void execute(GameRole* role, EnumMsgType enMsgType) = 0;
};

#endif /* State_h */
