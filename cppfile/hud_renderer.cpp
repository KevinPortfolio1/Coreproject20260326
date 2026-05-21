#include "hud_renderer.h"
#include <QTextOption>
#include <cmath>

void HudRenderer::drawHUD(QPainter &p, int width, const CoreState &state, const CyberPalette &palette) {
    p.save();
    p.setRenderHint(QPainter::Antialiasing);

    p.fillRect(0, 0, width, 40, palette.neonYellowBrush);
    p.setPen(Qt::black);
    p.setFont(palette.statusFont);

    // 🔍 資訊矩陣重組：同步渲染本輪耗時、歷史最快(MIN)、歷史最慢(MAX)
    QString hudText = QString(">> 矩陣狀態: %1 | 脈衝: %2 (最快: %3 / 最慢: %4) | 協定: %5 | 重載倒數: T-%6s")
                          .arg(state.statusMsg)
                          .arg(state.lastDecodeTimeStr)
                          .arg(state.minDecodeTimeStr)
                          .arg(state.maxDecodeTimeStr)
                          .arg(state.getMoodChinese())
                          .arg(state.remainingSeconds);

    p.drawText(20, 26, hudText);
    p.restore();
}

void HudRenderer::drawNewsStream(QPainter &p, int width, const CoreState &state, const CyberPalette &palette) {
    p.save();
    p.setRenderHint(QPainter::Antialiasing);

    p.setPen(palette.cyberCyanPen);
    p.setBrush(Qt::NoBrush);
    p.drawRect(15, 55, width - 30, 45);

    p.setFont(palette.newsFont);
    QString fullNewsText = "SCAN_STREAM: " + state.currentNews;

    int maxTextWidth = width - 100;
    QString displayNews = p.fontMetrics().elidedText(fullNewsText, Qt::ElideRight, maxTextWidth);

    p.drawText(30, 85, displayNews);
    p.restore();
}

void HudRenderer::drawChatBubble(QPainter &p, int width, int height, const CoreState &state, const CyberPalette &palette) {
    p.save();
    p.setRenderHint(QPainter::Antialiasing);

    QPointF catBase(135, 385);
    QRectF bubble(catBase.x() + 160, catBase.y() - 115 + (state.bodyBounce * 0.4f), width - 360, 205);

    p.setPen(palette.cyberCyanPen);
    p.setBrush(palette.transparentDarkBrush);
    p.drawRect(bubble);

    QRectF textRect = bubble.adjusted(16, 16, -16, -16);
    p.setFont(palette.chatFont);

    if (state.statusMsg == "DECODING") {
        p.setPen(palette.cyberCyanPen);
        QString alertMsg = ">> [⚠️核心脈衝] 正在同步深網 1B 輕量矩陣，解析黑牆封包中...";
        p.drawText(textRect, Qt::AlignLeft | Qt::AlignTop | Qt::TextWordWrap, alertMsg);

        QRectF progBar(bubble.x() + 16, bubble.y() + bubble.height() - 40, bubble.width() - 32, 7);
        p.setPen(palette.cyberCyanPen);
        p.setBrush(Qt::NoBrush);
        p.drawRect(progBar);

        float fillWidth = (state.decodeProgress / 100.0f) * progBar.width();
        p.fillRect(QRectF(progBar.x(), progBar.y(), fillWidth, progBar.height()), palette.NEON_CYAN);

        p.setFont(palette.statusFont);
        p.setPen(palette.NEON_CYAN);
        p.drawText(progBar.x(), progBar.y() + 22, QString("DECRYPTING CORE MATRIX: %1%").arg(int(state.decodeProgress)));
    } else {
        p.setPen(palette.cyberYellowPen);
        QString drawTextBuffer = "CYBER_DECODED >> " + state.catComment;

        // 防爆框文字自動截斷與保底
        QFontMetrics fm(palette.chatFont);
        if (fm.boundingRect(textRect.toRect(), Qt::TextWordWrap, drawTextBuffer).height() > textRect.height()) {
            QString tempText = "";
            QString lastValidText = "";
            for (int i = 0; i < drawTextBuffer.length(); ++i) {
                tempText += drawTextBuffer.at(i);
                if (fm.boundingRect(textRect.toRect(), Qt::TextWordWrap, tempText + "...喵。").height() > textRect.height()) {
                    break;
                }
                lastValidText = tempText;
            }
            drawTextBuffer = lastValidText + "...喵。";
        }

        QTextOption option;
        option.setWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        option.setAlignment(Qt::AlignLeft | Qt::AlignTop);
        p.drawText(textRect, drawTextBuffer, option);
    }

    p.restore();
}
