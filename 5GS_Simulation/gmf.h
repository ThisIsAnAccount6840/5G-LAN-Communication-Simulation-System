#ifndef GMF_H
#define GMF_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QTime>
#include <QCoreApplication>

#include "link_list.h"
#include "lan_info.h"
#include "mac_info.h"
#include "Time_Delay.h"
#include "ui_mainwindow.h"

class GMF : public QObject
{
    Q_OBJECT
public:
    GMF(QObject *parent = nullptr);
    explicit GMF(QObject *parent, int, QString [], LAN_INFO *, Ui::MainWindow*);//第一个数组里是最先建立的UE的SUPI列表；第二个数组是两个VN组信息
    ~GMF();

    Ui::MainWindow* ui;

    bool find_supi_in_lan_list(QString);
    bool find_member_id(int, LINK_LIST<QString> &, LINK_LIST<QString> &);//第一个参数是要查询的id数量。前一个链表参数是UE获得的所有memberID,后一个链表是这些ID对应的LAN组名称
    void add_supi_to_lan_list(QString);
    void add_memberID_to_group1(QString);//后续或许可以改成add_member_to_group
    void add_memberID_to_group2(QString);
    //Slot Functions
    void handle_group_regis_msg(int, QString, LINK_LIST<QString>, LINK_LIST<QString>, QString);//参数1：需要验证的ID及对应的组数量（目前是总数量）
                                                                                       //参数2：UE的SUPI；参数3：要验证的IDs；参数4：上述ID对应的LAN组名；参数5：UE的IP地址
    void handle_PDU_Session_Regis_Req(QString, LINK_LIST<QString> &);
private:
    LINK_LIST<QString> ue_with_lan_cap_list;//所有支持LAN的UE的SUPI列表
    LINK_LIST<QString> group1_memberID_list;//VN组1内所有UE的member ID
    LINK_LIST<QString> group2_memberID_list;//VN组2内所有UE的member ID
    LINK_LIST<MAC_Info> group1_member_mac_info;//VN组1内所有UE的IP地址及对应的SUPI和MemeberID
    LINK_LIST<MAC_Info> group2_member_mac_info;//VN组2内所有UE的IP地址及对应的SUPI和MemeberID
    LAN_INFO group1_config;//VN组1配置信息，包括组名、DNN+S-NSSAI和成员ID链表
    LAN_INFO group2_config;//VN组2配置信息，包括组名、DNN+S-NSSAI和成员ID链表
    LAN_INFO* LAN_Group_Config[2];//={nullptr,nullptr}//发送给UE的组配置信息
    LINK_LIST<QString> GMF_PDU_Session_ID;
    LINK_LIST<MAC_Info> MAC_Info_to_UE; //发送给UE的，其他成员的IP地址及对应的member id和supi
signals:
    void Group_Registration_Response(bool, LAN_INFO**, LINK_LIST<QString>*);//第二个参数：UE能访问的组的配置信息；第三个参数：UE在这些组内使用的Member ID
    void PDU_Session_Registration_Response(LINK_LIST<MAC_Info> *, QString);//第二个参数是UE所在的VN组名
};

#endif // GMF_H
