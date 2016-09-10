#ifndef __ENDSCENE_SCENE_H__
#define __ENDSCENE_SCENE_H__

#include "cocos2d.h"

class EndScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(EndScene);
};

#endif // __ENDSCENE_SCENE_H__
