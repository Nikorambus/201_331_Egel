#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QPushButton"
#include "QMessageBox"
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
    //Открываем файл на чтение
    QFile encrypt_file;
    encrypt_file.setFileName("D:/GitExam/201_331_Egel/data.json");
    encrypt_file.open(QIODevice::ReadOnly | QIODevice::Text);

    if (!encrypt_file.isOpen()) {
        qDebug() << "Ошибка в открытии исходного файла";
        return 0;
    }

    //Считываем зашифрованные данные
    QString encrypt_data = encrypt_file.readAll();

    //Расшифровываем данные
    QString decrypt_data = decrypt_file(encrypt_data.toUtf8());

    //Подготавливаем для формата json
    int lastIndex = decrypt_data.lastIndexOf('}');
    QString trimdecrypt_data = decrypt_data.left(lastIndex);
    trimdecrypt_data[trimdecrypt_data.length() - 1] = '\n';
    trimdecrypt_data[trimdecrypt_data.length() - 1] = '}';

    //Вывод
    qDebug() << "Расшифрованный файл: " + trimdecrypt_data;

    //Переводим данные в байты
    QByteArray temp = trimdecrypt_data.toUtf8();
    qDebug() << temp.data();

    //Переводим данные в json
    QJsonDocument jsonDocument = QJsonDocument::fromJson(temp);

    QJsonObject jsonObject = jsonDocument.object();
    for (auto it = jsonObject.begin(); it != jsonObject.end(); ++it) {
        // Получаем подобъект для каждого аккаунта
        QJsonObject accountObject = it.value().toObject();


        // Извлекаем информацию
        QString id = accountObject.value("id").toString();
        int sum = accountObject.value("sum").toInt();
        QString date = accountObject.value("date").toString();

        // Выводим информацию
        qDebug() << "ID:" << id << "Sum:" << sum << "Date:" << date;

        //Добавляем информацию в общий лист (сначала создаем темповые вектор)
        QVector<QString> temp;
        temp.push_back(id);
        temp.push_back(QString::number(sum));
        temp.push_back(date);

        //Добавление в лист
        list_of_account.push_back(temp);
    }

    //Отрисовываем текущий счет
    paint_data(number_of_account);
    return 0;
}

int MainWindow::paint_data(int number_of_account)
{
    //Красим в зависимости от карты
    if (number_of_account == 0) {
        this->setStyleSheet("background-color: brown;");
    }
    else if (number_of_account == 1) {
        this->setStyleSheet("background-color: #8732a8;");
    }
    else if (number_of_account == 2) {
        this->setStyleSheet("background-color: #327da8;");
    }

    //Вводим данные в окно
    ui->label_account->setText(list_of_account[number_of_account][0]);
    ui->label_sum->setText(list_of_account[number_of_account][1]);
    ui->label_date->setText(list_of_account[number_of_account][2]);

    return 0;
}

QString MainWindow::decrypt_file(QByteArray encrypt_data){
    //Расшифровка
    unsigned char decrypt_number[512];
    int decrypt_len = decrypt((unsigned char*)QByteArray::fromBase64(encrypt_data).data(), encrypt_data.length(),key,iv,decrypt_number);
    decrypt_number[decrypt_len] = '\0';
    QByteArray decrypt_bit =  QByteArray::fromRawData((const char*)decrypt_number, decrypt_len);

    return decrypt_bit.data();
}


int MainWindow::decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
                        unsigned char *iv, unsigned char *decryptext)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int decryptext_len;

    if(!(ctx = EVP_CIPHER_CTX_new()))
        crypt_error();


    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        crypt_error();


    if(1 != EVP_DecryptUpdate(ctx, decryptext, &len, ciphertext, ciphertext_len))
        crypt_error();
    decryptext_len = len;


    if(1 != EVP_DecryptFinal_ex(ctx, decryptext + len, &len))
        crypt_error();
    decryptext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return decryptext_len;
}


int MainWindow::crypt_error(void)
{
    return 1;
}


void MainWindow::on_pushButton_2_clicked()
{

    number_of_account+= 1;
    if (number_of_account == 3) {
        number_of_account = 0;
    }

    qDebug() << "number_of_account" << number_of_account;
    paint_data(number_of_account);

}

void MainWindow::on_pushButton_clicked()
{
    number_of_account-= 1;
    if (number_of_account == -1) {
        number_of_account = 2;
    }
    paint_data(number_of_account);
}

