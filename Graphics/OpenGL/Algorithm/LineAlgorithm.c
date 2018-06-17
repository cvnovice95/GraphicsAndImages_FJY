#define GLUT_DISABLE_ATEXIT_HACK
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <GL/glut.h>
#define ImproveBresenHam
void Init(){
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClearDepth(1.0f);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
}
int SWIN;
int MWIN;
float PointSize=1.0f;
float ViewWidth=1920.0f;
int sel = 0;
GLint x_0=0;
GLint y_0=0;
GLint x_1=5000;
GLint y_1=3000;
void BresenHamLine(int x0,int y0,int x1,int y1){
	int x,y,dx,dy;
	float k,e;
	dx = x1-x0;
	dy = y1-y0;
	k =(float)dy/dx;
	e = -0.5;
	x = x0;
	y = y0;
	int i=0;
	int j=0;
	if(fabs(k)<=1.0f){
    	for(;i<=dx;i++){
    		glVertex2i(x,y);
    			x++;
    			e=e+k;
    		if(e>=0.0f){
    			y++;
    			e=e-1;
    		}
   	 }
	}else if(fabs(k)>1.0f){
		for(;j<=dy;j++){
    		glVertex2i(x,y);
    			y++;
    			e=e+(1/k);
    		if(e>=0.0f){
    			x++;
    			e=e-1;
    		}
		}
	}

}

void ImproveBresenHamLine(int x0,int y0,int x1,int y1){
	    //k<1
		//e=-0.5   -> e`=-dx
	    //k=dy/dx    
	    //e=e+k   -> e=e+dy/dx  ->2*e*dx = 2*e*dx+2dy e`=2*e*dx  e`=e`+2dy
	    //e=e-1   -> e`=e`-2dx
        //k>1
	    //e=e+1/k ->e=e+dx/dy  ->2*e*dy = 2*e*dy+2dx  e`=e`+2dx
	    //e=e-1   ->e`=e`-2dy
	    //e=-0.5  ->e`=-dy
	    int x,y,dx,dy,e;
	    float k;
	    dx = x1-x0;
	    dy = y1-y0;
	    k =(float) dy/dx;
	    e = (fabs(k)<=1.0f)?-dx:-dy;
        x = x0;
        y = y0;
        int i=0;
        int j=0;
        if(fabs(k)<=1.0f){
        	for(;i<=dx;i++){
        		glVertex2i(x,y);
        			x++;
        			e=e+2*dy;
        		if(e>=0){
        			y++;
        			e=e-2*dx;
        		}
        	}
        }
        else if(fabs(k)>1.0f){
        
        	for(;j<=dy;j++){
        		glVertex2i(x,y);
        			y++;
        			e=e+2*dx;
        		if(e>=0){
        			x++;
        			e=e-2*dy;
        		}
        	}
        }
}
void MidPointLine(int x0,int y0,int x1,int y1){
	//|k|<=1
	//判别式 d=F(x+1,y+0.5)  d>=0(中点在直线上方) pDown(x+1,y) F(x+1+1,y+0.5) d<0 pUP(x+1,y+1) F(x+1+1,y+1+0.5)
	//增量 DDown = d+a  DUP= d+a+b  d0 = F(x0,y0)+a+0.5b 
	int a,b,d0,dUp,dDown,x,y;
	float K;
	K=(float)(y1-y0)/(float)(x1-x0);
	if(fabs(K)<=1.0f){
		a = y0-y1;
		b = x1-x0;
	}else if(fabs(K)>1.0f){
		a = x0-x1;
    	b = y1-y0;
	}
	d0 = 2*(a+0.5*b);
	dUp = 2*(a+b);
	dDown = 2*a;
	x=x0;
	y=y0;
	glVertex2i(x,y);
	//|k|<=1
    if(fabs(K)<=1.0f){
		while(x<=x1){
			if(d0<0){
				x++;
				y++;
				d0+=dUp;
			}
			else{
				x++;
				d0+=dDown;
			}
			glVertex2i(x,y);
		}
	}
	else if(fabs(K)>1.0f){ //|k|>1
		while(y<=y1){
			if(d0<=0){
				x++;
				y++;
				d0+=dUp;
			}
			else{
				y++;
				d0+=dDown;
			}
			glVertex2i(x,y);
		}
	}
}
void DDA(int x0,int y0,int x1,int y1){
	int x;
	float y,k,dx,dy,x_N;
	dx = x1-x0;
	dy = y1-y0;
	k = dy/dx;
	y = y0;
	x_N = x0;

	if(fabs(k)>1.0){
		for(;y<=y1;y++){
			glVertex2i(floor(x_N+0.5),y);
			x_N=x_N+1/k;
		}
	}
	else if(fabs(k)<=1.0){
		for(x=x0;x<=x1;x++){
			glVertex2i(x,floor(y+0.5));
			y=y+k;
		}
	}
}

void display(){
	glPointSize(PointSize);   //1---1920f   5--60f
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0f,0.0f,0.0f); //red
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,0.0f);
	glBegin(GL_POINTS);
    switch(sel){
    	case 0:DDA(x_0-200,y_0,x_1-200,y_1);break;
    	case 1:BresenHamLine(x_0-400,y_0,x_1-400,y_1);break;
    	case 2:ImproveBresenHamLine(x_0,y_0,x_1,y_1);break;
    	case 3:MidPointLine(x_0+100,y_0,x_1+100,y_1);break;
    	default:break;
    }
    //DDA(x_0-200,y_0,x_1-200,y_1);
    //BresenHamLine(x_0-400,y_0,x_1-400,y_1);
    //ImproveBresenHamLine(x_0,y_0,x_1,y_1);
    //MidPointLine(x_0+100,y_0,x_1+100,y_1);
    //DDA(x_0,y_0,5000,5000);
    //MidPointLine(x_0,y_0,5000,5000);
    glEnd();
    
    glutSwapBuffers();      //交换双缓存
    
}
void reshape(GLsizei w,GLsizei h){
	//设置坐标系为x(-100.0f,100.0f)、y(-100.0f,100.0f)、z(-100.0f,100.0f)  
    //GLfloat coordinatesize=100.0f;  
    GLfloat coordinatesize=ViewWidth;  //坐标系大小
	GLfloat ratio=(GLfloat)w/(GLfloat)h;
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	    //gluPerspective(135.0f,(GLdouble)width/(GLdouble)height,0.1f,100.0f);//立体投影
		//正交投影  
    if(w<h)  // left ,right bottom top near far
        glOrtho(-coordinatesize,coordinatesize,-coordinatesize/ratio,coordinatesize/ratio,-coordinatesize,coordinatesize);  
    else  
        glOrtho(-coordinatesize*ratio,coordinatesize*ratio,-coordinatesize,coordinatesize,-coordinatesize,coordinatesize); 
	glMatrixMode(GL_MODELVIEW);
}
void KEY_CONTROL(int key,int x,int y){
	switch(key){
		case GLUT_KEY_LEFT:sel=0;break;
		case GLUT_KEY_RIGHT:sel=1;break;
		case GLUT_KEY_UP:sel=2;break;
		case GLUT_KEY_DOWN:sel=3;break;
		case GLUT_KEY_PAGE_UP:
		       PointSize=5.0f;
		       ViewWidth=60.0f;
		       break;
		case GLUT_KEY_PAGE_DOWN:
		 	   PointSize=1.0f;
		 	   ViewWidth=1920.0f;
		 	   break;
		default:break;
	}
}

int main(int argc,char* argv[]){
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowPosition(1920/2,1080/2);
	glutInitWindowSize(600,600);
	glutInit(&argc,argv);
	MWIN=glutCreateWindow("LineAlgorithm");
	Init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(KEY_CONTROL);
    glutIdleFunc(display);

	glutMainLoop();


	return 0;
}
