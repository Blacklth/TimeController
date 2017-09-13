#include "events.h"
#include "ui_events.h"
#include <QMessageBox>
#include <QCheckBox>
#include <QFile>
#include <QDebug>
#include <QDateTime>
#include "datadialog.h"
Events::Events(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Events)
{
    ui->setupUi(this);
    //设置图标
    ui->pushBtnAdd->setIcon(QIcon(":/resources/add.png"));
    ui->pushBtnDel->setIcon(QIcon(":/resources/del.png"));
    //设置选中模式为单选且一行
    ui->tableWidgetBox->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetToday->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetBox->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidgetToday->setSelectionMode(QAbstractItemView::SingleSelection);
    //开启根据开始时间自动排序
    ui->tableWidgetBox->setSortingEnabled(true);
    ui->tableWidgetBox->sortByColumn(1,Qt::AscendingOrder);
    ui->tableWidgetToday->setSortingEnabled(true);
    ui->tableWidgetToday->sortByColumn(1,Qt::AscendingOrder);
    //设置末尾自动拉伸
    ui->tableWidgetBox->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetToday->horizontalHeader()->setStretchLastSection(true);

    load();
}

Events::~Events()
{
    delete ui;
}

void Events::on_pushBtnAdd_clicked()
{
    //建立数据窗口
    DataDialog *dataDialog = new DataDialog;
    connect(dataDialog,SIGNAL(addData(QString,QString,QString,QString,QString)),this,SLOT(addEvent(QString,QString,QString,QString,QString)));
    //关闭之后，取消信号和槽函数的关联
    if(dataDialog->exec()== close())
    {
        disconnect(dataDialog,SIGNAL(addData(QString,QString,QString,QString,QString)),this,SLOT(addEvent(QString,QString,QString,QString,QString)));
        delete dataDialog;
        dataDialog = NULL;
    }
    this->show();
}
void Events::on_pushBtnModify_clicked()
{
    QTableWidgetItem *curItem = ui->tableWidgetBox->currentItem();
    //保证指针非空
    if(curItem != NULL)
    {
        if(curItem->isSelected())
        {
            int row = curItem->row();
            //获取当前行信息
            QTableWidgetItem *itemName = ui->tableWidgetBox->item(row,0);
            QTableWidgetItem *itemStartTime = ui->tableWidgetBox->item(row,1);
            QTableWidgetItem *itemEndTime = ui->tableWidgetBox->item(row,2);
            QTableWidgetItem *itemRemindTime = ui->tableWidgetBox->item(row,3);
            QTableWidgetItem *itemDescription = ui->tableWidgetBox->item(row,4);
            QString strName = itemName->text();
            QString strStartTime = itemStartTime->text();
            QString strEndTime = itemEndTime->text();
            QString strRemindTime = itemRemindTime->text();
            QString strDescription = itemDescription->text();
            //建立数据窗口
            DataDialog *dataDialog = new DataDialog(strName,strStartTime,strEndTime,strRemindTime,strDescription);
            connect(dataDialog,SIGNAL(modifyData(QString,QString,QString,QString,QString)),this,SLOT(modifyEvent(QString,QString,QString,QString,QString)));
            //关闭之后，取消关联
            if(dataDialog->exec()== close())
            {
                disconnect(dataDialog,SIGNAL(modifyData(QString,QString,QString,QString,QString)),this,SLOT(modifyEvent(QString,QString,QString,QString,QString)));
                delete dataDialog;
                dataDialog = NULL;
            }
            this->show();
         }
     }
}
void Events::on_pushBtnDel_clicked()
{

    QTableWidgetItem *curItem = ui->tableWidgetBox->currentItem();
    //保证指针非空
    if(curItem != NULL)
    {
        if(curItem->isSelected())
        {
            int row = curItem->row();
            //获取当前行信息
            QTableWidgetItem *itemName = ui->tableWidgetBox->item(row,0);
            QTableWidgetItem *itemStartTime = ui->tableWidgetBox->item(row,1);
            QTableWidgetItem *itemEndTime = ui->tableWidgetBox->item(row,2);
            QTableWidgetItem *itemRemindTime = ui->tableWidgetBox->item(row,3);
            QTableWidgetItem *itemDescription = ui->tableWidgetBox->item(row,4);
            QString strName = itemName->text();
            QString strStartTime = itemStartTime->text();
            QString strEndTime = itemEndTime->text();
            QString strRemindTime = itemRemindTime->text();
            QString strDescription = itemDescription->text();
            //删除提醒
            if(strRemindTime!=" ")
            {
                QDateTime remindDateTime = QDateTime::fromString(strRemindTime,"yyyy/MM/dd hh:mm");
                QDate remindDay = remindDateTime.date();
                QDate today = QDate::currentDate();
                if(remindDay == today)
                {
                    QTime curTime = QTime::currentTime();
                    QTime remindTime = remindDateTime.time();
                    if(remindTime >= curTime)
                    {
                        remindList.delRemind(strName);
                    }
                }
            }
            //删除事件列表
            ui->tableWidgetBox->removeRow(row);
            //保存修改
            save();
            //根据内容调整列宽
            ui->tableWidgetBox->resizeColumnsToContents();
            ui->tableWidgetBox->horizontalHeader()->setStretchLastSection(true);
            //同时删除今日任务
            //根据内容,找到该行
            bool found = false;
            int foundRow=-1;
            int rowToday = ui->tableWidgetToday->rowCount();
            for(int i = 0;i<rowToday;i++)
            {
                QTableWidgetItem *itemName = ui->tableWidgetToday->item(i,0);
                QTableWidgetItem *itemStartTime = ui->tableWidgetToday->item(i,1);
                QTableWidgetItem *itemEndTime = ui->tableWidgetToday->item(i,2);
                QTableWidgetItem *itemRemindTime = ui->tableWidgetToday->item(i,3);
                QTableWidgetItem *itemDescription = ui->tableWidgetToday->item(i,4);
                QString curStrName = itemName->text();
                QString curStrStartTime = itemStartTime->text();
                QString curStrEndTime = itemEndTime->text();
                QString curStrRemindTime = itemRemindTime->text();
                QString curStrDescription = itemDescription->text();
                if(curStrName == strName && curStrStartTime == strStartTime && curStrEndTime == strEndTime && curStrRemindTime == strRemindTime && curStrDescription == strDescription)
                {
                    found = true;
                    foundRow = i;
                    break;
                }
            }
            if(found == true )
            {
                ui->tableWidgetToday->removeRow(foundRow);
                //根据内容调整有列宽
                ui->tableWidgetToday->resizeColumnsToContents();
                ui->tableWidgetToday->horizontalHeader()->setStretchLastSection(true);
            }
         }
     }
}
void Events::addEvent(QString strName, QString strStartTime, QString strEndTime, QString strRemindTime, QString strDescription)
{
    //关闭排序
    ui->tableWidgetBox->setSortingEnabled(false);
    int nOldRowCount = ui->tableWidgetBox->rowCount();
    ui->tableWidgetBox->insertRow(nOldRowCount);
    //获取数据
    //名称
    QTableWidgetItem *itemName = new QTableWidgetItem(strName);
    ui->tableWidgetBox->setItem(nOldRowCount,0,itemName);
    //开始时间
    QTableWidgetItem *itemStartTime = new QTableWidgetItem(strStartTime);
    ui->tableWidgetBox->setItem(nOldRowCount,1,itemStartTime);
    //结束时间
    QTableWidgetItem *itemEndTime = new QTableWidgetItem(strEndTime);
    ui->tableWidgetBox->setItem(nOldRowCount,2,itemEndTime);
    //提醒时间
    QTableWidgetItem *itemRemindTime = new QTableWidgetItem(strRemindTime);
    ui->tableWidgetBox->setItem(nOldRowCount,3,itemRemindTime);
    //详细描述
    QTableWidgetItem *itemDescription = new QTableWidgetItem(strDescription);
    ui->tableWidgetBox->setItem(nOldRowCount,4,itemDescription);
    //是否完成
    QTableWidgetItem * itemIsDone = new QTableWidgetItem();
    itemIsDone->setCheckState(Qt::Unchecked);
    ui->tableWidgetBox->setItem(nOldRowCount,5,itemIsDone);
    //开启排序
    ui->tableWidgetBox->setSortingEnabled(true);
    ui->tableWidgetBox->sortByColumn(1, Qt::AscendingOrder);
    //根据内容调整列宽
    ui->tableWidgetBox->resizeColumnsToContents();
    ui->tableWidgetBox->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetBox->setCurrentItem(itemName);
    ui->tableWidgetBox->scrollToItem(itemName);

    QDateTime startDateTime = QDateTime::fromString(strStartTime,"yyyy/MM/dd hh:mm");
    QDateTime endDateTime = QDateTime::fromString(strEndTime,"yyyy/MM/dd hh:mm");

    QDate startDay = startDateTime.date();
    QDate endDay = endDateTime.date();
    QDate today = QDate::currentDate();
    //设置提醒
    if(strRemindTime!=" ")
    {
        QDateTime remindDateTime = QDateTime::fromString(strRemindTime,"yyyy/MM/dd hh:mm");
        QDate remindDay = remindDateTime.date();
        //如果提醒时间为今天，就加到提醒列表中
        if(remindDay == today)
        {
            QTime curTime = QTime::currentTime();
            QTime remindTime = remindDateTime.time();
            if(remindTime >= curTime)
            {
                int mseconds = curTime.msecsTo(remindTime);
                remindList.addRemind(strName,mseconds);
            }
        }
    }
    //判断日期是否包括今天
    if(startDay<=today && today <= endDay)
    {
        //添加到今日任务
        ui->tableWidgetToday->setSortingEnabled(false);
        int nOldRowCount = ui->tableWidgetToday->rowCount();
        ui->tableWidgetToday->insertRow(nOldRowCount);
        //获取数据
        //名称
        QTableWidgetItem *itemName = new QTableWidgetItem(strName);
        ui->tableWidgetToday->setItem(nOldRowCount,0,itemName);
        //开始时间
        QTableWidgetItem *itemStartTime = new QTableWidgetItem(strStartTime);
        ui->tableWidgetToday->setItem(nOldRowCount,1,itemStartTime);
        //结束时间
        QTableWidgetItem *itemEndTime = new QTableWidgetItem(strEndTime);
        ui->tableWidgetToday->setItem(nOldRowCount,2,itemEndTime);
        //提醒时间
        QTableWidgetItem *itemRemindTime = new QTableWidgetItem(strRemindTime);
        ui->tableWidgetToday->setItem(nOldRowCount,3,itemRemindTime);
        //详细描述
        QTableWidgetItem *itemDescription = new QTableWidgetItem(strDescription);
        ui->tableWidgetToday->setItem(nOldRowCount,4,itemDescription);
        //是否完成
        QTableWidgetItem * itemIsDone = new QTableWidgetItem();
        itemIsDone->setCheckState(Qt::Unchecked);
        ui->tableWidgetToday->setItem(nOldRowCount,5,itemIsDone);
        //开启排序
        ui->tableWidgetToday->setSortingEnabled(true);
        ui->tableWidgetToday->sortByColumn(1, Qt::AscendingOrder);
        //根据内容调整列宽
        ui->tableWidgetToday->resizeColumnsToContents();
        ui->tableWidgetToday->horizontalHeader()->setStretchLastSection(true);
        ui->tableWidgetToday->setCurrentItem(itemName);
        ui->tableWidgetToday->scrollToItem(itemName);
    }
    //保存
    save();
}
void Events::modifyEvent(QString modifyStrName, QString modifyStrStartTime, QString modifyStrEndTime, QString modifyStrRemindTime, QString modifyStrDescription)
{

    QTableWidgetItem *curItem = ui->tableWidgetBox->currentItem();
    //保证指针非空
    if(curItem != NULL)
    {
        if(curItem->isSelected())
        {
            int row = curItem->row();
            //获取当前行信息
            QTableWidgetItem *itemName = ui->tableWidgetBox->item(row,0);
            QTableWidgetItem *itemStartTime = ui->tableWidgetBox->item(row,1);
            QTableWidgetItem *itemEndTime = ui->tableWidgetBox->item(row,2);
            QTableWidgetItem *itemRemindTime = ui->tableWidgetBox->item(row,3);
            QTableWidgetItem *itemDescription = ui->tableWidgetBox->item(row,4);
            QString strName = itemName->text();
            QString strStartTime = itemStartTime->text();
            QString strEndTime = itemEndTime->text();
            QString strRemindTime = itemRemindTime->text();
            QString strDescription = itemDescription->text();
            //删除提醒
            if(strRemindTime != " " )
            {
                QDateTime remindDateTime = QDateTime::fromString(strRemindTime,"yyyy/MM/dd hh:mm");
                QDate remindDay = remindDateTime.date();
                QDate today = QDate::currentDate();
                if(remindDay == today)
                {
                    QTime curTime = QTime::currentTime();
                    QTime remindTime = remindDateTime.time();
                    if(remindTime >= curTime)
                    {
                        remindList.delRemind(strName);
                    }
                }
            }

            //删除今日任务
            //根据内容,找到该行
            bool found = false;
            int foundRow=-1;
            int rowToday = ui->tableWidgetToday->rowCount();
            for(int i = 0;i<rowToday;i++)
            {
                QTableWidgetItem *itemName = ui->tableWidgetToday->item(i,0);
                QTableWidgetItem *itemStartTime = ui->tableWidgetToday->item(i,1);
                QTableWidgetItem *itemEndTime = ui->tableWidgetToday->item(i,2);
                QTableWidgetItem *itemRemindTime = ui->tableWidgetToday->item(i,3);
                QTableWidgetItem *itemDescription = ui->tableWidgetToday->item(i,4);
                QString curStrName = itemName->text();
                QString curStrStartTime = itemStartTime->text();
                QString curStrEndTime = itemEndTime->text();
                QString curStrRemindTime = itemRemindTime->text();
                QString curStrDescription = itemDescription->text();
                if(curStrName == strName && curStrStartTime == strStartTime && curStrEndTime == strEndTime && curStrRemindTime == strRemindTime && curStrDescription == strDescription)
                {
                    found = true;
                    foundRow = i;
                    break;
                }
            }
            if(found == true )
            {
                ui->tableWidgetToday->removeRow(foundRow);
                //根据内容调整列宽
                ui->tableWidgetToday->resizeColumnsToContents();
                ui->tableWidgetToday->horizontalHeader()->setStretchLastSection(true);
            }

            //修改事件列表的事件
            ui->tableWidgetBox->setSortingEnabled(false);
            ui->tableWidgetBox->item(row,0)->setText(modifyStrName);
            ui->tableWidgetBox->item(row,1)->setText(modifyStrStartTime);
            ui->tableWidgetBox->item(row,2)->setText(modifyStrEndTime);
            ui->tableWidgetBox->item(row,3)->setText(modifyStrRemindTime);
            ui->tableWidgetBox->item(row,4)->setText(modifyStrDescription);
            //开启排序
            ui->tableWidgetBox->setSortingEnabled(true);
            ui->tableWidgetBox->sortByColumn(1, Qt::AscendingOrder);
            //根据内容调整列宽
            ui->tableWidgetBox->resizeColumnsToContents();
            ui->tableWidgetBox->horizontalHeader()->setStretchLastSection(true);

            //重新添加提醒
            QDateTime startDateTime = QDateTime::fromString(modifyStrStartTime,"yyyy/MM/dd hh:mm");
            QDateTime endDateTime = QDateTime::fromString(modifyStrEndTime,"yyyy/MM/dd hh:mm");

            QDate startDay = startDateTime.date();
            QDate endDay = endDateTime.date();
            QDate today = QDate::currentDate();
            //设置提醒
            if(modifyStrRemindTime!=" ")
            {
                QDateTime remindDateTime = QDateTime::fromString(modifyStrRemindTime,"yyyy/MM/dd hh:mm");
                QDate remindDay = remindDateTime.date();
                if(remindDay == today)
                {
                    QTime curTime = QTime::currentTime();
                    QTime remindTime = remindDateTime.time();
                    if(remindTime >= curTime)
                    {
                        int mseconds = curTime.msecsTo(remindTime);
                        remindList.addRemind(modifyStrName,mseconds);
                    }
                }
            }
            //重新添加到今日列表
            if(startDay<=today && today <= endDay)
            {
                ui->tableWidgetToday->setSortingEnabled(false);
                int nOldRowCount = ui->tableWidgetToday->rowCount();
                ui->tableWidgetToday->insertRow(nOldRowCount);
                //名称
                QTableWidgetItem *itemName = new QTableWidgetItem(modifyStrName);
                ui->tableWidgetToday->setItem(nOldRowCount,0,itemName);
                //开始时间
                QTableWidgetItem *itemStartTime = new QTableWidgetItem(modifyStrStartTime);
                ui->tableWidgetToday->setItem(nOldRowCount,1,itemStartTime);
                //结束时间
                QTableWidgetItem *itemEndTime = new QTableWidgetItem(modifyStrEndTime);
                ui->tableWidgetToday->setItem(nOldRowCount,2,itemEndTime);
                //提醒时间
                QTableWidgetItem *itemRemindTime = new QTableWidgetItem(modifyStrRemindTime);
                ui->tableWidgetToday->setItem(nOldRowCount,3,itemRemindTime);
                //详细描述
                QTableWidgetItem *itemDescription = new QTableWidgetItem(modifyStrDescription);
                ui->tableWidgetToday->setItem(nOldRowCount,4,itemDescription);
                //是否完成
                QTableWidgetItem * itemIsDone = new QTableWidgetItem();
                itemIsDone->setCheckState(Qt::Unchecked);
                ui->tableWidgetToday->setItem(nOldRowCount,5,itemIsDone);
                //开启排序
                ui->tableWidgetToday->setSortingEnabled(true);
                ui->tableWidgetToday->sortByColumn(1, Qt::AscendingOrder);
                //根据内容调整列宽
                ui->tableWidgetToday->resizeColumnsToContents();
                ui->tableWidgetToday->horizontalHeader()->setStretchLastSection(true);
                ui->tableWidgetToday->setCurrentItem(itemName);
                ui->tableWidgetToday->scrollToItem(itemName);
            }
            //保存
            save();


         }
     }
}

void Events::load()
{
    QString strFileName = "event.data";
    QFile saveFile(strFileName);
    //打开文件
    if(!saveFile.open(QIODevice::ReadOnly))
    {
        return;
    }
    //输入到数据流
    QDataStream in(&saveFile);
    qint32 column;
    qint32 row ;
    in>>row>>column;
    ui->tableWidgetBox->setRowCount(row);
    //关闭自动排序
    ui->tableWidgetBox->setSortingEnabled(false);
    for(qint32 i=0;i<row;i++)
    {
        for(qint32 j=0;j<column;j++)
        {
            //从数据流加载数据
            QTableWidgetItem *theItem = new QTableWidgetItem();
            in>>*theItem;
            ui->tableWidgetBox->setItem(i,j,theItem);

        }

    }
    saveFile.close();
    //重新开启排序
    ui->tableWidgetBox->setSortingEnabled(true);
    ui->tableWidgetBox->sortByColumn(1, Qt::AscendingOrder);
    //根据内容调整列宽
    ui->tableWidgetBox->resizeColumnsToContents();
    ui->tableWidgetBox->horizontalHeader()->setStretchLastSection(true);
    for(qint32 i=0;i<row;i++)
    {
        QTableWidgetItem *itemName = ui->tableWidgetBox->item(i,0);
        QTableWidgetItem *itemStartTime = ui->tableWidgetBox->item(i,1);
        QTableWidgetItem *itemEndTime = ui->tableWidgetBox->item(i,2);
        QTableWidgetItem *itemRemindTime = ui->tableWidgetBox->item(i,3);
        QTableWidgetItem *itemDescription = ui->tableWidgetBox->item(i,4);
        QString strName = itemName->text();
        QString strStartTime = itemStartTime->text();
        QString strEndTime = itemEndTime->text();
        QString strRemindTime = itemRemindTime->text();
        QString strDescription = itemDescription->text();
        //获取时间
        QDateTime startDateTime = QDateTime::fromString(strStartTime,"yyyy/MM/dd hh:mm");
        QDateTime endDateTime = QDateTime::fromString(strEndTime,"yyyy/MM/dd hh:mm");

        QDate startDay = startDateTime.date();
        QDate endDay = endDateTime.date();
        QDate today = QDate::currentDate();
        //设置提醒
        if(strRemindTime!=" ")
        {
            QDateTime remindDateTime = QDateTime::fromString(strRemindTime,"yyyy/MM/dd hh:mm");
            QDate remindDay = remindDateTime.date();
            if(remindDay == today)
            {
                QTime curTime = QTime::currentTime();
                QTime remindTime = remindDateTime.time();
                if(remindTime >= curTime)
                {
                    int mseconds = curTime.msecsTo(remindTime);
                    remindList.addRemind(strName,mseconds);
                }
            }
        }
        //判断日期
        if(startDay<=today && today <= endDay)
        {
                ui->tableWidgetToday->setSortingEnabled(false);
                int nOldRowCount = ui->tableWidgetToday->rowCount();
                ui->tableWidgetToday->insertRow(nOldRowCount);

                //名称
                QTableWidgetItem *itemName = new QTableWidgetItem(strName);
                ui->tableWidgetToday->setItem(nOldRowCount,0,itemName);
                //开始时间
                QTableWidgetItem *itemStartTime = new QTableWidgetItem(strStartTime);
                ui->tableWidgetToday->setItem(nOldRowCount,1,itemStartTime);
                //结束时间
                QTableWidgetItem *itemEndTime = new QTableWidgetItem(strEndTime);
                ui->tableWidgetToday->setItem(nOldRowCount,2,itemEndTime);
                //提醒时间
                QTableWidgetItem *itemRemindTime = new QTableWidgetItem(strRemindTime);
                ui->tableWidgetToday->setItem(nOldRowCount,3,itemRemindTime);
                //详细描述
                QTableWidgetItem *itemDescription = new QTableWidgetItem(strDescription);
                ui->tableWidgetToday->setItem(nOldRowCount,4,itemDescription);
                //是否完成
                QTableWidgetItem * itemIsDone = new QTableWidgetItem();
                itemIsDone->setCheckState(Qt::Unchecked);
                ui->tableWidgetToday->setItem(nOldRowCount,5,itemIsDone);
                //开启排序
                ui->tableWidgetToday->setSortingEnabled(true);
                ui->tableWidgetToday->sortByColumn(1, Qt::AscendingOrder);
                //根据内容调整列宽
                ui->tableWidgetToday->resizeColumnsToContents();
                ui->tableWidgetToday->horizontalHeader()->setStretchLastSection(true);
                ui->tableWidgetToday->setCurrentItem(itemName);
                ui->tableWidgetToday->scrollToItem(itemName);
        }
    }
}

void Events::save()
{
    //获取行数列数
    qint32 column = ui->tableWidgetBox->columnCount();
    qint32 row = ui->tableWidgetBox->rowCount();
    QString strFileName = "event.data";
    QFile saveFile(strFileName);
    //打开文件
    saveFile.open( QIODevice::ReadWrite );
    //输入到数据流
    QDataStream out(&saveFile);
    out<<row<<column;
    for(qint32 i=0;i<row;i++)
    {
        for(qint32 j=0;j<column;j++)
        {
            //从数据流输入到文件
            QTableWidgetItem *curItem = ui->tableWidgetBox->item(i,j);
            out<<*curItem;
        }
    }
    saveFile.close();
}


void Events::on_tableWidgetBox_itemChanged(QTableWidgetItem *item)
{
    //保证指针非空
    if( NULL == item)
    {
        return;
    }
    //如果 是否完成 为勾选状态
    if(item->checkState() == Qt::Checked)
    {
        int row = item->row();
        //获取当前行信息
        QTableWidgetItem *itemName = ui->tableWidgetBox->item(row,0);
        QTableWidgetItem *itemStartTime = ui->tableWidgetBox->item(row,1);
        QTableWidgetItem *itemEndTime = ui->tableWidgetBox->item(row,2);
        QTableWidgetItem *itemRemindTime = ui->tableWidgetBox->item(row,3);
        QTableWidgetItem *itemDescription = ui->tableWidgetBox->item(row,4);
        QString strName = itemName->text();
        QString strStartTime = itemStartTime->text();
        QString strEndTime = itemEndTime->text();
        QString strRemindTime = itemRemindTime->text();
        QString strDescription = itemDescription->text();

        //根据内容,找到该行
        bool found = false;
        int foundRow=-1;
        int rowToday = ui->tableWidgetToday->rowCount();
        for(int i = 0;i<rowToday;i++)
        {
            QTableWidgetItem *itemName = ui->tableWidgetToday->item(i,0);
            QTableWidgetItem *itemStartTime = ui->tableWidgetToday->item(i,1);
            QTableWidgetItem *itemEndTime = ui->tableWidgetToday->item(i,2);
            QTableWidgetItem *itemRemindTime = ui->tableWidgetToday->item(i,3);
            QTableWidgetItem *itemDescription = ui->tableWidgetToday->item(i,4);
            QString curStrName = itemName->text();
            QString curStrStartTime = itemStartTime->text();
            QString curStrEndTime = itemEndTime->text();
            QString curStrRemindTime = itemRemindTime->text();
            QString curStrDescription = itemDescription->text();
            if(curStrName == strName && curStrStartTime == strStartTime && curStrEndTime == strEndTime && curStrRemindTime == strRemindTime && curStrDescription == strDescription)
            {
                found = true;
                foundRow = i;
                break;
            }
        }
        if(found == true )
        {
            QTableWidgetItem *itemIsDone = ui->tableWidgetToday->item(foundRow,5);
            itemIsDone->setCheckState(Qt::Checked);
        }
        //保存修改
        save();

    }
}

void Events::on_tableWidgetToday_itemChanged(QTableWidgetItem *item)
{
    //保证指针非空
    if( NULL == item)
    {
        return;
    }
    //如果 是否完成 为勾选状态
    if(item->checkState() == Qt::Checked)
    {
        int row = item->row();
        //获取当前行信息
        QTableWidgetItem *itemName = ui->tableWidgetToday->item(row,0);
        QTableWidgetItem *itemStartTime = ui->tableWidgetToday->item(row,1);
        QTableWidgetItem *itemEndTime = ui->tableWidgetToday->item(row,2);
        QTableWidgetItem *itemRemindTime = ui->tableWidgetToday->item(row,3);
        QTableWidgetItem *itemDescription = ui->tableWidgetToday->item(row,4);
        QString strName = itemName->text();
        QString strStartTime = itemStartTime->text();
        QString strEndTime = itemEndTime->text();
        QString strRemindTime = itemRemindTime->text();
        QString strDescription = itemDescription->text();

        //获取时间
        QDateTime endDateTime = QDateTime::fromString(strEndTime,"yyyy/MM/dd hh:mm");
        QDate endDay = endDateTime.date();
        QDate today = QDate::currentDate();

        if(today == endDay)//任务最后一天
        {
            //根据内容,找到该行
            bool found = false;
            int foundRow=-1;
            int row = ui->tableWidgetBox->rowCount();
            for(int i = 0;i<row;i++)
            {
                QTableWidgetItem *itemName = ui->tableWidgetBox->item(i,0);
                QTableWidgetItem *itemStartTime = ui->tableWidgetBox->item(i,1);
                QTableWidgetItem *itemEndTime = ui->tableWidgetBox->item(i,2);
                QTableWidgetItem *itemRemindTime = ui->tableWidgetBox->item(i,3);
                QTableWidgetItem *itemDescription = ui->tableWidgetBox->item(i,4);
                QString curStrName = itemName->text();
                QString curStrStartTime = itemStartTime->text();
                QString curStrEndTime = itemEndTime->text();
                QString curStrRemindTime = itemRemindTime->text();
                QString curStrDescription = itemDescription->text();
                if(curStrName == strName && curStrStartTime == strStartTime && curStrEndTime == strEndTime && curStrRemindTime == strRemindTime && curStrDescription == strDescription)
                {
                    found = true;
                    foundRow = i;
                    break;
                }
            }
            if(found == true )
            {
                QTableWidgetItem *itemIsDone = ui->tableWidgetBox->item(foundRow,5);
                itemIsDone->setCheckState(Qt::Checked);
            }

        }

    }
}


