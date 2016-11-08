//
//  Cinematic.cpp
//  Sunny Doll
//
//  Created by zhangsimon on 2016/11/6.
//
//

#include "Cinematic.hpp"

Cinematic::Cinematic(Node* roleNode, const string actionName, const double delaySecond, string actionContent):action(actionName), delay(delaySecond), userdata(actionContent){
    role = dynamic_cast<GameRole*>(roleNode);
};
