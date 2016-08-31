//
//  GameScene.h
//  Escape
//
//  Created by zhangsimon on 16/8/19.
//
//

#ifndef GameScene_h
#define GameScene_h

#define WEATHER_SUNNY "Sunny"
#define WEATHER_RAINY "Rainy"
#define INTERACTION_RANGE 30

USING_NS_CC;
using namespace std;

class GameScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init(string w = WEATHER_SUNNY);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GameScene);
    
    void setPhyWorld(PhysicsWorld* world){m_world = world;}
    
private:
    std::string weather;
    cocos2d::LayerColor* weatherLayer;
    TMXTiledMap* tileMap;
    map<string, unsigned int> effects;  // 音效ID集合
    bool initialized;   // 天气是否已初始化
    PhysicsWorld* m_world;  // 物理系统
    Map<string, Node*> collisionNodeWithAction; // 具有action的node
    
    
    void beSunny();
    
    void beRainy();
    
    void beSunnyGround();
    
    void beRainyGround();
    
    bool initMap(const string &);
    
    bool initInteraction();
    
    bool initCollision();
    
    bool initSideBar();
    
    bool initWeather();
    
    bool initDoll();
    
    bool initListener();
    
    void updateWeather(float dt);
    
};


#endif /* GameScene_h */
