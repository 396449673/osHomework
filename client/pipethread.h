#ifndef PIPETHREAD_H
#define PIPETHREAD_H

#include <QObject>
#include <QWidget>
#include <QThread>
extern "C" {
    #include "OSdll.h"
}

class PipeThread : public QThread{
    Q_OBJECT
private:
    QString message;
    HANDLE* pipeHandle;
    Response response;
    char mes[1024];
    int trans;
protected:
  volatile bool m_stop;
  void run();
public:
  PipeThread(HANDLE* pipeHandle,QObject *parent = 0);
  QString getMessage();
  void tranf();
  void stop();
signals:
  void recvM();
  void cleanUI();
};
#endif // PIPETHREAD_H
