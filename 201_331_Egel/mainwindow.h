#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QMessageBox"
#include "QLineEdit"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //Переменные для окна ввода пин кода
    QWidget *pin_window;
    QLineEdit *input_pincode;

    //Функция показа окна для ввода pincode
    void show_pin_check_window();

    //Данные для расшифровки
    unsigned char *key = (unsigned char *)"81dc9bdb52d04dc20036dbd8313ed055";
    unsigned char *iv = (unsigned char *)"0123456789012345";

    //Переменная которая хранит расшифрованные данные
    QString decrypt_file(QByteArray encrypt_data);

    //функции для расшифрования
    int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,unsigned char *iv, unsigned char *decryptext);
    int crypt_error(void);

    //Структура, хранящая в себе все банковские счета
    QVector<QVector<QString>> list_of_account;

    //Переменная, которая хранит номер текущего счета
    int number_of_account = 0;

private slots:


    //Функция которая проверяет пин код
    int check_pin_code();

    //Функция которая выводит основное окно
    int show_bank_window();

    //Функции отрисовки текущего счета
    int paint_data(int number_of_account);

    //Нажатие на кнопки
    void on_pushButton_2_clicked();
    void on_PushButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
