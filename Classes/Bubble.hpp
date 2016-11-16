//
//  Bubble.hpp
//  Sunny Doll
//
//  Created by zhangsimon on 2016/11/16.
//
//

#ifndef Bubble_hpp
#define Bubble_hpp

using namespace cocos2d;
using namespace std;

class Bubble: public Sprite{
public:
    void show();
    void hide();
    
    static Bubble * create(const string & bubbleName);
    
    
private:
    
};

#endif /* Bubble_hpp */
