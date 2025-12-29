#ifndef __LEVEL1_SCENE_H__
#define __LEVEL1_SCENE_H__

#include "cocos2d.h"
#include "board.h"
#include "Plant.h"
#include "Bullet.h"
#include "Zombie.h"
#include "SunManager.h"
#include "GameUIManager.h"
#include "GameScene.h" //为了重返关卡选择模块

class Level1Scene : public cocos2d::Scene {
public:
    const static std::string BACKGROUNDMUSIC;
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(Level1Scene);

    virtual void onEnter() override;

    virtual void update(float delta) override;

    bool _isGameOver = false; // 标记游戏是否结束
    void handleGameOver();

    void handleWin();

    //用于在场景中生成对应的僵尸
    void Level1Scene::spawnZombie();
    void onZombieKilled(); //用于反馈僵尸被击败

private:
    board* _gameBoard;
    GameUIManager* _uiManager;
    //  僵尸计数器相关 
    int _zombiesSpawnedCounter = 0;   // 已生成的僵尸数量
    int _zombiesKilledCounter = 0; //已经击败的僵尸数量
    const int _maxZombiesInLevel = 15; // 关卡总僵尸数（例如设定为15只）
    bool _isLevelFinished = false;    // 标记是否已经生成完所有僵尸
};

#endif