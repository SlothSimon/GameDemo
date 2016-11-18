//
//  Utils.h
//  Sunny Doll
//
//  Created by zhangsimon on 16/8/26.
//
//
#ifndef Utils_h
#define Utils_h

#include "GameRole.h"

USING_NS_CC;
using namespace std;

bool saveGameRole(const GameRole*);

bool saveStage(const int stage);

bool loadGameRole(GameRole* role);

bool loadStage(int & stage);

#endif /* Utils_h */
