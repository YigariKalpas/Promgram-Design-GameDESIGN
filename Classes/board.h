#ifndef __BOARD_H__
#define __BOARD_H__

#include "cocos2d.h"
#include <vector>

class board {
public:
    static constexpr int blockOriginX = 260;
    static constexpr int blockOriginY = 205;
    static constexpr int blockSizeX = 80;
    static constexpr int blockSizeY = 100;
    static const int columnNumber;
    static const int rollNumber;

    bool init();

    // 1. 【新增】声明坐标转换函数
    static cocos2d::Vec2 convertToGrid(float x, float y);

    static bool isCorrect(const int& column, const int& roll);
    static void changeToResolution(int& resolutionX, int& resolutionY, const int& column = 0, const int& roll = 0);

    bool addNewPlant(const int& column, const int& roll, cocos2d::Sprite* target);

    bool deletePlant(const int& column, const int& roll);

    // 获取某个位置是否有植物 (GameUIManager 需要用到)
    bool hasPlant(const int& column, const int& roll);

private:
    // 注意：原代码中有拼写错误 boardRecrod -> boardRecord，建议修正
    std::vector<std::vector<cocos2d::Sprite*>> boardRecord;
};

#endif // __BOARD_H__