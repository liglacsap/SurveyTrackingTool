#include "takedialog.h"
#include "ui_takedialog.h"

TakeDialog::TakeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TakeDialog)
{
    ui->setupUi(this);
}

TakeDialog::~TakeDialog()
{
    delete ui;
}

void TakeDialog::on_pushButton_2_clicked()
{
    QString text = "Take ";
    text.append(QString::number(ui->listWidget->count()));
    ui->listWidget->addItem(text);
}
