#ifndef TAKEDIALOG_H
#define TAKEDIALOG_H

#include <QDialog>

struct Take{
    float hardness;
    float size;
};

namespace Ui {
class TakeDialog;
}

class TakeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TakeDialog(QWidget *parent = 0);
    ~TakeDialog();

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::TakeDialog *ui;
};

#endif // TAKEDIALOG_H
