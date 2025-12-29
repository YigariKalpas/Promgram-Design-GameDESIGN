#ifndef __BULLET_H__
#define __BULLET_H__
#include "cocos2d.h"

class Bullet : public cocos2d::Sprite {
public:
    static Bullet* create(const std::string& filename, int damage, int row);
    void update(float dt) override;
    int getDamage() const { return _damage; }
    int getRow() const { return _row; }

private:
    int _damage;
    int _row;
    float _speed = 400.0f;
};
#endif