#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    _batchNode = CCSpriteBatchNode::create("Sprites.pvr.ccz");
    this->addChild(_batchNode);
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("Sprites.plist");

    _ship = CCSprite::createWithSpriteFrameName("SpaceFlier_sm_1.png");
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    _ship->setPosition(ccp(winSize.width * 0.1, winSize.height * 0.5));
    _batchNode->addChild(_ship, 1);

    // 1) Create the CCParallaxNode
	_backgroundNode = ParallaxNodeExtras::create();
	this->addChild(_backgroundNode,-1);

	// 2) Create the sprites will be added to the CCParallaxNode
	_spacedust1 = CCSprite::create("bg_front_spacedust.png");
	_spacedust2 = CCSprite::create("bg_front_spacedust.png");
	_planetsunrise = CCSprite::create("bg_planetsunrise.png");
	_galaxy = CCSprite::create("bg_galaxy.png");
	_spacialanomaly = CCSprite::create("bg_spacialanomaly.png");
	_spacialanomaly2 = CCSprite::create("bg_spacialanomaly2.png");

	// 3) Determine relative movement speeds for space dust and background
	CCPoint dustSpeed = ccp(0.1, 0.1);
	CCPoint bgSpeed = ccp(0.05, 0.05);

	// 4) Add children to CCParallaxNode
	_backgroundNode->addChild(_spacedust1, 0, dustSpeed, ccp(0,winSize.height/2) ); // 2
	_backgroundNode->addChild(_spacedust2, 0, dustSpeed, ccp( _spacedust1->getContentSize().width,winSize.height/2));
	_backgroundNode->addChild(_galaxy, -1, bgSpeed, ccp(0, winSize.height * 0.7));
	_backgroundNode->addChild(_planetsunrise, -1 , bgSpeed, ccp(600, winSize.height * 0));
	_backgroundNode->addChild(_spacialanomaly, -1, bgSpeed, ccp(900, winSize.height * 0.3));
	_backgroundNode->addChild(_spacialanomaly2, -1, bgSpeed, ccp(1500, winSize.height * 0.9));

	// Call the background scroll method
	this->scheduleUpdate();

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void HelloWorld::update(float dt) {
  CCPoint backgroundScrollVert = ccp(-1000, 0);
  _backgroundNode->setPosition(ccpAdd(_backgroundNode->getPosition(), ccpMult(backgroundScrollVert, dt)));

  CCArray *spaceDusts = CCArray::createWithCapacity(2);
  spaceDusts->addObject(_spacedust1);
  spaceDusts->addObject(_spacedust2);
  for ( int ii = 0; ii <spaceDusts->count(); ii++ ) {
      CCSprite * spaceDust = (CCSprite *)(spaceDusts->objectAtIndex(ii));
      float xPosition = _backgroundNode->convertToWorldSpace(spaceDust->getPosition()).x;
      float size = spaceDust->getContentSize().width;
      if ( xPosition < -size/2 ) {
          _backgroundNode->incrementOffset(ccp(spaceDust->getContentSize().width*2,0),spaceDust);
      }
  }

  CCArray *backGrounds = CCArray::createWithCapacity(4);
  backGrounds->addObject(_galaxy);
  backGrounds->addObject(_planetsunrise);
  backGrounds->addObject(_spacialanomaly);
  backGrounds->addObject(_spacialanomaly2);
  for ( int ii = 0; ii <backGrounds->count(); ii++ ) {
      CCSprite * background = (CCSprite *)(backGrounds->objectAtIndex(ii));
      float xPosition = _backgroundNode->convertToWorldSpace(background->getPosition()).x;
      float size = background->getContentSize().width;
      if ( xPosition < -size ) {
          _backgroundNode->incrementOffset(ccp(2000,0),background);
      }
  }
}
