﻿#include"devinfowidget.h"
#include"ui_devinfowidget.h"
#include"../dev/devpool.h"
#include"../dev/bluetooth.h"
#include"../dev/bodystat.h"
#include"../../../include/BodystatSDK.h"
#include<QMovie>
#include<QMessageBox>
#include<QCloseEvent>

DevInfoWidget::DevInfoWidget(QWidget *parent/*=0*/)
    :MdiSubWidget(parent)
    ,_task(TASK_NONE)
    ,_ui(new Ui::DevInfoWidget){
    _ui->setupUi(this);
    initUi();
}

DevInfoWidget::~DevInfoWidget(){
    delete _ui;
}

void DevInfoWidget::closeEvent(QCloseEvent *event){
    if(TASK_NONE==_task){
        MdiSubWidget::closeEvent(event);
    }else{
        QMessageBox msgBox(
            QMessageBox::Information,tr("提示"),
            tr("任务正在执行，不能退出，稍后重试！"));
        msgBox.setFont(font());
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setButtonText(QMessageBox::Ok,tr("确定"));
        msgBox.exec();
        event->ignore();
    }
}

void DevInfoWidget::onScanDevPushButtonClicked(bool){
    // 设置当前任务
    _task=TASK_SCAN;
    // 启动扫描
    DevPool::instance()->getBluetooth()->scanDev(
        DevPool::instance()->getBodyStat());
    // 失效按钮
    _ui->_unpairPushButton->setDisabled(true);
    _ui->_scanDevPushButton->setDisabled(true);
    _ui->_reloadDevPushButton->setDisabled(true);
    _ui->_resetTimePushButton->setDisabled(true);
    // 显示进度条
    _ui->_progressLabel->show();
}

void DevInfoWidget::onReloadDevPushButtonClicked(bool){
    // 设置当前任务
    _task=TASK_RELOAD;
    // 启动扫描
    DevPool::instance()->getBluetooth()->reloadDev(
        DevPool::instance()->getBodyStat());
    // 失效按钮
    _ui->_unpairPushButton->setDisabled(true);
    _ui->_scanDevPushButton->setDisabled(true);
    _ui->_reloadDevPushButton->setDisabled(true);
    _ui->_resetTimePushButton->setDisabled(true);
    // 显示进度条
    _ui->_progressLabel->show();
}

void DevInfoWidget::onResetTimePushButtonClicked(bool){
    if(!DevPool::instance()->getBodyStat()->getIsOpen()||
        !DevPool::instance()->getBodyStat()->getIsConnect()){
        QMessageBox msgBox(QMessageBox::Warning,
            tr("报警"),tr("设备尚未就绪！"));
        msgBox.setFont(font());
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setButtonText(QMessageBox::Ok,tr("确定"));
        msgBox.exec();
        return;
    }
    if(Bodystat::NoError!=Bodystat::BSWriteCurrentTime()){
        QMessageBox msgBox(QMessageBox::Warning,
            tr("报警"),tr("重置设备时间失败！"));
        msgBox.setFont(font());
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setButtonText(QMessageBox::Ok,tr("确定"));
        msgBox.exec();
    }else{
        QMessageBox msgBox(QMessageBox::Information,
            tr("提示"),tr("重置设备时间完成！"));
        msgBox.setFont(font());
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setButtonText(QMessageBox::Ok,tr("确定"));
        msgBox.exec();
    }
}

void DevInfoWidget::onUnauthDevPushButtonClicked(bool){
    // 设置当前任务
    _task=TASK_UNAUTH;
    // 取消配对
    DevPool::instance()->getBluetooth()->unauthDev(
        DevPool::instance()->getBodyStat());
    // 失效按钮
    _ui->_unpairPushButton->setDisabled(true);
    _ui->_scanDevPushButton->setDisabled(true);
    _ui->_reloadDevPushButton->setDisabled(true);
    _ui->_resetTimePushButton->setDisabled(true);
    // 显示进度条
    _ui->_progressLabel->show();
}

void DevInfoWidget::onBtDrivInfoChanged(){
    const QString drivInfo=DevPool::instance()->
        getBluetooth()->getDrivInfo();
    if(drivInfo.isEmpty()){
        _ui->_bluetoothLabel->setPixmap(
            QPixmap(":rc/bluetoothdisabled.png"));
    }else{
        _ui->_bluetoothLabel->setPixmap(
            QPixmap(":rc/bluetoothconnected.png"));
    }
    _ui->_btStackInfoLabel->setText(drivInfo);
}

void DevInfoWidget::onDevNameChanged(){
    _ui->_devModelLabel->setText(QString(tr(
        "设备模型: %1")).arg(DevPool::instance()->
        getBodyStat()->getName()));
}

void DevInfoWidget::onDevFirmwareVChanged(){
    _ui->_firmVerLabel->setText(QString(tr(
        "固件版本: %1")).arg(DevPool::instance()->
        getBodyStat()->getFirmwareVText()));
}

void DevInfoWidget::onDevSeriNumChanged(){
    _ui->_serialNoLabel->setText(QString(tr(
        "序列号: %1")).arg(DevPool::instance()->
        getBodyStat()->getSeriNum()));
}

void DevInfoWidget::onDevCalibDateChanged(){
    _ui->_calibDateLabel->setText(QString(tr(
        "校准日期: %1")).arg(DevPool::instance()->
        getBodyStat()->getCalibDateText()));
}

void DevInfoWidget::onBtTaskDone(
    const unsigned int /*id*/,const unsigned int err){
    // 重置当前任务
    _task=TASK_NONE;
    // 如果出错
    if(Bluetooth::TASK_ERR_NONE!=err){
        QMessageBox msgBox(
            QMessageBox::Warning,tr("警报"),
            Bluetooth::getTaskText(err));
        msgBox.setFont(font());
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setButtonText(QMessageBox::Ok,tr("确定"));
        msgBox.exec();
    }
    // 使能按钮
    _ui->_unpairPushButton->setDisabled(false);
    _ui->_scanDevPushButton->setDisabled(false);
    _ui->_reloadDevPushButton->setDisabled(false);
    _ui->_resetTimePushButton->setDisabled(false);
    // 隐藏进度条
    _ui->_progressLabel->hide();
}

void DevInfoWidget::initUi(){
    layout()->setSizeConstraint(QLayout::SetFixedSize);
    _ui->_bluetoothLabel->setPixmap(
        QPixmap(":rc/bluetoothdisabled.png"));
    _ui->_devLabel->setPixmap(
        QPixmap(":rc/Quadscan4000.bmp"));
    QMovie *movie=new QMovie(":rc/progress.gif",
        QByteArray(),_ui->_progressLabel);
    _ui->_progressLabel->setMovie(movie);
    movie->start();
    _ui->_progressLabel->hide();
    connect(_ui->_scanDevPushButton,SIGNAL(
        clicked(bool)),this,SLOT(
        onScanDevPushButtonClicked(bool)));
    connect(_ui->_reloadDevPushButton,
        SIGNAL(clicked(bool)),this,SLOT(
        onReloadDevPushButtonClicked(bool)));
    connect(_ui->_resetTimePushButton,
        SIGNAL(clicked(bool)),this,SLOT(
        onResetTimePushButtonClicked(bool)));
    connect(_ui->_unpairPushButton,
        SIGNAL(clicked(bool)),this,SLOT(
        onUnauthDevPushButtonClicked(bool)));
    connect(DevPool::instance()->getBluetooth(),
        SIGNAL(drivInfoChanged()),this,SLOT(
        onBtDrivInfoChanged()));
    connect(DevPool::instance()->getBluetooth(),
        SIGNAL(taskDone(const unsigned int,
        const unsigned int)),this,SLOT(
        onBtTaskDone(const unsigned int,
        const unsigned int)));
    connect(DevPool::instance()->getBodyStat(),
        SIGNAL(nameChanged()),this,SLOT(
        onDevNameChanged()));
    connect(DevPool::instance()->getBodyStat(),
        SIGNAL(firmwareVChanged()),this,SLOT(
        onDevFirmwareVChanged()));
    connect(DevPool::instance()->getBodyStat(),
        SIGNAL(seriNumChanged()),this,SLOT(
        onDevSeriNumChanged()));
    connect(DevPool::instance()->getBodyStat(),
        SIGNAL(calibDateChanged()),this,SLOT(
        onDevCalibDateChanged()));
}
