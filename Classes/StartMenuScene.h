#ifndef __START_MENU_SCENE_H__
#define __START_MENU_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class StartMenuScene : public cocos2d::Scene
{
public:
    // 静态创建方法
    static cocos2d::Scene* createScene();
    

    // init方法
    virtual bool init() override;

    // 添加场景生命周期方法
    virtual void onEnter() override;
    virtual void onExit() override;

    // 背景音乐控制方法
    void playBackgroundMusic();
    void stopBackgroundMusic();

    // 资源路径常量
    static const std::string BACKGROUND_IMAGE;
    static const std::string LOGO_IMAGE_0;
    static const std::string LOGO_IMAGE_1;
    static const std::string START_BUTTON_NORMAL;
    static const std::string START_BUTTON_PRESSED;
    static const std::string EXIT_BUTTON_NORMAL;
    static const std::string EXIT_BUTTON_PRESSED;
    static const std::string MENU_BACKGROUND;
    static const std::string MENU_BACKGROUND_MUSIC;  // 音乐文件常量

    // 回调函数
    void menuStartCallback(cocos2d::Ref* pSender);
    void menuExitCallback(cocos2d::Ref* pSender);

    // 创建UI组件
    void createLogoAnimation();
    void createMenuButtons();
    void createBackground();

    // CREATE_FUNC宏
    CREATE_FUNC(StartMenuScene);
};

#endif // __START_MENU_SCENE_H__