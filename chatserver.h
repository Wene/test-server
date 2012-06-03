#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QObject>
#include <QtNetwork>
#include "chatconnection.h"

class ChatServer : public QObject
{
    Q_OBJECT
public:
    explicit ChatServer(QObject *parent = 0);
    ChatServer(qint16 port, QObject *parent = 0);
    void setPort(qint16 port);
    bool isListening();

private:
    ChatConnection *Connection;
    QList<ChatConnection*> *ConnectionList;
    QTcpServer *Server;
    qint16 iPort;
    bool bActive;

private slots:
    void handleNewConnection();
    void forwardLog(QString);
    void closeConnection(ChatConnection*);
    void handleMessage(QString, QString);

signals:
    void newLog(QString);
    void newMessage(QString, QString);

public slots:
    void startServer();
    void stopServer();

};

#endif // CHATSERVER_H
