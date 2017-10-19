#pragma comment(lib, "legacy_stdio_definitions.lib")

#define M_PI 3.14159265358979323846					//������

#include <gl/glut.h>
#include <string.h>
#include <glaux.h>
#include <stdio.h>
#include <math.h>

void init(void);
void draw(void);
void resize(int width, int height);
void keyboard(unsigned char key, int x, int y);
void draw_string(void* font, const char* str, float x_position, float y_position);
void draw_text(void);
void emission_on(float r, float g, float b);
void emission_off();
void change_color(GLfloat arr[], float a, float b, float c);
void make_cube(float x1, float y1, float z1, float x2, float y2, float z2);
void make_square(float x1, float y1, float z1, float x2, float y2, float z2);
void draw_axis(void);
void draw_can(void); 
void draw_sun(void);

GLfloat emission[] = { 1.0f, 0.0f, 0.0f, 1.0f };

static double x_view = 20, y_view = 0, z_view = 0;
static int up = 1;							//ī�޶��� ���Ʒ�
static double r = sqrt(400);									//r�� �ʱⰪ
static double theta = acos(z_view / r);							//theta�� ���ϴ� ����
static double phi = atan(y_view / x_view);						//phi�� ���ϴ� ����
static int mouse_state = -1;
static int beforeX = -1, beforeY = -1;
static double tempX = 0, tempY = 0;

//�� �༺�� ����, ���� �ֱ�
double angle = 0;
double angle1[9] = { 0, };
double angle2[9] = { 0, };

int light_location = 1;
GLfloat light_y = 0.0;

GLfloat a[] = { 0.2f, 0.2f, 0.2f, 1.0f };
GLfloat b[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat c[] = { 1.0f, 1.0f, 1.0f, 1.0f };

GLuint texName[6];
GLuint texName2[4];
GLuint texName3[11];
GLUquadricObj* m_pQuadric1;

int current_state = 0;

void idle(void) {
	angle = angle + 0.05;
	if (angle > 360) angle -= 360;						//
	angle1[0] = angle1[0] + 1.141;
	if (angle1[0] > 360) angle1[0] -= 360;				//���� ����
	angle2[0] = angle2[0] + 0.0062;
	if (angle2[0] > 360) angle2[0] -= 360;				//���� ����
	angle1[1] = angle1[1] + 0.441;
	if (angle1[1] > 360) angle1[1] -= 360;				//�ݼ� ����
	angle2[1] = angle2[1] + 0.0015;
	if (angle2[1] > 360) angle2[1] -= 360;				//�ݼ� ����
	angle1[2] = angle1[2] + 0.273;
	if (angle1[2] > 360) angle1[2] -= 360;				//���� ����
	angle2[2] = angle2[2] + 0.001;
	if (angle2[2] > 360) angle2[2] -= 360;				//���� ����
	angle1[3] = angle1[3] + 3.66;
	if (angle1[3] > 360) angle1[3] -= 360;				//�� ����
	angle2[3] = angle2[3] + 0.013;
	if (angle2[3] > 360) angle2[3] -= 360;				//�� ����
	angle1[4] = angle1[4] + 0.145;
	if (angle1[4] > 360) angle1[4] -= 360;				//ȭ�� ����
	angle2[4] = angle2[4] + 0.3557;
	if (angle2[4] > 360) angle2[4] -= 360;				//ȭ�� ����
	angle1[5] = angle1[5] + 0.023;
	if (angle1[5] > 360) angle1[5] -= 360;				//�� ����
	angle2[5] = angle2[5] + 0.8902;
	if (angle2[5] > 360) angle2[5] -= 360;				//�� ����
	angle1[6] = angle1[6] + 0.009;
	if (angle1[6] > 360) angle1[6] -= 360;				//�伺 ����
	angle2[6] = angle2[6] + 0.8568;
	if (angle2[6] > 360) angle2[6] -= 360;				//�伺 ����
	angle1[7] = angle1[7] + 0.003;
	if (angle1[7] > 360) angle1[7] -= 360;				//õ�ռ� ����
	angle2[7] = angle2[7] + 0.5083;
	if (angle2[7] > 360) angle2[7] -= 360;				//õ�ռ� ����
	angle1[8] = angle1[8] + 0.002;
	if (angle1[8] > 360) angle1[8] -= 360;				//�ؿռ� ����
	angle2[8] = angle2[8] + 0.5456;
	if (angle2[8] > 360) angle2[8] -= 360;				//�ؿռ� ����
	
	glutPostRedisplay();										//��� �ٽ� ������
}
void init(void) {
	glGenTextures(6, texName);
	glGenTextures(4, texName2);
	glGenTextures(11, texName3);
	AUX_RGBImageRec* pRGBImage;

	//TexImage0.bmp
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	pRGBImage = auxDIBImageLoadA("TexImage0.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0,
		GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//TexImage1.bmp
	glBindTexture(GL_TEXTURE_2D, texName[1]);
	pRGBImage = auxDIBImageLoadA("TexImage1.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0,
		GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//TexImage2.bmp
	glBindTexture(GL_TEXTURE_2D, texName[2]);
	pRGBImage = auxDIBImageLoadA("TexImage2.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0,
		GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//TexImage3.bmp
	glBindTexture(GL_TEXTURE_2D, texName[3]);
	pRGBImage = auxDIBImageLoadA("TexImage3.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0,
		GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//TexImage4.bmp
	glBindTexture(GL_TEXTURE_2D, texName[4]);
	pRGBImage = auxDIBImageLoadA("TexImage4.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0,
		GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//TexImage5.bmp
	glBindTexture(GL_TEXTURE_2D, texName[5]);
	pRGBImage = auxDIBImageLoadA("TexImage5.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0,
		GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//cider_t.bmp
	glBindTexture(GL_TEXTURE_2D, texName2[0]);
	pRGBImage = auxDIBImageLoadA("cider_t.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0,
		GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//cider_b.bmp
	glBindTexture(GL_TEXTURE_2D, texName2[1]);
	pRGBImage = auxDIBImageLoadA("cider_b.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0,
		GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//cider_s.bmp
	glBindTexture(GL_TEXTURE_2D, texName2[2]);
	pRGBImage = auxDIBImageLoadA("cider_s.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0,
		GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//texture_sun.bmp
	glBindTexture(GL_TEXTURE_2D, texName3[0]);
	pRGBImage = auxDIBImageLoadA("texture_sun.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0,
		GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//texture_mercury.bmp
	glBindTexture(GL_TEXTURE_2D, texName3[1]);
	pRGBImage = auxDIBImageLoadA("texture_mercury.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0,
		GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//texture_venus_atmosphere.bmp
	glBindTexture(GL_TEXTURE_2D, texName3[2]);
	pRGBImage = auxDIBImageLoadA("texture_venus_atmosphere.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0,
		GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//texture_earth_clouds.bmp
	glBindTexture(GL_TEXTURE_2D, texName3[3]);
	pRGBImage = auxDIBImageLoadA("texture_earth_clouds.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0,
		GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// texture_moon.bmp
	glBindTexture(GL_TEXTURE_2D, texName3[4]);
	pRGBImage = auxDIBImageLoadA("texture_moon.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0,
		GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//texture_mars.bmp
	glBindTexture(GL_TEXTURE_2D, texName3[5]);
	pRGBImage = auxDIBImageLoadA("texture_mars.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0,
		GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//texture_jupiter.bmp
	glBindTexture(GL_TEXTURE_2D, texName3[6]);
	pRGBImage = auxDIBImageLoadA("texture_jupiter.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0,
		GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//texture_saturn.bmp
	glBindTexture(GL_TEXTURE_2D, texName3[7]);
	pRGBImage = auxDIBImageLoadA("texture_saturn.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0,
		GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//texture_saturn_ring.bmp
	glBindTexture(GL_TEXTURE_2D, texName3[8]);
	pRGBImage = auxDIBImageLoadA("texture_saturn_ring.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0,
		GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//texture_uranus.bmp
	glBindTexture(GL_TEXTURE_2D, texName3[9]);
	pRGBImage = auxDIBImageLoadA("texture_uranus.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0,
		GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//texture_neptune.bmp
	glBindTexture(GL_TEXTURE_2D, texName3[10]);
	pRGBImage = auxDIBImageLoadA("texture_neptune.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0,
		GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);\

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glEnable(GL_TEXTURE_2D);

	//0�� ���� ���� ����
	GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	//���� ����ġ�� 0�� ���� ����ġ �ѱ�
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//������ �ݻ� Ư�� ����
	GLfloat ambient_Sun[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuse_Sun[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat specular_Sun[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient_Sun);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse_Sun);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular_Sun);
	glMaterialf(GL_FRONT, GL_SHININESS, 64);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_DEPTH_TEST);

	m_pQuadric1 = gluNewQuadric();
	gluQuadricDrawStyle(m_pQuadric1, GLU_FILL);
	gluQuadricNormals(m_pQuadric1, GLU_SMOOTH);

	gluQuadricTexture(m_pQuadric1, true);				//textrue

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	/*ȭ�� ��ǥ ���� ����*/
	gluOrtho2D(0.0f, 1920.0f, 0.0f, 1080.0f);
}
void draw(void) {
	/*ȭ���� �����ϰ� �����*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	glColor3f(1, 1, 1);
	glDisable(GL_LIGHTING);
	draw_text();
	glEnd();
	
	if (current_state == 0) {
		glPushMatrix();
		gluLookAt(y_view, z_view, x_view, 0, 0, 0, 0, up, 0);
		draw_axis();
		glRotatef(angle, 0, 1, 0);
		glDisable(GL_LIGHTING);
		glColor3f(1, 1, 1);
		make_cube(-1, -1, -1, 1, 1, 1);
		glPopMatrix();
	}
	else if (current_state == 1) {
		glPushMatrix();
		gluLookAt(y_view, z_view, x_view, 0, 0, 0, 0, up, 0);
		draw_axis();
		draw_can();
		glPopMatrix();
	}
	else if (current_state == 2) {
		glPushMatrix();
		glEnable(GL_LIGHTING);
		gluLookAt(y_view, z_view, x_view, 0, 0, 0, 0, up, 0);
		draw_sun();
		glPopMatrix();
	}
	
	glFlush();
	glutSwapBuffers();						//double buffering��
}
void draw_sun(void) {
	glColor3f(1, 1, 1);
	GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	emission_on(1, 1, 1);
	glBindTexture(GL_TEXTURE_2D, texName3[0]);
	gluSphere(m_pQuadric1, 4, 20, 20);							//�¾�

	emission_off();

	glPushMatrix();										//�¾���ġ ���
	glPushMatrix();										//�¾���ġ ���
	glPushMatrix();										//�¾���ġ ���
	glPushMatrix();										//�¾���ġ ���
	glPushMatrix();										//�¾���ġ ���
	glPushMatrix();										//�¾���ġ ���
	glPushMatrix();										//�¾���ġ ���
	glPushMatrix();										//�¾���ġ ���

	glRotatef(angle1[0], 0, 1, 0);							//�¾� �߽� ���� ����
	glTranslatef(5, 0, 0);									//���� ��ġ
	glRotatef(angle2[0], 0, 1, 0);							//���� ����

	glBindTexture(GL_TEXTURE_2D, texName3[1]);
	gluSphere(m_pQuadric1, 1.2, 20, 20);;						//����

	glPopMatrix();											//����, �� �ֱ� �� ��ġ

	glRotatef(angle1[1], 0, 1, 0);							//�¾� �߽� �ݼ� ����
	glTranslatef(7, 0, 0);									//�ݼ� ��ġ
	glRotatef(angle2[1], 0, 1, 0);							//�ݼ� ����

	glBindTexture(GL_TEXTURE_2D, texName3[2]);
	gluSphere(m_pQuadric1, 1.6, 20, 20);;						//�ݼ�

	glPopMatrix();										//����, �� �ֱ� �� ��ġ

	glRotatef(angle1[2], 0, 1, 0);
	glTranslatef(7.7, 0, 0);
	glRotatef(angle2[2], 0, 1, 0);

	glBindTexture(GL_TEXTURE_2D, texName3[3]);
	gluSphere(m_pQuadric1, 1.65, 20, 20);						//����

	glRotatef(angle1[3], 0, 1, 0);
	glTranslatef(2, 0, 0);
	glRotatef(angle2[3], 0, 1, 0);

	glBindTexture(GL_TEXTURE_2D, texName3[4]);
	gluSphere(m_pQuadric1, 1.1, 20, 20);						//��

	glPopMatrix();

	glRotatef(angle1[4], 0, 1, 0);
	glTranslatef(8.8, 0, 0);
	glRotatef(angle2[4], 0, 1, 0);

	glBindTexture(GL_TEXTURE_2D, texName3[5]);
	gluSphere(m_pQuadric1, 1.34, 20, 20);						//ȭ��
	glPopMatrix();

	glRotatef(angle1[5], 0, 1, 0);
	glTranslatef(20.7, 0, 0);
	glRotatef(angle2[5], 0, 1, 0);

	glBindTexture(GL_TEXTURE_2D, texName3[6]);
	gluSphere(m_pQuadric1, 3, 20, 20);						//��
	glPopMatrix();

	glRotatef(angle1[6], 0, 1, 0);
	glTranslatef(30.6, 0, 0);
	glRotatef(angle2[6], 0, 1, 0);

	glBindTexture(GL_TEXTURE_2D, texName3[7]);
	gluSphere(m_pQuadric1, 2.8, 20, 20);						//�伺
	glRotatef(90, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, texName3[8]);
	gluDisk(m_pQuadric1, 3, 4, 20, 20);						//�伺
	glPopMatrix();

	glRotatef(angle1[7], 0, 1, 0);
	glTranslatef(54, 0, 0);
	glRotatef(angle2[7], 0, 1, 0);

	glBindTexture(GL_TEXTURE_2D, texName3[9]);
	gluSphere(m_pQuadric1, 2.3, 20, 20);						//õ�ռ�
	glPopMatrix();

	glRotatef(angle1[8], 0, 1, 0);
	glTranslatef(85, 0, 0);
	glRotatef(angle2[8], 0, 1, 0);

	glBindTexture(GL_TEXTURE_2D, texName3[10]);
	gluSphere(m_pQuadric1, 2.2, 20, 20);						//�ؿռ�
	glPopMatrix();
	glEnd();

}
void draw_axis(void) {
	glLineWidth(3);							//��ǥ���� �β�
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);						//x���� red
	glVertex3f(0, 0, 0);
	glVertex3f(4, 0, 0);

	glColor3f(0, 1, 0);						//x���� green
	glVertex3f(0, 0, 0);
	glVertex3f(0, 4, 0);

	glColor3f(0, 0, 1);						//x���� blue
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 4);

	glLineWidth(1);
	glEnd();
}
void draw_can(void) {
	glPushMatrix();													//���⼭�� ����ǰ� �ϱ� ���� ����
	glRotatef(angle, 1, 0, 0);
	glDisable(GL_LIGHTING);
	glColor3f(1, 1, 1);

	glTranslatef(0, 0, 2);											//�Ѳ���ġ
	glBindTexture(GL_TEXTURE_2D, texName2[0]);
	gluDisk(m_pQuadric1, 0, 2, 20, 10);								//�Ѳ� �׸�
	glTranslatef(0, 0, -4);											//�� ��ġ
	glBindTexture(GL_TEXTURE_2D, texName2[2]);
	gluCylinder(m_pQuadric1, 2, 2, 4, 10, 10);						// �� �׸�
	glBindTexture(GL_TEXTURE_2D, texName2[1]);
	gluDisk(m_pQuadric1, 0, 2, 20, 10);								//�Ѳ� �׸�
	

	glPopMatrix();													//���� ��

}
void make_cube(float x1, float y1, float z1, float x2, float y2, float z2) {
	//back
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	glBegin(GL_QUADS);
	make_square(x1, y1, z1, x1, y2, z2);
	glEnd();

	//front
	glBindTexture(GL_TEXTURE_2D, texName[1]);
	glBegin(GL_QUADS);
	make_square(x1, y1, z1, x2, y2, z1);
	glEnd();

	//left
	glBindTexture(GL_TEXTURE_2D, texName[2]);
	glBegin(GL_QUADS);
	make_square(x1, y1, z1, x2, y1, z2);
	glEnd();

	//right
	glBindTexture(GL_TEXTURE_2D, texName[3]);
	glBegin(GL_QUADS);
	make_square(x2, y2, z2, x1, y1, z2);
	glEnd();

	//top
	glBindTexture(GL_TEXTURE_2D, texName[4]);
	glBegin(GL_QUADS);
	make_square(x2, y2, z2, x1, y2, z1);
	glEnd();

	//bottom
	glBindTexture(GL_TEXTURE_2D, texName[5]);
	glBegin(GL_QUADS);
	make_square(x2, y2, z2, x2, y1, z1);
	glEnd();
	glFlush();
}
void make_square(float x1, float y1, float z1, float x2, float y2, float z2) {
	if (x1 == x2) {
		glTexCoord2f(0, 0); glVertex3f(x1, y1, z1);
		glTexCoord2f(1, 0); glVertex3f(x1, y1, z2);
		glTexCoord2f(1, 1); glVertex3f(x1, y2, z2);
		glTexCoord2f(0, 1); glVertex3f(x1, y2, z1);
	}
	else if (y1 == y2) {
		glTexCoord2f(0, 0); glVertex3f(x1, y1, z1);
		glTexCoord2f(1, 0); glVertex3f(x1, y1, z2);
		glTexCoord2f(1, 1); glVertex3f(x2, y1, z2);
		glTexCoord2f(0, 1); glVertex3f(x2, y1, z1);
	}
	else if (z1 == z2) {
		glTexCoord2f(0, 0); glVertex3f(x1, y1, z1);
		glTexCoord2f(1, 0); glVertex3f(x1, y2, z1);
		glTexCoord2f(1, 1); glVertex3f(x2, y2, z1);
		glTexCoord2f(0, 1); glVertex3f(x2, y1, z1);
	}
}
void keyboard(unsigned char key, int x, int y) {
	if (key == 'z') {
		current_state = 0;
	}
	else if (key == 'x') {
		current_state = 1;
	}
	else if (key == 'c'){
		current_state = 2;
	}
}
void mouse(int button, int state, int x, int y) {
	if (button == 0 && state == 0) {
		mouse_state = 0;
	}
	else if (button == 2 && state == 0) {
		mouse_state = 1;
	}
	else if (state == 1) {
		mouse_state = -1;
	}
}
void motion(int x, int y) {
	if (mouse_state == 0) {							//���� ��ư�� ������ ��
		tempX = beforeX - x;
		tempY = beforeY - y;
		if (tempX <30 && tempX >-30 && tempY <30 && tempY>-30) {
			phi = phi + M_PI / 180 * (tempX);
			theta = theta + M_PI / 180 * (tempY);
			if (theta < 0) theta += M_PI * 2;
			else if (theta >= M_PI * 2) theta -= M_PI * 2;
			if (phi < 0) phi += M_PI * 2;
			else if (phi >= M_PI * 2) phi -= M_PI * 2;
			if (theta >= 0 && theta < M_PI) up = 1;
			else up = -1;
			x_view = r*sin(theta)*cos(phi);						//������ǥ�踦 �̿��� x��ǥ ���ϴ� ����
			y_view = r*sin(theta)*sin(phi);						//������ǥ�踦 �̿��� y��ǥ ���ϴ� ����
			z_view = r*cos(theta);								//������ǥ�踦 �̿��� z��ǥ ���ϴ� ����
		}
		beforeX = x;
		beforeY = y;
	}
	else if (mouse_state == 1) {							//������ ��ư�� ������ ��
		tempX = beforeX - x;
		tempY = beforeY - y;
		if (tempX <30 && tempX >-30 && tempY <30 && tempY>-30) {
			r = r + M_PI / 180 * (tempX);
			if (r<0)
				r = 0.01;
			x_view = r*sin(theta)*cos(phi);						//������ǥ�踦 �̿��� x��ǥ ���ϴ� ����
			y_view = r*sin(theta)*sin(phi);						//������ǥ�踦 �̿��� y��ǥ ���ϴ� ����
			z_view = r*cos(theta);								//������ǥ�踦 �̿��� z��ǥ ���ϴ� ����
																//printf("%3f %3f %3f\n", x_view, y_view, z_view);
		}
		beforeX = x;
		beforeY = y;
	}
}
void emission_on(float r, float g, float b) {
	emission[0] = r;
	emission[1] = g;
	emission[2] = b;
	glMaterialfv(GL_FRONT, GL_EMISSION, emission);
}
void emission_off() {
	emission[0] = 0.0f;
	emission[1] = 0.0f;
	emission[2] = 0.0f;
	glMaterialfv(GL_FRONT, GL_EMISSION, emission);

}
void draw_string(void* font, const char* str, float x_position, float y_position) {
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-5, 5, -5, 5);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glLoadIdentity();
	glColor3f(1, 1, 0);
	glRasterPos3f(x_position, y_position, 0);
	for (unsigned int i = 0; i < strlen(str); i++) {
		glutBitmapCharacter(font, str[i]);
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopAttrib();
}
void draw_text(void) {
	glPushMatrix();
	glLoadIdentity();			//������ ȸ����ȯ�� ����
	draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "12121689 Lee Him Chan", -5, 4.5);		//��Ʈ, ����, ��ġ, �� ����
	char* content[3] = { "cube", "can", "solar_system" };
	draw_string(GLUT_BITMAP_TIMES_ROMAN_24, content[current_state], -5, 4);		//��Ʈ, ����, ��ġ, �� ����
	glFlush();
	glPopMatrix();
}
void resize(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)width / (float)height, 1, 500);
	glMatrixMode(GL_MODELVIEW);
}
int main(int argc, char** argv) {
	/*window �ʱ�ȭ */

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(1920, 1080);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("My Forth GL Program");

	init();		//->����� �ʱ�ȭ �Լ�

				//callback �Լ� ����
	glutDisplayFunc(draw);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);								//Ű�����Է�
	glutMouseFunc(mouse);									//���콺�Է�
	glutMotionFunc(motion);
	glutIdleFunc(idle);

	//Looping ����
	glutMainLoop();

	return 0;
}