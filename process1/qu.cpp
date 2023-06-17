#include "qu.h"

Qu::Qu(HANDLE* clientHandle,QObject *parent):QThread(parent){
    this->clientHandle = clientHandle;
    this->m_stop = false;
}

void Qu::stop(){
    this->m_stop = true;
  }

void Qu::run(){
    this->message = "屏幕将在3秒后清空\n";
    emit recvM();
    Sleep(3000);
    emit cleanUI();
    while(!m_stop){
        mebuff="";
        //getMessage(clientHandle,mebuff);
        if(strlen(mebuff)!=0)
                this->message = mebuff;
                emit recvM();
        }
    }

QString Qu::getMessage(){
    return this->message;
}
