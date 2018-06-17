#define GLUT_DISABLE_ATEXIT_HACK
#include  <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL\GLUT.H>
//#define AutoRotate
//#define HandControl
#define Async
#define SerialControl
#define PORT       "COM5"
//#define Serial
//全局变量
HANDLE hCom;

BOOL OpenCom(){
	hCom = CreateFileA(
		PORT,
		GENERIC_READ|GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
#ifdef Async
		FILE_ATTRIBUTE_NORMAL |FILE_FLAG_OVERLAPPED,
#else
		0,
#endif
		NULL
		);
	if (hCom == INVALID_HANDLE_VALUE){
		printf("%s\n", "OPEN COM FAILED");
		return FALSE;
	}
	return TRUE;
}

void ConfigCom(){
	SetupComm(hCom, 4096, 4096);

	COMMTIMEOUTS comTimeOut;
	comTimeOut.ReadIntervalTimeout = 1000;
	comTimeOut.ReadTotalTimeoutMultiplier = 500;
	comTimeOut.ReadTotalTimeoutConstant = 1000;
	comTimeOut.WriteTotalTimeoutMultiplier = 5000;
	comTimeOut.WriteTotalTimeoutConstant = 1000;
	SetCommTimeouts(hCom, &comTimeOut);

	DCB dcb;
	GetCommState(hCom, &dcb);
	dcb.BaudRate = CBR_115200;
	dcb.ByteSize = 8;
	dcb.StopBits = ONESTOPBIT;
	dcb.Parity = NOPARITY;
	SetCommState(hCom, &dcb);

	PurgeComm(hCom, PURGE_RXCLEAR | PURGE_TXCLEAR);
}

//同步读
BOOL SyncRead(unsigned char InBuffer[], int InSize){
	DWORD InBufferSize = InSize;
	DWORD ActualReadBytes;
	BOOL ReadStatus;
	ReadStatus = ReadFile(hCom, InBuffer, InBufferSize, &ActualReadBytes, NULL);
	if (!ReadStatus){
		printf("%s\n", "READ FAILED");
		return FALSE;
	}
	return TRUE;
}
//同步写
BOOL SyncWrite(unsigned char OutBuffer[], int OutSize){
	DWORD OutBufferSize = OutSize;
	DWORD ActualWirteBytes;
	DWORD ErrorFlags;
	COMSTAT comStat;
	BOOL WriteStatus;
	ClearCommError(hCom, &ErrorFlags, &comStat);
	WriteStatus = WriteFile(hCom, OutBuffer, OutBufferSize, &ActualWirteBytes, 0);
	if (!WriteStatus){
		printf("%s\n", "WRITE FAILED");
		return FALSE;
	}
	return TRUE;
}
//异步读
DWORD AsyncRead(unsigned char InBuffer[], int InSize){
	DWORD ActualReadBytes = InSize;
	DWORD ErrorFlags;
	COMSTAT comStat;
	BOOL ReadStatus;

	OVERLAPPED overlapped;
	memset(&overlapped, 0, sizeof(OVERLAPPED));
	overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	ClearCommError(hCom,&ErrorFlags, &comStat);

	ActualReadBytes = min(ActualReadBytes, comStat.cbInQue);
	if (!ActualReadBytes){
		return 0;
	}
	ReadStatus = ReadFile(hCom, InBuffer, ActualReadBytes, &ActualReadBytes, &overlapped);
	if (!ReadStatus){
		if (GetLastError() == ERROR_IO_PENDING){
			WaitForSingleObject(overlapped.hEvent, 1000);
			PurgeComm(hCom, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_TXCLEAR);
			return ActualReadBytes;
		}
		return 0;
	}
	PurgeComm(hCom, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_TXCLEAR);
	return ActualReadBytes;
}
//异步写
DWORD AsyncWrite(unsigned char OutBuffer[], int OutSize){
	DWORD ActualWriteBytes = OutSize;
	DWORD ErrorFlags;
	COMSTAT comStat;
	BOOL WriteStatus;
	OVERLAPPED overlapped;
	memset(&overlapped, 0, sizeof(OVERLAPPED));
	overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	ClearCommError(hCom, &ErrorFlags, &comStat);

	WriteStatus = WriteFile(hCom, OutBuffer, ActualWriteBytes, &ActualWriteBytes, &overlapped);
	if (!WriteStatus){
		if (GetLastError() == ERROR_IO_PENDING){
			WaitForSingleObject(overlapped.hEvent, 2000);
			PurgeComm(hCom, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_TXCLEAR);
			return ActualWriteBytes;
		}
		return 0;
	}
	PurgeComm(hCom, PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXABORT | PURGE_TXCLEAR);
	return ActualWriteBytes;
}
void CloseCom(){
	CloseHandle(hCom);
}
#ifdef Serial
int main(){
	float Roll = 0.0f;
	float Pitch = 0.0f;
	float Yaw = 0.0f;
	OpenCom();
	ConfigCom();
	
    unsigned char Buffer[11];
	int Size = 11;
	unsigned char OutBuffer[3] = { 0xa5, 0x45, 0xea };
	int OutSize = 3;
#ifdef  Async
	AsyncWrite( OutBuffer,OutSize);
	int ActualBytes = 0;
	while (1){
		ActualBytes=AsyncRead(Buffer,11);
		if (ActualBytes>0){
				if (Buffer[0] == 0x5a && Buffer[1] == 0x5a){
					Roll =(short (Buffer[4]<<8|Buffer[5]));
					Roll = Roll*0.01f;
					Pitch =(short(Buffer[6]<<8|Buffer[7]));
					Pitch = Pitch*0.01f;
					Yaw = (short(Buffer[8]<<8|Buffer[9]));
					Yaw = Yaw*0.01f;
					printf("Roll: %f      Pitch: %f       Yaw:%f       \n", Roll, Pitch, Yaw);
				}
				
		}
		/*if (Buffer[0] == 0x5a && Buffer[1] == 0x5a){
			for (int i = 0; i < 11; i++){
				printf("%x  ", (unsigned char)Buffer[i]);
			}
			printf("\n");
		}*/
	}
#else
	SyncWrite(OutBuffer, OutSize);
	while (1){
		SyncRead(Buffer, 11);
		/*for (int i = 0; i < 11; i++){
			printf("%x  ",Buffer[i]);
		}*/
		if (Buffer[0] == 0x5a && Buffer[1] == 0x5a){
			Roll = (float)(Buffer[4] << 8 | Buffer[5]) / 100;
			Pitch = (float)(Buffer[6] << 8 | Buffer[7]) / 100;
			Yaw = (float)(Buffer[8] << 8 | Buffer[9]) / 100;
			printf("Roll: %f      Pitch: %f       Yaw:%f       ", Roll, Pitch, Yaw);
			printf("\n");
		}
	}
#endif
}
#endif
void Init(){
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);   //设置清除颜色值
	glClearDepth(1.0f);					 //设置清除深度值 [0,1]
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);   //激活深度测试，以隐藏被遮挡面
}
GLfloat Rotate;
GLfloat RotateRoll;
GLfloat RotateYaw;
GLfloat RotatePitch;

GLfloat Roll;
GLfloat Yaw;
GLfloat Pitch;

void display(){
	//清除
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -5.0f);
#ifdef AutoRotate
	glRotatef(Rotate, 1.0f, 0.0f, 0.0f);
#endif
#ifdef HandControl
	glRotatef(Roll, 1.0f, 0.0f, 0.0f);
	glRotatef(Yaw, 0.0f, 1.0f, 0.0f);
	glRotatef(Pitch, 0.0f, 0.0f, 1.0f);
#endif
#ifdef SerialControl
	glRotatef(RotateRoll, 1.0f, 0.0f, 0.0f);
	glRotatef(RotateYaw, 0.0f, 1.0f, 0.0f);
	glRotatef(RotatePitch, 0.0f, 0.0f, 1.0f);
#endif
	glBegin(GL_QUADS);

	//底面
	glColor3f(1.0f, 0.0f, 0.0f);

	glVertex3f(-1.0f, -1.0f, -1.0f);   //顶点顺序连接
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	//正面
	glColor3f(0.0f, 1.0f, 0.0f);

	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	//顶面
	glColor3f(0.0f, 0.0f, 1.0f);

	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	//背面
	glColor3f(0.5f, 0.0f, 0.0f);

	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	//左面
	glColor3f(0.0f, 0.5f, 0.0f);

	glVertex3f(-1.0f, 1.0f, -1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	//右面
	glColor3f(0.0f, 0.0f, 0.5f);

	glVertex3f(1.0f, 1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);

	glEnd();
#ifdef AutoRotate
	Rotate -= 0.1f;
#endif

#ifdef SerialControl
	float Roll = 0.0f;
	float Pitch = 0.0f;
	float Yaw = 0.0f;
	unsigned char Buffer[11];
	int Size = 11;
	int ActualBytes = 0;
	ActualBytes = AsyncRead(Buffer, 11);
	if (ActualBytes>0){
		if (Buffer[0] == 0x5a && Buffer[1] == 0x5a){
			Roll = (short(Buffer[4] << 8 | Buffer[5]));
			Roll = Roll*0.01f;
			Pitch = (short(Buffer[6] << 8 | Buffer[7]));
			Pitch = Pitch*0.01f;
			Yaw = (short(Buffer[8] << 8 | Buffer[9]));
			Yaw = Yaw*0.01f;

			RotateRoll = ceil(Roll);
			RotateYaw = ceil(Yaw);
			RotatePitch = ceil(Pitch);

			//printf("Roll: %f      Pitch: %f       Yaw:%f       \n", Roll, Pitch, Yaw);
		}

	}
#endif
	glutSwapBuffers();      //交换双缓存
}
void KEY_CONTROL(int key, int x, int y){
	switch (key){
	case GLUT_KEY_LEFT:Pitch-= 3.0f; break;
	case GLUT_KEY_DOWN:Roll-= 3.0f; break;
	case GLUT_KEY_RIGHT:Yaw-= 3.0f; break;
	default:break;
	}
}
void ReShape(GLsizei width, GLsizei height){
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLdouble)width / (GLdouble)height, 0.5f, 100.0f);
	glMatrixMode(GL_MODELVIEW);
}
int main(int argc, char* argv[]){
#ifdef SerialControl
	OpenCom();
	ConfigCom();
	unsigned char OutBuffer[3] = { 0xa5, 0x45, 0xea };
	int OutSize = 3;
	AsyncWrite(OutBuffer, OutSize);
#endif
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(1920 / 2, 1080 / 2);
	glutInitWindowSize(600, 600);
	glutInit(&argc, argv);
	glutCreateWindow("MyCube");
	Init();
	glutDisplayFunc(display);
	glutReshapeFunc(ReShape);
#ifdef HandControl
	glutSpecialFunc(KEY_CONTROL);
#endif
	glutIdleFunc(display);
	glutMainLoop();
	return 0;
}