#include "Bullet.h"
USING_NS_CC;

Bullet* Bullet::create(const std::string& filename, int damage, int row) {
    auto b = new (std::nothrow) Bullet();
    if (b && b->initWithFile(filename)) {
        b->autorelease();
        b->_damage = damage;
        b->_row = row;
        b->scheduleUpdate();
        return b;
    }
    CC_SAFE_DELETE(b);
    return nullptr;
}

void Bullet::update(float dt) {
    this->setPositionX(this->getPositionX() + _speed * dt);
    // ³ö½çÏú»Ù
    if (this->getPositionX() > Director::getInstance()->getVisibleSize().width) {
        this->removeFromParent();
    }
}