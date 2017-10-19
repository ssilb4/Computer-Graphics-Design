#include <gl/glut.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

void init(void);					//����� �ʱ�ȭ �Լ�
void idle(void);					//��� ����Ǵ� �Լ�
void clear(void);					//ȭ�� ����� �Լ�
void draw(void);					//�׸� �׸��� �Լ�
void draw_point(void);				//���� ��� �Լ�
void draw_line(void);				//���� �׸��� �Լ�
void draw_string(void* font, const char* str, int x, int y);	//���� ����Լ�
void draw_string();												//���� ����Լ� ȣ���Լ�
void change_color();											//�� ��ȭ �Լ�

void sub_menu_function(int option);								//���� �޴� �۵��Լ�
void main_menu_function(int option);							//���� �޴� �۵��Լ�
void mouse(int button, int state, int x, int y);				//���콺 �Է��Լ�
void keyboard(unsigned char key, int x, int y);					//Ű���� �Է��Լ�

bool antialiase_on = false;										//antialise Ȯ��
bool string_on = false;											//string ��� ����
bool pivot_on = false;											//pivot ��ġ
bool trans_on = false;											//�̵� ����
bool rotate_on = false;											//ȸ�� ����
int shape = GL_POINTS;											//�⺻ ���(point�� �ʱ�ȭ)
float r = 0.0f, g = 0.0f, b = 0.0f, alpha = 1.0f;				//���� r,g,b,alpha �� (����Ʈ ������)
int xx[10000] = { 0, }, yy[10000] = { 0, };						//�� ������ x,y ��ǥ
float cc[10000] = { 0.0f, };									//�� ������ ��
int count = 0;													//���� ����
double spin = 0, trans = 0;										//ȸ������ �̵���
double cx=0, cy=0;												//ȸ���� ��ǥ

void init(void) {
	/*ȭ���� �⺻������ white ����*/
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	/*ȭ�� ��ǥ ���� ����*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0f, 500.0f, 0.0f, 500.0f);
}

void idle(void) {
	if (rotate_on == true) {									//rotate�� on�� ��
		spin = spin + 0.1;										//spin�� 0.1�� ����
		if (spin > 360) spin -= 360;							//spin�� 360�� ������ -360�� ����
	}
	if (trans_on == true) {										//trans�� on�� ��
		trans = trans + 0.1;									//trans�� 0.1Tlr ����
		if (trans > 500) trans = -500;							//trans�� 500�� ������ trans -500�� ����
	}
	glutPostRedisplay();										//��� �ٽ� ������
}


void clear(void) {
	/*ȭ���� �����ϰ� �����*/
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

void draw(void) {
	clear();														//ȭ�� ����� ����
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();												//��� �ʱ�ȭ
	if (rotate_on == true) {										//rotation�� on�� ��
		glTranslatef(cx, cy, 0);									//ȸ�� ������ ���� �̵�
		glRotatef(spin, 0, 0, 1);									//ȸ��
		glTranslatef(-cx, -cy, 0);									//���� �ִ� ������ �����̵�
	}
	if (trans_on == true) {											//rotation�� on�� ��
		glTranslatef(trans, 0, 0);									//ȸ�� ������ ���� �̵�z
	}
	glPointSize(10.0f);												//��Ʈ ũ�� 10
	glBegin(shape);													//������ ��� ������
	for (int i = 0; i < count; i++) {								//���� ������ŭ ��½���
		float tr = 0.0f, tb = 0.0f, tg = 0.0f;						//�ӽ� �� ���庯��
		if (shape == GL_TRIANGLES || shape == GL_QUADS) {
			if ((int)cc[i] % 10 == 3) {								//���� r�� ���
				tr = 1.0f;											//�ӽ� ���� red
				glColor3f(tr, tg, tb);
			}
			else if ((int)cc[i] % 10 == 2) {						//���� g�� ���
				tg = 1.0f;											//�ӽ� ���� green
				glColor3f(tr, tg, tb);
			}
			else if ((int)cc[i] % 10 == 1) {						//���� b�� ���
				tb = 1.0f;											//�ӽ� ���� blue
				glColor3f(tr, tg, tb);
			}
			else {													//�⺻ ��
				glColor3f(tr, tg, tb);
			}
			if (antialiase_on == true) {							//antialiase�� on�� ���
				if ((int)cc[i] / 10 == 0) {							//alpha ���� 0�� ��
					alpha = 0.0f;
				}
				else if ((int)cc[i] / 10 == 25) {					//alpha ���� 0.25�� ��
					alpha = 0.25f;
				}
				else if ((int)cc[i] / 10 == 50) {					//alpha ���� 0.5�� ��
					alpha = 0.5f;
				}
				else if ((int)cc[i] / 10 == 75) {					//alpha ���� 0.75�� ��
					alpha = 0.75f;
				}
				else if ((int)cc[i] / 10 == 100) {					//alpha ���� 1 �� ��
					alpha = 1.0f;
				}
				glColor4f(tr, tg, tb, alpha);						//���� alpha�� �����ؼ� ����
			}
		}
		else {
			glColor3f(r, g, b);										//alpha�� �����ؼ� ����
		}
		glVertex2i(xx[i], yy[i]);									//�� ���� x,y ��ǥ�� ����
	}
	glEnd();
	glFlush();
	if (string_on == true) {									//string�� on�� ��
		draw_string();											//string�� ��
	}
	glutSwapBuffers();											//�� ���� ���۸� ������ �ٲ�
}

void draw_string(void* font, const char* str, int x, int y) {	//string �׸���
	unsigned int i;
	glRasterPos2i(x, y);
	for (i = 0; i < strlen(str); i++) {
		glutBitmapCharacter(font, str[i]);
	}
}

void draw_string(void) {										//��Ȳ �� �׸� string
																//����
	glColor3f(0.0f, 0.0f, 0.0f);								//���� ����
	if (antialiase_on == true) {								//antialiase �� on �� ��
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Antialiase: ON", 10, 10);	//Antialiase: ON ���
	}
	else {
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Antialiase: OFF", 10, 10);//Antialiase: OFF ���
	}
	draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "12121689 Lee Him Chan", 250, 480); //�̸� �й� ���
	glFlush();
}

void change_color() {											//�� �����ִ� �Լ�
	int temp = count;											//����� �� ��������� ��, ���� ���� �ٲٱ� ���� ����
	if (shape == GL_TRIANGLES) {								//�ﰢ���� ��
		temp = temp % 3;										//�ﰢ���� �� ��������� ���� 1,2�� ������ ���� ���ؼ�
		for (int i = count - 3 - temp; i < count; i++) {		//�� 3~5���� ���� �Ȱ��� ��
			cc[i] = r * 3 + g * 2 + b * 1 + alpha * 1000;			//r,g,b,alpha�� �� ���� ����
		}
		for (int i = 0; i < count - 3 - temp; i = i + 3) {		//ó������ �������� 3���� ��� ���� ���� ��
			cc[i + 1] = cc[i];
			cc[i + 2] = cc[i];
		}
	}
	else if (shape == GL_QUADS) {								//quad �� ��
		temp = temp % 4;										//quad�� �� ��������� ��, ���� ���� �ٲٱ� ���� ����
		for (int i = count - 4 - temp; i < count; i++) {		//�� 4~7���� ���� �Ȱ�����
			cc[i] = r * 3 + g * 2 + b * 1 + alpha * 1000;		//r,g,b,alpha�� �� ���� ����
		}
		for (int i = 0; i < count - 4 - temp; i = i + 4) {		//ó������ �������� 4���� ��� ���� ���� ��
			cc[i + 1] = cc[i];
			cc[i + 2] = cc[i];
			cc[i + 3] = cc[i];
		}
	}
	draw();														//����� ���� �׸�
}


void sub_menu_function(int option) {							//���� �޴�
	printf("Submenu %d has been slected\n", option);
	if (option == 21) {											//line�� ����
		shape = GL_LINES;										//shape�� line���� �ٲ�
		draw();													//�׸�
	}
	else if (option == 22) {									//line_strip�� ����
		shape = GL_LINE_STRIP;									//shape�� line_strip���� �ٲ�
		draw();													//�׸�
	}
	else if (option == 23) {									//line_loop�� ����
		shape = GL_LINE_LOOP;									//shape�� line_loop�� �ٲ�
		draw();													//�׸�
	}
	else if (option == 31) {									//triangle�� ����
		shape = GL_TRIANGLES;									//shape�� triangle�� �ٲ�
		change_color();											//�� ����
		draw();													//�׸�
	}
	else if (option == 32) {									//triangle_strip�� ����
		shape = GL_TRIANGLE_STRIP;								//shape�� triangle_strip�� �ٲ�												
		change_color();											//�� ����
		draw();													//�׸�
	}
	else if (option == 33) {									//quad�� ����
		shape = GL_QUADS;										//shape�� quad�� �ٲ�													
		change_color();											//�� ����
		draw();													//�׸�
	}
	else if (option == 34) {									//POLYGON�� ����
		shape = GL_POLYGON;										//shape�� POLYGON�� �ٲ�												
		change_color();											//�� ����
		draw();													//�׸�
	}
}

void main_menu_function(int option) {							//���� �޴�
	printf("Main menu %d has been selected\n", option);
	if (option == 1) {											//����Ʈ ����
		shape = GL_POINTS;										//shpae�� point�� �ٲ�
		draw();													//�׸�
	}
	else {
		clear();												//clear ����
		count = 0;												//���ݱ��� ���� �� ����
		draw();													//string�� �ʿ��ϸ� �׸�
	}
}

void mouse(int button, int state, int x, int y) {				//���콺 �Է� �Լ�
	if (pivot_on == false) {										//pivot_on �� �������� ��
		if (button == 0 && state == 1) {							//���� ��ư�� ������ ��
			printf("mouse %d %d \n", x, y);
			xx[count] = x;											//x��ǥ ���
			yy[count] = 500 - y;									//y��ǥ ���
			cc[count] = r * 3 + g * 2 + b * 1 + alpha * 1000;		//�� ���
			count++;												//���� ���� ����
			draw();													//�׸�
		}
	}
	else if(pivot_on == true){										//pivot_on�� �������� ��
		cx = x;														//ȸ���� ����
		cy = 500-y;
		draw();														//�׸�
	}
}

void keyboard(unsigned char key, int x, int y) {
		if (key == 49) {											//1�� ������ ��
			r = 1.0f;												//���������� �ٲ�
			g = 0.0f;
			b = 0.0f;
			printf("red\n");
			if (shape == GL_TRIANGLES || shape == GL_QUADS) {		//triangle�̳� quad�� ��� �� ����
				cc[count] = r * 3 + g * 2 + b * 1 + alpha * 1000;
			}
			change_color();											//�� ����
		}
		else if (key == 50) {										//2�� ������ ��
			r = 0.0f;												//�ʷϻ����� �ٲ�
			g = 1.0f;
			b = 0.0f;
			printf("green\n");
			if (shape == GL_TRIANGLES || shape == GL_QUADS) {		//triangle�̳� quad�� ��� �� ����
				cc[count] = r * 3 + g * 2 + b * 1 + alpha * 1000;
			}
			change_color();											//�� ����
		}
		else if (key == 51) {										//3�� ������ ��
			r = 0.0f;												//�Ķ������� �ٲ�	
			g = 0.0f;
			b = 1.0f;
			printf("blue\n");
			if (shape == GL_TRIANGLES || shape == GL_QUADS) {		//triangle�̳� quad�� ��� �� ����
				cc[count] = r * 3 + g * 2 + b * 1 + alpha * 1000;
			}
			change_color();											//�� ����
		}
		else if (key == 97) {										//a�� ������ ��
			if (antialiase_on == false) {							//antialiase �� off �� ��
				glEnable(GL_BLEND);									//antialiase ������ �͵��� �Ѱ�
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_POINT_SMOOTH);
				glEnable(GL_LINE_SMOOTH);
				glEnable(GL_POLYGON_SMOOTH);
				glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
				glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
				glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
				antialiase_on = true;								//antialiase �� on
				draw();												//�׸�
			}
			else {													//antialiase�� on �� ��
				glDisable(GL_BLEND);								//antialiase ������ �͵��� ����
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glDisable(GL_POINT_SMOOTH);
				glDisable(GL_LINE_SMOOTH);
				glDisable(GL_POLYGON_SMOOTH);
				glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
				glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
				glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
				antialiase_on = false;								//antialiase�� off
				draw();												//�׸�
			}
		}
		else if (key == 99) {										//c�� ������ ��
			if (string_on == false) {								//string �� off �� ��
				draw_string();										//string �� �׸���
				string_on = true;									//string �� on ��
			}
			else {													//string �� on �� ��
				string_on = false;									//string �� off ��
				draw();												//�׸�
			}
		}
		else if (key == 121) {										//y�� ������ ��
			alpha = 0.0f;											//alpha �� 0
			change_color();											//�� ����
		}
		else if (key == 117) {										//u�� ������ ��
			alpha = 0.25f;											//alpha �� 0.25
			change_color();											//�� ����
		}
		else if (key == 105) {										//i�� ������ ��
			alpha = 0.5f;											//alpha �� 0.5
			change_color();											//�� ����
		}
		else if (key == 111) {										//o�� ������ ��
			alpha = 0.75f;											//alpha �� 0.75
			change_color();											//�� ����
		}
		else if (key == 112) {										//p�� ������ ��
			alpha = 1.0f;											//alpha �� 1.0
			change_color();											//�� ����
		}
		else if (key == 114) {										//r�� ������ ��
			if (rotate_on == false) {								//rotate �� off �� ��
				draw();												//�׸�
				rotate_on = true;									//rotate �� on ��
			}
			else {													//rotate �� on �� ��
				rotate_on = false;									//rotate �� off ��
				draw();												//�׸�
			}
		}
		else if (key == 116) {										//t�� ������ ��
			if (trans_on == false) {								//trans �� off �� ��
				draw();												//�׸�
				trans_on = true;									//trans �� on ��
			}
			else {													//trans �� on �� ��
				trans_on = false;									//trans �� off ��
				draw();												//�׸�
			}
		}
		else if (key == 118) {										//v�� ������ ��
			if (pivot_on == false) {								//pivot �� off �� ��
				draw();												//�׸�
				pivot_on = true;									//pivot �� on ��
			}
			else {													//pivot �� on �� ��
				pivot_on = false;									//pivot �� off ��
				draw();												//�׸�
			}
		}
}

int main(int argc, char** argv) {
	int submenu1 = 0, submenu2 = 0;					//���� �޴�
													/*window �ʱ�ȭ */

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("My Thrid GL Program");

	init();		//->����� �ʱ�ȭ �Լ�

				//Popup menu ���� �� �߰�
	submenu1 = glutCreateMenu(sub_menu_function);			//3���� �׸��� �ִ� ����޴� ����
	glutAddMenuEntry("GL_LINES", 21);						//line �� 21
	glutAddMenuEntry("GL_LINE_STRIP", 22);					//line_strip�� 22
	glutAddMenuEntry("GL_LINE_LOOP", 23);					//line_loop�� 23

	submenu2 = glutCreateMenu(sub_menu_function);			//4���� �׸��� �ִ� ����޴� ����
	glutAddMenuEntry("GL_TRIANGLES", 31);					//triangle�� 31
	glutAddMenuEntry("GL_TRIANFGLE_STRIP", 32);				//triangle_strip�� 32
	glutAddMenuEntry("GL_QUADS", 33);						//quad�� 33
	glutAddMenuEntry("GL_POLYGON", 34);						//polygon�� 34

	glutCreateMenu(main_menu_function);						//4���� �׸��� �ִ� �޴� ����
	glutAddMenuEntry("Point Draw", 1);						//point �� 1
	glutAddSubMenu("Line Draw", submenu1);					//line draw�� ����޴�1
	glutAddSubMenu("Polygon Draw", submenu2);				//polygon draw�� ����޴�2
	glutAddMenuEntry("Clear", 0);							//ȭ�������� 0
	glutAttachMenu(GLUT_RIGHT_BUTTON);						//�޴��� �����ʹ�ư

	glutKeyboardFunc(keyboard);								//Ű�����Է�
	glutMouseFunc(mouse);									//���콺�Է�

															//callback �Լ� ����
	glutDisplayFunc(draw);									//�׸�
	glutIdleFunc(idle);

															//Looping ����
	glutMainLoop();


	return 0;
}

