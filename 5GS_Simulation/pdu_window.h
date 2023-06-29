#ifndef PDU_WINDOW_H
#define PDU_WINDOW_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>

class PDU_Window : public QWidget
{
    Q_OBJECT
public:
    explicit PDU_Window(QWidget *parent = nullptr);
    QLabel DNN_Input_label;
    QLineEdit DNN_Input;
    QLabel S_NSSAI;
    QLabel SST_Input_label;
    QLineEdit SST_Input;
    QLabel SD_Input_label;
    QLineEdit SD_Input;
    QLabel member_label;
    QTextEdit member_id;
    QPushButton ok;

signals:

};

#endif // PDU_WINDOW_H
