# 🐾 CyberCat-Agent-HUD (賽博貓特工核心解碼系統)

一個基於Qt (C++17)框架開發的賽博朋克（Cyberpunk）風格桌面終端系統
本系統採用數據驅動與非同步管線設計，能即時攔截網頁 RSS 新聞流，將其送入本地大語言模型（Ollama/LLM）進行無損熔煉，並透過黑牆級 NLP 文本清洗與情緒分析，最終由一隻擁有動態骨骼（正弦波驅動）的「機械貓特工」進行全開吐槽與全像 HUD 介面呈現。

---

## 🛠️ 系統架構與模組職責

本專案嚴格遵循狀態與渲染分離的設計原則，所有視覺渲染器皆為無狀態（Stateless）的靜態類別，藉此達成高內聚、低耦合的硬體架構。

### 1. 核心狀態與樣式 (Model)
CoreState(核心狀態矩陣)：全域執行期狀態機，控管解碼進度（`decodeProgress`）、動態物理步進（`bodyBounce`）、當前情緒（`Mood`）及歷史解碼耗時。
CyberPalette(霓虹調色盤)：定義專案專屬的賽博色彩矩陣（霓虹青、螢光粉、矩陣綠、警示黃），並預載入適用於 Lubuntu 系統環境的粗體字型與發光畫筆（QPen）。

### 2. 網路與 AI 動態流水線 (Controller)
RssFetcher(新聞攔截器)：利用 `QNetworkAccessManager` 非同步抓取網路新聞，自動建立去重歷史佇列（`MAX_HISTORY_SIZE = 50`），防止重複數據流入。
OllamaClient(AI 轉譯核心)：對接本地大模型。內部實作動態 Prompt 工程，並動態調控 `num_predict`（32~64 Token），確保貓咪吐槽精簡毒舌。
CyberFormatter(文本清洗器)：過濾 AI 響應中殘留的 `<think>` 思考標籤、Markdown 語法及雜訊，抽離純淨文本。
MoodAnalyzer(情緒語意分析器)：輕量級 NLP 引擎。透過關鍵字權衡機制，即時將文本映射至 `SASS`（吐槽）、`ANGRY`（暴怒）、`SAD`（無聊想睡）與 `ANALYZING`（數據攔截）四種核心情緒。

### 3. 全像視覺渲染層 (View)
HudRenderer (全像 HUD 渲染器)：負責頂部系統狀態欄、中間 RSS 數據方格矩陣、歷史極限耗時（Min/Max/Last）以及右側對話氣泡與解碼進度條的純代碼繪製。
CatRenderer (機械貓外觀渲染器)：使用 `QPainter` 進行純數學幾何向量繪製。將貓體分解為外殼、內耳、霓虹眼、動態鬍鬚與賽博面具，並完美耦合 `bodyBounce` 物理步進。

---

## 🔄 核心數據流向圖 (Data Pipeline)

=========================================【 CYBER_DATA_PIPELINE 】=========================================

 [ 主時脈核心 ] ──( 15秒定時脈衝 )──► [ RssFetcher 新聞攔截器 ]
                                               │
                                       ( QNetworkReply 非同步 XML )
                                               │
                                               ▼
 [ 數據重映射字典 ]                      [ newsReady(QString) 訊號 ]
        │                                      │
  ( 賽博字詞替換 )                             ▼
        ▼                               ┌──────────────┐
 [ CyberFormatter ] ◄──( 文本清洗 ) ◄── │  MainWindow  │ ──( 攔截數據啟動計時 )
        │                               └──────────────┘
        ▼                                      │
 [ MoodAnalyzer   ]                            │ ( 非同步 POST 封包 )
        │                                      ▼
  ( 四大情緒權衡 )                       [ OllamaClient ] ──( 算力調配 32~64 Tokens )
        │                                      │
  ( Mood 映射變更 )                            ▼
        │                              [ responseArrival(QString) 訊號 ]
        │                                      │
        └─────────────────► [ 狀態機變更 ] ◄───┘
                                   │
                           ( update() 觸發重繪 )
                                   │
                                   ▼
                        ┌──────────────────────┐
                        │  MainWindow::paint   │
                        └──────────────────────┘
                           /                \
          ( 傳遞全域狀態 CoreState )       ( 傳遞全域狀態 CoreState )
                         /                    \
                        ▼                      ▼
               [ HudRenderer ]            [ CatRenderer ]
             ( 終端矩陣、極限耗時 )      ( 正弦波骨骼、掃描眼 )

===========================================================================================================
## 🚀 執行環境與編譯指南

### 前提條件
作業系統：Linux (建議 Lubuntu 輕量級環境) 或 Windows / macOS
編譯器：支援 C++17 或以上之編譯器 (GCC / Clang / MSVC)
框架：Qt 5.12+ 或 Qt 6.x (需內含 `Network` 與 `Widgets` 模組)
本地 AI 後端：需在背景執行 `Ollama` 並部署對應之大語言模型

