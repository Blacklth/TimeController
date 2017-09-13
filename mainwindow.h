#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QStackedLayout>
#include <QSystemTrayIcon>
#include <QList>
#include "events.h"
#include "note.h"
#include "curriculum.h"
#include "remindpage.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void timeUpdate();//时间更新槽函数

    void on_toolBtnEvent_clicked();//日程按钮槽函数

    void on_toolBtnNote_clicked();//便签按钮槽函数

    void on_toolBtnCurriculum_clicked();//课表按钮槽函数
private:
    void closeEvent(QCloseEvent *event);//重载关闭事件

private:
    Ui::MainWindow *ui;
    //状态栏时间
    QLabel *currentTimeLabel;
    QTimer *timer1;
    //系统布局
    QStackedLayout *stackedlayout;
    Events eventPage;
    Note notePage;
    Curriculum curriculumPage;
    QTimer *timer2;
    //系统托盘
    QSystemTrayIcon *tray;
    QAction *quitAct;
    QMenu *trayMenu;
};

#endif // MAINWINDOW_H
