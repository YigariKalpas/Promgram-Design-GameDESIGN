#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first , the father class's init func 
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    ////

    auto label = Label::createWithTTF("Arknights game Test", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }

    // add "HelloWorld" splash screen"
    auto sprite3 = Sprite::create("HelloWorld.png");
    auto sprite2 = Sprite::create("SodRoll.png");
   /* auto sprite1 = Sprite::create("down.gif");*/
    auto sprite = Sprite::create("Surface.webp");
    sprite3->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    sprite2->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    //sprite1->setPosition(Vec2(2 * visibleSize.width / 3 + origin.x, 2 * visibleSize.height / 3 + origin.y));
    sprite->setPosition(Vec2(2 * visibleSize.width / 3 + origin.x,  visibleSize.height / 3 + origin.y));
    if (sprite3 == nullptr)
    {
        this->addChild(sprite, 0); // after this scentence the scene render the png and we can see the action applied to the sprite
        //this->addChild(sprite1, 0); // after this scentence the scene render the png and we can see the action applied to the sprite
        this->addChild(sprite2, 0); // after this scentence the scene render the png and we can see the action applied to the sprite
        this->addChild(sprite3, 0); // after this scentence the scene render the png and we can see the action applied to the sprite
        problemLoading("'Surface.webp'");
    }
    else
    {
        // position the sprite on the center of the screen
        sprite3->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
        sprite3->setScale(0.5);
        // add the sprite as a child to this layer
        this->addChild(sprite3, 0); // after this scentence the scene render the png and we can see the action applied to the sprite
        this->addChild(sprite2, 0); // after this scentence the scene render the png and we can see the action applied to the sprite
        this->addChild(sprite, 0); // after this scentence the scene render the png and we can see the action applied to the sprite
        log("the first page loaded successfully");
    }
    return true;
}

// the function of different situation of the windows
void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
