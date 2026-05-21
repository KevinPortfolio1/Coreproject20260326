#include "mood_analyzer.h"

Mood MoodAnalyzer::analyze(const QString &rawContent) {
    if (rawContent.contains("警告") || rawContent.contains("過載") || rawContent.contains("核心") ||
        rawContent.contains("錯誤") || rawContent.contains("垃圾") || rawContent.contains("該死")) {
        return Mood::ANGRY;
    } else if (rawContent.contains("無聊") || rawContent.contains("不想") || rawContent.contains("無趣") ||
               rawContent.contains("懶得") || rawContent.contains("睡")) {
        return Mood::SAD;
    }
    return Mood::SASS;
}
