#include "remindpage.h"
#include "ui_remindpage.h"
#include <QDebug>
RemindPage::RemindPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RemindPage)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);//隐藏菜单栏
    this->move((desktop.availableGeometry().width()- this->width()),desktop.availableGeometry().height());//初始化位置到右下角
    remindMusic = new QSound(":/resources/dididi.wav",this);
}

RemindPage::~RemindPage()
{
    delete ui;
}
void RemindPage::showAnimation(QString strName){
    //设置弹框内容
    ui->textBrowser->setText(strName+tr("到时间了"));
    //显示弹出动画和音乐
    animation=new QPropertyAnimation(this,"pos");
    animation->setDuration(2000);
    animation->setStartValue(QPoint(this->x(),this->y()));
    animation->setEndValue(QPoint((desktop.availableGeometry().width()-this->width()),(desktop.availableGeometry().height()-this->height())));
    animation->start();
    remindMusic->play();

    //设置弹出框显示2秒、在弹回去
    remainTimer=new QTimer();
    connect(remainTimer,SIGNAL(timeout()),this,SLOT(closeAnimation()));
    remainTimer->start(4000);
}
void RemindPage::closeAnimation(){
    //清除Timer指针和信号槽
    remainTimer->stop();
    disconnect(remainTimer,SIGNAL(timeout()),this,SLOT(closeAnimation()));
    delete remainTimer;
    remainTimer=NULL;
    //弹出框回去动画
    animation->setStartValue(QPoint(this->x(),this->y()));
    animation->setEndValue(QPoint((desktop.availableGeometry().width()-this->width()),desktop.availableGeometry().height()));
    animation->start();
    //弹回动画完成后清理动画指针
    connect(animation,SIGNAL(finished()),this,SLOT(clearAll()));
}
void RemindPage::clearAll(){
    disconnect(animation,SIGNAL(finished()),this,SLOT(clearAll()));
    delete animation;
    animation=NULL;
    this->hide();
}
