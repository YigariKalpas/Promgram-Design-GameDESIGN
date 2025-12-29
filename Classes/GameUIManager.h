#ifndef __GAME_UI_MANAGER_H__
#define __GAME_UI_MANAGER_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Plant.h"
#include "board.h"
#include <map>
#include <vector>
#include <string>

class GameUIManager : public cocos2d::Layer {
public:
    // 静态创建方法，必须传入场景和网格引用
    static GameUIManager* create(cocos2d::Scene* scene, board* gameBoard);

    // 初始化函数
    virtual bool init(cocos2d::Scene* scene, board* gameBoard);

    // 每帧调用：根据阳光更新卡片状态
    void updateCardStates(float dt);

private:
    // --- UI 构建 ---
    void createSeedBank();    // 创建卡片栏
    void createPauseMenu();   // 创建暂停按钮

    // --- 事件监听 ---
    void setupTouchListener(); // 设置全局触摸监听（用于种植）

    // --- 交互回调 ---
    // 点击卡片的回调
    void onCardClicked(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type, PlantType pType);
    // 点击暂停的回调
    void onPauseClicked(cocos2d::Ref* sender);
    // 处理屏幕点击进行种植
    void handlePlantingAt(cocos2d::Vec2 screenPos);

    // --- 辅助函数 ---
    // 根据植物类型获取图标路径
    std::string getIconPathByType(PlantType type);
    std::string getPicPathByType(PlantType type);

    // --- 成员变量 ---
    cocos2d::Scene* _parentScene;      // 持有父场景引用
    board* _gameBoard;               // 持有网格系统引用

    bool _isPlantingMode;              // 是否处于种植预览模式
    PlantType _selectedPlantType;      // 当前选中的植物类型
    cocos2d::Sprite* _previewShadow;   // 跟随鼠标的半透明预览图

    // 数据配置
    std::map<PlantType, int> _priceMap; // 植物价格表
    // 按钮与植物类型的映射，用于遍历更新状态
    std::map<cocos2d::ui::Button*, PlantType> _buttonToTypeMap;
};

#endif // __GAME_UI_MANAGER_H__