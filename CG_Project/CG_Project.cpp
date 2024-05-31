#include <windows.h>
#include <iostream>
//#include <stdio.h>
//#include <glaux.h>
//#include <gl.h>
//#include <glu.h>
#include <glut.h>

#define GL_PI 3.1415f

void myDisplay();
void myReshape(int, int);
void init();
void drawModel();
void myKeyboard(unsigned char key, int x, int y);
void myMouseMotion(GLint X, GLint Y);
void createCylinder(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);

int viewport;
GLfloat gYAngle, gXAngle, gZAngle;
GLint moveX, moveY;
//GLuint texture[2];
GLUquadricObj* body;
GLUquadricObj* head;

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Fubao");

	glutDisplayFunc(myDisplay);
	glutReshapeFunc(myReshape);
	glutKeyboardFunc(myKeyboard);
	glutMotionFunc(myMouseMotion);
	glutMainLoop();
	return 0;
}

void myDisplay() {

	init();
	drawModel();

	glutSwapBuffers();
}

void myReshape(int w, int h) {	

	glLoadIdentity();
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
}

void init() {
	float ambientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };	// �ֺ���
	float diffuseColor[] = { 0.8f, 0.8f, 0.8f, 1.0f };	// �л걤
	float specularColor[] = { 0.1f, 0.1f, 0.1f, 1.0f };	// �ݻ籤
	float emissionColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };	// ��ü(��絵)
	float position[] = { 100.0f, 100.0f, 400.0f, 1.0f };	// ���� ��ġ

	glShadeModel(GL_SMOOTH);
	glFrontFace(GL_CCW);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// ���� ����
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientColor);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseColor);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularColor);
	glLightfv(GL_LIGHT0, GL_EMISSION, emissionColor);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	// ���� ����
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseColor);

	// ��� ���� ����
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void drawModel() {
	// ��ü�� ���� ǥ��
	GLfloat d1[] = { 0.0, 0.5, 0.83, 1.0 };
	GLfloat d2[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat d3[] = { 0.7, 0.0, 0.0, 1.0 };

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0 + viewport, 0 + viewport, 600 - 2 * viewport, 600 - 2 * viewport);
	glColor3f(1.0, 1.0, 1.0);
	////glBindTexture(GL_TEXTURE_2D, texture[0]);
	////glBindTexture(GL_TEXTURE_2D, texture[1]);
	//glMatrixMode(GL_PROJECTION);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// ���콺 �巡�׷� ���� ��ü�� ȸ��
	glRotatef(gYAngle, 0.0f, 1.0f, 0.0f);
	glRotatef(gXAngle, 1.0f, 0.0f, 1.0f);
	glRotatef(gZAngle, 0.0f, 0.0f, 0.0f);

	// gluCylinder �Լ� ����� ���� ���� 
	GLUquadricObj* obj;
	obj = gluNewQuadric();
	gluQuadricDrawStyle(obj, GLU_FILL);
	gluQuadricNormals(obj, GL_SMOOTH);

	//glutSolidSphere(0.3, 50, 30);

	// �Ӹ�
	head = gluNewQuadric();
	gluQuadricDrawStyle(head, GLU_FILL);
	//glBindTexture(GL_TEXTURE_2D, texture[0]);
	glRotatef(270.0, 1.0, 0.0, 0.0);
	gluSphere(head, 0.2, 50, 30);
	glRotatef(90.0, 1.0, 0.0, 0.0);

	// �ֵ���
	glPushMatrix();
	glTranslatef(0.0, 0.0, 0.15);
	glRotatef(180, 0.0, 1.0, 0.0);
	glutSolidSphere(0.095, 20, 20);
	glPopMatrix();

	// ��
	glPushMatrix();
	glTranslatef(0.0, 0.025, 0.23);
	glRotatef(180, 0.0, 1.0, 0.0);
	glutSolidSphere(0.03, 20, 20);
	glPopMatrix();

	// ���� ��
	glPushMatrix();
	glTranslatef(-0.16, 0.16, 0.0);
	glRotatef(180, 0.0, 1.0, 0.0);
	glutSolidSphere(0.06, 20, 20);
	glPopMatrix();

	// ������ ��
	glPushMatrix();
	glTranslatef(0.16, 0.16, 0.0);
	glRotatef(180, 0.0, 1.0, 0.0);
	glutSolidSphere(0.06, 20, 20);
	glPopMatrix();

	// ��
	body = gluNewQuadric();
	gluQuadricDrawStyle(body, GLU_FILL);
	//glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTranslatef(0.0, -0.33, 0.0);
	glRotatef(270.0, 1.0, 0.0, 0.0);
	gluSphere(head, 0.249, 30, 20);
	glRotatef(90.0, 1.0, 0.0, 0.0);

	// ���� �ٸ�  
	glMaterialfv(GL_FRONT, GL_DIFFUSE, d2);
	glPushMatrix();
	glTranslatef(-0.1, -0.1, 0.1);
	glRotatef(75, 0.0, 0.0, 0.1);
	glRotatef(355.0, 0.0, 1.0, 0.0);
	gluCylinder(obj, 0.1, 0.07, 0.2, 30, 10);
	glPopMatrix();

	// �޹�
	glPushMatrix();
	glTranslatef(-0.1, -0.1, 0.3);
	glRotatef(180, 0.1, 0.0, 0.0);
	glutSolidSphere(0.08, 20, 20);
	glPopMatrix();

	// �� �߰���
	glPushMatrix();
	glTranslatef(-0.06, -0.03, 0.3);
	glRotatef(180, 0.1, 0.0, 0.0);
	glutSolidSphere(0.025, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.1, -0.02, 0.3);
	glRotatef(180, 0.1, 0.0, 0.0);
	glutSolidSphere(0.025, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.14, -0.03, 0.3);
	glRotatef(180, 0.1, 0.0, 0.0);
	glutSolidSphere(0.025, 20, 20);
	glPopMatrix();

	// ������ �ٸ�
	glPushMatrix();
	glTranslatef(0.1, -0.1, 0.1);
	glRotatef(75, 0.0, 0.0, 0.1);
	glRotatef(3, 0.0, 0.1, 0.0);
	gluCylinder(obj, 0.1, 0.07, 0.2, 30, 30);
	glPopMatrix();

	// ���� ��
	glPushMatrix();
	glTranslatef(0.1, -0.1, 0.3);
	glRotatef(180, 0.1, 0.0, 0.0);
	glutSolidSphere(0.08, 20, 20);
	glPopMatrix();

	// ���� �߰���
	glPushMatrix();
	glTranslatef(0.06, -0.03, 0.3);
	glRotatef(180, 0.1, 0.0, 0.0);
	glutSolidSphere(0.025, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.1, -0.02, 0.3);
	glRotatef(180, 0.1, 0.0, 0.0);
	glutSolidSphere(0.025, 20, 20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.14, -0.03, 0.3);
	glRotatef(180, 0.1, 0.0, 0.0);
	glutSolidSphere(0.025, 20, 20);
	glPopMatrix();

	// ������ ��
	glPushMatrix();
	glTranslatef(0.11, 0.13, 0.0);
	glRotatef(90, 0.1, 0.3, 0.0);
	gluCylinder(obj, 0.08, 0.04, 0.2, 50, 1);

	// ������
	glTranslatef(0.0, -0.007, 0.2);
	glutSolidSphere(0.07, 20, 20);
	glPopMatrix();

	// ���� ��  
	glPushMatrix();
	glTranslatef(-0.11, 0.13, 0.0);
	glRotatef(90, 0.1, -0.3, 0.0);
	gluCylinder(obj, 0.08, 0.04, 0.2, 50, 1);

	// �޼�
	glTranslatef(0.0, -0.007, 0.2);
	glutSolidSphere(0.07, 20, 20);
	glPopMatrix();

	// ����
	glPushMatrix();
	glRotatef(180, 0.0, 1.0, 0.0);
	glTranslatef(0.0, -0.05, 0.2);
	glutSolidSphere(0.07, 20, 20);
	glPopMatrix();

	// �볪��
	glPushMatrix();
	glRotatef(150, 0.0, 0.8, 0.5);
	createCylinder(-0.3, 0.0, 0.0, 0.02, 0.6);

	glRotatef(60, 0.0, -0.8, 0.0);
	createCylinder(0.15, 0.0, 0.43, 0.02, 0.2);
	glPopMatrix();

	glFlush();
}

void myKeyboard(unsigned char key, int x, int y) {

	switch (key) {
		case 'a':	// zoom-in
			viewport -= 5;
			glutPostRedisplay();
			break;
		case 'z':	// zoom-out
			viewport += 5;
			glutPostRedisplay();
			break;
		case 'q':
			exit(0);
			break;
	}
}

void myMouseMotion(GLint x, GLint y) {

	if (x - moveX > 0)
		gYAngle += 5.0f;
	else if (x - moveX < 0)
		gYAngle -= 5.0f;
	if (y - moveY > 0)
		gXAngle += 5.0f;
	else if (y - moveY < 0)
		gXAngle -= 5.0f;
	moveX = x;
	moveY = y;
	glutPostRedisplay();
}

void createCylinder(GLfloat centerx, GLfloat centery, GLfloat centerz, GLfloat radius, GLfloat h) {
	/*
	function createCyliner()
	������� �߽� x,y,z��ǥ, ������, ���̸� �޾� ������� �����ϴ� �Լ�(+z�������� ������ �þ)
	centerx : ����� ���� �߽� x��ǥ
	centery : ����� ���� �߽� y��ǥ
	centerz : ����� ���� �߽� z��ǥ
	radius : ������� ������
	h : ������� ����
	*/
	GLfloat x, y, angle;

	glBegin(GL_TRIANGLE_FAN);           //������� ����
	glNormal3f(0.0f, 0.0f, -1.0f);
	//glColor3ub(139, 69, 19);
	glVertex3f(centerx, centery, centerz);

	for (angle = 0.0f; angle < (2.0f * GL_PI); angle += (GL_PI / 8.0f))
	{
		x = centerx + radius * sin(angle);
		y = centery + radius * cos(angle);
		glNormal3f(0.0f, 0.0f, -1.0f);
		glVertex3f(x, y, centerz);
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);            //������� ����
	for (angle = 0.0f; angle < (2.0f * GL_PI); angle += (GL_PI / 8.0f))
	{
		x = centerx + radius * sin(angle);
		y = centery + radius * cos(angle);
		glNormal3f(sin(angle), cos(angle), 0.0f);
		glVertex3f(x, y, centerz);
		glVertex3f(x, y, centerz + h);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);           //������� �ظ�
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(centerx, centery, centerz + h);
	for (angle = (2.0f * GL_PI); angle > 0.0f; angle -= (GL_PI / 8.0f))
	{
		x = centerx + radius * sin(angle);
		y = centery + radius * cos(angle);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glVertex3f(x, y, centerz + h);
	}
	glEnd();
}
