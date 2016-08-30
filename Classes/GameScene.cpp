//
//  GameScene.cpp
//  Escape
//
//  Created by zhangsimon on 16/8/19.
//
//

#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "ui/UIButton.h"
#include "ui/UILayout.h"
#include "GameRole.h"

USING_NS_CC;
using namespace CocosDenshion;
using namespace cocos2d::ui;

Scene* GameScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = GameScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool GameScene::init(std::string w)
{
    if ( !Layer::init() )
    {
        return false;
    }
    

    initialized = false;
    weather = w;
    
//    Vec2 origin = Director::getInstance()->getVisibleOrigin();
//    auto visibleSize = Director::getInstance()->getVisibleSize();
//    auto touchLayer = LayerColor::create(cocos2d::Color4B::WHITE, visibleSize.width, visibleSize.height);
//    touchLayer->setPosition(origin + Vec2(100,100));
//    touchLayer->setVisible(true);
//    addChild(touchLayer, 1);
    
    initMap("1_1");
    
    initSideBar();
    
    initWeather();
    
    initDoll();
    
    initListener();
    
    if (weather == WEATHER_SUNNY)
        beSunny();
    else if (weather == WEATHER_RAINY)
        beRainy();


    
    
    return true;
}

void GameScene::updateWeather(float dt){
    log("I am updating weather~");
    ParticleSystem* rain = static_cast<ParticleSystem*>(this->getChildByName("rain"));
    log("Rain EmissionRate: %f", rain->getEmissionRate());
    if (weather == WEATHER_SUNNY){
        if (rain && (rain->getEmissionRate() > 200)){
            rain->setEmissionRate(200);
            rain->setLife(3);
        }
        else{
            SimpleAudioEngine::getInstance()->stopEffect(effects["rain"]);
            rain->setEmissionRate(0);
            rain->setSpeed(0);
            auto bt = static_cast<Button*>(getChildByName("sideBar")->getChildByName("rain"));
            bt->setEnabled(true);
            unschedule(schedule_selector(GameScene::updateWeather));
        }
    }
    else {
        if (rain->getEmissionRate() != 400.0f){
            rain->setEmissionRate(rain->getEmissionRate() + 100);
            rain->setLife(1);
            rain->setSpeed(300);
        }else{
            auto bt = static_cast<Button*>(getChildByName("sideBar")->getChildByName("sun"));
            bt->setEnabled(true);
            unschedule(schedule_selector(GameScene::updateWeather));
        }
    }
}

void GameScene::beSunny(){
    
    weather = WEATHER_SUNNY;
    
    // Button Status
    auto bt = static_cast<Button*>(getChildByName("sideBar")->getChildByName("rain"));
    bt->setEnabled(false);
    bt->setBright(false);
    
    // Animation
    auto fadein = FadeIn::create(3.0f);
    weatherLayer->getChildByName("sun")->runAction(fadein);
    auto tint = TintTo::create(3.0f, 204, 235, 255);
    weatherLayer->runAction(tint);
    
    schedule(schedule_selector(GameScene::updateWeather), 1.0f, kRepeatForever, 0);
    runAction(EffectTo::create(2.0f, 0.0));
    
    // sun change
    beSunnyGround();
}

void GameScene::beRainy(){
    weather = WEATHER_RAINY;
    
    // Button Status
    auto bt = static_cast<Button*>(getChildByName("sideBar")->getChildByName("sun"));
    bt->setEnabled(false);
    bt->setBright(false);
    
    // Animation
    auto fadeout = FadeOut::create(3.0f);
    weatherLayer->getChildByName("sun")->runAction(fadeout);
    auto tint = TintTo::create(3.0f, 238, 238, 238);
    weatherLayer->runAction(tint);
    
    schedule(schedule_selector(GameScene::updateWeather), 1.0f, kRepeatForever, 0);
    
    // rain effect
    effects["rain"] = SimpleAudioEngine::getInstance()->playEffect("music/rain.wav", true);
    runAction(EffectTo::create(4.0f, 1.0));
    
    // rain change
    beRainyGround();
}

void GameScene::beSunnyGround(){
    beRainyGround();
}

void GameScene::beRainyGround(){
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float pixelPerTile = visibleSize.width/tileMap->getTileSize().width;

    auto layers = tileMap->getLayerWithWeather();
    for (auto i = 0; i < layers.size(); i++){
        auto action = layers.at(i)->getProperty("Action").asString();
        if (action == "Move"){
            
            auto x = layers.at(i)->getProperty(weather + "TileX").asInt() * pixelPerTile;
            auto y = layers.at(i)->getProperty(weather + "TileY").asInt() * pixelPerTile;
            
            if (initialized){
                
                auto m = MoveTo::create(layers.at(i)->getProperty("Duration").asFloat(), Vec2(x, y));
                layers.at(i)->runAction(Sequence::create(DelayTime::create(layers.at(i)->getProperty("Delay").asFloat()), EaseSineInOut::create(m), NULL));
            }else{
                layers.at(i)->setPosition(Vec2(x,y));
                if (i == layers.size() - 1)
                    initialized = true;
            }
        }
    }
}


bool GameScene::initMap(const string & mapName){
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    string path = "map/";
    path = path + mapName + ".tmx";
    
    tileMap = TMXTiledMap::create(path);
    if (tileMap == nullptr)
        return false;
    
    tileMap->setPosition(origin);
    tileMap->setScale(visibleSize.width/(tileMap->getContentSize().width));
    
    initInteraction();
    
    addChild(tileMap, 10);
    
    return true;
}

bool GameScene::initInteraction(){
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto objs = tileMap->getObjectGroup("interaction");
    if (objs == nullptr)
        return false;
    
//    auto visibleSize = Director::getInstance()->getVisibleSize();
//    float pixelPerTile = visibleSize.width/tileMap->getTileSize().width;
    
    for (const auto & obj : (objs->getObjects())){
        auto uiInfo = obj.asValueMap();
        auto targetLayer = tileMap->getLayer(uiInfo["LayerName"].asString());
        if (targetLayer == nullptr)
            return false;
        
        auto touchLayer = LayerColor::create(Color4B::RED, uiInfo["width"].asFloat() * tileMap->getScale(), uiInfo["height"].asFloat()*tileMap->getScale());
        touchLayer->setPosition(origin + Vec2(uiInfo["x"].asFloat(), uiInfo["y"].asFloat())*tileMap->getScale());
        touchLayer->setVisible(false);
        addChild(touchLayer, 999, "panel");
        auto touchLayerListener = EventListenerTouchOneByOne::create();
        touchLayerListener->setSwallowTouches(true);
        
        touchLayerListener->onTouchBegan = [targetLayer, touchLayer](Touch* touch, Event* event){
            Vec2 locationInNode = touchLayer->convertToNodeSpace(touch->getLocation());
            Size s = touchLayer->getContentSize();
            Rect rect = Rect(0, 0, s.width, s.height);
            //判断触摸区域是否在目标上
            if (rect.containsPoint(locationInNode)){
                targetLayer->stopAllActions();
                targetLayer->runAction(Sequence::create(Show::create(),
                                                        DelayTime::create(5),
                                                        Hide::create(),
                                                        NULL));
                return true;    // return true 会使其他listener失效
            }
            return false;       // return false 会继续执行其他listener
        };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchLayerListener, touchLayer);
    }
    
    return true;
}

bool GameScene::initSideBar(){
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto sideBar = Layout::create();
    sideBar->setPosition(origin + Vec2(10, visibleSize.height - 10));
    
    // Vertical layout with top margin
    sideBar->setLayoutType(Layout::Type::VERTICAL);
    auto sideBarPatameters = LinearLayoutParameter::create();
    sideBarPatameters->setMargin(Margin(0, 5, 0, 0));
    
    this->addChild(sideBar, 1001, "sideBar");
    
    auto ButtonSun = Button::create("button_sun.png");
    ButtonSun->setVisible(false);
    ButtonSun->setLayoutParameter(sideBarPatameters);
    ButtonSun->addClickEventListener([this](Ref* psender){
        if (weather != WEATHER_SUNNY)
            beSunny();
    });
    
    auto ButtonRain = Button::create("button_rain.png");
    ButtonRain->setVisible(false);
    ButtonRain->setLayoutParameter(sideBarPatameters);
    ButtonRain->addClickEventListener([this](Ref* psender){
        if (weather != WEATHER_RAINY)
            beRainy();
    });
    
    auto ButtonMenu = Button::create("button_menu.png");
    ButtonMenu->setLayoutParameter(sideBarPatameters);
    ButtonMenu->addClickEventListener([ButtonSun, ButtonRain](Ref* psender){
        if (ButtonSun->isVisible()){
            ButtonSun->setVisible(false);
            ButtonRain->setVisible(false);
        }else{
            ButtonSun->setVisible(true);
            ButtonRain->setVisible(true);
        }
    });
    
    sideBar->addChild(ButtonMenu, 1, "menu");
    sideBar->addChild(ButtonSun, 1, "sun");
    sideBar->addChild(ButtonRain, 1, "rain");
    
    return true;
}

bool GameScene::initWeather(){
    // initialize weatherLayer
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    weatherLayer = cocos2d::LayerColor::create(cocos2d::Color4B::WHITE, visibleSize.width, visibleSize.height);
    weatherLayer->setPosition(origin);
    this->addChild(weatherLayer, 0);
    
    // particleRain sys
    ParticleSystem* rain = ParticleRain::create();
    rain->setSpeed(0.0f);
    rain->setEmissionRate(0);
    this->addChild(rain, 10, "rain");
    
    // rain effect
    SimpleAudioEngine::getInstance()->preloadEffect("music/rain.wav");
    SimpleAudioEngine::getInstance()->setEffectsVolume(0);
    
    // sun
    auto sun = Sprite::create("sun.png");
    sun->setOpacity(0);
    sun->setScale(0.5);
    sun->setPosition(origin.x + visibleSize.width - sun->getContentSize().width*sun->getScale(),
                     origin.y + visibleSize.height - sun->getContentSize().height*sun->getScale());
    weatherLayer->addChild(sun, 1, "sun");
    
    return true;
}

bool GameScene::initDoll(){
    
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto dollPoint = tileMap->getObjectGroup("roles")->getObject("doll");
    
    
    auto doll = GameRole::create("doll");
    doll->setPosition(origin + Vec2(dollPoint["x"].asInt()*tileMap->getScale(), dollPoint["y"].asInt()*tileMap->getScale()));
    doll->setScale((dollPoint["y"].asInt()*tileMap->getScale()/2)/doll->getContentSize().height);
    addChild(doll, 1000, "doll");
//    doll->runAction(dollWalkAnim);
    
    return true;
}

bool GameScene::initListener(){
    auto touchLayerListener = EventListenerTouchOneByOne::create();
    touchLayerListener->setSwallowTouches(true);
    
    touchLayerListener->onTouchBegan = [this](Touch* touch, Event* event){
        auto doll = static_cast<GameRole*>(this->getChildByName("doll"));
        
        // TODO: 判断边界和障碍物
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        
        auto targetX = touch->getLocation().x;
        auto dollWidth = doll->getContentSize().width*doll->getScale()/2;
        
        if (targetX < origin.x + dollWidth){
            targetX = 0 + dollWidth;
        }else if (targetX > origin.x + visibleSize.width - dollWidth){
            targetX = origin.x + visibleSize.width - dollWidth;
        }
        
        doll->walkTo(Vec2(targetX,touch->getLocation().y));
        
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchLayerListener, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchLayerListener->clone(), getChildByName("panel"));
    
    return true;
}
