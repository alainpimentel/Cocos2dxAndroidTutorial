#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

using namespace cocos2d;

class HelloWorldHud : public cocos2d::Layer
{
public:
    void numCollectedChanged(int numCollected);
    virtual bool init();
    CREATE_FUNC(HelloWorldHud);

    cocos2d::LabelTTF *label;
};

class HelloWorld : public cocos2d::Layer
{
private:
	TMXTiledMap *_tileMap;
    TMXLayer *_background;
    CCSprite *_player;
    TMXLayer *_meta; //collide
    TMXLayer *_foreground;
    int _numCollected;
    static HelloWorldHud *_hud;

public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

    void setViewPointCenter(CCPoint position);

    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    //void setViewPointCenter(cocos2d::Point position);
    void setPlayerPosition(cocos2d::Point position);
    //calculate the tile's coordinate
    Point tileCoordForPosition(cocos2d::Point position);
};

#endif // __HELLOWORLD_SCENE_H__
