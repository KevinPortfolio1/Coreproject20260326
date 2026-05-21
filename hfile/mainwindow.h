#pragma once

#include <QMainWindow>
#include <QTimer>
#include <QElapsedTimer>
#include <QNetworkAccessManager>
#include <QPixmap>
#include "core_state.h"
#include "rss_fetcher.h"
#include "ollama_client.h"
#include "cyber_palette.h" // 直接引入外部獨立調色盤

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow() = default;

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void handleOllamaArrival(const QString &rawResponse);

private:
    void logMessage(const QString &category, const QString &msg);
    void updateBackgroundCache();

    // 核心硬體與狀態組件
    CoreState coreState;
    CyberPalette cyberPalette;
    QPixmap bgCache;

    // 網路與 AI 動態流水線
    QNetworkAccessManager *netManager{nullptr};
    RssFetcher *rssFetcher{nullptr};
    OllamaClient *ollamaClient{nullptr};

    // 系統時脈與動態骨骼計時器
    QElapsedTimer frameTimer;
    QElapsedTimer ollamaTimer;
    float animeStep{0.0f};
};
