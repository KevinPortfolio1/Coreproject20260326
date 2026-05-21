#ifndef CORE_STATE_H
#define CORE_STATE_H

#include <QString>

// 補齊所有情緒狀態，確保渲染器、分析器、主視窗完美接軌
enum class Mood { SASS, ANGRY, SAD, ANALYZING };

struct CoreState {
    QString statusMsg = "STANDBY";
    Mood currentMood = Mood::SASS;
    int remainingSeconds = 10;

    QString currentNews = "等待深網脈衝...";
    QString catComment = "機械貓特工核心已離線載入，黑牆級 NLP 無損熔煉流水線就緒...";

    QString lastDecodeTimeStr = "0.00s";
    QString minDecodeTimeStr = "--";
    QString maxDecodeTimeStr = "--";
    float decodeProgress = 0.0f;

    float bodyBounce = 0.0f;
    float moodEffect = 0.0f;

    QString getMoodChinese() const {
        switch(currentMood) {
        case Mood::SASS:      return "全開吐槽";
        case Mood::ANGRY:     return "核心暴怒";
        case Mood::SAD:       return "無聊想睡";
        case Mood::ANALYZING: return "數據攔截";
        default:              return "追蹤";
        }
    }
};

#endif // CORE_STATE_H
