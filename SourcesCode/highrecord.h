#ifndef HIGHRECORD_H
#define HIGHRECORD_H

#include <QDialog>

namespace Ui {
class HighRecord;
}

class HighRecord : public QDialog
{
    Q_OBJECT

public:
    explicit HighRecord(QWidget *parent = 0);
    ~HighRecord();
    void setHighScore(int score,int index);

private slots:
    void on_pushButton_clicked();

private:
    Ui::HighRecord *ui;
    int m_HighScore;
};

#endif // HIGHRECORD_H
