//
//  GameRole.cpp
//  Sunny Doll
//
//  Created by zhangsimon on 16/8/26.
//
//

#include "GameRole.h"
#include "Constants.h"
#include "GameScene.h"
#include "ui/UIButton.h"

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
        
        // TODO: 应当把所有资源只加载一次，避免重复加载吃内存
        role->origText = role->getTexture();
        role->initPhysicsBody();
        role->mFSM = GameRoleFSM::createWithGameRole(role);
        role->addChild(role->mFSM);
        role->initListener();
        
        CC_SAFE_DELETE_ARRAY(path);
        return role;
    }
    CC_SAFE_DELETE(role);
    return nullptr;
}

void GameRole::initAnim(){
    addAnim("walk");
    if (getName() == GameRoleName::Doll){
        addAnim("drown");
        addAnim("walkwithgirl");
        addAnim("drownwithgirl");
    }
}

void GameRole::initPhysicsBody(){
    auto body = PhysicsBody::createCircle(getContentSize().width / 4, PhysicsMaterial(1, 0, 1));//创建一个附加在精灵身体上的圆形物理body
    if (getName() == GameRoleName::Doll){
        body->setContactTestBitmask(0xFF);
        body->setCategoryBitmask(0x0F);
        body->setCollisionBitmask(0xF0);
    }else{
        body->setContactTestBitmask(0xFF);
        body->setCategoryBitmask(0x0E);
        body->setCollisionBitmask(0xF0);
    }
    
    body->setPositionOffset(Vec2(0.5, 0.3));
    body->setMass(999);
    body->setRotationEnable(false);
    setPhysicsBody(body);
}

void GameRole::addAnim(const string & animName){
    // TODO: 未来换成加载效率更高的plist方式
    auto anim = Animation::create();
    for (int i = 1; i <= MAX_ANIM_SIZE; ++i){
        auto path = ImagePath::getRoleFramePath(getName(), animName, i);
        if (FileUtils::getInstance()->isFileExist(path)){
            anim->addSpriteFrameWithFile(path);
            CC_SAFE_DELETE_ARRAY(path);
        }else{
            break;
        }
    }
    anim->setRestoreOriginalFrame(true);
    anim->setDelayPerUnit(0.4/4.0);
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
    int dist = 0;
    if (roleFollowed == nullptr)
        dist = pos.x - visibleSize.width/2;
    else
        dist = pos.x - getPosition().x;
    
    if (dist >= 0){
        setFlippedX(false);
    }else{
        setFlippedX(true);
    }
    
    int isNegative = isFlippedX() ? -1 : 1;
    for(auto c : getChildren()){
        auto s = dynamic_cast<Sprite*>(c);
        if (s != NULL && s->getName() == "balloon"){
            if (s->isFlippedX() == isFlippedX()){
                s->setRotation(-s->getRotation());
            }
            s->setFlippedX(!isFlippedX());
        }
    }
    
    if (flyNode == nullptr){
        float speed = 100.0;
        getPhysicsBody()->setSurfaceVelocity(Vec2(speed * (dist>0 ? ((dist==0) ? 0 : -1) : 1), 0));
    }else{
        float flyspeed = 50.0;
        getPhysicsBody()->setVelocity(Vec2(flyspeed * (dist>0 ? ((dist==0) ? 0 : 1) : -1), 0));
    }
    
    // 如果没有走动的动画，生成动画
    if (!getActionByTag(1011)){
        Animate * walk = nullptr;
        if (withGirl){
            walk = Animate::create(AnimationCache::getInstance()->getAnimation(getName() + "_walkwithgirl"));
        }else{
            walk = Animate::create(AnimationCache::getInstance()->getAnimation(getName() + "_walk"));
        }
        walk->setTag(1011);
        runAction(walk);
    }
}

void GameRole::idle(){
    
    stopActionByTag(1011);

    auto physicsbody = getPhysicsBody();
    float speedY = physicsbody->getVelocity().y;
    speedY = speedY <= 0 ? speedY : 0;
    physicsbody->setVelocity(Vec2(0, speedY));
    physicsbody->setSurfaceVelocity(Vec2(0, 0));
    if (!_isMovable){
        if (getName() == GameRoleName::Doll){
            Animate * drown = nullptr;
            if (withGirl){
                drown = Animate::create(AnimationCache::getInstance()->getAnimation(getName() + "_drownwithgirl"));
                setTexture("roles/doll_standwithgirl.png");
            }
            else{
                drown = Animate::create(AnimationCache::getInstance()->getAnimation(getName() + "_drown"));
                setTexture("roles/doll_stand.png");
            }
            origText = getTexture();
            drown->getAnimation()->setLoops(1);
            drown->getAnimation()->setRestoreOriginalFrame(false);
            physicsbody->setDynamic(true);
            physicsbody->setGravityEnable(false);
            runAction(Sequence::create(drown->reverse(),
                                       CallFunc::create([physicsbody]{
                physicsbody->setGravityEnable(true);
            }),
                                       NULL));
        }
    }else{
        setTexture(origText);
    }
    _isMovable = true;
}

void GameRole::drown(){
    static int count = 0;
    auto physicsbody = getPhysicsBody();
    physicsbody->setVelocity(Vec2::ZERO);
    physicsbody->setSurfaceVelocity(Vec2::ZERO);
    stopActionByTag(1011);
    setTexture(origText);

    if (_isMovable){
        if (getName() == GameRoleName::Doll){
            Animate * drown = nullptr;
            if (withGirl){
                drown = Animate::create(AnimationCache::getInstance()->getAnimation(getName() + "_drownwithgirl"));
            }
            else{
                drown = Animate::create(AnimationCache::getInstance()->getAnimation(getName() + "_drown"));
            }
            drown->getAnimation()->setLoops(1);
            drown->getAnimation()->setRestoreOriginalFrame(false);
//            drown = Animate::create(AnimationCache::getInstance()->getAnimation(getName() + "_walk"));
//            runAction(Sequence::create(
//                                    MoveBy::create(1.0f, Vec2(getPosition().x - getPhysicsBody()->getPosition().x, 0)),
//                                    CallFunc::create([this]{
//                _isMovable = false;}),
//                                       NULL));
            runAction(drown);
            if (count == 0)
                runAction(drown->clone());
        }
    }
    _isMovable = false;
}

void GameRole::think(const string & content, CallFunc* callback){
    if (ImagePath::getBubblePath(content) != nullptr){
        auto scale = ScaleBy::create(0.1f, 4.0f);
        Sprite* thinkbubble = static_cast<Sprite*>(getChildByName("bubble"));
        if (thinkbubble != nullptr){
            thinkbubble->removeFromParent();
        }
        thinkbubble = Sprite::create(ImagePath::getBubblePath(content));
        // This is a relative postion
        thinkbubble->setPosition(getContentSize().width/2, getContentSize().height*1.5);
        thinkbubble->setScale(0.8);
        
        addChild(thinkbubble);
        thinkbubble->setName("bubble");
        thinkbubble->runAction(Sequence::create(scale,
                                                DelayTime::create(2.0f),
                                                scale->reverse(),
                                                CallFunc::create([thinkbubble](){
                                                    thinkbubble->removeFromParent();
                                                }),
                                                callback == nullptr ? NULL : callback,
                                                NULL));
        string cont = content;
        thinkbubble->setUserData((void*)&cont);
    }else{
        log("Error: There is no %s bubble!", content.c_str());
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
            auto doll = dynamic_cast<GameRole*>(role1);
            if (doll){
                log("Drown: %f, %f", getPhysicsBody()->getPosition().x, getPhysicsBody()->getPosition().y);
                log("Drown:Sprite: %f, %f", getPosition().x, getPosition().y);
                doll->doAction(GameRoleState::State::Drown);
                return true;
            }
        }else if (role1->getName() == GameRoleName::Girl && role2->getName() == "Water"){
            auto doll = dynamic_cast<GameRole*>(role1->getParent()->getChildByName(GameRoleName::Doll));
            doll->loadGirl();
            return true;
        }
        
        return false;
    };
    
    contactListener->onContactPreSolve = [](PhysicsContact & contact, PhysicsContactPreSolve & solve){
        solve.setRestitution(0);
        return true;
    };
    
    contactListener->onContactSeparate = [this](PhysicsContact & contact){
        auto role1 = contact.getShapeA()->getBody()->getNode();
        auto role2 = contact.getShapeB()->getBody()->getNode();
        
        if (role1->getName() == GameRoleName::Doll && role2->getName() == "Water"){
            auto doll = dynamic_cast<GameRole*>(role1);
            if (doll){
                log("Idle: %f, %f", getPhysicsBody()->getPosition().x, getPhysicsBody()->getPosition().y);
                log("Idle:Sprite: %f, %f", getPosition().x, getPosition().y);
                doAction(GameRoleState::State::Idle);
                return true;
            }
        }
        
        return false;
    };
    
    if (getName() == GameRoleName::Doll)
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
}

void GameRole::changeState(State *state) const {
    mFSM->changeState(state);
}

void GameRole::doAction(const string& action, map<string, void*>& userdata) const{
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(GameRoleState::convertToEventName(this, action), &userdata);
}

void GameRole::doAction(const string& action, string userdata) const{
    map<string, void*> m;
    if (userdata != "")
        m["Data"] = &userdata;
    doAction(action, m);
}

void GameRole::doAction(const string& action, Vec2 pos) const{
    map<string, void*> m;
    m["Data"] = &pos;
    doAction(action, m);
}

void GameRole::doAction(const string& action, GameRole* role) const{
    map<string, void*> m;
    m["Data"] = role;
    doAction(action, m);
}

void GameRole::cry(){
    log("The girl starts crying.");
    // TODO: cry animation texture
    think(GameRoleState::SayContent::Cry);
}

void GameRole::follow(GameRole *role){
    roleFollowed = role;
    schedule(schedule_selector(GameRole::updateFollow), 0.1f, kRepeatForever, 0);
}

void GameRole::unfollow(){
    roleFollowed = nullptr;
    unschedule(schedule_selector(GameRole::updateFollow));
}

void GameRole::fly(Node * flynode){
    if (flyNode == nullptr){
        flyNode = flynode;
        getPhysicsBody()->setGravityEnable(false);
        runAction(Sequence::create(
                                   MoveTo::create(1, getPosition() + Vec2(0, 25)),
                                 Repeat::create(Sequence::create(MoveBy::create(1, Vec2(0,5)),
                                                                 MoveBy::create(1, Vec2(0,-5)),
                                                                 NULL),
                                                -1),
                                 NULL));
        schedule(schedule_selector(GameRole::updateFly), 0.5f, kRepeatForever, 0);
        auto bt = static_cast<Button*>(getParent()->getChildByName("sideBar")->getChildByName("rain"));
        bt->setEnabled(false);
    }
}

void GameRole::unfly(){
    flyNode = nullptr;
    getPhysicsBody()->setGravityEnable(true);
    stopAllActions();
    unschedule(schedule_selector(GameRole::updateFly));
    auto bt = static_cast<Button*>(getParent()->getChildByName("sideBar")->getChildByName("rain"));
    bt->setEnabled(true);
}

void GameRole::updateFollow(float dt){
    auto dist = getPosition().distance(roleFollowed->getPosition());
    float followDist = getContentSize().width * getScale();
    if (dist > followDist || dist < -followDist){
        doAction(GameRoleState::State::Walk, roleFollowed->getPosition());
    }else{
        doAction(GameRoleState::State::Idle);
    }
}

void GameRole::updateFly(float dt){
    if (getPositionX() - getContentSize().width/2*getScale() > flyNode->getPositionX() + flyNode->getContentSize().width || getPositionX() + getContentSize().width/2*getScale() < flyNode->getPositionX()){
        unfly();
    }
}

void GameRole::addItem(const string &itemName, int count){
    if (itemList.find(itemName) == itemList.cend())
        itemList[itemName] = 0;
    if (itemName == "balloon"){
        for (int i = 0 ; i < count ; i++){
            auto bl = Sprite::create(ImagePath::Balloon);
            if (bl){
                bl->setScale(5);
                bl->setPosition(getContentSize().width/2, getContentSize().height*0.8);
                bl->setOpacity(0);
                bl->setName("balloon");
                bl->setAnchorPoint(Vec2(0.4, 0));
                bl->setRotation(-30 + 15 * (itemList[itemName] + i));
                addChild(bl);
                bl->setFlippedX(!isFlippedX());
                bl->setLocalZOrder(-1);
                bl->runAction(FadeIn::create(1));
            }
        }
    }
    itemList[itemName] += count;
}

const map<string, int> GameRole::getItemList() const {return itemList;}

void GameRole::loadGirl(){
    if (getName() == GameRoleName::Doll && !withGirl){
        withGirl = true;
//        setTexture("roles/doll_standwithgirl.png");
        getParent()->getChildByName(GameRoleName::Girl)->runAction(Sequence::create(FadeOut::create(1),
                                                                                    CallFunc::create([this]{
                                                                                        if (_isMovable)
                                                                                            setTexture("roles/doll_standwithgirl.png");
                                                                                        else
                                                                                            setTexture("roles/doll_drownwithgirl_4.png");
                                                                                        origText = getTexture();
                                                                                    }),
                                                                                    NULL));
    }
}

void GameRole::unloadGirl(){
    if (getName() == GameRoleName::Doll && withGirl){
        withGirl = false;
        if (_isMovable)
            setTexture("roles/doll_stand.png");
        else
            setTexture("roles/doll_drown_4.png");
        origText = getTexture();
        auto girl = getParent()->getChildByName(GameRoleName::Girl);
        girl->setPosition(getPosition());
        girl->runAction(FadeIn::create(1));
    }
}
