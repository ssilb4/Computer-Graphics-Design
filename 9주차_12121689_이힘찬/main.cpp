#include <gl/glut.h>

void init(void);
void draw(void);
void resize(int width, int height);

//�� �༺�� ����, ���� �ֱ�
double angle1 = 0, angle2 = 0, angle3 = 0, angle4 = 0, angle5 = 0, angle6 = 0
, angle7 = 0, angle8 = 0, angle9 = 0, angle10 = 0, angle11 = 0, angle12 = 0;

void idle(void) {
	angle1 = angle1 + 0.1;
	if (angle1 > 360) angle1 -= 360;				//���� ����
	angle2 = angle2 + 1;
	if (angle2 > 360) angle2 -= 360;				//���� ����
	angle3 = angle3 + 0.53;
	if (angle3 > 360) angle3 -= 360;				//ȭ�� ����
	angle4 = angle4 + 0.97;
	if (angle4 > 360) angle4 -= 360;				//ȭ�� ����
	angle5 = angle5 + 13.51;
	if (angle5 > 360) angle5 -= 360;				//�� ����
	angle6 = angle6 + 0.03;
	if (angle6 > 360) angle6 -= 360;				//�� ����
	angle7 = angle7 + 0.41;
	if (angle7 > 360) angle7 -= 360;				//���� ����
	angle8 = angle8 + 0.016;
	if (angle8 > 360) angle8 -= 360;				//���� ����
	angle9 = angle9 + 30.77;
	if (angle9 > 360) angle9 -= 360;				//�� ����
	angle10 = angle10 + 2.43;
	if (angle10 > 360) angle10 -= 360;				//�� ����
	angle11 = angle11 + 12.21;
	if (angle11 > 360) angle11 -= 360;				//�伺 ����
	angle12 = angle12 + 2.27;
	if (angle12 > 360) angle12 -= 360;				//�伺 ����

	glutPostRedisplay();										//��� �ٽ� ������
}
void init(void) {
	
	//Light ���� -> �ϴ� �׳� �̿�
	GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat specularLight[] = { 0.9f, 0.9f, 0.9f, 1.0f };
	GLfloat specularMaterial[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
	glMateriali(GL_FRONT, GL_SHININESS, 20);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glPolygonMode(GL_BACK, GL_LINE);
	//glEnable(GL_CULL_FACE);

	//Depth-Test Enable
	glFrontFace(GL_CW);						//teapot�� CW�� ���ǵ�
	glEnable(GL_DEPTH_TEST);
	
	/*ȭ���� �⺻������ black ����*/
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	/*ȭ�� ��ǥ ���� ����*/
	gluOrtho2D(0.0f, 2000.0f, 0.0f, 1000.0f);
}
void draw(void) {
	/*ȭ���� �����ϰ� �����*/
	//glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);

	glColor3f(1, 1, 0);									//���
	glutWireTeapot(1);									//�¾�

	glPushMatrix();										//�¾���ġ ���
	glPushMatrix();										//�¾���ġ ���
	glPushMatrix();										//�¾���ġ ���
	glPushMatrix();										//�¾���ġ ���

	glRotatef(angle1, 0, 1, 0);							//�¾� �߽� ���� ����
	glTranslatef(5, 0, 0);								//���� ��ġ
	glRotatef(angle2, 0, 1, 0);							//���� ����
	glColor3f(0, 1, 0);									//������ �ʷϻ�
	glutWireTeapot(1);									//����

	glRotatef(angle5, 0, 1, 0);							//���� �߽� �� ����
	glTranslatef(2, 0, 0);								//�� ��ġ
	glRotatef(angle6, 0, 1, 0);							//�� ����
	glColor3f(1, 1, 1);									//�� ���
	glutWireTeapot(1);									//��

	glPopMatrix();										//����, �� �ֱ� �� ��ġ

	glRotatef(angle3, 0, 1, 0);							//�¾� �߽� ȭ�� ����
	glTranslatef(8, 0, 0);								//ȭ�� ��ġ
	glRotatef(angle4, 0, 1, 0);							//ȭ�� ����
	glColor3f(1, 0, 0);									//ȭ���� ������
	glutWireTeapot(1);									//ȭ��

	glPopMatrix();										//����, �� �ֱ� �� ��ġ

	glRotatef(angle7, 0, 1, 0);							//�¾� �߽� �ݼ� ����
	glTranslatef(2, 0, 0);								//���� ��ġ
	glRotatef(angle8, 0, 1, 0);							//���� ����
	glColor3f(0, 0, 0.8);								//������ ��ο� �Ķ���
	glutWireTeapot(1);									//����

	glPopMatrix();										//����, �� �ֱ� �� ��ġ

	glRotatef(angle9, 0, 1, 0);							//�¾� �߽� �� ����
	glTranslatef(10, 0, 0);								//�� ��ġ
	glRotatef(angle10, 0, 1, 0);						//�� ����
	glColor3f(0.3, 0, 0);								//���� ����
	glutWireTeapot(1);									//��

	glPopMatrix();										//����, �� �ֱ� �� ��ġ

	glRotatef(angle11, 0, 1, 0);						//�¾� �߽� �伺 ����
	glTranslatef(12, 0, 0);								//�伺 ��ġ
	glRotatef(angle12, 0, 1, 0);						//�伺 ����
	glColor3f(0.5, 0.5, 0);								//�伺�� Ȳ���
	glutWireTeapot(1);									//�伺

	glFlush();
	glutSwapBuffers();						//double buffering��
}
void resize(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)width / (float)height, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
}
int main(int argc, char** argv) {
	/*window �ʱ�ȭ */

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(2000, 1000);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("My Seventh GL Program");

	init();		//->����� �ʱ�ȭ �Լ�

				//callback �Լ� ����
	glutDisplayFunc(draw);
	glutReshapeFunc(resize);
	glutIdleFunc(idle);

	//Looping ����
	glutMainLoop();

	return 0;
}