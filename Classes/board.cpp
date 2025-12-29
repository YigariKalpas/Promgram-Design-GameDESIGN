#include "board.h"
#include <vector>

using namespace cocos2d;
using namespace ui;


const int board::columnNumber = 9;
const int board::rollNumber = 5;

bool board::init() {
    try {
        for (int i = 0; i < rollNumber; i++) {
            std::vector<cocos2d::Sprite*> temp;
            for (int j = 0; j < columnNumber; j++) {
                temp.push_back(nullptr);
            }
            // 注意这里修正了变量名拼写：boardRecord
            boardRecord.push_back(temp);
        }
    }
    catch (const std::exception& e) {
        CCLOG(e.what());
        return false;
    }
    return true;
}

// ... isCorrect 和 changeToResolution 保持原样 ...
bool board::isCorrect(const int& column, const int& roll) {
    if (column < 0 || column >= columnNumber) return false;
    if (roll < 0 || roll >= rollNumber) return false;
    return true;
}

void board::changeToResolution(int& resolutionX, int& resolutionY, const int& column, const int& roll) {
    if (!isCorrect(column, roll)) {
        CCLOGERROR("changeToResolution illegal");
    }
    else {
        resolutionX = blockOriginX + column * blockSizeX;
        resolutionY = blockOriginY - 0 + roll * blockSizeY;
    }
}

// 3. 【新增】实现 convertToGrid
// 作用：将屏幕点击的像素坐标 (x, y) 转换为网格索引 (col, row)
Vec2 board::convertToGrid(float x, float y) {
    // 逆向计算： (像素 - 起点) / 格子大小
    int col = std::floor((x - blockOriginX) / (float)blockSizeX);
    int row = std::floor((y - blockOriginY) / (float)blockSizeY);
    CCLOG("Touch Pixel: %.1f, %.1f | Calc Grid: %d, %d", x, y, col, row); //调试用
    return Vec2(col, row);
}

// 4. 【修改】实现 addNewPlant (去掉引用符 &，修正变量名)
bool board::addNewPlant(const int& column, const int& roll, Sprite* target) {
    if (!isCorrect(column, roll))
        return false;
    else {
        if (boardRecord[roll][column] != nullptr) {
            CCLOG("There is already a plant");
            return false;
        }
        else {
            boardRecord[roll][column] = target;
            return true;
        }
    }
}

// 5. 【新增】实现 hasPlant (GameUIManager 需要调用)
bool board::hasPlant(const int& column, const int& roll) {
    if (!isCorrect(column, roll)) return false;
    return boardRecord[roll][column] != nullptr;
}

bool board::deletePlant(const int& column, const int& roll) {
    if (!isCorrect(column, roll)) return false;
    boardRecord[roll][column] = nullptr; // 简单置空，具体内存管理由 Scene 负责移除
    return true;
}