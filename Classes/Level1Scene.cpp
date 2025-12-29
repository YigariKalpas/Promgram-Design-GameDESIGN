#include "Level1Scene.h"
#include "SimpleAudioEngine.h" // 音效管理
#include "AudioEngine.h"

using namespace CocosDenshion;

USING_NS_CC;

const std::string Level1Scene::BACKGROUNDMUSIC = "audio/Bgm_Forest_Fight_JX.mp3";

Scene* Level1Scene::createScene() {
    return Level1Scene::create();
}
void Level1Scene::update(float delta) {
    if (_isGameOver) return; // 如果游戏已结束，不再执行后续逻辑
    auto children = this->getChildren();
    // 检查僵尸是否越过底线
    for (auto child : children) {
        auto zombie = dynamic_cast<Zombie*>(child);
        if (zombie) {
            // 设定底线阈值，例如 X = 40 (第一列左侧)
            if (zombie->getPositionX() < 100.0f) {
                this->handleGameOver();
                return; // 发现一个僵尸过线即触发失败
            }
        }
        if (_isLevelFinished && !zombie) {
            this->handleWin();
            return;
        }
    }

    std::vector<Bullet*> bullets;
    std::vector<Zombie*> zombies;

    // 筛选出当前的子弹和僵尸,通过迭代器完成实现
    for (auto child : children) {
        if (auto b = dynamic_cast<Bullet*>(child)) bullets.push_back(b);
        else if (auto z = dynamic_cast<Zombie*>(child)) zombies.push_back(z);
    }

    // 2. 双重循环判定碰撞
    for (auto b : bullets) {
        for (auto z : zombies) {
            // 同行判定 + 矩形相交
            if (b->getRow() == z->getRow() && b->getBoundingBox().intersectsRect(z->getBoundingBox())) {
                z->takeDamage(b->getDamage()); // 僵尸扣血
                b->removeFromParent(); // 子弹销毁
                break;
            }
        }
    }
}

void Level1Scene::onZombieKilled() {
    if (_isGameOver) return; // 游戏已结束则不处理

    _zombiesKilledCounter++;
    CCLOG("Zombies Defeated: %d / %d", _zombiesKilledCounter, _maxZombiesInLevel);

    // 判定胜利：所有生成的僵尸都被消灭
    if (_zombiesKilledCounter >= _maxZombiesInLevel) {
        this->handleWin();
    }
}

void Level1Scene::onEnter() {
    Scene::onEnter(); 
    cocos2d::experimental::AudioEngine::stopAll(); // 停掉之前的
    int audioID = cocos2d::experimental::AudioEngine::play2d(BACKGROUNDMUSIC, true, 0.5f);
}

void Level1Scene::spawnZombie() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    //到达计数器上限时直接结束
    if (_zombiesSpawnedCounter >= _maxZombiesInLevel) {
        if (!_isLevelFinished) {
            _isLevelFinished = true;
            // 停止定时生成器的调度
            this->unschedule("zombie_spawn_key");
            CCLOG("All zombies spawned. Waiting for cleanup...");
        }
        return;
    }
    //在0-4行中随机一行添加僵尸
    int row = RandomHelper::random_int(0, 4);
    //随机数生成不同强度的僵尸
    int typeRand = RandomHelper::random_int(0, 100) + _zombiesSpawnedCounter; //随着当局进入后期也可以增加难度

    Zombie* zombie = nullptr;

    if (typeRand < 60) {
        zombie = NormalZombie::create(row);
    }
    else if (typeRand < 90) {
        zombie = ConeheadZombie::create(row);
    }
    else {
        zombie = BucketheadZombie::create(row);
    }

    if (zombie) {
        int temp = 0; //由于找到的素材存在网格畸变，在实际尝试搭建中出现上方过高下方过低的情况引入此参数进行修改
        if (row == 0)
            temp = -40;
        if (row == 1)
            temp = -10;
        if (row == 2)
            temp = -0;
        if (row == 3)
            temp = 25;
        if (row == 4)
            temp = 30;
        float posY = (row - 2) * _gameBoard->blockSizeY + visibleSize.height / 2 + _gameBoard->blockSizeY/6 + temp; //以中心轴为基本Y轴参考点
        zombie->setPosition(Vec2(Director::getInstance()->getVisibleSize().width - 50, posY));
        this->addChild(zombie, 200 - row);
        _zombiesSpawnedCounter++;
    }
}

bool Level1Scene::init() {
    // 基类初始化校验
    if (!Scene::init()) {
        return false;
    }

    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const auto origin = Director::getInstance()->getVisibleOrigin();

    //  背景设置
    auto layerColor = LayerColor::create(Color4B(34, 34, 139, 100));
    this->addChild(layerColor, 0);

    auto background = Sprite::create("background1.webp");
    if (background) {
        background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
        this->addChild(background, 0);
    }

    // 逻辑对象初始化
    // 初始化网格数据
    _gameBoard = new board();
    _gameBoard->init();

    // 初始化阳光管理器并关联当前场景
    SunManager::getInstance()->init(this);
    SunManager::getInstance()->startGeneratingSuns();

    // UI管理器初始化
    // 传入 this (Scene*) 和 _gameBoard (board*) 确保种植逻辑能访问网格
    float timer = std::max(20.0f - 2.2 * _zombiesSpawnedCounter,2.0);
    _uiManager = GameUIManager::create(this, _gameBoard);
    if (_uiManager) {
        this->addChild(_uiManager, 200); // 确保 UI 在最上层
    }
    this->schedule([this](float dt) {
        this->spawnZombie();
        }, timer, "zombie_spawn_key"); //逐渐增加产生僵尸的速度

    this->scheduleUpdate();
    return true;
}

void Level1Scene::handleWin() {
    // 停止所有调度器
    this->unscheduleUpdate();

    auto label = Label::createWithSystemFont("LEVEL 1 COMPLETED!", "Arial", 48);
    label->setPosition(Director::getInstance()->getWinSize() / 2);
    label->setColor(Color3B::YELLOW);
    this->addChild(label, 999);

    CCLOG("You Win!");
    // 停止背景音乐
    cocos2d::experimental::AudioEngine::stopAll(); // 停掉之前的

    // 创建切换动画
    auto transition = TransitionFade::create(0.8f, GameScene::createScene());

    // 切换场景
    Director::getInstance()->replaceScene(transition);
}

void Level1Scene::handleGameOver() {
    _isGameOver = true;

    //  停止所有定时器（停止阳光产生、僵尸生成等）
    this->unscheduleAllCallbacks();
    
    //  暂停所有子节点的动作（僵尸停止移动，植物停止攻击）
    for (auto child : this->getChildren()) {
        child->pause();
    }

    //  弹出失败 UI 层
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    // 遮罩层
    auto layer = LayerColor::create(Color4B(0, 0, 0, 150));
    this->addChild(layer, 500);

    // 失败提示字样
    auto label = Label::createWithTTF("Zombies Ate Your Brains!", "fonts/arial.ttf", 48);
    label->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2 + 50));
    label->setColor(Color3B::RED);
    layer->addChild(label);

    // 停止背景音乐
    cocos2d::experimental::AudioEngine::stopAll(); // 停掉之前的


    // 重试按钮
    auto retryBtn = ui::Button::create("smallButton.png", "smallButtonFade.png","smallButton.png");
    retryBtn->setTitleText("Retry");
    retryBtn->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2 - 50));
    retryBtn->addClickEventListener([](Ref* sender) {
        // 重新加载当前场景
        Director::getInstance()->replaceScene(Level1Scene::createScene());
    });
    layer->addChild(retryBtn);
}