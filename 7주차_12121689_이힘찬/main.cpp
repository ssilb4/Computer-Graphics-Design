#include <gl/glut.h>
#include <math.h>
#include <stdio.h>

#define M_PI 3.14159265358979323846					//������

void init(void);									//����� ���� �Լ�
void draw(void);									//�׸� �׸��� �Լ�
void resize(int width, int height);					//â ũ�� �ٲ��� �� ����Ǵ� �Լ�
void draw_axis(void);								//���� �׸��� �Լ�
void specialkeyboard(int key, int x, int y);		//����Ű ������ �� ����Ǵ� �Լ�

double eyeX = 20, eyeY = 0, eyeZ = 0;				//����� �������� x,y,z ��ǥ
int up = 1;											//ī�޶��� ���� ��Ÿ���� ����

const double r = sqrt(402);							//r�� ������
double theta = acos(eyeZ / r);						//theta�� ���ϴ� ����
double phi = atan(eyeY / eyeX);						//phi�� ���ϴ� ����

void init(void) {
	/*ȭ���� �⺻������ black ����*/
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	/*ȭ�� ��ǥ ���� ����*/
	gluOrtho2D(0.0f, 500.0f, 0.0f, 500.0f);
}
void draw(void) {									//�׸��� �Լ�
	/*ȭ���� �����ϰ� �����*/
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyeY, eyeZ, eyeX, 0, 0, 0, 0, up, 0);		//eyeX,eyeY,eyeZ�� ����� ���⿡ ��ǥ�� ������ ������ opengl������ x,y,z ����

	glColor3f(1, 1, 0);									//�����
	glutWireTeapot(4);									//������
	draw_axis();										//x,y,z �� �׸���

	glFlush();
	glutSwapBuffers();									//double buffering��
}
void resize(int width, int height) {					//â ũ�� �ٲ��� ��
	glViewport(0, 0, width, height);					//���̶� �ʺ� �ٲ��� ��
	glMatrixMode(GL_PROJECTION);						//projection matrix Ȱ��ȭ
	glLoadIdentity();
	gluPerspective(45, (float)width / (float)height, 1, 500);
	glMatrixMode(GL_MODELVIEW);							//MODELVIEW�� ȯ��
}
void draw_axis(void) {						//�� �׸���
	glLineWidth(3);							//��ǥ���� �β�
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);						//opengl ���� x���� red
	glVertex3f(0, 0, 0);
	glVertex3f(4, 0, 0);

	glColor3f(0, 1, 0);						//opengl ���� y���� green
	glVertex3f(0, 0, 0);
	glVertex3f(0, 4, 0);

	glColor3f(0, 0, 1);						//opengl ���� z���� blue
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 4);

	glEnd();
	glLineWidth(1);							//�� ���� 1
}
void specialkeyboard(int key, int x, int y) {				//ȭ��ǥ �Է� ���� ��
	if (key == GLUT_KEY_LEFT) {								//���� ȭ��ǥ�� ������ ��
		phi = phi - M_PI/180;								//phi��  1���� ����
		if (phi < 0) {										//phi�� 0���� ������ 2*�������� ������
			phi += 2 * M_PI;
		}
		if (phi >= 2 * M_PI) {								//phi�� 2*���������� ũ��  2*�������� ����
			phi -= 2 * M_PI;
		}
		eyeX = r*sin(theta)*cos(phi);						//������ǥ�踦 �̿��� x��ǥ ���ϴ� ����
		eyeY = r*sin(theta)*sin(phi);						//������ǥ�踦 �̿��� y��ǥ ���ϴ� ����
		eyeZ = r*cos(theta);								//������ǥ�踦 �̿��� z��ǥ ���ϴ� ����
	}
	else if (key == GLUT_KEY_RIGHT) {						//������ ȭ��ǥ�� ������ ��
		phi = phi + M_PI / 180;								//phi�� 1���� ����
		if (phi < 0) {										//phi�� 0���� ������ 2*�������� ������
			phi += 2*M_PI;
		}
		if (phi >= 2 * M_PI) {								//phi�� 2*���������� ũ��  2*�������� ����
			phi -= 2 * M_PI;
		}
		eyeX = r*sin(theta)*cos(phi);						//������ǥ�踦 �̿��� x��ǥ ���ϴ� ����
		eyeY = r*sin(theta)*sin(phi);						//������ǥ�踦 �̿��� y��ǥ ���ϴ� ����
		eyeZ = r*cos(theta);								//������ǥ�踦 �̿��� z��ǥ ���ϴ� ����
	}
	else if (key == GLUT_KEY_UP) {							//���� ȭ��ǥ�� ������ ��
		theta = theta - M_PI / 180;							//theta��  1���� ����
		if (theta < 0) {									//theta�� 0���� ������ 2*�������� ������
			theta += M_PI*2;
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
		eyeX = r*sin(theta)*cos(phi);						//������ǥ�踦 �̿��� x��ǥ ���ϴ� ����
		eyeY = r*sin(theta)*sin(phi);						//������ǥ�踦 �̿��� y��ǥ ���ϴ� ����
		eyeZ = r*cos(theta);								//������ǥ�踦 �̿��� z��ǥ ���ϴ� ����
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
		eyeX = r*sin(theta)*cos(phi);						//������ǥ�踦 �̿��� x��ǥ ���ϴ� ����
		eyeY = r*sin(theta)*sin(phi);						//������ǥ�踦 �̿��� y��ǥ ���ϴ� ����
		eyeZ = r*cos(theta);								//������ǥ�踦 �̿��� z��ǥ ���ϴ� ����
	}
	draw();													//�ٽ� �׸�
}
int main(int argc, char** argv) {
	/*window �ʱ�ȭ */

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);							//â ũ��
	glutInitWindowPosition(300, 300);						//��ġ
	glutCreateWindow("My Forth GL Program");

	init();		//->����� �ʱ�ȭ �Լ�

				//callback �Լ� ����
	glutDisplayFunc(draw);									//�׸���
	glutReshapeFunc(resize);								//âũ�Ⱑ �ٲ� ��
	glutSpecialFunc(specialkeyboard);						//����Ű�� ������ ��

	//Looping ����	
	glutMainLoop();											//�ݺ�

	return 0;
}