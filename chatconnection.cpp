#include "chatconnection.h"

//Constructors
ChatConnection::ChatConnection(QObject *parent) :
    QObject(parent)
{
    IsConnected = false;
}

ChatConnection::ChatConnection(QTcpSocket *s, QObject *parent) :
    QObject(parent)
{
    setSocket(s);
}

//getters and setters
QString ChatConnection::nick()
{
    return sNick;
}

//slots
void ChatConnection::CloseAndDelete()
{
    QString sMessage = tr("Bye Bye %0\n").arg(sNick);
    Socket->write(sMessage.toUtf8());
    Socket->flush();
    Socket->disconnectFromHost();
    Socket->deleteLater();
    sMessage = tr("Verbindung zu %0 geschlossen.").arg(sNick);
    emit newLog(sMessage);
    this->deleteLater();
}


void ChatConnection::processData()
{
    if(IsConnected)
    {
        QByteArray sData = Socket->readAll();
        QString sMessage = QString::fromUtf8(sData);
        sMessage.remove("\r");
        sMessage.remove("\n");
        if(!hasNick)
        {
            sNick = sMessage;
            hasNick = true;
            sMessage = tr("Neuer Nick gesetzt: \"%0\"").arg(sNick);
            emit newLog(sMessage);
            sMessage = tr("Willkommen %0\n").arg(sNick);
            Socket->write(sMessage.toUtf8());
        }
        else
        {
            emit newMessage(sNick, sMessage);
        }
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
    sNick = "Hugo";
    QString sMessage = tr("Neue Verbindung hergestellt. Temporärer Nick: \"%0\"").arg(sNick);
    emit newLog(sMessage);
    sMessage.append("\n");
    Socket->write(sMessage.toUtf8());
    sMessage = tr("Gib bitte einen neuen Nick ein: ");
    Socket->write(sMessage.toUtf8());
    hasNick = false;
    connect(Socket, SIGNAL(readyRead()), this, SLOT(processData()));
    connect(Socket, SIGNAL(aboutToClose()), this, SLOT(sendCloseRequest()));
}
