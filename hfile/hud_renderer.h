#ifndef HUD_RENDERER_H
#define HUD_RENDERER_H

#include <QPainter>
#include "cyber_palette.h"
#include "core_state.h"

class HudRenderer {
public:
    /**
     * @brief 頂部系統狀態欄繪製（新增歷史極限耗時渲染）
     */
    static void drawHUD(QPainter &p, int width, const CoreState &state, const CyberPalette &palette);

    /**
     * @brief 中間 RSS 新聞流資料矩陣繪製
     */
    static void drawNewsStream(QPainter &p, int width, const CoreState &state, const CyberPalette &palette);

    /**
     * @brief 右側機械貓 AI 吐槽對話框與解碼進度條繪製
     */
    static void drawChatBubble(QPainter &p, int width, int height, const CoreState &state, const CyberPalette &palette);
};

#endif // HUD_RENDERER_H
