#ifndef __ZOMBIE_H__
#define __ZOMBIE_H__

#include "cocos2d.h"
//向前声明

class Plant;
class Level1Scene;
class Level2Scene;

// 僵尸基类
class Zombie : public cocos2d::Sprite {
public:
    // 保留原有接口：默认创建普通僵尸
    static Zombie* createZombie(int row);

    cocos2d::Rect getLogicHitbox() ; //由于当前情况下由于每个素材可能出现大小不同的情况通过缩小hitbox避免隔空攻击

    virtual void update(float dt) override;
    void takeDamage(float damage);

    int getRow() const { return _row; };

    bool isTargetPlantValid();

protected:
    // 新增：供派生类调用的通用初始化接口
    bool initWithProperties(int row, int hp, float speed, int damage, const std::string& texture);

    int _hp;
    int _row;
    float _speed;
    int _attackDamage;
    Plant* _targetPlant;

    void checkCollision();
};

// --- 派生类声明 ---

class NormalZombie : public Zombie {
public:
    static NormalZombie* create(int row);
};

class ConeheadZombie : public Zombie {
public:
    static ConeheadZombie* create(int row);
};

class BucketheadZombie : public Zombie {
public:
    static BucketheadZombie* create(int row);
};

#endif