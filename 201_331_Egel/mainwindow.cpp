#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QPushButton"
#include "QMessageBox"
#include "QRandomGenerator"

#include "QByteArray"
#include "QFile"
#include "QJsonDocument"
#include "QJsonObject"
#include <openssl/evp.h>
#include "QDebug"
#include "QByteArray"
#include "QBuffer"
#include "QFile"
#include "QJsonParseError"
#include "QJsonObject"
#include "QJsonArray"
#include "QDir"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::show_pin_check_window() {

    pin_window = new QWidget(); //Создание виджета как окна для ввода пин кода
    pin_window->setGeometry(683,350,270,120);

    QVBoxLayout *layout_pin = new QVBoxLayout;

    QLabel *pin_code_label = new QLabel("Пин код"); //Лейбл для пинкода
    input_pincode = new QLineEdit(); // Инпут для пинкода
    input_pincode->setEchoMode(QLineEdit::Password);

    QPushButton *check_pin_btn = new QPushButton("Вход"); //Кнопка для проверки
    connect(check_pin_btn, SIGNAL(clicked()), this, SLOT(check_pin_code())); //При нажатии на кнопку отправляемся в функцию проверки

    //Добавляем в окно лейбл кнопку и инпут
    layout_pin->addWidget(pin_code_label);
    layout_pin->addWidget(input_pincode);
    layout_pin->addWidget(check_pin_btn);
    pin_window->setLayout(layout_pin);

    pin_window->show();
}

int MainWindow::check_pin_code() {
    const QString orig_code = "1234";
    QString code = input_pincode->text();

    if (orig_code == code) {
        delete[] pin_window;
        this->show_bank_window(); //запускаем основное окно
        this->show();
        return 0;

    }
    else {
        QMessageBox::critical(NULL,QObject::tr("Ошибка"),tr("Ошибка ввода пин-кода"));
        return 0;
    }
}
