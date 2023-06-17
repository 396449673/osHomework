#ifndef SHARETHREAD_H
#define SHARETHREAD_H

#include <QObject>
#include <QWidget>
#include <QThread>
extern "C" {
    #include "OSdll.h"
}

class ShareThread : public QThread{
    Q_OBJECT
private:
    QString message;
    ShareMemResponse response;
    int trans;
    char* pBuff;
protected:
  volatile bool m_stop;
  void run();
public:
  ShareThread(char* pBuff,QObject *parent = 0);
  QString getMessage();
  void stop();
  void tranf();
signals:
  void recvM();
  void cleanUI();
};
#endif // SHARETHREAD_H
