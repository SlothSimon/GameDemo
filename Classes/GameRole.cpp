//
//  GameRole.cpp
//  Escape
//
//  Created by zhangsimon on 16/8/26.
//
//

#include "GameRole.h"
#include "Constants.h"

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
        role->mFSM = GameRoleFSM::createWithGameRole(role);
        role->addChild(role->mFSM);
        if (roleName == "doll")
            role->initListener();
        
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
    body->setRotationEnable(false);
    setPhysicsBody(body);
}

void GameRole::addAnim(const string & animName){
    auto anim = Animation::create();
    for (int i = 1; i <= MAX_ANIM_SIZE; ++i){
        auto path = ImagePath::getRoleFramePath(getName(), animName, i);
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

void GameRole::walk(const Vec2 & pos){
    // 判断左右
    auto visibleSize = Director::getInstance()->getVisibleSize();
    int dist = pos.x - visibleSize.width/2;
    if (dist >= 0){
        setFlippedX(false);
    }else{
        setFlippedX(true);
    }
    
    float speed = 100.0;
    getPhysicsBody()->setSurfaceVelocity(Vec2(speed * (dist>0 ? ((dist==0) ? 0 : -1) : 1), 0));
    
    
    // 如果没有走动的动画，生成动画
    if (!getActionByTag(1011)){
        auto walk = Animate::create(AnimationCache::getInstance()->getAnimation(getName() + "_walk"));
        walk->setTag(1011);
        runAction(walk);
    }
}

void GameRole::idle(){
    stopActionByTag(1011);
    float speedY = getPhysicsBody()->getVelocity().y;
    speedY = speedY <= 0 ? speedY : 0;
    getPhysicsBody()->setVelocity(Vec2(0, speedY));
    getPhysicsBody()->setSurfaceVelocity(Vec2(0, 0));
    setTexture(origText);
    _isMovable = true;
    Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(this->getParent());
}

void GameRole::drown(){
    getPhysicsBody()->setVelocity(Vec2::ZERO);
    getPhysicsBody()->setSurfaceVelocity(Vec2::ZERO);
    stopAllActions();
    _isMovable = false;
    Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(this->getParent());
}

void GameRole::think(const string & content){
    if (ImagePath::BubbleMap.find(content) != ImagePath::BubbleMap.cend()){
        auto thinkbubble = Sprite::create(ImagePath::BubbleMap.at(content));
        thinkbubble->setPosition(Vec2(-30, 200));  // TODO: 需要相对位置
        thinkbubble->setScale(this->getContentSize().width/thinkbubble->getContentSize().width/4);
        
        addChild(thinkbubble);
        auto scale = ScaleBy::create(0.1f, 4.0f);
        thinkbubble->runAction(Sequence::create(scale,
                                                DelayTime::create(2.0f),
                                                scale->reverse(),
                                                CallFunc::create([thinkbubble](){
                                                    thinkbubble->removeFromParent();
                                                }),
                                                NULL));
    }else{
        log("Error: There is no such bubble!");
    }
}

GameRoleFSM* GameRole::getFSM() const{
    return mFSM;
}

bool GameRole::IsMovable() const {
    return _isMovable;
}

void GameRole::initListener(){
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = [this](PhysicsContact & contact){
        auto role1 = contact.getShapeA()->getBody()->getNode();
        auto role2 = contact.getShapeB()->getBody()->getNode();
        
        if (role1->getName() == "doll" && role2->getName() == "Water"){
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(GameRoleState::toString(this, GameRoleState::State::Drown));
        }
        
        return true;
    };
    
    contactListener->onContactSeparate = [this](PhysicsContact & contact){
        auto role1 = contact.getShapeA()->getBody()->getNode();
        auto role2 = contact.getShapeB()->getBody()->getNode();
        
        if (role1->getName() == "doll" && role2->getName() == "Water"){
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(GameRoleState::toString(this, GameRoleState::State::Idle));
        }
        
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
}
