#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "randomnumber.h"
using namespace std;

bool MainWindow::used_keep=false;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //初始界面
    this->setWindowTitle(QObject::tr("初始界面"));
    this->setGeometry(0,0,864,720);
    this->setStyleSheet("background-image: url(:/BackGround/START.png);");
    this->move((QApplication::desktop()->width() - 864)/2,(QApplication::desktop()->height() - 720)/2);
    ClickToStart=new QPushButton(this);
    ClickToStart->setGeometry(0,0,864,720);
    ClickToStart->setText("");
    ClickToStart->setStyleSheet("background:transparent;");
    connect(ClickToStart, SIGNAL(clicked()), this, SLOT(Start_clicked()));

    //音效初始化
    putSound->setMedia(QUrl("qrc:/SoundEffect/putBox.mp3"));
    putSound->setVolume(15);
    divideSound->setMedia(QUrl("qrc:/SoundEffect/divideBox.mp3"));
    divideSound->setVolume(30);
    over->setMedia(QUrl("qrc:/SoundEffect/over.mp3"));
    over->setVolume(10);

    //数据初始化
    init_vector();
    current_score = 0;
    highest_score = 0;

    //结束画面初始化
    gameOver=new QWidget();
    gameOver->setWindowTitle(QObject::tr("游戏结束"));
    gameOver->setGeometry(600,250,600,600);
    gameOver->setStyleSheet("background-image: url(:/BackGround/GameOver.png);");
    gameOver->hide();
    OverScore =new QLabel(gameOver);
    OverScore->setStyleSheet("background:transparent;color:#FFF3CC;");
    OverScore->setFont( QFont("MS Shell Dlg 2", 38, 50));
    OverScore->setGeometry(300,260,250,100);
    OverScore->setAlignment(Qt::AlignHCenter);
    Retry =new QPushButton(gameOver);
    Retry->setText("Restart!");
    Retry->setStyleSheet("background:#FF694A;color:#FFF3CC;border-radius: 6px;");
    Retry->setFont( QFont("MS Shell Dlg 2", 22, 50));
    Retry->setGeometry(350,475,250,65);
    connect(Retry, SIGNAL(clicked()), this, SLOT(Retry_clicked()));
}

MainWindow::~MainWindow(){
    delete ui;
}
//记录九宫格数字变量的初始化
void MainWindow::init_vector(){
    nums.resize(3);
    for (int i=0;i<3;i++){
        nums[i].resize(3);
    }
    for(int m=0;m<3;m++){
        for(int n=0;n<3;n++){
            nums[m][n] = 0;
            NUM[3*m+n]=0;
        }
    }
    NUM[9]=0;
    generator=new RandomNumber(30);
}
//更新数据
void MainWindow::update_data(int pos, int newNum){
    nums[pos/3][pos%3] = newNum;
    NUM[pos]=newNum;
}
//响应（判断并触发除法消除）
void MainWindow::react_to_changes(int pos, int deep){
    if(pos==0){
        int num=nums[0][0];
        int num_r=nums[0][1];
        int num_d=nums[1][0];
        int value1 = 0;
        int value2 = 0;

        if(num_r!=0){
            int num1 = num%num_r;
            if(!num1){
                divideSound->play();
                value1 = (num-(num/num_r))*deep;
            }
        }
        if(num_d!=0){
            int num2 = num%num_d;
            if(!num2){
                divideSound->play();
                value2=(num-(num/num_d))*deep;
            }
        }
        if(value1>=value2&&(value1>0||num_r==1)){
            current_score+=value1;
            if(current_score>highest_score){
                highest_score = current_score;
                ui->best_score->display(highest_score);
            }
            ui->now_score->display(current_score);
            ui->now_score->display(current_score);
            ui->box_1->setText(QString::number(num/num_r));
            nums[0][0] = num/num_r;
            nums[0][1]=0;
            ui->box_2->setText("");
            ui->box_2->setStyleSheet("background-color:rgb(255, 141, 103); border-radius: 10px;");
            react_to_changes(0,deep+1);
            if(nums[1][0]){
                react_to_changes(3,deep+1);
            }
        }else if(value1<value2&&(value2>0||num_d==1)){
            current_score+=value2;
            if(current_score>highest_score){
                highest_score = current_score;
                ui->best_score->display(highest_score);
            }
            ui->now_score->display(current_score);
            ui->box_1->setText(QString::number(num/num_d));
            nums[0][0] = num/num_d;
            nums[1][0]=0;
            ui->box_4->setText("");
            ui->box_4->setStyleSheet("background-color:rgb(255, 141, 103); border-radius: 10px;");
            react_to_changes(0,deep+1);
            if(nums[0][1]){
               react_to_changes(1,deep+1);
            }
        }
    }else if(pos==1){
        int num=nums[0][1];
        int num_l=nums[0][0];
        int num_r=nums[0][2];
        int num_d=nums[1][1];
        int value1 = 0;
        int value2 = 0;
        int value3 = 0;
        if(num_l!=0){
            int num1 = num%num_l;
            if(!num1){
                divideSound->play();
                value1 = (num-(num/num_l))*deep;
            }
        }
        if(num_r!=0){
            int num2 = num%num_r;
            if(!num2){
                divideSound->play();
                value2=(num-(num/num_r))*deep;
            }
        }
        if(num_d!=0){
            int num3 = num%num_d;
            if(!num3){
                divideSound->play();
                value3 = (num-(num/num_d))*deep;
            }
        }
        int max1 = qMax(value1,value2);
        int max = qMax(max1,value3);
        if(value1==max&&(value1>0||num_l==1)){
            current_score+=value1;
            if(current_score>highest_score){
                highest_score = current_score;
                ui->best_score->display(highest_score);
            }
            ui->now_score->display(current_score);
            ui->box_2->setText(QString::number(num/num_l));
            nums[0][1] = num/num_l;
            nums[0][0]=0;
            ui->box_1->setText("");
            ui->box_1->setStyleSheet("background-color:rgb(255, 141, 103); border-radius: 10px;");
            react_to_changes(1,deep+1);
            if(nums[0][2]){
                react_to_changes(2,deep+1);
            }
            if(nums[1][1]){
               react_to_changes(4,deep+1);
            }
        }else if(value2==max&&(value2>0||num_r==1)){
            current_score+=value2;
            if(current_score>highest_score){
                highest_score = current_score;
                ui->best_score->display(highest_score);
            }
            ui->now_score->display(current_score);
            ui->box_2->setText(QString::number(num/num_r));
            nums[0][1] = num/num_r;
            nums[0][2]=0;
            ui->box_3->setText("");
            ui->box_3->setStyleSheet("background-color:rgb(255, 141, 103); border-radius: 10px;");
            react_to_changes(1,deep+1);
            if(nums[0][0]){
               react_to_changes(0,deep+1);
            }
            if(nums[1][1]){
                react_to_changes(4,deep+1);
            }
        }else if(value3==max&&(value3>0||num_d==1)){
            current_score+=value3;
            if(current_score>highest_score){
                highest_score = current_score;
                ui->best_score->display(highest_score);
            }
            ui->now_score->display(current_score);
            ui->box_2->setText(QString::number(num/num_d));
            nums[0][1] = num/num_d;
            nums[1][1]=0;
            ui->box_5->setText("");
            ui->box_5->setStyleSheet("background-color:rgb(255, 141, 103); border-radius: 10px;");
            react_to_changes(1,deep+1);
            if(nums[0][2]){
                react_to_changes(2,deep+1);
            }
            if(nums[0][0]){
               react_to_changes(0,deep+1);
            }
        }
    }else if(pos==2){
        int num=nums[0][2];
        int num_l=nums[0][1];
        int num_d=nums[1][2];
        int value1=0;
        int value2=0;
        if(num_l!=0){
            int num1 = num%num_l;
            if(!num1){
                divideSound->play();
                value1 = (num-(num/num_l))*deep;
            }
        }
        if(num_d!=0){
            int num2 = num%num_d;
            if(!num2){
                divideSound->play();
                value2 = (num-(num/num_d))*deep;
            }
        }
        int max = qMax(value1,value2);
        if(value1==max&&(value1>0||num_l==1)){
            current_score+=value1;
            if(current_score>highest_score){
                highest_score = current_score;
                ui->best_score->display(highest_score);
            }
            ui->now_score->display(current_score);
            ui->box_3->setText(QString::number(num/num_l));
            nums[0][2] = num/num_l;
            nums[0][1]=0;
            ui->box_2->setText("");
            ui->box_2->setStyleSheet("background-color:rgb(255, 141, 103); border-radius: 10px;");
            react_to_changes(2,deep+1);
            if(nums[1][2]){
                react_to_changes(5,deep+1);
            }
        }else if(value2==max&&(value2>0||num_d==1)){
            current_score+=value2;
            if(current_score>highest_score){
                highest_score = current_score;
                ui->best_score->display(highest_score);
            }
            ui->now_score->display(current_score);
            ui->box_3->setText(QString::number(num/num_d));
            nums[0][2] = num/num_d;
            nums[1][2]=0;
            ui->box_6->setText("");
            ui->box_6->setStyleSheet("background-color:rgb(255, 141, 103); border-radius: 10px;");
            react_to_changes(2,deep+1);
            if(nums[0][1]){
                react_to_changes(1,deep+1);
            }
        }
    }else if(pos==3){
        int num=nums[1][0];
        int num_u=nums[0][0];
        int num_d=nums[2][0];
        int num_r=nums[1][1];
        int value1=0;
        int value2=0;
        int value3=0;
        if(num_u!=0){
            int num1 = num%num_u;
            if(!num1){
                divideSound->play();
                value1 = (num-(num/num_u))*deep;
            }
        }
        if(num_d!=0){
            int num2 = num%num_d;
            if(!num2){
                divideSound->play();
                value2 = (num-(num/num_d))*deep;
            }
        }
        if(num_r!=0){
            int num3 = num%num_r;
            if(!num3){
                divideSound->play();
                value3 = (num-(num/num_r))*deep;
            }
        }
        int max1 = qMax(value1,value2);
        int max = qMax(max1,value3);
        if(value1==max&&(value1>0||num_u==1)){
            current_score+=value1;
            if(current_score>highest_score){
                highest_score = current_score;
                ui->best_score->display(highest_score);
            }
            ui->now_score->display(current_score);
            ui->box_4->setText(QString::number(num/num_u));
            nums[1][0] = num/num_u;
            nums[0][0]=0;
            ui->box_1->setText("");
            ui->box_1->setStyleSheet("background-color:rgb(255, 141, 103); border-radius: 10px;");
            react_to_changes(3,deep+1);
            if(nums[2][0]){
                react_to_changes(6,deep+1);
            }
            if(nums[1][1]){
                react_to_changes(4,deep+1);
            }
        }else if(value2==max&&(value2>0||num_d==1)){
            current_score+=value2;
            if(current_score>highest_score){
                highest_score = current_score;
                ui->best_score->display(highest_score);
            }
            ui->now_score->display(current_score);
            ui->box_4->setText(QString::number(num/num_d));
            nums[1][0] = num/num_d;
            nums[2][0]=0;
            ui->box_7->setText("");
            ui->box_7->setStyleSheet("background-color:rgb(255, 141, 103); border-radius: 10px;");
            react_to_changes(3,deep+1);
            if(nums[0][0]){
                react_to_changes(0,deep+1);
            }
            if(nums[1][1]){
                react_to_changes(4,deep+1);
            }
        }else if(value3==max&&(value3>0||num_r==1)){
            current_score+=value3;
            if(current_score>highest_score){
                highest_score = current_score;
                ui->best_score->display(highest_score);
            }
            ui->now_score->display(current_score);
            ui->box_4->setText(QString::number(num/num_r));
            nums[1][0] = num/num_r;
            nums[1][1]=0;
            ui->box_5->setText("");
            ui->box_5->setStyleSheet("background-color:rgb(255, 141, 103); border-radius: 10px;");
            react_to_changes(3,deep+1);
            if(nums[0][0]){
                react_to_changes(0,deep+1);
            }
            if(nums[2][0]){
               react_to_changes(6,deep+1);
            }
        }
    }else if(pos==4){
        int num=nums[1][1];
        int num_u=nums[0][1];
        int num_d=nums[2][1];
        int num_l=nums[1][0];
        int num_r=nums[1][2];
        int value1=0;
        int value2=0;
        int value3=0;
        int value4=0;
        if(num_u){
            int num1 = num%num_u;
            if(!num1){
                divideSound->play();
                value1 = (num-(num/num_u))*deep;
            }
        }
        if(num_d){
            int num2 = num%num_d;
            if(!num2){
                divideSound->play();
                value2 = (num-(num/num_d))*deep;
            }
        }
        if(num_l){
            int num3 = num%num_l;
            if(!num3){
                divideSound->play();
                value3 = (num-(num/num_l))*deep;
            }
        }
        if(num_r){
            int num4 = num%num_r;
             if(!num4){
                 divideSound->play();
                 value4 = (num-(num/num_r))*deep;
            }
        }
        int max1 = qMax(value1,value2);
        int max2 = qMax(max1,value3);
        int max = qMax(max2,value4);
        if(value1==max&&(value1>0||num_u==1)){
            current_score+=value1;
            if(current_score>highest_score){
                highest_score = current_score;
                ui->best_score->display(highest_score);
            }
            ui->now_score->display(current_score);
            ui->box_5->setText(QString::number(num/num_u));
            nums[1][1] = num/num_u;
            nums[0][1]=0;
            ui->box_2->setText("");
            ui->box_2->setStyleSheet("background-color:rgb(255, 141, 103); border-radius: 10px;");
            react_to_changes(4,deep+1);
            if(nums[2][1]){
                react_to_changes(7,deep+1);
            }
            if(nums[1][0]){
                react_to_changes(3,deep+1);
            }
            if(nums[1][2]){
                react_to_changes(5,deep+1);
            }
        }else if(value2==max&&(value2>0||num_d==1)){
            current_score+=value2;
            if(current_score>highest_score){
                highest_score = current_score;
                ui->best_score->display(highest_score);
            }
            ui->now_score->display(current_score);
            ui->box_5->setText(QString::number(num/num_d));
            nums[1][1] = num/num_d;
            nums[2][1]=0;
            ui->box_8->setText("");
            ui->box_8->setStyleSheet("background-color:rgb(255, 141, 103); border-radius: 10px;");
            react_to_changes(4,deep+1);
            if(nums[0][2]){
                react_to_changes(2,deep+1);
            }
            if(nums[1][0]){
                react_to_changes(3,deep+1);
            }
            if(nums[1][2]){
                react_to_changes(5,deep+1);
            }
        }else if(value3==max&&(value3>0||num_l==1)){
            current_score+=value3;
            if(current_score>highest_score){
                highest_score = current_score;
                ui->best_score->display(highest_score);
            }
            ui->now_score->display(current_score);
            ui->box_5->setText(QString::number(num/num_l));
            nums[1][1] = num/num_l;
            nums[1][0]=0;
            ui->box_4->setText("");
            ui->box_4->setStyleSheet("background-color:rgb(255, 141, 103); border-radius: 10px;");
            react_to_changes(4,deep+1);
            if(nums[0][2]){
                react_to_changes(2,deep+1);
            }
            if(nums[2][1]){
                react_to_changes(7,deep+1);
            }
            if(nums[1][2]){
                react_to_changes(5,deep+1);
            }
        }else if(value4==max&&(value4>0||num_r==1)){
            current_score+=value4;
            if(current_score>highest_score){
                highest_score = current_score;
                ui->best_score->display(highest_score);
            }
            ui->now_score->display(current_score);
           ui->box_5->setText(QString::number(num/num_r));
           nums[1][1] = num/num_r;
           nums[1][2]=0;
           ui->box_6->setText("");
           ui->box_6->setStyleSheet("background-color:rgb(255, 141, 103); border-radius: 10px;");
           react_to_changes(4,deep+1);
           if(nums[0][2]){
               react_to_changes(2,deep+1);
           }
           if(nums[2][1]){
               react_to_changes(7,deep+1);
           }
           if(nums[1][0]){
               react_to_changes(3,deep+1);
           }
        }
    }else if(pos==5){
        int num=nums[1][2];
        int num_u=nums[0][2];
        int num_d=nums[2][2];
        int num_l=nums[1][1];
        int value1=0;
        int value2=0;
        int value3=0;
        if(num_u){
            int num1 = num%num_u;
            if(!num1){
                divideSound->play();
                value1 = (num-(num/num_u))*deep;
            }
        }
        if(num_d){
            int num2 = num%num_d;
            if(!num2){
                divideSound->play();
                value2 = (num-(num/num_d))*deep;
            }
        }
        if(num_l){
            int num3 = num%num_l;
            if(!num3){
                divideSound->play();
                value3 = (num-(num/num_l))*deep;
            }
        }
        int max1 = qMax(value1,value2);
        int max = qMax(max1,value3);
        if(value1==max&&(value1>0||num_u==1)){
            current_score+=value1;
            if(current_score>highest_score){
                highest_score = current_score;
                ui->best_score->display(highest_score);
            }
            ui->now_score->display(current_score);
            ui->box_6->setText(QString::number(num/num_u));
            nums[1][2] = num/num_u;
            nums[0][2]=0;
            ui->box_3->setText("");
            ui->box_3->setStyleSheet("background-color:rgb(255, 141, 103); border-radius: 10px;");
            react_to_changes(5,deep+1);
            if(nums[2][2]){
                react_to_changes(8,deep+1);
            }
            if(nums[1][1]){
                react_to_changes(4,deep+1);
            }
        }else if(value2==max&&(value2>0||num_d==1)){
            current_score+=value2;
            if(current_score>highest_score){
                highest_score = current_score;
                ui->best_score->display(highest_score);
            }
            ui->now_score->display(current_score);
            ui->box_6->setText(QString::number(num/num_d));
            nums[1][2] = num/num_d;
            nums[2][2]=0;
            ui->box_9->setText("");
            ui->box_9->setStyleSheet("background-color:rgb(255, 141, 103); border-radius: 10px;");
            react_to_changes(5,deep+1);
            if(nums[0][2]){
                react_to_changes(2,deep+1);
            }
            if(nums[1][1]){
                react_to_changes(4,deep+1);
            }
        }else if(value3==max&&(value3>0||num_l==1)){
            current_score+=value3;
            if(current_score>highest_score){
                highest_score = current_score;
                ui->best_score->display(highest_score);
            }
            ui->now_score->display(current_score);
            ui->box_6->setText(QString::number(num/num_l));
            nums[1][2]= num/num_l;
            nums[1][1]=0;
            ui->box_5->setText("");
            ui->box_5->setStyleSheet("background-color:rgb(255, 141, 103); border-radius: 10px;");
            react_to_changes(5,deep+1);
            if(nums[0][2]){
                react_to_changes(2,deep+1);
            }
            if(nums[2][2]){
                react_to_changes(8,deep+1);
            }
        }
    }else if(pos==6){
        int num=nums[2][0];
        int num_u=nums[1][0];
        int num_r=nums[2][1];
        int value1=0;
        int value2=0;
        if(num_u){
            int num1 = num%num_u;
            if(!num1){
                divideSound->play();
                value1 = (num-(num/num_u))*deep;
            }
        }
        if(num_r){
            int num2 = num%num_r;
            if(!num2){
                divideSound->play();
                value2 = (num-(num/num_r))*deep;
            }
        }
        int max = qMax(value1,value2);
        if(value1==max&&(value1>0||num_u==1)){
            current_score+=value1;
            if(current_score>highest_score){
                highest_score = current_score;
                ui->best_score->display(highest_score);
            }
            ui->now_score->display(current_score);
            ui->box_7->setText(QString::number(num/num_u));
            nums[2][0] = num/num_u;
            nums[1][0]=0;
            ui->box_4->setText("");
            ui->box_4->setStyleSheet("background-color:rgb(255, 141, 103); border-radius: 10px;");
            react_to_changes(6,deep+1);
            if(nums[2][1]){
                react_to_changes(7,deep+1);
            }
        }else if(value2==max&&(value2>0||num_r==1)){
            current_score+=value2;
            if(current_score>highest_score){
                highest_score = current_score;
                ui->best_score->display(highest_score);
            }
            ui->now_score->display(current_score);
            ui->box_7->setText(QString::number(num/num_r));
            nums[2][0] = num/num_r;
            nums[2][1]=0;
            ui->box_8->setText("");
            ui->box_8->setStyleSheet("background-color:rgb(255, 141, 103); border-radius: 10px;");
            react_to_changes(6,deep+1);
            if(nums[1][0]){
                react_to_changes(3,deep+1);
            }
        }
    }else if(pos==7){
        int num=nums[2][1];
        int num_u=nums[1][1];
        int num_l=nums[2][0];
        int num_r=nums[2][2];
        int value1=0;
        int value2=0;
        int value3=0;
        if(num_u){
            int num1 = num%num_u;
            if(!num1){
                divideSound->play();
                value1 = (num-(num/num_u))*deep;
            }
        }
        if(num_l){
            int num2 = num%num_l;
            if(!num2){
                divideSound->play();
                value2 = (num-(num/num_l))*deep;
            }
        }
        if(num_r){
            int num3 = num%num_r;
            if(!num3){
                divideSound->play();
                value3 = (num-(num/num_r))*deep;
            }
        }
        int max1 = qMax(value1,value2);
        int max = qMax(max1,value3);
        if(value1==max&&(value1>0||num_u==1)){
            current_score+=value1;
            if(current_score>highest_score){
                highest_score = current_score;
                ui->best_score->display(highest_score);
            }
            ui->now_score->display(current_score);
            ui->box_8->setText(QString::number(num/num_u));
            nums[2][1] = num/num_u;
            nums[1][1]=0;
            ui->box_5->setText("");
            ui->box_5->setStyleSheet("background-color:rgb(255, 141, 103); border-radius: 10px;");
            react_to_changes(7,deep+1);
            if(nums[2][0]){
               react_to_changes(6,deep+1);
            }
            if(nums[2][2]){
                react_to_changes(8,deep+1);
            }
        }else if(value2==max&&(value2>0||num_l==1)){
            current_score+=value2;
            if(current_score>highest_score){
                highest_score = current_score;
                ui->best_score->display(highest_score);
            }
            ui->now_score->display(current_score);
            ui->box_8->setText(QString::number(num/num_l));
            nums[2][1] = num/num_l;
            nums[2][0]=0;
            ui->box_7->setText("");
            ui->box_7->setStyleSheet("background-color:rgb(255, 141, 103); border-radius: 10px;");
            react_to_changes(7,deep+1);
            if(nums[1][1]){
                react_to_changes(4,deep+1);
            }
            if(nums[2][2]){
                react_to_changes(8,deep+1);
            }
        }else if(value3==max&&(value3>0||num_r==1)){
            current_score+=value3;
            if(current_score>highest_score){
                highest_score = current_score;
                ui->best_score->display(highest_score);
            }
            ui->now_score->display(current_score);
            ui->box_8->setText(QString::number(num/num_r));
            nums[2][1] = num/num_r;
            nums[2][2]=0;
            ui->box_9->setText("");
            ui->box_9->setStyleSheet("background-color:rgb(255, 141, 103); border-radius: 10px;");
            react_to_changes(7,deep+1);
            if(nums[1][1]){
                react_to_changes(4,deep+1);
            }
            if(nums[2][0]){
               react_to_changes(6,deep+1);
            }
        }
    }else if(pos==8){
        int num=nums[2][2];
        int num_u=nums[1][2];
        int num_l=nums[2][1];
        int value1=0;
        int value2=0;
        if(num_u){
            int num1 = num%num_u;
            if(!num1){
                divideSound->play();
                value1 = (num-(num/num_u))*deep;
            }
        }
        if(num_l){
            int num2 = num%num_l;
            if(!num2){
                divideSound->play();
                value2 = (num-(num/num_l))*deep;
            }
        }
        int max = qMax(value1,value2);
        if(value1==max&&(value1>0||num_u==1)){
            current_score+=value1;
            if(current_score>highest_score){
                highest_score = current_score;
                ui->best_score->display(highest_score);
            }
            ui->now_score->display(current_score);
            ui->box_9->setText(QString::number(num/num_u));
            nums[2][2] = num/num_u;
            nums[1][2]=0;
            ui->box_6->setText("");
            ui->box_6->setStyleSheet("background-color:rgb(255, 141, 103); border-radius: 10px;");
            react_to_changes(8,deep+1);
            if(nums[2][1]){
                react_to_changes(7,deep+1);
            }
        }else if(value2==max&&(value2>0||num_l==1)){
            current_score+=value2;
            if(current_score>highest_score){
                highest_score = current_score;
                ui->best_score->display(highest_score);
            }
            ui->now_score->display(current_score);
            ui->box_9->setText(QString::number(num/num_l));
            nums[2][2] = num/num_l;
            nums[2][1]=0;
            ui->box_8->setText("");
            ui->box_8->setStyleSheet("background-color:rgb(255, 141, 103); border-radius: 10px;");
            react_to_changes(8,deep+1);
            if(nums[1][2]){
                react_to_changes(5,deep+1);
            }
        }
    }
}
void MainWindow::as_divisor(int pos){
    if(pos==0){
        if(nums[0][1]){
            react_to_changes(1,1);
        }
        if(nums[1][0]){
            react_to_changes(3,1);
        }
    }
    else if(pos==1){
        if(nums[0][2]){
            react_to_changes(2,1);
        }
        if(nums[1][1]){
            react_to_changes(4,1);
        }
        if(nums[0][0]){
            react_to_changes(0,1);
        }
    }
    else if(pos==2){
        if(nums[0][1]){
            react_to_changes(1,1);
        }
        if(nums[1][2]){
            react_to_changes(5,1);
        }
    }
    else if(pos==3){
        if(nums[0][0]){
            react_to_changes(0,1);
        }
        if(nums[1][1]){
            react_to_changes(4,1);
        }
        if(nums[2][0]){
            react_to_changes(6,1);
        }
    }
    else if(pos==4){
        if(nums[0][1]){
            react_to_changes(1,1);
        }
        if(nums[1][0]){
            react_to_changes(3,1);
        }
        if(nums[1][2]){
            react_to_changes(5,1);
        }
        if(nums[2][1]){
            react_to_changes(7,1);
        }
    }
    else if(pos==5){
        if(nums[0][2]){
            react_to_changes(2,1);
        }
        if(nums[1][1]){
            react_to_changes(4,1);
        }
        if(nums[2][2]){
            react_to_changes(8,1);
        }
    }
    else if(pos==6){
        if(nums[1][0]){
            react_to_changes(3,1);
        }
        if(nums[2][1]){
            react_to_changes(7,1);
        }
    }
    else if(pos==7){
        if(nums[1][1]){
            react_to_changes(4,1);
        }
        if(nums[2][2]){
            react_to_changes(8,1);
        }
        if(nums[2][0]){
            react_to_changes(6,1);
        }
    }
    else if(pos==8){
        if(nums[1][2]){
            react_to_changes(5,1);
        }
        if(nums[2][1]){
            react_to_changes(7,1);
        }
    }
}
//判断游戏是否结束
bool MainWindow::whether_end(){
    for(int m=0;m<3;m++)
        for(int n=0;n<3;n++)
            if(!nums[m][n])
                return false;
    return true;

}
//网格中放置相关数字
bool MainWindow::setBox(int current_pos,QPushButton* box){
    if (box->text().length())  {return false;}
    else
    {
        if(box!=ui->box_keep){
            current_score++;
            putSound->play();
        }
        ui->now_score->display(current_score);
        if(current_score>highest_score){
            highest_score = current_score;
            ui->best_score->display(highest_score);
        }

        if(used_keep)
        {
            box->setText(ui->box_keep->text());
            update_data(current_pos-1, ui->box_keep->text().toInt());
            ui->box_keep->setText("");
            used_keep=false;

            QColor c=ui->box_keep->palette().color(QPalette::Background);
            if(c==QColor(246,170,0,255))
                box->setStyleSheet("background-color:#F6AA00;border-radius: 10px;color:#FFF3CC");

            ui->box_keep->setStyleSheet("background-color:#ff8d67;border-radius: 10px;color:#FFF3CC");
            ui->box_put->setStyleSheet("background-color:#F6AA00;border-radius: 10px;color:#FFF3CC;border: 4px solid red;");
        }
        else
        {
            box->setText(ui->box_put->text());
            update_data(current_pos-1, ui->box_put->text().toInt());
            QColor c=ui->box_put->palette().color(QPalette::Background);
            if(c==QColor(246,170,0,255))
                box->setStyleSheet("background-color:#F6AA00;border-radius: 10px;color:#FFF3CC");
            ui->box_put->setText(ui->box_next->text());
            ui->box_next->setText(QString::number(generator->get(NUM)));

        }
        react_to_changes(current_pos-1,1);
        as_divisor(current_pos-1);
        if(whether_end()){
            over->play();
            OverScore->setText(QString::number(current_score));
            gameOver->show();
        }
        return true;
    }
}
void MainWindow::on_box_1_clicked(){
    setBox(1,ui->box_1);
}
void MainWindow::on_box_2_clicked(){
    setBox(2,ui->box_2);
}
void MainWindow::on_box_3_clicked(){
    setBox(3,ui->box_3);
}
void MainWindow::on_box_4_clicked(){
    setBox(4,ui->box_4);
}
void MainWindow::on_box_5_clicked(){
     setBox(5,ui->box_5);
}
void MainWindow::on_box_6_clicked(){
    setBox(6,ui->box_6);
}
void MainWindow::on_box_7_clicked(){
    setBox(7,ui->box_7);
}
void MainWindow::on_box_8_clicked(){
    setBox(8,ui->box_8);
}
void MainWindow::on_box_9_clicked(){
    setBox(9,ui->box_9);
}
void MainWindow::on_box_keep_clicked(){
    if(!setBox(0,ui->box_keep))
    {
        ui->box_put->setStyleSheet("background-color:#F6AA00;border-radius: 10px;color:#FFF3CC;");
        ui->box_keep->setStyleSheet("background-color:#F6AA00;border-radius: 10px;color:#FFF3CC;border: 4px solid red;");
        used_keep=true;
    }
    else
        NUM[9]=ui->box_keep->text().toInt();
}
//主界面中restart游戏
void MainWindow::on_pushButton_clicked(){
    init_vector();
    current_score=0;
    ui->now_score->display(0);
    ui->best_score->display(highest_score);
    //num = 2;
    used_keep = false;
    ui->box_put->setText(QString::number(generator->get(NUM)));
    ui->box_next->setText(QString::number(generator->get(NUM)));
    ui->box_keep->setText("");
    ui->box_keep->setStyleSheet("background-color:rgb(255, 141, 103); border-radius: 10px;");
    ui->box_1->setText("");
    ui->box_1->setStyleSheet("background-color:rgb(255, 141, 103); border-radius: 10px;");
    ui->box_2->setText("");
    ui->box_2->setStyleSheet("background-color:rgb(255, 141, 103); border-radius: 10px;");
    ui->box_3->setText("");
    ui->box_3->setStyleSheet("background-color:rgb(255, 141, 103); border-radius: 10px;");
    ui->box_4->setText("");
    ui->box_4->setStyleSheet("background-color:rgb(255, 141, 103); border-radius: 10px;");
    ui->box_5->setText("");
    ui->box_5->setStyleSheet("background-color:rgb(255, 141, 103); border-radius: 10px;");
    ui->box_6->setText("");
    ui->box_6->setStyleSheet("background-color:rgb(255, 141, 103); border-radius: 10px;");
    ui->box_7->setText("");
    ui->box_7->setStyleSheet("background-color:rgb(255, 141, 103); border-radius: 10px;");
    ui->box_8->setText("");
    ui->box_8->setStyleSheet("background-color:rgb(255, 141, 103); border-radius: 10px;");
    ui->box_9->setText("");
    ui->box_9->setStyleSheet("background-color:rgb(255, 141, 103); border-radius: 10px;");
}
//游戏结束界面restart游戏
void MainWindow::Retry_clicked(){
    gameOver->hide();
    on_pushButton_clicked();
}
//初始界面start游戏
void MainWindow::Start_clicked(){
    ui->setupUi(this);
    ui->box_put->setStyleSheet("background-color:#F6AA00;border-radius: 10px;color:#FFF3CC;border: 4px solid red;");
    ui->box_put->setText(QString::number(generator->get(NUM)));
    ui->box_next->setStyleSheet("background-color:#F6AA00;border-radius: 10px;color:#FFF3CC");
    ui->box_next->setText(QString::number(generator->get(NUM)));
    ClickToStart->close();
}
