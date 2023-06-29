#ifndef SENDPACKET_WINDOW_H
#define SENDPACKET_WINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLabel>
#include <QTextEdit>
#include "mac_info.h"
#include "link_list.h"

class SendPacket_Window : public QWidget
{
    Q_OBJECT
public:
    explicit SendPacket_Window(QWidget *parent = nullptr);
    LINK_LIST<other_ue_info> *other_ue_info_list;

    QPushButton unicast;
    QPushButton multicast;
    QPushButton broadcast;
    QLabel content_label;
    QTextEdit content;

    //unicast related buttons
    QRadioButton *unicast_buttons[5];
    QString member_ips[5];
    //Group1
    QRadioButton ue0_grp1;
    QRadioButton ue1_grp1;
    QRadioButton ue2_grp1;
    //Group2
    QRadioButton ue0_grp2;
    QRadioButton ue1_grp2;

    //Broadcast-related buttons
    QRadioButton group1_select;
    QRadioButton group2_select;

    //Multicast-related texts
    QLabel prompt;
    QLabel member1;
    QLabel member2;
    QLabel member3;

    QPushButton start;

    //slot functions
    void start_to_send_packet();
signals:

};

#endif // SENDPACKET_WINDOW_H
