#ifndef SETTING_H
#define SETTING_H

#include <QDialog>

namespace Ui {
class setting;
}

class setting : public QDialog
{
    Q_OBJECT

public:
    explicit setting(QWidget *parent = 0);
    ~setting();

private slots:
    void on_save_setting_pushButton_clicked();

    void on_cancel_setting_pushButton_clicked();

private:
    Ui::setting *ui;
};

#endif // SETTING_H
