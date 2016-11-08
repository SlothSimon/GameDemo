//
//  Cinematic.hpp
//  Sunny Doll
//
//  Created by zhangsimon on 2016/11/6.
//
//

#ifndef Cinematic_hpp
#define Cinematic_hpp

#include "GameRole.h"

USING_NS_CC;
using namespace std;

struct Cinematic{
    // TODO: 可能需要兼容其他类型的动画，而不只是人物的状态机变化
    Cinematic(Node* roleNode, const string actionName, double delayTime = -1, string usrdata = NULL);
    GameRole* role;
    const string action;
    string userdata;
    double delay;
};

#endif /* Cinematic_hpp */
