#include "cat_renderer.h"
#include <cmath>

void CatRenderer::drawCyberCat(QPainter &p, const CoreState &state, const CyberPalette &palette) {
    p.save();
    p.setRenderHint(QPainter::Antialiasing);

    QPointF catBase(135, 385);
    p.translate(catBase.x() + state.moodEffect, catBase.y() + state.bodyBounce);

    QColor bodyColor = palette.NEON_CYAN;
    if (state.statusMsg == "DECODING") {
        bodyColor = QColor(0, 180, 180);
    } else if (state.currentMood == Mood::ANGRY) {
        bodyColor = palette.NEON_PINK; // 暴怒時，小貓全身發紅
    } else if (state.currentMood == Mood::SAD) {
        bodyColor = QColor(100, 100, 200);
    }

    p.setBrush(bodyColor.darker(150));
    p.setPen(palette.cyberYellowPen);

    float earDrip = (state.currentMood == Mood::SAD) ? 14.0f : 0.0f;
    float wiggles = (state.statusMsg == "DECODING") ? std::sin(state.decodeProgress) * 5.0f : 0.0f;

    p.drawPolygon(QPolygonF() << QPointF(-40, -40 + earDrip) << QPointF(-30 - wiggles, -75 + earDrip) << QPointF(-10, -40 + earDrip));
    p.drawPolygon(QPolygonF() << QPointF(10, -40 + earDrip) << QPointF(30 + wiggles, -75 + earDrip) << QPointF(40, -40 + earDrip));

    p.setBrush(bodyColor);
    p.drawRoundedRect(-65, -50, 130, 100, 18, 18);

    p.setBrush(palette.neonYellowBrush);
    p.setPen(Qt::NoPen);

    float eyeX = (state.statusMsg == "DECODING") ? 0.0f : (std::sin(state.bodyBounce * 0.1f) * 4.0f);
    float eyeY = (state.statusMsg == "DECODING") ? 0.0f : (std::cos(state.bodyBounce * 0.1f) * 2.0f);
    bool isBlinking = (std::fmod(state.bodyBounce, 12.0f) > 11.4f);

    if (isBlinking && state.statusMsg != "DECODING") {
        p.fillRect(-40, -10, 28, 4, palette.neonYellowBrush);
        p.fillRect(12, -10, 28, 4, palette.neonYellowBrush);
    } else {
        if (state.currentMood == Mood::ANGRY) {
            // 憤怒眼：斜線倒三角
            p.drawPolygon(QPolygonF() << QPointF(-40 + eyeX, -25 + eyeY) << QPointF(-12 + eyeX, -15 + eyeY) << QPointF(-40 + eyeX, -5 + eyeY));
            p.drawPolygon(QPolygonF() << QPointF(40 + eyeX, -25 + eyeY) << QPointF(12 + eyeX, -15 + eyeY) << QPointF(40 + eyeX, -5 + eyeY));
        } else if (state.currentMood == Mood::SAD) {
            p.setPen(QPen(palette.NEON_YELLOW, 5, Qt::SolidLine, Qt::RoundCap));
            p.drawLine(-42 + eyeX, -15, -14 + eyeX, -15);
            p.drawLine(14 + eyeX, -15, 42 + eyeX, -15);
        } else if (state.statusMsg == "DECODING") {
            float pulseRadius = 28.0f + std::sin(state.decodeProgress * 0.5f) * 2.0f;
            p.setBrush(Qt::NoBrush);
            p.setPen(QPen(palette.NEON_CYAN, 3));
            p.drawEllipse(-40, -25, pulseRadius, pulseRadius);
            p.drawEllipse(12, -25, pulseRadius, pulseRadius);

            p.setPen(QPen(palette.NEON_CYAN, 1.5));
            p.drawLine(-26, -21, -26, -1); p.drawLine(-36, -11, -16, -11);
            p.drawLine(26, -21, 26, -1); p.drawLine(16, -11, 36, -11);
        } else {
            p.drawChord(-40 + eyeX, -25 + eyeY, 28, 28, 0, 180 * 16);
            p.drawChord(12 + eyeX, -25 + eyeY, 28, 28, 0, 180 * 16);
        }
    }

    p.setPen(QPen(palette.NEON_CYAN, 1.5, Qt::SolidLine, Qt::RoundCap));
    p.drawLine(-55, -2, -85, -6 + (state.moodEffect * 0.1f));
    p.drawLine(-55, 5, -88, 5);
    p.drawLine(-55, 12, -82, 16 - (state.moodEffect * 0.1f));
    p.drawLine(55, -2, 85, -6 - (state.moodEffect * 0.1f));
    p.drawLine(55, 5, 88, 5);
    p.drawLine(55, 12, 82, 16 + (state.moodEffect * 0.1f));

    p.restore();
}
