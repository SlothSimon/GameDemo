//
//  GameScene_2.hpp
//  Sunny Doll
//
//  Created by zhangsimon on 2016/11/5.
//
//

#ifndef GameScene_2_hpp
#define GameScene_2_hpp

#include "GameScene.h"

class GameScene_2 : public GameScene{
public:
    static Scene* createScene();
    
    CREATE_FUNC(GameScene_2);
    
    bool initSpecfic() override;
    
    void onEnterTransitionDidFinish() override;
    
    void enterStage() override;
};

#endif /* GameScene_2_hpp */
