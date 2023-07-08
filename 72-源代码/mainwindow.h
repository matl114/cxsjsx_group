#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "randomnumber.h"
#include <QMainWindow>
#include <QDesktopWidget>
#include <QMediaPlayer>
#include <QPushButton>
#include <QLabel>
#include <vector>
#include <math.h>
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
private:
    Q_OBJECT
    Ui::MainWindow *ui;
    //记录积分
    int current_score;
    //最高记录
    int highest_score;
    //记录九宫格及keep数字相关
    vector<vector<int>> nums;
    int NUM[10]={};
    static bool used_keep;
    //界面
    QWidget *gameOver;
    QLabel *OverScore;
    QPushButton *Retry,*ClickToStart;
    //音效
    QMediaPlayer *divideSound=new QMediaPlayer;
    QMediaPlayer *putSound=new QMediaPlayer;
    QMediaPlayer *over=new QMediaPlayer;
    //"随机数"生成
    RandomNumber *generator;

public:
    explicit MainWindow(QWidget * parent=nullptr);
    ~MainWindow();
private:
    bool setBox(int current_pos, QPushButton* box);
    //初始化容器
    void init_vector();
    //更新九宫格数据
    void update_data(int pos, int newNum);
    //对于数据变化触发除法
    void react_to_changes(int pos, int deep);
    //新填充数字充当除数
    void as_divisor(int pos);
    //判断结束
    bool whether_end();

private slots:
    void on_box_1_clicked();
    void on_box_2_clicked();
    void on_box_3_clicked();
    void on_box_4_clicked();
    void on_box_5_clicked();
    void on_box_6_clicked();
    void on_box_7_clicked();
    void on_box_8_clicked();
    void on_box_9_clicked();
    void on_box_keep_clicked();
    void on_pushButton_clicked();
    void Retry_clicked();
    void Start_clicked();
};

#endif // MAINWINDOW_H
