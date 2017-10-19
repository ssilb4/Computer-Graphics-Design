#include <gl/glut.h>
#include <string.h>

//���� �Լ���
void init(void);									//����� ���� �ʱ�ȭ �Լ�
void draw(void);									//�׸��� �Լ�
void resize(int width, int height);					//ũ�� ���� �Լ�
void draw_can(void);								//can �׸��� �Լ�
void draw_disk(void);								//disk �׸��� �Լ�
void draw_teapot(void);								//������ �׸��� �Լ�
void draw_text(void);								//���� ���� �Լ�
void draw_axis(void);								//�� �׸��� �Լ�

bool is_rotate = true;								//ȸ������
double spin = 0;									//ȸ����
float color0 = 0.0f, color1 = 1.0f;					//�� ����

void init(void) {
	//Depth ���
	glEnable(GL_DEPTH_TEST);

	/*ȭ���� �⺻������ white ����*/
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	/*ȭ�� ��ǥ ���� ����*/
	gluOrtho2D(0.0f, 500.0f, 0.0f, 500.0f);
}
void idle(void) {
	if(is_rotate == true)										//is_rotate�� true��
		spin = spin + 0.1;										//0.1�� ȸ��
	if (spin >= 360) {											//ȸ������ 360�� ������
		spin -= 360;											//360�� ����
	}
	glutPostRedisplay();										//��� �ٽ� ������
}
void draw_string(void* font, const char* str, float x_position, float y_position, float red, float green, float blue) {
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(-5, 5, -5, 5);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glLoadIdentity();
	glColor3f(red, green, blue);
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
void draw(void) {
	/*ȭ���� �����ϰ� �����*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);

	draw_text();													//���� ǥ��
	draw_disk();													//��ũ ǥ��
	draw_teapot();													//������ ǥ��
	draw_can();														//ĵ ǥ��

	glutSwapBuffers();
}
void draw_can(void) {
	glPushMatrix();													//���⼭�� ����ǰ� �ϱ� ���� ����

	glViewport(0, 0, 800 / 2, 600 / 2);								//3��и�
	draw_axis();													//�� �׸�
	glRotatef(spin, 1, 0, 0);

	GLUquadricObj* m_pQuadric1;
	m_pQuadric1 = gluNewQuadric();
	gluQuadricDrawStyle(m_pQuadric1, GLU_FILL);
	gluQuadricNormals(m_pQuadric1, GLU_SMOOTH);

	GLUquadricObj* m_pQuadric2;
	m_pQuadric2 = gluNewQuadric();
	gluQuadricDrawStyle(m_pQuadric2, GLU_FILL);
	gluQuadricNormals(m_pQuadric2, GLU_SMOOTH);

	glTranslatef(0, 0, 2);											//�Ѳ���ġ
	glColor3f(color1, color0, color0);
	gluDisk(m_pQuadric2, 0, 2, 20, 10);								//�Ѳ� �׸�
	glTranslatef(0, 0, -4);											//�� ��ġ
	glColor3f(color0, color1, color0);
	gluCylinder(m_pQuadric1, 2, 2, 4, 10, 10);						//�� �׸�
	glColor3f(color1, color0, color0);
	gluDisk(m_pQuadric2, 0, 2, 20, 10);								//�Ѳ� �׸�

	glFlush();

	glPopMatrix();													//���� ��
}
void draw_disk(void) {
	glPushMatrix();													//���⼭�� ����ǰ� �ϱ� ���� ����

	glViewport(800 / 2, 600 / 2, 800 / 2, 600 / 2);					//1��и�
	draw_axis();													//�� �׸�
	glColor3f(color0, color1, color1);
	glRotatef(spin, 0, 1, 0);
	GLUquadricObj* m_pQuadric2;
	m_pQuadric2 = gluNewQuadric();
	gluQuadricDrawStyle(m_pQuadric2, GLU_FILL);
	gluQuadricNormals(m_pQuadric2, GLU_SMOOTH);
	gluDisk(m_pQuadric2, 2, 4, 20, 10);								//��ũ �׸�

	glFlush();

	glPopMatrix();													//���� ��
}
void draw_teapot(void) {
	glPushMatrix();													//���⼭�� ����ǰ� �ϱ� ���� ����

	glViewport(800 / 2, 0, 800 / 2, 600 / 2);						//2��и�
	draw_axis();													//�� �׸�
	glColor3f(color1, color1, color0);
	glRotatef(spin, 0, 0, 1);
	glutWireTeapot(2);												//������ �׸�

	glFlush();

	glPopMatrix();													//���� ��
}
void draw_text(void) {
	glPushMatrix();

	glViewport(0, 600 / 2, 800 / 2, 600 / 2);						//4��и�
	glLoadIdentity();			//������ ȸ����ȯ�� ����
	draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Graphics are cool!!", 0, 0, color0, color0, color1);		//��Ʈ, ����, ��ġ, �� ����
	draw_string(GLUT_BITMAP_HELVETICA_18, "Multi-viewport Example!!", 0, 1, color1, color0, color1);


	glFlush();

	glPopMatrix();
}
void draw_axis(void) {
	glLineWidth(3);							//��ǥ���� �β�
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);						//x���� red
	glVertex3f(0, 0, 0);
	glVertex3f(4, 0, 0);

	glColor3f(0, 1, 0);						//y���� green
	glVertex3f(0, 0, 0);
	glVertex3f(0, 4, 0);

	glColor3f(0, 0, 1);						//z���� blue
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 4);

	glEnd();
	glLineWidth(1);
}
void keyboard(unsigned char key, int x, int y) {
	if (key == 's') {										//s�� ������ ��
		is_rotate = is_rotate == true ? false : true;		//is_rotate ���� ���
	}
	else if (key == 'c') {									//c�� ������ ��
		color0 = color0 == 1.0f ? 0.0f : 1.0f;				//color ������ 0�� 1�� ���
		color1 = color1 == 1.0f ? 0.0f : 1.0f;
	}
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
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("My Forth GL Program");

	init();		//->����� �ʱ�ȭ �Լ�

				//callback �Լ� ����
	glutKeyboardFunc(keyboard);								//Ű�����Է�
	glutReshapeFunc(resize);
	glutDisplayFunc(draw);
	glutIdleFunc(idle);

	//Looping ����
	glutMainLoop();

	return 0;
}