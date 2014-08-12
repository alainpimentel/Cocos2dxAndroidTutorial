#include "HelloWorldScene.h"

USING_NS_CC;

HelloWorldHud *HelloWorld::_hud = NULL;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    auto hud = HelloWorldHud::create();
    _hud = hud;

    scene->addChild(hud);

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

    _tileMap = TMXTiledMap::create("TileMaps/TileMap.tmx");
    _background = _tileMap->layerNamed("Background");
    _foreground = _tileMap->getLayer("Foreground");

    addChild(_tileMap, 0, 1);

    //Adding played in spawnPoint

    TMXObjectGroup *objectGroup = _tileMap->getObjectGroup("Objects");

    if(objectGroup == NULL){
        CCLog("tile map has no objects object layer");
        return false;
    }

    auto spawnPoint = objectGroup->getObject("SpawnPoint");

    int x = spawnPoint["x"].asInt();
    int y = spawnPoint["y"].asInt();

    _player = CCSprite::create("Player.png");
    _player->setPosition(ccp(x,y));

    this->addChild(_player);
    this->setViewPointCenter(_player->getPosition());

    // touch for movement
    auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [&](Touch *touch, Event *unused_event)->bool {return true;};
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	// collide
	_meta = _tileMap->layerNamed("Meta");
	_meta->setVisible(false);

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

void HelloWorld::setViewPointCenter(CCPoint position) {

    CCSize winSize = CCDirector::sharedDirector()->getWinSize();

    int x = MAX(position.x, winSize.width/2);
    int y = MAX(position.y, winSize.height/2);
    x = MIN(x, (_tileMap->getMapSize().width * this->_tileMap->getTileSize().width) - winSize.width / 2);
    y = MIN(y, (_tileMap->getMapSize().height * _tileMap->getTileSize().height) - winSize.height/2);
    CCPoint actualPosition = ccp(x, y);

    CCPoint centerOfView = ccp(winSize.width/2, winSize.height/2);
    CCPoint viewPoint = ccpSub(centerOfView, actualPosition);
    this->setPosition(viewPoint);
}

void HelloWorld::onTouchEnded(Touch *touch, Event *unused_event)
{
	auto actionTo1 = RotateTo::create(0, 0, 180);
	auto actionTo2 = RotateTo::create(0, 0, 0);
	auto touchLocation = touch->getLocation();

	touchLocation = this->convertToNodeSpace(touchLocation);

	auto playerPos = _player->getPosition();
	auto diff = touchLocation - playerPos;
	if (abs(diff.x) > abs(diff.y)) {
		if (diff.x > 0) {
			playerPos.x += _tileMap->getTileSize().width / 2;
			_player->runAction(actionTo2);
		}
		else {
			playerPos.x -= _tileMap->getTileSize().width / 2;
			_player->runAction(actionTo1);
		}
	}
	else {
	 if (diff.y > 0) {
	    playerPos.y += _tileMap->getTileSize().height / 2;
	 }
		else {
		 playerPos.y -= _tileMap->getTileSize().height / 2;
		}
 }

	if (playerPos.x <= (_tileMap->getMapSize().width * _tileMap->getMapSize().width) &&
	 playerPos.y <= (_tileMap->getMapSize().height * _tileMap->getMapSize().height) &&
	 playerPos.y >= 0 &&
	 playerPos.x >= 0)
	{
	 this->setPlayerPosition(playerPos);

 }

	this->setViewPointCenter(_player->getPosition());
}

void HelloWorld::setPlayerPosition(Point position)
{
	Point tileCoord = this->tileCoordForPosition(position);
	int tileGid = _meta->getTileGIDAt(tileCoord);
	if (tileGid) {
		auto properties = _tileMap->getPropertiesForGID(tileGid).asValueMap();
		if (!properties.empty()) {
			auto collision = properties["Collidable"].asString();
			if ("True" == collision) {
				return;
			}
			auto collectable = properties["Collectable"].asString();
			if ("True" == collectable) {
				_meta->removeTileAt(tileCoord);
				_foreground->removeTileAt(tileCoord);
				this->_numCollected++;
				this->_hud->numCollectedChanged(_numCollected);
			}
		}
	}
	_player->setPosition(position);
}


Point HelloWorld::tileCoordForPosition(Point position)
{
	int x = position.x / _tileMap->getTileSize().width;
	int y = ((_tileMap->getMapSize().height * _tileMap->getTileSize().height) - position.y) / _tileMap->getTileSize().height;
	return Point(x, y);
}

bool HelloWorldHud::init()
{
    if (!Layer::init())
    {
        return false;
    }
    auto visibleSize = Director::getInstance()->getVisibleSize();
    label = LabelTTF::create("0", "fonts/Marker Felt.ttf", 18.0f, Size(50, 20), TextHAlignment::RIGHT);
    label->setColor(Color3B(0, 0, 0));
    int margin = 10;
    label->setPosition(visibleSize.width - (label->getDimensions().width / 2) - margin,
        label->getDimensions().height / 2 + margin);
    this->addChild(label);

    return true;
}

void HelloWorldHud::numCollectedChanged(int numCollected)
{
    char showStr[20];
    sprintf(showStr, "%d", numCollected);
    label->setString(showStr);
}

