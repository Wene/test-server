#include "chatserver.h"

//Konstruktoren
ChatServer::ChatServer(QObject *parent) :
    QObject(parent)
{
    ConnectionList = new QList<ChatConnection*>();
    setPort(2342);
    bActive = false;
}

ChatServer::ChatServer(qint16 port, QObject *parent) :
    QObject(parent)
{
    ConnectionList = new QList<ChatConnection*>();
    setPort(port);
    bActive = false;
}

//setter and getter
void ChatServer::setPort(qint16 port)
{
    if(port < 1024)
        port = 1024;
    iPort = port;
}

bool ChatServer::isListening()
{
    return bActive;
}

//public slots
void ChatServer::startServer()
{
    if(!bActive)
    {
        Server = new QTcpServer(this);
        if(Server->listen(QHostAddress::AnyIPv6, iPort))
        {
            connect(Server, SIGNAL(newConnection()), this, SLOT(handleNewConnection()));
            bActive = true;
            QString sMessage = tr("Server gestartet, höre auf Port %0.").arg(iPort);
            emit newLog(sMessage);
        }
    }
}

void ChatServer::stopServer()
{
    if(bActive)
    {
        for(int i = 0; i < ConnectionList->count(); i++)
        {
            Connection = ConnectionList->at(i);
            Connection->CloseAndDelete();
        }
        ConnectionList->clear();
        Server->close();
        Server->deleteLater();
        QString sMessage = tr("Server und alle Verbindungen gestoppt.");
        emit newLog(sMessage);
        bActive = false;
    }
}

//private slots
void ChatServer::handleNewConnection()
{
    QTcpSocket *Socket = Server->nextPendingConnection();
    Connection = new ChatConnection(Socket, this);          //Damit wird der Client auch gleich aufgefordert einen Nick anzugeben
    ConnectionList->append(Connection);
    connect(Connection, SIGNAL(newLog(QString)), this, SLOT(forwardLog(QString)));
    connect(Connection, SIGNAL(newMessage(QString,QString)), this, SLOT(handleMessage(QString,QString)));
    connect(Connection, SIGNAL(aboutToClose(ChatConnection*)), this, SLOT(closeConnection(ChatConnection*)));
}

void ChatServer::closeConnection(ChatConnection *conn)
{
    Connection = conn;
    ConnectionList->removeAll(Connection);
    QString sMessage = tr("Verbindung zu %0 wurde vom Client abgebrochen.")
            .arg(Connection->nick());
    emit newLog(sMessage);
    Connection->CloseAndDelete();
}

void ChatServer::forwardLog(QString sMessage)
{
    emit newLog(sMessage);
}

void ChatServer::handleMessage(QString sNick, QString sText)
{
    QString sMessage = tr("Neue Nachricht von %0: %1")
            .arg(sNick)
            .arg(sText);
    emit newLog(sMessage);
    for(int i = 0; i < ConnectionList->count(); i++)
    {
        Connection = ConnectionList->at(i);
        if(sNick != Connection->nick())
        {
            sMessage = tr("%0> %1")
                    .arg(sNick)
                    .arg(sText);
            Connection->sendMessage(sMessage);
        }
    }
}
