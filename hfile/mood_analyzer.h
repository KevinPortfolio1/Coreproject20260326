#ifndef MOOD_ANALYZER_H
#define MOOD_ANALYZER_H

#include "core_state.h"

class MoodAnalyzer {
public:
    static Mood analyze(const QString &rawContent);
};

#endif // MOOD_ANALYZER_H
