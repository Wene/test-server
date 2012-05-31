#include "chatconnection.h"

ChatConnection::ChatConnection(QObject *parent) :
    QObject(parent)
{
    IsConnected = false;
}

ChatConnection::ChatConnection(QTcpSocket *s, QObject *parent) :
    QObject(parent)
{
    Socket = s;
    IsConnected = true;
    AskForNick();
    connect(Socket, SIGNAL(aboutToClose()), this, SLOT(sendCloseRequest()));
}

QString ChatConnection::nick()
{
    return sNick;
}

void ChatConnection::CloseAndDelete()
{
    QString sMessage = tr("Bye Bye %0").arg(sNick);
    Socket->write(sMessage.toUtf8());
    Socket->flush();
    Socket->close();
    Socket->deleteLater();
    sMessage = tr("Verbindung zu %0 geschlossen.").arg(sNick);
    emit newLog(sMessage);
    this->deleteLater();
}

void ChatConnection::AskForNick()
{
    if(IsConnected)
    {
        QString sMessage = tr("Gib bitte einen Nick ein: ");
        Socket->write(sMessage.toUtf8());
        connect(Socket, SIGNAL(readyRead()), this, SLOT(processNick()));
    }
}

void ChatConnection::processNick()
{
    if(IsConnected)
    {
        disconnect(Socket, SIGNAL(readyRead()), this, SLOT(AskForNick()));
        QByteArray sData = Socket->readAll();
        sNick.fromUtf8(sData);
        QString sMessage = tr("Hallo %0, willkommen im Chat.\n").arg(sNick);
        Socket->write(sMessage.toUtf8());
        sMessage = tr("Neuer Benutzer angemeldet: %0.").arg(sNick);
        emit newLog(sMessage);
        connect(Socket, SIGNAL(readyRead()), this, SLOT(processData()));
    }
}

void ChatConnection::processData()
{
    if(IsConnected)
    {
        QByteArray sData = Socket->readAll();
        QString sMessage;
        sMessage.fromUtf8(sData);
        emit newMessage(sNick, sMessage);
    }
}

void ChatConnection::sendCloseRequest()
{
    emit aboutToClose(this);
}

void ChatConnection::setSocket(QTcpSocket *s)
{
    Socket = s;
    IsConnected = true;
    connect(Socket, SIGNAL(aboutToClose()), this, SLOT(sendCloseRequest()));
}
