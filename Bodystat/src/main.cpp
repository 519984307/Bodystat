﻿#include"ui/maindlg.h"
#include"db/dbmanager.h"
#include"ui/startdialog.h"
#include<QApplication>
#include<QMessageBox>
//#include<vld.h>

int main(int argc,char *argv[]){
    QApplication a(argc,argv);
    if(DBManager::instance()->open()<0){
        QMessageBox msgBox(QMessageBox::Warning,
            QApplication::tr("警报"),
            QApplication::tr("对不起，连接数据库失败，请重试！"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setButtonText(QMessageBox::Ok,
            QApplication::tr("确定"));
        msgBox.exec();
        return(-1);
    }
    StartDialog d;
    if(d.exec()<0){
        QMessageBox msgBox(QMessageBox::Warning,
            QApplication::tr("警报"),
            QApplication::tr("对不起，启动软件失败，请重试！"));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setButtonText(QMessageBox::Ok,
            QApplication::tr("确定"));
        msgBox.exec();
        DBManager::instance()->close();
        return(-2);
    }
    MainDlg w;
    w.show();
    const int result=a.exec();
    DBManager::instance()->close();
    return(result);
}
