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



int currCam = 0;
struct {
	float camX = 0;
	float camY = 30;
	float camZ = 40;
} CAM_FLOAT;

struct {
	float x = 100;
	float y = 1;
	float z = 100;
	float angle = 0;
} CAM_FPS;

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

float getHeight(int x, int z){
	return ((float)img_data[x + img_height*z])/(255.0f/60.0f);
}

float getHeightf(float x, float z){
	float x1 = floor(x);
	float x2 = x1 + 1;
	float z1 = floor(z);
	float z2 = z1 + 1;
	float fx = x - x1;
	float fz = z - z1;
	float h_x1_z = getHeight(x1, z1) * (1 - fz) + getHeight(x1, z2) * fz;
	float h_x2_z = getHeight(x2, z1) * (1 - fz) + getHeight(x2, z2) * fz;
	return h_x1_z * (1 - fx) + h_x2_z * fx;
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
	switch(key){
		case 'w':
			CAM_FPS.x += cos(CAM_FPS.angle);
			CAM_FPS.z += sin(CAM_FPS.angle);
			break;
		case 's':
			CAM_FPS.x -= cos(CAM_FPS.angle);
			CAM_FPS.z -= sin(CAM_FPS.angle);
			break;
		case 'a':
			CAM_FPS.x -= cos(CAM_FPS.angle + M_PI_2);
			CAM_FPS.z -= sin(CAM_FPS.angle + M_PI_2);
			break;
		case 'd':
			CAM_FPS.x += cos(CAM_FPS.angle + M_PI_2);
			CAM_FPS.z += sin(CAM_FPS.angle + M_PI_2);
			break;
		case 'q':
			CAM_FPS.angle -= M_PI/32;
			break;
		case 'e':
			CAM_FPS.angle += M_PI/32;
			break;
		case '+':
			CAM_FPS.y += 0.5;
			break;
		case '-':
			CAM_FPS.y -=0.5;
			break;
		case 'c':
			currCam = !currCam;
	}
// put code to process regular keys in here
}

void processMouseButtons(int button, int state, int xx, int yy) {
	if(currCam != 0) return;
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
	if(currCam != 0) return;
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
	CAM_FLOAT.camX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	CAM_FLOAT.camZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	CAM_FLOAT.camY = rAux * 							     sin(betaAux * 3.14 / 180.0);
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
		float tx = x + img_width/2;
		float tz = z + img_height/2;
		glTranslatef(tx, getHeightf(tx,tz), tz);
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
	glColor3f(0, 0, 1);
	float d_angle = 360.0/(float)N;
	for(int i=0; i<N; i++){
		glPushMatrix();
		float ang = i*d_angle*(M_PI/180.0f) + angle;
		float x = cos(ang)*radius + img_width/2;
		float z = sin(ang)*radius + img_height/2;
		glTranslatef(x, getHeightf(x, z), z);
		glRotatef(ang * (180.0f/M_PI), 0, -1, 0);
		draw_teapot(size);
		glPopMatrix();
	}
	glPopMatrix();
}

void draw_outer_teapots(int N, float radius, float size, float angle){
	glPushMatrix();
	glColor3f(1, 0, 0);
	float d_angle = 360.0/(float)N;
	for(int i=0; i<N; i++){
		glPushMatrix();
		float ang = i*d_angle*(M_PI/180.0f) + angle;
		float x = cos(-ang)*radius + img_width/2;
		float z = sin(-ang)*radius + img_height/2;
		glTranslatef(x, getHeightf(x, z), z);
		glRotatef(ang * (180.0f/M_PI) , 0, 1, 0);
		draw_teapot(size);
		glPopMatrix();
	}
	glPopMatrix();
}


void drawTerrain() {
	glPushMatrix();
	//glTranslatef((-(float)img_width)/2.0f, -10, -((float)img_height)/2.0f);

	glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	
	glDrawArrays(GL_TRIANGLES, 0, size);

	glPopMatrix();
}

void setupCamera(){
	if(currCam == 0)
        gluLookAt( CAM_FLOAT.camX + img_width/2
                 , CAM_FLOAT.camY
                 , CAM_FLOAT.camZ + img_height/2
                 , img_height/2,0.0,img_width/2
                 , 0.0f,1.0f,0.0f);
	else if(currCam == 1){
		gluLookAt( CAM_FPS.x
		         , getHeightf(CAM_FPS.x, CAM_FPS.z) + CAM_FPS.y
				 , CAM_FPS.z
				 , CAM_FPS.x + cos(CAM_FPS.angle)
				 , getHeightf(CAM_FPS.x, CAM_FPS.z) + CAM_FPS.y
				 , CAM_FPS.z + sin(CAM_FPS.angle)
				 , 0, 1, 0);
	}

}

float teapot_angle = 0;
void renderScene(void) {

	srand(0);
	float pos[4] = {-1.0, 1.0, 1.0, 0.0};

	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	setupCamera();

	draw_axis();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(1,1,1);
	drawTerrain();


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(0,1,0);
	//printFramerate();
	draw_trees(100);
	draw_inner_teapots(10, 20, 2, teapot_angle);
	draw_outer_teapots(10, 60, 2, teapot_angle);
	glColor3f(1.0, 170.0/255.0, 170.0/255.0);
	glTranslatef(img_width/2, getHeightf(img_width/2,img_height/2), img_height/2);
	glutSolidTorus(2, 5, 10, 10);

	teapot_angle += 0.01;
	teapot_angle = fmod(teapot_angle, 360.0);

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

	size = (img_width-1)*(img_height-1)*6*3;
	vertexB = (float*)calloc(size, sizeof(float));
	int tmp = 0;
	for(int i=0; i<(img_width-1); i++)
		for(int j=0; j<(img_width-1); j++){
			vertexB[tmp++] = i; vertexB[tmp++] = getHeight(i,j); vertexB[tmp++] = j;
			vertexB[tmp++] = i; vertexB[tmp++] = getHeight(i,j+1); vertexB[tmp++] = j+1;
			vertexB[tmp++] = i+1; vertexB[tmp++] = getHeight(i+1,j+1); vertexB[tmp++] = j+1;

			vertexB[tmp++] = i; vertexB[tmp++] = getHeight(i,j); vertexB[tmp++] = j;
			vertexB[tmp++] = i+1; vertexB[tmp++] = getHeight(i+1,j+1); vertexB[tmp++] = j+1;
			vertexB[tmp++] = i+1; vertexB[tmp++] = getHeight(i+1,j); vertexB[tmp++] = j;
		}
	printf("%d %d\n", size, tmp);

// 	OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glEnableClientState(GL_VERTEX_ARRAY);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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

