#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <array>
#include <vector>
#include "vertex.h"
#include "catmull.h"

float cam_alpha = M_PI_4;
float cam_beta = M_PI_4;
float cam_rad = 12;
float cam_alpha_diff = 0.1;
float cam_beta_diff = 0.1;
float radius = 0.5;
float height = 1;
int slices = 10;
int stacks = 10;

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


void drawAxis(){
	glBegin(GL_LINES);
// X axis in red
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f( 100.0f, 0.0f, 0.0f);
// Y Axis in Green
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 100.0f, 0.0f);
// Z Axis in Blue
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);
	glEnd();
}

float t = 0;
void renderScene(void) {
	srand(0);

	// clear buffers
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// set the camera
	glLoadIdentity();
	gluLookAt(cam_rad*cos(cam_beta)*sin(cam_alpha),
			  cam_rad*sin(cam_beta),
			  cam_rad*cos(cam_beta)*cos(cam_alpha),
			  0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);
	glColor3f(1,1,1);

	drawAxis();

	Catmull catmull;
	catmull.insert(Vertex(0,-1,0));
	catmull.insert(Vertex(0,1,1));
	catmull.insert(Vertex(1,1,0));
	catmull.insert(Vertex(1,-1,1));
	catmull.insert(Vertex(2,-1,0));
	catmull.insert(Vertex(3,0,1));
	catmull.insert(Vertex(0,-1,0));
	t += 0.02;
	Vertex tmp = catmull.getAt(t);
	glTranslatef(tmp.x, tmp.y, tmp.z);
	glutSolidTeapot(1);


	//    End of frame
	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {
	switch(c){
		case 'h':
			cam_alpha -= cam_alpha_diff;
			break;
		case 'l':
			cam_alpha += cam_alpha_diff;
			break;
		case 'j':
			cam_beta -= cam_beta_diff;
			break;
		case 'k':
			cam_beta += cam_beta_diff;
			break;
		case 'H':
			slices--;
			break;
		case 'L':
			slices++;
			break;
		case 'P':
			stacks++;
			break;
		case 'U':
			stacks--;
			break;
		case 'J':
			height -= 0.2;
			break;
		case 'K':
			height += 0.2;
			break;
		case '+':
			cam_rad -= 0.5;
			break;
		case '-':
			cam_rad += 0.5;
			break;
		case 'r':
			cam_alpha = 0;
			cam_beta  = 0;
			break;

	}
	cam_alpha = fmod(cam_alpha, 2*M_PI);
	cam_beta = fmod(cam_beta + M_PI_2, M_PI) - M_PI_2;
	cam_beta = fmod(cam_beta - M_PI_2, M_PI) + M_PI_2;
	glutPostRedisplay();

}


void processSpecialKeys(int key, int xx, int yy) {
	switch(key){
		case GLUT_KEY_UP:
			cam_beta += cam_beta_diff;
			break;
	}
	glutPostRedisplay();

}


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM");

// Required callback registry 
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
