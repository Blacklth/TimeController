#ifndef REMINDPAGE_H
#define REMINDPAGE_H

#include <QDialog>
#include <QDesktopWidget>
#include <QPropertyAnimation>
#include <QTimer>
#include <QPoint>
#include <QtMultimedia/QSound>
namespace Ui {
class RemindPage;
}
//提醒窗口类
class RemindPage : public QDialog
{
    Q_OBJECT

public:
    explicit RemindPage(QWidget *parent = 0);
    ~RemindPage();
private slots:
    void closeAnimation();//关闭动画

    void clearAll();//清楚指针

    void showAnimation(QString strName);//弹出动画
private:
    Ui::RemindPage *ui;
    QDesktopWidget desktop;//桌面
    QPropertyAnimation *animation;//动画
    QTimer *remainTimer;//保持时间
    QSound *remindMusic;//提醒音乐
};

#endif // REMINDPAGE_H
