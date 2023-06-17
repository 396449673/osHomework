#include "socketthread.h"
#include<string>
#include<QMessageBox>
SocketThread::SocketThread(SOCKET* serverSocket,QObject *parent):QThread(parent){
    this->serverSocket = serverSocket;
    this->m_stop = false;

  }

void SocketThread::stop(){
    this->m_stop = true;
  }

void SocketThread::run(){
    SOCKET* clientSocket = acceptLink_Socket(this->serverSocket);
    this->clientSocket=clientSocket;
    emit getC();
    this->message = "客户端连接成功！屏幕将在3秒后清空\n";
    emit recvM();
    Sleep(3000);
    emit cleanUI();
    while(!m_stop){
        char message[1024]={0};
        getMessage_Socket(clientSocket,message);
        if(strlen(message)!=0){
            this->message = message;
            emit recvM();
        }
    }
  }

QString SocketThread::getMessage(){
    return this->message;
}

SOCKET* SocketThread::getClient(){
    return clientSocket;
}
