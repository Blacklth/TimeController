#include "curriculum.h"
#include "ui_curriculum.h"
#include <QFile>
#include <QDebug>
Curriculum::Curriculum(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Curriculum)
{
    ui->setupUi(this);
    //每次选中一格
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectItems);
    //关闭自动排序
    ui->tableWidget->setSortingEnabled(false);
    //把选中模式设为单选
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    //为每个item初始化
    int column = ui->tableWidget->columnCount();
    int row = ui->tableWidget->rowCount();
    for(int i=0;i<row;i++)
    {
        for(int j=0;j<column;j++)
        {
            QString str = NULL;
            QTableWidgetItem *item = new QTableWidgetItem(str);
            ui->tableWidget->setItem(i,j,item);

        }
    }
    //加载
    load();

}

Curriculum::~Curriculum()
{
    delete ui;
}

void Curriculum::on_tableWidget_itemChanged(QTableWidgetItem *item)
{
    //设置居中对齐
    item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
}

void Curriculum::load()
{
    QString strFileName = "curriculum.data";
    QFile saveFile(strFileName);
    //打开文件
    if(!saveFile.open(QIODevice::ReadOnly))
    {
        return;
    }
    else
    {
        //输入进数据流
        QDataStream in(&saveFile);
        qint32 column;
        qint32 row ;
        in>>row>>column;
        ui->tableWidget->setRowCount(row);
        for(qint32 i=0;i<row;i++)
        {
            for(qint32 j=0;j<column;j++)
            {
                //从数据流读取数据
                QTableWidgetItem *theItem = new QTableWidgetItem();
                in>>*theItem;
                ui->tableWidget->setItem(i,j,theItem);

            }

        }
        //关闭文件
        saveFile.close();
    }
}

void Curriculum::save()
{
    //获取行数列数
    qint32 column = ui->tableWidget->columnCount();
    qint32 row = ui->tableWidget->rowCount();
    QString strFileName = "curriculum.data";
    QFile saveFile(strFileName);
    //打开文件
    saveFile.open( QIODevice::ReadWrite);
    //输出到数据流
    QDataStream out(&saveFile);
    out<<row<<column;
    for(qint32 i=0;i<row;i++)
    {
        for(qint32 j=0;j<column;j++)
        {
            //从数据流输出文件
            QTableWidgetItem *curItem = ui->tableWidget->item(i,j);
            out<<*curItem;
        }
    }
    //关闭文件
    saveFile.close();
}

void Curriculum::on_pushButtonSave_clicked()
{
    save();
}
