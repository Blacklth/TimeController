#ifndef DATADIALOG_H
#define DATADIALOG_H

#include <QDialog>

namespace Ui {
class DataDialog;
}
//数据窗口类
class DataDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DataDialog(QWidget *parent = 0);//添加窗口构造函数
    DataDialog(QString strName,QString strStartTime,QString strEndTime,QString strRemindTime,QString strDescription,QWidget *parent = 0);//修改窗口构造函数
    ~DataDialog();
signals:
    void addData(QString strName,QString strStartTime,QString strEndTime,QString strRemindTime,QString strDescription);//添加信号

    void modifyData(QString strName,QString strStartTime,QString strEndTime,QString strRemindTime,QString strDescription);//修改信号
private slots:
    void on_pushBtnConfirm_clicked();//确认按钮槽函数

    void on_pushBtnCancel_clicked();//取消按钮槽函数

    void on_checkBoxRemind_stateChanged(int arg1);//是否提醒状态改变槽函数

private:
    Ui::DataDialog *ui;
};

#endif // DATADIALOG_H
