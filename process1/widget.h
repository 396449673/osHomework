#ifndef WIDGET_H
#define WIDGET_H
#include<winsock2.h>
#include<windows.h>
#include <QWidget>
#include<QMessageBox>
#include<QString>
#include<winuser.h>
#include "socketthread.h"
#include "pipethread.h"
#include "sharethread.h"
#include "Response.h"
#include"qu.h"
extern "C" {
    #include "OSdll.h"
}

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void processSocket(SOCKET* serverSocket);
    bool startSocket();

    bool createPipeServer();

    bool createShare();

    bool connectClient();
public slots:
    void appendMessage();
    void cleanUI();
private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();
    void on_comboBox_currentIndexChanged(int index);
    void getC();
private:
    Ui::Widget *ui;

    int index;

    SOCKET* clientSocket;
    SOCKET* serverSocket;
    SocketThread *socketThread;

    HANDLE pipeHandle;
    PipeThread *pipeThread;

    ShareThread* shareThread;
    char* pBuff;

    HANDLE *clientHandle;
    Qu* quthread;
};
#endif // WIDGET_H
