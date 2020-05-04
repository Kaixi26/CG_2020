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

float cam_alpha = M_PI_4;
float cam_beta = M_PI_4;
float cam_rad = 3;
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

void vertexPolar(const float alpha,
				 const float beta,
				 const float rad){
	glVertex3f(rad*cos(beta)*sin(alpha),
			   rad*sin(beta),
			   rad*cos(beta)*cos(alpha));
}

void drawSphere(const float radius, const float slices, const float stacks){
	const float beta_d = M_PI/stacks;
	const float alpha_d = (2*M_PI)/slices;
	srand(0);
	glBegin(GL_TRIANGLES);
	for(int i = 0; i<slices; i++)
		for(int j = 0; j<stacks; j++){
			glColor3f((float)rand()/INT_MAX,
					  (float)rand()/INT_MAX,
					  (float)rand()/INT_MAX);
			vertexPolar(alpha_d*i-M_PI, beta_d*j-M_PI_2, radius);
			vertexPolar(alpha_d*(i+1)-M_PI, beta_d*j-M_PI_2, radius);
			vertexPolar(alpha_d*(i+1)-M_PI, beta_d*(j+1)-M_PI_2, radius);
			glColor3f((float)rand()/INT_MAX,
					  (float)rand()/INT_MAX,
					  (float)rand()/INT_MAX);
			vertexPolar(alpha_d*i-M_PI, beta_d*j-M_PI_2, radius);
			vertexPolar(alpha_d*(i+1)-M_PI, beta_d*(j+1)-M_PI_2, radius);
			vertexPolar(alpha_d*i-M_PI, beta_d*(j+1)-M_PI_2, radius);

		}
	glEnd();
}

void drawCone(const float radius, const float height, const int slices, const int stacks){
	const float vertical_diff = height/stacks;
	const float alpha_diff = (2*M_PI)/slices;
	glColor3f(1.0f, 1.0f, 1.0f);
	srand(0);
	glBegin(GL_TRIANGLES);
	for(int i=0; i<slices; i++)
		for(int j=0; j<stacks; j++){
			float col = (float)rand()/INT_MAX;
			glColor3f((float)rand()/INT_MAX,
					  (float)rand()/INT_MAX,
					  (float)rand()/INT_MAX);
			float curr_rad =
				((height - j*vertical_diff)*radius) / height;
			float upper_rad =
				((height - (j+1)*vertical_diff)*radius) / height;

			if(j != (stacks-1)){
			glVertex3f(sin(alpha_diff*i)*curr_rad,
					   j*vertical_diff,
					   cos(alpha_diff*i)*curr_rad);
			glVertex3f(sin(alpha_diff*(i+1))*curr_rad,
					   j*vertical_diff,
					   cos(alpha_diff*(i+1))*curr_rad);
			glVertex3f(sin(alpha_diff*i)*upper_rad,
					   (j+1)*vertical_diff,
					   cos(alpha_diff*i)*upper_rad);
			}

			glColor3f((float)rand()/INT_MAX,
					  (float)rand()/INT_MAX,
					  (float)rand()/INT_MAX);
			glVertex3f(sin(alpha_diff*(i+1))*curr_rad,
					   j*vertical_diff,
					   cos(alpha_diff*(i+1))*curr_rad);
			glVertex3f(sin(alpha_diff*(i+1))*upper_rad,
					   (j+1)*vertical_diff,
					   cos(alpha_diff*(i+1))*upper_rad);
			glVertex3f(sin(alpha_diff*i)*upper_rad,
					   (j+1)*vertical_diff,
					   cos(alpha_diff*i)*upper_rad);

		}
	for(int i=0; i<slices; i++){
			glColor3f((float)rand()/INT_MAX,
					  (float)rand()/INT_MAX,
					  (float)rand()/INT_MAX);
			glVertex3f(0.0f, 0.0f, 0.0f);
			glVertex3f(sin(alpha_diff*(i+1))*radius,
					   0.0f,
					   cos(alpha_diff*(i+1))*radius);
			glVertex3f(sin(alpha_diff*i)*radius,
					   0.0f,
					   cos(alpha_diff*i)*radius);
	}
	glEnd();
}

void colRand(){
			glColor3f((float)rand()/INT_MAX,
					  (float)rand()/INT_MAX,
					  (float)rand()/INT_MAX);
}

void drawBox(const float x, const float y, const float z, const int div){
	srand(0);
	const float d_x = x/div;
	const float d_y = y/div;
	const float d_z = z/div;
	glBegin(GL_TRIANGLES);
	for(int i=0; i<div; i++)
		for(int j=0; j<div; j++){
			colRand();
			glVertex3f(j*d_x - x/2, y/2, i*d_z - z/2);
			glVertex3f((j+1)*d_x - x/2, y/2, (i+1)*d_z - z/2);
			glVertex3f((j+1)*d_x - x/2, y/2, i*d_z - z/2);
			colRand();
			glVertex3f(j*d_x - x/2, y/2, i*d_z - z/2);
			glVertex3f(j*d_x - x/2, y/2, (i+1)*d_z - z/2);
			glVertex3f((j+1)*d_x - x/2, y/2, (i+1)*d_z - z/2);
			colRand();
			glVertex3f(j*d_x - x/2, -y/2, i*d_z - z/2);
			glVertex3f((j+1)*d_x - x/2, -y/2, i*d_z - z/2);
			glVertex3f((j+1)*d_x - x/2, -y/2, (i+1)*d_z - z/2);
			colRand();
			glVertex3f(j*d_x - x/2, -y/2, i*d_z - z/2);
			glVertex3f((j+1)*d_x - x/2, -y/2, (i+1)*d_z - z/2);
			glVertex3f(j*d_x - x/2, -y/2, (i+1)*d_z - z/2);
		}
	for(int j=0; j<div; j++)
		for(int k=0; k<div; k++){
			colRand();
			glVertex3f(j*d_x - x/2, k*d_y - y/2, z/2);
			glVertex3f((j+1)*d_x - x/2, (k+1)*d_y - y/2, z/2);
			glVertex3f(j*d_x - x/2, (k+1)*d_y - y/2, z/2);
			colRand();
			glVertex3f(j*d_x - x/2, k*d_y - y/2, z/2);
			glVertex3f((j+1)*d_x - x/2, k*d_y - y/2, z/2);
			glVertex3f((j+1)*d_x - x/2, (k+1)*d_y - y/2, z/2);
			colRand();
			glVertex3f(j*d_x - x/2, k*d_y - y/2, -z/2);
			glVertex3f(j*d_x - x/2, (k+1)*d_y - y/2, -z/2);
			glVertex3f((j+1)*d_x - x/2, (k+1)*d_y - y/2, -z/2);
			colRand();
			glVertex3f(j*d_x - x/2, k*d_y - y/2, -z/2);
			glVertex3f((j+1)*d_x - x/2, (k+1)*d_y - y/2, -z/2);
			glVertex3f((j+1)*d_x - x/2, k*d_y - y/2, -z/2);
		}
	for(int i=0; i<div; i++)
		for(int k=0; k<div; k++){
			colRand();
			glVertex3f(-x/2, k*d_y - y/2, i*d_z - z/2);
			glVertex3f(-x/2, (k+1)*d_y - y/2, (i+1)*d_z - z/2);
			glVertex3f(-x/2, (k+1)*d_y - y/2, i*d_z - z/2);
			colRand();
			glVertex3f(-x/2, k*d_y - y/2, i*d_z - z/2);
			glVertex3f(-x/2, k*d_y - y/2, (i+1)*d_z - z/2);
			glVertex3f(-x/2, (k+1)*d_y - y/2, (i+1)*d_z - z/2);
			colRand();
			glVertex3f(x/2, k*d_y - y/2, i*d_z - z/2);
			glVertex3f(x/2, (k+1)*d_y - y/2, i*d_z - z/2);
			glVertex3f(x/2, (k+1)*d_y - y/2, (i+1)*d_z - z/2);
			colRand();
			glVertex3f(x/2, k*d_y - y/2, i*d_z - z/2);
			glVertex3f(x/2, (k+1)*d_y - y/2, (i+1)*d_z - z/2);
			glVertex3f(x/2, k*d_y - y/2, (i+1)*d_z - z/2);
		}
	glEnd();
}

void drawCylinder(float radius, float height, int slices) {

	glColor3f(0.0f, 0.0f, 0.0f);
	//top
	float alpha_diff = (2*M_PI)/slices;
	glBegin(GL_TRIANGLES);
	for(int i=0; i<slices; i++){
		glColor3f((float)i/(slices*2) + 0.5, 0.0f, 0.0f);
		glVertex3f(0.0f, height/2, 0.0f);
		glVertex3f(sin(alpha_diff*i)*radius,
				   height/2,
				   cos(alpha_diff*i)*radius);
		glVertex3f(sin(alpha_diff*(i+1))*radius,
				   height/2,
				   cos(alpha_diff*(i+1))*radius);
	}
	//upper mid
	for(int i=0; i<=slices; i++){
		glColor3f(0.0f, (float)i/(slices*2) + 0.5, 0.0f);
		glVertex3f(sin(alpha_diff*i)*radius,
				   height/2,
				   cos(alpha_diff*i)*radius);
		glVertex3f(sin(alpha_diff*(i+0.5))*radius,
				   -height/2,
				   cos(alpha_diff*(i+0.5))*radius);
		glVertex3f(sin(alpha_diff*(i+1))*radius,
				   height/2,
				   cos(alpha_diff*(i+1))*radius);

	}
	//lower mid
	for(int i=0; i<=slices; i++){
		glColor3f(0.0f, 0.0f, (float)i/(slices*2) + 0.5);
		glVertex3f(sin(alpha_diff*(i+0.5))*radius,
				   -height/2,
				   cos(alpha_diff*(i+0.5))*radius);
		glVertex3f(sin(alpha_diff*(i+1.5))*radius,
				   -height/2,
				   cos(alpha_diff*(i+1.5))*radius);
		glVertex3f(sin(alpha_diff*(i+1))*radius,
				   height/2,
				   cos(alpha_diff*(i+1))*radius);
	}
	//bottom
	for(int i=0; i<slices; i++){
		glColor3f((float)i/(slices*2) + 0.5, 0.0f, 0.0f);
		glVertex3f(0.0f, -height/2, 0.0f);
		glVertex3f(sin(alpha_diff*(i+1.5))*radius,
				   -height/2,
				   cos(alpha_diff*(i+1.5))*radius);
		glVertex3f(sin(alpha_diff*(i+0.5))*radius,
				   -height/2,
				   cos(alpha_diff*(i+0.5))*radius);
	}
	glEnd();
}


void renderScene(void) {

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
	//drawCylinder(radius,height,slices);
	//glColor3f(1.0f, 1.0f, 1.0f);
	//glutWireSphere(radius, slices, stacks);
	//glutWireCone(radius, height, slices, stacks);
	drawCone(radius, height, slices, stacks);
	//drawSphere(radius, slices, stacks);
	//drawBox(2,0.5,1,slices);
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
			cam_rad -= 0.1;
			break;
		case '-':
			cam_rad += 0.1;
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
