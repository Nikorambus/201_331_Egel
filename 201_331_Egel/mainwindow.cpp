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

int MainWindow::show_bank_window() {
    QFile encrypt_file;
    encrypt_file.setFileName("D:/GitExam/201_331_Egel/data.json");
    encrypt_file.open(QIODevice::ReadOnly | QIODevice::Text);

    if (!encrypt_file.isOpen()) {
        qDebug() << "Ошибка в открытии исходного файла";
        return 0;
    }
    QString encrypt_data = encrypt_file.readAll();

    QString decrypt_data = decrypt_file(encrypt_data.toUtf8());

    int lastIndex = decrypt_data.lastIndexOf('}');

    // Обрезаем строку после последнего символа новой строки
    QString trimdecrypt_data = decrypt_data.left(lastIndex);

    trimdecrypt_data[trimdecrypt_data.length() - 1] = '\n';
    trimdecrypt_data[trimdecrypt_data.length() - 1] = '}';

    qDebug() << "Расшифрованный файл: " + trimdecrypt_data;


    QByteArray temp = trimdecrypt_data.toUtf8();
    qDebug() << temp.data();

    QJsonDocument jsonDocument = QJsonDocument::fromJson(temp);


    QJsonObject jsonObject = jsonDocument.object();
    int i = 0;
    for (auto it = jsonObject.begin(); it != jsonObject.end(); ++it) {
        // Получаем подобъект для каждого аккаунта
        QJsonObject accountObject = it.value().toObject();


        // Извлекаем информацию
        QString id = accountObject.value("id").toString();
        int sum = accountObject.value("sum").toInt();
        QString date = accountObject.value("date").toString();

        // Выводим информацию
        qDebug() << "ID:" << id << "Sum:" << sum << "Date:" << date;

        QVector<QString> temp;
        temp.push_back(id);
        temp.push_back(QString::number(sum));
        temp.push_back(date);

        list_of_account.push_back(temp);
    }

    ui->label_account->setText(list_of_account[0][0]);
    ui->label_sum->setText(list_of_account[0][1]);
    ui->label_date->setText(list_of_account[0][2]);
    return 0;
}
