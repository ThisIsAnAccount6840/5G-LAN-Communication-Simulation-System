#ifndef IES_H
#define IES_H

#include <QString>
#include <QDebug>

class F_TEID{  //当IP地址为null，TEID=-1时表示all-match
public:
    QString IP_Address;//Interface的IP地址
    int TEID;
};

class PDI{
public:
    PDI();
    int Source_Interface;// 只包含Interface Value，0-Access, 1-Core, 4-5G VN Internal
    F_TEID Local_FTEID;//F-TEID由TEID和接口IP地址组成
    QString UE_IP_Address;//可以是源也可以是目的地址（上行是源地址，下行就是目的地址）
    QString Network_Instance;//用于区分不同VN组的数据包(使用VN组名)

    bool operator==(PDI &);
    bool Source_Interface_match(int);
    bool UE_IP_Address_match(QString);
    void print_pdi();
};

class Forwarding_Parameters{
public:
int Destination_Interface;//只包含Interface Value.0-Access; 1-Core; 5-5G VN Internal
F_TEID Outer_Header_Creation;//包含了要加在包头部的F-TEID
};

class Duplicating_Parameters{
public:
int Destination_Interface;//只包含Interface Value.0-Access; 1-Core; 5-5G VN Internal
F_TEID Outer_Header_Creation;
};

class Packet_Replication_and_Detection_CarryOn_Info{
public:
    Packet_Replication_and_Detection_CarryOn_Info();
    bool PRIUEAI;//指示是否从N3接口收到了对应UE的数据包,1-received
    bool PRIN19I;//指示是否从N19接口收到了对应UE的数据包
    bool PRIN6I;//指示是否从N6接口收到了对应UE的数据包
    bool DCARONI;//指示是否需要继续匹配
};

class Create_FAR{
public:
    Create_FAR();
    int FAR_ID;
    QString Apply_Action;//2bit string, 只包含5th oct的2个参数：DUPL和FOWRD；1-执行，0-不执行
    Forwarding_Parameters forwarding_parameters;
    Duplicating_Parameters duplicating_parameters;
    void print_far();
};

class Create_PDR{
public:
    Create_PDR();
    PDI pdi;
    int PDR_ID;//1开头
    int FAR_ID;//2开头
    int Precedence;
    Packet_Replication_and_Detection_CarryOn_Info rep_detc_info;
    void print_PDR();
};

#endif // IES_H
