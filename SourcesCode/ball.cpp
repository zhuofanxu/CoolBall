#include "ball.h"
#include "utility.h"
#include <QPainter>

Ball::Ball()
{
    m_color=produceRandomNumber(5);
    m_isSelected=false;
}
Ball::Ball(bool load)
{
    if(load)
         m_isSelected=false;
}
Ball::~Ball()
{

}

void Ball::beenDrawed(QRect &rect,QPainter *paint)
{
    if(!paint)
        qDebug("QPainter init failed!");
    //设置画笔和填充颜色
    switch (m_color) {
    case black:
         paint->setPen(Qt::black);
        paint->setBrush(QBrush(Qt::black,Qt::SolidPattern));
        break;
    case red:
        paint->setPen(Qt::red);
        paint->setBrush(QBrush(Qt::red,Qt::SolidPattern));
        break;
    case green:
         paint->setPen(Qt::green);
        paint->setBrush(QBrush(Qt::green,Qt::SolidPattern));
        break;
    case blue:
         paint->setPen(Qt::blue);
        paint->setBrush(QBrush(Qt::blue,Qt::SolidPattern));
        break;
    case gray:
        paint->setPen(Qt::gray);
        paint->setBrush(QBrush(Qt::gray,Qt::SolidPattern));
        break;
    default:
        break;
    }

   paint->drawEllipse(rect.topLeft().x()+4,rect.topLeft().y()+4
                      ,WIDTH-8,HEIGHT-8);

}
