#include "widget.h"
#include "ui_widget.h"
#include<QMessageBox>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->index=0;
    ui->pushButton_2->setEnabled(false);
    ui->textEdit_2->setEnabled(false);
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    switch(index){
    case 0:{
        QString portS = ui->textEdit->toPlainText();
        if(!isDigitStr(portS)||ui->textEdit->toPlainText()==""){
            QMessageBox::critical(NULL,"错误","请输入正确的端口号");
            ui->textEdit->clear();
            return;
        }
        int port = portS.toInt();
        ui->pushButton->setEnabled(true);
        ui->pushButton_2->setEnabled(false);
        if(connectServer(port)){
            socketThread = new SocketThread(serverSocket);
            socketThread->start();
            connect(socketThread,&SocketThread::cleanUI,this,&Widget::cleanUI);
            connect(socketThread,&SocketThread::recvM,this,&Widget::appendMessage);
            ui->textEdit_2->setEnabled(true);
            ui->comboBox->setEnabled(false);
        }
        break;
    }
    case 1:{
        if(startConnectPipe()){
            pipeThread->start();
            connect(pipeThread,&PipeThread::cleanUI,this,&Widget::cleanUI);
            connect(pipeThread,&PipeThread::recvM,this,&Widget::appendMessage);
            ui->pushButton_2->setEnabled(false);
            ui->comboBox->setEnabled(false);
            ui->textEdit->setEnabled(false);
            ui->pushButton->setEnabled(false);
            ui->textEdit_2->setEnabled(true);
        }
        break;
    }
    case 2:{
        if(connectShare()){
            shareThread->start();
            connect(shareThread,&ShareThread::cleanUI,this,&Widget::cleanUI);
            connect(shareThread,&ShareThread::recvM,this,&Widget::appendMessage);
            ui->pushButton_2->setEnabled(false);
            ui->comboBox->setEnabled(false);
            ui->textEdit->setEnabled(false);
            ui->pushButton->setEnabled(false);
            ui->textEdit_2->setEnabled(true);
        }
        break;
    }
    default:{
        if(connectServerq()){
            quthread->start();
            connect(quthread,&Qu::cleanUI,this,&Widget::cleanUI);
            connect(quthread,&Qu::recvM,this,&Widget::appendMessage);
            ui->pushButton_2->setEnabled(false);
            ui->comboBox->setEnabled(false);
            ui->textEdit->setEnabled(false);
            ui->pushButton->setEnabled(false);
            ui->textEdit_2->setEnabled(true);
        }
        break;
    }
    }


}

void Widget::on_pushButton_2_clicked(){
    std::string lcmd =ui->textEdit_2->toPlainText().toStdString();
    if(lcmd==""){
        return;
    }
    char* c2 = lcmd.data();
    bool success=false;
    ui->textBrowser->append("Send:"+QString(c2)+"\n");
    switch(index){
    case 0:{
        int rc = sendMessage_Socket(this->serverSocket,c2);
        if(rc==-1){
            ui->textBrowser->append("发送失败，请检查Socket!\n");
        }
        else
            success=true;
        break;
    }
    case 1:{
        Response response = sendMessage_Pipe(&pipeHandle,c2);
        if(response.code==-1){
            ui->textBrowser->append("发送失败，请检查Pipe!\n");
        }
        else{
            pipeThread->tranf();
            ui->pushButton_2->setEnabled(false);
            success = true;
        }
        break;
    }
    case 2:{
        sendMessage_ShareMem(pBuff,c2);
        ui->pushButton_2->setEnabled(false);
        success = true;
        shareThread->tranf();
        break;
    }
    default:{
        int rc = postmessageq(serverHandle,c2);
        if(rc == 0){
            ui->textBrowser->append("发送失败，请检查!\n");
        }
        else
            success = true;
        break;
    }
    }
    if(success)
        ui->textEdit_2->clear();



}

void Widget::cleanUI(){
    ui->textBrowser->clear();
}
void Widget::appendMessage(){
    switch(index){
        case 0:{
            ui->textBrowser->append("Recv:"+socketThread->getMessage()+"\n");
            break;
        }
        case 1:{
            ui->textBrowser->append("Recv:"+pipeThread->getMessage()+"\n");
            ui->pushButton_2->setEnabled(true);
            break;
        }
        case 2:{
            ui->textBrowser->append("Recv:"+shareThread->getMessage()+"\n");
            ui->pushButton_2->setEnabled(true);
            break;
        }
        default:{
            ui->textBrowser->append("Recv:"+quthread->getMessage()+"\n");
            ui->pushButton_2->setEnabled(true);
            break;
        }
        }
}

bool Widget::connectServer(int port){
    ui->textBrowser->clear();
    ui->textBrowser->append("正在创建Socket!\n");
    Response response=createSocket();
    if(response.code == -1){
        ui->textBrowser->append("创建Socket失败，请重试"+QString(response.result)+"\n");
        return false;
    }ui->textBrowser->append("创建Socket成功!\n");
    ui->textBrowser->append("正在连接Socket!\n");
    serverSocket = (SOCKET*)response.object;
    response = connectLink_Socket(port,serverSocket);
    if(response.code==-1){
         ui->textBrowser->append("连接Socket失败，请重试"+QString(response.result)+"\n");
         return false;
    }
    this->serverSocket =(SOCKET*)response.object;
    ui->textBrowser->append("连接Socket成功!\n");
    ui->textEdit->setEnabled(false);
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(true);
    ui->comboBox->setEnabled(false);
    return true;
}

bool Widget::isDigitStr(QString src)  {
    QByteArray ba = src.toLatin1();
    const char *s = ba.data();
    while(*s && *s>='0' && *s<='9') s++;
    if (*s){
        return false;
    }
    else{
        return true;
    }
}

bool Widget::startConnectPipe(){
    ui->textBrowser->clear();
    std::string pipeName = "\\\\.\\pipe\\"+ui->textEdit->toPlainText().toStdString();
    if(pipeName=="\\\\.\\pipe\\"){
        QMessageBox::critical(NULL,"错误","请输入正确的管道名称!");
        return false;
    }
    char* c2 = pipeName.data();
    ui->textBrowser->append("正在连接管道服务!\n");
    if(connectPipe(c2,&pipeHandle)!=-1){
        ui->textBrowser->append("连接Pipe成功！\n");
        ui->textBrowser->append("等待对方发送消息\n");
        ui->pushButton_2->setEnabled(false);
        pipeThread = new PipeThread(&pipeHandle);
        return true;
    }
    return false;
}

bool Widget::connectShare(){
    ui->textBrowser->clear();
    std::string shareName = ui->textEdit->toPlainText().toStdString();
    if(shareName==""){
        QMessageBox::critical(NULL,"错误","请输入正确的共享存储区名称!");
        return false;
    }
    char* c2 = shareName.data();
    ui->textBrowser->append("正在连接共享存储区服务!\n");
    ShareMemResponse response = openFileMapping(c2);
    if(response.errorCode!=-1){
        ui->textBrowser->append("连接共享存储区成功！\n");
        ui->textBrowser->append("等待对方发送消息\n");
        ui->pushButton_2->setEnabled(false);
        this->pBuff=response.Buff;
        shareThread = new ShareThread(this->pBuff);
        return true;
    }
    return false;
}

bool Widget::connectServerq(){
    ui->textBrowser->clear();
    std::string sharemem =ui->textEdit->toPlainText().toStdString();
    if(sharemem==""){
        QMessageBox::critical(NULL,"错误","请输入正确的服务端名称!");
        return false;
    }
    char* c2 = sharemem.data();
    *serverHandle = GetWindowHandle(c2);
    if(*serverHandle == NULL){
        ui->textBrowser->append("出错，请重试！ \n");
        QMessageBox::critical(NULL,"错误","找不到服务端");
        return false;
    }
    return true;
}
void Widget::on_comboBox_currentIndexChanged(int index)
{
    this->index=index;

}

