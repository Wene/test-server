#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtGui>
#include <QtNetwork/QtNetwork>

class MainWidget : public QWidget
{
    Q_OBJECT

private:
    //Layout
    QVBoxLayout *VLayout;
    QHBoxLayout *HLayout;

    //Buttons
    QPushButton *BtnQuit, *BtnStart, *BtnStop;

    //Display
    QTextEdit *Display;

    //Network
    QTcpServer *Server;
    bool ServerIsActive;
    QTcpSocket *Socket;
    QList<QTcpSocket*> *SocketList;

    //Funktionen
    void ShowMessage(QString sMessage);


private slots:
    void ServerStart();
    void ServerStop();
    void HandleNewConnection();
    void HandleNewData();

public:
    MainWidget(QWidget *parent = 0);
    ~MainWidget();
};

#endif // MAINWIDGET_H
