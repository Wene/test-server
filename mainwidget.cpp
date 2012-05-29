#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowTitle(trUtf8("Test Server App"));
    this->resize(600,250);
    VLayout = new QVBoxLayout(this);
    Display = new QTextEdit();
    Display->setReadOnly(true);
    Display->setFontFamily("courier");
    Display->setFontPointSize(10);
    VLayout->addWidget(Display);
    HLayout = new QHBoxLayout();
    VLayout->addLayout(HLayout);
    BtnStart = new QPushButton(trUtf8("Server Starten"));
    BtnStop = new QPushButton(trUtf8("Server Stoppen"));
    BtnQuit = new QPushButton(trUtf8("Beenden"));
    HLayout->addWidget(BtnStart);
    HLayout->addWidget(BtnStop);
    HLayout->addStretch();
    HLayout->addWidget(BtnQuit);

    connect(BtnQuit, SIGNAL(clicked()), this, SLOT(close()));
    connect(BtnStart, SIGNAL(clicked()), this, SLOT(ServerStart()));
    connect(BtnStop, SIGNAL(clicked()), this, SLOT(ServerStop()));

    SocketList = new QList<QTcpSocket*>();
    ServerIsActive = false;

    ShowMessage(trUtf8("Programm gestartet, Server läuft nicht."));
    ShowMessage(trUtf8("Klicke auf Start um den Server zu starten."));
}

MainWidget::~MainWidget()
{

}


void MainWidget::ServerStart()
{
    if(ServerIsActive)
    {
        ShowMessage(trUtf8("Server läuft bereits."));
    }
    else
    {
        Server = new QTcpServer();
        ServerIsActive = true;
        if(Server->listen(QHostAddress::AnyIPv6,7512))
        {
            connect(Server, SIGNAL(newConnection()), this, SLOT(HandleNewConnection()));
            ShowMessage(trUtf8("Server gestartet"));
        }
        else
        {
            QString sError = Server->errorString();
            QString sMessage;
            if(sError.isEmpty())
            {
                sMessage = trUtf8("Unbekannter Fehler beim Starten - ");
                QAbstractSocket::SocketError error = Server->serverError();

                //error handling - incomplete
                switch(error)
                {
                case QAbstractSocket::ConnectionRefusedError: sMessage.append(
                                trUtf8("The connection was refused by the peer (or timed out)."));
                    break;
                case QAbstractSocket::RemoteHostClosedError: sMessage.append(
                                trUtf8("The remote host closed the connection. Note that the client socket (i.e., this socket) will be closed after the remote close notification has been sent."));
                    break;
                case QAbstractSocket::HostNotFoundError: sMessage.append(
                                trUtf8("The host address was not found."));
                    break;
                case QAbstractSocket::SocketAccessError: sMessage.append(
                                trUtf8("The socket operation failed because the application lacked the required privileges."));
                    break;
                case QAbstractSocket::SocketResourceError: sMessage.append(
                                trUtf8("The local system ran out of resources (e.g., too many sockets)."));
                    break;
                case QAbstractSocket::UnknownSocketError: sMessage.append(
                                trUtf8("An unidentified error occurred."));
                    break;

                default:
                    sMessage.append("\n");

                }

            }
            else
            {
                sMessage = trUtf8("Fehler beim Starten: %0").arg(Server->errorString());
            }
            ShowMessage(sMessage);
        }
    }
}

void MainWidget::ServerStop()
{
    if(ServerIsActive)
    {
        QString sMessage = trUtf8("Der Server wurde beenden. Bye Bye ;-)\n");
        QByteArray sData = sMessage.toUtf8();
        for(int i = 0; i < SocketList->count(); i++)
        {
            Socket = SocketList->at(i);
            Socket->write(sData);
            Socket->flush();
            Socket->close();
            Socket->deleteLater();
        }
        SocketList->clear();
        Server->deleteLater();
        ServerIsActive = false;
        ShowMessage(trUtf8("Server gestoppt"));
    }
    else
    {
        ShowMessage(trUtf8("Server läuft nicht."));
    }
}

void MainWidget::HandleNewConnection()
{
    Socket = Server->nextPendingConnection();
    SocketList->append(Socket);
    connect(Socket, SIGNAL(readyRead()), this, SLOT(HandleNewData()));
    QString sMessage = trUtf8("Neue Verbindung Nummer %0").arg(SocketList->count());
    ShowMessage(sMessage);
    sMessage.append("\n");
    QByteArray sData = sMessage.toUtf8();
    Socket->write(sData);

}

void MainWidget::HandleNewData()
{
    QByteArray sData;
    QStringList sTextList;
    for(int i = 0; i < SocketList->count(); i++)
    {
        sData.clear();
        Socket = SocketList->at(i);
        sData = Socket->readAll();
        if(sData.size())
        {
            QString sText = trUtf8("Von Verbindung %0: %1").arg(i+1).arg(
                        QString::fromUtf8(sData));
            sTextList.append(sText);
        }
    }
    for(int i = 0; i < SocketList->count(); i++)
    {
        sData.clear();
        Socket = SocketList->at(i);
        for(int j = 0; j < sTextList.count(); j++)
        {
            sData.append(sTextList.at(j));
        }
        Socket->write(sData);
    }

}

void MainWidget::ShowMessage(QString sMessage)
{
    //Zeile anhängen
    Display->append(sMessage);

    //AutoScroll
    QTextCursor c = Display->textCursor();
    c.movePosition(QTextCursor::End);
    Display->setTextCursor(c);
}
