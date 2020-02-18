#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
#include <stdio.h>

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


void draw_pyramid(){
	glBegin(GL_TRIANGLES);
	glRotatef(90, 1 , 1, 1);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	glColor3f(0, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, -1.0f);
	glVertex3f(0.0f, 1.0f, 0.0f);

	glEnd();
}

void draw_axis(){
	glBegin(GL_LINES);
// X axis in red
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-100.0f, 0.0f, 0.0f);
	glVertex3f( 100.0f, 0.0f, 0.0f);
// Y Axis in Green
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -100.0f, 0.0f);
	glVertex3f(0.0f, 100.0f, 0.0f);
// Z Axis in Blue
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -100.0f);
	glVertex3f(0.0f, 0.0f, 100.0f);
	glEnd();
}

float angle = 0;
float angle_inc = 16;
float pyramid_x = 0;
float pyramid_y = 0;
float pyramid_z = 0;
float pyramid_scl = 1;
float axis_inc = 0.1;
float vel = 0;
float acc = -0.01;

float calc_y(){
	vel+=acc;
	vel*=0.99;
	pyramid_y+=vel;
	if(pyramid_y <= 0) pyramid_y = 0;
	return pyramid_y;
}

void keyboard_handler(unsigned char key, int x, int y){
	switch(key){
		case 'd':
			angle += angle_inc;
			break;
		case 's':
			angle -= angle_inc;
			break;
		case 'h':
			pyramid_x -= axis_inc;
			break;
		case 'l':
			pyramid_x += axis_inc;
			break;
		case 'j':
			pyramid_z -= axis_inc;
			break;
		case 'k':
			pyramid_z += axis_inc;
			break;
		case 'a':
			pyramid_scl -= axis_inc;
			break;
		case 'f':
			pyramid_scl += axis_inc;
			break;
		case 'b':
			vel = 0.25;
			break;
	}
	angle = fmod(angle, 360);
	printf("%f\n", angle);
	glutPostRedisplay();
}

void renderScene(void) {

	// clear buffers
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(5.0,5.0,5.0, 
			  0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);


// put the geometric transformations here


// put drawing instructions here
	draw_axis();
	glTranslatef(pyramid_x, calc_y(), pyramid_z);
	glScalef(1, pyramid_scl, 1);
	glRotatef(angle, 0, 1, 0);
	draw_pyramid();

	// End of frame
	glutSwapBuffers();
}



// write function to process keyboard events






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
	glutKeyboardFunc(keyboard_handler);

	
// put here the registration of the keyboard callbacks




//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
