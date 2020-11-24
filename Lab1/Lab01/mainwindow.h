#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "exprconv.h"

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    inline QString getText() { return m_input->text().trimmed().replace(" ", ""); }

private:
    Ui::MainWindow *ui;
    QLineEdit      *m_input;
    QLineEdit      *m_suffix;
    ExprConv       *m_conv;
};
#endif // MAINWINDOW_H
