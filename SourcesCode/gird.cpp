#include "gird.h"

Gird::Gird(int index):m_isEmpty(true),
    m_index(-1)
{
    int topx=(index%10)*WIDTH+XBEGIN;
    int topy=(index/10)*HEIGHT+YBEGIN;
    m_topLeft=new QPoint(topx,topy);
    m_rect=new QRect(*m_topLeft,QSize(WIDTH,HEIGHT));
}

Gird::~Gird()
{
    if(m_topLeft)
        delete m_topLeft;
    if(m_rect)
        delete m_rect;
}

bool Gird::containsTouchPoint(QPoint& p)
{
    if(p.x()>=m_rect->topLeft().x()&&p.x()<=m_rect->topRight().x())
    {
        if(p.y()>=m_rect->topLeft().y()&&p.y()<=m_rect->bottomLeft().y())
            return true;
        else
            return false;
    }
    else
        return false;
}
