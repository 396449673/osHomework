#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->textEdit_2->setEnabled(false);
    ui->pushButton->setEnabled(false);
    this->index=0;
}

Widget::~Widget()
{
    delete ui;
}



void Widget::appendMessage(){
    switch(index){
    case 0:{
        ui->textBrowser->append("Recv:"+socketThread->getMessage()+"\n");
        break;
    }
    case 1:{
        ui->textBrowser->append("Recv:"+pipeThread->getMessage()+"\n");
        ui->pushButton->setEnabled(true);
        break;
    }
    case 2:{
        ui->textBrowser->append("Recv:"+shareThread->getMessage()+"\n");
        ui->pushButton->setEnabled(true);
        break;
    }
    default:{
        ui->textBrowser->append("Recv:"+quthread->getMessage()+"\n");
        ui->pushButton->setEnabled(true);
        break;
    }
    }

}

void Widget::cleanUI(){
    ui->textBrowser->clear();
}
void Widget::getC(){
    switch(index){
    case 0:{
        this->clientSocket = socketThread->getClient();
        break;
    }
    case 1:{
        break;
    }
    case 2:{
        break;
    }
    default:{
        break;
    }
    }
}

void Widget::on_pushButton_2_clicked()
{
    switch(index){
    case 0:{
        if(startSocket()){
            connect(socketThread,&SocketThread::cleanUI,this,&Widget::cleanUI);
            connect(socketThread,&SocketThread::recvM,this,&Widget::appendMessage);
            connect(socketThread,&SocketThread::getC,this,&Widget::getC);
            ui->comboBox->setEnabled(false);
        }
        break;
    }
    case 1:{
        if(createPipeServer()){
            connect(pipeThread,&PipeThread::cleanUI,this,&Widget::cleanUI);
            connect(pipeThread,&PipeThread::recvM,this,&Widget::appendMessage);
            pipeThread->start();
            ui->comboBox->setEnabled(false);
            ui->textEdit_2->setEnabled(false);
            ui->pushButton_2->setEnabled(false);
            ui->textEdit->setEnabled(true);
            ui->pushButton->setEnabled(true);
        }
        break;
    }
    case 2:{
        if(createShare()){
            connect(shareThread,&ShareThread::cleanUI,this,&Widget::cleanUI);
            connect(shareThread,&ShareThread::recvM,this,&Widget::appendMessage);
            shareThread->start();
            ui->comboBox->setEnabled(false);
            ui->textEdit_2->setEnabled(false);
            ui->pushButton_2->setEnabled(false);
            ui->textEdit->setEnabled(true);
            ui->pushButton->setEnabled(true);
        }
        break;
    }
    default:{
        if(connectClient()){
            connect(quthread,&Qu::cleanUI,this,&Widget::cleanUI);
            connect(quthread,&Qu::recvM,this,&Widget::appendMessage);
            quthread->start();
            ui->comboBox->setEnabled(false);
            ui->textEdit_2->setEnabled(false);
            ui->pushButton_2->setEnabled(false);
            ui->textEdit->setEnabled(true);
            ui->pushButton->setEnabled(true);
        }
        break;
    }
    }
}


void Widget::on_pushButton_clicked()
{
    std::string lcmd =ui->textEdit->toPlainText().toStdString();
    if(lcmd==""){
        return;
    }
    char* c2 = lcmd.data();
    bool success=false;
    ui->textBrowser->append("Send:"+QString(c2)+"\n");
    switch(index){
    case 0:{
        int rc = sendMessage_Socket(this->clientSocket,c2);
        if(rc==-1){
            ui->textBrowser->append("发送失败，请检查Socket!\n");
        }
        else
            success = true;
        break;
    }
    case 1:{
        Response response = sendMessage_Pipe(&pipeHandle,c2);
        if(response.code==-1){
            ui->textBrowser->append("发送失败，请检查Pipe!\n");
        }
        else{
            pipeThread->tranf();
            ui->pushButton->setEnabled(false);
            success = true;
        }
        break;
    }
    case 2:{
        sendMessage_ShareMem(pBuff,c2);
        ui->pushButton->setEnabled(false);
        success = true;
        shareThread->tranf();
        break;
    }
    default:{
        int rc = postmessageq(clientHandle,c2);
        if(rc == 0){
            ui->textBrowser->append("发送失败，请检查!\n");
        }
        else
            success = true;
        break;
    }
    }
    if(success)
        ui->textEdit->clear();

}


void Widget::on_comboBox_currentIndexChanged(int index)
{
    this->index= index;
    switch(index){
    case 0:{
        ui->pushButton_2->setEnabled(true);
        ui->textEdit_2->setEnabled(false);
        ui->pushButton->setEnabled(false);
        break;
    }
    case 1:{
        ui->pushButton_2->setEnabled(true);
        ui->textEdit_2->setEnabled(true);
        ui->pushButton->setEnabled(false);
        break;
    }
    case 2:{
        ui->pushButton_2->setEnabled(true);
        ui->textEdit_2->setEnabled(true);
        ui->pushButton->setEnabled(false);
        break;
    }
    default:{
        ui->pushButton_2->setEnabled(true);
        ui->textEdit_2->setEnabled(true);
        ui->pushButton->setEnabled(false);
        break;
    }
    }
}

bool Widget::startSocket(){
    processSocket(serverSocket);
    if(serverSocket == NULL){
        return false;
    }
    socketThread= new SocketThread(serverSocket);
    socketThread->start();
    return true;
}

void Widget::processSocket(SOCKET* serverSocket){
    ui->textBrowser->clear();
    ui->textBrowser->append("正在创建Socket！\n");
    ui->pushButton_2->setDisabled(true);
    ui->pushButton->setDisabled(false);
    Response response = createServerSocket();
    if(response.code == -1){
        ui->textBrowser->append("创建socket出错，请重试！ 错误原因："+QString(response.result)+"\n");
        QMessageBox::critical(NULL,"错误",response.result);
        return ;
    }
    ui->textBrowser->append("创建Socket成功！\n");
    ui->textBrowser->append("正在监听Socket！\n");
    serverSocket = (SOCKET*)response.object;
    this->serverSocket = (SOCKET*)response.object;
    response = listenSocket(serverSocket);
    if(response.code == -1){
        ui->textBrowser->append("监听socket出错，请重试！ 错误原因："+QString::fromUtf8(response.result)+"\n");
        QMessageBox::critical(NULL,"错误",response.result);
        return ;
    }

    ui->textBrowser->append("监听Socket成功！\n");
    int port = getPort_Socket(serverSocket);
    ui->textBrowser->append("Socket端口为"+QString::number(port)+"！\n");
    ui->textBrowser->append("等待客户端连接...\n");
    ui->pushButton_2->setEnabled(false);
    ui->pushButton->setEnabled(true);
    return ;
}

bool Widget::createPipeServer(){
//    pipeHandle=std::malloc(sizeof (HANDLE));
    ui->textBrowser->clear();
    std::string pipeName = "\\\\.\\pipe\\"+ui->textEdit_2->toPlainText().toStdString();
    if(pipeName=="\\\\.\\pipe\\"){
        QMessageBox::critical(NULL,"错误","请输入正确的管道名称!");
        return false;
    }
    char* c2 = pipeName.data();
    ui->textBrowser->append("正在创建管道服务!\n");
    Response response= createPipe(c2,&pipeHandle);
    if(response.code == -1){
        ui->textBrowser->append("创建Pipe出错，请重试！ 错误原因："+QString::fromUtf8(response.result)+"\n");
        QMessageBox::critical(NULL,"错误",response.result);
        return false;
    }
    ui->textBrowser->append("创建Pipe成功！\n");
    ui->textBrowser->append("等待客户端连接...\n");
    pipeThread = new PipeThread(&pipeHandle);
    return true;
}

bool Widget::createShare(){
    ui->textBrowser->clear();
    std::string sharemem =ui->textEdit_2->toPlainText().toStdString();
    if(sharemem==""){
        QMessageBox::critical(NULL,"错误","请输入正确的共享存储区名称!");
        return false;
    }
    char* c2 = sharemem.data();
    ui->textBrowser->append("正在创建共享存储区!\n");
    ShareMemResponse response= creatFilemapping(c2);
    if(response.errorCode == -1){
        ui->textBrowser->append("创建共享存储区出错，请重试！ 错误原因："+QString::fromUtf8(response.errorResult)+"\n");
        QMessageBox::critical(NULL,"错误",response.errorResult);
        return false;
    }
    this->pBuff=response.Buff;
    memset(pBuff, '\0', 1024);
    ui->textBrowser->append("创建共享存储区成功！\n");
    shareThread = new ShareThread(this->pBuff);
    return true;
}

bool Widget::connectClient(){
    ui->textBrowser->clear();
    std::string sharemem =ui->textEdit_2->toPlainText().toStdString();
    if(sharemem==""){
        QMessageBox::critical(NULL,"错误","请输入正确的客户端名称!");
        return false;
    }
    char* c2 = sharemem.data();
    *clientHandle = GetWindowHandle(c2);
    if(*clientHandle == NULL){
        ui->textBrowser->append("出错，请重试！ \n");
        QMessageBox::critical(NULL,"错误","找不到客户端");
        return false;
    }
    return true;
}
