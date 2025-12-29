#include "GameUIManager.h"
#include "SunManager.h"

USING_NS_CC;
using namespace cocos2d::ui;

GameUIManager* GameUIManager::create(Scene* scene, board* gameBoard) {
    auto ret = new (std::nothrow) GameUIManager();
    if (ret && ret->init(scene, gameBoard)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool GameUIManager::init(Scene* scene, board* gameBoard) {
    if (!Layer::init()) return false;

    // 参数校验
    if (!scene || !gameBoard) {
        return false;
    }

    _parentScene = scene;
    _gameBoard = gameBoard;
    _isPlantingMode = false;
    _previewShadow = nullptr;

    // 1. 初始化植物价格配置
    _priceMap[PlantType::PEASHOOTER] = 75;
    _priceMap[PlantType::SUNFLOWER] = 50;
    _priceMap[PlantType::WALLNUT] = 50;
    _priceMap[PlantType::CABBAGE] = 100;
    _priceMap[PlantType::DOOMSHROOM] = 125;
    _priceMap[PlantType::POTATOMINE] = 25;

    // 2. 初始化UI组件
    createSeedBank();
    createPauseMenu();
    setupTouchListener();

    // 3. 开启调度器，每0.2秒检查一次阳光余额以更新卡片状态
    this->schedule(CC_SCHEDULE_SELECTOR(GameUIManager::updateCardStates), 0.2f);

    return true;
}

// 辅助函数,根据类型返回资源名
std::string GameUIManager::getIconPathByType(PlantType type) {
    // 请确保 Resources 目录下有对应的 png 文件
    switch (type) {
    case PlantType::PEASHOOTER: return "Peashooter.webp";
    case PlantType::SUNFLOWER:  return "SunFlower.webp";
    case PlantType::WALLNUT:    return "BigWallNut.webp";
    case PlantType::CABBAGE:    return "Cabbage.webp";
    case PlantType::DOOMSHROOM: return "DoomShroom.webp";
    case PlantType::POTATOMINE: return "PotatoMine.webp";
    default: return "icon_default.png";
    }
}

// 辅助函数：根据类型返回资源名
std::string GameUIManager::getPicPathByType(PlantType type) {
    // 请确保 Resources 目录下有对应的 png 文件
    switch (type) {
    case PlantType::PEASHOOTER: return "Peashooter.png";
    case PlantType::SUNFLOWER:  return "SunFlower.png";
    case PlantType::WALLNUT:    return "WallNut.png";
    case PlantType::CABBAGE:    return "Cabbage.png";
    case PlantType::DOOMSHROOM: return "DoomShroom.png";
    case PlantType::POTATOMINE: return "PotatoMineRD.png";
    default: return "icon_default.png";
    }
}

void GameUIManager::createSeedBank() {
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // --- 资源预留：卡片槽背景 (SeedBank_BG.png) ---
    auto bankBg = Sprite::create("SeedBank_BG.png");
    if (!bankBg) {
        // 无资源时的备用色块
        bankBg = (Sprite*)LayerColor::create(Color4B(0, 0, 0, 100), 550, 90);
    }
    // 设置在顶部居中偏右的位置，避开左上角的阳光栏
    bankBg->setScale(1.2f);
    bankBg->setAnchorPoint(Vec2(0.5f, 1.0f));
    bankBg->setPosition(Vec2(visibleSize.width / 2 + 80, visibleSize.height - 10));
    _parentScene->addChild(bankBg, 50);

    // 定义卡片顺序
    std::vector<PlantType> plantList = {
        PlantType::SUNFLOWER,
        PlantType::PEASHOOTER,
        PlantType::WALLNUT,
        PlantType::CABBAGE,
        PlantType::POTATOMINE,
        PlantType::DOOMSHROOM
    };

    float startX = 50.0f;     // 起始X坐标
    float spacing = 80.0f;    // 间距

    for (auto type : plantList) {
        // 如果没有图片，则替代为纯色图（当前github所呈现的文件不完整，需要后续重新提交）
        auto btn = Button::create("smallButton.png", "smallButtonFade.png", "smallButton.png");

        // 如果加载失败，创建一个简单的文字按钮作为备用
        if (!btn->getRendererNormal()) {
            btn = Button::create();
            btn->setTitleText("Plant");
            btn->setTitleFontSize(14);
        }

        btn->setPosition(Vec2(startX - 250, bankBg->getContentSize().height / 2 - 150));

        // 添加植物图标,为了简化处理，全都以亮的卡片显示
        auto icon = Sprite::create(getIconPathByType(type),Rect(0,0,215,258/2));
        if (icon) {
            icon->setPosition(btn->getContentSize() / 2);
            icon->setScale(0.6f); // 适当缩小以放入卡片
            btn->addChild(icon);
        }

        // 添加价格标签
        auto priceLabel = Label::createWithTTF(std::to_string(_priceMap[type]), "fonts/Marker Felt.ttf", 16);
        priceLabel->setPosition(Vec2(btn->getContentSize().width / 2, 10)); // 放在底部
        priceLabel->setColor(Color3B::YELLOW);
        priceLabel->enableOutline(Color4B::BLACK, 1);
        btn->addChild(priceLabel);

        // 绑定回调事件
        btn->addTouchEventListener(CC_CALLBACK_2(GameUIManager::onCardClicked, this, type));

        // 记录按钮与类型的映射关系
        _buttonToTypeMap[btn] = type;

        bankBg->addChild(btn);
        startX += spacing;
    }
}
// 修改后的 updateCardStates 实现
void GameUIManager::updateCardStates(float dt) {

    int currentSun = SunManager::getInstance()->getSunCount();

    for (const auto& pair : _buttonToTypeMap) {
        auto btn = pair.first;       // 获取按钮指针 (Key)
        PlantType type = pair.second;// 获取植物类型 (Value)

        //  检查价格
        if (_priceMap.find(type) != _priceMap.end()) {
            int cost = _priceMap[type];

            if (currentSun < cost) {
                // 阳光不足：禁用并变灰
                btn->setEnabled(false);
                btn->setColor(Color3B(100, 100, 100));
            }
            else {
                // 阳光充足：启用并恢复白色
                btn->setEnabled(true);
                btn->setColor(Color3B::WHITE);
            }
        }
    }
}

void GameUIManager::onCardClicked(Ref* sender, Widget::TouchEventType type, PlantType pType) {
    if (type == Widget::TouchEventType::ENDED) {
        // ... 阳光检查 ...

        _isPlantingMode = true;
        _selectedPlantType = pType;

        if (_previewShadow) {
            _previewShadow->removeFromParent();
        }

        // 方案：直接通过图片路径创建简单的 Sprite，而不是复杂的 Plant 类
        std::string path = getPicPathByType(pType);
        if (pType == PlantType::POTATOMINE) {
            _previewShadow = Sprite::create(path,Rect(0, 140, 130, 110));
        }
        else {
            _previewShadow = Sprite::create(path);
        }
        if (_previewShadow) {
            _previewShadow->setOpacity(128);
            _previewShadow->setColor(Color3B::GRAY);
            _parentScene->addChild(_previewShadow, 999);
            _previewShadow->setPosition(Vec2(-100, -100));
        }
    }
}

void GameUIManager::setupTouchListener() {
    auto listener = EventListenerTouchOneByOne::create();
    // 不吞噬触摸，允许点击穿透到下面的按钮（虽然这层主要是给种植用的...）因为出现了重叠问题
    listener->setSwallowTouches(false);

    //  触摸开始：仅当处于种植模式时处理
    listener->onTouchBegan = [this](Touch* t, Event* e) {
        return _isPlantingMode;
        };

    // 触摸移动：更新预览图位置
    listener->onTouchMoved = [this](Touch* t, Event* e) {
        if (_isPlantingMode && _previewShadow) {
            Vec2 touchPos = _parentScene->convertToNodeSpace(t->getLocation());
            _previewShadow->setPosition(touchPos);

            // 可选：在这里调用 board::convertToGrid 和 changeToResolution 
            // 也可以让预览图实时吸附到格子上，增强手感未来可以通过board实现
        }
        };

    // 3. 触摸结束：尝试种植
    listener->onTouchEnded = [this](Touch* t, Event* e) {
        if (_isPlantingMode) {
            handlePlantingAt(t->getLocation());
        }
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

/// <summary>
/// 仍然存在映射问题：种植时的指针位置与真实的网格位置相差了y轴的100左右
/// </summary>
/// <param name="screenPos"></param>
void GameUIManager::handlePlantingAt(Vec2 screenPos) {
    // 将屏幕坐标转换为场景（世界）坐标
    Vec2 scenePos = _parentScene->convertToNodeSpace(screenPos);

    // 计算点击所在的网格索引
    int col = (scenePos.x - board::blockOriginX) / board::blockSizeX;
    int row = (scenePos.y + 200 - board::blockOriginY) / board::blockSizeY;

    //  严谨校验：位置合法且该格没有植物
    if (_gameBoard->isCorrect(col, row) && !_gameBoard->hasPlant(col, row)) {

        int cost = _priceMap[_selectedPlantType]; // 获取选中植物的价格

        // 3. 检查并扣除阳光
        if (SunManager::getInstance()->spendSun(cost)) {

            // 4. 创建正式的植物实例
            Plant* newPlant = Plant::createWithType(_selectedPlantType);

            if (newPlant) {
                // 先注入所有必要的数据指针
                newPlant->setGridPos(row, col);

                _gameBoard->addNewPlant(col, row, static_cast<Sprite*>(newPlant));

                // 获取格子的对齐坐标
                int resX, resY;
                _gameBoard->changeToResolution(resX, resY, col, row);

                // 设置位置（加上 40 偏移量使植物居中）
                newPlant->setPosition(Vec2(resX + 40, resY + 40));
                newPlant->setGridPos(row, col); // 设置行号用于 ZOrder 排序

                // 5. 添加到场景
                _parentScene->addChild(newPlant, 100 - row);

                // 6. 将植物存入 board 的内存管理中用于后续检测
                _gameBoard->addNewPlant(col, row, static_cast<Sprite*>(newPlant));

                newPlant->setBoard(this->_gameBoard); // 将 UI 管理器持有的 board 传给植物

                CCLOG("Planting Success! PlantType: %d, Grid: [%d, %d]", _selectedPlantType, col, row);
            }
        }
        else {
            CCLOG("Not enough sun!");
        }
    }

    _isPlantingMode = false;
    if (_previewShadow) {
        _previewShadow->removeFromParent();
        _previewShadow = nullptr;
    }
}

void GameUIManager::createPauseMenu() {
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // --- 资源预留：暂停按钮 (Pause_Btn.png) ---
    auto pauseBtn = Button::create("Pause_Btn.png");
    if (!pauseBtn->getRendererNormal()) {
        pauseBtn = Button::create(); // 备用
        pauseBtn->setTitleText("||");
        pauseBtn->setTitleFontSize(30);
        pauseBtn->setTitleColor(Color3B::WHITE);
    }

    // 放在右上角
    pauseBtn->setPosition(Vec2(visibleSize.width - 50, visibleSize.height - 50));
    pauseBtn->addClickEventListener(CC_CALLBACK_1(GameUIManager::onPauseClicked, this));

    _parentScene->addChild(pauseBtn, 100);
}

void GameUIManager::onPauseClicked(Ref* sender) {
    // 简单的暂停逻辑
    if (Director::getInstance()->isPaused()) {
        Director::getInstance()->resume();
    }
    else {
        Director::getInstance()->pause();

        // 可以在这里添加一个全屏遮罩层作为暂停菜单
        // ... (此处省略复杂的暂停菜单创建代码，仅做基本暂停)
    }
}