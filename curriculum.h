#ifndef CURRICULUM_H
#define CURRICULUM_H

#include <QWidget>
#include <QTableWidgetItem>
namespace Ui {
class Curriculum;
}
//课表类
class Curriculum : public QWidget
{
    Q_OBJECT

public:
    explicit Curriculum(QWidget *parent = 0);
    ~Curriculum();

private slots:
    void on_tableWidget_itemChanged(QTableWidgetItem *item);

    void load();//加载函数

    void save();//保存函数


    void on_pushButtonSave_clicked();//保存按钮的槽函数

private:
    Ui::Curriculum *ui;
};

#endif // CURRICULUM_H
