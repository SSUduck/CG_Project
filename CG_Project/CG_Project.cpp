#include <windows.h>
#include <glut.h>

void reshape(int w, int h) {
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluOrtho2D(0.0, 100.0, 0.0, 100.0);
}

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);
	glRectf(30.0, 30.0, 50.0, 50.0);
	glutSwapBuffers();
}

void main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("example");
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMainLoop();
}