#ifndef __PLANT_H__
#define __PLANT_H__

#include "cocos2d.h"
#include "board.h"
#include"SunManager.h" //太阳花要用
#include "Zombie.h" //攻击逻辑要用
#include "SimpleAudioEngine.h" // 音效管理

using namespace CocosDenshion; //声音管理

// 植物类型枚举
enum class PlantType {
    PEASHOOTER,
    SUNFLOWER,
    WALLNUT,
    CABBAGE,
    DOOMSHROOM,
    POTATOMINE
};

class Plant : public cocos2d::Sprite {
public:
    // 基础属性
    struct PlantStats {
        int hp;
        int cost;
        float attackInterval;
        PlantType type;
    } targetStats;

    board *_gameBoard = nullptr;
    int _gridRow; // 逻辑行
    int _gridCol; // 逻辑列

    /**
     * 设置植物在网格中的逻辑位置
     * @param row 行索引 (0-4)
     * @param col 列索引 (0-8)
     */
    void setGridPos(int row, int col);

    cocos2d::Rect getLogicHitbox();

    // 获取逻辑坐标的接口（僵尸检测逻辑会用到） 
    //预留作为虚函数是由于可能会在后续增加玉米加农炮等非单格作物
    virtual int getGridRow() const { return _gridRow; }
    
    void setBoard(board * b){ _gameBoard = b; } // 注入接口; // 注入接口
    virtual int getGridCol() const { return _gridCol; }

    static Plant* createWithType(PlantType type);

    virtual bool init() override;
    virtual void update(float dt) override; // 每一帧的逻辑处理

    virtual void takeDamage(float damage);

    // 核心虚函数：子类需重写
    virtual void doAbility() = 0;          // 执行植物特殊能力（射击、产阳光、爆炸等）
    virtual void onDamaged(int damage);    // 被僵尸啃咬时触发

    // 常用属性获取
    int getHp() const { return _stats.hp; }
    PlantType getType() const { return _stats.type; }
    //进行是否前方有僵尸的判断
    void startAttack();
    //开火
    void fire();

protected:
    PlantStats _stats;
    float _timer = 0.0f;       // 通用化的计时器可以用于处理攻击间隔、阳光生产间隔以及土豆雷是否准备的逻辑功能
    bool _isReady = false;     // 植物是否初始化完成并开始工作
    // extra属性
    bool _isPrimed = false;     // 是否准备就绪（用于土豆雷）
    float _preparationTime = 0.0f; // 准备所需时间

    // 攻击属性
    int _damage;                // 攻击力
    float _attackRange;         // 攻击/爆炸半径（用于毁灭菇）
};

///以下将基于植物类对六种不同的实体植物对象进行派生以及重写其doability函数

// --- 豌豆射手 ---
class Peashooter : public Plant {
public:
    static Peashooter* create();
    virtual bool init() override;
    // 重写 Plant 的能力执行方法
    virtual void doAbility() override;
};

// --- 向日葵 ---
class Sunflower : public Plant {
public:
    static Sunflower* create();
    virtual bool init() override;
    // 重写 Plant 的能力执行方法
    virtual void doAbility() override;
};

//坚果
class WallNut : public Plant {
public:
    static WallNut* create();
    virtual bool init() override;
    void takeDamage(float damage);
    virtual void doAbility() override { /* 坚果墙被动阻挡，不需要此功能 */ }
};

//包心菜 (Cabbage) ---
class Cabbage : public Plant {
public:
    static Cabbage* create();
    virtual bool init() override;

    /**
     * 重写能力：投掷卷心菜
     * 逻辑：检测同行僵尸并抛射高伤害子弹
     */
    virtual void doAbility() override;
};
// --- 土豆雷 (PotatoMine) ---
class PotatoMine : public Plant {
public:
    static PotatoMine* create();
    virtual bool init() override;

    /**
     * 重写每一帧逻辑：
     * 1. 计时处理准备期（_isPrimed 状态切换）
     * 2. 准备好后进行脚下范围检测
     */
    virtual void update(float dt) override;

    /**
     * 重写能力：触发爆炸
     * 逻辑：对脚下极小范围内的僵尸造成致死伤害并自毁
     */
    virtual void doAbility() override;

protected:
    // _isPrimed 和 _preparationTime 已在基类中定义
};

// --- 毁灭菇 (DoomShroom) ---
class DoomShroom : public Plant {
public:
    static DoomShroom* create();
    virtual bool init() override;

    /**
     * 重写能力：毁灭性爆炸
     * 逻辑：对超大半径（_attackRange）内的僵尸造成极高伤害并自毁
     */
    virtual void doAbility() override;
};
#endif