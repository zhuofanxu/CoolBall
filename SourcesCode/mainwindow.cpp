#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QTime>
#include <QFile>
#include <QStringList>
#include <qtextstream.h>
#include <qiodevice.h>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    helpDialog(new HelpDialog),
    highRecord(new HighRecord),
    drawRect(false),
    selected(false),
    drawNewBalls(false),
    m_Score(0),
    m_Touchenabled(true),
    m_MoveEnable(false)
{
    srand((unsigned)time(NULL));
    ui->setupUi(this);
    setGeometry(283,100,800,600);        //设置窗口大小以及背景颜色
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, Qt::darkGreen);
    this->setPalette(palette);
    fillBoundarySet();
    produceDiffrentNumber();
    for(int i=0;i<100;i++)
    {
        m_gird=new Gird(i);
        vg.push_back(m_gird);
    }

    loadGameData();
    if(vsavecolor.size()<3)
    {
        produceDiffrentNumber();
        for(int i=0;i<3;i++)
        {
            m_ball=new Ball();
            m_ball->setIndexInGird(randIndex[i]);
            vb.push_back(m_ball);
            vindex.push_back(randIndex[i]);       //将含有小球的方格索引存储到索引数组
           // qDebug("vindex.size=%d",vindex.size());
        }
    }
    loadHighRecord();
    ui->lcdNumber->setProperty("intValue", QVariant(m_Score));

}

MainWindow::~MainWindow()
{
    delete ui;
    delete helpDialog;
    saveHighRecord();
}

void MainWindow::loadHighRecord()
{
    QFile file("../highrecord.data");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        QString line;
        int i=0;
        while (!stream.atEnd())
        {
            line = stream.readLine(); // 不包括“\n”的一行文本
            highScore[i]=line.toInt();
            highRecord->setHighScore(highScore[i],i);
            i++;
        }
            file.close();
    }
}

void MainWindow::saveHighRecord()
{
    QFile file("../highrecord.data");
    if (file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QTextStream stream(&file);
        for(int i=0;i<3;i++)
        {
            if(m_Score>highScore[i])
            {
                stream<<m_Score<<"\n";
                m_Score=highScore[i];
            }
            else
                stream<<highScore[i]<<"\n";
        }
        file.close();
    }
}

void MainWindow::loadGameData()
{
    QFile fileindex("../saveindex.data");
    if (fileindex.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&fileindex);
        QString lineindex;
        int index=0;
        while (!stream.atEnd())
        {
            lineindex = stream.readLine(); // 不包括“\n”的一行文本
            index=lineindex.toInt();
            vindex.push_back(index);
        }
        fileindex.close();
    }

    QFile filecolor("../savecolor.data");
    if (filecolor.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&filecolor);
        QString linecolor;
        int color=0;
        while (!stream.atEnd())
        {
            linecolor = stream.readLine(); // 不包括“\n”的一行文本
            color=linecolor.toInt();
            vsavecolor.push_back(color);
        }
        filecolor.close();
    }
    QFile filescore("../savescore.data");
    if (filescore.open(QIODevice::ReadOnly|QIODevice::Text))
    {
         QTextStream stream(&filescore);
         QString linescore;
         while(!stream.atEnd())
         {
             linescore=stream.readLine();
             m_Score=linescore.toInt();
         }
         filescore.close();
    }
    if(vindex.size()!=vsavecolor.size())
        qDebug("读取游戏数据出错");
    else if(vsavecolor.size()>2)
    {
        for(int i=0;i<vindex.size();i++)
        {
            vg[vindex[i]]->setIsEmpty(false);
            m_ball=new Ball(true);
            m_ball->setIndexInGird(vindex[i]);
            m_ball->setColor(vsavecolor[i]);
            vb.push_back(m_ball);
        }
    }
}

void MainWindow::saveGameDate()
{
    QFile fileindex("../saveindex.data");
    if (fileindex.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QTextStream stream(&fileindex);
        for(int i=0;i<vindex.size();i++)
        {
            stream<<vindex[i]<<"\n";
        }
        fileindex.close();
    }
    QFile filecolor("../savecolor.data");
    if (filecolor.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QTextStream stream(&filecolor);
        int ballindex=0;
        for(int i=0;i<vindex.size();i++)
        {
            ballindex=vg[vindex[i]]->getBallIndex();
            stream<<vb[ballindex]->getColor()<<"\n";
        }
        filecolor.close();
    }
    QFile filescore("../savescore.data");
    if (filescore.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QTextStream stream(&filescore);
        stream<<m_Score<<"\n";
        filescore.close();
    }
}

void MainWindow::fillBoundarySet()
{
    for(int i=0;i<10;i++)
        boundarySet.insert(i);
    for(int i=10;i<=90;i+=10)
        boundarySet.insert(i);
    for(int i=91;i<=99;i++)
        boundarySet.insert(i);
    for(int i=19;i<=89;i+=10)
        boundarySet.insert(i);

    for(int i=10;i<=90;i+=10)
        LboundaryMaxSet.insert(i);
    for(int i=91;i<=98;i++)
        LboundaryMaxSet.insert(i);
    for(int i=1;i<10;i++)
        LboundaryMinSet.insert(i);
    for(int i=19;i<=89;i+=10)
        LboundaryMinSet.insert(i);

    for(int i=91;i<=99;i++)
        RboundaryMaxSet.insert(i);
    for(int i=19;i<=89;i+=10)
        RboundaryMaxSet.insert(i);
    for(int i=0;i<9;i++)
        RboundaryMinSet.insert(i);
    for(int i=10;i<=80;i+=10)
        RboundaryMinSet.insert(i);


}

int MainWindow::countSameColorRow(int index)
{
    int row=index/10;                 //index所在行
    int countRow=0;
   // int ballindex=vg[index]->getBallIndex();
   // qDebug("ballindex=%d",ballindex);
    int currcolor=vb[vg[index]->getBallIndex()]->getColor(); //当前小球的颜色
    if(index==row*10)                                   //index恰好是左边缘索引
    {
        for(int i=index+1;i<=row*10+9;i++)   //只遍历index的右面 index+1->右
        {
            if(vg[i]->getIsEmpty()||(!vg[i]->getIsEmpty()&&
                                     vb[vg[i]->getBallIndex()]->getColor()!=currcolor))
            {
               countRow=i-index;    //行方向与index内的球颜色相同的个数
               for(int j=index;j<i;j++)   //将同色小球所在方格索引保存到中
               {
                   vsameindexR.push_back(j);
               }
               break;
            }
            else if(i==row*10+9)                  //从index到index所在行的边缘都为同色球的情况
            {
                countRow=i-index+1;
                for(int j=index;j<=i;j++)
                {
                    vsameindexR.push_back(j);
                }
            }

        }
    }
    else if(index==row*10+9)                 //index恰好是右边缘索引
    {
        for(int i=index-1;i>=row*10;i--)   //只遍历index的左面 index-1->左
        {
            if(vg[i]->getIsEmpty()||(!vg[i]->getIsEmpty()&&
                                     vb[vg[i]->getBallIndex()]->getColor()!=currcolor))
            {
               countRow=index-i;    //行方向与index内的球颜色相同的个数
               for(int j=index;j>i;j--)
               {
                   vsameindexR.push_back(j);
               }
               break;
            }
            else if(i==row*10)
            {
                countRow=index-i+1;
                for(int j=index;j>=i;j--)
                {
                    vsameindexR.push_back(j);
                }
            }

        }
    }
    else
    {
        for(int i=index+1;i<=row*10+9;i++)   //先遍历index的右面 index+1->右
        {
            int ballindex=vg[i]->getBallIndex();
            if(vg[i]->getIsEmpty()||(!vg[i]->getIsEmpty()&&
                                     vb[vg[i]->getBallIndex()]->getColor()!=currcolor))
            {
               countRow+=i-index;    //index右面与index内的球颜色相同的个数
               for(int j=index;j<i;j++)
               {
                   vsameindexR.push_back(j);
               }
               break;
            }
            else if(i==row*10+9)
            {
                countRow+=i-index+1;
                for(int j=index;j<=i;j++)
                {
                    vsameindexR.push_back(j);
                }
            }

        }
        for(int i=index-1;i>=row*10;i--)   //然后遍历index的左面 index-1->左
        {
            if(vg[i]->getIsEmpty()||(!vg[i]->getIsEmpty()&&
                                     vb[vg[i]->getBallIndex()]->getColor()!=currcolor))
            {
               countRow+=index-i;    //index左面与index内的球颜色相同的个数
               for(int j=index-1;j>i;j--)   //将同色小球所在方格索引保存到中
               {
                   vsameindexR.push_back(j);
               }
               break;
            }
            else if(i==row*10)
            {
                countRow+=index-i+1;
                for(int j=index-1;j>=i;j--)
                {
                    vsameindexR.push_back(j);
                }
            }

        }
        countRow-=1;             //index本身算了两次 故要减一
    }
   // qDebug("countRow=%d",countRow);
   // qDebug("vsameR.size=%d",vsameindexR.size());
    return countRow;
}

int  MainWindow::countSameColorCol(int index)
{
    int col=index%10;
    int countCol=0;
    int currcolor=vb[vg[index]->getBallIndex()]->getColor(); //当前小球的颜色
    if(index==col)                                        //index恰好是上边缘索引
    {
        for(int i=index+10;i<=90+col; i+=10)
        {
            if(vg[i]->getIsEmpty()||(!vg[i]->getIsEmpty()&&
                                     vb[vg[i]->getBallIndex()]->getColor()!=currcolor))
            {
                countCol=(i-index)/10;
                for(int j=index;j<i;j+=10)
                {
                    vsameindexC.push_back(j);
                }
                break;
            }
            else if(i==90+col)                //从index到index所在列的边缘都为同色球的情况
            {
                countCol=(i-index)/10+1;
                for(int j=index;j<=i;j+=10)
                {
                    vsameindexC.push_back(j);
                }
            }
        }
    }
    else if(index==col+90)                          //小球恰好是下边缘索引
    {
        for(int i=index-10;i>=col;i-=10)
        {
            if(vg[i]->getIsEmpty()||(!vg[i]->getIsEmpty()&&
                                     vb[vg[i]->getBallIndex()]->getColor()!=currcolor))
            {
                countCol=(index-i)/10;
                for(int j=index;j>i;j-=10)
                {
                    vsameindexC.push_back(j);
                }
                break;
            }
            else if(i==col)
            {
                countCol=(index-i)/10+1;
                for(int j=index;j>=i;j-=10)
                {
                    vsameindexC.push_back(j);
                }
            }
        }
    }

    else
    {
        for(int i=index+10;i<=90+col; i+=10)
        {
            if(vg[i]->getIsEmpty()||(!vg[i]->getIsEmpty()&&
                                     vb[vg[i]->getBallIndex()]->getColor()!=currcolor))
            {
                countCol+=(i-index)/10;
                for(int j=index;j<i;j+=10)
                {
                    vsameindexC.push_back(j);
                }
                break;
            }
            else if(i==90+col)
            {
                countCol+=(i-index)/10+1;
                for(int j=index;j<=i;j+=10)
                {
                    vsameindexC.push_back(j);
                }
            }
        }
        for(int i=index-10;i>=col;i-=10)
        {
            if(vg[i]->getIsEmpty()||(!vg[i]->getIsEmpty()&&
                                     vb[vg[i]->getBallIndex()]->getColor()!=currcolor))
            {
                countCol+=(index-i)/10;
                for(int j=index-10;j>i;j-=10)
                {
                    vsameindexC.push_back(j);
                }
                break;
            }
            else if(i==col)
            {
                countCol+=(index-i)/10+1;
                for(int j=index-10;j>=i;j-=10)
                {
                    vsameindexC.push_back(j);
                }
            }
        }

        countCol-=1;
    }
   // qDebug("countCol=%d",countCol);
   // qDebug("vsameC.size=%d",vsameindexC.size());
    return countCol;
}

void MainWindow::countSameColorOblique(int index, ObliqueDirection odir)
{
    int offset;
    int min=index;
    int max=index;
    int currcolor=vb[vg[index]->getBallIndex()]->getColor(); //当前小球的颜色
    if(odir==LEFT)
    {
        offset=10-1;
        if(LboundaryMaxSet.count(index)==1)
        {
            while(LboundaryMinSet.count(min)!=1)
            {
                min-=offset;
            }
        }
        else if(LboundaryMinSet.count(index)==1)
        {
            while(LboundaryMaxSet.count(max)!=1)
            {
                max+=offset;
            }
        }
        else
        {
            while(boundarySet.count(min)!=1)
            {
                min-=offset;
            }
            while(boundarySet.count(max)!=1)
            {
                max+=offset;
            }
        }
    }
    else if(odir==RIGHT)
    {
        offset=10+1;
        if(RboundaryMaxSet.count(index)==1)
        {
            while(RboundaryMinSet.count(min)!=1)
            {
                min-=offset;
            }
        }
        else if(RboundaryMinSet.count(index)==1)
        {
            while(RboundaryMaxSet.count(max)!=1)
            {
                max+=offset;
            }
        }
        else
        {
            while(boundarySet.count(min)!=1)
            {
                min-=offset;
            }
            while(boundarySet.count(max)!=1)
            {
                max+=offset;
            }
        }
    }


    if(index==min && index!=max)
    {
        for(int i=index+offset;i<=max;i+=offset)
        {
            if(vg[i]->getIsEmpty()||(!vg[i]->getIsEmpty()&&
                                     vb[vg[i]->getBallIndex()]->getColor()!=currcolor))
            {
                for(int j=index;j<i;j+=offset)
                {
                    if(odir==LEFT)
                        vsameindexLb.push_back(j);
                    else if(odir==RIGHT)
                        vsameindexRb.push_back(j);
                }
                break;
            }
            else if(i==max)
            {
                for(int j=index;j<=max;j+=offset)
                {
                    if(odir==LEFT)
                        vsameindexLb.push_back(j);
                    else if(odir==RIGHT)
                        vsameindexRb.push_back(j);
                }
            }
        }
    }
    else if(index==max && index!=min)
    {
        for(int i=index-offset;i>=min;i-=offset)
        {
            if(vg[i]->getIsEmpty()||(!vg[i]->getIsEmpty()&&
                                     vb[vg[i]->getBallIndex()]->getColor()!=currcolor))
            {
                for(int j=index;j>i;j-=offset)
                {
                    if(odir==LEFT)
                        vsameindexLb.push_back(j);
                    else if(odir==RIGHT)
                        vsameindexRb.push_back(j);
                }
                break;
            }
            else if(i==min)
                for(int j=index;j>=min;j-=offset)
                {
                    if(odir==LEFT)
                        vsameindexLb.push_back(j);
                    else if(odir==RIGHT)
                        vsameindexRb.push_back(j);
                }

        }
    }
    else if(index!=min && index!=max)
    {
        for(int i=index-offset;i>=min;i-=offset)
        {
            if(vg[i]->getIsEmpty()||(!vg[i]->getIsEmpty()&&
                                     vb[vg[i]->getBallIndex()]->getColor()!=currcolor))
            {
                for(int j=index;j>i;j-=offset)
                {
                    if(odir==LEFT)
                        vsameindexLb.push_back(j);
                    else if(odir==RIGHT)
                        vsameindexRb.push_back(j);
                }
                break;
            }
            else if(i==min)
                for(int j=index;j>=min;j-=offset)
                {
                    if(odir==LEFT)
                        vsameindexLb.push_back(j);
                    else if(odir==RIGHT)
                        vsameindexRb.push_back(j);
                }

        }
        for(int i=index+offset;i<=max;i+=offset)
        {
            if(vg[i]->getIsEmpty()||(!vg[i]->getIsEmpty()&&
                                     vb[vg[i]->getBallIndex()]->getColor()!=currcolor))
            {
                for(int j=index+offset;j<i;j+=offset)
                {
                    if(odir==LEFT)
                        vsameindexLb.push_back(j);
                    else if(odir==RIGHT)
                        vsameindexRb.push_back(j);
                }
                break;
            }
            else if(i==max)
            {
                for(int j=index+offset;j<=max;j+=offset)
                {
                    if(odir==LEFT)
                        vsameindexLb.push_back(j);
                    else if(odir==RIGHT)
                        vsameindexRb.push_back(j);
                }
            }
        }
    }
    if(odir==LEFT)
    qDebug("Lb.size=%d",vsameindexLb.size());
    else
    qDebug("Rb.size=%d",vsameindexRb.size());

}

void MainWindow::produceDiffrentNumber()
{
    int temp=0;
    randIndex[0]=produceRandomNumber(100);
    temp=produceRandomNumber(100);
    while(temp==randIndex[0])
    {
          temp=produceRandomNumber(100);
    }
    randIndex[1]=temp;
    temp=produceRandomNumber(100);
    while(temp==randIndex[0]||temp==randIndex[1])
    {
        temp=produceRandomNumber(100);
    }
    randIndex[2]=temp;
}
int MainWindow::randNoRepeatLoop()
{
    int temp=0;
    int loop=true;
    int breakloop=true;
    temp=produceRandomNumber(100);
    while(loop)
    {
        //qDebug("vindex.size=%d",vindex.size());
        for(int i=0;i<vindex.size();i++)
        {
            if(temp==vindex[i])
            {
                breakloop=false;
               // break;             //一旦找到重复的就跳出for循环 减少遍历次数
            }                       //因为之前的index都是不相同的

        }
        if(breakloop)              //没有与已有小球的方格索引重复，则跳出while循环
            loop=false;
        else                          //否则继续生成随机数
        {
            temp=produceRandomNumber(100);
        }
        breakloop=true;

    }
    return temp;

}

void MainWindow::produceSpecialNumber()
{

    for(int i=0;i<3;i++)
    {
        randIndexWithoutBall[i]=randNoRepeatLoop();
        vg[randIndexWithoutBall[i]]->setIsEmpty(false);
        vindex.push_back(randIndexWithoutBall[i]);     //添加一个新的含有小球的方格索引
    }
}

void MainWindow::produceNewBalls()
{
    for(int i=0;i<3;i++)
    {
        m_ball=new Ball();
        if(randIndexWithoutBall)
            m_ball->setIndexInGird(randIndexWithoutBall[i]);
        vb.push_back(m_ball);

    }

}

void MainWindow::updateBallIndexOfGird(int deleteballindex)
{
    int lastBallIndex;
    for(int i=0;i<100;i++)
    {
        if(!vg[i]->getIsEmpty()&&vg[i]->getBallIndex()>deleteballindex)
        {
            lastBallIndex=vg[i]->getBallIndex();
            vg[i]->setBallIndex(lastBallIndex-1);
        }
    }
}

void MainWindow::deleteSameColorBalls(int targetindex)
{
    int ballindex=0;
    std::vector<Ball*>::iterator it;
    if(vsameindexR.size()>=5)
    {
        for(int i=0;i<vsameindexR.size();i++)
        {
            if(vsameindexR[i]!=targetindex)
            {
                ballindex=vg[vsameindexR[i]]->getBallIndex();
                vg[vsameindexR[i]]->setIsEmpty(true);
                vit=std::find(vindex.begin(),vindex.end(),vsameindexR[i]);
                it=std::find(vb.begin(),vb.end(),vb[ballindex]);
                delete *it;
                vb.erase(it);
                updateBallIndexOfGird(ballindex);
                vindex.erase(vit);
            }
        }

    }
    if(vsameindexC.size()>=5)
    {

        for(int i=0;i<vsameindexC.size();i++)
        {
            if(vsameindexC[i]!=targetindex)
            {
                ballindex=vg[vsameindexC[i]]->getBallIndex();
                vg[vsameindexC[i]]->setIsEmpty(true);
                vit=std::find(vindex.begin(),vindex.end(),vsameindexC[i]);
                it=std::find(vb.begin(),vb.end(),vb[ballindex]);
                delete *it;
                vb.erase(it);
                updateBallIndexOfGird(ballindex);
                vindex.erase(vit);
            }
        }

    }
    if(vsameindexLb.size()>=5)
    {
        for(int i=0;i<vsameindexLb.size();i++)
        {
            if(vsameindexLb[i]!=targetindex)
            {
                ballindex=vg[vsameindexLb[i]]->getBallIndex();
                vg[vsameindexLb[i]]->setIsEmpty(true);
                vit=std::find(vindex.begin(),vindex.end(),vsameindexLb[i]);
                it=std::find(vb.begin(),vb.end(),vb[ballindex]);
                delete *it;
                vb.erase(it);
                updateBallIndexOfGird(ballindex);
                vindex.erase(vit);
            }
        }

    }
    if(vsameindexRb.size()>=5)
    {
        for(int i=0;i<vsameindexRb.size();i++)
        {
            if(vsameindexRb[i]!=targetindex)
            {
                ballindex=vg[vsameindexRb[i]]->getBallIndex();
                vg[vsameindexRb[i]]->setIsEmpty(true);
                vit=std::find(vindex.begin(),vindex.end(),vsameindexRb[i]);
                it=std::find(vb.begin(),vb.end(),vb[ballindex]);
                delete *it;
                vb.erase(it);
                updateBallIndexOfGird(ballindex);
                vindex.erase(vit);
            }
        }
    }
    ballindex=vg[targetindex]->getBallIndex();
    vg[targetindex]->setIsEmpty(true);
    vit=std::find(vindex.begin(),vindex.end(),targetindex);
    it=std::find(vb.begin(),vb.end(),vb[ballindex]);
    delete *it;
    vb.erase(it);
    updateBallIndexOfGird(ballindex);
    vindex.erase(vit);
}

void MainWindow::Scoring(int countdir,int countballs)
{
    countballs=countballs-countdir+1;   //实际消去的小球个数
    qDebug("countballs=%d",countballs);
    switch(countballs)
    {
    case 5:
        m_Score+=countballs*10;
        break;
    case 6:
        m_Score+=countballs*12;
        break;
    case 7:
        m_Score+=countballs*15;
        break;
    case 8:
        m_Score+=countballs*20;
        break;
    case 9:
        m_Score+=countballs*30;
        break;
    case 10:
        m_Score+=countballs*50;
        break;
    default:
        m_Score+=countballs*100;
    }
    ui->lcdNumber->setProperty("intValue", QVariant(m_Score));

}

void MainWindow::decideMoveEnabled(int select, int target)
{
    int diffrence=0;
    if(select>target)
    {
        diffrence=select-target;
        if(diffrence%11==0)          //目标为右斜往上
        {
            m_MoveEnable=true;
            for(int i=select-11;i>=target;i-=11)
                if(!vg[i]->getIsEmpty())
                    m_MoveEnable=false;
        }
        else if(diffrence%10==0)    //目标为纵向往上
        {
            m_MoveEnable=true;
            for(int i=select-10;i>=target;i-=10)
                if(!vg[i]->getIsEmpty())
                    m_MoveEnable=false;
        }
        else if(diffrence%9==0)
        {
            m_MoveEnable=true;
//            if(boundarySet.count(select)==1)   //select在边界 目标为横向往左
//            {
//                for(int i=select-1;i>=target;i--)
//                    if(!vg[i]->getIsEmpty())
//                        m_MoveEnable=false;
//            }

                for(int i=select-9;i>=target;i-=9) //目标为左斜往上
                    if(!vg[i]->getIsEmpty())
                        m_MoveEnable=false;

        }
        else if(diffrence<9&&select/10==target/10)  //目标为横向往左
        {
            m_MoveEnable=true;
            for(int i=select-1;i>=target;i--)
                if(!vg[i]->getIsEmpty())
                    m_MoveEnable=false;
        }

    }
    else if(select<target)
    {
        diffrence=target-select;
        if(diffrence%11==0)           //目标为右斜往下
        {
            m_MoveEnable=true;
            for(int i=select+11;i<=target;i+=11)
                if(!vg[i]->getIsEmpty())
                    m_MoveEnable=false;
        }
        else if(diffrence%10==0)     //目标为纵向往下
        {
            m_MoveEnable=true;
            for(int i=select+10;i<=target;i+=10)
                if(!vg[i]->getIsEmpty())
                    m_MoveEnable=false;
        }
        else if(diffrence%9==0)
        {
            m_MoveEnable=true;
//            if(boundarySet.count(select)==1)     //目标为横向往右
//            {
//                for(int i=select+1;i<=target;i++)
//                    if(!vg[i]->getIsEmpty())
//                        m_MoveEnable=false;
//            }
                //目标为左斜往下

                for(int i=select+9;i<=target;i+=9)
                    if(!vg[i]->getIsEmpty())
                        m_MoveEnable=false;

        }
        else if(diffrence<9&&select/10==target/10)  //目标为横向往右
        {
            m_MoveEnable=true;
            for(int i=select+1;i<=target;i++)
                if(!vg[i]->getIsEmpty())
                    m_MoveEnable=false;
        }
    }
}

void MainWindow::gameMainLogic(int targetindex)
{

    int countdiretions=0;
    int countSameBalls=0;
    countSameColorRow(targetindex);
    countSameColorCol(targetindex);
    countSameColorOblique(targetindex,LEFT);
    countSameColorOblique(targetindex,RIGHT);

    if(vsameindexR.size()>=5)
    {
        countdiretions++;
        countSameBalls+=vsameindexR.size();
    }
    if(vsameindexC.size()>=5)
    {
        countdiretions++;
        countSameBalls+=vsameindexC.size();
    }
    if(vsameindexLb.size()>=5)
    {
        countdiretions++;
        countSameBalls+=vsameindexLb.size();
    }
    if(vsameindexRb.size()>=5)
    {
        countdiretions++;
        countSameBalls+=vsameindexRb.size();
    }
    if(countdiretions>=1)
    {
        Scoring(countdiretions,countSameBalls);
        deleteSameColorBalls(targetindex);

    }
    vsameindexR.clear();
    vsameindexC.clear();
    vsameindexLb.clear();
    vsameindexRb.clear();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QPoint p(event->x(),event->y());

    for(int i=0;i<100;i++)
    {
        if(vg[i]->containsTouchPoint(p)&&!vg[i]->getIsEmpty()&&
                m_Touchenabled)  //selected
        {
            drawRect=true;
            selected=true;
            selectIndex=i;
           // qDebug("x=%d y=%d i=%d",p.x(),p.y(),i);
            update();
        }
        else if(vg[i]->containsTouchPoint(p)&&vg[i]->getIsEmpty()&&
                selected&&m_Touchenabled)
        {
            decideMoveEnabled(selectIndex,i);
            if(m_MoveEnable)
            {
                vg[selectIndex]->setIsEmpty(true);//将要移出小球的方格设置为空
                vg[i]->setIsEmpty(false);        //将要移入小球的方格设置不为空
                //vindex.find()
                vit=std::find(vindex.begin(),vindex.end(),selectIndex);
                if(vit!=vindex.end())
                    *vit=i;                      //更新含有小球的方格索引
                moveToIndex=i;                   //保存目标方格索引
                vb[vg[selectIndex]->getBallIndex()]->setIsSelected(true);//小球设为选中状态
                drawRect=false;        //移动结束选中矩形消失
                selected=false;
                produceSpecialNumber();
                produceNewBalls();   //每次移动后添加三个小球
                update(); //触发paintEvent事件

                QTime t;
                t.start();
                while(t.elapsed()<500)
                {

                    QCoreApplication::processEvents();
                }

                gameMainLogic(moveToIndex);
                update();

                for(int i=0;i<3;i++)          //新生成的三个小球逻辑处理
                {
                    gameMainLogic(randIndexWithoutBall[i]);
                    update();
                }
            }
            else
                QMessageBox::information(this,tr("GameRule"),tr("Can't move!"));
        }

     }
    if(vindex.size()>=97&&m_Touchenabled)
    {
        QMessageBox::information(this,tr("GameOver"),tr("Please restart!"));
        m_Touchenabled=false;
    }
    m_MoveEnable=false;
}


void MainWindow::drawBalls()
{
    //qDebug("vb.size=%d",vb.size());
    for(int i=0;i<vb.size();i++)
    {
          if(!vb[i]->getIsSelected())
          {
             vb[i]->beenDrawed(*vg[vb[i]->getIndexInGird()]->getRect(),paint);
             vg[vb[i]->getIndexInGird()]->setBallIndex(i);
          }
          else
          {
              vb[i]->beenDrawed(*(vg[moveToIndex]->getRect()),paint);
              vg[moveToIndex]->setBallIndex(i); //保存方格内小球的索引号
              vb[i]->setIsSelected(false);  //移动结束小球设为初始未选择状态
              vb[i]->setIndexInGird(moveToIndex);    //保存小球的当前位置
          }

    }


}

void MainWindow::drawSelectRect(int index)
{
    paint->setBrush(Qt::NoBrush);
    paint->setPen(QPen(Qt::darkRed,2,Qt::SolidLine));
    paint->drawRect(vg[index]->getRect()->topLeft().x()+3
                    ,vg[index]->getRect()->topLeft().y()+3,WIDTH-6,HEIGHT-6);
}
void MainWindow::drawChessBoard()
{
    paint->setPen(Qt::black);
    for(int i=0;i<11;i++)
    {
        paint->drawLine(XBEGIN,YBEGIN+40*i,600,YBEGIN+40*i);
    }
    for(int i=0;i<11;i++)
    {
        paint->drawLine(XBEGIN+40*i,YBEGIN,XBEGIN+40*i,530);
    }
}

void MainWindow::paintEvent(QPaintEvent *)
{
    paint=new QPainter;
    paint->begin(this);
    drawChessBoard();
    drawBalls();


    if(drawRect)
        drawSelectRect(selectIndex);
    paint->end();
    //qDebug("paint=%x",paint);

}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionIntroduce_triggered()
{
   helpDialog->show();
}

void MainWindow::on_actionRestart_triggered()
{
    m_Touchenabled=true;
    int ballindex=0;
    std::vector<Ball*>::iterator it;
    for(int i=0;i<100;i++)
    {
        bool flag=vg[i]->getIsEmpty();
        if(!flag)
        {
            ballindex=vg[i]->getBallIndex();
            vg[i]->setIsEmpty(true);
            qDebug("ballindex=%d i=%d",ballindex,i);
            vit=std::find(vindex.begin(),vindex.end(),i);
            it=std::find(vb.begin(),vb.end(),vb[ballindex]);
            delete *it;
            vb.erase(it);
            updateBallIndexOfGird(ballindex);
            vindex.erase(vit);
        }
    }
    qDebug("vindex.size=%d",vindex.size());

    for(int i=0;i<3;i++)
    {
        m_ball=new Ball();
        m_ball->setIndexInGird(randIndex[i]);
        vb.push_back(m_ball);
        vindex.push_back(randIndex[i]);       //将含有小球的方格索引存储到索引数组
       // qDebug("vindex.size=%d",vindex.size());
        vg[randIndex[i]]->setIsEmpty(false);

    }

    saveHighRecord();
    qDebug("vindex.size=%d",vindex.size());
    m_Score=0;
    ui->lcdNumber->setProperty("intValue", QVariant(m_Score));
    update();

}

void MainWindow::on_actionBillboard_triggered()
{
    loadHighRecord();
    highRecord->show();
}

void MainWindow::on_actionSave_triggered()
{
    saveGameDate();
    QMessageBox::information(this,tr("GameSaving"),tr("Successful!"));
}
