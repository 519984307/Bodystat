﻿#ifndef DEV_H
#define DEV_H

#include<QObject>

class Dev
    :public QObject{
    Q_OBJECT
public:
    explicit Dev(QObject *parent=0);
    virtual ~Dev();
private:
    Dev(const Dev &);
    Dev &operator=(const Dev &);
};

#endif // DEV_H
