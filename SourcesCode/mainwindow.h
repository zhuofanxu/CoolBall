#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <set>
//#include <qwidget.h>
//#include <QPainter>
#include "gird.h"
#include "ball.h"
#include "helpdialog.h"
#include "highrecord.h"
namespace Ui {
class MainWindow;
}
enum ObliqueDirection
{
    LEFT,
    RIGHT
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();

    void on_actionIntroduce_triggered();

    void on_actionRestart_triggered();

    void on_actionBillboard_triggered();

    void on_actionSave_triggered();

private:

    void fillBoundarySet();
    void drawSelectRect(int index);
    int  countSameColorRow(int index);     //横向@index 目标方格索引
    int  countSameColorCol(int index);    //纵向
    void countSameColorOblique(int index,ObliqueDirection odir);//斜向
    void drawBalls();
    void drawChessBoard();
    void produceNewBalls();
    int  randNoRepeatLoop();
    void produceDiffrentNumber();
    void produceSpecialNumber();
    void Scoring(int countdir,int countballs);                 //游戏记分
    void deleteSameColorBalls(int targetindex);
    void updateBallIndexOfGird(int deleteballindex); //更新含有小球的方格的小球索引号
    void gameMainLogic(int targetindex);                   //游戏主逻辑
    void decideMoveEnabled(int select,int target);   //判断小球是否能移动到目标点
    void loadGameData();
    void saveGameDate();
    void loadHighRecord();
    void saveHighRecord();
    Ui::MainWindow *ui;
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent *event);
    QPainter* paint;
    QMouseEvent* mouse;
    HelpDialog *helpDialog;
    HighRecord *highRecord;
    Gird        *m_gird;
    Ball        *m_ball;
    std::set<int>      boundarySet;  //方格边界索引集合
    std::set<int>      LboundaryMaxSet; //左斜方的max边界索引集合
    std::set<int>      LboundaryMinSet; //左斜方的min边界索引集合
    std::set<int>      RboundaryMaxSet; //右斜方的max边界索引集合
    std::set<int>      RboundaryMinSet; //右斜方的min边界索引集合
    std::vector<Ball*> vb;
    std::vector<Gird*> vg;
    std::vector<int>   vindex;
    std::vector<int>   vsameindexLb;
    std::vector<int>   vsameindexRb;
    std::vector<int>   vsameindexR;
    std::vector<int>   vsameindexC;
    std::vector<int>   vsavecolor;
    std::vector<int>::iterator vit;
    bool m_MoveEnable;          //是否可以移动
    bool m_Touchenabled;        //点击是否有效
    bool drawRect;           //是否绘制选中矩形
    bool drawNewBalls;       //是否绘制新球
    bool selected;           //是否选中小球
    int  selectIndex;       //选中的小球所在方格索引
    int  moveToIndex;       //要移动到的目标空方格索引
    int  randIndex[3];      //存储3个不同的随机方格索引用于游戏初始时放置小球
    int  randIndexWithoutBall[3]; //3个不同的且为空的方格索引用于游戏进行时
                                 //放置新生成的小球
    int  m_Score;             //得分
    int  highScore[3];



};

#endif // MAINWINDOW_H
