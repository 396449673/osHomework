#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

#include <QObject>
#include <QWidget>
#include <QThread>
#include <winsock2.h>
extern "C" {
    #include "OSdll.h"
}

class SocketThread : public QThread{
    Q_OBJECT
private:
    SOCKET* serverSocket;
    QString message;
protected:
  volatile bool m_stop;
  void run();
public:
  SocketThread(SOCKET* serverSocket,QObject *parent = 0);
  QString getMessage();
  void stop();

signals:
  void recvM();
  void cleanUI();
};

#endif // SOCKETTHREAD_H
