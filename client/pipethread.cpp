#include "pipethread.h"

PipeThread::PipeThread(HANDLE* pipeHandle,QObject *parent):QThread(parent){
    trans = 1;
    this->pipeHandle=pipeHandle;
    this->m_stop = false;
}

void PipeThread::stop(){
    this->m_stop = true;
  }

void PipeThread::run(){
    this->message = "屏幕将在3秒后清空\n";
    emit recvM();
    Sleep(3000);
    emit cleanUI();
    while(!m_stop){
        if(trans==1){
            mes[1023]={0};
            response = getMessage_Pipe(pipeHandle,mes);
            if(response.code!=-1){
                this->message = mes;
                emit recvM();
                tranf();
            }
        }
    }
  }

QString PipeThread::getMessage(){
    return this->message;
}
void PipeThread::tranf(){
    trans=!trans;
}
