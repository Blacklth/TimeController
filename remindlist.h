#ifndef REMINDLIST_H
#define REMINDLIST_H

#include <QObject>
#include <QList>
#include <QTimer>
#include "remindpage.h"
struct Remind
{
    QString strName;
    QTimer * remindTimer;
};
//提醒类
class RemindList : public QObject
{
    Q_OBJECT
public:
    explicit RemindList(QObject *parent = 0);

    ~RemindList();

    bool static compareTime(const Remind &remind1,const Remind &remind2);//比较函数

    void addRemind(QString strName,int mseconds);//添加提醒函数

    void delRemind(QString strName);//删除提醒函数
signals:
    void remindInformation(QString strName);//提醒信号
public slots:
    void remindTime();//提醒槽函数
private:
    QList <Remind> remindList;//提醒列表
    RemindPage *remindpage;//提醒窗口

};

#endif // REMINDLIST_H
