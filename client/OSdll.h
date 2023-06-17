// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 OSDLL_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// OSDLL_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#include "Response.h"
#include<winsock2.h>
#include<Windows.h>

#ifdef OSDLL_EXPORTS
#define OSDLL_API __declspec(dllexport)
#else
#define OSDLL_API __declspec(dllimport)
#endif


OSDLL_API Response createSocket();
OSDLL_API Response createServerSocket();
OSDLL_API Response listenSocket(SOCKET* socketServer);
OSDLL_API SOCKET* acceptLink_Socket(SOCKET* socketServer);
OSDLL_API int sendMessage_Socket(SOCKET* clientSock, char* message);
OSDLL_API void closeSock(SOCKET* socketG);
OSDLL_API void closeSockS(SOCKET* serverSocket, SOCKET* clientSocket);
OSDLL_API Response connectLink_Socket(int sPort, SOCKET* clientSocket);
OSDLL_API char* getMessage_Socket(SOCKET* clientSocket, char* szBuffer);
OSDLL_API int getPort_Socket(SOCKET* serverSocket);
OSDLL_API Response createPipe(char* pipeName, HANDLE* pipeHandle);
OSDLL_API Response connectWaitPipe(HANDLE* pipeHandle);
OSDLL_API Response sendMessage_Pipe(HANDLE* pipeHandle, char* message);
OSDLL_API Response getMessage_Pipe(HANDLE* pipeHandle,char* message);
OSDLL_API Response closePipe(HANDLE pipeHandle);
OSDLL_API int connectPipe(char* pipeName, HANDLE* pipeHandle);
OSDLL_API HANDLE GetProcessHandle(char* lpName);
OSDLL_API HANDLE GetWindowHandle(char* lpName);
OSDLL_API ShareMemResponse creatFilemapping(char* sharememory);
OSDLL_API ShareMemResponse openFileMapping(char* sharememory);
OSDLL_API void CloseFileMap(ShareMemResponse response);
OSDLL_API void sendMessage_ShareMem(char* pBuff, char* msg);
OSDLL_API int postmessageq(HANDLE* clientHandle, char* message);
OSDLL_API int getmessage(HANDLE* clientHandle, char* message);
OSDLL_API HANDLE connectMail(char* mailName);
OSDLL_API int snedMail(HANDLE* mailHandle, char* message);
OSDLL_API HANDLE createMail(char* mailName);
OSDLL_API int getMessage(HANDLE* mailHandle, char* message);
