#ifndef EVENTS_H
#define EVENTS_H

#include <QWidget>
#include <QTableWidgetItem>
#include "remindlist.h"
namespace Ui {
class Events;
}
//日程类
class Events : public QWidget
{
    Q_OBJECT

public:
    explicit Events(QWidget *parent = 0);
    ~Events();

private slots:
    void on_pushBtnAdd_clicked();//添加按钮槽函数

    void on_pushBtnDel_clicked();//删除按钮槽函数

    void on_pushBtnModify_clicked();//修改按钮槽函数

    void addEvent(QString strName, QString strStartTime, QString strEndTime, QString strRemindTime, QString strDescription);//添加事件槽函数

    void modifyEvent(QString modifyStrName, QString modifyStrStartTime, QString modifyStrEndTime, QString modifyStrRemindTime, QString modifyStrDescription);//修改事件槽函数

    void load();//加载函数

    void save();//保存函数

    void on_tableWidgetBox_itemChanged(QTableWidgetItem *item);

    void on_tableWidgetToday_itemChanged(QTableWidgetItem *item);

private:
    Ui::Events *ui;
    RemindList remindList;//提醒列表
};

#endif // EVENTS_H
