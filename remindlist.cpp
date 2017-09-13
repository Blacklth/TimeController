#include "remindlist.h"

RemindList::RemindList(QObject *parent) : QObject(parent)
{
    remindpage = new RemindPage(0);
    connect(this,SIGNAL(remindInformation(QString)),remindpage,SLOT(showAnimation(QString)));//关联提醒信号和开始动画槽函数
}

RemindList::~RemindList()
{

}
void RemindList::addRemind(QString strName, int mseconds)
{
    //创建提醒
    Remind remind;
    remind.strName = strName;
    remind.remindTimer = new QTimer();
    remind.remindTimer->start(mseconds);
    remind.remindTimer->setSingleShot(true);
    connect(remind.remindTimer,SIGNAL(timeout()),remindpage,SLOT(show()));
    connect(remind.remindTimer,SIGNAL(timeout()),this,SLOT(remindTime()));
    remindList<<remind;
    qSort(remindList.begin(),remindList.end(),compareTime);
}
void RemindList::delRemind(QString strName)
{
    //删除提醒
    QList <Remind>::iterator it;
    for(it = remindList.begin();it != remindList.end();it++)
    {
        if((*it).strName == strName )
        {
            disconnect((*it).remindTimer,SIGNAL(timeout()),remindpage,SLOT(show()));
            disconnect((*it).remindTimer,SIGNAL(timeout()),this,SLOT(remindTime()));
            remindList.erase(it);
            break;
        }
    }
}

bool RemindList::compareTime(const Remind &remind1, const Remind &remind2)
{
    int mseconds1 = remind1.remindTimer->remainingTime();
    int mseconds2 = remind2.remindTimer->remainingTime();
    if(mseconds1 < mseconds2)
    {
        return true;
    }
    return false;
}
void RemindList::remindTime()
{
    emit remindInformation(remindList[0].strName);
    //到时间之后自动删除当前提醒
    disconnect(remindList[0].remindTimer,SIGNAL(timeout()),remindpage,SLOT(show()));
    disconnect(remindList[0].remindTimer,SIGNAL(timeout()),this,SLOT(remindTime()));
    remindList.removeAt(0);
    qSort(remindList.begin(),remindList.end(),compareTime);
}
