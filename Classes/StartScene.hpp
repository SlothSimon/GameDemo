//
//  StartScene.hpp
//  Sunny Doll
//
//  Created by zhangsimon on 2016/11/21.
//
//

#ifndef StartScene_hpp
#define StartScene_hpp

USING_NS_CC;
using namespace std;

class StartScene : public Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    CREATE_FUNC(StartScene);

private:
    
    void chooseStage(int stage);

};

#endif /* StartScene_hpp */
