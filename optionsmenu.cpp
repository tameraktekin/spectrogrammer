#include <iostream>
#include "optionsmenu.h"
#include "ui_optionsmenu.h"

using namespace std;

OptionsMenu::OptionsMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsMenu)
{
    ui->setupUi(this);

    fftLen = 0;
    windowType = "";
}

OptionsMenu::~OptionsMenu()
{
    delete ui;
}

int OptionsMenu::getFftLen()
{
    return this->fftLen;
}

QString OptionsMenu::getWindowType()
{
    return this->windowType;
}

void OptionsMenu::on_fftLenBox_activated(const QString &arg1)
{
    this->fftLen = arg1.toInt();
}

void OptionsMenu::on_comboBox_2_activated(const QString &arg1)
{
    this->windowType = arg1;
}
