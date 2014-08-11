#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class HelloWorld : public cocos2d::Layer
{
private:
	cocos2d::CCSpriteBatchNode * _batchNode;
	cocos2d::CCSprite * _ship;
	CCParallaxNode *_backgroundNode;
	CCSprite *_spacedust1;
	CCSprite *_spacedust2;
	CCSprite *_planetsunrise;
	CCSprite *_galaxy;
	CCSprite *_spacialanomaly;
	CCSprite *_spacialanomaly2;

public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
};

#endif // __HELLOWORLD_SCENE_H__
