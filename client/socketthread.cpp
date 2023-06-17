#include "socketthread.h"
#include<QDebug>
#include<QMessageBox>
SocketThread::SocketThread(SOCKET* serverSocket,QObject *parent):QThread(parent){
    this->serverSocket = serverSocket;
    this->m_stop = false;
  }

void SocketThread::stop(){
    this->m_stop = true;
  }

void SocketThread::run(){
    this->message = "屏幕将在3秒后清空\n";
    emit recvM();
    Sleep(3000);
    emit cleanUI();
    while(!m_stop){
        char message[MAXBYTE]="";
        getMessage_Socket(serverSocket,message);
        if(strlen(message)!=0){
            this->message = message;
            emit recvM();
        }
    }
  }

QString SocketThread::getMessage(){
    return this->message;
}
