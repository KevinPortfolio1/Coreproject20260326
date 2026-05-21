#include "cyber_formatter.h"
#include <QRegularExpression>
#include <QHash>
#include <QHashIterator>

QString CyberFormatter::cleanNoiseAndCode(const QString &raw) {
    QString text = raw;

    static const QRegularExpression noiseRegex(R"(\[(SASS|ANGRY|SAD|心情)\]|[\{\}\"\[\]\\]|Taiwan\d*)", QRegularExpression::CaseInsensitiveOption);
    text.remove(noiseRegex);

    static const QRegularExpression foreignScriptRegex(R"([\x{0900}-\x{097F}]+|।)", QRegularExpression::UseUnicodePropertiesOption);
    text.remove(foreignScriptRegex);

    static const QHash<QString, QString> cyberDict = {
        {"台灣", "控制網格(Taiwan-Grid)"}, {"美國", "NUSA"}, {"日本", "企業殖民地-JAPAN"},
        {"新北", "新北荒原分區"}, {"台北", "邊緣核心分區"}, {"高雄", "南部重工業港區"},
        {"國民黨", "保守派軍閥"}, {"蘇巧慧", "蘇氏改造人"}, {"李四川", "李氏企業狗"},
        {"新聞", "資料流"}, {"人類", "碳基生物"}, {"公司", "巨型企業"},
        {"瘋狂", "邏輯線路燒毀"}, {"錯誤", "邏輯溢出錯誤"}, {"政治人物", "企業傀儡代理人"}
    };

    QHashIterator<QString, QString> i(cyberDict);
    while (i.hasNext()) {
        i.next();
        text.replace(i.key(), i.value());
    }
    return text;
}
