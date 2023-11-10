#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QSignalMapper"
#include "QListWidgetItem"
#include "QVBoxLayout"
#include "QPushButton"
#include "QMessageBox"
#include "QLabel"
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
    void show_pin_check_window();//Функция показа окна для ввода pincode
    unsigned char *key = (unsigned char *)"81dc9bdb52d04dc20036dbd8313ed055";
    unsigned char *iv = (unsigned char *)"0123456789012345";
    QString decrypt_file(QByteArray encrypt_data);
    int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,unsigned char *iv, unsigned char *decryptext);
    int crypt_error(void);
    QVector<QVector<QString>> list_of_account;

    int show_bank_window();
private slots:
    int check_pin_code();



private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
