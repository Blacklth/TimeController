#include "note.h"
#include "ui_note.h"
#include <QString>
#include <QColorDialog>
#include <QFile>
Note::Note(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Note)
{
    ui->setupUi(this);
    //设置图标
    ui->pushBtnAdd->setIcon(QIcon(":/resources/add.png"));
    ui->pushBtnDel->setIcon(QIcon(":/resources/del.png"));
    ui->pushBtnSave->setIcon(QIcon(":/resources/save.png"));
    //每次选中模式
    ui->listWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    //将光标移到输入框
    ui->textEdit->setFocus();

    ui->pushBtnChuTi->setCheckable(true);
    ui->pushBtnXiaHua->setCheckable(true);

    load();
}

Note::~Note()
{
    delete ui;
}

void Note::on_pushBtnDel_clicked()
{
    QListWidgetItem *curItem = ui->listWidget->currentItem();
    //保证指针非空
    if(curItem != NULL)
    {
        if(curItem->isSelected())
        {
            //从列表控制移除选中的条目
            int row = ui->listWidget->currentRow();
            QListWidgetItem *itemDel = ui->listWidget->takeItem( row );
            //彻底删除移除的条目
            delete itemDel; itemDel = NULL;
            save();
         }
     }

}

void Note::on_pushBtnAdd_clicked()
{
    //新建item到listwidget中
    QString strName = ui->textEdit->toPlainText();
    QString strtext = ui->textEdit->toHtml();
    QListWidgetItem *newItem = new QListWidgetItem(strName,ui->listWidget);
    newItem->setToolTip(strtext);
    ui->listWidget->setCurrentItem(newItem);
    save();
}

void Note::on_pushBtnChuTi_clicked(bool checked)
{
    if(checked)
        {
            ui->textEdit->setFontWeight(QFont::Bold);   //粗体
        }
        else
        {
            ui->textEdit->setFontWeight(QFont::Normal); //普通
        }
}

void Note::on_pushBtnXiaHua_clicked(bool checked)
{
    ui->textEdit->setFontUnderline(checked);
}

void Note::on_textEdit_currentCharFormatChanged(const QTextCharFormat &format)
{
    //粗体检测
        if(format.fontWeight() == QFont::Bold)
        {
            ui->pushBtnChuTi->setChecked(true);
        }
        else
        {
            ui->pushBtnChuTi->setChecked(false);
        }

        //下划线检测
        ui->pushBtnXiaHua->setChecked( format.fontUnderline() );
}

void Note::on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if( NULL == current )
        {
            return;
        }
        QString strText = current->toolTip();
        ui->textEdit->setText( strText );
}


void Note::on_pushBtnSave_clicked()
{
    //新的工具提示信息
    QString strNew = ui->textEdit->toHtml();
    QString str = ui->textEdit->toPlainText();
    //获取当前选中条目
    QListWidgetItem *curItem = ui->listWidget->currentItem();
    if( curItem != NULL )   //检查是否为空指针
    {
        //设置新的工具提示信息
        curItem->setText(str);
        curItem->setToolTip(strNew);
        save();
    }
}
void Note::save()
{
    //获取行数
    qint32 row = ui->listWidget->count();
    QString strFileName = "note.data";
    QFile saveFile(strFileName);
    //打开文件
    saveFile.open( QIODevice::ReadWrite );

    QDataStream out(&saveFile);
    out<<row;
    for(qint32 i=0;i<row;i++)
    {
       QListWidgetItem *curItem = ui->listWidget->item(i);
       out<<*curItem;
    }
    saveFile.close();
}
void Note::load()
{
    QString strFileName = "note.data";
    QFile saveFile(strFileName);
    //打开文件
    if(!saveFile.open(QIODevice::ReadOnly))
    {
        return;
    }
    QDataStream in(&saveFile);
    qint32 row ;
    in>>row;
    for(qint32 i=0;i<row;i++)
    {
        QListWidgetItem *theItem = new QListWidgetItem(ui->listWidget);
        in>>*theItem;
    }
    saveFile.close();
}


