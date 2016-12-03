//
//  GameScene.cpp
//  Sunny Doll
//
//  Created by zhangsimon on 16/8/19.
//
//
#include <regex>
#include <unistd.h>

#include "GameScene.h"
#include "EndScene.h"
#include "SimpleAudioEngine.h"
#include "ui/UIButton.h"
#include "ui/UILayout.h"
#include "GameRole.h"
#include "Constants.h"
#include "Bubble.hpp"

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
    scene->getPhysicsWorld()->setGravity(Vec2(0, -200));
    if (DebugParameters::DoDebug)
        scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);//调试
    
//    // 设定每秒帧数，防止刚体乱跳
//    scene->getPhysicsWorld()->setFixedUpdateRate(50);
    
    auto layer = create();
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
    
    if (!this->initSpecfic())
        return false;
    
    loadGameRole(dynamic_cast<GameRole*>(getChildByName(GameRoleName::Doll)));
    
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
            SimpleAudioEngine::getInstance()->setEffectsVolume(1.0);
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
    effects["rain"] = SimpleAudioEngine::getInstance()->playEffect(MusicPath::RainEffect, true);
    SimpleAudioEngine::getInstance()->setEffectsVolume(0);
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
    
    tileMap = TMXTiledMap::create(ImagePath::getMapPath(mapName));
    if (tileMap == nullptr)
        return false;
    
    weather = tileMap->getProperty("Weather").asString();
    tileMap->setPosition(origin);
    tileMap->setScale(visibleSize.width/(tileMap->getContentSize().width));
    addChild(tileMap, 10);
    
    if (!initInteraction())
        return false;
    
    if (!initCollision())
        return false;
    
    if (!initWeather())
        return false;
    
    if (!initRole())
        return false;
    
    return true;
}

bool GameScene::initInteraction(){
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    auto objs = tileMap->getObjectGroup("interaction");
    if (objs == nullptr)
        return false;
    
    
    for (const auto & obj : (objs->getObjects())){
        auto uiInfo = obj.asValueMap();
        auto touchNode = Node::create();
//        auto touchNode = LayerColor::create(Color4B::BLACK);
        touchNode->setContentSize(Size(uiInfo["width"].asFloat() * tileMap->getScale(), uiInfo["height"].asFloat()*tileMap->getScale()));
        touchNode->setPosition(origin + Vec2(uiInfo["x"].asFloat(), uiInfo["y"].asFloat())*tileMap->getScale());
        addChild(touchNode, INTERACTION_ZORDER, uiInfo["name"].asString());
        if (DebugParameters::DoDebug)
            touchNode->setOpacity(0.3);
        else
            touchNode->setOpacity(0);
        
        auto touchNodeListener = EventListenerTouchOneByOne::create();
        touchNodeListener->setSwallowTouches(true);
        
        if (uiInfo["type"].asString() == "Message"){
            
        
            auto sunnyMsg = Bubble::create(uiInfo["SunnyBubbleName"].asString());
            if (sunnyMsg == nullptr)
                return false;
            sunnyMsg->setPosition(Vec2(touchNode->getContentSize().width/2, touchNode->getContentSize().height));
            sunnyMsg->setAnchorPoint(Vec2(0.5, 0));
            sunnyMsg->setVisible(false);
            
            auto rainyMsg = Bubble::create(uiInfo["RainyBubbleName"].asString());
            if (rainyMsg == nullptr)
                return false;
            rainyMsg->setPosition(Vec2(touchNode->getContentSize().width/2, touchNode->getContentSize().height));
            rainyMsg->setAnchorPoint(Vec2(0.5, 0));
            rainyMsg->setVisible(false);
            
            touchNode->addChild(sunnyMsg);
            touchNode->addChild(rainyMsg);
            
            touchNodeListener->onTouchBegan = [=](Touch* touch, Event* event){
                Vec2 locationInNode = touchNode->convertTouchToNodeSpace(touch);
                Size s = touchNode->getContentSize();
                Rect rect = Rect(0, 0, s.width, s.height);
                //判断触摸区域是否在目标上
                if (rect.containsPoint(locationInNode)){
                    
                    // 判断交互对象和主角的距离
                    auto doll = dynamic_cast<GameRole*>(getChildByName("doll"));
                    if (doll == NULL)
                        return false;
                    
                    auto dist = doll->getPosition().distance(touchNode->getPosition() + touchNode->getContentSize()/2);
                    if (dist <= INTERACTION_MESSAGE_RANGE){
                        if (weather == WEATHER_SUNNY){
                            rainyMsg->hide();
                            sunnyMsg->show();
                        }
                        else{
                            sunnyMsg->hide();
                            rainyMsg->show();
                        }
                    }else{
                        doll->doAction(GameRoleState::State::Think, GameRoleState::ThinkContent::Walk);
                    }

                    return true;    // return true 会使其他listener失效
                }
                return false;       // return false 会继续执行其他listener
            };
        }else if (uiInfo["type"].asString() == "Item"){
            string itemName = uiInfo["name"].asString();
            touchNodeListener->onTouchBegan = [=](Touch* touch, Event* event){
                Vec2 locationInNode = touchNode->convertTouchToNodeSpace(touch);
                Size s = touchNode->getContentSize();
                Rect rect = Rect(0, 0, s.width, s.height);
                //判断触摸区域是否在目标上
                if (rect.containsPoint(locationInNode)){
                    
                    // 判断交互对象和主角的距离
                    auto doll = dynamic_cast<GameRole*>(getChildByName("doll"));
                    if (doll == NULL)
                        return false;
                    
                    auto dist = doll->getPosition().distance(touchNode->getPosition() + touchNode->getContentSize()/2);
                    if (dist <= INTERACTION_ITEM_RANGE){
                        this->pushCinematic(new Cinematic(doll, GameRoleState::State::Say, -1, GameRoleState::SayContent::Ask));
                        auto blLayer = tileMap->getLayer(itemName);
                        blLayer->setLocalZOrder(INTERACTION_ZORDER);
                        blLayer->setCascadeOpacityEnabled(true);
                        blLayer->runAction(Sequence::create(DelayTime::create(3),
                                                            FadeOut::create(1),  // TODO: 引擎bug，子节点并不会透明
                                                            CallFunc::create([blLayer, doll, itemName]{
                                                                blLayer->setVisible(false);
                                                                doll->addItem(itemName);
                                                            }),
                                                             NULL));
                        pushCinematic(new Cinematic(doll, GameRoleState::State::Say, -1, GameRoleState::SayContent::Love));
                        doAfterAddItem(itemName);
                        // Bug: 此处无论是移除node还是移除listener都会导致下层（本来被swallow）的touchlistener被触发
                        Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(touchNode);
                    }else{
                        doll->doAction(GameRoleState::State::Think, GameRoleState::ThinkContent::Walk);
                    }
                    
                    return true;    // return true 会使其他listener失效
                }
                return false;       // return false 会继续执行其他listener
            };
        }else if (uiInfo["type"].asString() == "Float"){
            touchNodeListener->onTouchBegan = [=](Touch* touch, Event* event){
                Vec2 locationInNode = touchNode->convertTouchToNodeSpace(touch);
                Size s = touchNode->getContentSize();
                Rect rect = Rect(0, 0, s.width, s.height);
                //判断触摸区域是否在目标上
                if (rect.containsPoint(locationInNode)){
                    
                    // 判断交互对象和主角的距离
                    auto doll = dynamic_cast<GameRole*>(getChildByName("doll"));
                    if (doll == NULL)
                        return false;
                    
                    auto dist = doll->getPosition().distance(touchNode->getPosition() + touchNode->getContentSize()/2);
                    if (dist <= INTERACTION_ITEM_RANGE){
                        // TODO: 小女孩坐在娃娃肩膀处的动画
                        if (weather == WEATHER_SUNNY){
                            doll->fly(touchNode);   
                        }
                    }else{
                        doll->doAction(GameRoleState::State::Think, GameRoleState::ThinkContent::Walk);
                    }
                    return true;    // return true 会使其他listener失效
                }
                return false;       // return false 会继续执行其他listener
            };
        }
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchNodeListener, touchNode);
    }
    
    return true;
}

bool GameScene::initCollision(){
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float pixelPerTile = tileMap->getContentSize().width/tileMap->getMapSize().width;
    
    // 边界
    auto body = PhysicsBody::createEdgeSegment(Vec2(visibleSize.width, 0), Vec2(visibleSize.width, visibleSize.height));
    body->setCategoryBitmask(0x10);
    body->setCollisionBitmask(0x01);
    body->setContactTestBitmask(0x01);
    
    
    // 触到右侧边界进入下一关
    auto listener = EventListenerPhysicsContact::create();
    listener->onContactBegin = [visibleSize,this](PhysicsContact& contact){
        if (contact.getShapeB()->getBody()->getNode()->getName() == "edge"){
            enterStage();
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
        if (bInfo["type"].asString() == "Ground" || bInfo["type"].asString() == "Slope" || bInfo["type"].asString() == "Water" || bInfo["type"].asString() == "Wood"){
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
            for (auto &s : body->getShapes()){
                s->setRestitution(0);       // TODO: 禁止弹跳，根本不起作用，可能是引擎bug
                s->setFriction(PhysicsBodyParameters::getFriction(bInfo["type"].asString()));
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
                }
            }
        }else if (bInfo["type"].asString() == "Fire"){
            auto pf = ParticleSystemQuad::create("map/fire.plist");
            pf->setStartSize(bInfo["width"].asFloat()*tileMap->getScale());
            pf->setPosition(Vec2(bInfo["x"].asFloat() + bInfo["width"].asFloat()/2, bInfo["y"].asFloat() + bInfo["width"].asFloat())*tileMap->getScale());
            pf->setName(bInfo["type"].asString());
            addChild(pf, ROLE_ZORDER);
            if (effects.find("fire") == effects.cend())
                effects["fire"] = SimpleAudioEngine::getInstance()->playEffect(MusicPath::FireEffect, true);
        }else if (bInfo["type"].asString() == "Leaves"){
            auto pf = ParticleSystemQuad::create("map/leaves.plist");
            pf->setPosition(Vec2(bInfo["x"].asFloat() + bInfo["width"].asFloat()/2, bInfo["y"].asFloat() + bInfo["width"].asFloat())*tileMap->getScale());
            pf->setName(bInfo["type"].asString());
            addChild(pf, ROLE_ZORDER - 1);
        }
    }
    
    return true;
}

bool GameScene::initSideBar(){
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto sideBar = Layout::create();
    
    // Vertical layout with top margin
    sideBar->setLayoutType(Layout::Type::VERTICAL);
    auto sideBarPatameters = LinearLayoutParameter::create();
    
    this->addChild(sideBar, INTERACTION_ZORDER + 1, "sideBar");
    
    auto ButtonSun = Button::create(ImagePath::SunnyButton);
    ButtonSun->setVisible(false);
    ButtonSun->setLayoutParameter(sideBarPatameters);
    ButtonSun->addClickEventListener([this](Ref* psender){
        if (weather != WEATHER_SUNNY)
            beSunny();
    });
    ButtonSun->setScale(visibleSize.height*0.1/ButtonSun->getContentSize().height);
    
    sideBarPatameters->setMargin(Margin(0, 5, 0, 0));
    sideBar->setAnchorPoint(Vec2(0, 1));
    sideBar->setPosition(origin + Vec2(10, visibleSize.height - 10));
    
    auto ButtonRain = Button::create(ImagePath::RainyButton);
    ButtonRain->setVisible(false);
    ButtonRain->setLayoutParameter(sideBarPatameters);
    ButtonRain->addClickEventListener([this](Ref* psender){
        if (weather != WEATHER_RAINY)
            beRainy();
    });
    ButtonRain->setScale(visibleSize.height*0.1/ButtonRain->getContentSize().height);
    
    auto ButtonMenu = Button::create(ImagePath::MenuButton);
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
    ButtonMenu->setScale(visibleSize.height*0.1/ButtonMenu->getContentSize().height);
    
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
        SimpleAudioEngine::getInstance()->preloadEffect(MusicPath::RainEffect);
        SimpleAudioEngine::getInstance()->setEffectsVolume(0);
        
        // sun
        auto sun = Sprite::create(ImagePath::Sun);
        sun->setOpacity(0);
        sun->setScale(0.5);
        sun->setPosition(visibleSize.width - sun->getContentSize().width*sun->getScale(),
                         origin.y + visibleSize.height - sun->getContentSize().height*sun->getScale());
        weatherLayer->addChild(sun, 1, "sun");
            
    }
    
    if (weather == WEATHER_SUNNY)
        beSunny();
    else if (weather == WEATHER_RAINY)
        beRainy();
    
    return true;
}

bool GameScene::initRole(){
    float pixelPerTile = tileMap->getContentSize().width/tileMap->getMapSize().width;
    float scale = tileMap->getScale();
    pixelPerTile *= scale;
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto roles = tileMap->getObjectGroup("roles")->getObjects();
    for (auto & r : roles){
        auto role = r.asValueMap();
        auto roleSprite = GameRole::create(role["name"].asString());
        if (role["name"].asString() == GameRoleName::Doll){
            addChild(roleSprite, DOLL_ZORDER, role["name"].asString());
        }
        else
            addChild(roleSprite, ROLE_ZORDER, role["name"].asString());
        
        roleSprite->setScale(2*pixelPerTile/roleSprite->getContentSize().height);
        roleSprite->setPosition(origin + Vec2(role["x"].asInt()*scale, role["y"].asInt()*scale));
    }
    
    return true;
}

bool GameScene::initListener(){
    auto touchNode = Node::create();
    auto touchLayerListener = EventListenerTouchOneByOne::create();
    touchLayerListener->setSwallowTouches(true);
    
    touchLayerListener->onTouchBegan = [this](Touch* touch, Event* event){
        auto role = dynamic_cast<GameRole*>(getChildByName(GameRoleName::Doll));
        log("I will move");
        if (role == NULL)
            return false;
        
        if (role->IsMovable()){
            role->doAction(GameRoleState::State::Walk, touch->getLocation());
        }
        else{
            role->doAction(GameRoleState::State::Think, GameRoleState::ThinkContent::Drown);
        }
        return true;
    };
    
    touchLayerListener->onTouchEnded = [this](Touch* touch, Event* event){
        
        auto role = dynamic_cast<GameRole*>(getChildByName(GameRoleName::Doll));
        if (role != NULL)
            if (role->IsMovable())
                role->doAction(GameRoleState::State::Idle);
    };
    
    addChild(touchNode, INTERACTION_ZORDER - 1);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchLayerListener, touchNode);
    
    auto notouchNode = Node::create();
    auto notouchNodeListener = EventListenerTouchOneByOne::create();
    notouchNodeListener->setSwallowTouches(true);
    notouchNodeListener->onTouchBegan = [this](Touch* touch, Event* event){
        if (isPlayCinematic)
            return true;
        else
            return false;
    };
    addChild(notouchNode, INTERACTION_ZORDER + 2, "NoTouchNode");
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(notouchNodeListener, notouchNode);
    // Bug: pause了但是依然会进入到onTouchBegan
    Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(notouchNode);
    
    return true;
}

bool GameScene::initBGM(){
    if (!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying()){
        SimpleAudioEngine::getInstance()->preloadBackgroundMusic(MusicPath::normalBGM);
        SimpleAudioEngine::getInstance()->playBackgroundMusic(MusicPath::normalBGM, true);
        SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.1);
        

    }
    
    return true;
}

void GameScene::playCinematic(Cinematic cine){
    map<string, void*> m;
    m["Data"] = &cine.userdata;
    
    if (cine.delay < 0)
        m["Callback"] = CallFunc::create([=]{this->nextCinematic();});
    else if (cine.delay == 0){
        thread mThread([this,cine]{
            this->nextCinematic();
        });
        mThread.detach();
    }
    else {
        thread mThread([this,cine]{
            sleep(cine.delay);
            this->nextCinematic();
        });
        mThread.detach();
    }
    cine.role->doAction(cine.action, m);
}

void GameScene::nextCinematic(){
    // 会发生并发问题吗？
    if (seqCinematic.empty()){
        isPlayCinematic = false;
//        Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(getChildByName("NoTouchNode"));
    }else{
        auto func = seqCinematic.front();
        playCinematic(func);
        seqCinematic.pop();
    }
}

void GameScene::pushCinematic(Cinematic* cine){
    if (!isPlayCinematic){
//        Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(getChildByName("NoTouchNode"));
        isPlayCinematic = true;
        playCinematic(*cine);
    }else{
        seqCinematic.push(*cine);
    }
    CC_SAFE_DELETE(cine);
}

bool GameScene::initSpecfic(){
    return true;
}

void GameScene::enterStage(){
    SimpleAudioEngine::getInstance()->stopAllEffects();
    auto doll = dynamic_cast<GameRole*>(getChildByName(GameRoleName::Doll));
    if (doll != NULL)
        saveGameRole(doll);
}

void GameScene::doAfterAddItem(const string &){}
