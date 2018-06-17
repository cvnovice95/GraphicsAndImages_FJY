#define GLUT_DISABLE_ATEXIT_HACK
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <GL/glut.h>
#define RightF 100       //d
#define LeftF  101       //e
#define UpL    102       //f
#define DownL  103       //g
#define FRight 104       //h
#define BRight 105       //i

//#define AutoRotate
#define HandControl
void Init(){
	glClearColor(1.0f,1.0f,1.0f,1.0f);   //设置清除颜色值
	glClearDepth(1.0f);					 //设置清除深度值 [0,1]
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);   //激活深度测试，以隐藏被遮挡面
}
GLfloat Rotate;

GLfloat RotateRoll;
GLfloat RotateYall;

int Area[8]={0,1,2,3,4,5,6,7};
struct AreaRotateBlock{
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

struct AreaRotateBlock RotateBlock[8];

void display(){
	//清除
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,-5.0f);
#ifdef AutoRotate
	glRotatef(Rotate,1.0f,0.0f,0.0f);
#endif
#ifdef HandControl
	glRotatef(RotateRoll,1.0f,0.0f,0.0f);
	glRotatef(RotateYall,0.0f,1.0f,0.0f);
#endif
	glBegin(GL_QUADS);
		
		//底面
		glColor3f(1.0f,0.0f,0.0f);

		glVertex3f(-1.0f,-1.0f,-1.0f);   //顶点顺序连接
		glVertex3f(1.0f,-1.0f,-1.0f);
		glVertex3f(1.0f,-1.0f,1.0f);
		glVertex3f(-1.0f,-1.0f,1.0f);
		//正面
		glColor3f(0.0f,1.0f,0.0f);

		glVertex3f(-1.0f,1.0f,1.0f);
		glVertex3f(1.0f,1.0f,1.0f);
		glVertex3f(1.0f,-1.0f,1.0f);
		glVertex3f(-1.0f,-1.0f,1.0f);
		//顶面
		glColor3f(0.0f,0.0f,1.0f);

        glVertex3f(-1.0f,1.0f,-1.0f);
		glVertex3f(1.0f,1.0f,-1.0f);
		glVertex3f(1.0f,1.0f,1.0f);
		glVertex3f(-1.0f,1.0f,1.0f);
		//背面
		glColor3f(0.5f,0.0f,0.0f);

        glVertex3f(-1.0f,1.0f,-1.0f);
		glVertex3f(1.0f,1.0f,-1.0f);
		glVertex3f(1.0f,-1.0f,-1.0f);
		glVertex3f(-1.0f,-1.0f,-1.0f);
		//左面
		glColor3f(0.0f,0.5f,0.0f);

        glVertex3f(-1.0f,1.0f,-1.0f);
		glVertex3f(-1.0f,1.0f,1.0f);
		glVertex3f(-1.0f,-1.0f,1.0f);
		glVertex3f(-1.0f,-1.0f,-1.0f);
		//右面
		glColor3f(0.0f,0.0f,0.5f);

        glVertex3f(1.0f,1.0f,-1.0f);
		glVertex3f(1.0f,-1.0f,-1.0f);
		glVertex3f(1.0f,-1.0f,1.0f);
		glVertex3f(1.0f,1.0f,1.0f);

	glEnd();
#ifdef AutoRotate
    Rotate-=0.1f;
#endif
    glutSwapBuffers();      //交换双缓存
}
void CubeRightUpF(){
	//glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
    glTranslatef(0.0f,0.0f,-5.0f);
    
    glRotatef(RotateBlock[0].x,1.0f,0.0f,0.0f);    //0
    glRotatef(RotateBlock[0].y,0.0f,1.0f,0.0f);
    glRotatef(RotateBlock[0].z,0.0f,0.0f,1.0f);
    
   

    glBegin(GL_QUADS);
    	glColor3f(1.0f,0.0f,0.0f);  //red
    	//正面
    	glVertex3f(0.0f,1.0f,1.0f);
    	glVertex3f(1.0f,1.0f,1.0f);
    	glVertex3f(1.0f,0.0f,1.0f);
    	glVertex3f(0.0f,0.0f,1.0f);

    	glColor3f(0.0f,1.0f,0.0f); //green
    	//顶面
    	glVertex3f(0.0f,1.0f,0.0f);
    	glVertex3f(1.0f,1.0f,0.0f);
    	glVertex3f(1.0f,1.0f,1.0f);
    	glVertex3f(0.0f,1.0f,1.0f);

    	glColor3f(0.0f,0.0f,1.0f); //blue
    	//右面
    	glVertex3f(1.0f,1.0f,0.0f);
    	glVertex3f(1.0f,0.0f,0.0f);
    	glVertex3f(1.0f,0.0f,1.0f);
    	glVertex3f(1.0f,1.0f,1.0f);

    	glColor3f(0.0f,0.0f,0.0f);  //black
    	//背面
    	glVertex3f(0.0f,1.0f,0.0f);
    	glVertex3f(1.0f,1.0f,0.0f);
    	glVertex3f(1.0f,0.0f,0.0f);
    	glVertex3f(0.0f,0.0f,0.0f);

    	glColor3f(0.0f,0.0f,0.0f);
    	//左面
    	glVertex3f(0.0f,1.0f,0.0f);
    	glVertex3f(0.0f,1.0f,1.0f);
    	glVertex3f(0.0f,0.0f,1.0f);
    	glVertex3f(0.0f,0.0f,0.0f);

    	glColor3f(0.0f,0.0f,0.0f);
    	//底面
    	glVertex3f(0.0f,0.0f,0.0f);
    	glVertex3f(1.0f,0.0f,0.0f);
    	glVertex3f(1.0f,0.0f,1.0f);
    	glVertex3f(0.0f,0.0f,1.0f);
    glEnd();
    //glutSwapBuffers();
}
void CubeRightUpB(){
	//glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
    glTranslatef(0.0f,0.0f,-5.0f);

    glRotatef(RotateBlock[1].x,1.0f,0.0f,0.0f);     //1
    glRotatef(RotateBlock[1].y,0.0f,1.0f,0.0f);
    glRotatef(RotateBlock[1].z,0.0f,0.0f,1.0f);
 

    glBegin(GL_QUADS);
    	glColor3f(0.0f,0.0f,0.0f);  //black
    	//正面
    	glVertex3f(0.0f,1.0f,0.0f);
    	glVertex3f(1.0f,1.0f,0.0f);
    	glVertex3f(1.0f,0.0f,0.0f);
    	glVertex3f(0.0f,0.0f,0.0f);

    	glColor3f(0.0f,1.0f,0.0f); //green
    	//顶面
    	glVertex3f(0.0f,1.0f,-1.0f);
    	glVertex3f(1.0f,1.0f,-1.0f);
    	glVertex3f(1.0f,1.0f,0.0f);
    	glVertex3f(0.0f,1.0f,0.0f);

    	glColor3f(0.0f,0.0f,1.0f); //blue
    	//右面
    	glVertex3f(1.0f,1.0f,-1.0f);
    	glVertex3f(1.0f,0.0f,-1.0f);
    	glVertex3f(1.0f,0.0f,0.0f);
    	glVertex3f(1.0f,1.0f,0.0f);

    	glColor3f(0.5f,0.0f,0.0f);  
    	//背面
    	glVertex3f(0.0f,1.0f,-1.0f);
    	glVertex3f(1.0f,1.0f,-1.0f);
    	glVertex3f(1.0f,0.0f,-1.0f);
    	glVertex3f(0.0f,0.0f,-1.0f);

    	glColor3f(0.0f,0.0f,0.0f);
    	//左面
    	glVertex3f(0.0f,1.0f,-1.0f);
    	glVertex3f(0.0f,0.0f,-1.0f);
    	glVertex3f(0.0f,0.0f,0.0f);
    	glVertex3f(0.0f,1.0f,0.0f);

    	glColor3f(0.0f,0.0f,0.0f);
    	//底面
    	glVertex3f(0.0f,0.0f,-1.0f);
    	glVertex3f(1.0f,0.0f,-1.0f);
    	glVertex3f(1.0f,0.0f,0.0f);
    	glVertex3f(0.0f,0.0f,0.0f);
    glEnd();
    //glutSwapBuffers();
}
void CubeLeftUpF(){
	//glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
    glTranslatef(0.0f,0.0f,-5.0f);
    
    glRotatef(RotateBlock[3].x,1.0f,0.0f,0.0f);     //3
    glRotatef(RotateBlock[3].y,0.0f,1.0f,0.0f);
    glRotatef(RotateBlock[3].z,0.0f,0.0f,1.0f);
   

    glBegin(GL_QUADS);
    	glColor3f(1.0f,0.0f,0.0f);  //red
    	//正面
    	glVertex3f(-1.0f,1.0f,1.0f);
    	glVertex3f(0.0f,1.0f,1.0f);
    	glVertex3f(0.0f,0.0f,1.0f);
    	glVertex3f(-1.0f,0.0f,1.0f);

    	glColor3f(0.0f,1.0f,0.0f); //green
    	//顶面
    	glVertex3f(-1.0f,1.0f,0.0f);
    	glVertex3f(0.0f,1.0f,0.0f);
    	glVertex3f(0.0f,1.0f,1.0f);
    	glVertex3f(-1.0f,1.0f,1.0f);

    	glColor3f(0.0f,0.0f,0.0f); //black
    	//右面
    	glVertex3f(0.0f,1.0f,0.0f);
    	glVertex3f(0.0f,0.0f,0.0f);
    	glVertex3f(0.0f,0.0f,1.0f);
    	glVertex3f(0.0f,1.0f,1.0f);

    	glColor3f(0.5f,0.0f,0.0f);  
    	//背面
    	glVertex3f(-1.0f,1.0f,0.0f);
    	glVertex3f(0.0f,1.0f,0.0f);
    	glVertex3f(0.0f,0.0f,0.0f);
    	glVertex3f(-1.0f,0.0f,0.0f);

    	glColor3f(0.0f,0.0f,0.5f);
    	//左面
    	glVertex3f(-1.0f,1.0f,0.0f);
    	glVertex3f(-1.0f,1.0f,1.0f);
    	glVertex3f(-1.0f,0.0f,1.0f);
    	glVertex3f(-1.0f,0.0f,0.0f);

    	glColor3f(0.0f,0.0f,0.0f);
    	//底面
    	glVertex3f(-1.0f,0.0f,0.0f);
    	glVertex3f(0.0f,0.0f,0.0f);
    	glVertex3f(0.0f,0.0f,1.0f);
    	glVertex3f(-1.0f,0.0f,1.0f);
    glEnd();
    //glutSwapBuffers();
}
void CubeLeftUpB(){
	//glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
    glTranslatef(0.0f,0.0f,-5.0f);

    glRotatef(RotateBlock[2].x,1.0f,0.0f,0.0f);    //2
    glRotatef(RotateBlock[2].y,0.0f,1.0f,0.0f);
    glRotatef(RotateBlock[2].z,0.0f,0.0f,1.0f);
   

    glBegin(GL_QUADS);
    	glColor3f(0.0f,0.0f,0.0f);  //black
    	//正面
    	glVertex3f(-1.0f,1.0f,0.0f);
    	glVertex3f(0.0f,1.0f,0.0f);
    	glVertex3f(0.0f,0.0f,0.0f);
    	glVertex3f(-1.0f,0.0f,0.0f);

    	glColor3f(0.0f,1.0f,0.0f); //green
    	//顶面
    	glVertex3f(-1.0f,1.0f,-1.0f);
    	glVertex3f(0.0f,1.0f,-1.0f);
    	glVertex3f(0.0f,1.0f,0.0f);
    	glVertex3f(-1.0f,1.0f,0.0f);

    	glColor3f(0.0f,0.0f,0.0f); //black
    	//右面
    	glVertex3f(0.0f,1.0f,-1.0f);
    	glVertex3f(0.0f,0.0f,-1.0f);
    	glVertex3f(0.0f,0.0f,0.0f);
    	glVertex3f(0.0f,1.0f,0.0f);

    	glColor3f(0.5f,0.0f,0.0f);  
    	//背面
    	glVertex3f(-1.0f,1.0f,-1.0f);
    	glVertex3f(0.0f,1.0f,-1.0f);
    	glVertex3f(0.0f,0.0f,-1.0f);
    	glVertex3f(-1.0f,0.0f,-1.0f);

    	glColor3f(0.0f,0.0f,0.5f);
    	//左面
    	glVertex3f(-1.0f,1.0f,-1.0f);
    	glVertex3f(-1.0f,0.0f,-1.0f);
    	glVertex3f(-1.0f,0.0f,0.0f);
    	glVertex3f(-1.0f,1.0f,0.0f);

    	glColor3f(0.0f,0.0f,0.0f);
    	//底面
    	glVertex3f(-1.0f,0.0f,-1.0f);
    	glVertex3f(0.0f,0.0f,-1.0f);
    	glVertex3f(0.0f,0.0f,0.0f);
    	glVertex3f(-1.0f,0.0f,0.0f);
    glEnd();
    //glutSwapBuffers();
}
void CubeRightDownF(){
	//glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
    glTranslatef(0.0f,0.0f,-5.0f);
    
    glRotatef(RotateBlock[4].x,1.0f,0.0f,0.0f);     //4
    glRotatef(RotateBlock[4].y,0.0f,1.0f,0.0f);
    glRotatef(RotateBlock[4].z,0.0f,0.0f,1.0f);
   

    glBegin(GL_QUADS);
    	glColor3f(1.0f,0.0f,0.0f);  //red
    	//正面
    	glVertex3f(0.0f,0.0f,1.0f);
    	glVertex3f(1.0f,0.0f,1.0f);
    	glVertex3f(1.0f,-1.0f,1.0f);
    	glVertex3f(0.0f,-1.0f,1.0f);

    	glColor3f(0.0f,0.0f,0.0f); //black
    	//顶面
    	glVertex3f(0.0f,0.0f,0.0f);
    	glVertex3f(1.0f,0.0f,0.0f);
    	glVertex3f(1.0f,0.0f,1.0f);
    	glVertex3f(0.0f,0.0f,1.0f);

    	glColor3f(0.0f,0.0f,1.0f); //blue
    	//右面
    	glVertex3f(1.0f,0.0f,0.0f);
    	glVertex3f(1.0f,-1.0f,0.0f);
    	glVertex3f(1.0f,-1.0f,1.0f);
    	glVertex3f(1.0f,0.0f,1.0f);

    	glColor3f(0.0f,0.0f,0.0f);  
    	//背面
    	glVertex3f(0.0f,0.0f,0.0f);
    	glVertex3f(1.0f,0.0f,0.0f);
    	glVertex3f(1.0f,-1.0f,0.0f);
    	glVertex3f(0.0f,-1.0f,0.0f);

    	glColor3f(0.0f,0.0f,0.0f);
    	//左面
    	glVertex3f(0.0f,0.0f,0.0f);
    	glVertex3f(0.0f,-1.0f,0.0f);
    	glVertex3f(0.0f,-1.0f,1.0f);
    	glVertex3f(0.0f,0.0f,1.0f);

    	glColor3f(0.0f,0.5f,0.0f);  //0.5g
    	//底面
    	glVertex3f(0.0f,-1.0f,0.0f);
    	glVertex3f(1.0f,-1.0f,0.0f);
    	glVertex3f(1.0f,-1.0f,1.0f);
    	glVertex3f(0.0f,-1.0f,1.0f);
    glEnd();
    //glutSwapBuffers();
}
void CubeRightDownB(){
	//glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
    glTranslatef(0.0f,0.0f,-5.0f);
    
    glRotatef(RotateBlock[5].x,1.0f,0.0f,0.0f);     //5
    glRotatef(RotateBlock[5].y,0.0f,1.0f,0.0f);
    glRotatef(RotateBlock[5].z,0.0f,0.0f,1.0f);
    

    glBegin(GL_QUADS);
    	glColor3f(0.0f,0.0f,0.0f);  //black
    	//正面
    	glVertex3f(0.0f,0.0f,0.0f);
    	glVertex3f(1.0f,0.0f,0.0f);
    	glVertex3f(1.0f,-1.0f,0.0f);
    	glVertex3f(0.0f,-1.0f,0.0f);

    	glColor3f(0.0f,0.0f,0.0f); //black
    	//顶面
    	glVertex3f(0.0f,0.0f,-1.0f);
    	glVertex3f(1.0f,0.0f,-1.0f);
    	glVertex3f(1.0f,0.0f,0.0f);
    	glVertex3f(0.0f,0.0f,0.0f);

    	glColor3f(0.0f,0.0f,1.0f); //blue
    	//右面
    	glVertex3f(1.0f,0.0f,0.0f);
    	glVertex3f(1.0f,0.0f,-1.0f);
    	glVertex3f(1.0f,-1.0f,-1.0f);
    	glVertex3f(1.0f,-1.0f,0.0f);

    	glColor3f(0.5f,0.0f,0.0f);  
    	//背面
    	glVertex3f(0.0f,0.0f,-1.0f);
    	glVertex3f(1.0f,0.0f,-1.0f);
    	glVertex3f(1.0f,-1.0f,-1.0f);
    	glVertex3f(0.0f,-1.0f,-1.0f);

    	glColor3f(0.0f,0.0f,0.0f);
    	//左面
    	glVertex3f(0.0f,0.0f,0.0f);
    	glVertex3f(0.0f,0.0f,-1.0f);
    	glVertex3f(0.0f,-1.0f,-1.0f);
    	glVertex3f(0.0f,-1.0f,0.0f);

    	glColor3f(0.0f,0.5f,0.0f);  //0.5g
    	//底面
    	glVertex3f(0.0f,-1.0f,-1.0f);
    	glVertex3f(1.0f,-1.0f,-1.0f);
    	glVertex3f(1.0f,-1.0f,0.0f);
    	glVertex3f(0.0f,-1.0f,0.0f);
    glEnd();
    //glutSwapBuffers();
}
void CubeLeftDownF(){
	//glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
    glTranslatef(0.0f,0.0f,-5.0f);

 
    glRotatef(RotateBlock[7].x,1.0f,0.0f,0.0f);     //7
    glRotatef(RotateBlock[7].y,0.0f,1.0f,0.0f);
    glRotatef(RotateBlock[7].z,0.0f,0.0f,1.0f);
   

    glBegin(GL_QUADS);
    	glColor3f(1.0f,0.0f,0.0f);  //red
    	//正面
    	glVertex3f(-1.0f,0.0f,1.0f);
    	glVertex3f(0.0f,0.0f,1.0f);
    	glVertex3f(0.0f,-1.0f,1.0f);
    	glVertex3f(-1.0f,-1.0f,1.0f);

    	glColor3f(0.0f,0.0f,0.0f); //black
    	//顶面
    	glVertex3f(0.0f,0.0f,0.0f);
    	glVertex3f(0.0f,0.0f,1.0f);
    	glVertex3f(-1.0f,0.0f,1.0f);
    	glVertex3f(-1.0f,0.0f,0.0f);

    	glColor3f(0.0f,0.0f,0.0f); //black
    	//右面
    	glVertex3f(0.0f,0.0f,0.0f);
    	glVertex3f(0.0f,-1.0f,0.0f);
    	glVertex3f(0.0f,-1.0f,1.0f);
    	glVertex3f(0.0f,0.0f,1.0f);

    	glColor3f(0.0f,0.0f,0.0f);  
    	//背面
    	glVertex3f(0.0f,0.0f,0.0f);
    	glVertex3f(0.0f,-1.0f,0.0f);
    	glVertex3f(-1.0f,-1.0f,0.0f);
    	glVertex3f(-1.0f,0.0f,0.0f);

    	glColor3f(0.0f,0.0f,0.5f);
    	//左面
    	glVertex3f(-1.0f,0.0f,0.0f);
    	glVertex3f(-1.0f,-1.0f,0.0f);
    	glVertex3f(-1.0f,-1.0f,1.0f);
    	glVertex3f(-1.0f,0.0f,1.0f);

    	glColor3f(0.0f,0.5f,0.0f);  //0.5g
    	//底面
    	glVertex3f(-1.0f,-1.0f,0.0f);
    	glVertex3f(0.0f,-1.0f,0.0f);
    	glVertex3f(0.0f,-1.0f,1.0f);
    	glVertex3f(-1.0f,-1.0f,1.0f);
    glEnd();
    //glutSwapBuffers();
}
void CubeLeftDownB(){
	//glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
    glTranslatef(0.0f,0.0f,-5.0f);
    
    glRotatef(RotateBlock[6].x,1.0f,0.0f,0.0f);     //8
    glRotatef(RotateBlock[6].y,0.0f,1.0f,0.0f);
    glRotatef(RotateBlock[6].z,0.0f,0.0f,1.0f);
   

    glBegin(GL_QUADS);
    	glColor3f(0.0f,0.0f,0.0f);  //black
    	//正面
    	glVertex3f(0.0f,0.0f,0.0f);
    	glVertex3f(-1.0f,0.0f,0.0f);
    	glVertex3f(-1.0f,-1.0f,0.0f);
    	glVertex3f(0.0f,-1.0f,0.0f);

    	glColor3f(0.0f,0.0f,0.0f); //black
    	//顶面
    	glVertex3f(0.0f,0.0f,0.0f);
    	glVertex3f(-1.0f,0.0f,0.0f);
    	glVertex3f(-1.0f,0.0f,-1.0f);
    	glVertex3f(0.0f,0.0f,-1.0f);

    	glColor3f(0.0f,0.0f,0.0f); //black
    	//右面
    	glVertex3f(0.0f,0.0f,0.0f);
    	glVertex3f(0.0f,-1.0f,0.0f);
    	glVertex3f(0.0f,-1.0f,-1.0f);
    	glVertex3f(0.0f,0.0f,-1.0f);

    	glColor3f(0.5f,0.0f,0.0f);  
    	//背面
    	glVertex3f(-1.0f,0.0f,-1.0f);
    	glVertex3f(0.0f,0.0f,-1.0f);
    	glVertex3f(0.0f,-1.0f,-1.0f);
    	glVertex3f(-1.0f,-1.0f,-1.0f);

    	glColor3f(0.0f,0.0f,0.5f);
    	//左面
    	glVertex3f(-1.0f,0.0f,-1.0f);
    	glVertex3f(-1.0f,-1.0f,-1.0f);
    	glVertex3f(-1.0f,-1.0f,0.0f);
    	glVertex3f(-1.0f,0.0f,0.0f);

    	glColor3f(0.0f,0.5f,0.0f);  //0.5g
    	//底面
    	glVertex3f(-1.0f,-1.0f,-1.0f);
    	glVertex3f(0.0f,-1.0f,-1.0f);
    	glVertex3f(0.0f,-1.0f,0.0f);
    	glVertex3f(-1.0f,-1.0f,0.0f);
    glEnd();
    //glutSwapBuffers();
}
void MagicCube(){
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

    CubeRightUpF();
    CubeRightUpB();
    CubeLeftUpF();
    CubeLeftUpB();

    CubeRightDownF();
    CubeRightDownB();
    CubeLeftDownF();
    CubeLeftDownB();

    glutSwapBuffers();
}
void KEY_CONTROL(unsigned char key,int x,int y){
	switch(key){
		case 100:RotateRoll-=3.0f;break;
		case 13:RotateYall-=3.0f;break;
		default:break;
	}
}
void KEY_CTRL(unsigned char key,int x,int y){
	int TEMP;
	switch(key){
		case RightF: 
			 RotateBlock[Area[0]].x-=3.0f;
			 RotateBlock[Area[1]].x-=3.0f;
			 RotateBlock[Area[4]].x-=3.0f;
			 RotateBlock[Area[5]].x-=3.0f;
             if(abs(RotateBlock[Area[0]].x) % 90==0){
             	TEMP = Area[5];
             	Area[5] = Area[1];
             	Area[1] = Area[0];
             	Area[0] = Area[4];
             	Area[4] = TEMP;
             }
		     break;
		case LeftF:
		     RotateBlock[Area[3]].x-=3.0f;
		     RotateBlock[Area[2]].x-=3.0f;
		     RotateBlock[Area[6]].x-=3.0f;
		     RotateBlock[Area[7]].x-=3.0f;
		     if(abs(RotateBlock[Area[3]].x) % 90==0){
             	TEMP =  Area[7];
             	Area[7] = Area[6];
             	Area[6] = Area[2];
             	Area[2] = Area[3];
             	Area[3] = TEMP;
         	}
			 break;
		case UpL:

			 RotateBlock[Area[0]].y-=3.0f;
			 RotateBlock[Area[3]].y-=3.0f;
			 RotateBlock[Area[2]].y-=3.0f;
			 RotateBlock[Area[1]].y-=3.0f;
 			 if(abs(RotateBlock[Area[0]].y) % 90==0){
				 TEMP = Area[1];
				 Area[1] = Area[2];
				 Area[2] = Area[3];
			 	 Area[3] = Area[0];
			 	 Area[0] = TEMP;
			 }
			break;
		case DownL:
		     RotateBlock[Area[4]].y-=3.0f;
		     RotateBlock[Area[7]].y-=3.0f;
		     RotateBlock[Area[6]].y-=3.0f;
		     RotateBlock[Area[5]].y-=3.0f;
 			 if(abs(RotateBlock[Area[4]].y) % 90==0){
		     	TEMP = Area[5];
		     	Area[5] = Area[6];
		     	Area[6] = Area[7];
		     	Area[7] = Area[4];
		     	Area[4] =TEMP;
		     }
			break;
		case FRight:
		     RotateBlock[Area[0]].z-=3.0f;
		     RotateBlock[Area[4]].z-=3.0f;
		     RotateBlock[Area[7]].z-=3.0f;
		     RotateBlock[Area[3]].z-=3.0f;
		     if(abs(RotateBlock[Area[0]].z) % 90==0){
		    	 TEMP = Area[3];
		     	 Area[3] = Area[7];
		    	 Area[7] = Area[4];
		     	 Area[4] = Area[0];
		     	 Area[0] =TEMP;
			}
			break;
		case BRight:
		     RotateBlock[Area[2]].z-=3.0f;
		     RotateBlock[Area[1]].z-=3.0f;
		     RotateBlock[Area[5]].z-=3.0f;
		     RotateBlock[Area[6]].z-=3.0f;
		    if(abs(RotateBlock[Area[2]].z) % 90==0){
		      	 TEMP = Area[6];
		     	 Area[6] = Area[5];
		         Area[5] = Area[1];
		         Area[1] = Area[2];
		         Area[2] =TEMP;
		    }
			break;
		default:break;
	}
}
void ReShape(GLsizei width,GLsizei height){
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(135.0f,(GLdouble)width/(GLdouble)height,0.5f,100.0f);
	glMatrixMode(GL_MODELVIEW);
}
int main(int argc,char* argv[]){
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowPosition(1920/2,1080/2);
	glutInitWindowSize(600,600);
	glutInit(&argc,argv);
	glutCreateWindow("MyCube");
    Init();
    glutDisplayFunc(display);
    glutReshapeFunc(ReShape);
 #ifdef HandControl
    glutKeyboardFunc(KEY_CONTROL);
 #endif
    glutIdleFunc(display);
	glutMainLoop();
	return 0;
}