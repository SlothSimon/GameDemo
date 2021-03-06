#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "StartScene.hpp"
#include "Utils.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    
    auto layer = HelloWorld::create();

    scene->addChild(layer);

    return scene;
}

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto origin = Director::getInstance()->getVisibleOrigin();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    auto bg = LayerColor::create(Color4B::WHITE, visibleSize.width, visibleSize.height);
    bg->setPosition(origin);
    addChild(bg);
    
    auto logo = Sprite::create("sloth.png");
    logo->setPosition(origin + visibleSize/2);
    logo->setAnchorPoint(Vec2(0.5, 0.5));
    logo->setScale(visibleSize.width*0.5/logo->getContentSize().width);
    logo->setOpacity(0);
    
    addChild(logo);
    logo->runAction(Sequence::create(FadeIn::create(2),
                                     DelayTime::create(2),
                                     CallFunc::create([](){
        Director::getInstance()->replaceScene(TransitionFade::create(2, StartScene::createScene()));
    }), NULL));
    
    
    return true;
}

