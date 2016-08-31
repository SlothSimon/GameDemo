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
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_CONTACT);//调试
    
    auto layer = GameScene::create();
    layer->setPhyWorld(scene->getPhysicsWorld());//将创建的物理世界传递到子层中
    
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
    
    initMap("1_1");
    
    initSideBar();
    
    initWeather();
    
    initDoll();
    
    initListener();


    
    
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
    float pixelPerTile = tileMap->getContentSize().width/tileMap->getMapSize().width;

    auto layers = tileMap->getLayerWithWeather();
    for (auto i = 0; i < layers.size(); i++){
        auto action = layers.at(i)->getProperty("Action").asString();
        if (action == "Move"){
            
            auto deltaDist = Vec2(layers.at(i)->getProperty(weather + "TileX").asInt() * pixelPerTile ,
                                  layers.at(i)->getProperty(weather + "TileY").asInt() * pixelPerTile) - layers.at(i)->getPosition();
            
            if (initialized){
                
                auto m = MoveBy::create(layers.at(i)->getProperty("Duration").asFloat(), deltaDist);
                auto seq = Sequence::create(DelayTime::create(layers.at(i)->getProperty("Delay").asFloat()), EaseSineInOut::create(m), NULL);
                layers.at(i)->runAction(seq);
                auto lname = layers.at(i)->getLayerName();
                if (collisionNodeWithAction.find(lname) != collisionNodeWithAction.cend()){
                    auto m = MoveBy::create(layers.at(i)->getProperty("Duration").asFloat(), deltaDist);
                    auto seq = Sequence::create(DelayTime::create(layers.at(i)->getProperty("Delay").asFloat()), EaseSineInOut::create(m), NULL);
                    collisionNodeWithAction.at(lname)->runAction(seq);
                }
            }else{
                layers.at(i)->setPosition(layers.at(i)->getPosition() + deltaDist);
                auto lname = layers.at(i)->getLayerName();
                if (collisionNodeWithAction.find(lname) != collisionNodeWithAction.cend()){
                    collisionNodeWithAction.at(lname)->setPosition(collisionNodeWithAction.at(lname)->getPosition() + deltaDist);
                }
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
    
    initCollision();
    
    
    addChild(tileMap, 10);
    
    return true;
}

bool GameScene::initInteraction(){
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto objs = tileMap->getObjectGroup("interaction");
    if (objs == nullptr)
        return false;
    
    
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
        
        touchLayerListener->onTouchBegan = [=](Touch* touch, Event* event){
            Vec2 locationInNode = touchLayer->convertToNodeSpace(touch->getLocation());
            Size s = touchLayer->getContentSize();
            Rect rect = Rect(0, 0, s.width, s.height);
            //判断触摸区域是否在目标上
            if (rect.containsPoint(locationInNode)){
                
                // 判断交互对象和主角的距离
                auto doll = static_cast<GameRole*>(getChildByName("doll"));
                auto dist = doll->getPosition().distance(touchLayer->getPosition());
                if (dist <= INTERACTION_RANGE){
                    targetLayer->stopAllActions();
                    targetLayer->runAction(Sequence::create(Show::create(),
                                                            DelayTime::create(5),
                                                            Hide::create(),
                                                            NULL));
                }else{
//                    doll->think("walk");
                    log("doll think walk");
                }
                
                
                return true;    // return true 会使其他listener失效
            }
            return false;       // return false 会继续执行其他listener
        };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchLayerListener, touchLayer);
    }
    
    return true;
}

bool GameScene::initCollision(){
    float scale = tileMap->getScale();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto body = PhysicsBody::createEdgeBox(visibleSize);
    setPhysicsBody(body);
    
    auto barriers = tileMap->getObjectGroup("barriers")->getObjects();
    for (auto & b : barriers){
        auto bInfo = b.asValueMap();
        auto parentLayerName = bInfo["parentLayer"].asString();
        float x = bInfo["x"].asFloat(), y = bInfo["y"].asFloat(), width = bInfo["width"].asFloat(), height = bInfo["height"].asFloat();

        auto body = PhysicsBody::createBox(Size(width, height), PhysicsMaterial(0,0,0));
        body->setDynamic(false);
        body->setPositionOffset(Vec2(width/2, height/2)*scale);     // 刚体的锚点在重心，因此要调整到左下角
        
        
        auto n = Sprite::create();
        n->setPosition(Vec2(x, y));
        n->setAnchorPoint(Vec2(0,0));
        n->setPhysicsBody(body);
        tileMap->addChild(n);
        
        
        if (parentLayerName != ""){
            collisionNodeWithAction.insert(parentLayerName, n);
            
            // TODO：如何使人物与木头一体，在水位下降时不会弹跳？
//            body->setContactTestBitmask(true);
//            auto listener = EventListenerPhysicsContact::create();
//            listener->onContactBegin = [](PhysicsContact & contact){
//                auto body1 = contact.getShapeA()->getBody();
//                auto body2 = contact.getShapeB()->getBody();
//                
//                if (body1->isDynamic())
////                    body1->setVelocity(Vec2::ZERO);
//                    body1->setDynamic(false);
//                if (body2->isDynamic())
////                    body2->setVelocity(Vec2::ZERO);
//                    body2->setDynamic(false);
//                
////                body1->resetForces();
////                body2->resetForces();
//                
//                return true;
//            };
//            
//            listener->onContactSeparate = [](PhysicsContact & contact){
//                auto role1 = static_cast<GameRole*>(contact.getShapeA()->getBody()->getNode());
//                auto role2 = static_cast<GameRole*>(contact.getShapeB()->getBody()->getNode());
//                
//                if (role1->getName() == "doll")
//                    role1->getPhysicsBody()->setDynamic(true);
//                if (role2->getName() == "doll")
//                    role2->getPhysicsBody()->setDynamic(true);
//                
//                
//                return true;
//            };
//            Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, n);
        }
 
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
    
    if (weather == WEATHER_SUNNY)
        beSunny();
    else if (weather == WEATHER_RAINY)
        beRainy();
    
    return true;
}

bool GameScene::initDoll(){
    
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto dollPoint = tileMap->getObjectGroup("roles")->getObject("doll");
    
    
    auto doll = GameRole::create("doll");
    doll->setPosition(origin + Vec2(dollPoint["x"].asInt()*tileMap->getScale(), dollPoint["y"].asInt()*tileMap->getScale()));
    doll->setScale((dollPoint["y"].asInt()*tileMap->getScale()/2)/doll->getContentSize().height);
    addChild(doll, 1000, "doll");
    
    return true;
}

bool GameScene::initListener(){
    auto touchLayerListener = EventListenerTouchOneByOne::create();
    touchLayerListener->setSwallowTouches(true);
    
    touchLayerListener->onTouchBegan = [this](Touch* touch, Event* event){
        auto doll = static_cast<GameRole*>(this->getChildByName("doll"));
        
        doll->startWalk(touch->getLocation());
        
        return true;
    };
    
    touchLayerListener->onTouchMoved = [this](Touch* touch, Event* event){
        auto doll = static_cast<GameRole*>(this->getChildByName("doll"));
        
        auto velocity = doll->getPhysicsBody()->getVelocity();
        auto turnAround = ((touch->getLocation().x - doll->getPosition().x) * int(velocity.x)) >= 0 ? false : true;
        if (turnAround)
            doll->turnAround();
        else if(int(velocity.x) == 0)
            doll->stopWalk();
        
        return true;
    };

    
    touchLayerListener->onTouchEnded = [this](Touch* touch, Event* event){
        auto doll = static_cast<GameRole*>(this->getChildByName("doll"));
        doll->stopWalk();
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchLayerListener, this);
    
    return true;
}
