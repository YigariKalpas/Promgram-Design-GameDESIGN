#include "Plant.h"
#include "Bullet.h"

USING_NS_CC;

Plant* Plant::createWithType(PlantType type) {
    switch (type) {
    case PlantType::PEASHOOTER:
        return Peashooter::create(); // 调用 Peashooter 自己的 create
    case PlantType::SUNFLOWER:
        return Sunflower::create();
    case PlantType::WALLNUT: 
        return WallNut::create();
    case PlantType::CABBAGE:
        return Cabbage::create();
    case PlantType::DOOMSHROOM:
        return DoomShroom::create();
    case PlantType::POTATOMINE:
        return PotatoMine::create();
        // 后续扩展增加其他植物
    default:
        return nullptr;
    }
}

void Plant::setGridPos(int row, int col) {
    _gridRow = row;
    _gridCol = col;
    // 核心逻辑：行数越大（越靠下），ZOrder越高，实现遮挡关系
    this->setLocalZOrder(100 + row);
}

void Plant::takeDamage(float damage) {
    //如果后续出现增加防御力等植物可用扩展
    //getSituation()
    //damage = reduce or exp ()
    _stats.hp -= damage;
    if (_stats.hp <= 0) {
        if (_gameBoard) {
            _gameBoard->deletePlant(_gridCol, _gridRow);
        }
        //在网格中标记失效
        this->removeFromParent();
    }
}

Rect Plant::getLogicHitbox() {
    Rect rect = this->getBoundingBox();
    // 返回一个宽度收缩 40% 的矩形
    return Rect(rect.origin.x + rect.size.width * 0.2f, rect.origin.y,
        rect.size.width * 0.3f, rect.size.height);
}

bool Plant::init() {
    if (!Sprite::init()) return false;
    // 开启每帧更新
    this->scheduleUpdate();
    return true;
}

void Plant::update(float dt) {
    if (!_isReady) return;

    _timer += dt;
    if (_timer >= _stats.attackInterval) {
        doAbility();
        _timer = 0;
    }
}

void Plant::startAttack() {
    if (targetStats.type == PlantType::PEASHOOTER) {
        // 每 1.5 秒发射一颗豌豆
        this->schedule([this](float dt) {
            // 严谨逻辑：此处应调用一个检测函数，判断该行(gridRow)是否有僵尸
            // if (ZombieManager::getInstance()->hasZombieInRow(this->_gridRow)) {
            this->fire();
            // }
            }, 1.5f, "pea_fire_key");
    }
}

// Plant.cpp
void Plant::fire() {
    auto bullet = Bullet::create("Bullet.webp", 20, _gridRow);
    if (bullet) {
        bullet->setPosition(this->getPosition() + Vec2(30, 20));
        this->getParent()->addChild(bullet, this->getLocalZOrder());
    }
}

void Plant::onDamaged(int damage) {
    _stats.hp -= damage;
    if (_stats.hp <= 0) {
        
        if (_gameBoard) {
            _gameBoard->deletePlant(_gridCol, _gridRow); //在此部分中直接删除
        }
        this->removeFromParent();
    }
}

/// <summary>
/// 派生类对象的具体功能实现
/// </summary>
/// <returns></returns>

Peashooter* Peashooter::create() {
    auto p = new (std::nothrow) Peashooter();
    if (p && p->init()) {
        p->autorelease();
        return p;
    }
    CC_SAFE_DELETE(p);
    return nullptr;
}

bool Peashooter::init() {
    if (!Plant::initWithFile("Peashooter.png")) return false;

    _stats.hp = 300;
    _stats.attackInterval = 1.5f; // 1.5秒射一次
    _stats.type = PlantType::PEASHOOTER;

    _isReady = true;
    this->scheduleUpdate(); //开启自身的 update 循环进行阳光生产
    return true;
}

void Peashooter::doAbility() {
    // 检查当前行是否有僵尸
    bool hasZombie = false;
    auto children = this->getParent()->getChildren();
    for (auto child : children) {
        auto z = dynamic_cast<Zombie*>(child);
        if (z && z->getRow() == this->_gridRow && z->getPositionX() > this->getPositionX()) {
            hasZombie = true;
            break;
        }
    }

    if (hasZombie) {
        SimpleAudioEngine::getInstance()->playEffect("audio/bowlingimpact.mp3");
        auto bullet = Bullet::create("Bullet.webp", 20, _gridRow);
        bullet->setPosition(this->getPosition() + Vec2(30, 20));
        this->getParent()->addChild(bullet, this->getLocalZOrder());
    }
}


Sunflower* Sunflower::create() {
    auto s = new (std::nothrow) Sunflower();
    if (s && s->init()) {
        s->autorelease();
        return s;
    }
    CC_SAFE_DELETE(s);
    return nullptr;
}

bool Sunflower::init() {
    if (!Plant::initWithFile("Sunflower.png")) return false;

    _stats.hp = 300;
    _stats.attackInterval = 12.0; // 大约15秒产一次阳光
    _stats.type = PlantType::SUNFLOWER;

    _isReady = true;
    this->scheduleUpdate(); //开启自身的 update 循环进行阳光生产
    return true;
}

void Sunflower::doAbility() {
    Vec2 myPos = this->getPosition();

    SunManager::getInstance()->generateSunAtPosition(myPos);

    CCLOG("Sunflower at (%f, %f) produced a sun.", myPos.x, myPos.y);
}


WallNut* WallNut::create() {
    auto pRet = new (std::nothrow) WallNut();
    if (pRet && pRet->init()) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool WallNut::init() {
    if (!Plant::init()) return false;
    this->initWithFile("WallNut.png");
    _stats.hp = 4000; // 高生命值 
    _stats.cost = 50;
    _stats.type = PlantType::WALLNUT;
    return true;
}

void WallNut::takeDamage(float damage) {
    _stats.hp -= damage * 0.6f; //假设有40％的抗性

    // 根据血量比例切换贴图（1/3 和 2/3 阈值） 
    if (_stats.hp < 2333) {
        this->setTexture("WallNut_Cracked2.png");
    }

    if (_stats.hp <= 0) {
        this->removeFromParent();
    }
}

//卷心菜具体实现
Cabbage* Cabbage::create() {
    auto pRet = new (std::nothrow) Cabbage();
    if (pRet && pRet->init()) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool Cabbage::init() {
    if (!Plant::init()) return false;
    this->initWithFile("Cabbage.png"); // 替换为实际素材路径
    _stats.hp = 300;
    _stats.cost = 100;
    _stats.attackInterval = 2.5f; // 投掷速度比豌豆略慢
    _stats.type = PlantType::CABBAGE;
    _damage = 80; // 卷心菜伤害较高 
    _isReady = true;
    this->scheduleUpdate();
    return true;
}

void Cabbage::doAbility() {
    // 逻辑：检测当前行是否有僵尸
    bool hasZombie = false;
    auto children = this->getParent()->getChildren();
    for (auto child : children) {
        auto z = dynamic_cast<Zombie*>(child);
        if (z && z->getRow() == this->_gridRow && z->getPositionX() > this->getPositionX()) {
            hasZombie = true;
            break;
        }
    }

    if (hasZombie) {
        // 创建卷心菜子弹
        auto bullet = Bullet::create("Bullet.png", _damage, _gridRow);
        if (bullet) {
            SimpleAudioEngine::getInstance()->playEffect("audio/bowlingimpact2.mp3");
            bullet->setPosition(this->getPosition() + Vec2(20, 40));
            this->getParent()->addChild(bullet, this->getLocalZOrder());
        }
    }
}

DoomShroom* DoomShroom::create() {
    auto pRet = new (std::nothrow) DoomShroom();
    if (pRet && pRet->init()) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool DoomShroom::init() {
    if (!Plant::init()) return false;
    this->initWithFile("DoomShroom.png");
    _stats.hp = 300;
    _stats.cost = 125;
    _stats.type = PlantType::DOOMSHROOM;
    _attackRange = 4000.0f; // 超大爆炸半径 
    _isReady = true; 
    /*this->scheduleOnce([this](float dt) {
        this->doAbility();
        }, 1.1f, "doom_explode_key");
    this->doAbility();*/
    return true;
}

void DoomShroom::doAbility() {
    // 遍历场景中所有僵尸
    auto children = this->getParent()->getChildren();
    Vec2 myPos = this->getPosition();

    for (auto child : children) {
        auto z = dynamic_cast<Zombie*>(child);
        if (z) {
            // 计算欧几里得距离，对应 _attackRange (400.0f)
            float dist = myPos.distance(z->getPosition());
            if (dist <= _attackRange) {
                z->takeDamage(1800); // 全屏级别伤害
            }
        }
    }

    // 创建一个巨大的蘑菇云图片
    auto explosionVisual = Sprite::create("SOS.png");
    SimpleAudioEngine::getInstance()->playEffect("audio/Boom.mp3");
    explosionVisual->setPosition(myPos);
    this->getParent()->addChild(explosionVisual, 999);
    explosionVisual->runAction(Sequence::create(FadeOut::create(0.5f), RemoveSelf::create(), nullptr));

    // 3. 地形影响：在原位留下一个“坑洞”图片，在当前实现中暂时以永久无法种植为例
    // 这可以通过创建一个不具备功能的 Sprite 实现
    auto crater = Sprite::create("SOS.png");
    crater->setPosition(myPos);
    this->getParent()->addChild(crater, this->getLocalZOrder() - 1);

    if (_gameBoard) {
        _gameBoard->deletePlant(_gridCol, _gridRow); //特殊处理因为不是由takedamage函数消除接管
    }
    // 4. 自毁
    this->removeFromParent();
}
PotatoMine* PotatoMine::create() {
    auto pRet = new (std::nothrow) PotatoMine();
    if (pRet && pRet->init()) {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}

bool PotatoMine::init() {
    if (!Plant::init()) return false;
    this->initWithFile("PotatoMineNR .png", Rect(0, 140, 130, 110)); // 初始埋在地下状态
    _stats.hp = 300;
    _stats.cost = 25;
    _stats.type = PlantType::POTATOMINE;
    _isPrimed = false; // 初始未就绪 
    _preparationTime = 15.0f; // 准备时间
    this->scheduleUpdate(); // 必须开启更新以计时
    return true;
}

void PotatoMine::update(float dt) {
    if (!_isPrimed) {
        _timer += dt;
        if (_timer >= _preparationTime) {
            _isPrimed = true;
            _isReady = true;
            this->setTexture("PotatoMineRD.png"); // 切换为出土状态 
            this->setTextureRect(Rect(0, 140, 130, 110));
        }
        return;
    }

    // 就绪后，检测是否有僵尸接触
    auto children = this->getParent()->getChildren();
    for (auto child : children) {
        auto z = dynamic_cast<Zombie*>(child);
        // 判定条件：同行且距离极近
        if (z && z->getRow() == _gridRow && abs(z->getPositionX() - this->getPositionX()) < 100.0f) {
            this->doAbility(); // 触发爆炸
            break;
        }
    }
}
void PotatoMine::doAbility() {
    // 爆炸的视觉反馈,缩放
    auto scaleExp = ScaleTo::create(0.1f, 1.5f);
    auto fadeOut = FadeOut::create(0.1f);

    // 核心伤害判定逻辑
    auto children = this->getParent()->getChildren();
    for (auto child : children) {
        auto z = dynamic_cast<Zombie*>(child);
        // 范围判定：同行且水平距离在 100 像素内
        if (z && z->getRow() == _gridRow && abs(z->getPositionX() - this->getPositionX()) < 100.0f) {
            z->takeDamage(4800); // 造成瞬杀伤害
        }
    }
    // 触发音效（如果已集成音效管理器）
    SimpleAudioEngine::getInstance()->playEffect("audio/Boom.mp3");
    if (_gameBoard) {
        _gameBoard->deletePlant(_gridCol, _gridRow);
    }
    // 4. 延迟销毁，确保动画能播放完一帧或直接移除
    this->removeFromParent();
}