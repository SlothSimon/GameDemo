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
    auto path = ImagePath::getRoleFramePath(roleName, "stand");
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
    if (getName() == GameRoleName::Doll)
        addAnim("drown");
}

void GameRole::initPhysicsBody(){
    auto body = PhysicsBody::createCircle(getContentSize().width / 2, PhysicsMaterial(1, 0, 1));//创建一个附加在精灵身体上的圆形物理body
    if (getName() == GameRoleName::Doll){
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
}

void GameRole::drown(){
    getPhysicsBody()->setVelocity(Vec2::ZERO);
    getPhysicsBody()->setSurfaceVelocity(Vec2::ZERO);
    stopAllActions();
    _isMovable = false;
}

void GameRole::think(const string & content, CallFunc* callback){
    if (ImagePath::BubbleMap.find(content) != ImagePath::BubbleMap.cend()){
        auto scale = ScaleBy::create(0.1f, 4.0f);
        Node* thinkbubble = getChildByName("bubble");
        if (thinkbubble != nullptr){
            thinkbubble->removeFromParent();
        }
        thinkbubble = Sprite::create(ImagePath::BubbleMap.at(content));
        // This is a relative postion
        thinkbubble->setPosition(Vec2(60, 200));
        thinkbubble->setScale(this->getContentSize().width/thinkbubble->getContentSize().width/4);
        
        addChild(thinkbubble);
        thinkbubble->setName("bubble");
        auto act = Sequence::create(CallFunc::create([thinkbubble, scale, callback]{
                                        thinkbubble->runAction(Sequence::create(scale,
                                                                                DelayTime::create(2.0f),
                                                                                scale->reverse(),
                                                                                CallFunc::create([thinkbubble](){
                                                                                    thinkbubble->removeFromParent();
                                                                                }),
                                                                                callback == nullptr ? NULL : callback,
                                                                                NULL));
                                    }),
                                    NULL);
        string cont = content;
        thinkbubble->setUserData((void*)&cont);
        getActionManager()->addAction(act, this, false);
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
        
        if (role1->getName() == GameRoleName::Doll && role2->getName() == "Water"){
            doAction(GameRoleState::State::Drown);
        }
        
        return true;
    };
    
    contactListener->onContactSeparate = [this](PhysicsContact & contact){
        auto role1 = contact.getShapeA()->getBody()->getNode();
        auto role2 = contact.getShapeB()->getBody()->getNode();
        
        if (role1->getName() == GameRoleName::Doll && role2->getName() == "Water"){
            doAction(GameRoleState::State::Idle);
        }
        
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
}

void GameRole::changeState(State *state) const {
    mFSM->changeState(state);
}

void GameRole::doAction(const string& action, void* userdata) const{
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(GameRoleState::convertToEventName(this, action), userdata);
    
}
