#ifndef NOTE_H
#define NOTE_H

#include <QWidget>
#include <QListWidgetItem>
#include <QTextCharFormat>

namespace Ui {
class Note;
}
//便签类
class Note : public QWidget
{
    Q_OBJECT

public:
    explicit Note(QWidget *parent = 0);
    ~Note();
private slots:
    void on_pushBtnAdd_clicked();//添加按钮槽函数

    void on_pushBtnDel_clicked();//删除按钮槽函数

    void on_pushBtnSave_clicked();//保存按钮槽函数

    void on_pushBtnChuTi_clicked(bool checked);//粗体按钮槽函数

    void on_pushBtnXiaHua_clicked(bool checked);//下划线按钮槽函数

    void on_textEdit_currentCharFormatChanged(const QTextCharFormat &format);

    void on_listWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void load();//加载函数

    void save();//保存函数


private:
    Ui::Note *ui;
};

#endif // NOTE_H
