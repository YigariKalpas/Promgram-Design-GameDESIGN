#include "StartMenuScene.h"
#include "GameScene.h" // 后续会创建的游戏主场景
#include "SimpleAudioEngine.h" // 音效管理

using namespace cocos2d;
using namespace ui;
using namespace CocosDenshion;

// 资源路径定义
// 注意：实际开发时请将图片资源放入Resources文件夹，并确保路径正确
// 您需要准备以下图片（示例文件名，可根据实际修改）：
// - start_menu_bg.jpg/png      : 开始菜单背景图
// - game_logo.png              : 游戏Logo
// - button_start_normal.png    : 开始按钮正常状态
// - button_start_pressed.png   : 开始按钮按下状态
// - button_exit_normal.png     : 退出按钮正常状态
// - button_exit_pressed.png    : 退出按钮按下状态
// - menu_frame.png             : 菜单背景框

const std::string StartMenuScene::BACKGROUND_IMAGE = "Surface.webp";
const std::string StartMenuScene::LOGO_IMAGE_1 = "\images\Zombies\ZombieZombie.webp";
const std::string StartMenuScene::START_BUTTON_NORMAL = "button_start_normal.png";
const std::string StartMenuScene::START_BUTTON_PRESSED = "button_start_pressed.png";
const std::string StartMenuScene::EXIT_BUTTON_NORMAL = "button_exit_normal.png";
const std::string StartMenuScene::EXIT_BUTTON_PRESSED = "button_exit_pressed.png";
const std::string StartMenuScene::MENU_BACKGROUND = "menu_frame.png";

// 创建场景
Scene* StartMenuScene::createScene()
{
    auto scene = Scene::create();
    auto layer = StartMenuScene::create();
    scene->addChild(layer);
    return scene;
}

// 初始化方法
bool StartMenuScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    // 获取可见区域尺寸和原点
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 1. 创建背景
    createBackground();

    // 2. 创建Logo动画
    createLogoAnimation();

    // 3. 创建菜单按钮
    createMenuButtons();

    // 4. 添加版权信息
    auto copyrightLabel = Label::createWithTTF(
        "Plant vs Zombies Demo",
        "fonts/Marker Felt.ttf",
        20
    );
    copyrightLabel->setPosition(Vec2(
        origin.x + visibleSize.width / 2,
        origin.y + copyrightLabel->getContentSize().height + 10
    ));
    copyrightLabel->setColor(Color3B(255, 255, 255));
    this->addChild(copyrightLabel);

    // 5. 添加版本号
    auto versionLabel = Label::createWithTTF(
        "Version 1.0.0",
        "fonts/Marker Felt.ttf",
        16
    );
    versionLabel->setPosition(Vec2(
        origin.x + visibleSize.width - versionLabel->getContentSize().width / 2 - 10,
        origin.y + versionLabel->getContentSize().height + 10
    ));
    versionLabel->setColor(Color3B(200, 200, 200));
    this->addChild(versionLabel);

    return true;
}

// 创建背景
void StartMenuScene::createBackground()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 加载背景图片
    // 资源提示：建议使用1024x768或类似尺寸的背景图，适配各种分辨率
    auto background = Sprite::create(BACKGROUND_IMAGE);
    if (!background)
    {
        // 如果图片加载失败，创建颜色背景作为备用
        background = Sprite::create();
        auto backgroundLayer = LayerColor::create(Color4B(100, 180, 100, 255));
        this->addChild(backgroundLayer, 0);
        CCLOG("Background image not found: %s", BACKGROUND_IMAGE.c_str());
    }
    else
    {
        // 设置背景位置和缩放以适应屏幕
        background->setPosition(Vec2(
            origin.x + visibleSize.width / 2 - 150,
            origin.y + visibleSize.height / 2
        ));

        // 计算缩放比例以适应屏幕
        float scaleX = visibleSize.width / background->getContentSize().width;
        float scaleY = visibleSize.height / background->getContentSize().height;
        background->setScale(scaleX > scaleY ? scaleX : scaleY);

        this->addChild(background, 0);
    }
}

// 创建Logo动画
void StartMenuScene::createLogoAnimation()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 加载游戏Logo
    // 资源提示：建议使用透明背景的PNG格式Logo，尺寸约400x200
    auto logo = Sprite::create(LOGO_IMAGE);
    if (!logo)
    {
        // 如果Logo图片不存在，创建文本Logo作为备用
        logo = Sprite::create();
        auto textLogo = Label::createWithTTF(
            "PLANT VS ZOMBIES",
            "fonts/Marker Felt.ttf",
            48
        );
        textLogo->setPosition(Vec2(
            origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height - 150
        ));
        textLogo->setColor(Color3B(255, 255, 0));
        textLogo->enableShadow(Color4B(0, 0, 0, 128), Size(2, -2));
        this->addChild(textLogo, 1);
        CCLOG("Logo image not found: %s", LOGO_IMAGE.c_str());
    }
    else
    {
        // 设置Logo初始位置（屏幕上方之外）
        logo->setPosition(Vec2(
            origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height + logo->getContentSize().height / 2
        ));
        this->addChild(logo, 1);

        // 创建Logo下落动画
        auto moveDown = MoveTo::create(1.0f, Vec2(
            origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height - 150
        ));

        // 创建弹跳效果
        auto bounceUp = MoveBy::create(0.2f, Vec2(0, 30));
        auto bounceDown = MoveBy::create(0.2f, Vec2(0, -30));

        // 创建闪烁效果
        auto fadeOut = FadeOut::create(0.3f);
        auto fadeIn = FadeIn::create(0.3f);
        auto blink = Sequence::create(fadeOut, fadeIn, fadeOut->clone(), fadeIn->clone(), nullptr);

        // 组合动画
        auto logoAnimation = Sequence::create(
            moveDown,
            bounceUp,
            bounceDown,
            DelayTime::create(0.5f),
            blink,
            nullptr
        );

        logo->runAction(logoAnimation);
    }
}

// 创建菜单按钮
void StartMenuScene::createMenuButtons()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 创建菜单背景框（可选）
    // 资源提示：建议使用带透明度的PNG作为菜单背景框
    auto menuBg = Sprite::create(MENU_BACKGROUND);
    if (menuBg)
    {
        menuBg->setPosition(Vec2(
            origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height / 2
        ));
        this->addChild(menuBg, 1);
    }

    // 创建开始游戏按钮
    // 资源提示：按钮图片建议使用两种状态（正常/按下），尺寸约200x80
    auto startButton = Button::create(
        START_BUTTON_NORMAL,
        START_BUTTON_PRESSED,
        START_BUTTON_NORMAL // 禁用状态使用正常图片
    );

    if (!startButton)
    {
        // 如果按钮图片不存在，创建文本按钮
        startButton = Button::create();
        startButton->setTitleText("开始游戏");
        startButton->setTitleFontSize(28);
        CCLOG("Start button images not found, using default button");
    }

    startButton->setPosition(Vec2(
        origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height / 2 + 50
    ));

    // 添加按钮点击音效（可选）
    startButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED)
        {
            // 播放点击音效
            // SimpleAudioEngine::getInstance()->playEffect("button_click.wav");

            // 执行开始游戏回调
            menuStartCallback(sender);
        }
        });

    this->addChild(startButton, 2);

    // 创建退出游戏按钮
    auto exitButton = Button::create(
        EXIT_BUTTON_NORMAL,
        EXIT_BUTTON_PRESSED,
        EXIT_BUTTON_NORMAL
    );

    if (!exitButton)
    {
        exitButton = Button::create();
        exitButton->setTitleText("退出游戏");
        exitButton->setTitleFontSize(28);
        CCLOG("Exit button images not found, using default button");
    }

    exitButton->setPosition(Vec2(
        origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height / 2 - 50
    ));

    exitButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED)
        {
            // 播放点击音效
            // SimpleAudioEngine::getInstance()->playEffect("button_click.wav");

            // 执行退出游戏回调
            menuExitCallback(sender);
        }
        });

    this->addChild(exitButton, 2);

    // 添加按钮动画效果（悬停放大）
    auto scaleUp = ScaleTo::create(0.3f, 1.1f);
    auto scaleDown = ScaleTo::create(0.3f, 1.0f);
    auto scaleSequence = Sequence::create(scaleUp, scaleDown, nullptr);
    auto repeatScale = RepeatForever::create(scaleSequence);

    // 为开始按钮添加脉冲动画
    startButton->runAction(repeatScale->clone());
}

// 开始游戏按钮回调
void StartMenuScene::menuStartCallback(Ref* pSender)
{
    CCLOG("Start button clicked");

    // 创建场景切换动画
   // auto transition = TransitionFade::create(0.5f, GameScene::createScene());

    // 切换场景
   // Director::getInstance()->replaceScene(transition);
}

// 退出游戏按钮回调
void StartMenuScene::menuExitCallback(Ref* pSender)
{
    CCLOG("Exit button clicked");

    // 退出游戏
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}