#include <QtScript/QScriptEngine>
#include <QtScript/QScriptValue>
#include <QtScript/QScriptValueIterator>
#include "httpget.h"

HttpGet::HttpGet(QObject *parent)
: QObject(parent) {
    http = new QHttp(this);
    buffer = NULL;
    connect(http, SIGNAL(done(bool)), this, SLOT(httpDone(bool)));
    connect(http, SIGNAL(requestFinished(int, bool)),
            this, SLOT(httpRequestFinished(int, bool)));
    connect(http, SIGNAL(dataReadProgress(int, int)),
            this, SLOT(updateDataReadProgress(int, int)));
    connect(http, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)),
            this, SLOT(readResponseHeader(const QHttpResponseHeader &)));
}

HttpGet::~HttpGet() {
}

bool HttpGet::doHttpRequest(QUrl url, QString method, QString data, int type) {

    if (buffer != NULL) {
        delete buffer;
        buffer = NULL;
    }
    buffer = new QBuffer();
    requesttype = type;

    http->setHost(url.host(), url.port() != -1 ? url.port() : 80);
    if (!url.userName().isEmpty())
        http->setUser(url.userName(), url.password());

    QHttpRequestHeader header(method, url.path(), 1, 1); /* header */
    header.setValue("Host", url.host());
    //header.setValue("Connection", "keep-alive");
    header.setValue("User-Agent", "Danny");
    header.setValue("Content-length", QString::number(data.length()));
    httpGetId = http->request(header, data.toUtf8(), buffer);

    //httpGetId = http->get(url.path(), buffer);
    httpRequestAborted = false;
    return true;
}

void HttpGet::httpRequestFinished(int requestId, bool error) {

    if (requestId != httpGetId)
        return;

    if (error) {
        qDebug() << tr("Error: %1")
                .arg(http->errorString());
    } else {
        qDebug() << tr("HTTP Request is finished");
    }
}

void HttpGet::readResponseHeader(const QHttpResponseHeader &responseHeader) {
    qDebug() << responseHeader.statusCode();
    if (responseHeader.statusCode() != 200) {
        qDebug() << tr("HTTP Request is failed")
                .arg(responseHeader.reasonPhrase());
        httpRequestAborted = true;
        http->abort();
        return;
    }
}

void HttpGet::updateDataReadProgress(int bytesRead, int totalBytes) {
    if (httpRequestAborted)
        return;
    //qDebug() << tr("Has downloaded: %1%").arg(100 * qRound(bytesRead / totalBytes));
    qDebug() << tr("bytesRead: %1, totalBytes: %2").arg(bytesRead).arg(totalBytes);
}

void HttpGet::httpDone(bool error) {
    if (error) {
        qDebug() << tr("Error:") << qPrintable(http->errorString()) << endl;
    }
    // It can emit signal to those who have slots to listen to it.
    emit done(buffer->buffer(), requesttype);
}
