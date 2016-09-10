#include "EndScene.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"
#include "ui/UIText.h"

USING_NS_CC;
using namespace cocos2d::ui;

Scene* EndScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = EndScene::create();

    scene->addChild(layer);

    return scene;
}

bool EndScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto origin = Director::getInstance()->getVisibleOrigin();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    auto bg = LayerColor::create(Color4B::BLACK, visibleSize.width, visibleSize.height);
    bg->setPosition(origin);
    addChild(bg);
    
    auto message = Text::create("TO BE CONTINUED...", "arial", 20);
    
    message->setPosition(origin + visibleSize/2);
    message->setAnchorPoint(Vec2(0.5, 0.5));
    message->setOpacity(0);
    message->setTextColor(Color4B::WHITE);
    
    
    addChild(message);
    message->runAction(Sequence::create(FadeIn::create(2),
                                     DelayTime::create(2),
                                     CallFunc::create([](){
        Director::getInstance()->replaceScene(TransitionFade::create(2, HelloWorld::createScene()));
    }), NULL));
    
    return true;
}

