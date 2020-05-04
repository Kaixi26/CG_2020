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
#include "point.h"
#include "controlpoints.h"
#include "benzierpatch.h"
#include <array>
#include <vector>

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

void drawPoint(Point p){
	glVertex3f(p.x, p.y, p.z);
}

void drawCtrlPoints(ControlPoints cp){
	glBegin(GL_LINE_STRIP);
	int max = 10;
	for(int i=0; i<=max; i++){
		drawPoint(cp.evalBenzier(i/(float)max));
	}
	glEnd();

}

std::vector<BenzierPatch>  readFile(){
	FILE* f = fopen("test", "r");
	int n_patches;
	int n_points;
	char tmp[1024];
	fgets(tmp, 1024, f);
	sscanf(tmp , "%d\n", &n_patches);
	std::vector<std::array<float, 16>> ind_patches;
	ind_patches.reserve(n_patches);
	for(int i=0; i<n_patches; i++){
		std::array<float, 16> curr;
		fgets(tmp, 1024, f);
		sscanf(tmp , "%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f", &curr[0] , &curr[1] , &curr[2] , &curr[3] , &curr[4] , &curr[5] , &curr[6] , &curr[7] , &curr[8] , &curr[9] , &curr[10] , &curr[11] , &curr[12] , &curr[13] , &curr[14] , &curr[15] , &curr[15]);
		ind_patches[i] = curr;
	}
	fgets(tmp, 1024, f);
	sscanf(tmp , "%d", &n_points);
	std::vector<Point> points;
	points.reserve(n_points);
	for(int i=0; i<n_points; i++){
		std::array<float, 3> curr;
		fgets(tmp, 1024, f);
		sscanf(tmp , "%f, %f, %f", &curr[0] , &curr[1] , &curr[2]);
		points[i] = Point(curr[0], curr[1], curr[2]);
	}
	std::vector<BenzierPatch> bpatches;
	bpatches.reserve(n_patches);
	for(int i=0; i<n_patches; i++){
		bpatches[i] = BenzierPatch({
			  std::array<Point, 4>{ points[ind_patches[i][12]], points[ind_patches[i][13]], points[ind_patches[i][14]], points[ind_patches[i][15]]}
			, std::array<Point, 4>{ points[ind_patches[i][8]], points[ind_patches[i][9]], points[ind_patches[i][10]], points[ind_patches[i][11]]}
			, std::array<Point, 4>{ points[ind_patches[i][4]], points[ind_patches[i][5]], points[ind_patches[i][6]], points[ind_patches[i][7]]}
			, std::array<Point, 4>{ points[ind_patches[i][0]], points[ind_patches[i][1]], points[ind_patches[i][2]], points[ind_patches[i][3]]}
		});
	}
	fclose(f);
	return bpatches;
}

void drawBenz(BenzierPatch bpatch , int divu, int divv){
		glColor3f(
			  random()/(float)INT_MAX
			, random()/(float)INT_MAX
			, random()/(float)INT_MAX
		);
	for(int i=0; i<divu; i++){
		glBegin(GL_TRIANGLES);
		for(int j=0; j<divv; j++){
			drawPoint(bpatch.evalBenzier(i/(float)divu, j/(float)divv));
			drawPoint(bpatch.evalBenzier((i+1)/(float)divu, j/(float)divv));
			drawPoint(bpatch.evalBenzier((i+1)/(float)divu, (j+1)/(float)divv));

			drawPoint(bpatch.evalBenzier((i+1)/(float)divu, (j+1)/(float)divv));
			drawPoint(bpatch.evalBenzier(i/(float)divu, (j+1)/(float)divv));
			drawPoint(bpatch.evalBenzier(i/(float)divu, j/(float)divv));
		}
		glEnd();
	}
}


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

	drawAxis();

	BenzierPatch bpatch = BenzierPatch({
		std::array<Point,4> { Point(0, 0, 0)
		, Point(1, 2, 0)
		, Point(2, 2, 0)
		, Point(3, 0, 0)
		},
		std::array<Point,4> { Point(0, 0.5, 1)
		, Point(1, 1.5, 1)
		, Point(2, 1.5, 1)
		, Point(3, 0.5, 1)
		},
	 	std::array<Point,4>	{ Point(0, 0.5, 2)
		, Point(1, 1.5, 2)
		, Point(2, 1.5, 2)
		, Point(3, 0.5, 2)
		},
		std::array<Point,4> { Point(0, 0, 3)
		, Point(1, 3, 3)
		, Point(2, 3, 3)
		, Point(3, 0, 3)
		}
	});

	glColor3f(1,1,1);
	//drawBenz(bpatch, 5, 5);
	auto bpatches = readFile();
	for(int i=0; i<bpatches.capacity(); i++){
		drawBenz(bpatches[i], 10, 10);
	}

	//drawBenz(ctrl, 5);
	//drawPatch(bpatch, 2, 5);

	//glColor3f(1,1,1);
	//glBegin(GL_LINE_STRIP);
	//drawPoint(p[0]);
	//drawPoint(p[1]);
	//drawPoint(p[2]);
	//drawPoint(p[3]);
	//glEnd();

	//glBegin(GL_LINE_STRIP);
	//drawPoint(points[0]);
	//drawPoint(points[1]);
	//drawPoint(points[2]);
	//glEnd();

	//glBegin(GL_LINE_STRIP);
	//glColor3f(1,0,1);
	//drawPoint(points[3]);
	//drawPoint(points[4]);
	//glEnd();


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
	//glutIdleFunc(renderScene);
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
