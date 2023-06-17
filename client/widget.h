#ifndef WIDGET_H
#define WIDGET_H
#include<winsock2.h>
#include <QWidget>
#include"Response.h"
#include"socketthread.h"
#include"pipethread.h"
#include "sharethread.h"
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
    bool connectServer(int port);
    bool startConnectPipe();
    bool connectShare();
    bool connectServerq();
public slots:
    void appendMessage();
    void cleanUI();
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::Widget *ui;
    int index;

    bool isDigitStr(QString src);
    SOCKET* serverSocket;
    SocketThread* socketThread;

    HANDLE pipeHandle;
    PipeThread *pipeThread;

    ShareThread* shareThread;
    char* pBuff;

    HANDLE* serverHandle;
    Qu* quthread;
};
#endif // WIDGET_H
