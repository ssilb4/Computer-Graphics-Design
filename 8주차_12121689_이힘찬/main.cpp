#include <gl/glut.h>
#include <math.h>
#include <stdio.h>

#define M_PI 3.14159265358979323846					//������

//�Լ�
void idle(void);									//�׻� ����Ǵ� ��
void init(void);									//����� �ʱ�ȭ
void draw(void);									//�׸��� �Լ�
void rotate(void);									//ȸ���ϴ� �Լ�
void resize(int width, int height);					//â ũ�Ⱑ ������� ��
void specialkeyboard(int key, int x, int y);		//����Ű�� ������ ��
void keyboard(unsigned char key, int x, int y);		//�Ϲ�Ű�� ������ ��

double m_RotateAngle;								//ȸ������ ����
bool is_rotate = true;								//ȸ�� ���� �ľ�
double Xp = 1, Yp = 0, Zp = 0;			//����� �������� x,y,z ��ǥ ����� �������� ��Ÿ���� ��
double xx = 10, yy = 10, zz = 10;					//ī�޶��� ��ġ�� ��Ÿ���� ������
int up = 1;											//ī�޶��� ���� ��Ÿ���� ����

const double r = sqrt(1);							//r�� ������
double theta = acos(Zp / r);						//theta�� ���ϴ� ����
double phi = atan(Yp / Xp);						//phi�� ���ϴ� ����

void idle(void) {									//�׻� ����
	if (is_rotate == true) {						//is_rotate�� Ʈ���� ��
		m_RotateAngle = m_RotateAngle + 0.5;		//ȸ�������� 0.5�� ����
	}
	glutPostRedisplay();							//��� �ٽ� ������
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
	/*ȭ���� �⺻������ Whtie ����*/
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	/*ȭ�� ��ǥ ���� ����*/
	gluOrtho2D(0.0f, 500.0f, 0.0f, 500.0f);
}
void rotate(void) {
	glRotatef(m_RotateAngle, 0, 1, 0);						//m_RotateAngle��ŭ ȸ���Ѵ�.
	GLdouble eq[4] = { Yp, Zp, Xp, 0.0 };					//�߸� ���� ������
	glClipPlane(GL_CLIP_PLANE0, eq);						//�߸� ���� ���������� �ڸ�
	glEnable(GL_CLIP_PLANE0);								//����
}
void draw(void) {
	/*ȭ���� �����ϰ� �����*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);								//MODELVIEW ���
	glLoadIdentity();										//�ʱ�ȭ
	gluLookAt(xx, yy, zz, 0, 0, 0, 0, 1, 0);				//xx,yy,zz ��ǥ���� 0,0,0 �� �ٶ�

	glPushMatrix();											//���� ��ǥ�� ����
	rotate();												//ȸ��
	glPopMatrix();											//������ ��ǥ�� �ҷ���

	glColor3f(1, 0, 0);										//���� ����
	glutSolidTeapot(4);										//�ָ��� ������

	glFlush();
	glutSwapBuffers();										//double buffering��
}
void resize(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);								//projection ���
	glLoadIdentity();											//�ʱ�ȭ
	gluPerspective(45, (float)width / (float)height, 1, 500);	//perspective ���
	glMatrixMode(GL_MODELVIEW);									//MODELVIEW ���
}
void specialkeyboard(int key, int x, int y) {				//ȭ��ǥ �Է� ���� ��
	if (key == GLUT_KEY_LEFT) {								//���� ȭ��ǥ�� ������ ��
		phi = phi - M_PI / 180;								//phi��  1���� ����
		if (phi < 0) {										//phi�� 0���� ������ 2*�������� ������
			phi += 2 * M_PI;
		}
		if (phi >= 2 * M_PI) {								//phi�� 2*���������� ũ��  2*�������� ����
			phi -= 2 * M_PI;
		}
		Xp = r*sin(theta)*cos(phi);						//������ǥ�踦 �̿��� x��ǥ ���ϴ� ����
		Yp = r*sin(theta)*sin(phi);						//������ǥ�踦 �̿��� y��ǥ ���ϴ� ����
		Zp = r*cos(theta);								//������ǥ�踦 �̿��� z��ǥ ���ϴ� ����
	}
	else if (key == GLUT_KEY_RIGHT) {						//������ ȭ��ǥ�� ������ ��
		phi = phi + M_PI / 180;								//phi�� 1���� ����
		if (phi < 0) {										//phi�� 0���� ������ 2*�������� ������
			phi += 2 * M_PI;
		}
		if (phi >= 2 * M_PI) {								//phi�� 2*���������� ũ��  2*�������� ����
			phi -= 2 * M_PI;
		}
		Xp = r*sin(theta)*cos(phi);						//������ǥ�踦 �̿��� x��ǥ ���ϴ� ����
		Yp = r*sin(theta)*sin(phi);						//������ǥ�踦 �̿��� y��ǥ ���ϴ� ����
		Zp = r*cos(theta);								//������ǥ�踦 �̿��� z��ǥ ���ϴ� ����
	}
	else if (key == GLUT_KEY_UP) {							//���� ȭ��ǥ�� ������ ��
		theta = theta - M_PI / 180;							//theta��  1���� ����
		if (theta < 0) {									//theta�� 0���� ������ 2*�������� ������
			theta += M_PI * 2;
		}
		else if (theta >= M_PI * 2) {						//theta�� 2*���������� ũ��  2*�������� ����
			theta -= M_PI * 2;
		}
		if (theta >= 0 && theta < M_PI) {					//theta�� 0~���� ���̸� ī�޶��� ������ = 1
			up = 1;
		}
		else {												//�ƴϸ� ī�޶��� ������ = -1
			up = -1;
		}
		Xp = r*sin(theta)*cos(phi);						//������ǥ�踦 �̿��� x��ǥ ���ϴ� ����
		Yp = r*sin(theta)*sin(phi);						//������ǥ�踦 �̿��� y��ǥ ���ϴ� ����
		Zp = r*cos(theta);								//������ǥ�踦 �̿��� z��ǥ ���ϴ� ����
	}
	else if (key == GLUT_KEY_DOWN) {						//�Ʒ��� ȭ��ǥ�� ������ ��
		theta = theta + M_PI / 180;							//theta�� 1���� ����
		if (theta < 0) {									//theta�� 0���� ������ 2*�������� ������
			theta += M_PI * 2;
		}
		else if (theta >= M_PI * 2) {						//theta�� 2*���������� ũ��  2*�������� ����
			theta -= M_PI * 2;
		}
		if (theta >= 0 && theta < M_PI) {					//theta�� 0~���� ���̸� ī�޶��� ������ = 1
			up = 1;
		}
		else {												//�ƴϸ� ī�޶��� ������ = -1
			up = -1;
		}
		Xp = r*sin(theta)*cos(phi);						//������ǥ�踦 �̿��� x��ǥ ���ϴ� ����
		Yp = r*sin(theta)*sin(phi);						//������ǥ�踦 �̿��� y��ǥ ���ϴ� ����
		Zp = r*cos(theta);								//������ǥ�踦 �̿��� z��ǥ ���ϴ� ����
	}
	draw();													//�ٽ� �׸�
}
void keyboard(unsigned char key, int x, int y) {
	if (key == 's') {										//s�� ������ ��
		is_rotate = is_rotate == true ? false : true;		//is_rotate ���� ���
	}
	else if (key == 'z') {									//z�� ������ ��
		xx -= 0.2;											//xx,yy,zz�� 0.2�� ����
		yy -= 0.2;
		zz -= 0.2;
	}
	else if (key == 'x') {									//x�� ������ ��
		xx += 0.2;											//xx,yy,zz�� 0.2�� ����
		yy += 0.2;
		zz += 0.2;
	}
}
int main(int argc, char** argv) {
	/*window �ʱ�ȭ */

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("My Sixth GL Program");

	init();		//->����� �ʱ�ȭ �Լ�

				//callback �Լ� ����
	glutDisplayFunc(draw);									//�׸��� �׸�
	glutReshapeFunc(resize);								//reshape �Լ� ����
	glutIdleFunc(idle);										//idle �Լ�����
	glutSpecialFunc(specialkeyboard);						//����Ű�� ������ ��
	glutKeyboardFunc(keyboard);								//Ű�����Է�

	//Looping ����
	glutMainLoop();

	return 0;
}