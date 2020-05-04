#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>
#include <climits>

float alfa = 0.0f, beta = 0.5f, radius = 100.0f;
float camX, camY, camZ;
float teapot_angle = 0;
int seed = (int)time(NULL);

void spherical2Cartesian() {

	camX = radius * cos(beta) * sin(alfa);
	camY = radius * sin(beta);
	camZ = radius * cos(beta) * cos(alfa);
}


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

void draw_axis(){
    glBegin(GL_LINES);
// X axis in red
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f( 0.0f, 0.0f, 0.0f);
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

void draw_plane(){
	glColor3f(0.2f, 0.8f, 0.2f);
	glBegin(GL_TRIANGLES);
		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);

		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);
		glVertex3f(100.0f, 0, 100.0f);
	glEnd();
}

// Draws a tree with base in (0,0,0) and upward direction in the Y axis
void draw_tree(){
	glPushMatrix();
	glRotatef(90, -1, 0, 0);

	glColor3f(1, 0.5, 0);
	glutSolidCone(2, 15, 10, 10);

	glColor3f(0, 1, 0);
	glTranslatef(0, 0, 5);
	glutSolidCone(5, 15, 10, 10);
	glPopMatrix();
}

void draw_trees(int N){
	int drawn = 0;
	float sup_lim = 100;
	float inf_lim = -100;
	float sqr_max_radius = 50*50;
	while(drawn < N){
		float x = ((float)rand() / (float)INT_MAX) * (sup_lim + abs(inf_lim)) + inf_lim;
		float z = ((float)rand() / (float)INT_MAX) * (sup_lim + abs(inf_lim)) + inf_lim;
		if(x*x + z*z < sqr_max_radius) continue;
		glPushMatrix();
		glTranslatef(x, 0, z);
		draw_tree();
		glPopMatrix();
		drawn++;
	}
}

// Draws a teapot with base in aproximatly (0,0,0) and upward direction in the Y axis pointing to the X axis
void draw_teapot(float size){
	glPushMatrix();
	glTranslatef(0, size/1.25, 0);
	glutSolidTeapot(size);
	glPopMatrix();
}

void draw_inner_teapots(int N, float radius, float size, float angle){
	glPushMatrix();
	glRotatef(teapot_angle, 0, -1, 0);
	glColor3f(0, 0, 1);
	float d_angle = 360.0/(float)N;
	for(int i=0; i<N; i++){
		glPushMatrix();
		glRotatef(i*d_angle, 0, 1, 0);
		glTranslatef(radius, 0, 0);
		draw_teapot(size);
		glPopMatrix();
	}
	glPopMatrix();
}

void draw_outer_teapots(int N, float radius, float size, float angle){
	glPushMatrix();
	glRotatef(teapot_angle, 0, 1, 0);
	glColor3f(1, 0, 0);
	float d_angle = 360.0/(float)N;
	for(int i=0; i<N; i++){
		glPushMatrix();
		glRotatef(i*d_angle, 0, 1, 0);
		glTranslatef(radius, 0, 0);
		glRotatef(90, 0, 1, 0);
		draw_teapot(size);
		glPopMatrix();
	}
	glPopMatrix();
}


void renderScene(void) {
	srand(seed);	

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);
	//draw_axis();
	draw_plane();
	draw_trees(100);
	draw_inner_teapots(10, 20, 2, teapot_angle);
	draw_outer_teapots(10, 40, 2, teapot_angle);
	glColor3f(1.0, 170.0/255.0, 170.0/255.0);
	glutSolidTorus(2, 5, 10, 10);

	// End of frame
	glutSwapBuffers();
	teapot_angle += 0.1;
	teapot_angle = fmod(teapot_angle, 360.0);
}


void processKeys(unsigned char c, int xx, int yy) {

// put code to process regular keys in here

}


void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {

	case GLUT_KEY_RIGHT:
		alfa -= 0.1; break;

	case GLUT_KEY_LEFT:
		alfa += 0.1; break;

	case GLUT_KEY_UP:
		beta += 0.1f;
		if (beta > 1.5f)
			beta = 1.5f;
		break;

	case GLUT_KEY_DOWN:
		beta -= 0.1f;
		if (beta < -1.5f)
			beta = -1.5f;
		break;

	case GLUT_KEY_PAGE_DOWN: radius -= 5.0f;
		if (radius < 1.0f)
			radius = 1.0f;
		break;

	case GLUT_KEY_PAGE_UP: radius += 5.0f; break;
	}
	spherical2Cartesian();
	glutPostRedisplay();

}


void printInfo() {

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));

	printf("\nUse Arrows to move the camera up/down and left/right\n");
	printf("Home and End control the distance from the camera to the origin");
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
	//glEnable(GL_CULL_FACE);

	spherical2Cartesian();

	printInfo();

// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
