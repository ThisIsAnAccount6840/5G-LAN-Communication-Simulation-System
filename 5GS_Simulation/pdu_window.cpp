#include "pdu_window.h"

PDU_Window::PDU_Window(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(450,500);
    DNN_Input_label.setParent(this);
    DNN_Input_label.move(10,0);
    DNN_Input_label.setText("DNN: ");
    DNN_Input.setParent(this);
    DNN_Input.setPlaceholderText("Please input the DNN");
    DNN_Input.move(70,0);
    S_NSSAI.setParent(this);
    S_NSSAI.move(10,40);
    S_NSSAI.setText("S-NSSAI:");
    SST_Input_label.setParent(this);
    SST_Input_label.move(20,80);
    SST_Input_label.setText("SST:");
    SST_Input.setParent(this);
    SST_Input.move(70,80);
    SST_Input.setPlaceholderText("Please input the SST");
    SD_Input_label.setParent(this);
    SD_Input_label.move(20,120);
    SD_Input_label.setText("SD:");
    SD_Input.setParent(this);
    SD_Input.move(70,120);
    SD_Input.setPlaceholderText("Please input the SD");
    member_label.setParent(this);
    member_label.move(10,160);
    member_label.setText("Member IDs:");
    member_id.setParent(this);
    member_id.move(70,200);
    member_id.setPlaceholderText("Please enter the Member IDs of the UEs that this UE wants to connect with, in the form of id1,id2,id3,...");
    ok.setParent(this);
    ok.move(60,420);
    ok.setText("Start PDU Session Establishment");

}

