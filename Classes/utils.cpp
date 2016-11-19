//
//  Utils.cpp
//  Sunny Doll
//
//  Created by zhangsimon on 16/8/26.
//
//
#include "Utils.h"
#include "Constants.h"

bool saveGameRole(const GameRole* role){
    try {
        auto m = role->getItemList();
        for (const auto i : m){
            UserDefault::getInstance()->setIntegerForKey(("Item:" + i.first).c_str(), i.second);
        }
        UserDefault::getInstance()->flush();
    } catch (exception e) {
        log("ERROR: saveGameRole failed!");
        return false;
    }
    
    return true;
}

bool saveStage(const int stage){
    try {
        UserDefault::getInstance()->setIntegerForKey("currentStage", stage);
        UserDefault::getInstance()->flush();
    } catch (exception e) {
        log("ERROR: saveStage failed!");
        return false;
    }
    
    return true;
}

bool loadGameRole(GameRole* role){
    try {
        int count;
        for (auto i : ItemInfo::NameList){
            count = UserDefault::getInstance()->getIntegerForKey(("Item:" + i).c_str());
            role->addItem(i, count);
        }
    } catch (exception e) {
        log("ERROR: saveGameRole failed!");
        return false;
    }
    
    return true;
}

bool loadStage(int & stage){
    try {
        stage = UserDefault::getInstance()->getIntegerForKey("currentStage");
    } catch (exception e) {
        log("ERROR: loadStage failed!");
        return false;
    }
    
    return true;
}

bool initUserDefault(){
    try {
        UserDefault::getInstance()->deleteValueForKey("currentStage");
        for (auto i : ItemInfo::NameList){
            UserDefault::getInstance()->deleteValueForKey(("Item:" + i).c_str());
        }
    } catch (exception e) {
        return false;
    }
    return true;
}
