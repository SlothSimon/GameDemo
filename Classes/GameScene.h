//
//  GameScene.h
//  Sunny Doll
//
//  Created by zhangsimon on 16/8/19.
//
//

#ifndef GameScene_h
#define GameScene_h

#include "GameRole.h"
#include "Cinematic.hpp"
#include "Macros.h"
#include "Utils.h"

#define WEATHER_SUNNY "Sunny"
#define WEATHER_RAINY "Rainy"
#define INTERACTION_MESSAGE_RANGE 50
#define INTERACTION_ITEM_RANGE 120
#define DOLL_ZORDER 1001
#define ROLE_ZORDER 1000
#define INTERACTION_ZORDER 999


USING_NS_CC;
using namespace std;

class GameScene : public cocos2d::Layer
{
public:
    std::string weather;
    cocos2d::LayerColor* weatherLayer;
    TMXTiledMap* tileMap;
    map<string, unsigned int> effects;  // 音效ID集合
    bool initialized;   // 天气是否已初始化
    PhysicsWorld* m_world;  // 物理系统
    multimap<string, Node*> collisionNodeWithAction; // 具有action的node
    int currentStage;
    
    
    static Scene* createScene();
    
    virtual bool init();
    
    virtual void onEnterTransitionDidFinish();
    
    virtual void onExitTransitionDidStart();
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
    
    void setPhyWorld(PhysicsWorld* world){m_world = world;}
    
    // 剧情动画
    bool isPlayCinematic = false;
    queue<Cinematic> seqCinematic;
    void playCinematic(Cinematic);
    void pushCinematic(Cinematic*);
    void nextCinematic();
    
    // 场景变换
    virtual void enterStage();
    
    // weather change
    void beSunny();
    
    void beRainy();
    
    virtual void beSunnyGround();
    
    virtual void beRainyGround();
    
    void updateWeather(float dt);
    
    // initial
    bool initMap(const string &);
    
    bool initInteraction();
    
    bool initCollision();
    
    bool initSideBar();
    
    bool initWeather();
    
    bool initRole();
    
    bool initListener();
    
    bool initBGM();
    
    // Each GameScene derived class should implement it: story cinematics and so on
    virtual bool initSpecfic();
    
};


#endif /* GameScene_h */
