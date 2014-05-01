#ifndef BALL_H
#define BALL_H
#include<qrect.h>
#include<QPainter>
//小球的五种颜色
enum BallColor
{
    black,
    red,
    green,
    blue,
    gray
};

//小球类
class Ball
{
public:
    Ball();
    Ball(bool load);
    ~Ball();
    void beenDrawed(QRect& rect,QPainter* paint);//小球被绘制 参数方格所在矩形和绘画类指针
   // void beenSelected();                       //小球被玩家选择
    inline int  getIsSelected(){return m_isSelected;}
    inline void setIsSelected(bool select){m_isSelected=select;}
    inline int  getIndexInGird(){return m_indexInGird;}
    inline void setIndexInGird(int index){m_indexInGird=index;}
    inline void setColor(int color){m_color=color;}
    inline int  getColor(){return m_color;}
private:
    int       m_color;       //小球颜色
    int       m_indexInGird;       //小球所在方格索引
    bool      m_isSelected;

};

#endif // BALL_H
