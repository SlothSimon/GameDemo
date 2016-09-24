//
//  GameScene.cpp
//  Escape
//
//  Created by zhangsimon on 16/8/19.
//
//
#include <regex>

#include "GameScene.h"
#include "EndScene.h"
#include "SimpleAudioEngine.h"
#include "ui/UIButton.h"
#include "ui/UILayout.h"
#include "GameRole.h"
#include "AppDelegate.h"

USING_NS_CC;
using namespace CocosDenshion;
using namespace cocos2d::ui;

// TODO: 挪进utils.h中
std::vector<Vec2> StringToPoints(const string & str){
    std::vector<Vec2> poss;     // TODO: 此处必须用std::vector，cocos2d封装的vector没有重载[]操作符，且限制较多
    istringstream iss(str);
    string pos;
    
    while(getline(iss, pos, ';')){
        const std::regex pattern("\\(([-+]?[0-9]*\\.?[0-9]+),([-+]?[0-9]*\\.?[0-9]+)\\)");
    
        std::match_results<string::const_iterator> result;
        
        bool valid = std::regex_match(pos, result, pattern);
        
        if(valid){
            poss.push_back(Vec2(stof(result[1]), stof(result[2])));
        }
    }
    
    
    return poss;
}



Scene* GameScene::createScene()
{
    auto scene = Scene::createWithPhysics();
    log("Gravity:");
    scene->getPhysicsWorld()->setGravity(Vec2(0, -200));
    if (DEBUG)
        scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);//调试
    
    auto layer = GameScene::create();
    if (layer != NULL){
        layer->setPhyWorld(scene->getPhysicsWorld());//将创建的物理世界传递到子层中
        scene->addChild(layer);
    }
    else{
        
        Director::getInstance()->end();
        exit(0);
    }
    
    
    
    return scene;
}

bool GameScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    currentStage = UserDefault::getInstance()->getIntegerForKey("currentStage", 1);

    initialized = false;
    
    if (!initSideBar())
        return false;
    
    if (!initMap(to_string(currentStage)))
        return false;
    
    if (!initListener())
        return false;

    if (!initBGM())
        return false;
    
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
            
            // 若为场景未初始化，进行无动画的初始化，否则进行动画
            if (initialized){
                
                auto m = MoveBy::create(layers.at(i)->getProperty("Duration").asFloat(), deltaDist);
                auto seq = Sequence::create(DelayTime::create(layers.at(i)->getProperty("Delay").asFloat()), EaseSineInOut::create(m), NULL);
                layers.at(i)->runAction(seq);
                auto lname = layers.at(i)->getLayerName();
                if (collisionNodeWithAction.find(lname) != collisionNodeWithAction.cend()){
                    auto m = MoveBy::create(layers.at(i)->getProperty("Duration").asFloat(), deltaDist);
                    auto seq = Sequence::create(DelayTime::create(layers.at(i)->getProperty("Delay").asFloat()), EaseSineInOut::create(m), NULL);
                    for (auto iter = collisionNodeWithAction.equal_range(lname); iter.first != iter.second; iter.first++){
                        iter.first->second->runAction(seq->clone());
                    }
                }
            }else{
                layers.at(i)->setPosition(layers.at(i)->getPosition() + deltaDist);
                auto lname = layers.at(i)->getLayerName();
                if (collisionNodeWithAction.find(lname) != collisionNodeWithAction.cend()){
                    for (auto iter = collisionNodeWithAction.equal_range(lname); iter.first != iter.second; iter.first++){
                        iter.first->second->setPosition(iter.first->second->getPosition() + deltaDist);
                    }
                }
                    
            }
        }
    }
    initialized = true;
}


bool GameScene::initMap(const string & mapName){
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    string path = "map/";
    path = path + mapName + ".tmx";
    
    tileMap = TMXTiledMap::create(path);
    if (tileMap == nullptr)
        return false;
    
    weather = tileMap->getProperty("Weather").asString();
    tileMap->setPosition(origin);
    tileMap->setScale(visibleSize.width/(tileMap->getContentSize().width));
    
    initInteraction();
    
    initCollision();
    
    initWeather();
    
    initDoll();
    
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
        addChild(touchLayer, INTERACTION_ZORDER);
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
//    float scale = tileMap->getScale();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float pixelPerTile = tileMap->getContentSize().width/tileMap->getMapSize().width;
    
    // 边界
    auto body = PhysicsBody::createEdgeSegment(Vec2(visibleSize.width, 0), Vec2(visibleSize.width, visibleSize.height));
//    setPhysicsBody(body);
    body->setCategoryBitmask(0x10);
    body->setCollisionBitmask(0x01);
    body->setContactTestBitmask(0x01);
    
    
    // 触到右侧边界进入下一关
    auto listener = EventListenerPhysicsContact::create();
    listener->onContactBegin = [visibleSize,this](PhysicsContact& contact){
        if (contact.getShapeB()->getBody()->getNode()->getName() == "edge"){
            if (currentStage == ENDSTAGE){
                SimpleAudioEngine::getInstance()->stopAllEffects();
                SimpleAudioEngine::getInstance()->stopBackgroundMusic();
                Director::getInstance()->replaceScene(TransitionFade::create(2, EndScene::createScene()));
            }else{
                UserDefault::getInstance()->setIntegerForKey("currentStage", currentStage + 1);
                Director::getInstance()->replaceScene(TransitionFade::create(2, GameScene::createScene()));
            }
            return false;
        }
        return true;
    };
    
    
    auto n = Node::create();
    n->setPosition(Vec2::ZERO);
    n->setPhysicsBody(body);
    n->setName("edge");
    addChild(n);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, n);
    
    // 在左侧放置刚体防止越过边界坠入深渊  TODO: edge系列body会穿越而无法反弹，不得不放置非edge的刚体
    body = PhysicsBody::createBox(Size(1, visibleSize.height));
    body->setDynamic(false);
    n = Sprite::create();
    n->setPosition(Vec2::ZERO);
    n->setAnchorPoint(Vec2(1,0));
    n->setPhysicsBody(body);
    addChild(n);
    
    // 障碍物
    auto barriers = tileMap->getObjectGroup("barriers")->getObjects();
    for (auto & b : barriers){
        auto bInfo = b.asValueMap();
        
        auto vecArr = StringToPoints(bInfo["VecPoints"].asString());
        for_each(vecArr.begin(), vecArr.end(), [pixelPerTile](Vec2 & pos){
            pos = pos * pixelPerTile;
        });
        
        if (vecArr.size() == 0){
            log("error! vecArr's size is 0");
            exit(0);
        }
            
        auto body = PhysicsBody::createPolygon(&vecArr[0], int(vecArr.size()), PhysicsMaterial(0,0,1.0f));
        
        body->setDynamic(false);
//        body->setPositionOffset(Vec2(width/2, height/2)*scale);     // TODO: 若为规则形状，刚体的锚点在重心，因此要调整到左下角
        for (auto &s : body->getShapes()){
            s->setRestitution(0);       // TODO: 禁止弹跳，根本不起作用，可能是引擎bug
            if (bInfo["type"].asString() == "Water")
                s->setFriction(0.0f);
            else if(bInfo["type"].asString() == "Slope")
                s->setFriction(2.0f);
        }
        
        
        auto n = Sprite::create();
        n->setPhysicsBody(body);
        n->setPosition(Vec2::ZERO);
        n->setAnchorPoint(Vec2::ZERO);
        n->setName(bInfo["type"].asString());
        tileMap->addChild(n);
        
        // 会随其他layer有动画的障碍物
        auto parentLayerName = bInfo["parentLayer"].asString();
        if (parentLayerName != ""){
            collisionNodeWithAction.insert({parentLayerName, n});
            if (parentLayerName == "water"){
                body->setCategoryBitmask(0xFF);
                body->setCollisionBitmask(0xFF);
                body->setContactTestBitmask(0x02);
                
                // 沾水不能动
                auto listener = EventListenerPhysicsContact::create();
                listener->onContactBegin = [this](PhysicsContact & contact){
                    auto role1 = contact.getShapeA()->getBody()->getNode();
                    auto role2 = contact.getShapeB()->getBody()->getNode();
                    
                    if (role1->getName() == "doll" && role2->getName() == "Water"){
                        role1->getPhysicsBody()->setVelocity(Vec2::ZERO);
                        role1->getPhysicsBody()->setSurfaceVelocity(Vec2::ZERO);
                        role1->stopAllActions();
                        Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(this);
                    }
                    
                    return true;
                };
                
                listener->onContactSeparate = [this](PhysicsContact & contact){
                    auto role1 = contact.getShapeA()->getBody()->getNode();
                    auto role2 = contact.getShapeB()->getBody()->getNode();
                    
                    if (role1->getName() == "doll" && role2->getName() == "Water"){
                        Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(this);
                    }
                    
                    return true;
                };
                
                Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, n);
            }
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
    
    this->addChild(sideBar, 20, "sideBar");
    
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
    if (getChildByName("weatherLayer") == nullptr){
        // initialize weatherLayer
        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        weatherLayer = cocos2d::LayerColor::create(cocos2d::Color4B::WHITE, visibleSize.width, visibleSize.height);
        weatherLayer->setPosition(origin);
        this->addChild(weatherLayer, 0, "weatherLayer");
        
        // particleRain sys
        ParticleSystem* rain = ParticleRain::create();
        rain->setSpeed(0.0f);
        rain->setEmissionRate(0);
        this->addChild(rain, 11, "rain");
        
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
            
    }
    
    if (weather == WEATHER_SUNNY)
        beSunny();
    else if (weather == WEATHER_RAINY)
        beRainy();
    
    return true;
}

bool GameScene::initDoll(){
    float pixelPerTile = tileMap->getContentSize().width/tileMap->getMapSize().width;
    float scale = tileMap->getScale();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto roles = tileMap->getObjectGroup("roles")->getObjects();
    for (auto & r : roles){
        auto role = r.asValueMap();
        auto roleSprite = GameRole::create(role["name"].asString());
        if (role["name"].asString() == "doll")
            addChild(roleSprite, DOLL_ZORDER, role["name"].asString());
        else
            addChild(roleSprite, ROLE_ZORDER, role["name"].asString());
        
        roleSprite->setScale((2*pixelPerTile)/roleSprite->getContentSize().height);
        roleSprite->setPosition(origin + Vec2(role["x"].asInt()*scale, role["y"].asInt()*scale));
    }
    
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
    
    touchLayerListener->onTouchEnded = [this](Touch* touch, Event* event){
        auto doll = static_cast<GameRole*>(this->getChildByName("doll"));
        doll->stopWalk();
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchLayerListener, this);
    
    return true;
}

bool GameScene::initBGM(){
    if (!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()){
        SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/firstlove_light.mp3");
        SimpleAudioEngine::getInstance()->playBackgroundMusic("music/firstlove_light.mp3", true);
    }
    
    return true;
}


