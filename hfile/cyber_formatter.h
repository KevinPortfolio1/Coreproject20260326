#ifndef CYBER_FORMATTER_H
#define CYBER_FORMATTER_H

#include <QString>

class CyberFormatter {
public:
    static QString cleanNoiseAndCode(const QString &raw);
};

#endif // CYBER_FORMATTER_H
