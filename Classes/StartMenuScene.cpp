#include "StartMenuScene.h"
#include "GameScene.h" // 创建的游戏主场景
#include "SimpleAudioEngine.h" // 音效管理

using namespace cocos2d;
using namespace ui;
using namespace CocosDenshion; 

// 图片资源管理部分
const std::string StartMenuScene::BACKGROUND_IMAGE = "Surface.webp";
const std::string StartMenuScene::LOGO_IMAGE_0 = "DoomShroom.png";
const std::string StartMenuScene::LOGO_IMAGE_1 = "Zombie-1.png";
const std::string StartMenuScene::START_BUTTON_NORMAL = "buttonSlot.png";
const std::string StartMenuScene::START_BUTTON_PRESSED = "buttonSlotFade.png";
const std::string StartMenuScene::EXIT_BUTTON_NORMAL = "buttonSlot.png";
const std::string StartMenuScene::EXIT_BUTTON_PRESSED = "buttonSlotFade.png";
const std::string StartMenuScene::MENU_BACKGROUND = "Notice.webp";
const std::string StartMenuScene::MENU_BACKGROUND_MUSIC = "audio/diary.mp3"; 

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
    // 上层没有初始化成功则直接返回
    if (!Scene::init())
    {
        return false;
    }

    // 获取可见区域尺寸和原点，方便在后续运算位置时简化语句
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 预加载背景音乐资源
    auto audioEngine = SimpleAudioEngine::getInstance();
    audioEngine->preloadBackgroundMusic(MENU_BACKGROUND_MUSIC.c_str());

    // 1. 创建背景
    createBackground();

    // 2. 创建Logo动画
    createLogoAnimation();

    // 3. 创建菜单按钮
    createMenuButtons();

    // 4. 添加作业信息
    auto homeWorkLabel = Label::createWithTTF(
        "Plant vs Zombies Demo 2056173 2452686",
        "fonts/Marker Felt.ttf",
        20
    );
    homeWorkLabel->setPosition(Vec2(
        origin.x + visibleSize.width / 2 + 150,
        origin.y + homeWorkLabel->getContentSize().height + 10
    ));
    homeWorkLabel->setColor(Color3B(185, 185, 185));
    this->addChild(homeWorkLabel, 1);

    // 5. 添加版本号
    auto versionLabel = Label::createWithTTF(
        "Version 1.2.1",
        "fonts/Marker Felt.ttf",
        16
    );
    versionLabel->setPosition(Vec2(
        origin.x + visibleSize.width - versionLabel->getContentSize().width / 2 - 10,
        origin.y + versionLabel->getContentSize().height + 10
    ));
    versionLabel->setColor(Color3B(200, 200, 200));
    this->addChild(versionLabel, 1);

    // 添加音乐控制按钮（可选）
    // createMusicControlUI();

    return true;
}

// 场景进入时调用
void StartMenuScene::onEnter()
{
    Scene::onEnter();
    CCLOG("StartMenuScene entered");

    // 播放背景音乐
    playBackgroundMusic();
}

// 场景退出时调用
void StartMenuScene::onExit()
{
    // 停止背景音乐
    stopBackgroundMusic();

    Scene::onExit();
    CCLOG("StartMenuScene exited");
}

// 播放背景音乐
void StartMenuScene::playBackgroundMusic()
{
    auto audioEngine = SimpleAudioEngine::getInstance();

    // 检查文件是否存在
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(MENU_BACKGROUND_MUSIC);

    if (FileUtils::getInstance()->isFileExist(fullPath))
    {
        // 播放背景音乐，true表示循环播放
        audioEngine->playBackgroundMusic(MENU_BACKGROUND_MUSIC.c_str(), true);

        // 设置音量（0.0f ~ 1.0f）
        audioEngine->setBackgroundMusicVolume(0.7f);

        CCLOG("开始播放背景音乐: %s (循环播放)", MENU_BACKGROUND_MUSIC.c_str());
    }
    else
    {
        CCLOG("背景音乐文件不存在: %s", MENU_BACKGROUND_MUSIC.c_str());
        CCLOG("完整路径: %s", fullPath.c_str());
    }
}

// 停止背景音乐
void StartMenuScene::stopBackgroundMusic()
{
    auto audioEngine = SimpleAudioEngine::getInstance();
    audioEngine->stopBackgroundMusic();
    CCLOG("背景音乐已停止");
}

// 创建背景（保持原样）
void StartMenuScene::createBackground()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 加载背景图片
    auto background = Sprite::create(BACKGROUND_IMAGE);
    if (!background)
    {
        // 如果图片加载失败，创建颜色背景作为备用并在调试模式下输出这个报错信息
        background = Sprite::create();
        auto backgroundLayer = LayerColor::create(Color4B(100, 180, 100, 255));
        this->addChild(backgroundLayer, 10);
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
        background->setScale(scaleX > scaleY ? scaleX : scaleY); //对当前背景图进行缩放，以保证不会有黑边出现

        this->addChild(background, 0);
    }
}

// 创建Logo动画（保持原样）
void StartMenuScene::createLogoAnimation()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 加载游戏Logo
    auto logo = Sprite::create(LOGO_IMAGE_1);
    auto logo_plant = Sprite::create(LOGO_IMAGE_0);
    if (!logo || !logo_plant)
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
        CCLOG("Logo image not found: %s , %s \n\n", LOGO_IMAGE_1.c_str(), LOGO_IMAGE_0.c_str());
    }
    else
    {
        // 设置Logo初始位置（屏幕上方之外）
        logo->setPosition(Vec2(
            origin.x + visibleSize.width / 2 + 100,
            origin.y + visibleSize.height + logo->getContentSize().height / 2
        ));
        this->addChild(logo, 1);
        logo_plant->setPosition(Vec2(
            origin.x + visibleSize.width / 2 - 100,
            origin.y + visibleSize.height + logo->getContentSize().height / 2
        ));
        this->addChild(logo_plant, 1);

        // 创建Logo下落动画
        auto moveDown_zombie = MoveTo::create(1.0f, Vec2(
            origin.x + visibleSize.width / 2 + 50,
            origin.y + visibleSize.height - 150
        ));
        auto moveDown_plant = MoveTo::create(1.0f, Vec2(
            origin.x + visibleSize.width / 2 - 50,
            origin.y + visibleSize.height - 150
        ));

        // 创建弹跳效果
        auto bounceUp = MoveBy::create(0.2f, Vec2(0, 30));
        auto bounceDown = MoveBy::create(0.2f, Vec2(0, -30));

        // 创建隐藏效果
        auto hide = FadeOut::create(0.3f);

        // 创建闪烁效果
        auto fadeOut = FadeOut::create(0.3f);
        auto fadeIn = FadeIn::create(0.3f);
        auto blink = Sequence::create(fadeOut, fadeIn, fadeOut->clone(), fadeIn->clone(), nullptr);

        // 组合动画
        auto logoAnimation_1 = Sequence::create(
            moveDown_zombie,
            bounceUp,
            bounceDown,
            DelayTime::create(0.5f),
            blink,
            hide,
            nullptr
        );
        //如果不使用clone会导致错误的图像动作
        auto logoAnimation_2 = Sequence::create(
            moveDown_plant,
            bounceUp->clone(),
            bounceDown->clone(),
            DelayTime::create(0.5f),
            blink->clone(),
            hide->clone(),
            nullptr
        );

        logo->runAction(logoAnimation_1);
        logo_plant->runAction(logoAnimation_2);
        /*DelayTime::create(2);
        logo->removeFromParentAndCleanup(1);
        logo_plant->removeFromParentAndCleanup(1);*/

        //创建游戏标题
        auto titleGame = Label::createWithTTF("! PLANT   VS   ZOMBIES !",
            "fonts/Marker Felt.ttf",
            72);
        if (!titleGame) {
            CCLOG("Game title not created successful \n\n");
            exit(1);
        }
        else {
            titleGame->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 200));
            titleGame->setTextColor(Color4B(144, 244, 144, 255));

            this->addChild(titleGame, 2);

            auto titleAnimation = Sequence::create(
                fadeIn->clone(),
                nullptr
            );
            titleGame->runAction(titleAnimation);
        }
    }
}

void StartMenuScene::createMenuButtons()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto menuBg = Sprite::create(MENU_BACKGROUND);
    if (menuBg)
    {
        menuBg->setPosition(Vec2(
            origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height / 2
        ));
        menuBg->setScale(1.4);
        this->addChild(menuBg, 0);
    }

    // 创建开始游戏按钮
    auto startButton = Button::create(
        START_BUTTON_NORMAL,
        START_BUTTON_PRESSED,
        START_BUTTON_NORMAL // 禁用状态使用正常图片
    );
    auto textStartButton = Label::createWithTTF(
        "START TO PLAY",
        "fonts/Marker Felt.ttf",
        40);
    textStartButton->setColor(Color3B(255, 255, 255));

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
    textStartButton->setPosition(Vec2(
        origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height / 2 + 50
    )); //居中

    // 添加按钮点击音效
    startButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED)
        {
            // 播放点击音效
            SimpleAudioEngine::getInstance()->playEffect("audio/puff.mp3");

            // 执行开始游戏回调
            menuStartCallback(sender);
        }
        });

    this->addChild(startButton, 2);
    this->addChild(textStartButton, 3);

    // 创建退出游戏按钮
    auto exitButton = Button::create(
        EXIT_BUTTON_NORMAL,
        EXIT_BUTTON_PRESSED,
        EXIT_BUTTON_NORMAL
    );
    auto textEndButton = Label::createWithTTF(
        "END OF THE GAME",
        "fonts/Marker Felt.ttf",
        32);
    textEndButton->setColor(Color3B(255, 255, 255));

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

    textEndButton->setPosition(Vec2(
        origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height / 2 - 50
    )); //居中

    exitButton->addTouchEventListener([&](Ref* sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED)
        {
            // 播放点击音效
            SimpleAudioEngine::getInstance()->playEffect("audio/puff.mp3");

            // 执行退出游戏回调
            menuExitCallback(sender);
        }
        });

    this->addChild(exitButton, 2);
    this->addChild(textEndButton, 3);

    // 添加按钮动画效果（悬停放大）
    auto scaleUp = ScaleTo::create(0.3f, 1.1f);
    auto scaleDown = ScaleTo::create(0.3f, 1.0f);
    auto scaleSequence = Sequence::create(scaleUp, scaleDown, nullptr);
    auto repeatScale = RepeatForever::create(scaleSequence);

    // 为开始按钮添加脉冲动画
    startButton->runAction(repeatScale->clone());
    textStartButton->runAction(repeatScale->clone());
}

// 开始游戏按钮回调（修改为实际场景切换）
void StartMenuScene::menuStartCallback(Ref* pSender)
{
    CCLOG("Start button clicked");

    // 停止当前音乐
    stopBackgroundMusic();

    // 创建场景切换动画
    auto transition = TransitionFade::create(0.5f, GameScene::createScene());

    // 切换场景
    Director::getInstance()->replaceScene(transition);
}

// 退出游戏按钮
void StartMenuScene::menuExitCallback(Ref* pSender)
{
    CCLOG("Exit button clicked");

    // 停止音乐
    stopBackgroundMusic();

    // 退出游戏
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}