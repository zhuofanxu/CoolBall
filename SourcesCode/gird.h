#ifndef GIRD_H
#define GIRD_H
#include<qrect.h>
#include"utility.h"
//棋盘方格类
class Gird
{
public:
    Gird(int index);  //参数为方格所在方格数组的索引,主要用于确定方格矩形左上角点
    ~Gird();
    bool containsTouchPoint(QPoint& p); //检测点击点是否在方格内
    inline bool getIsEmpty(){return m_isEmpty;}
    inline void setIsEmpty(bool empty){m_isEmpty=empty;}
    inline QRect* getRect(){return m_rect;}
    inline int  getBallIndex(){return m_index;}
    inline void setBallIndex(int index){m_index=index;}
private:
    bool   m_isEmpty;           //是否有小球
    QRect  *m_rect;            //方格所属矩形
    QPoint *m_topLeft;         //方格所属矩形的左上角点
    int    m_index;            //方格拥有小球的编号


};
#endif    // GIRD_H
