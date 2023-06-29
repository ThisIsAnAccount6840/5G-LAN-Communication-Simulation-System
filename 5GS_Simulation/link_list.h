#ifndef LINK_LIST_H
#define LINK_LIST_H

#include <QDebug>
#include <QTime>
#include <QCoreApplication>
#include "ui_mainwindow.h"

template <typename T> class LINK_LIST;

template <typename T> class NODE{
public:
    T info;
    NODE<T> *link;
    NODE();
    NODE(const T &);
    void insert_after(NODE<T> *);

    friend class LINK_LIST<T>;
};

template <typename T> class LINK_LIST{
 public:
    NODE<T> *head, *tail;
    QString Name;
    Ui::MainWindow *ui;
    LINK_LIST(QString ="NULL");
    LINK_LIST(QString, Ui::MainWindow*);
    ~LINK_LIST();
    NODE<T>* find_node(T &);
    void Insert_Rear(NODE<T> *);
    void DeleteNode(NODE<T> *);
    void Make_Empty();
    void print();
    int node_num();
};

template <typename T> NODE<T>::NODE(){
    link=nullptr;
}

template  <typename T> NODE<T>::NODE(const T &i){
    info=i;
}

template <typename T> void NODE<T>::insert_after(NODE<T> *p){
    p->link=link;
    link=p;
}

template <typename T> LINK_LIST<T>::LINK_LIST(QString n){
    Name=n;
    head=tail=new NODE<T>();
    ui=nullptr;
}

template <typename T> LINK_LIST<T>::LINK_LIST(QString name, Ui::MainWindow* ui_p){
    Name=name;
    head=tail=new NODE<T>();
    ui=ui_p;
}

template  <typename T> LINK_LIST<T>::~LINK_LIST(){
    Make_Empty();
    delete head;
}
template <typename T> void LINK_LIST<T>::Make_Empty(){
    NODE<T> *temp;
    while(head->link!=nullptr){
        temp=head->link;
        head->link=temp->link;
        delete temp;
    }
    tail=head;//代表空链表
}

template <typename T> NODE<T>* LINK_LIST<T>::find_node(T &info){
    NODE<T> *temp=head->link;
    while(temp!=nullptr){
        if(temp->info == info) return temp; //需要在自定义类里面重载==
        temp=temp->link;
    }
    return nullptr;
}

template <typename T> void LINK_LIST<T>::Insert_Rear(NODE<T> *node){
    node->link=tail->link;
    tail->link=node;
    tail=node;
}

template <typename T> void LINK_LIST<T>::DeleteNode(NODE<T>* node){
    NODE<T> *p=head;
    while(p->link!=node && p->link!=nullptr) p=p->link;
    if(p->link!=nullptr) p->link=node->link;
    else qDebug()<<"NODE NOT FOUND!"<<endl;
}

template <typename T> void LINK_LIST<T>::print(){
    NODE<T> *temp;
    for(temp=head->link;temp!=nullptr;temp=temp->link){
        qDebug()<<temp->info<<endl;
        if(ui!=nullptr) {
            ui->Output_Window->append(temp->info+"\n");
        }
    }
}

template <typename T> int LINK_LIST<T>::node_num(){
    int num;
    NODE<T> *node;
    for(num=0,node=head->link; node!=nullptr; node=node->link){
        num++;
    }
    return num;
}

#endif // LINK_LIST_H
