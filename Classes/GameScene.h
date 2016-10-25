//
//  GameScene.h
//  Escape
//
//  Created by zhangsimon on 16/8/19.
//
//

#ifndef GameScene_h
#define GameScene_h

#include "GameRole.h"

#define WEATHER_SUNNY "Sunny"
#define WEATHER_RAINY "Rainy"
#define INTERACTION_RANGE 30
#define DOLL_ZORDER 1001
#define ROLE_ZORDER 1000
#define INTERACTION_ZORDER 999

USING_NS_CC;
using namespace std;

struct Cinematic{
    // TODO: 可能需要兼容其他类型的动画，而不只是人物的状态机变化
    Cinematic() = default;
    Cinematic(GameRole* r, const string s, double d = -1, string u = NULL):role(r), action(s), userdata(u), delay(d){};
    GameRole* role;
    const string action;
    string userdata;
    double delay = -1;
};

class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    
    virtual void onEnterTransitionDidFinish();
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
    
    void setPhyWorld(PhysicsWorld* world){m_world = world;}
    
    bool createCinematic(const string & cineName);
    
private:
    std::string weather;
    cocos2d::LayerColor* weatherLayer;
    TMXTiledMap* tileMap;
    map<string, unsigned int> effects;  // 音效ID集合
    bool initialized;   // 天气是否已初始化
    PhysicsWorld* m_world;  // 物理系统
    multimap<string, Node*> collisionNodeWithAction; // 具有action的node
    int currentStage;
    
    // 剧情动画
    bool isPlayCinematic = false;
    queue<Cinematic> seqCinematic;
    void playCinematic(Cinematic&);
    void pushCinematic(Cinematic&);
    void nextCinematic();
    
    // 场景变换
    void enterStage(const int &);
    
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
    
    bool initDoll();
    
    bool initListener();
    
    bool initBGM();
    
    
};


#endif /* GameScene_h */
