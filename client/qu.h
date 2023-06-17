#ifndef QU_H
#define QU_H


#include <QObject>
#include <QWidget>
#include <QThread>
extern "C" {
    #include "OSdll.h"
}

class Qu : public QThread{
    Q_OBJECT
private:
    QString message;
    ShareMemResponse response;
    char* mebuff;
    HANDLE* clientHandle;
protected:
  volatile bool m_stop;
  void run();
public:
  Qu(HANDLE* clientHandle,QObject *parent = 0);
  QString getMessage();
  void stop();
signals:
  void recvM();
  void cleanUI();
};

#endif // QU_H
