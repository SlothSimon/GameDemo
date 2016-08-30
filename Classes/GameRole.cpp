//
//  GameRole.cpp
//  Escape
//
//  Created by zhangsimon on 16/8/26.
//
//

#include "GameRole.h"

constexpr static int PATH_SIZE = 50;
constexpr static int MAX_ANIM_SIZE = 10;

USING_NS_CC;
using namespace cocos2d::ui;

GameRole* GameRole::create(const string & roleName)
{
    GameRole *role = new (nothrow) GameRole();
    char path[PATH_SIZE] = {0};
    sprintf(path, "roles/%s_stand.png", roleName.c_str());
    if (role && role->initWithFile(path))
    {
        role->autorelease();
        
        role->setName(roleName);
        role->initAnim();
        role->setAnchorPoint(Vec2(0.5,0));
        role->origText = role->getTexture();
//        auto body = PhysicsBody::createCircle(role->getContentSize().width / 2);//创建一个附加在精灵身体上的圆形物理body
//        role->setPhysicsBody(body);
        
        return role;
    }
    CC_SAFE_DELETE(role);
    return nullptr;
}

void GameRole::initAnim(){
    addAnim("walk");
}

void GameRole::addAnim(const string & animName){
    auto anim = Animation::create();
    for (int i = 1; i <= MAX_ANIM_SIZE; ++i){
        char path[PATH_SIZE] = {0};
        sprintf(path, "roles/%s_%s_%d.png", getName().c_str(), animName.c_str(), i);
        if (FileUtils::getInstance()->isFileExist(path)){
            anim->addSpriteFrameWithFile(path);
        }else{
            break;
        }
    }
    anim->setRestoreOriginalFrame(true);
    anim->setDelayPerUnit(0.3/3.0);
    anim->setLoops(-1);
    AnimationCache::getInstance()->addAnimation(anim, getName() + "_" + animName);
}

void GameRole::walkTo(const Vec2 & pos){
    // 如果正在行走，停止action
    if (getActionByTag(1010)){
        stopActionByTag(1010);
        setTexture(origText);
    }
    
    // 判断左右
    auto dist = pos.x - getPosition().x;
    if (dist > 0){
        setFlippedX(false);
    }else{
        setFlippedX(true);
    }
    
    // 设置速度
    float speed = 100.0;
    dist = abs(dist);
    MoveTo* m = MoveTo::create(dist/speed, Vec2(pos.x, getPosition().y));
    
    Sequence* seq = Sequence::create(m,
                                     CallFunc::create([this](){
                                        this->stopActionByTag(1011);
                                        this->setTexture(this->origText);}),
                                     NULL);
    seq->setTag(1010);
    
    // 如果没有走动的动画，生成动画
    if (!getActionByTag(1011)){
        auto walk = Animate::create(AnimationCache::getInstance()->getAnimation(getName() + "_walk"));
        walk->setTag(1011);
        runAction(walk);
    }

    runAction(seq);
}