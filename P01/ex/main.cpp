#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>


double width;
double height;

double ratio(){
	return width/height;
}

void changeSize(int w, int h) {

    // Prevent a divide by zero, when window is too short
    // (you cant make a window with zero width).
    if(h == 0)
        h = 1;

    // compute window's aspect ratio
    width = w;
	height = h;
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



float tmp = 0.5;
float tmp_rot = 0;
void renderScene(void) {

    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // set the camera
    glLoadIdentity();
    gluLookAt(0.0,0.0,5.0,
              0.0,0.0,-1.0,
              0.0f,1.0f,0.0f);


// put drawing instructions here
	//glRotatef(tmp_rot*45,0,1,0);
	glRotatef(cos(tmp_rot)*45,1,0,0);
	glRotatef(sin(tmp_rot)*45,0,1,0);
	glRotatef((cos(tmp_rot)+sin(tmp_rot))*45,0,0,1);
	glutWireTeapot(abs(sin(tmp))+0.5);
	tmp+=0.005;
	tmp_rot+=0.005;



    // End of frame
    glutSwapBuffers();
}




int main(int argc, char **argv) {

// put GLUT init here
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,800);
    glutCreateWindow("CG@Di");
	glutReshapeFunc(changeSize);
// put callback registration here
    glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);


// OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glClearColor(0.0f,0.0f,0.0f,0.0f);

// enter GLUT's main loop
    glutMainLoop();

    return 1;
}
