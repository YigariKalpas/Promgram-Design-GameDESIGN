#include "Zombie.h"
#include "Plant.h"
#include "SimpleAudioEngine.h" // 音效管理

using namespace CocosDenshion; 
USING_NS_CC;

class Level1Scene;
// 基类通用初始化,可以简化生成过程，对于当前只有生命、移速不同的僵尸来说可扩展性较好
bool Zombie::initWithProperties(int row, int hp, float speed, int damage, const std::string& texture) {
    if (!Sprite::initWithFile(texture)) return false;

    _row = row;
    _hp = hp;
    _speed = speed;
    _attackDamage = damage;
    _targetPlant = nullptr;

    this->scheduleUpdate();
    return true;
}

// 保留原有接口实现，默认返回普通僵尸
Zombie* Zombie::createZombie(int row) {
    return NormalZombie::create(row);
}

// --- 派生类具体实现 ---

NormalZombie* NormalZombie::create(int row) {
    auto z = new (std::nothrow) NormalZombie();
    // 普通僵尸：200血，20速
    if (z && z->initWithProperties(row, 200, 20.0f, 20, "Zombie.png")) {
        z->autorelease();
        return z;
    }
    CC_SAFE_DELETE(z);
    return nullptr;
}

ConeheadZombie* ConeheadZombie::create(int row) {
    auto z = new (std::nothrow) ConeheadZombie();
    // 路障僵尸：500血，20速 (防御力更高)
    if (z && z->initWithProperties(row, 500, 20.0f, 20, "ConeheadZombie.png")) {
        z->autorelease();
        return z;
    }
    CC_SAFE_DELETE(z);
    return nullptr;
}

BucketheadZombie* BucketheadZombie::create(int row) {
    auto z = new (std::nothrow) BucketheadZombie();
    // 铁桶僵尸：1100血，20速 (极高防御)
    if (z && z->initWithProperties(row, 1100, 20.0f, 20, "BucketheadZombie.png")) {
        z->autorelease();
        return z;
    }
    CC_SAFE_DELETE(z);
    return nullptr;
}

bool Zombie::isTargetPlantValid() {
    if (_targetPlant == nullptr) return false;

    // 获取僵尸所在的层（通常是 Level1Scene）
    auto parent = this->getParent();
    if (!parent) return false;

    const auto& children = parent->getChildren();
    for (const auto& child : children) {
        if (child == _targetPlant) {
            return true; // 植物依然存在
        }
    }
    return false; // 植物已被销毁
}

void Zombie::update(float dt) {
    // 1. 首先检查指针是否为空，以及该对象是否还在场景中
    if (_targetPlant && isTargetPlantValid()) {

        // 2. 此时访问是安全的
        _targetPlant->takeDamage(_attackDamage * dt);

    }
    else {
        // 4. 如果植物无效（已死亡或不存在），重置指针并恢复移动
        _targetPlant = nullptr;
        this->setPositionX(this->getPositionX() - _speed * dt);
        checkCollision();
    }
}

Rect Zombie::getLogicHitbox() {
    Rect rect = this->getBoundingBox();
    // 返回一个宽度收缩 40% 的矩形
    return Rect(rect.origin.x + rect.size.width * 0.2f, rect.origin.y,
        rect.size.width * 0.6f, rect.size.height);
}

void Zombie::checkCollision() {
    auto children = this->getParent()->getChildren();
    for (auto child : children) {
        auto plant = dynamic_cast<Plant*>(child);
        if (plant && plant->getGridRow() == this->_row) {
            // 使用矩形碰撞检测
            if (this->getLogicHitbox().intersectsRect(plant->getLogicHitbox())) {
                _targetPlant = plant;
                SimpleAudioEngine::getInstance()->playEffect("audio/Artichoke_Attack.mp3");
                break;
            }
        }
    }
}

void Zombie::takeDamage(float damage) {
    _hp -= damage;
    if (_hp <= 0) {
        // 关键：在被从父节点移除前，通知场景计数
        auto scene = dynamic_cast<Level1Scene*>(this->getScene());
        if (scene) {
            scene->onZombieKilled();
        }

        // 销毁逻辑
        this->removeFromParent();
    }
}