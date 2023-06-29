#ifndef UPF_H
#define UPF_H

#include <QObject>
#include <QTime>
#include <QCoreApplication>

#include "IEs.h"
#include "N4_Session_Context.h"
#include "N4_Session_Establishment_Request.h"
#include "link_list.h"
#include "interface.h"
#include "Time_Delay.h"
#include "ui_mainwindow.h"

class UPF : public QObject
{
    Q_OBJECT
public:
    explicit UPF(QObject *parent, QString, int, QString, QString, QString, QString, QString, Ui::MainWindow*);//第二个参数：服务的VN组名;第三个参数：TEID起始值;第四个参数：UPF_N3接口IP;第五个参数：UPF_N19接口IP; 最后三个参数是N3,N6和N19接口名
    QString serving_group_name;//该UPF服务的VN组名
    LINK_LIST<N4_SESSION_CONTEXT> N4_Session_Context_List;//根据TS29.244，一个PFCP Session与一个PDU会话对应
    int TEID_UPF;//UPF为每个PDU会话分配的TEID，UPF1从1开始，UPF2从40开始
    LINK_LIST<int> UPF_TEID_List;//该UPF分配过的所有TEID的值组成的链表

    Ui::MainWindow *ui;

    QString UPF_N3_Interface_IP;
    UPF_N3 UPF_N3_Interface;
    UPF_N6 UPF_N6_Interface;
    //QString UPF_N19_Interface_IP;
    UPF_N19 UPF_N19_Interface;
    //_5G_VN_INTERNAL _5g_VN_Internal;
    void _5G_VN_Internal_Recv(Data_Packet &);

    //match and execute
    void find_PFCP_Session_with_a_matching_PDR(Data_Packet &, QString ="null");//第二个参数：接口IP
    bool execute_FAR(Data_Packet &, Create_FAR *, Create_PDR *, NODE<N4_SESSION_CONTEXT> *);//返回值是Detection Carry-On Indication

    //slot functions
    void handle_N4_Session_Est_Req(N4_SESSION_ESTABLISHMENT_REQUEST *);
    void handle_N4_Session_Modi_Req(int, QString, QString, QString);//第一个参数是gNB分配的TEID，第二个参数是gNB的IP地址，第三个参数是group name,第四个参数是SUPI
    void handle_Group_Level_N4_Seesion_Est_Request(N4_SESSION_ESTABLISHMENT_REQUEST *);
    void handle_broadcast_packet(QString, int, QString, QString);//第一个参数是发出广播数据包的UE IP，第二个参数是接口编号（3/6/19）,第三个参数是目的地址（广播或组播地址），第四个参数是network instance
signals:
    void N4_Session_Estblishment_Response(int);//暂时只把TEID传回给UE
    void N4_Session_Modification_Response();
    //void send_to_5g_VN_Internal(Data_Packet &);
    void send_to_N3_Interface(Data_Packet &);
    void send_to_N6_Interface(Data_Packet &, QString);
    void send_to_N19_Interface(Data_Packet &);
};

#endif // UPF_H
