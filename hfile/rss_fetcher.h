#ifndef RSS_FETCHER_H
#define RSS_FETCHER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QStringList>

class RssFetcher : public QObject {
    Q_OBJECT
public:
    explicit RssFetcher(QNetworkAccessManager *manager, QObject *parent = nullptr);
    void fetch();

signals:
    void newsReady(const QString &newsTitle);
    void fetchFailed();

private slots:
    void onReplyArrival();

private:
    QNetworkAccessManager *netManager;
    QStringList processedNewsHistory;
    const int MAX_HISTORY_SIZE = 50;
};

#endif // RSS_FETCHER_H
