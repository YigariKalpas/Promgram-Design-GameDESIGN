// SunManager.h
#ifndef __SUN_MANAGER_H__
#define __SUN_MANAGER_H__

#include "cocos2d.h"
#include <vector>

class SunManager : public cocos2d::Node
{
public:
    // 单例模式获取实例
    static SunManager* getInstance();

    // 初始化：关联场景并创建UI
    void reset();
    void init(cocos2d::Scene* gameScene);

    // 控制天空阳光生成的开关
    void startGeneratingSuns();
    void stopGeneratingSuns();

    // 阳光操作核心接口
    void addSun(int amount);       // 增加阳光
    bool spendSun(int amount);    // 消耗阳光（种植时调用）
    int getSunCount() const { return _sunCount; }

    // 两种生成方式供自然生成以及植物生成
    void generateRandomSun();                          // 天空自然掉落
    void generateSunAtPosition(const cocos2d::Vec2& position); // 植物（如向日葵）产出

    // 每一帧逻辑更新
    virtual void update(float delta) override;

private:
    SunManager();
    ~SunManager();
    static SunManager* _instance;

    // 内部私有方法
    void createSunUI();            // 创建左上角阳光计数框
    void updateSunDisplay();       // 更新数字Label
    void collectSun(cocos2d::Sprite* sun); // 收集阳光的动作执行
    cocos2d::Sprite* createSunSprite(const cocos2d::Vec2& position);

    // 成员变量
    cocos2d::Scene* _gameScene;
    cocos2d::Label* _sunCountLabel;
    cocos2d::Vector<cocos2d::Sprite*> _activeSuns; // 追踪当前场上存活的阳光

    int _sunCount;              // 当前拥有阳光总数
    int _sunValue;              // 单个阳光的面值 (25)
    float _generationInterval;  // 天空生成间隔
    float _timeSinceLastSun;    // 计时器
    float _sunLifetime;         // 阳光未被收集的消失时间
    bool _isActive;             // 是否开启天空掉落
};

#endif