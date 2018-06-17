#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
GLint x_0 = 0;
GLint y_0 = 0;
GLint x_1 = 200;
GLint y_1 = 200;
GLint R = 100;
void init(){
	glClearColor(1.0f,1.0f,1.0f,1.0f);
	glClearDepth(1.0f);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
}
void test_display(){
	glVertex2i(100,100);
	glVertex2i(-100,100);
	glVertex2i(100,-100);
	glVertex2i(-100,-100);
}
void CirclePoints(int x,int y){
	//第一象限
	glVertex2i(x,y);
	glVertex2i(y,x);
	//第二象限
	glVertex2i(-x,y);
	glVertex2i(-y,x);
	//第三象限
	glVertex2i(-x,-y);
	glVertex2i(-y,-x);
	//第四象限
	glVertex2i(x,-y);
	glVertex2i(y,-x);
}
void EllipsePoints(int x,int y){
	//第一象限
	glVertex2i(x,y);
	//第二象限
	glVertex2i(-x,y);
	//第三象限
	glVertex2i(-x,-y);
	//第四象限
	glVertex2i(x,-y);
}
void MyEllipse(int a,int b){
	//椭圆方程
	//x^2/a^2+y^2/b^2=1 (a>b>0)  F(x,y) = x^2/a^2+y^2/b^2-1
	// k<=-1 是 中点判别式 d=F(x+1,y-0.5)   d0=F(x0,y0)+b^2(2x+1)+a^2(1/4-y)
	// d<0 中点在椭圆内 下一个坐标(x+1,y)
	// d>=0 中点在椭圆外 下一个坐标(x+1,y-1) 
	//判别式增量为
	//d<0 F(x+1+1,y-0.5)=d+b^2(2x+3);
	//d>=0 F(x+1+1,y-1-0.5)=d+b^2(2x+3)+a^2(2-2y);
    // k=-1 切线的计算
    // F(x,y)/∂x+F(x,y)/∂x = 0   
    // => 2*b^2x+2*a^2y*y' = 0
    // => y' = -2*(b^2)*x/2*(a^2)*y   -1 = dy/dx =  -2*(b^2)x/2*(a^2)y
    //  k<=-1 在上半部分   则其中点判别式满足  (b^2)*(x+1)<(a^2)*(y-0.5)
    //  k>-1  在下半部分   则其中点判别式妈祖  (b^2)*(x+1)>=(a^2)*(y-0.5)
    //  下半部分 中点判别式
    //  d=F(x+0.5,y-1)=F(x,y)+b^2(x+1/4)+a^2(1-2y)  //初始化值为上部分最后一个点
    //  d<0   F(x+0.5+1,y-1-1) =d+b^2(2x+2)+a^2(3-2y)
    //  d>=0 F(x+0.5,y-1-1) =d+a^2(3-2y)
    int x,y,B2,A2;
    float d0_up,d0_down;
    x = 0;
    y = b;
    B2 = pow(b,2);
    A2 = pow(a,2);
    d0_up = B2*(2*x+1)+(float)(A2*(0.25-b));
    EllipsePoints(x,y);
    while((float)(B2*(x+1))<(float)(A2*(y-0.5))){

    	if(d0_up<0){
    		d0_up+=B2*(2*x+3);
    	}else{
    		d0_up+=B2*(2*x+3)+A2*(2-2*y);
    		y--;
    	}
    	x++;
    	EllipsePoints(x,y);
    }
    d0_down = B2*(x+1/4)+A2*(1-2*y);
    while(y>=0){
    	if(d0_down<0){
    		d0_down+=B2*(2*x+2)+A2*(3-2*y);
    		x++;
    	}else{
    		d0_down+=A2*(3-2*y);
    	}
    	y--;
    	EllipsePoints(x,y);
    }
}
void BresenhamCirclePoints(int x,int y){
	//第一象限
    glVertex2i(x,y);
	//第二象限
    glVertex2i(-x,y);
	//第三象限
    glVertex2i(-x,-y);
	//第四象限
	glVertex2i(x,-y);
}
void BresenhamCircle(int r){
	//下一个参考点 可能取值 dH (x+1,y) dD(x+1,y-1) dV(x,y-1)
	// dH = F(x+1,y) = d+2x+1;
	// dD = F(x+1,y-1) = d+2x-2y+2;
	// dv = F(x,y-1) = d-2y+1;
	// deltaHD = 2(dD+y)-1;  // |H|-|D|
	// deltaDV = 2(dD-x)-1;  // |D|-|V|
	// dD<0 在圆内  deltaHD <0  离H近````																																																																																																																																																																																																																																																																																																																																									
	// dD>0 在圆外  deltaDV <0  离D近
	// dD=0 在圆上  
    int x,y,dD,deltaHD,deltaDV,Sel;
    x = 0;
    y = r;
    dD = 2-2*r;
    while(y>=0){
    	BresenhamCirclePoints(x,y);
    	if(dD>0){
    		deltaDV = 2*(dD-x)-1;
    		Sel = (deltaDV<0)?2:3;
    	}else if(dD<0){
    		deltaHD = 2*(dD+y)-1;
    		Sel = (deltaHD<0)?1:2;
    	}else{
    		Sel = 2;
    	}
    	switch(Sel){
    		case 1:x++;dD+=2*x+1;break; //下一个H的增量
    		case 2:x++;y--;dD+=2*x-2*y+2; break; //下一个D的增量
    		case 3:y--;dD+=-2*y+1; break;//下一个V的增量
    	}
    }
}
void CirCle(int r){
	//判别式
	//F(x,y)=x^2+y^2-R^2  
	//d=F(x+1,y-0.5);  d为中点判别式
	//d0=F(x0,y0)+2*x0-y0+1.25;  
	//d<0 中点在圆内   下一个坐标为 (x+1,y);
	//d>=0 中点在圆外  下一个坐标为 (x+1,y-1);
	//判别式增量计算
	//d<0 F(x+1+1,y-0.5)=d+2*x; 
	//d>= F(x+1+1,y-1-0.5)=d+2*x-2*y+5
    int x,y;
    float d;
    x = 0;
    y = r;
    d = (float)(2*x-y)+1.25f;   //d0
    CirclePoints(x,y);
    while(x<=y){
    	y = (d<0.0f)?y:(y-1);
    	d += (d<0.0f)?(float)(2*x):(float)(2*(x-y))+5.0f;
    	x++;
    	CirclePoints(x,y);
    }
}
void diplay(){
	glPointSize(1.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0f,0.0f,0.0f);
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,0.0f);
	glBegin(GL_POINTS);
	//test_display();
	//CirCle(200);
	//BresenhamCircle(200);
	MyEllipse(200,100);
	glEnd();
	glutSwapBuffers();
}
void reshape(GLsizei w,GLsizei h){
	GLfloat coordinate = 1920/2;
	GLfloat ratio =(GLfloat)w/(GLfloat)h;
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(w<h){
		glOrtho(-coordinate,coordinate,-coordinate/ratio,coordinate/ratio,-coordinate,coordinate);
	}else{
		glOrtho(-coordinate*ratio,coordinate*ratio,-coordinate,coordinate,-coordinate,coordinate);
	}
	glMatrixMode(GL_MODELVIEW);
}
void key_control(int key,int x,int y){

}
int main(int argc, char *argv[])
{
	//窗体的构建
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowPosition(1920/2,1080/2);
	glutInitWindowSize(600,600);
	glutInit(&argc,argv);
	glutCreateWindow("CircleAlgorithm");
	
	//初始化
	init();
	//
	glutDisplayFunc(diplay);
	glutReshapeFunc(reshape);
	//glutSpecialFunc(key_control);
	glutIdleFunc(diplay);

	glutMainLoop();
	return 0;
}