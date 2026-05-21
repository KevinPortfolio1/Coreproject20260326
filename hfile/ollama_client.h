#pragma once

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>

class QNetworkReply;

class OllamaClient : public QObject {
    Q_OBJECT

public:
    explicit OllamaClient(QNetworkAccessManager *manager, QObject *parent = nullptr);
    virtual ~OllamaClient() = default;

    // 主叫函式：內部自動進行 32~64 Token 隨機分配與調控
    void sendRequest(const QString &title);

signals:
    // 當後端數據清洗、翻譯、重構完畢後發射此訊號
    void responseArrival(const QString &content);
    // 網路請求失敗（如 Ollama 未啟動）時發射
    void requestFailed();

private:
    void onReplyArrival(QNetworkReply *reply, const QString &originalTitle);
    QString filterAndTransform(const QString &raw, const QString &title);

    QNetworkAccessManager *netManager{nullptr};
};
