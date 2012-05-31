#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    //Window Settings
    this->setWindowTitle(tr("Test Chat Server"));
    this->resize(600,250);

    //Layout
    LayMain = new QVBoxLayout(this);
    EdtDisplay = new QTextEdit();
    EdtDisplay->setReadOnly(true);
    EdtDisplay->setFontFamily("courier");
    EdtDisplay->setFontPointSize(10);
    LayMain->addWidget(EdtDisplay);
    LayConfig = new QHBoxLayout();
    LayMain->addLayout(LayConfig);
    LblPort = new QLabel(tr("Port:"));
    LayConfig->addWidget(LblPort);
    BoxPort = new QSpinBox();
    BoxPort->setMinimum(1024);
    BoxPort->setMaximum(49151);
    BoxPort->setValue(2342);
    LayConfig->addWidget(BoxPort);
    LayConfig->addStretch();
    LayButtons = new QHBoxLayout();
    LayMain->addLayout(LayButtons);
    BtnStart = new QPushButton(tr("Server Starten"));
    BtnStop = new QPushButton(tr("Server Stoppen"));
    BtnQuit = new QPushButton(tr("Beenden"));
    LayButtons->addWidget(BtnStart);
    LayButtons->addWidget(BtnStop);
    LayButtons->addStretch();
    LayButtons->addWidget(BtnQuit);

    connect(BtnQuit, SIGNAL(clicked()), this, SLOT(close()));
    connect(BtnStart, SIGNAL(clicked()), this, SLOT(ServerStart()));
    connect(BtnStop, SIGNAL(clicked()), this, SLOT(ServerStop()));

//    SocketList = new QList<QTcpSocket*>();
//    ServerIsActive = false;
    Server = new ChatServer();
    connect(Server, SIGNAL(newLog(QString)), this, SLOT(handleLog(QString)));

    handleLog(tr("Programm gestartet, Server läuft nicht."));
    handleLog(tr("Klicke auf Start um den Server zu starten."));
}

MainWidget::~MainWidget()
{

}


void MainWidget::ServerStart()
{
    if(!Server->isListening())
    {
        Server->setPort(BoxPort->value());
        Server->startServer();
    }
    else
    {
        handleLog(tr("Server läuft bereits."));
    }

    /*
    if(ServerIsActive)
    {
        ShowLog(tr("Server läuft bereits."));
    }
    else
    {
        Server = new QTcpServer();
        ServerIsActive = true;
        if(Server->listen(QHostAddress::AnyIPv6,BoxPort->value()))
        {
            connect(Server, SIGNAL(newConnection()), this, SLOT(HandleNewConnection()));
            ShowLog(tr("Server gestartet"));
        }
        else
        {
            QString sError = Server->errorString();
            QString sMessage;
            if(sError.isEmpty())
            {
                sMessage = tr("Unbekannter Fehler beim Starten - ");
                QAbstractSocket::SocketError error = Server->serverError();

                //error handling - incomplete
                switch(error)
                {
                case QAbstractSocket::ConnectionRefusedError: sMessage.append(
                                tr("The connection was refused by the peer (or timed out)."));
                    break;
                case QAbstractSocket::RemoteHostClosedError: sMessage.append(
                                tr("The remote host closed the connection. Note that the client socket (i.e., this socket) will be closed after the remote close notification has been sent."));
                    break;
                case QAbstractSocket::HostNotFoundError: sMessage.append(
                                tr("The host address was not found."));
                    break;
                case QAbstractSocket::SocketAccessError: sMessage.append(
                                tr("The socket operation failed because the application lacked the required privileges."));
                    break;
                case QAbstractSocket::SocketResourceError: sMessage.append(
                                tr("The local system ran out of resources (e.g., too many sockets)."));
                    break;
                case QAbstractSocket::UnknownSocketError: sMessage.append(
                                tr("An unidentified error occurred."));
                    break;

                default:
                    sMessage.append("\n");

                }

            }
            else
            {
                sMessage = tr("Fehler beim Starten: %0").arg(Server->errorString());
            }
            ShowLog(sMessage);
        }
    }
    */
}

void MainWidget::ServerStop()
{
    if(Server->isListening())
    {
        Server->stopServer();
    }
    else
    {
        handleLog(tr("Server nicht aktiv."));
    }

    /*
    if(ServerIsActive)
    {
        QString sMessage = tr("Der Server wurde beenden. Bye Bye ;-)\n");
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
        newLog(tr("Server gestoppt"));
    }
    else
    {
        newLog(tr("Server läuft nicht."));
    }
    */
}

/*
void MainWidget::HandleNewConnection()
{
    Socket = Server->nextPendingConnection();
    SocketList->append(Socket);
    connect(Socket, SIGNAL(readyRead()), this, SLOT(HandleNewData()));
    QString sMessage = tr("Neue Verbindung Nummer %0").arg(SocketList->count());
    ShowLog(sMessage);
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
            QString sText = tr("Von Verbindung %0: %1").arg(i+1).arg(
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
*/

void MainWidget::handleLog(QString sMessage)
{
    //Zeile anhängen
    EdtDisplay->append(sMessage);

    //AutoScroll
    QTextCursor c = EdtDisplay->textCursor();
    c.movePosition(QTextCursor::End);
    EdtDisplay->setTextCursor(c);
}
