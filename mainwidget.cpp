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
}


void MainWidget::handleLog(QString sMessage)
{
    //Zeile anhängen
    EdtDisplay->append(sMessage);

    //AutoScroll
    QTextCursor c = EdtDisplay->textCursor();
    c.movePosition(QTextCursor::End);
    EdtDisplay->setTextCursor(c);
}
