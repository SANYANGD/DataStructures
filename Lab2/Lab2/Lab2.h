#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Lab2.h"

class Lab2 : public QMainWindow
{
    Q_OBJECT

public:
    Lab2(QWidget *parent = Q_NULLPTR);

private:
    Ui::Lab2Class ui;
};
