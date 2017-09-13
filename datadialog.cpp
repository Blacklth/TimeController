#include "datadialog.h"
#include "ui_datadialog.h"
#include <QMessageBox>
#include <QDebug>
DataDialog::DataDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataDialog)
{
    ui->setupUi(this);
    //设置窗口标题
    this->setWindowTitle(tr("添加事件"));
    //初始化工作
    //设置日期编辑控件显示时间为当前时间
    ui->dateTimeEditSt->setDateTime(QDateTime::currentDateTime());
    ui->dateTimeEditEt->setDateTime(QDateTime::currentDateTime());
    ui->dateTimeEditRt->setDateTime(QDateTime::currentDateTime());
    //设置日期编辑控件可以弹出日历
    ui->dateTimeEditSt->setCalendarPopup(true);
    ui->dateTimeEditEt->setCalendarPopup(true);
    ui->dateTimeEditRt->setCalendarPopup(true);
    ui->dateTimeEditRt->setEnabled(false);
}
DataDialog::DataDialog(QString strName, QString strStartTime, QString strEndTime, QString strRemindTime, QString strDescription, QWidget *parent):
    QDialog(parent),
    ui(new Ui::DataDialog)
{
    ui->setupUi(this);
    //设置窗口标题
    this->setWindowTitle(tr("修改事件"));
    //从事件读取信息
    //事件名称
    ui->lineEditName->setText(strName);
    //详细信息
    ui->textEdit->setText(strDescription);
    //开始时间
    QDateTime startTime = QDateTime::fromString(strStartTime,"yyyy/MM/dd hh:mm");
    ui->dateTimeEditSt->setDateTime(startTime);
    ui->dateTimeEditSt->setCalendarPopup(true);
    //结束时间
    QDateTime endTime = QDateTime::fromString(strEndTime,"yyyy/MM/dd hh:mm");
    ui->dateTimeEditEt->setDateTime(endTime);
    ui->dateTimeEditEt->setCalendarPopup(true);
    //提醒时间
    if(strRemindTime != " ")
    {
        QDateTime remindTime = QDateTime::fromString(strRemindTime,"yyyy/MM/dd hh:mm");
        ui->dateTimeEditRt->setDateTime(remindTime);
        ui->dateTimeEditRt->setCalendarPopup(true);
        ui->dateTimeEditRt->setEnabled(true);
        ui->checkBoxRemind->setChecked(true);
    }
    else
    {
        ui->dateTimeEditRt->setDateTime(QDateTime::currentDateTime());
        ui->dateTimeEditRt->setCalendarPopup(true);
        ui->dateTimeEditRt->setEnabled(false);
    }
}

DataDialog::~DataDialog()
{
    delete ui;
}

void DataDialog::on_pushBtnConfirm_clicked()
{
    //名称
    QString strName = ui->lineEditName->text().trimmed();//剔除名称两端空格
    //判断名称是否为空
    if(strName.isEmpty())
    {
        QMessageBox::warning(this,tr("添加行"),tr("名称不能为空！"));
        return;
    }
    //开始时间
    QString strStartTime = ui->dateTimeEditSt->dateTime().toString("yyyy/MM/dd hh:mm");
    //结束时间
    QString strEndTime = ui->dateTimeEditEt->dateTime().toString("yyyy/MM/dd hh:mm");
    //判断开始时间是否比结束时间早
    QDateTime startTime = ui->dateTimeEditSt->dateTime();
    QDateTime endTime = ui->dateTimeEditEt->dateTime();
    if(startTime > endTime)
    {
        QMessageBox::warning(this,tr("添加行"),tr("开始时间不能晚于结束时间！"));
        return;
    }
    //提醒时间
    QString strRemindTime = " ";
    Qt::CheckState state = ui->checkBoxRemind->checkState();
    if(state == Qt::Checked)
    {
       strRemindTime = ui->dateTimeEditRt->dateTime().toString("yyyy/MM/dd hh:mm");
    }
    //详细信息
    QString strDescription = ui->textEdit->toPlainText();
    //根据窗口标题发送信号
    if(this->windowTitle() == tr("添加事件"))
    {
        emit addData(strName,strStartTime,strEndTime,strRemindTime,strDescription);
    }
    else
    {
        emit modifyData(strName,strStartTime,strEndTime,strRemindTime,strDescription);
    }
    //关闭窗口
    this->close();
}

void DataDialog::on_pushBtnCancel_clicked()
{
    this->close();
}

void DataDialog::on_checkBoxRemind_stateChanged(int arg1)
{
    //根据复选框状态决定提醒时间能否编辑
    if(arg1 == Qt::Checked)
    {
        ui->dateTimeEditRt->setEnabled(true);
    }
    else
    {
        ui->dateTimeEditRt->setEnabled(false);
    }
}
