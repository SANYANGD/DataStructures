#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSizePolicy>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_input = ui->input;
    m_suffix = ui->suffix;

    QGridLayout *kbdLayout = new QGridLayout();
    ui->keyboard->setLayout(kbdLayout);

    // Funtion button
    QPushButton *ceBtn = new QPushButton("CE");
    QPushButton *lbBtn = new QPushButton("(");
    QPushButton *rbBtn = new QPushButton(")");
    ceBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    lbBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    rbBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(ceBtn, &QPushButton::pressed, [=] { m_input->setText(""); m_suffix->setText(""); });
    connect(lbBtn, &QPushButton::pressed, [=] { m_input->setText(m_input->text().append("(")); });
    connect(rbBtn, &QPushButton::pressed, [=] { m_input->setText(m_input->text().append(")")); });
    kbdLayout->addWidget(ceBtn, 0, 0, 1, 1);
    kbdLayout->addWidget(lbBtn, 0, 1, 1, 1);
    kbdLayout->addWidget(rbBtn, 0, 2, 1, 1);

    // Setup number keys
    for (int i = 0; i < 9; ++i)
    {
        QPushButton *button = new QPushButton(QString::number(i + 1));
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        connect(button, &QPushButton::pressed, [=] {
            QString text = m_input->text();
            m_input->setText(text.append(QString::number(i + 1)));
        });
        kbdLayout->addWidget(button, i/3 + 1, i%3, 1, 1);
    }
    QPushButton *zeroBtn = new QPushButton("0");
    zeroBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(zeroBtn, &QPushButton::pressed, [=] {
        QString text = m_input->text();
        m_input->setText(text.append("0"));
    });
    kbdLayout->addWidget(zeroBtn, 4, 1, 1, 1);

    // DOT
    QPushButton *dotBtn = new QPushButton(".");
    dotBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(dotBtn, &QPushButton::pressed, [=]{
        QString text = m_input->text();
        m_input->setText(text.append("."));
    });
    kbdLayout->addWidget(dotBtn, 4, 2, 1, 1);


    // Setup symbols
    const char symbols[] = {'+', '-', '*', '/'};

    for (size_t i = 0; i < sizeof(symbols); ++i)
    {
        QPushButton *button = new QPushButton(QString(symbols[i]));
        button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        connect(button, &QPushButton::pressed, [=] {
            QString text = m_input->text();
            m_input->setText(text.append(QString(symbols[i])));
        });
        kbdLayout->addWidget(button, i ,4, 1, 1);
    }

    // Result
    QPushButton *eqlBtn = new QPushButton("=");
    eqlBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(eqlBtn, &QPushButton::pressed, [=]{
        Stack<Token*> *suffix = ExprConv::toSuffix(m_input->text().toStdString());
        m_suffix->setText("");

        try {
            //  输出结果
            for (size_t i = 0; i < suffix->size(); ++i) {
                if (suffix->at(i)->isNumber() || suffix->at(i)->isOperator()) {
                    auto x = suffix->at(i)->getData();
                    m_suffix->setText(m_suffix->text().append(x + " "));
                }
            }

            QString result = QString::number(ExprConv::calculateSuffix(suffix));
            m_input->setText(result);
        }  catch (QString err) {
            m_suffix->setText(err);
        }
    });
    kbdLayout->addWidget(eqlBtn, 4, 4, 1, 1);
}

MainWindow::~MainWindow()
{
    delete ui;
}
