// SunManager.cpp
#include "SunManager.h"
#include "audio/include/SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

SunManager* SunManager::_instance = nullptr;

SunManager* SunManager::getInstance() {
    if (!_instance) {
        _instance = new (std::nothrow) SunManager();
    }
    return _instance;
}

SunManager::SunManager() :
    _gameScene(nullptr), _sunCountLabel(nullptr),
    _sunCount(50), _sunValue(25),
    _generationInterval(10.0f), _timeSinceLastSun(0.0f),
    _sunLifetime(15.0f), _isActive(false)
{
}

SunManager::~SunManager() {}

void SunManager::reset() {
    _sunCount = 150; // 重置初始阳光
    this->unscheduleAllCallbacks(); // 停止所有阳光产生调度
}

void SunManager::init(Scene* gameScene) {
    reset();
    _gameScene = gameScene;
    _gameScene->addChild(this); // 将管理器节点加入场景以启用 update
    createSunUI();
    this->scheduleUpdate();
}

void SunManager::createSunUI() {
    if (!_gameScene) return;
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 1. 阳光计数板，用一个太阳简单替代(sun2.png)
    auto board = Sprite::create("sun2.png",Rect(0,0,130,140));
    board->setPosition(Vec2(150, visibleSize.height - 60));
    _gameScene->addChild(board, 100);

    // 2. 阳光数值文本
    _sunCountLabel = Label::createWithTTF(std::to_string(_sunCount), "fonts/Marker Felt.ttf", 32);
    _sunCountLabel->setPosition(board->getContentSize().width / 2 + 0, board->getContentSize().height / 2);
    _sunCountLabel->setColor(Color3B::BLACK);
    board->addChild(_sunCountLabel);
}

void SunManager::startGeneratingSuns() { _isActive = true; }
void SunManager::stopGeneratingSuns() { _isActive = false; }

void SunManager::update(float delta) {
    if (!_isActive) return;

    _timeSinceLastSun += delta;
    if (_timeSinceLastSun >= _generationInterval) {
        generateRandomSun();
        _timeSinceLastSun = 0;
    }
}

// 方式一：天空随机生成
void SunManager::generateRandomSun() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float startX = CCRANDOM_0_1() * (visibleSize.width * 0.7f) + (visibleSize.width * 0.15f);
    float startY = visibleSize.height + 50;
    float targetY = visibleSize.height * 0.2f + CCRANDOM_0_1() * (visibleSize.height * 0.5f);

    auto sun = createSunSprite(Vec2(startX, startY));
    sun->runAction(MoveTo::create(4.0f, Vec2(startX, targetY)));
}

// 方式二：植物生产生成（如向日葵）
void SunManager::generateSunAtPosition(const Vec2& position) {
    auto sun = Sprite::create("sun2.png",Rect(0,0,130,140)); // 确保资源名一致
    if (!sun) return;

    sun->setPosition(position);
    _gameScene->addChild(sun, 1000); // 阳光层级最高

    // 给向日葵产出的阳光添加一个微小的弹出动画（视觉更自然）
    auto jump = JumpBy::create(0.5f, Vec2(RandomHelper::random_int(-20, 20), 0), 30, 1);
    sun->runAction(jump);

    // 绑定触摸事件（复用你现有的阳光点击回调）
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this, sun](Touch* touch, Event* event) {
        if (sun->getBoundingBox().containsPoint(touch->getLocation())) {
            this->collectSun(sun); // 调用统一的收集逻辑：加分、移动到计数器、销毁
            return true;
        }
        return false;
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, sun);
}

Sprite* SunManager::createSunSprite(const Vec2& position) {
    auto sun = Sprite::create("sun2.png",Rect(0,0,130,140));
    sun->setPosition(position);
    sun->setScale(0.8f);
    _gameScene->addChild(sun, 200);

    // 绑定触摸监听
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [this, sun](Touch* touch, Event* event) {
        Vec2 pos = sun->getParent()->convertToNodeSpace(touch->getLocation());
        if (sun->getBoundingBox().containsPoint(pos)) {
            this->collectSun(sun);
            return true;
        }
        return false;
        };
    _gameScene->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, sun);

    _activeSuns.pushBack(sun);

    // 自动过期消失逻辑
    sun->runAction(Sequence::create(
        DelayTime::create(_sunLifetime),
        FadeOut::create(1.0f),
        CallFunc::create([this, sun]() {
            _activeSuns.eraseObject(sun);
            sun->removeFromParent();
            }), nullptr
    ));

    return sun;
}

void SunManager::collectSun(Sprite* sun) {
    // 移除监听避免重复点击
    _gameScene->getEventDispatcher()->removeEventListenersForTarget(sun);
    _activeSuns.eraseObject(sun);

    // 播放音效
    SimpleAudioEngine::getInstance()->playEffect("audio/points.mp3");

    // 飞向UI位置的动画 (假设UI坐标为 150, height-60)
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 targetPos = Vec2(150, visibleSize.height - 60);

    sun->runAction(Sequence::create(
        Spawn::create(MoveTo::create(0.6f, targetPos), ScaleTo::create(0.6f, 0.4f), nullptr),
        CallFunc::create([this, sun]() {
            this->addSun(_sunValue);
            sun->removeFromParent();
            }), nullptr
    ));
}

void SunManager::addSun(int amount) {
    _sunCount += amount;
    updateSunDisplay();
}

bool SunManager::spendSun(int amount) {
    if (_sunCount >= amount) {
        _sunCount -= amount;
        updateSunDisplay();
        return true;
    }
    return false;
}

void SunManager::updateSunDisplay() {
    if (_sunCountLabel) {
        _sunCountLabel->setString(std::to_string(_sunCount));
        // 简单缩放动画，仅做表现
        _sunCountLabel->runAction(Sequence::create(ScaleTo::create(0.1f, 1.2f), ScaleTo::create(0.1f, 1.0f), nullptr));
    }
}