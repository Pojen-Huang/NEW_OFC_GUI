#ifndef HTTPGET_H
#define HTTPGET_H

#include <QObject>
#include <QtNetwork>

#define REST_TOPO_SWITCHCLUSTERS        1
#define REST_TOPO_LINKS                 2
#define REST_DEVICES                    3

class QFile;

class HttpGet : public QObject {
    Q_OBJECT

public:
    //enum HTTPRequestType {
    //    GET_SwitchesDpidList,   // /wm/core/controller/switches/json
    //    GET_PerSwitchStats,     // /wm/coreswitch/<siwtchid>/<statType>json
    //};
    
    HttpGet(QObject *parent = 0);
    ~HttpGet();
    bool doHttpRequest(QUrl url, QString method, QString data, int);
    
signals:
    void done(QByteArray&, int);
private slots:
    void httpRequestFinished(int requestId, bool error);
    void readResponseHeader(const QHttpResponseHeader &responseHeader);
    void updateDataReadProgress(int bytesRead, int totalBytes);
    void httpDone(bool error);
private:
    QHttp *http;
    QBuffer *buffer;
    int requesttype;
    int httpGetId;
    bool httpRequestAborted;
    bool httpDataDone;
    QString fileName;
};

#endif // HTTPGET_H
