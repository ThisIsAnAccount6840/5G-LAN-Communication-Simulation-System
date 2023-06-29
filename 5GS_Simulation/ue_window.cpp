#include "ue_window.h"

UE_Window::UE_Window(QWidget *parent, QString name) : QWidget(parent)
{
    this->setWindowTitle(name);

    this->resize(500,200);

    Register.setParent(this);
    Register.setText("Register");
    Register.move(50,40);
    PDU_Est.setParent(this);
    PDU_Est.setText("Establish PDU Session");
    PDU_Est.move(250,40);
    Start.setParent(this);
    Start.setText("Start to send packets");
    Start.move(150,120);
    Start.hide();

    multi_error_win.prompt.setText("ERROR: This UE does not support multicast!");
    multi_error_win.setFixedSize(500,100);

}

void UE_Window::UE_Window_Show(){
    this->show();
}
