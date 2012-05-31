#ifndef CHATCONNECTION_H
#define CHATCONNECTION_H

#include <QObject>
#include <QtNetwork>

class ChatConnection : public QObject
{
    Q_OBJECT
public:
    explicit ChatConnection(QObject *parent = 0);
    ChatConnection(QTcpSocket *s, QObject *parent = 0);
    void setSocket(QTcpSocket *s);
    QString nick();

private:
    QString sNick;
    QTcpSocket *Socket;
    bool IsConnected;

signals:
    void newLog(QString);
    void newMessage(QString, QString);
    void aboutToClose(ChatConnection*);

public slots:
    void CloseAndDelete();
    void AskForNick();

private slots:
    void processNick();
    void processData();
    void sendCloseRequest();

};

#endif // CHATCONNECTION_H
