//
//  Macros.h
//  Sunny Doll
//
//  Created by zhangsimon on 2016/11/13.
//
//

#ifndef Macros_h
#define Macros_h

/** @def CREATE_GAMESCENE_SUBCLASS(__TYPE__)
 * Define a subclass for GameScene, such as GameScene_1.
 *
 * @param __TYPE__  class type name, such as GameScene_1.
 */
#define CREATE_GAMESCENE_SUBCLASS(__TYPE__) \
class __TYPE__ : public GameScene{ \
public: \
    CREATE_GAMESCENE_SUBCLASS_FUNC(__TYPE__); \
    bool initSpecfic(); \
    void onEnterTransitionDidFinish(); \
    void enterStage(); \
};


#define CREATE_GAMESCENE_SUBCLASS_FUNC(__TYPE__) \
CREATE_FUNC(__TYPE__); \
static Scene* createScene(){ \
    auto scene = Scene::createWithPhysics(); \
    scene->getPhysicsWorld()->setGravity(Vec2(0, -200)); \
    if (DebugParameters::DoDebug) \
        scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL); \
\
    scene->getPhysicsWorld()->setFixedUpdateRate(50); \
\
    auto layer = create(); \
    if (layer != NULL){ \
        layer->setPhyWorld(scene->getPhysicsWorld()); \
        scene->addChild(layer); \
    } \
    else{ \
        \
        Director::getInstance()->end(); \
        exit(0); \
    } \
\
    return scene; \
}

#endif /* Macros_h */
