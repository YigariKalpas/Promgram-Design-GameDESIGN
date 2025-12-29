#include "GameScene.h"
#include "Level1Scene.h"     // 关卡1场景仍需搭建
//#include "Level2Scene.h"     // 关卡2场景仍需搭建
#include "SimpleAudioEngine.h" // 音效管理

using namespace cocos2d;
using namespace ui;
using namespace CocosDenshion;

// 资源路径常量定义
const std::string GameScene::BACKGROUND_IMAGE = "Almanac_Select_Background.webp";
const std::string GameScene::CHALLENGE1_BUTTON_NORMAL = "smallButton.png";
const std::string GameScene::CHALLENGE1_BUTTON_PRESSED = "smallButtonFade.png";
const std::string GameScene::CHALLENGE2_BUTTON_NORMAL = "smallButton.png";
const std::string GameScene::CHALLENGE2_BUTTON_PRESSED = "smallButtonFade.png";
const std::string GameScene::END_BUTTON_NORMAL = "buttonSlot.png";
const std::string GameScene::END_BUTTON_PRESSED = "buttonSlotFade.png";
const std::string GameScene::MENU_BACKGROUND = "Sculpture0.png";
const std::string GameScene::GAME_BACKGROUND_MUSIC = "audio/Time_Is_Money.mp3";
const std::string GameScene::LEVEL1_PREVIEW = "background1.webp";
const std::string GameScene::LEVEL2_PREVIEW = "Forest.webp";\
const std::string GameScene::LEVEL2_PREVIEW11111111111111111111111111111111111111111111111111111111 = "钟的方向反.png";

// 创建场景
Scene* GameScene::createScene()
{
    auto scene = Scene::create();
    auto layer = GameScene::create();
    scene->addChild(layer);
    return scene;
}

// 初始化方法
bool GameScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    // 初始化变量
    _selectedLevel = 0; //储存当前选中的场景标号
    _levelInfoLabel = nullptr; //1简单
    _levelDifficultyLabel = nullptr; //2困难

    // 获取可见区域尺寸和原点
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 预加载背景音乐
    auto audioEngine = SimpleAudioEngine::getInstance();
    audioEngine->preloadBackgroundMusic(GAME_BACKGROUND_MUSIC.c_str()); //通过内置方法给出预加载防止在切换到此场景中无法播放

    // 1. 创建背景
    createBackground();

    // 2. 创建标题
    createTitle();

    // 3. 创建关卡预览
    createLevelPreviews();

    // 4. 创建菜单按钮
    createMenuButtons();

    // 5. 创建关卡信息面板
    createLevelInfoPanel();

    // 6. 添加返回按钮
    createEndButtons();

    return true;
}

// 场景进入时调用
void GameScene::onEnter()
{
    Scene::onEnter();
    CCLOG("GameScene entered");

    // 播放背景音乐
    playBackgroundMusic();
}

// 场景退出时调用
void GameScene::onExit()
{
    // 停止背景音乐
    stopBackgroundMusic();

    Scene::onExit();
    CCLOG("GameScene exited");
}

// 播放背景音乐
void GameScene::playBackgroundMusic()
{
    auto audioEngine = SimpleAudioEngine::getInstance();

    // 检查文件是否存在
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(GAME_BACKGROUND_MUSIC);

    if (FileUtils::getInstance()->isFileExist(fullPath))
    {
        // 循环播放背景音乐
        audioEngine->playBackgroundMusic(GAME_BACKGROUND_MUSIC.c_str(), true);

        audioEngine->setBackgroundMusicVolume(0.6f);

        CCLOG("GAMESCENE中播放关卡选择背景音乐: %s", GAME_BACKGROUND_MUSIC.c_str());
    }
    else
    {
        CCLOG("GAMESCENE中背景音乐文件不存在: %s", GAME_BACKGROUND_MUSIC.c_str());
    }
}

// 停止背景音乐
void GameScene::stopBackgroundMusic()
{
    auto audioEngine = SimpleAudioEngine::getInstance();
    audioEngine->stopBackgroundMusic();
    CCLOG("背景音乐已停止");
}

// 创建背景
void GameScene::createBackground()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 加载背景图片
    auto background = Sprite::create(BACKGROUND_IMAGE);
    if (!background)
    {
        // 如果图片加载失败，创建颜色背景作为备用
        background = Sprite::create();
        auto backgroundLayer = LayerColor::create(Color4B(50, 80, 120, 255)); // 深蓝色背景
        this->addChild(backgroundLayer, 0);
        CCLOG("Background image not found: %s", BACKGROUND_IMAGE.c_str());
    }
    else
    {
        // 设置背景位置和缩放以适应屏幕
        background->setPosition(Vec2(
            origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height / 2
        ));

        // 计算缩放比例以适应屏幕
        float scaleX = visibleSize.width / background->getContentSize().width;
        float scaleY = visibleSize.height / background->getContentSize().height;
        float scale = scaleX > scaleY ? scaleX : scaleY;
        background->setScale(scale);

        this->addChild(background, 0);
    }
}

// 创建标题
void GameScene::createTitle()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 创建关卡选择标题
    auto title = Label::createWithTTF(
        "SELECT LEVEL",
        "fonts/Marker Felt.ttf",
        96
    );
    auto sun = Sprite::create("sun2.png", Rect(0, 0, 3900 / 30.0, 140)); //创建阳光图标

    sun->setPosition(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height - 200);

    title->setPosition(Vec2(
        origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height - 80
    ));
    title->setColor(Color3B(255, 255, 100));
    title->enableShadow(Color4B(0, 0, 0, 128), Size(2, -2));

    // 添加发光效果
    title->enableGlow(Color4B(255, 200, 0, 128));

    this->addChild(title, 2);
    this->addChild(sun,2) ;

    // 创建副标题
    auto subtitle = Label::createWithTTF(
        "Choose your battlefield",
        "fonts/Marker Felt.ttf",
        24
    );
    subtitle->setPosition(Vec2(
        origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height - 120
    ));
    subtitle->setColor(Color3B(200, 200, 200));

    this->addChild(subtitle, 2);
}

// 创建关卡预览
void GameScene::createLevelPreviews()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 创建预览背景框
    auto previewBg1 = Sprite::create("preview_frame.png");
    auto previewBg2 = Sprite::create("preview_frame.png");

    if (previewBg1 && previewBg2)
    {
        previewBg1->setPosition(Vec2(
            origin.x + visibleSize.width / 4,
            origin.y + visibleSize.height / 2 + 50
        ));
        previewBg2->setPosition(Vec2(
            origin.x + visibleSize.width * 3 / 4,
            origin.y + visibleSize.height / 2 + 50
        ));

        previewBg1->setScale(1.2f);
        previewBg2->setScale(1.2f);

        this->addChild(previewBg1, 1);
        this->addChild(previewBg2, 1);
    }

    // 关卡1预览图
    auto level1Preview = Sprite::create(LEVEL1_PREVIEW);
    if (!level1Preview)
    {
        // 备用预览图
        level1Preview = Sprite::create();
        auto placeholder1 = LayerColor::create(Color4B(100, 150, 100, 255), 200, 150);
        level1Preview->addChild(placeholder1);
        CCLOG("Level 1 preview image not found: %s", LEVEL1_PREVIEW.c_str());
    }

    level1Preview->setPosition(Vec2(
        origin.x + visibleSize.width / 4,
        origin.y + visibleSize.height / 2 + 50
    ));
    level1Preview->setScale(0.5f);
    level1Preview->setTag(100); // 标记为关卡1预览

    this->addChild(level1Preview, 2);

    // 关卡1名称标签
    auto level1Label = Label::createWithTTF(
        "GREEN GARDEN",
        "fonts/Marker Felt.ttf",
        28
    );
    level1Label->setPosition(Vec2(
        origin.x + visibleSize.width / 4,
        origin.y + visibleSize.height / 2 - 80
    ));
    level1Label->setColor(Color3B(100, 255, 100));

    this->addChild(level1Label, 2);

    // 关卡2预览图
    auto level2Preview = Sprite::create(LEVEL2_PREVIEW);
    if (!level2Preview)
    {
        // 备用预览图默认纯色
        level2Preview = Sprite::create();
        auto placeholder2 = LayerColor::create(Color4B(150, 100, 100, 255), 200, 150);
        level2Preview->addChild(placeholder2);
        CCLOG("Level 2 preview image not found: %s", LEVEL2_PREVIEW.c_str());
    }

    level2Preview->setPosition(Vec2(
        origin.x + visibleSize.width * 3 / 4,
        origin.y + visibleSize.height / 2 + 50
    ));
    level2Preview->setScale(0.5f);
    level2Preview->setTag(101); // 标记为关卡2预览

    this->addChild(level2Preview, 2);

    // 关卡2名称标签
    auto level2Label = Label::createWithTTF(
        "GREEN FOREST HARDER",
        "fonts/Marker Felt.ttf",
        28
    );
    level2Label->setPosition(Vec2(
        origin.x + visibleSize.width * 3 / 4,
        origin.y + visibleSize.height / 2 - 80
    ));
    level2Label->setColor(Color3B(255, 100, 100));

    this->addChild(level2Label, 2);
}

// 创建菜单按钮
void GameScene::createMenuButtons()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 关卡1按钮
    auto challenge1Button = Button::create(
        CHALLENGE1_BUTTON_NORMAL,
        CHALLENGE1_BUTTON_PRESSED,
        CHALLENGE1_BUTTON_NORMAL
    );

    // 如果按钮图片不存在，创建文本按钮
    if (!challenge1Button)
    {
        challenge1Button = Button::create();
        challenge1Button->setTitleText("LEVEL 1");
        challenge1Button->setTitleFontSize(24);
        CCLOG("Challenge 1 button images not found, using default button");
    }

    challenge1Button->setPosition(Vec2(
        origin.x + visibleSize.width / 4,
        origin.y + visibleSize.height / 2 - 150
    ));

    // 添加按钮点击事件
    challenge1Button->addTouchEventListener([this](Ref* sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED)
        {
            // 播放点击音效
            SimpleAudioEngine::getInstance()->playEffect("audio/puff.mp3");

            // 设置选中的关卡
            _selectedLevel = 1;

            // 更新关卡信息
            updateLevelInfo(1);


            // 延迟执行关卡进入
            this->scheduleOnce([this](float dt) {
                challenge1Callback(nullptr);
                }, 0.2f, "enter_level1");
        }
        else if (type == Widget::TouchEventType::BEGAN)
        {
            // 鼠标按下时更新预览信息
            updateLevelInfo(1);

        }
        });

    this->addChild(challenge1Button, 3);

    // 添加关卡1按钮文字
    auto challenge1Label = Label::createWithTTF(
        "START LEVEL 1",
        "fonts/Marker Felt.ttf",
        20
    );
    challenge1Label->setPosition(Vec2(
        origin.x + visibleSize.width / 4,
        origin.y + visibleSize.height / 2 - 150
    ));
    challenge1Label->setColor(Color3B(255, 255, 255));

    this->addChild(challenge1Label, 4);

    // 关卡2按钮
    auto challenge2Button = Button::create(
        CHALLENGE2_BUTTON_NORMAL,
        CHALLENGE2_BUTTON_PRESSED,
        CHALLENGE2_BUTTON_NORMAL
    );

    // 如果按钮图片不存在，创建文本按钮
    if (!challenge2Button)
    {
        challenge2Button = Button::create();
        challenge2Button->setTitleText("LEVEL 2");
        challenge2Button->setTitleFontSize(24);
        CCLOG("Challenge 2 button images not found, using default button");
    }

    challenge2Button->setPosition(Vec2(
        origin.x + visibleSize.width * 3 / 4,
        origin.y + visibleSize.height / 2 - 150
    ));

    // 添加按钮点击事件
    challenge2Button->addTouchEventListener([this](Ref* sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED)
        {
            // 播放点击音效
            SimpleAudioEngine::getInstance()->playEffect("audio/puff.mp3");

            // 设置选中的关卡
            _selectedLevel = 2;

            // 更新关卡信息
            updateLevelInfo(2);


            // 延迟执行关卡进入
            this->scheduleOnce([this](float dt) {
                challenge2Callback(nullptr);
                }, 0.2f, "enter_level2");
        }
        else if (type == Widget::TouchEventType::BEGAN)
        {
            // 鼠标按下时更新预览信息
            updateLevelInfo(2);
        }
        });

    this->addChild(challenge2Button, 3);

    // 添加关卡2按钮文字
    auto challenge2Label = Label::createWithTTF(
        "START LEVEL 2",
        "fonts/Marker Felt.ttf",
        20
    );
    challenge2Label->setPosition(Vec2(
        origin.x + visibleSize.width * 3 / 4,
        origin.y + visibleSize.height / 2 - 150
    ));
    challenge2Label->setColor(Color3B(255, 255, 255));

    this->addChild(challenge2Label, 4);

    // 为按钮添加脉冲动画
    auto pulseAnimation = Sequence::create(
        ScaleTo::create(0.5f, 1.05f),
        ScaleTo::create(0.5f, 1.0f),
        nullptr
    );
    auto repeatPulse = RepeatForever::create(pulseAnimation);

    //统一进行闪动
    challenge1Button->runAction(repeatPulse->clone());
    challenge2Button->runAction(repeatPulse->clone());
    challenge1Label->runAction(repeatPulse->clone());
    challenge2Label->runAction(repeatPulse->clone());
}

// 创建关卡信息面板
void GameScene::createLevelInfoPanel()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 信息面板背景
    auto infoBg = Sprite::create("info_panel.png"); //实在找不到合适的难度提示素材，此语句仅为保持文件鲁棒性而留
    if (!infoBg)
    {
        infoBg = Sprite::create();
        auto bgColor = Label::createWithTTF(
            "Information : 1 is easier",
            "fonts/Marker Felt.ttf",
            28
        );

        bgColor->setTextColor(Color4B(255,255,255,200));
        bgColor->setPosition(Vec2(visibleSize.height / 10,visibleSize.width/2));
        infoBg->addChild(bgColor);
    }

    infoBg->setPosition(Vec2(
        origin.x + visibleSize.width / 2,
        origin.y + 120
    ));

    this->addChild(infoBg, 1);

    // 关卡信息标题
    auto infoTitle = Label::createWithTTF(
        "LEVEL INFORMATION",
        "fonts/Marker Felt.ttf",
        22
    );
    infoTitle->setPosition(Vec2(
        origin.x + visibleSize.width / 2,
        origin.y + 180
    ));
    infoTitle->setColor(Color3B(255, 255, 100));

    this->addChild(infoTitle, 2);

    // 关卡信息标签
    _levelInfoLabel = Label::createWithTTF(
        "Select a level to view details",
        "fonts/Marker Felt.ttf",
        18
    );
    _levelInfoLabel->setPosition(Vec2(
        origin.x + visibleSize.width / 2,
        origin.y + 140
    ));
    _levelInfoLabel->setColor(Color3B(255, 255, 255));
    _levelInfoLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    _levelInfoLabel->setWidth(380);

    this->addChild(_levelInfoLabel, 2);

    // 关卡难度标签
    _levelDifficultyLabel = Label::createWithTTF(
        "Difficulty: --",
        "fonts/Marker Felt.ttf",
        16
    );
    _levelDifficultyLabel->setPosition(Vec2(
        origin.x + visibleSize.width / 2,
        origin.y + 100
    ));
    _levelDifficultyLabel->setColor(Color3B(200, 200, 200));

    this->addChild(_levelDifficultyLabel, 2);
}

// 创建返回按钮
void GameScene::createEndButtons()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 返回按钮
    auto backButton = Button::create(
        END_BUTTON_NORMAL,
        END_BUTTON_PRESSED,
        END_BUTTON_NORMAL
    );

    if (!backButton)
    {
        backButton = Button::create();
        backButton->setTitleText("BACK");
        backButton->setTitleFontSize(20);
        CCLOG("Back button images not found, using default button");
    }

    backButton->setPosition(Vec2(
        origin.x + visibleSize.width/5,
        origin.y + visibleSize.height - 40
    ));

    backButton->addTouchEventListener([this](Ref* sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED)
        {
            // 播放点击音效
            SimpleAudioEngine::getInstance()->playEffect("audio/puff.mp3");

            // 执行返回回调
            backCallback(sender);
        }
        });

    this->addChild(backButton, 3);

    // 返回按钮文字
    auto backLabel = Label::createWithTTF(
        "EXIT the GAME",
        "fonts/Marker Felt.ttf",
        16
    );
    backLabel->setPosition(Vec2(
        origin.x + visibleSize.width / 5,
        origin.y + visibleSize.height - 40
    ));
    backLabel->setColor(Color3B(255, 255, 255));

    this->addChild(backLabel, 4);
}

// 更新关卡信息
void GameScene::updateLevelInfo(int level)
{
    if (!_levelInfoLabel || !_levelDifficultyLabel) return;

    if (level == 1)
    {
        _levelInfoLabel->setString("Green Garden: A peaceful garden during daytime. Basic zombies appear slowly. Perfect for beginners.");
        _levelDifficultyLabel->setString("Difficulty: EASY");
        _levelDifficultyLabel->setColor(Color3B(100, 255, 100));
    }
    else if (level == 2)
    {
        _levelInfoLabel->setString("Green Forest: Harder and more danger than the garden.");
        _levelDifficultyLabel->setString("Difficulty: HARD");
        _levelDifficultyLabel->setColor(Color3B(255, 100, 100));
    }
}


// 关卡1回调
void GameScene::challenge1Callback(Ref* pSender)
{
    CCLOG("Level 1 selected");

    // 停止背景音乐
    stopBackgroundMusic();

    // 创建切换动画
    auto transition = TransitionFade::create(0.8f, Level1Scene::createScene());

    // 切换场景
    Director::getInstance()->replaceScene(transition);
}

// 关卡2回调
void GameScene::challenge2Callback(Ref* pSender)
{
    CCLOG("Level 2 selected");

    // 停止背景音乐
    stopBackgroundMusic();

    // 创建切换动画
    //auto transition = TransitionFade::create(0.8f, Level2Scene::createScene());

    // 切换场景
    //Director::getInstance()->replaceScene(transition);
}

// 返回按钮回调
void GameScene::backCallback(Ref* pSender)
{
    CCLOG("end of the game");

    // 停止背景音乐
    stopBackgroundMusic();

    //退出游戏
    Director::getInstance()->end();
}