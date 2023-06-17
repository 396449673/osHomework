#include "sharethread.h"

ShareThread::ShareThread(char* pBuff,QObject *parent):QThread(parent){
    this->pBuff = pBuff;
    trans = 0;
    this->m_stop = false;
}

void ShareThread::stop(){
    this->m_stop = true;
  }

void ShareThread::run(){
    this->message = "屏幕将在3秒后清空\n";
    emit recvM();
    Sleep(3000);
    emit cleanUI();
    while(!m_stop){
        if(trans==1){
            if(pBuff[0] !='\0'){
                this->message = pBuff;
                memset(pBuff, '\0', 1024);
                emit recvM();
                tranf();
            }
        }
    }
  }

QString ShareThread::getMessage(){
    return this->message;
}

void ShareThread::tranf(){
    trans=!trans;
}
