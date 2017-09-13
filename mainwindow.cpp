#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QString>
#include <QVBoxLayout>
#include <QCloseEvent>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("TimeController"));

    //设置背景白色
    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, Qt::white);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    //设置托盘信息
    tray = new QSystemTrayIcon();
    tray->setToolTip(tr("Time Controller"));
    tray->setIcon(QIcon(":/resources/tray.png"));
    quitAct = new QAction(tr("退出"),this);
    connect(quitAct,SIGNAL(triggered()),qApp,SLOT(quit()));
    connect(tray,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(showNormal()));
    trayMenu = new QMenu(this);
    trayMenu->addAction(quitAct);
    tray->setContextMenu(trayMenu);
    tray->show();
    //按钮设置
    ui->toolBtnEvent->setCheckable(true);
    ui->toolBtnNote->setCheckable(true);
    ui->toolBtnCurriculum->setCheckable(true);
    //设置状态栏时间
    currentTimeLabel = new QLabel;
    timer1 = new QTimer(this);
    timer1->start(1000); // 每次发射timeout信号时间间隔为1秒
    connect(timer1, SIGNAL(timeout()), this, SLOT(timeUpdate()));
    //设置页面选择
    stackedlayout = new QStackedLayout();
    stackedlayout->addWidget(&eventPage);
    stackedlayout->addWidget(&notePage);
    stackedlayout->addWidget(&curriculumPage);
    ui->verticalLayout->addLayout(stackedlayout);
    stackedlayout->setCurrentWidget(&eventPage);//默认第一页
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::timeUpdate()
{
    QDateTime CurrentTime = QDateTime::currentDateTime();
    QString Timestr = CurrentTime.toString("  yyyy/MM/dd  hh:mm:ss  "); // 设置显示的格式
    currentTimeLabel->setText(Timestr);
    ui->statusBar->addPermanentWidget(currentTimeLabel);
}

void MainWindow::on_toolBtnEvent_clicked()
{
    stackedlayout->setCurrentWidget(&eventPage);
}

void MainWindow::on_toolBtnNote_clicked()
{
    stackedlayout->setCurrentWidget(&notePage);
}

void MainWindow::on_toolBtnCurriculum_clicked()
{
    stackedlayout->setCurrentWidget(&curriculumPage);
}
void MainWindow::closeEvent(QCloseEvent *event)
{
    if(tray->isVisible())
    {
        this->hide();
        tray->showMessage(tr("提醒"),tr("程序正在后台运行"));
        event->ignore();
    }
}
