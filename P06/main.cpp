#include<stdio.h>
#include<stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

#include <IL/il.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <time.h>



float camX = 00, camY = 30, camZ = 40;
int startX, startY, tracking = 0;

int alpha = 0, beta = 45, r = 50;

unsigned int t;
int img_width;
int img_height;
unsigned char* img_data;
float* vertexB = NULL;

int size;
GLuint buffers[1];


void printFramerate(){
	static int frames = 0;
	static time_t lastTime;
	static time_t currentTime;
	if(frames == 0){
		time(&lastTime);
	}
	time(&currentTime);
	time_t tmp = currentTime - lastTime;
	frames++;
	if(gmtime(&tmp)->tm_sec >= 1){
		printf("FPS: %d\n", frames);
		frames = 0;
	}
}

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
    glViewport(0, 0, w, h);

	// Set the correct perspective
	gluPerspective(45,ratio,1,1000);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}

void processKeys(unsigned char key, int xx, int yy) {

// put code to process regular keys in here
}

void processMouseButtons(int button, int state, int xx, int yy) {
	
	if (state == GLUT_DOWN)  {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
		else
			tracking = 0;
	}
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha += (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {
			
			r -= yy - startY;
			if (r < 3)
				r = 3.0;
		}
		tracking = 0;
	}
}


void processMouseMotion(int xx, int yy) {

	int deltaX, deltaY;
	int alphaAux, betaAux;
	int rAux;

	if (!tracking)
		return;

	deltaX = xx - startX;
	deltaY = yy - startY;

	if (tracking == 1) {


		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0)
			betaAux = 85.0;
		else if (betaAux < -85.0)
			betaAux = -85.0;

		rAux = r;
	}
	else if (tracking == 2) {

		alphaAux = alpha;
		betaAux = beta;
		rAux = r - deltaY;
		if (rAux < 3)
			rAux = 3;
	}
	camX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camY = rAux * 							     sin(betaAux * 3.14 / 180.0);
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

float getHeight(int x, int z){
	return ((float)img_data[x + img_height*z])/(255.0f/60.0f);
}

void drawTerrain() {
	glPushMatrix();
	glTranslatef((-(float)img_width)/2.0f, -10, -((float)img_height)/2.0f);

	glColor3f(1,1,1);
	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	
	for(int j=0; j<img_height-1; j++)
		glDrawArrays(GL_TRIANGLE_STRIP, j*(img_width-1)*2, (img_width-1)*2);

	glPopMatrix();
}

void renderScene(void) {

	float pos[4] = {-1.0, 1.0, 1.0, 0.0};

	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(camX, camY, camZ, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);

	//draw_axis();
	drawTerrain();
	printFramerate();

	// just so that it renders something before the terrain is built
	// to erase when the terrain is ready

// End of frame
	glutSwapBuffers();
}

void loadImage(){
	ilGenImages(1, &t);
	ilBindImage(t);
	ilLoadImage((ILstring)"terreno.jpg");
	ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);
	
	img_width = ilGetInteger(IL_IMAGE_WIDTH);
	img_height = ilGetInteger(IL_IMAGE_HEIGHT);
	img_data = ilGetData();
}

void init() {

// 	Load the height map "terreno.jpg"
	loadImage();

// 	Build the vertex arrays

	size = (img_width-1)*(img_height-1)*3*2;
	vertexB = (float*)calloc(size, sizeof(float));
	int tmp = 0;
	for(int j=0; j<(img_height-1); j++){
		glBegin(GL_TRIANGLE_STRIP);
		for(int i=0; i<(img_width-1); i++){
			vertexB[tmp++] = i;
			vertexB[tmp++] = getHeight(i,j);
			vertexB[tmp++] = j;

			vertexB[tmp++] = i;
			vertexB[tmp++] = getHeight(i,j+1);
			vertexB[tmp++] = j+1;
		}
		glEnd();
	}
	printf("%d %d\n", size, tmp);

// 	OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnableClientState(GL_VERTEX_ARRAY);
	glGenBuffers(1, buffers);
	glBindBuffer(GL_ARRAY_BUFFER,buffers[0]);
	glVertexPointer(3,GL_FLOAT,0,0);
	glBufferData(GL_ARRAY_BUFFER, tmp * sizeof(float), vertexB, GL_STATIC_DRAW);

}


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(320,320);
	glutCreateWindow("CG@DI-UM");
		

// Required callback registry 
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);

	glewInit();
	ilInit();
	init();	

// enter GLUT's main cycle
	glutMainLoop();
	
	return 0;
}

