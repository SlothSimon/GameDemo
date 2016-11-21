//
//  StartScene.cpp
//  Sunny Doll
//
//  Created by zhangsimon on 2016/11/21.
//
//

#include "StartScene.hpp"
#include "GameScene_1.hpp"
#include "GameScene_2.hpp"
#include "GameScene_3.hpp"
#include "GameScene_4.hpp"
#include "GameScene_5.hpp"
#include "Utils.h"
#include "ui/UILayout.h"
#include "ui/UIButton.h"

using namespace cocos2d::ui;

Scene* StartScene::createScene()
{

    auto scene = Scene::create();
    
    auto layer = StartScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool StartScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto origin = Director::getInstance()->getVisibleOrigin();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    UserDefault::getInstance()->setBoolForKey("End2", false);
    
    bool end1 = UserDefault::getInstance()->getBoolForKey("End1", false);
    bool end2 = UserDefault::getInstance()->getBoolForKey("End2", false);
    
    string path = "";
    if (end1 || end2)
        path = "startbg.png";
    else
        path = "startbg2.png";
    auto bg = Sprite::create(path);
    bg->setPosition(origin);
    bg->setAnchorPoint(Vec2::ZERO);
    
    float spx = bg->getTextureRect().getMaxX();
    float spy = bg->getTextureRect().getMaxY();
    bg->setScaleX(visibleSize.width/spx); //设置精灵宽度缩放比例
    bg->setScaleY(visibleSize.height/spy);
    
    auto name  = Sprite::create("name.png");
    name->setAnchorPoint(Vec2(0, 1));
    name->setScale(bg->getScaleX());
    name->setPosition(Vec2(origin.x + name->getContentSize().width*0.05, visibleSize.height));
    addChild(bg);
    addChild(name);
    
    auto button_start = MenuItemImage::create("button_start.png",
                                              "button_start.png",
                                              [](Ref *sender){
                                                  log("start");
                                                  initUserDefault();
                                                  Director::getInstance()->replaceScene(TransitionFade::create(2, GameScene_1::createScene()));
                                              });
    button_start->setScale(bg->getScaleX());
    
    // BUG: 如果使用了同一个素材，就会导致有的按钮显示不出来
    auto button_continue = MenuItemImage::create("button_continue.png",
                                                 "button_continue.png",
                                                 [&](Ref *sender){
                                                    auto stage = UserDefault::getInstance()->getIntegerForKey("currentStage", DebugParameters::StartStage);
                                                     chooseStage(stage);
                                                 });
    button_continue->setScale(bg->getScaleX());
    auto menu = Menu::create(button_start, button_continue, nullptr);

    menu->alignItemsVerticallyWithPadding(button_start->getContentSize().height/2);
    menu->setPosition(Vec2(visibleSize.width/5, visibleSize.height/2));
    menu->setAnchorPoint(Vec2(0, 1));
    addChild(menu);

    return true;
}

void StartScene::chooseStage(int stage){
#define enterStage(__stage__) \
Director::getInstance()->replaceScene(TransitionFade::create(2, __stage__::createScene()));
    switch (stage) {
        case 1: enterStage(GameScene_1); break;
        case 2: enterStage(GameScene_2); break;
        case 3: enterStage(GameScene_3); break;
        case 4: enterStage(GameScene_4); break;
        case 5: enterStage(GameScene_5); break;
        case 6: enterStage(GameScene_5); break;
        default:
            break;
    }
#undef enterStage
}
