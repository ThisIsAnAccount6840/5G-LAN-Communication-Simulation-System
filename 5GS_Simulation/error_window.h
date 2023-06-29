#ifndef ERROR_WINDOW_H
#define ERROR_WINDOW_H

#include <QWidget>
#include <QLabel>

class ERROR_WINDOW : public QWidget
{
    Q_OBJECT
public:
    explicit ERROR_WINDOW(QWidget *parent = nullptr);
    explicit ERROR_WINDOW(QWidget *parent, QString);
    QLabel prompt;

signals:

};

#endif // ERROR_WINDOW_H
