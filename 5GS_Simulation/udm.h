#ifndef UDM_H
#define UDM_H

#include <QObject>
#include <QString>

#include "ue_sub_data_node.h"
#include "lan_info.h"
#include "ue.h"
#include "link_list.h"

class UDM : public QObject
{
    Q_OBJECT
public:
    explicit UDM(QObject *parent = nullptr);
    ~UDM();
    //UDM Database
    LINK_LIST<UE_Sub_Data> ue_sub_data_base;
    //UE_Sub_Data *first_ue;
    void add_ue_record(UE &, int, LINK_LIST<QString> &, LINK_LIST<QString> &, LINK_LIST<QString> &, LINK_LIST<int> &, LINK_LIST<int> &);//将UE加入链表
    //参数1：UE；参数2：UE能加入的组数量；参数3：UE能加入的所有组的名称；参数4：UE在这些组里面使用的ID；参数5、6、7：这些组对应的DNN+S-NSSAI
    //UDM utility functions
    UE_Sub_Data *find_ue_sub_data(QString);//Use UE's supi to find its subscription data

    //slot functions
    void handle_Get_UE_Sub_Data(QString);

signals:
    void UE_Sub_Data_send(UE_Sub_Data*);

};

#endif // UDM_H
