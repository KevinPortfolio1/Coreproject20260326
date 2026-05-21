#include "mainwindow.h"
#include "hud_renderer.h"
#include "cat_renderer.h"
#include "cyber_formatter.h"
#include "mood_analyzer.h"
#include <QPainter>
#include <QDateTime>
#include <QRegularExpression>
#include <cmath>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("NEWS_CAT_OS v3.1 [Frame-Locked Anti-Glitch Engine]");
    resize(900, 550);

    // 初始化核心網絡與解碼組件
    netManager = new QNetworkAccessManager(this);
    rssFetcher = new RssFetcher(netManager, this);
    ollamaClient = new OllamaClient(netManager, this);

    // 🔗 脈衝資料流連線：抓取新聞事件
    connect(rssFetcher, &RssFetcher::newsReady, this, [this](const QString &title) {
        coreState.currentNews = title;
        logMessage("NEWS", coreState.currentNews);

        coreState.statusMsg = "DECODING";
        coreState.currentMood = Mood::ANALYZING;
        coreState.decodeProgress = 0.0f;
        ollamaTimer.start();

        logMessage("OLLAMA", "隨機觸發 32~64 Token 深網解碼核心...");
        ollamaClient->sendRequest(coreState.currentNews);
    });

    // 🔗 網絡中斷攔截
    connect(rssFetcher, &RssFetcher::fetchFailed, this, [this]() {
        logMessage("ERROR", "網絡同步中斷。");
        coreState.remainingSeconds = 15;
        coreState.statusMsg = "NET_FAULT";
        update();
    });

    // 🔗 AI 數據解碼完成連線
    connect(ollamaClient, &OllamaClient::responseArrival, this, &MainWindow::handleOllamaArrival);

    // 🔗 AI 客戶端故障連線
    connect(ollamaClient, &OllamaClient::requestFailed, this, [this]() {
        logMessage("ERROR", "Ollama 終端連線失敗。");
        coreState.remainingSeconds = 10;
        coreState.statusMsg = "STANDBY";
        update();
    });

    // 啟動主系統時脈
    frameTimer.start();
    logMessage("STATUS", "NEWS_CAT_OS 動態反饋神經元就緒。");

    // 🚀 骨骼動畫與進度更新時脈計時器 (~30FPS)
    QTimer *animTimer = new QTimer(this);
    connect(animTimer, &QTimer::timeout, this, [this]() {
        float dt = frameTimer.restart() / 1000.0f;

        // 💡 【超載防護鎖】如果單幀卡頓超過 0.1 秒，強制壓回標準的 0.033 秒
        // 徹底阻斷因為後台 Ollama 結束瞬間造成前台貓咪「瞬間大跳躍/頓挫」的視覺干擾
        if (dt > 0.1f) {
            dt = 0.033f;
        }

        animeStep += dt * 2.2f;

        // 處理解碼面板的虛擬百分比
        if (coreState.statusMsg == "DECODING") {
            if (coreState.decodeProgress < 85.0f) coreState.decodeProgress += dt * 22.0f;
            else if (coreState.decodeProgress < 98.0f) coreState.decodeProgress += dt * 0.5f;
        } else {
            coreState.decodeProgress = 0.0f;
        }

        // 動態控制機械貓的晃動速度與振幅
        float speedMult = 1.0f;
        if (coreState.currentMood == Mood::ANGRY) {
            speedMult = 4.5f;
            coreState.moodEffect = std::sin(animeStep * 12.0f) * 3.0f;
        } else if (coreState.currentMood == Mood::SAD) {
            speedMult = 0.35f;
            coreState.moodEffect = 0;
        } else {
            speedMult = 1.0f;
            coreState.moodEffect = std::cos(animeStep * 0.6f) * 25.0f;
        }

        coreState.bodyBounce = std::sin(animeStep * speedMult) * 8.0f;
        update();
    });
    animTimer->start(33);

    // ⏱️ 全域脈衝倒數計時器 (每秒執行一次)
    QTimer *secTimer = new QTimer(this);
    connect(secTimer, &QTimer::timeout, this, [this]() {
        if (coreState.statusMsg != "DECODING" && coreState.statusMsg != "SYNC_NET") {
            if (coreState.remainingSeconds > 0) {
                coreState.remainingSeconds--;
            } else {
                coreState.statusMsg = "SYNC_NET";
                logMessage("STATUS", "重新同步脈衝資料...");
                rssFetcher->fetch();
            }
        }
    });
    secTimer->start(1000);
}

void MainWindow::handleOllamaArrival(const QString &rawResponse) {
    // ⏱️ 計算本次精準解碼耗時
    double elapsedSec = ollamaTimer.elapsed() / 1000.0;
    coreState.lastDecodeTimeStr = QString("%1s").arg(elapsedSec, 0, 'f', 2);

    // 📈 刷新與記錄最高/最低歷史硬體極限
    if (coreState.minDecodeTimeStr == "--" || elapsedSec < coreState.minDecodeTimeStr.left(coreState.minDecodeTimeStr.length() - 1).toDouble()) {
        coreState.minDecodeTimeStr = QString("%1s").arg(elapsedSec, 0, 'f', 2);
    }
    if (coreState.maxDecodeTimeStr == "--" || elapsedSec > coreState.maxDecodeTimeStr.left(coreState.maxDecodeTimeStr.length() - 1).toDouble()) {
        coreState.maxDecodeTimeStr = QString("%1s").arg(elapsedSec, 0, 'f', 2);
    }

    QString rawContent = rawResponse;

    // 1. 英文字母殘留的安全防護標記
    static const QRegularExpression englishWordRegex("[a-zA-Z]{2,}");
    if (rawContent.contains(englishWordRegex)) {
        rawContent.remove(englishWordRegex);
        rawContent = "【⚠️網絡干擾】" + rawContent;
    }

    // 2. 情緒判定分析
    coreState.currentMood = MoodAnalyzer::analyze(rawContent);

    // 🔥 【生理同步】如果本機 Ollama 生成超過 12 秒導致硬體發燙，貓咪強制進入狂暴暴怒狀態！
    if (elapsedSec >= 12.0) {
        coreState.currentMood = Mood::ANGRY;
    }

    // 3. 通過 CyberFormatter 進行黑牆終端清洗
    coreState.catComment = CyberFormatter::cleanNoiseAndCode(rawContent).trimmed();

    // 4. 根據最終情緒，由主界面統一附加貓尾綴
    if (coreState.currentMood == Mood::ANGRY) {
        coreState.catComment += "..喵個屁！";
    } else if (coreState.currentMood == Mood::SAD) {
        coreState.catComment += "..喵嗚。";
    } else {
        coreState.catComment += "..喵。";
    }

    // 🎯 完美恢復：分立式傳統日誌排版，每一行皆擁有精確的獨立時間戳標籤
    logMessage("PERFORMANCE", QString("Ollama 解碼耗時: %1").arg(coreState.lastDecodeTimeStr));
    logMessage("AI_RAW"     , rawResponse);
    logMessage("AI_REMARK"  , coreState.catComment);

    coreState.statusMsg = "READY";
    coreState.remainingSeconds = 35;

    // 切換完畢瞬間重置時間戳，清空解碼期間累積的延遲
    frameTimer.restart();
    update();
}

void MainWindow::logMessage(const QString &category, const QString &msg) {
    qDebug() << QString("[%1] %2: %3")
                    .arg(QDateTime::currentDateTime().toString("HH:mm:ss"),
                         category.leftJustified(11),
                         msg);
}

void MainWindow::updateBackgroundCache() {
    bgCache = QPixmap(size());
    bgCache.fill(CyberPalette::DARK_BG);
    QPainter p(&bgCache);
    p.setPen(QColor(0, 255, 255, 20));
    for(int i = 0; i < width(); i += 40) p.drawLine(i, 0, i, height());
    for(int i = 0; i < height(); i += 40) p.drawLine(0, i, width(), i);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    updateBackgroundCache();
    QMainWindow::resizeEvent(event);
}

void MainWindow::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.drawPixmap(0, 0, bgCache); // 繪製高效快取背景

    CoreState renderState = this->coreState;
    renderState.currentNews = CyberFormatter::cleanNoiseAndCode(this->coreState.currentNews);

    // 呼叫 HUD 與機械貓渲染流水線
    HudRenderer::drawHUD(p, width(), renderState, cyberPalette);
    HudRenderer::drawNewsStream(p, width(), renderState, cyberPalette);
    HudRenderer::drawChatBubble(p, width(), height(), renderState, cyberPalette);
    CatRenderer::drawCyberCat(p, renderState, cyberPalette);
}
