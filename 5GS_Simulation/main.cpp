#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ue.h"
#include "ue_window.h"
#include "gnb.h"
#include "amf.h"
#include "smf.h"
#include "upf.h"
#include "udm.h"
#include "gmf.h"
#include "lan_info.h"
#include "link_list.h"

#include <QApplication>
#include <QDebug>

//using namespace std;

const int ini_LAN_UE_num=6;//初始支持LAN的UE数目
const int group1_ue_num=4;//VN组1的UE数目
const int group2_ue_num=3;//VN组2的UE数目
const int gnb1_ue_num=3;//接到第一个基站的UE数目
const int gnb2_ue_num=2;//接到第二个基站的UE数目

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    Ui::MainWindow *ui;
    ui=w.ui;
    ui->UE0_Label->hide();
    ui->UE1_Label->hide();
    ui->UE2_Label->hide();
    ui->UE3_Label->hide();
    ui->UE4_Label->hide();
    ui->UE5_Label->hide();
    ui->UE0_Label_2->hide();
    ui->UE1_Label_2->hide();
    ui->UE2_Label_2->hide();
    ui->UE3_Label_2->hide();
    ui->UE4_Label_2->hide();
    ui->UE5_Label_2->hide();
    //ui->OAI_Output_Label->hide();
    //ui->OAI_Output_Window->hide();
    //ui->Delay_Label->hide();
    //ui->Jitter_Label->hide();

    //gNB configuration Info
    QString gNB1_ue_all_supi[gnb1_ue_num]={"208950000000031", "208950000000032", "208950000000033"};
    QString gNB2_ue_all_supi[gnb2_ue_num]={"208950000000034", "208950000000035"};
    LINK_LIST<QString> gNB1_ue_supi_list;
    LINK_LIST<QString> gNB2_ue_supi_list;
    int j;
    NODE<QString> *node;
    for(j=0;j<gnb1_ue_num;j++){
        node=new NODE<QString>(gNB1_ue_all_supi[j]);
        gNB1_ue_supi_list.Insert_Rear(node);
    }
    for(j=0;j<gnb2_ue_num;j++){
        node=new NODE<QString>(gNB2_ue_all_supi[j]);
        gNB2_ue_supi_list.Insert_Rear(node);
    }

    //Interface Configuration Info
    QString gNB1_N3_Interface_Ip="a";
    QString gNB2_N3_Interface_Ip="aa";
    QString UPF1_N3_Interface_Ip="b";
    QString UPF2_N3_Interface_Ip="c";
    QString UPF1_N19_Interface_Ip="d";
    QString UPF2_N19_Interface_Ip="e";

    //SMF Configuration Info
    QString group1_ue_supi_list_arr[group1_ue_num]={"208950000000031", "208950000000032", "208950000000033", "208950000000036"};
    LINK_LIST<QString> group1_ue_supi_list;
    QString group2_ue_supi_list_arr[group2_ue_num]={ "208950000000033", "208950000000034", "208950000000035"};
    LINK_LIST<QString> group2_ue_supi_list;
    int k;
    for(k=0;k<group1_ue_num;k++){
        node=new NODE<QString>(group1_ue_supi_list_arr[k]);
        group1_ue_supi_list.Insert_Rear(node);
    }
    for(k=0;k<group2_ue_num;k++){
        node=new NODE<QString>(group2_ue_supi_list_arr[k]);
        group2_ue_supi_list.Insert_Rear(node);
    }

    //UE configurations summary
    int i;
    NODE<QString> *new_node;
    QString ue_supi[ini_LAN_UE_num]={"208950000000031", "208950000000032", "208950000000033", "208950000000034", "208950000000035", "208950000000036"};
    QString group1_id_list[group1_ue_num]={"VN1_UE0", "VN1_UE1", "VN1_UE2", "VN1_UE3"};
    QString group2_id_list[group2_ue_num]={"VN2_UE0", "VN2_UE1", "VN2_UE2"};
    LINK_LIST<QString> GROUP1_MEMBER_ID_LIST("VN_Group1", ui);//名称是对应VN组的名称
    LINK_LIST<QString> GROUP2_MEMBER_ID_LIST("VN_Group2", ui);
    for(i=0;i<group1_ue_num;i++){
        new_node=new NODE<QString>(group1_id_list[i]);
        GROUP1_MEMBER_ID_LIST.Insert_Rear(new_node);
    }
    for(i=0;i<group2_ue_num;i++){
        new_node=new NODE<QString>(group2_id_list[i]);
        GROUP2_MEMBER_ID_LIST.Insert_Rear(new_node);
    }
    new_node=nullptr;

    LAN_INFO LAN_GROUP1("VN_Group1", "VN01", 223, 22, GROUP1_MEMBER_ID_LIST);
    LAN_INFO LAN_GROUP2("VN_Group2", "VN02", 225, 20, GROUP2_MEMBER_ID_LIST);
    LAN_INFO LAN_INFO_SUM[2];//此时只调用默认构造函数
    LAN_INFO_SUM[0]=LAN_GROUP1;
    LAN_INFO_SUM[1]=LAN_GROUP2;

    //UE0 Config:
    NODE<QString> *ue0_group_name1=new NODE<QString>("VN_Group1");
    LINK_LIST<QString> ue0_all_group_name;//UE能加入的所有组的名称
    ue0_all_group_name.Insert_Rear(ue0_group_name1);
    NODE<QString> *ue0_group1_id1=new NODE<QString>("VN1_UE0");
    LINK_LIST<QString> ue0_all_member_id;//UE在这些组里使用的ID
    ue0_all_member_id.Insert_Rear(ue0_group1_id1);
    NODE<QString> *UE0_DNN1=new NODE<QString>("VN01");
    LINK_LIST<QString> ue0_all_dnn;//UE能访问的所有DNN列表
    ue0_all_dnn.Insert_Rear(UE0_DNN1);
    NODE<int> *UE0_SST1=new NODE<int>(223);
    LINK_LIST<int> ue0_all_sst;//UE能访问的所有DNN对应的S-NSSAI
    ue0_all_sst.Insert_Rear(UE0_SST1);
    NODE<int> *UE0_SD1=new NODE<int>(22);
    LINK_LIST<int> ue0_all_sd;//UE能访问的所有DNN对应的S-NSSAI
    ue0_all_sd.Insert_Rear(UE0_SD1);
    //UE1 Config:
    NODE<QString> *ue1_group_name1=new NODE<QString>("VN_Group1");
    LINK_LIST<QString> ue1_all_group_name;//UE能加入的所有组的名称
    ue1_all_group_name.Insert_Rear(ue1_group_name1);
    NODE<QString> *ue1_group1_id1=new NODE<QString>("VN1_UE1");
    LINK_LIST<QString> ue1_all_member_id;//UE在这些组里使用的ID
    ue1_all_member_id.Insert_Rear(ue1_group1_id1);
    NODE<QString> *UE1_DNN1=new NODE<QString>("VN01");
    LINK_LIST<QString> ue1_all_dnn;//UE能访问的所有DNN列表
    ue1_all_dnn.Insert_Rear(UE1_DNN1);
    NODE<int> *UE1_SST1=new NODE<int>(223);
    LINK_LIST<int> ue1_all_sst;//UE能访问的所有DNN对应的S-NSSAI
    ue1_all_sst.Insert_Rear(UE1_SST1);
    NODE<int> *UE1_SD1=new NODE<int>(22);
    LINK_LIST<int> ue1_all_sd;//UE能访问的所有DNN对应的S-NSSAI
    ue1_all_sd.Insert_Rear(UE1_SD1);

    //UE2 Config:---mutual UE
    NODE<QString> *ue2_group_name1=new NODE<QString>("VN_Group1");
    NODE<QString> *ue2_group_name2=new NODE<QString>("VN_Group2");
    LINK_LIST<QString> ue2_all_group_name;//UE能加入的所有组的名称
    ue2_all_group_name.Insert_Rear(ue2_group_name1);
    ue2_all_group_name.Insert_Rear(ue2_group_name2);
    NODE<QString> *ue2_group1_id1=new NODE<QString>("VN1_UE2");
    NODE<QString> *ue2_group2_id1=new NODE<QString>("VN2_UE0");
    LINK_LIST<QString> ue2_all_member_id;//UE在这些组里使用的ID
    ue2_all_member_id.Insert_Rear(ue2_group1_id1);
    ue2_all_member_id.Insert_Rear(ue2_group2_id1);
    NODE<QString> *UE2_DNN1=new NODE<QString>("VN01");
    NODE<QString> *UE2_DNN2=new NODE<QString>("VN02");
    LINK_LIST<QString> ue2_all_dnn;//UE能访问的所有DNN列表
    ue2_all_dnn.Insert_Rear(UE2_DNN1);
    ue2_all_dnn.Insert_Rear(UE2_DNN2);
    NODE<int> *UE2_SST1=new NODE<int>(223);
    NODE<int> *UE2_SD1=new NODE<int>(22);
    NODE<int> *UE2_SST2=new NODE<int>(225);
    NODE<int> *UE2_SD2=new NODE<int>(20);
    LINK_LIST<int> ue2_all_sst;//UE能访问的所有DNN对应的S-NSSAI
    ue2_all_sst.Insert_Rear(UE2_SST1);
    ue2_all_sst.Insert_Rear(UE2_SST2);
    LINK_LIST<int> ue2_all_sd;//UE能访问的所有DNN对应的S-NSSAI
    ue2_all_sd.Insert_Rear(UE2_SD1);
    ue2_all_sd.Insert_Rear(UE2_SD2);
    //UE3 Config:
    NODE<QString> *ue3_group_name1=new NODE<QString>("VN_Group2");
    LINK_LIST<QString> ue3_all_group_name;//UE能加入的所有组的名称
    ue3_all_group_name.Insert_Rear(ue3_group_name1);
    NODE<QString> *ue3_group2_id1=new NODE<QString>("VN2_UE1");
    LINK_LIST<QString> ue3_all_member_id;//UE在这些组里使用的ID
    ue3_all_member_id.Insert_Rear(ue3_group2_id1);
    NODE<QString> *UE3_DNN2=new NODE<QString>("VN02");
    LINK_LIST<QString> ue3_all_dnn;//UE能访问的所有DNN列表
    ue3_all_dnn.Insert_Rear(UE3_DNN2);
    NODE<int> *UE3_SST2=new NODE<int>(225);
    LINK_LIST<int> ue3_all_sst;//UE能访问的所有DNN对应的S-NSSAI
    ue3_all_sst.Insert_Rear(UE3_SST2);
    NODE<int> *UE3_SD2=new NODE<int>(20);
    LINK_LIST<int> ue3_all_sd;//UE能访问的所有DNN对应的S-NSSAI
    ue3_all_sd.Insert_Rear(UE3_SD2);
    //UE4 Config:
    NODE<QString> *ue4_group_name1=new NODE<QString>("VN_Group2");
    LINK_LIST<QString> ue4_all_group_name;//UE能加入的所有组的名称
    ue4_all_group_name.Insert_Rear(ue4_group_name1);
    NODE<QString> *ue4_group2_id1=new NODE<QString>("VN2_UE2");
    LINK_LIST<QString> ue4_all_member_id;//UE在这些组里使用的ID
    ue4_all_member_id.Insert_Rear(ue4_group2_id1);
    NODE<QString> *UE4_DNN2=new NODE<QString>("VN02");
    LINK_LIST<QString> ue4_all_dnn;//UE能访问的所有DNN列表
    ue4_all_dnn.Insert_Rear(UE4_DNN2);
    NODE<int> *UE4_SST2=new NODE<int>(225);
    LINK_LIST<int> ue4_all_sst;//UE能访问的所有DNN对应的S-NSSAI
    ue4_all_sst.Insert_Rear(UE4_SST2);
    NODE<int> *UE4_SD2=new NODE<int>(20);
    LINK_LIST<int> ue4_all_sd;//UE能访问的所有DNN对应的S-NSSAI
    ue4_all_sd.Insert_Rear(UE4_SD2);
    //UE5 Config:
    NODE<QString> *ue5_group_name1=new NODE<QString>("VN_Group1");
    LINK_LIST<QString> ue5_all_group_name;//UE能加入的所有组的名称
    ue5_all_group_name.Insert_Rear(ue5_group_name1);
    NODE<QString> *ue5_group2_id1=new NODE<QString>("VN1_UE3");
    LINK_LIST<QString> ue5_all_member_id;//UE在这些组里使用的ID
    ue5_all_member_id.Insert_Rear(ue5_group2_id1);
    NODE<QString> *UE5_DNN2=new NODE<QString>("VN01");
    LINK_LIST<QString> ue5_all_dnn;//UE能访问的所有DNN列表
    ue5_all_dnn.Insert_Rear(UE5_DNN2);
    NODE<int> *UE5_SST2=new NODE<int>(223);
    LINK_LIST<int> ue5_all_sst;//UE能访问的所有DNN对应的S-NSSAI
    ue5_all_sst.Insert_Rear(UE5_SST2);
    NODE<int> *UE5_SD2=new NODE<int>(22);
    LINK_LIST<int> ue5_all_sd;//UE能访问的所有DNN对应的S-NSSAI
    ue5_all_sd.Insert_Rear(UE5_SD2);

    //core network components
    AMF amf(nullptr, ui);
    SMF smf(nullptr, group1_ue_supi_list, group2_ue_supi_list, ui);
    UPF upf1(nullptr, "VN_Group1", 0, UPF1_N3_Interface_Ip, UPF1_N19_Interface_Ip, "UPF1 N3 Interface", "UPF1 N6 Interface", "UPF1 N19 Interface", ui);
    UPF upf2(nullptr, "VN_Group2", 40, UPF2_N3_Interface_Ip, UPF2_N19_Interface_Ip, "UPF2 N3 Interface", "UPF2 N6 Interface", "UPF2 N19 Interface", ui);
    UDM udm;
    GMF gmf(nullptr, ini_LAN_UE_num, ue_supi, LAN_INFO_SUM, ui);

    //gNB
    gNB gnb1(nullptr, gNB1_ue_supi_list, gNB1_N3_Interface_Ip, 10, ui);
    gNB gnb2(nullptr, gNB2_ue_supi_list, gNB2_N3_Interface_Ip, 50, ui);

    //UE
    UE ue0(nullptr, "UE0", "208950000000031", "12.1.1.1", "08:00:20:0A:8C:6C", true, false, ui);
    udm.add_ue_record(ue0, 1, ue0_all_group_name, ue0_all_member_id, ue0_all_dnn, ue0_all_sst, ue0_all_sd);
    UE ue1(nullptr, "UE1", "208950000000032", "12.1.1.2", "08:00:20:0A:8C:6D", true, true, ui);
    udm.add_ue_record(ue1, 1, ue1_all_group_name, ue1_all_member_id, ue1_all_dnn, ue1_all_sst, ue1_all_sd);
    UE ue2(nullptr, "UE2", "208950000000033", "12.1.1.3", "08:00:20:0A:8C:6E", true, true, ui);
    udm.add_ue_record(ue2, 2, ue2_all_group_name, ue2_all_member_id, ue2_all_dnn, ue2_all_sst, ue2_all_sd);
    UE ue3(nullptr, "UE3", "208950000000034", "12.1.1.4", "08:00:20:0A:8C:6F", true, false, ui);
    udm.add_ue_record(ue3, 1, ue3_all_group_name, ue3_all_member_id, ue3_all_dnn, ue3_all_sst, ue3_all_sd);
    UE ue4(nullptr, "UE4", "208950000000035", "12.1.1.5", "08:00:20:0A:8C:70", true, false, ui);
    udm.add_ue_record(ue4, 1, ue4_all_group_name, ue4_all_member_id, ue4_all_dnn, ue4_all_sst, ue4_all_sd);
    UE ue5(nullptr, "UE5", "208950000000036", "12.1.1.6", "08:00:20:0A:8C:71", true, true, ui);
    udm.add_ue_record(ue5, 1, ue5_all_group_name, ue5_all_member_id, ue5_all_dnn, ue5_all_sst, ue5_all_sd);

    //UE button connect functions
    QObject::connect(ui->UE0_b,&QPushButton::released,&(ue0.ue_w), &UE_Window::UE_Window_Show);
    QObject::connect(ui->UE1_b,&QPushButton::released,&(ue1.ue_w), &UE_Window::UE_Window_Show);
    QObject::connect(ui->UE2_b,&QPushButton::released,&(ue2.ue_w), &UE_Window::UE_Window_Show);
    QObject::connect(ui->UE3_b,&QPushButton::released,&(ue3.ue_w), &UE_Window::UE_Window_Show);
    QObject::connect(ui->UE4_b,&QPushButton::released,&(ue4.ue_w), &UE_Window::UE_Window_Show);
    QObject::connect(ui->UE5_b,&QPushButton::released,&(ue5.ue_w), &UE_Window::UE_Window_Show);

    //Send packet Window connect functions
    QObject::connect(&(ue0.ue_w.Start), &QPushButton::released, &ue0, &UE::sendPacketWindow_show);
    QObject::connect(&ue0.ue_w.send_packet_window.unicast, &QPushButton::released, &ue0, &UE::unicast_member_show);
    QObject::connect(&ue0.ue_w.send_packet_window.broadcast, &QPushButton::released, &ue0, &UE::broadcast_group_show);
    QObject::connect(&ue0.ue_w.send_packet_window.multicast, &QPushButton::released, &ue0, &UE::multicast_member_show);
    QObject::connect(&ue0.ue_w.send_packet_window.start, &QPushButton::released, &ue0, &UE::recv_packet_info);
    QObject::connect(&(ue1.ue_w.Start), &QPushButton::released, &ue1, &UE::sendPacketWindow_show);
    QObject::connect(&ue1.ue_w.send_packet_window.unicast, &QPushButton::released, &ue1, &UE::unicast_member_show);
    QObject::connect(&ue1.ue_w.send_packet_window.broadcast, &QPushButton::released, &ue1, &UE::broadcast_group_show);
    QObject::connect(&ue1.ue_w.send_packet_window.multicast, &QPushButton::released, &ue1, &UE::multicast_member_show);
    QObject::connect(&ue1.ue_w.send_packet_window.start, &QPushButton::released, &ue1, &UE::recv_packet_info);
    QObject::connect(&(ue2.ue_w.Start), &QPushButton::released, &ue2, &UE::sendPacketWindow_show);
    QObject::connect(&ue2.ue_w.send_packet_window.unicast, &QPushButton::released, &ue2, &UE::unicast_member_show);
    QObject::connect(&ue2.ue_w.send_packet_window.broadcast, &QPushButton::released, &ue2, &UE::broadcast_group_show);
    QObject::connect(&ue2.ue_w.send_packet_window.multicast, &QPushButton::released, &ue2, &UE::multicast_member_show);
    QObject::connect(&ue2.ue_w.send_packet_window.start, &QPushButton::released, &ue2, &UE::recv_packet_info);
    QObject::connect(&(ue3.ue_w.Start), &QPushButton::released, &ue3, &UE::sendPacketWindow_show);
    QObject::connect(&ue3.ue_w.send_packet_window.unicast, &QPushButton::released, &ue3, &UE::unicast_member_show);
    QObject::connect(&ue3.ue_w.send_packet_window.broadcast, &QPushButton::released, &ue3, &UE::broadcast_group_show);
    QObject::connect(&ue3.ue_w.send_packet_window.multicast, &QPushButton::released, &ue3, &UE::multicast_member_show);
    QObject::connect(&ue3.ue_w.send_packet_window.start, &QPushButton::released, &ue3, &UE::recv_packet_info);
    QObject::connect(&(ue4.ue_w.Start), &QPushButton::released, &ue4, &UE::sendPacketWindow_show);
    QObject::connect(&ue4.ue_w.send_packet_window.unicast, &QPushButton::released, &ue4, &UE::unicast_member_show);
    QObject::connect(&ue4.ue_w.send_packet_window.broadcast, &QPushButton::released, &ue4, &UE::broadcast_group_show);
    QObject::connect(&ue4.ue_w.send_packet_window.multicast, &QPushButton::released, &ue4, &UE::multicast_member_show);
    QObject::connect(&ue4.ue_w.send_packet_window.start, &QPushButton::released, &ue4, &UE::recv_packet_info);
    QObject::connect(&(ue5.ue_w.Start), &QPushButton::released, &ue5, &UE::sendPacketWindow_show);
    QObject::connect(&ue5.ue_w.send_packet_window.unicast, &QPushButton::released, &ue5, &UE::unicast_member_show);
    QObject::connect(&ue5.ue_w.send_packet_window.broadcast, &QPushButton::released, &ue5, &UE::broadcast_group_show);
    QObject::connect(&ue5.ue_w.send_packet_window.multicast, &QPushButton::released, &ue5, &UE::multicast_member_show);
    QObject::connect(&ue5.ue_w.send_packet_window.start, &QPushButton::released, &ue5, &UE::recv_packet_info);

    //Establish Window connect functions
    QObject::connect(&(ue0.ue_w.PDU_Est), &QPushButton::released, &ue0, &UE::PDU_Window_Show);
    QObject::connect(&ue0.ue_w.PDU_win.ok, &QPushButton::released, &ue0, &UE::recv_pdu_est_info);
    QObject::connect(&(ue1.ue_w.PDU_Est), &QPushButton::released, &ue1, &UE::PDU_Window_Show);
    QObject::connect(&ue1.ue_w.PDU_win.ok, &QPushButton::released, &ue1, &UE::recv_pdu_est_info);
    QObject::connect(&(ue2.ue_w.PDU_Est), &QPushButton::released, &ue2, &UE::PDU_Window_Show);
    QObject::connect(&ue2.ue_w.PDU_win.ok, &QPushButton::released, &ue2, &UE::recv_pdu_est_info);
    QObject::connect(&(ue3.ue_w.PDU_Est), &QPushButton::released, &ue3, &UE::PDU_Window_Show);
    QObject::connect(&ue3.ue_w.PDU_win.ok, &QPushButton::released, &ue3, &UE::recv_pdu_est_info);
    QObject::connect(&(ue4.ue_w.PDU_Est), &QPushButton::released, &ue4, &UE::PDU_Window_Show);
    QObject::connect(&ue4.ue_w.PDU_win.ok, &QPushButton::released, &ue4, &UE::recv_pdu_est_info);
    QObject::connect(&(ue5.ue_w.PDU_Est), &QPushButton::released, &ue5, &UE::PDU_Window_Show);
    QObject::connect(&ue5.ue_w.PDU_win.ok, &QPushButton::released, &ue5, &UE::recv_pdu_est_info);

    //UE Registration Button Connect Functions
    QObject::connect(&ue0.ue_w.Register, &QPushButton::released, &ue0, &UE::sendRegisReq);
    QObject::connect(&ue1.ue_w.Register, &QPushButton::released, &ue1, &UE::sendRegisReq);
    QObject::connect(&ue2.ue_w.Register, &QPushButton::released, &ue2, &UE::sendRegisReq);
    QObject::connect(&ue3.ue_w.Register, &QPushButton::released, &ue3, &UE::sendRegisReq);
    QObject::connect(&ue4.ue_w.Register, &QPushButton::released, &ue4, &UE::sendRegisReq);
    QObject::connect(&ue5.ue_w.Register, &QPushButton::released, &ue5, &UE::sendRegisReq);

    //Send PDU Session Establishment function is included in the recv_pdu_est_info function

    //Registration Procedure Connect Functions
    QObject::connect(&ue0, &UE::Registration_Request_Msg, &amf, &AMF::handle_Regis_Req);
    QObject::connect(&ue1, &UE::Registration_Request_Msg, &amf, &AMF::handle_Regis_Req);
    QObject::connect(&ue2, &UE::Registration_Request_Msg, &amf, &AMF::handle_Regis_Req);
    QObject::connect(&ue3, &UE::Registration_Request_Msg, &amf, &AMF::handle_Regis_Req);
    QObject::connect(&ue4, &UE::Registration_Request_Msg, &amf, &AMF::handle_Regis_Req);
    QObject::connect(&ue5, &UE::Registration_Request_Msg, &amf, &AMF::handle_Regis_Req);
    QObject::connect(&amf, &AMF::UE_Subscrib_Data_Get, &udm, &UDM::handle_Get_UE_Sub_Data);
    QObject::connect(&udm, &UDM::UE_Sub_Data_send, &amf, &AMF::recev_ue_sub_data);
    QObject::connect(&amf, &AMF::UE_Group_Registration_Request, &gmf, &GMF::handle_group_regis_msg);
    QObject::connect(&gmf, &GMF::Group_Registration_Response, &amf, &AMF::handle_group_regis_resp);

    QObject::connect(&amf, &AMF::Registration_Accept_Msg, &ue0, &UE::recv_Regis_Accept);
    QObject::connect(&amf, &AMF::Registration_Accept_Msg, &ue1, &UE::recv_Regis_Accept);
    QObject::connect(&amf, &AMF::Registration_Accept_Msg, &ue2, &UE::recv_Regis_Accept);
    QObject::connect(&amf, &AMF::Registration_Accept_Msg, &ue3, &UE::recv_Regis_Accept);
    QObject::connect(&amf, &AMF::Registration_Accept_Msg, &ue4, &UE::recv_Regis_Accept);
    QObject::connect(&amf, &AMF::Registration_Accept_Msg, &ue5, &UE::recv_Regis_Accept);

    //PDU Session Establishment Procedure Connect Functions
    QObject::connect(&ue0, &UE::PDU_Session_Esta_Req, &amf, &AMF::handle_PDU_Session_Est_Req);
    QObject::connect(&ue1, &UE::PDU_Session_Esta_Req, &amf, &AMF::handle_PDU_Session_Est_Req);
    QObject::connect(&ue2, &UE::PDU_Session_Esta_Req, &amf, &AMF::handle_PDU_Session_Est_Req);
    QObject::connect(&ue3, &UE::PDU_Session_Esta_Req, &amf, &AMF::handle_PDU_Session_Est_Req);
    QObject::connect(&ue4, &UE::PDU_Session_Esta_Req, &amf, &AMF::handle_PDU_Session_Est_Req);
    QObject::connect(&ue5, &UE::PDU_Session_Esta_Req, &amf, &AMF::handle_PDU_Session_Est_Req);

    QObject::connect(&amf, &AMF::PDUSession_CreateSMContext_Request, &smf, &SMF::handle_CreateSMContext_Req);
    QObject::connect(&smf, &SMF::PDU_Session_Registration_Request, &gmf, &GMF::handle_PDU_Session_Regis_Req);
    QObject::connect(&gmf, &GMF::PDU_Session_Registration_Response, &smf, &SMF::PDU_Session_Regis_Response_Recv);
    QObject::connect(&smf, &SMF::N4_Session_Est_Req, &upf1, &UPF::handle_N4_Session_Est_Req);
    QObject::connect(&smf, &SMF::N4_Session_Est_Req, &upf2, &UPF::handle_N4_Session_Est_Req);
    QObject::connect(&upf1, &UPF::N4_Session_Estblishment_Response, &smf, &SMF::handle_N4_Session_Est_Response);
    QObject::connect(&upf2, &UPF::N4_Session_Estblishment_Response, &smf, &SMF::handle_N4_Session_Est_Response);
    QObject::connect(&smf, &SMF::N1N2_Message_Transfer_smf, &amf, &AMF::N1N2_Message_Transfer_amf);
    QObject::connect(&amf, &AMF::N2_PDU_Session_Request, &gnb1, &gNB::handle_N2_PDUSession_Request);
    QObject::connect(&amf, &AMF::N2_PDU_Session_Request, &gnb2, &gNB::handle_N2_PDUSession_Request);
    QObject::connect(&gnb1, &gNB::AN_Specific_Resource_Setup, &ue0, &UE::handle_AN_Specific_Resoure_Setup);
    QObject::connect(&gnb1, &gNB::AN_Specific_Resource_Setup, &ue1, &UE::handle_AN_Specific_Resoure_Setup);
    QObject::connect(&gnb1, &gNB::AN_Specific_Resource_Setup, &ue2, &UE::handle_AN_Specific_Resoure_Setup);
    QObject::connect(&gnb1, &gNB::AN_Specific_Resource_Setup, &ue5, &UE::handle_AN_Specific_Resoure_Setup);//只是行使功能，实际上UE5不走基站
    QObject::connect(&gnb2, &gNB::AN_Specific_Resource_Setup, &ue3, &UE::handle_AN_Specific_Resoure_Setup);
    QObject::connect(&gnb2, &gNB::AN_Specific_Resource_Setup, &ue4, &UE::handle_AN_Specific_Resoure_Setup);
    QObject::connect(&ue0, &UE::AN_Specific_Resource_Setup_UE, &gnb1, &gNB::AN_Specific_Resource_Setup_Recv);
    QObject::connect(&ue1, &UE::AN_Specific_Resource_Setup_UE, &gnb1, &gNB::AN_Specific_Resource_Setup_Recv);
    QObject::connect(&ue2, &UE::AN_Specific_Resource_Setup_UE, &gnb1, &gNB::AN_Specific_Resource_Setup_Recv);
    QObject::connect(&ue3, &UE::AN_Specific_Resource_Setup_UE, &gnb2, &gNB::AN_Specific_Resource_Setup_Recv);
    QObject::connect(&ue4, &UE::AN_Specific_Resource_Setup_UE, &gnb2, &gNB::AN_Specific_Resource_Setup_Recv);
    QObject::connect(&gnb1, &gNB::N2_PDUSession_Response, &amf, &AMF::handle_N2_PDUSession_Response);
    QObject::connect(&gnb2, &gNB::N2_PDUSession_Response, &amf, &AMF::handle_N2_PDUSession_Response);
    QObject::connect(&amf, &AMF::PDUSession_UpdateSMContext_Request, &smf, &SMF::handle_UpdateSMContext_Req);
    QObject::connect(&smf, &SMF::N4_Session_Modification_Request, &upf1, &UPF::handle_N4_Session_Modi_Req);
    QObject::connect(&smf, &SMF::N4_Session_Modification_Request, &upf2, &UPF::handle_N4_Session_Modi_Req);
    QObject::connect(&upf1, &UPF::N4_Session_Modification_Response, &smf, &SMF::handle_N4_Session_Modi_Response);
    QObject::connect(&upf2, &UPF::N4_Session_Modification_Response, &smf, &SMF::handle_N4_Session_Modi_Response);
    QObject::connect(&smf, &SMF::PDU_Success_Show, &ue0, &UE::PDU_Success_Label_Show);

    QObject::connect(&smf, &SMF::Group_Level_N4_Session_Est_Request_1,  &upf1, &UPF::handle_Group_Level_N4_Seesion_Est_Request);
    QObject::connect(&smf, &SMF::Group_Level_N4_Session_Est_Request_2,  &upf2, &UPF::handle_Group_Level_N4_Seesion_Est_Request);

    //Send packet connection functions
    QObject::connect(&ue0, &UE::Packet_sent_by_UE, &gnb1.gNB_N3_Interface, &gNB_N3::Recev_packet);
    QObject::connect(&ue1, &UE::Packet_sent_by_UE, &gnb1.gNB_N3_Interface, &gNB_N3::Recev_packet);
    QObject::connect(&ue2, &UE::Packet_sent_by_UE, &gnb1.gNB_N3_Interface, &gNB_N3::Recev_packet);
    QObject::connect(&ue3, &UE::Packet_sent_by_UE, &gnb2.gNB_N3_Interface, &gNB_N3::Recev_packet);
    QObject::connect(&ue4, &UE::Packet_sent_by_UE, &gnb2.gNB_N3_Interface, &gNB_N3::Recev_packet);
    QObject::connect(&ue5, &UE::Packet_sent_by_UE, &upf1.UPF_N6_Interface, &UPF_N6::Recev_packet);
    QObject::connect(&upf1.UPF_N3_Interface, &UPF_N3::broadcast_packet_receive, &upf1, &UPF::handle_broadcast_packet);
    QObject::connect(&upf1.UPF_N6_Interface, &UPF_N6::broadcast_packet_receive, &upf1, &UPF::handle_broadcast_packet);
    QObject::connect(&upf1.UPF_N19_Interface, &UPF_N19::broadcast_packet_receive, &upf1, &UPF::handle_broadcast_packet);
    QObject::connect(&upf2.UPF_N3_Interface, &UPF_N3::broadcast_packet_receive, &upf2, &UPF::handle_broadcast_packet);
    QObject::connect(&upf2.UPF_N6_Interface, &UPF_N6::broadcast_packet_receive, &upf2, &UPF::handle_broadcast_packet);
    QObject::connect(&upf2.UPF_N19_Interface, &UPF_N19::broadcast_packet_receive, &upf2, &UPF::handle_broadcast_packet);
    QObject::connect(&gnb1.gNB_N3_Interface, &gNB_N3::uplink_data, &upf1.UPF_N3_Interface, &UPF_N3::Recev_packet);
    QObject::connect(&gnb2.gNB_N3_Interface, &gNB_N3::uplink_data, &upf2.UPF_N3_Interface, &UPF_N3::Recev_packet);
    QObject::connect(&gnb1.gNB_N3_Interface, &gNB_N3::downlink_data, &ue0, &UE::UE_recv_packet);
    QObject::connect(&gnb1.gNB_N3_Interface, &gNB_N3::downlink_data, &ue1, &UE::UE_recv_packet);
    QObject::connect(&gnb1.gNB_N3_Interface, &gNB_N3::downlink_data, &ue2, &UE::UE_recv_packet);
    QObject::connect(&gnb2.gNB_N3_Interface, &gNB_N3::downlink_data, &ue3, &UE::UE_recv_packet);
    QObject::connect(&gnb2.gNB_N3_Interface, &gNB_N3::downlink_data, &ue4, &UE::UE_recv_packet);
    QObject::connect(&upf1.UPF_N6_Interface, &UPF_N6::downlink_data, &ue5, &UE::UE_recv_packet);
    QObject::connect(&upf1.UPF_N3_Interface, &UPF_N3::downlink_data, &gnb1.gNB_N3_Interface, &gNB_N3::Recev_packet);
    QObject::connect(&upf2.UPF_N3_Interface, &UPF_N3::downlink_data, &gnb2.gNB_N3_Interface, &gNB_N3::Recev_packet);
    QObject::connect(&upf1.UPF_N3_Interface, &UPF_N3::uplink_data, &upf1, &UPF::find_PFCP_Session_with_a_matching_PDR);
    QObject::connect(&upf1.UPF_N6_Interface, &UPF_N6::uplink_data, &upf1, &UPF::find_PFCP_Session_with_a_matching_PDR);
    QObject::connect(&upf2.UPF_N3_Interface, &UPF_N3::uplink_data, &upf2, &UPF::find_PFCP_Session_with_a_matching_PDR);
    //FAR emit signal...connect with 5g_VN_Internal function
    QObject::connect(&upf1, &UPF::send_to_N3_Interface, &upf1.UPF_N3_Interface, &UPF_N3::Recev_packet);
    QObject::connect(&upf2, &UPF::send_to_N3_Interface, &upf2.UPF_N3_Interface, &UPF_N3::Recev_packet);
    QObject::connect(&upf1, &UPF::send_to_N6_Interface, &upf1.UPF_N6_Interface, &UPF_N6::Recev_packet);
    QObject::connect(&upf1, &UPF::send_to_N19_Interface, &upf1.UPF_N19_Interface, &UPF_N19::Recev_packet);
    QObject::connect(&upf2, &UPF::send_to_N19_Interface, &upf2.UPF_N19_Interface, &UPF_N19::Recev_packet);
    QObject::connect(&upf1.UPF_N19_Interface, &UPF_N19::send_data, &upf2.UPF_N19_Interface, &UPF_N19::Recev_packet);
    QObject::connect(&upf2.UPF_N19_Interface, &UPF_N19::send_data, &upf1.UPF_N19_Interface, &UPF_N19::Recev_packet);
    QObject::connect(&upf1.UPF_N19_Interface, &UPF_N19::recv_data, &upf1, &UPF::find_PFCP_Session_with_a_matching_PDR);
    QObject::connect(&upf2.UPF_N19_Interface, &UPF_N19::recv_data, &upf2, &UPF::find_PFCP_Session_with_a_matching_PDR);

    return a.exec();
}
