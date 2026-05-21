#ifndef CYBER_PALETTE_H
#define CYBER_PALETTE_H

#include <QFont>
#include <QPen>
#include <QBrush>
#include <QColor>

struct CyberPalette {
    // 1. 霓虹基本色矩陣 (加上 static constexpr，確保全域唯一且不重複定義)
    static constexpr QColor DARK_BG{10, 10, 15};
    static constexpr QColor NEON_CYAN{0, 255, 255};
    static constexpr QColor NEON_PINK{255, 0, 255};
    static constexpr QColor NEON_YELLOW{255, 255, 0};

    // 2. 核心相容性顏色別名
    static constexpr QColor GLITCH_RED{255, 0, 85};
    static constexpr QColor WARN_YELLOW{255, 255, 0};
    static constexpr QColor TEXT_WHITE{240, 244, 250};
    static constexpr QColor MATRIX_GREEN{0, 255, 136};

    // 3. 建構子：在物件建立時安全初始化字型與畫筆
    CyberPalette()
        : statusFont("Monospace", 10, QFont::Bold)
        , newsFont("Noto Sans CJK TC", 10, QFont::Bold)
        , chatFont("Noto Sans CJK TC", 13, QFont::Bold)
        , cyberCyanPen(NEON_CYAN, 2)
        , cyberYellowPen(NEON_YELLOW, 2)
        , cyberPinkPen(NEON_PINK, 2)
        , neonYellowBrush(NEON_YELLOW)
        , transparentDarkBrush(QColor(0, 0, 0, 230))
    {}

    // Lubuntu 專屬加粗字型
    QFont statusFont;
    QFont newsFont;
    QFont chatFont;

    // 畫筆與筆刷
    QPen cyberCyanPen;
    QPen cyberYellowPen;
    QPen cyberPinkPen;
    QBrush neonYellowBrush;
    QBrush transparentDarkBrush;
};

#endif // CYBER_PALETTE_H
