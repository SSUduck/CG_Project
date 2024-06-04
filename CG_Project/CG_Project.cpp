#pragma warning(disable:4996)
#include <windows.h>
#include <iostream>
#include <glaux.h>
#include <glut.h>

#define GL_PI 3.1415f

class Texture {
public:
	GLuint loadBMP_custom(const char* imagepath);
};

GLuint Texture::loadBMP_custom(const char* imagepath) {
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int width, height;
	unsigned int imageSize;

	unsigned char* data;


	FILE* file = fopen(imagepath, "rb");
	if (!file) {
		printf("Image could not be opened\n");
		return 0;
	}

	if (fread(header, 1, 54, file) != 54) {
		printf("Not a correct BMP file\n");
		return false;
	}

	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
		return 0;
	}

	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);


	if (imageSize == 0)    imageSize = width * height * 3;
	if (dataPos == 0)      dataPos = 54;


	data = new unsigned char[imageSize];


	fread(data, 1, imageSize, file);


	fclose(file);


	GLuint textureID;
	glGenTextures(1, &textureID);


	glBindTexture(GL_TEXTURE_2D, textureID);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	return textureID;

}

void myDisplay();
void myReshape(int, int);
void init();
void drawModel();
void myKeyboard(unsigned char key, int x, int y);
void myMouseMotion(GLint X, GLint Y);
void createCylinder(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);

Texture tex;
GLuint BMP = 0;

int viewport;
GLfloat gYAngle, gXAngle, gZAngle;
GLint moveX, moveY;
GLUquadricObj* body;
GLUquadricObj* head;
GLUquadricObj* snout;

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
	float ambientColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };	// 주변광
	float diffuseColor[] = { 0.8f, 0.8f, 0.8f, 1.0f };	// 분산광
	float specularColor[] = { 0.1f, 0.1f, 0.1f, 1.0f };	// 반사광
	float emissionColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };	// 물체(방사도)
	float position[] = { 100.0f, 100.0f, 400.0f, 1.0f };	// 조명 위치

	glShadeModel(GL_SMOOTH);
	glFrontFace(GL_CCW);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	// 광원 설정
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientColor);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseColor);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularColor);
	glLightfv(GL_LIGHT0, GL_EMISSION, emissionColor);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	// 재질 설정
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientColor);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseColor);

	// 배경 색상 설정
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void drawModel() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0 + viewport, 0 + viewport, 600 - 2 * viewport, 600 - 2 * viewport);
	glColor3f(1.0, 1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// 마우스 드래그로 인한 물체의 회전
	glRotatef(gYAngle, 0.0f, 1.0f, 0.0f);
	glRotatef(gXAngle, 1.0f, 0.0f, 1.0f);
	glRotatef(gZAngle, 0.0f, 0.0f, 0.0f);

	// gluCylinder 함수 사용을 위한 설정 
	GLUquadricObj* obj;
	obj = gluNewQuadric();
	gluQuadricDrawStyle(obj, GLU_FILL);
	gluQuadricNormals(obj, GL_SMOOTH);

	// 머리
	head = gluNewQuadric();
	gluQuadricDrawStyle(head, GLU_FILL);
	BMP = tex.loadBMP_custom("./head.bmp");
	gluQuadricTexture(head, GLU_TRUE);
	glRotatef(270.0, 1.0, 0.0, 0.0);
	gluSphere(head, 0.2, 50, 30);
	glRotatef(90.0, 1.0, 0.0, 0.0);

	// 주둥이
	snout = gluNewQuadric();
	gluQuadricDrawStyle(snout, GLU_FILL);
	BMP = tex.loadBMP_custom("./snout.bmp");
	gluQuadricTexture(snout, GLU_TRUE);
	glTranslatef(0.0, 0.0, 0.15);
	glRotatef(270.0, 1.0, 0.0, 0.0);
	gluSphere(snout, 0.095, 20, 20);
	glRotatef(90.0, 1.0, 0.0, 0.0);

	// 코
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(0.0, 0.025, 0.1);
	glRotatef(180, 0.0, 1.0, 0.0);
	glutSolidSphere(0.03, 20, 20);
	glPopMatrix();

	// 왼쪽 귀
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(-0.17, 0.16, -0.2);
	glRotatef(180, 0.0, 1.0, 0.0);
	glutSolidSphere(0.06, 20, 20);
	glPopMatrix();

	// 오른쪽 귀
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(0.17, 0.16, -0.2);
	glRotatef(180, 0.0, 1.0, 0.0);
	glutSolidSphere(0.06, 20, 20);
	glPopMatrix();

	// 몸
	body = gluNewQuadric();
	gluQuadricDrawStyle(body, GLU_FILL);
	glColor3f(1.0, 1.0, 1.0);
	BMP = tex.loadBMP_custom("./body.bmp");
	gluQuadricTexture(body, GLU_TRUE);
	glTranslatef(0.0, -0.33, 0.0);
	glRotatef(270.0, 1.0, 0.0, 0.0);
	gluSphere(body, 0.249, 30, 20);
	glRotatef(90.0, 1.0, 0.0, 0.0);

	// 왼쪽 다리  
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(-0.1, -0.1, 0.1);
	glRotatef(75, 0.0, 0.0, 0.1);
	glRotatef(355.0, 0.0, 1.0, 0.0);
	gluCylinder(obj, 0.1, 0.07, 0.2, 30, 10);
	glPopMatrix();

	// 왼발
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(-0.1, -0.1, 0.3);
	glRotatef(180, 0.1, 0.0, 0.0);
	glutSolidSphere(0.08, 20, 20);
	glPopMatrix();

	// 왼 발가락
	glColor3f(0.25, 0.25, 0.25);

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

	// 오른쪽 다리
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(0.1, -0.1, 0.1);
	glRotatef(75, 0.0, 0.0, 0.1);
	glRotatef(3, 0.0, 0.1, 0.0);
	gluCylinder(obj, 0.1, 0.07, 0.2, 30, 30);
	glPopMatrix();

	// 오른 발
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(0.1, -0.1, 0.3);
	glRotatef(180, 0.1, 0.0, 0.0);
	glutSolidSphere(0.08, 20, 20);
	glPopMatrix();

	// 오른 발가락
	glColor3f(0.25, 0.25, 0.25);

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

	// 오른쪽 팔
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(0.11, 0.13, 0.0);
	glRotatef(90, 0.1, 0.3, 0.0);
	gluCylinder(obj, 0.08, 0.04, 0.2, 50, 1);

	// 오른손
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(0.0, -0.007, 0.2);
	glutSolidSphere(0.07, 20, 20);
	glPopMatrix();

	// 왼쪽 팔  
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(-0.11, 0.13, 0.0);
	glRotatef(90, 0.1, -0.3, 0.0);
	gluCylinder(obj, 0.08, 0.04, 0.2, 50, 1);

	// 왼손
	glColor3f(0.0, 0.0, 0.0);
	glTranslatef(0.0, -0.007, 0.2);
	glutSolidSphere(0.07, 20, 20);
	glPopMatrix();

	// 꼬리
	glPushMatrix();
	glColor3f(0.0, 0.0, 0.0);
	glRotatef(180, 0.0, 1.0, 0.0);
	glTranslatef(0.0, -0.13, 0.2);
	glutSolidSphere(0.07, 20, 20);
	glPopMatrix();

	// 대나무
	glPushMatrix();
	glColor3f(0.0, 0.7, 0.0);
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
	GLfloat x, y, angle;

	glBegin(GL_TRIANGLE_FAN);           //원기둥의 윗면
	glNormal3f(0.0f, 0.0f, -1.0f);
	//glColor3ub(0, 0, 0);
	glVertex3f(centerx, centery, centerz);

	for (angle = 0.0f; angle < (2.0f * GL_PI); angle += (GL_PI / 8.0f))
	{
		x = centerx + radius * sin(angle);
		y = centery + radius * cos(angle);
		glNormal3f(0.0f, 0.0f, -1.0f);
		glVertex3f(x, y, centerz);
	}
	glEnd();

	glBegin(GL_QUAD_STRIP);            //원기둥의 옆면
	for (angle = 0.0f; angle < (2.0f * GL_PI); angle += (GL_PI / 8.0f))
	{
		x = centerx + radius * sin(angle);
		y = centery + radius * cos(angle);
		glNormal3f(sin(angle), cos(angle), 0.0f);
		glVertex3f(x, y, centerz);
		glVertex3f(x, y, centerz + h);
	}
	glEnd();

	glBegin(GL_TRIANGLE_FAN);           //원기둥의 밑면
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
