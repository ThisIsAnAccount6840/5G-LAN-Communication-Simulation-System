#include "error_window.h"

ERROR_WINDOW::ERROR_WINDOW(QWidget *parent) : QWidget(parent)
{
    this->setWindowTitle("Error");
    this->setFixedSize(400,100);
    prompt.setParent(this);
    prompt.move(50,40);
    prompt.setText("ERROR: This UE is not registered!");

}

ERROR_WINDOW::ERROR_WINDOW(QWidget *parent, QString text):QWidget(parent){
    this->setWindowTitle("Error");
    this->setFixedSize(400,100);
    prompt.setParent(this);
    prompt.move(50,40);
    prompt.setText(text);
}
