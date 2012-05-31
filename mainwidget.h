#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtGui>
#include <QtNetwork>
#include "chatserver.h"

class MainWidget : public QWidget
{
    Q_OBJECT

private:
    //Layout
    QVBoxLayout *LayMain;
    QHBoxLayout *LayButtons, *LayConfig;

    //Widgets
    QTextEdit *EdtDisplay;
    QPushButton *BtnQuit, *BtnStart, *BtnStop;
    QLabel *LblPort;
    QSpinBox *BoxPort;

    //Network
    ChatServer *Server;
//    QTcpServer *Server;
//    bool ServerIsActive;
//    QTcpSocket *Socket;
//    QList<QTcpSocket*> *SocketList;


private slots:
    void ServerStart();
    void ServerStop();
    void handleLog(QString);

//    void HandleNewConnection();
//    void HandleNewData();

public:
    MainWidget(QWidget *parent = 0);
    ~MainWidget();
};

#endif // MAINWIDGET_H
