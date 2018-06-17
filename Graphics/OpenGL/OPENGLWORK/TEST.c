#define  GLUT_DISABLE_ATEXIT_HACK
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <GL/glut.h>

GLfloat rquad;      //立方体旋转角度
void init(void)
{
 glClearColor(0.0f,0.0f,0.0f,0.0f);
 glShadeModel(GL_SMOOTH);
 glEnable(GL_DEPTH_TEST);   //激活深度测试，以隐藏被遮挡面
}

void display(void)
{
 //清除颜色缓存和深度缓存
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 glLoadIdentity();
 glTranslatef(0.0f,0.0f,-7.0f);
 glRotatef(rquad,1.0f,0.0f,0.0f);
 glBegin(GL_QUADS);
 glColor3f(0.0f,1.0f,0.0f);
 glVertex3f(1.0f,1.0f,-1.0f);
 glVertex3f(-1.0f,1.0f,-1.0f);
 glVertex3f(-1.0f,1.0f,1.0f);
 glVertex3f(1.0f,1.0f,-1.0f);

 glColor3f(1.0f,0.5f,0.0f);
 glVertex3f(1.0f,-1.0f,1.0f);
 glVertex3f(-1.0f,-1.0f,1.0f);
 glVertex3f(-1.0f,-1.0f,-1.0f);
 glVertex3f(1.0f,-1.0f,-1.0f);

 glColor3f(1.0f,0.0f,0.0f);
 glVertex3f(1.0f,1.0f,1.0f);
 glVertex3f(-1.0f,1.0f,1.0f);
 glVertex3f(-1.0f,-1.0f,1.0f);
 glVertex3f(1.0f,-1.0f,1.0f);

 glColor3f(1.0f,1.0f,0.0f);
 glVertex3f(1.0f,-1.0f,-1.0f);
 glVertex3f(-1.0f,-1.0f,-1.0f);
 glVertex3f(-1.0f,1.0f,-1.0f);
 glVertex3f(1.0f,1.0f,-1.0f);

 glColor3f(0.0f,0.0f,1.0f);
 glVertex3f(-1.0f,1.0f,1.0f);
 glVertex3f(-1.0f,1.0f,-1.0f);
 glVertex3f(-1.0f,-1.0f,-1.0f);
 glVertex3f(-1.0f,-1.0f,1.0f);

 glColor3f(1.0f,0.0f,1.0f);
 glVertex3f(1.0f,1.0f,-1.0f);
 glVertex3f(1.0f,1.0f,1.0f);
 glVertex3f(1.0f,-1.0f,1.0f);
 glVertex3f(-1.0f,-1.0f,-1.0f);

  glEnd();
  //rquad-=0.2f;            //加一个角度
  glutSwapBuffers();      //交换双缓存
}

void reshape(int width,int height)
{
 glViewport(0,0,width,height);
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
}

void keyboard(unsigned char key,int x,int y)
{
 switch (key)
 {
   case 13:
     rquad-=1.0f;
   break;
      default:
    break;
 }
}

int main(int argc,char** argv)
{
 glutInit(&argc,argv);
 //使用双缓存模式和深度缓存
 glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
 glutInitWindowSize(600,350);
 glutInitWindowPosition(200,200);
 glutCreateWindow("旋转动画");
 init();
 glutDisplayFunc(display);
 glutReshapeFunc(reshape);
 glutKeyboardFunc(keyboard);
 glutIdleFunc(display);   //设置空闲时用的函数
 glutMainLoop();
 return 0;
}