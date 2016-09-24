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
        role->initPhysicsBody();
        
        return role;
    }
    CC_SAFE_DELETE(role);
    return nullptr;
}

void GameRole::initAnim(){
    addAnim("walk");
}

void GameRole::initPhysicsBody(){
    auto body = PhysicsBody::createCircle(getContentSize().width / 2, PhysicsMaterial(1, 0, 1));//创建一个附加在精灵身体上的圆形物理body
    if (getName() == "doll"){
        body->setContactTestBitmask(0xFF);
        body->setCategoryBitmask(0x0F);
        body->setCollisionBitmask(0xF0);
    }else{
        body->setContactTestBitmask(0xFF);
        body->setCategoryBitmask(0x0E);
        body->setCollisionBitmask(0xF0);
    }
    
    body->setMass(999);
    body->setVelocityLimit(100);
    body->setRotationEnable(false);
    setPhysicsBody(body);
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

void GameRole::startWalk(const Vec2 & pos){
    // 判断左右
    auto visibleSize = Director::getInstance()->getVisibleSize();
    int dist = pos.x - visibleSize.width/2;
    if (dist >= 0){
        setFlippedX(false);
    }else{
        setFlippedX(true);
    }
    
//    auto force = getPhysicsBody()->getForce();
//    auto velocityDirection = (force == Vec2::ZERO) ? Vec2(1,0) : Vec2(force.y, -force.x)/sqrt(force.x * force.x + force.y * force.y);
//    
//    if (velocityDirection.y / velocityDirection.x > 1 )
//        velocityDirection = Vec2(0.5, 0.5);
    
    float speed = 100.0;
//    float speedY = getPhysicsBody()->getVelocity().y;
//    getPhysicsBody()->setVelocity(Vec2(speed * (dist>0 ? (dist=0 ? 0 : 1) : -1), speedY));
    getPhysicsBody()->setSurfaceVelocity(Vec2(speed * (dist>0 ? (dist=0 ? 0 : -1) : 1), 0));
    
    
    // 如果没有走动的动画，生成动画
    if (!getActionByTag(1011)){
        auto walk = Animate::create(AnimationCache::getInstance()->getAnimation(getName() + "_walk"));
        walk->setTag(1011);
        runAction(walk);
    }

}

void GameRole::turnAround(){
    if (isFlippedX()){
        setFlippedX(false);
    }else{
        setFlippedX(true);
    }
    float speedX = -getPhysicsBody()->getVelocity().x;
    float speedY = getPhysicsBody()->getVelocity().y;
    getPhysicsBody()->setVelocity(Vec2(speedX, speedY));
}

void GameRole::stopWalk(){
    stopActionByTag(1011);
    float speedY = getPhysicsBody()->getVelocity().y;
    speedY = speedY <= 0 ? speedY : 0;
    getPhysicsBody()->setVelocity(Vec2(0, speedY));
    getPhysicsBody()->setSurfaceVelocity(Vec2(0, 0));
    setTexture(origText);
}
