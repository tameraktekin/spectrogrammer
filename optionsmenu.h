#ifndef OPTIONSMENU_H
#define OPTIONSMENU_H

#include <QDialog>

namespace Ui {
class OptionsMenu;
}

class OptionsMenu : public QDialog
{
    Q_OBJECT

public:
    explicit OptionsMenu(QWidget *parent = nullptr);
    ~OptionsMenu();

    int getFftLen();
    QString getWindowType();

private slots:
    void on_fftLenBox_activated(const QString &arg1);

    void on_comboBox_2_activated(const QString &arg1);

private:
    Ui::OptionsMenu *ui;
    int fftLen;
    QString windowType;

};

#endif // OPTIONSMENU_H
