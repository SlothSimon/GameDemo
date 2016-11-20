#include "EndScene.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"
#include "ui/UIText.h"

USING_NS_CC;
using namespace cocos2d::ui;
using namespace CocosDenshion;
using namespace std;

Scene* EndScene::createScene(const string & msg)
{
    SimpleAudioEngine::getInstance()->stopAllEffects();
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    
    auto scene = Scene::create();
    
    auto layer = EndScene::create(msg);

    scene->addChild(layer);

    return scene;
}

EndScene * EndScene::create(const string & msg){
    EndScene *pRet = new(std::nothrow) EndScene();
    pRet->endMessage = msg;
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
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

    auto ending = Text::create(endMessage, "fonts/arial.ttf", 20);
    ending->setPosition(origin + visibleSize/2);
    ending->setAnchorPoint(Vec2(0.5,0.5));
    ending->setOpacity(0);
    ending->setTextColor(Color4B::WHITE);
    addChild(ending);
    ending->runAction(Sequence::create(FadeIn::create(2),
                                        DelayTime::create(2),
                                       CallFunc::create([](){
        Director::getInstance()->replaceScene(TransitionFade::create(2, HelloWorld::createScene()));
    }),
                                        NULL));
    
    return true;
}

