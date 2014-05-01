#include "highrecord.h"
#include "ui_highrecord.h"

HighRecord::HighRecord(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HighRecord),
    m_HighScore(0)
{
    ui->setupUi(this);
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background, Qt::darkGreen);
    this->setPalette(palette);
   //  ui->lcdNumber1->setProperty("intValue", QVariant(100));
}

HighRecord::~HighRecord()
{
    delete ui;
}

void HighRecord::setHighScore(int score,int index)
{
    m_HighScore=score;
    if(index==0)
        ui->lcdNumber1->setProperty("intValue", QVariant(m_HighScore));
    else if(index==1)
        ui->lcdNumber2->setProperty("intValue", QVariant(m_HighScore));
    else if(index==2)
        ui->lcdNumber3->setProperty("intValue", QVariant(m_HighScore));
}

void HighRecord::on_pushButton_clicked()
{
    this->hide();
}
