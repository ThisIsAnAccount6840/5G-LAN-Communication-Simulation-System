#ifndef UE_H
#define UE_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <QTime>
#include <QCoreApplication>
#include <QLabel>

#include "ui_mainwindow.h"
#include "MM_State.h"
#include "registration_request.h"
#include "registration_accept.h"
#include "pdu_session_est_req.h"
#include "PDU_SESSION_EST_ACCEPT.h"
#include "ue_window.h"
#include "link_list.h"
#include "lan_info.h"
#include "mac_info.h"
#include "Request_Type.h"
#include "data_packet.h"
#include "Time_Delay.h"

class UE : public QObject
{
    Q_OBJECT
public:
    explicit UE(QObject *parent = nullptr,QString name="UE",QString su="000000000",QString ip="0.0.0.0", QString mac="00:00:00:00",bool LAN_C=false, bool multi=false, Ui::MainWindow *ui=nullptr);
    ~UE();
    void setSUPI(QString &);
    void setIP(QString &);
    void setMAC(QString &);
    void setState(MM_STATE &);
    void setLANGroupName(QString &);
    void setGroupMemberID(QString &);
    void setLANCap(bool);

    QString getSUPI();
    QString getIP();
    QString getMAC();
    MM_STATE getState();
    void getLANGroupName();
    void getGroupMemberID();
    bool getLANCapa();

    Ui::MainWindow *ui;
    UE_Window ue_w; //点击UE按钮时跳出来的窗口
    int mode;//转发模式.0-unicast;1-multicast;2-broadcast

    // Slot Functions
    void sendRegisReq();
    void recv_Regis_Accept(QString, LAN_INFO* *, LINK_LIST<QString>*);

    void recv_pdu_est_info();
    void PDU_Window_Show();
    void send_PDU_Session_Est_Req();
    void handle_AN_Specific_Resoure_Setup(PDU_SESSION_ESTABLISHMENT_ACCEPT *, QString);

    void sendPacketWindow_show();
    void recv_packet_info();
    void unicast_member_show();
    void broadcast_group_show();
    void multicast_member_show();
    void send_broadcast_packet();
    void UE_send_packet(QString="null");//参数为PDU会话ID
    void UE_recv_packet(Data_Packet &);
    void PDU_Success_Label_Show(QString);
private:
    MM_STATE mm_state;
    QString supi;
    QString IP_addr;
    QString MAC_addr;
    MM_STATE MM_State; //State of UE, see TS 24.501 Figure 5.1.3.2.1.1.1
    bool IS_in_Multicast_Group;//UE是否在组播组中
    LINK_LIST<other_ue_info> other_member_info_list;
    // PDU Session-related parameters
    LINK_LIST<QString> PDU_Session_ID;
    QString DNN_input;
    int SST_input;
    int SD_input;
    LINK_LIST<QString> Member_ID_Input_List;
    REQ_TYPE req_t;
    // Send Packet Related Parameters
    QString content_input;//数据包的内容
    QString dst_ip_input;//数据包目的IP地址
    Data_Packet *ue_data_packet;
    // LAN-related Parameters
    LINK_LIST<QString> LAN_Group_Name;//Name of the LAN group that the UR belongs to
    LINK_LIST<QString> Group_Member_ID; //All 5GLAN Group member IDs of UE
    bool LAN_Capa;
    REGISTRATION_REQUEST *reg_req;
    PDU_SESSION_EST_REQ *est_req;

signals:
    //void start_PDU_Session_Est();
    void Registration_Request_Msg(REGISTRATION_REQUEST *);
    void PDU_Session_Esta_Req(PDU_SESSION_EST_REQ *);
    void AN_Specific_Resource_Setup_UE();
    void Packet_sent_by_UE(Data_Packet &, QString);//第二个参数：PDU会话ID
};

#endif // UE_H
