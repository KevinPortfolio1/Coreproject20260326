#include "rss_fetcher.h"
#include <QNetworkReply>
#include <QXmlStreamReader>
#include <QDateTime>
#include <QRandomGenerator>

RssFetcher::RssFetcher(QNetworkAccessManager *manager, QObject *parent)
    : QObject(parent), netManager(manager) {}

void RssFetcher::fetch() {
    QUrl url("https://news.google.com/rss?hl=zh-TW&gl=TW&ceid=TW:zh-Hant&nocache=" + QString::number(QDateTime::currentMSecsSinceEpoch()));
    QNetworkRequest request(url);
    QNetworkReply *reply = netManager->get(request);
    connect(reply, &QNetworkReply::finished, this, &RssFetcher::onReplyArrival);
}

void RssFetcher::onReplyArrival() {
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    if (reply->error() == QNetworkReply::NoError) {
        QXmlStreamReader xml(reply->readAll());
        QStringList freshTitles;

        while (!xml.atEnd() && !xml.hasError()) {
            if (xml.readNext() == QXmlStreamReader::StartElement && xml.name() == QLatin1String("item")) {
                while (xml.readNextStartElement()) {
                    if (xml.name() == QLatin1String("title")) {
                        QString t = xml.readElementText().section(QLatin1String(" - "), 0, 0).trimmed();
                        if (!t.isEmpty() && !processedNewsHistory.contains(t)) {
                            freshTitles.append(t);
                        }
                        break;
                    } else {
                        xml.skipCurrentElement();
                    }
                }
            }
        }

        if (!freshTitles.isEmpty()) {
            QString selectedNews = freshTitles.at(QRandomGenerator::global()->bounded(freshTitles.size()));
            processedNewsHistory.append(selectedNews);
            if (processedNewsHistory.size() > MAX_HISTORY_SIZE) {
                processedNewsHistory.removeFirst();
            }
            emit newsReady(selectedNews);
        } else {
            if (!processedNewsHistory.isEmpty()) {
                emit newsReady(processedNewsHistory.at(QRandomGenerator::global()->bounded(processedNewsHistory.size())));
            } else {
                emit fetchFailed();
            }
        }
    } else {
        emit fetchFailed();
    }
    reply->deleteLater();
}
