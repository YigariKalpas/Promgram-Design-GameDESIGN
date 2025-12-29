#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class GameScene : public cocos2d::Scene
{
public:
    // 静态创建方法
    static cocos2d::Scene* createScene();

    // init方法
    virtual bool init();

    // 场景生命周期
    virtual void onEnter();
    virtual void onExit();

    // 资源路径常量
    static const std::string BACKGROUND_IMAGE;              // 背景图片
    static const std::string CHALLENGE1_BUTTON_NORMAL;      // 关卡1按钮正常状态
    static const std::string CHALLENGE1_BUTTON_PRESSED;     // 关卡1按钮按下状态
    static const std::string CHALLENGE2_BUTTON_NORMAL;      // 关卡2按钮正常状态
    static const std::string CHALLENGE2_BUTTON_PRESSED;     // 关卡2按钮按下状态
    static const std::string END_BUTTON_NORMAL;           // 返回按钮正常状态
    static const std::string END_BUTTON_PRESSED;          // 返回按钮按下状态
    static const std::string MENU_BACKGROUND;              // 菜单背景框
    static const std::string GAME_BACKGROUND_MUSIC;        // 游戏背景音乐
    static const std::string LEVEL2_PREVIEW11111111111111111111111111111111111111111111111111111111;

    // 关卡预览图片
    static const std::string LEVEL1_PREVIEW;
    static const std::string LEVEL2_PREVIEW;

    // 回调函数
    void challenge1Callback(cocos2d::Ref* pSender);
    void challenge2Callback(cocos2d::Ref* pSender);
    void backCallback(cocos2d::Ref* pSender);

    // 创建UI组件
    void createTitle();
    void createMenuButtons();
    void createBackground();
    void createLevelPreviews();
    void createEndButtons();

    // 创建关卡信息面板
    void createLevelInfoPanel();

    // 音乐控制
    void playBackgroundMusic();
    void stopBackgroundMusic();

    // CREATE_FUNC宏
    CREATE_FUNC(GameScene);

    //更新关卡信息（在未来扩展可能会用到）
    void updateLevelInfo(int level);

private:
    // 当前选中的关卡
    int _selectedLevel;

    // 关卡信息标签
    cocos2d::Label* _levelInfoLabel;
    cocos2d::Label* _levelDifficultyLabel;
};

#endif // __GAME_SCENE_H__