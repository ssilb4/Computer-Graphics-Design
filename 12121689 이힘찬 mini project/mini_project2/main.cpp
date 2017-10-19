#pragma comment(lib,"Winmm.lib") // 배경음악
#pragma comment(lib, "legacy_stdio_definitions.lib")
#include <stdio.h>
#include <gl/glut.h>
#include <string.h>
#include <math.h>
#include <glaux.h>
#include <windows.h>
#include <mmsystem.h>
#include <fstream>
#include <sstream>
#include "Mmsystem.h"
#include "Digitalv.h"

//사용될 함수들
void init(void);									//사용자 정의 초기화 함수
void draw(void);									//그리는 함수
void resize(int width, int height);					//크기 조절 함수
void idle(void);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void draw_string(void* font, const char* str, float x_position, float y_position, float red, float green, float blue);
void draw_text(void);
void draw_lego(int block_name, int color_name, int x_count, int z_count);
void make_square(float x1, float y1, float z1, float x2, float y2, float z2);
void make_cube(float x1, float y1, float z1, float x2, float y2, float z2);
void make_brick(int x_count, int z_count);
void make_plate(int x_count, int z_count);
void make_wheel();
void make_body();
void make_s_brick();
void make_s_plate();
void file_read();
void file_write();

#define M_PI 3.14159265358979323846					//원주율
#define MAX 100

int draw_count = 0;

bool is_transparent = false;							//투명여부
bool is_spin = false;									//전체 회전 여부
bool is_rotate = true;									//회전여부
bool draw_count_sel = true;								//선택모드일 때 draw_count
bool is_bgm = true;										//배경음악
bool is_sound = true;									//효과음
float color[9][4] = { { 1,0,0,1 },{ 0,1,0,1 },{ 0,0,1,1 },{ 1,1,0,1 },{ 1,0,1,1 },{ 0,1,1,1 },{ 0,0,0,1 },{ 1,1,1,1 },{ 1,1,1,0.1 } };
const int red = 0, green = 1, blue = 2, m = 3, yellow = 4, cy = 5, black = 6, white = 7, transparent = 8;
const int brick = 0, plate = 1, wheel = 2, body = 3, s_brick = 4, s_plate = 5;
int x_count[MAX] = { 0, }, z_count[MAX] = { 0, };						//블록의 크기
const int selection = 0, move = 1, rotate = 2;			//선택 이동 회전 모드

double x_view = 20, y_view = 0, z_view = 0;
int up = 1;							//카메라의 위아래

double r = sqrt(400);									//r의 초기값
double theta = acos(z_view / r);							//theta값 구하는 공식
double phi = atan(y_view / x_view);						//phi값 구하는 공식

int mouse_state = -1;
int beforeX = -1, beforeY = -1;
double tempX = 0, tempY = 0;
//모드용 변수
int mode = 0;										//현재 모드 상태
int mode_index = 0;
char* select_mode[3] = { "selection","translate","rotate" };
//선택모드일 때 사용되는 변수
int select_color[MAX] = { 0, };								//현재 색
int select_block[MAX] = { 0, };								//현재 종류
																	//이동모드일 떄 사용되는 변수
double x_move[MAX] = { 0, }, y_move[MAX] = { 0, }, z_move[MAX] = { 0, };
//회전모드일 떄 사용되는 변수
int x_rot[MAX] = { 0, }, y_rot[MAX] = { 0, }, z_rot[MAX] = { 0, };

GLuint texName[2];

GLUquadricObj* m_pQuadric1 = gluNewQuadric();
double spin_angle = 0;
DWORD m_dwDeviceID;
MCI_OPEN_PARMS mciOpen;
MCI_PLAY_PARMS mciPlay;

int dwID;

void beep(void) {				//비프음
	printf("\a");
}
void idle(void) {
	//배경음악
	if (is_bgm == true) {
		mciOpen.lpstrElementName = "01.mp3"; // 파일 경로 입력
		mciOpen.lpstrDeviceType = "mpegvideo";

		mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE,
			(DWORD)(LPVOID)&mciOpen);

		dwID = mciOpen.wDeviceID;
			mciSendCommand(dwID, MCI_PLAY, MCI_DGV_PLAY_REPEAT, // play & repeat
				(DWORD)(LPVOID)&mciPlay);
	}
	if (is_bgm == false) {
		mciSendCommand(dwID, MCI_CLOSE, 0, NULL);
	}
	if (is_spin == true) {
		spin_angle += 0.1;
		if (spin_angle >= 360) spin_angle -= 360;
	}
	glutPostRedisplay();
}
void init(void) {

	glGenTextures(2, texName);
	AUX_RGBImageRec* pRGBImage;

	//plastic.bmp
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	pRGBImage = auxDIBImageLoadA("plastic2.bmp");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0,
		GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//dart.bmp
	glBindTexture(GL_TEXTURE_2D, texName[1]);
	pRGBImage = auxDIBImageLoadA("circle.BMP");
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pRGBImage->sizeX, pRGBImage->sizeY, 0,
		GL_RGB, GL_UNSIGNED_BYTE, pRGBImage->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//0번 조명 관련 설정
	GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);


	//조명 스위치와 0번 조명 스위치 켜기
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	
	//재질의 반사 특성 설정
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
														/*화면의 기본색으로 black 설정*/
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	/*화면 좌표 정보 설정*/
	gluOrtho2D(0.0f, 500.0f, 0.0f, 500.0f);

}
void draw(void) {
	/*화면을 깨끗하게 지우기*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	gluLookAt(y_view, z_view, x_view, 0, 0, 0, 0, up, 0);
	glViewport(0, 300, 400, 300);
	draw_text();
	if (x_count[draw_count] == 0)
		x_count[draw_count] = 1;
	if (z_count[draw_count] == 0)
		z_count[draw_count] = 1;

	glPushMatrix();
	glViewport(0, 0, 400, 300);
	draw_lego(select_block[draw_count], select_color[draw_count], x_count[draw_count], z_count[draw_count]);
	glFlush();
	glEnd();
	glPopMatrix();

	glViewport(400, 0, 800, 600);

	glBindTexture(GL_TEXTURE_2D, texName[1]);
	glBegin(GL_QUADS);
	glColor3f(1, 1, 1);
	make_square(-15, 0, -15, 15, 0, 15);
	glEnd();
	glFlush();

	glPushMatrix();
	for (int i = 0; i <= draw_count; i++) {
		if (draw_count_sel == true && i == draw_count) {
			break;
		}
		glPushMatrix();
		glRotatef(spin_angle, 0, 1, 0);
		glTranslatef(x_move[i], y_move[i], z_move[i]);
		glRotatef(x_rot[i], 1, 0, 0);
		glRotatef(y_rot[i], 0, 1, 0);
		glRotatef(z_rot[i], 0, 0, 1);
		draw_lego(select_block[i], select_color[i], x_count[i], z_count[i]);
		glEnd();
		glFlush();
		glPopMatrix();
	}
	glPopMatrix();

	glFlush();
	glutSwapBuffers();
}
void keyboard(unsigned char key, int x, int y) {
	if (is_sound == true) {
		beep();
	}
	if (key == 'a') {
		if (draw_count_sel == false) {
			draw_count_sel = true;
			draw_count++;
		}
		mode = selection;
		mode_index = 0;
	}
	else if (key == 's' && (draw_count > 0 || draw_count_sel == false)) {
		if (draw_count_sel == true) {
			draw_count_sel = false;
			draw_count--;
		}
		mode = move;
		mode_index = 1;
	}
	else if (key == 'd' && (draw_count > 0 || draw_count_sel == false)) {
		if (draw_count_sel == true) {
			draw_count_sel = false;
			draw_count--;
		}
		mode = rotate;
		mode_index = 2;
	}
	else if (key == 'h') {
		if (mode == selection) {
			x_count[draw_count]++;
		}
		else if (mode == move) {
			x_move[draw_count] -= 0.1;
		}
		else if (mode == rotate) {
			x_rot[draw_count]++;
			if (x_rot[draw_count] >= 360) {
				x_rot[draw_count] -= 360;
			}
		}
	}
	else if (key == 'j') {
		if (mode == selection) {
			z_count[draw_count]++;
		}
		else if (mode == move) {
			z_move[draw_count] += 0.1;
		}
		else if (mode == rotate) {
			z_rot[draw_count]++;
			if (z_rot[draw_count] >= 360) {
				z_rot[draw_count] -= 360;
			}
		}
	}
	else if (key == 'k') {
		if (mode == selection) {
			if (z_count[draw_count]>1)
				z_count[draw_count]--;
		}
		else if (mode == move) {
			z_move[draw_count] -= 0.1;
		}
		else if (mode == rotate) {
			z_rot[draw_count]--;
			if (z_rot[draw_count] < 0) {
				z_rot[draw_count] += 360;
			}
		}
	}
	else if (key == 'l') {
		if (mode == selection) {
			if (x_count[draw_count]>1)
				x_count[draw_count]--;
		}
		else if (mode == move) {
			x_move[draw_count] += 0.1;
		}
		else if (mode == rotate) {
			x_rot[draw_count]--;
			if (x_rot[draw_count] < 0) {
				x_rot[draw_count] += 360;
			}
		}
	}
	else if (key == 'u') {
		if (mode == selection) {
			select_block[draw_count]++;
			if (select_block[draw_count] > 5) {
				select_block[draw_count] -= 6;
			}
		}
		else if (mode == move) {
			y_move[draw_count] += 0.1;
		}
		else if (mode == rotate) {
			y_rot[draw_count]++;
			if (y_rot[draw_count] >= 360) {
				y_rot[draw_count] -= 360;
			}
		}
	}
	else if (key == 'm') {
		if (mode == selection) {
			select_color[draw_count]++;
			if (select_color[draw_count] > 8) {
				select_color[draw_count] -= 9;
			}
		}
		else if (mode == move) {
			y_move[draw_count] -= 0.1;
		}
		else if (mode == rotate) {
			y_rot[draw_count]--;
			if (y_rot[draw_count] < 0) {
				y_rot[draw_count] += 360;
			}
		}
	}
	else if (key == 'f') {
		draw_count++;
	}
	else if (key == 'z') {
		if (draw_count > 0) {
			draw_count--;
		}
	}
	else if (key == '1') {
		is_sound = is_sound == true ? false : true;
	}
	else if (key == '2') {
		is_bgm = is_bgm == true ? false : true;
	}
	else if (key == '3') {
		is_spin = is_spin == true ? false : true;
	}
	else if (key == 'o') {
		file_read();
	}
	else if (key == 'p') {
		file_write();
	}								//전체 회전 여부
	glutPostRedisplay();										//계속 다시 보여줌
}
void resize(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)width / (float)height, 1, 500);
	glMatrixMode(GL_MODELVIEW);
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
	if (mouse_state == 0) {							//왼쪽 버튼을 눌렀을 때
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
			x_view = r*sin(theta)*cos(phi);						//구면좌표계를 이용한 x좌표 구하는 공식
			y_view = r*sin(theta)*sin(phi);						//구면좌표계를 이용한 y좌표 구하는 공식
			z_view = r*cos(theta);								//구면좌표계를 이용한 z좌표 구하는 공식
		}
		beforeX = x;
		beforeY = y;
	}
	else if (mouse_state == 1) {							//오른쪽 버튼을 눌렀을 때
		tempX = beforeX - x;
		tempY = beforeY - y;
		if (tempX <30 && tempX >-30 && tempY <30 && tempY>-30) {
			r = r + M_PI / 180 * (tempX);
			if (r<0)
				r = 0.01;
			x_view = r*sin(theta)*cos(phi);						//구면좌표계를 이용한 x좌표 구하는 공식
			y_view = r*sin(theta)*sin(phi);						//구면좌표계를 이용한 y좌표 구하는 공식
			z_view = r*cos(theta);								//구면좌표계를 이용한 z좌표 구하는 공식
		}
		beforeX = x;
		beforeY = y;
	}
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
void draw_text(void) {
	glPushMatrix();
	glLoadIdentity();			//기존의 회전변환을 무시
	draw_string(GLUT_BITMAP_TIMES_ROMAN_24, select_mode[mode_index], -5, 4., 1, 0, 0);		//폰트, 내용, 위치, 색 지정

	glFlush();

	glPopMatrix();
}
void draw_lego(int block_name, int color_name, int x_count, int z_count) {
	glColor3f(color[color_name][0], color[color_name][1], color[color_name][2]);
	glColor4f(color[color_name][0], color[color_name][1], color[color_name][2], color[color_name][3]);
	if (block_name == brick) {
		make_brick(x_count, z_count);
	}
	else if (block_name == plate) {
		make_plate(x_count, z_count);
	}
	else if (block_name == wheel) {
		make_wheel();
	}
	else if (block_name == body) {
		make_body();
	}
	else if (block_name == s_brick) {
		make_s_brick();
	}
	else if (block_name == s_plate) {
		make_s_plate();
	}
	//glDisable(GL_BLEND);
}
void make_cube(float x1, float y1, float z1, float x2, float y2, float z2) {
	//back
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	glBegin(GL_QUADS);
	make_square(x1, y1, z1, x1, y2, z2);
	glEnd();
	//front
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	glBegin(GL_QUADS);
	make_square(x1, y1, z1, x2, y2, z1);
	glEnd();
	//left
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	glBegin(GL_QUADS);
	make_square(x1, y1, z1, x2, y1, z2);
	glEnd();
	//right
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	glBegin(GL_QUADS);
	make_square(x2, y2, z2, x1, y1, z2);
	glEnd();
	//top
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	glBegin(GL_QUADS);
	make_square(x2, y2, z2, x1, y2, z1);
	glEnd();
	//bottom
	glBindTexture(GL_TEXTURE_2D, texName[0]);
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
	glEnd();
	glFlush();
}
void make_brick(int x_count, int z_count) {
	glPushMatrix();
	for (int i = 0; i < x_count; i++) {
		glPushMatrix();
		glTranslatef(0.8*i, 0, 0);
		for (int j = 0; j < z_count; j++) {
			glPushMatrix();
			glTranslatef(0, 0, 0.8*j);

			make_cube(0.0f, 0.0f, 0.0f, 0.8f, 0.8f, 0.8f);
			glEnd();
			glTranslatef(0.4f, 1.0f, 0.4f);
			glRotatef(90, 1, 0, 0);
			glBindTexture(GL_TEXTURE_2D, texName[0]);
			gluCylinder(m_pQuadric1, 0.3, 0.3, 0.2, 10, 10);
			glBindTexture(GL_TEXTURE_2D, texName[0]);
			gluDisk(m_pQuadric1, 0, 0.3, 10, 10);

			glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();
}
void make_plate(int x_count, int z_count) {
	glPushMatrix();
	for (int i = 0; i < x_count; i++) {
		glPushMatrix();
		glTranslatef(0.8*i, 0, 0);
		for (int j = 0; j < z_count; j++) {
			glPushMatrix();
			glTranslatef(0, 0, 0.8*j);

			glBegin(GL_QUADS);
			make_cube(0.0f, 0.0f, 0.0f, 0.8f, 0.3f, 0.8f);
			glEnd();
			glTranslatef(0.4f, 0.5f, 0.4f);
			glRotatef(90, 1, 0, 0);
			gluCylinder(m_pQuadric1, 0.3, 0.3, 0.2, 10, 10);
			gluDisk(m_pQuadric1, 0, 0.3, 10, 10);
			glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();
}
void make_wheel() {
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	gluCylinder(m_pQuadric1, 0.5, 0.5, 0.3, 10, 10);
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	gluDisk(m_pQuadric1, 0, 0.5, 10, 10);
	glTranslatef(0.0f, 0.0f, 0.3f);
	glColor3f(0.2, 0.2, 0.2);
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	gluDisk(m_pQuadric1, 0.05, 0.5, 10, 10);
	glPopMatrix();
}
void make_body() {
	glPushMatrix();
	glBegin(GL_QUADS);
	//상체 몸
	make_cube(0.0f, 1.6f, 0.0f, 0.8f, 3.2f, 1.6f);
	//상체 팔
	glColor3f(0.5f, 0.5f, 0.5f);
	make_cube(0.2f, 1.8f, -0.1f, 0.6f, 3.1f, 0.0f);
	make_cube(0.2f, 1.8f, 1.6f, 0.6f, 3.1f, 1.7f);
	//하체
	glColor3f(0.5f, 0.5f, 0.5f);
	make_cube(0.0f, 0.0f, 0.0f, 0.4f, 1.6f, 1.6f);
	make_cube(0.0f, 0.0f, 0.0f, 0.8f, 0.1f, 1.6f);
	make_cube(0.0f, 1.5f, 0.0f, 0.8f, 1.6f, 1.6f);

	glEnd();
	glTranslatef(0.4f, 3.4f, 0.8f);
	glRotatef(90, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	gluCylinder(m_pQuadric1, 0.3, 0.3, 0.2, 10, 10);
	glColor3f(0.2, 0.2, 0.2);
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	gluDisk(m_pQuadric1, 0, 0.3, 10, 10);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.4f, 1.7f, -0.1f);
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	gluCylinder(m_pQuadric1, 0.2, 0.2, 0.1, 10, 10);
	glTranslatef(0.0f, 0.0f, 1.7f);
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	gluCylinder(m_pQuadric1, 0.2, 0.2, 0.1, 10, 10);
	glPopMatrix();
}
void make_s_brick() {
	glPushMatrix();
	glBegin(GL_QUADS);
	make_cube(0.0f, 0.0f, 0.0f, 0.8f, 0.8f, 0.8f);
	glEnd();
	glTranslatef(0.4f, 1.0f, 0.4f);
	glRotatef(90, 1, 0, 0);
	glColor3f(0.5, 0.5, 0.5);
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	gluCylinder(m_pQuadric1, 0.3, 0.3, 0.2, 10, 10);
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	gluDisk(m_pQuadric1, 0, 0.3, 10, 10);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.4f, 0.4f, -0.2f);
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	gluDisk(m_pQuadric1, 0, 0.3, 10, 10);
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	gluCylinder(m_pQuadric1, 0.3, 0.3, 0.2, 10, 10);
	glPopMatrix();
}
void make_s_plate() {
	glPushMatrix();
	glBegin(GL_QUADS);
	make_cube(0.0f, 0.0f, 0.0f, 0.8f, 0.3f, 0.8f);
	glEnd();
	glTranslatef(0.4f, 0.5f, 0.4f);
	glRotatef(90, 1, 0, 0);
	glColor3f(0.5, 0.5, 0.5);
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	gluCylinder(m_pQuadric1, 0.3, 0.3, 0.2, 10, 10);
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	gluDisk(m_pQuadric1, 0, 0.3, 10, 10);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.4f, 0.15f, -0.2f);
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	gluCylinder(m_pQuadric1, 0.05, 0.05, 1.2, 10, 10);
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	gluDisk(m_pQuadric1, 0, 0.05, 10, 10);
	glPopMatrix();
}
void file_read() {
	std::ifstream inFile("lego.txt");
	char lego_string[50] = { 0, };								//파일 한 줄
	while (inFile.getline(lego_string, 50)) {
		char* pch;
		pch = strtok(lego_string, " ");
		select_color[draw_count] = atoi(pch);
		pch = strtok(NULL, " ");
		select_block[draw_count] = atoi(pch);
		pch = strtok(NULL, " ");
		x_move[draw_count] = atof(pch);
		pch = strtok(NULL, " ");
		y_move[draw_count] = atof(pch);
		pch = strtok(NULL, " ");
		z_move[draw_count] = atof(pch);
		pch = strtok(NULL, " ");
		x_rot[draw_count] = atoi(pch);
		pch = strtok(NULL, " ");
		y_rot[draw_count] = atoi(pch);
		pch = strtok(NULL, " ");
		z_rot[draw_count] = atoi(pch);
		if (select_block[draw_count] == brick || select_block[draw_count] == plate) {
			pch = strtok(NULL, " ");
			x_count[draw_count] = atoi(pch);
			pch = strtok(NULL, " ");
			z_count[draw_count] = atoi(pch);
		}
		draw_count++;
	}
	inFile.close();
}
void file_write() {
	std::ofstream outFile("lego.txt");
	for (int i = 0; i <= draw_count; i++) {
		if (draw_count_sel == true && i == draw_count) {
			break;
		}
		outFile << select_color[i] << " " << select_block[i] << " "
			<< x_move[i] << " " << y_move[i] << " " << z_move[i] << " "
			<< x_rot[i] << " " << y_rot[i] << " " << z_rot[i];
		if (select_block[i] == brick || select_block[i] == plate) {
			outFile << " " << x_count[i] << " " << z_count[i];
		}
		outFile << std::endl;
	}
}
int main(int argc, char** argv) {


	/*window 초기화 */
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1200, 600);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("My Mini Project");

	init();		//->사용자 초기화 함수

				//callback 함수 정의
	glutKeyboardFunc(keyboard);								//키보드입력
	glutMouseFunc(mouse);									//마우스입력
	glutMotionFunc(motion);
	glutReshapeFunc(resize);
	glutDisplayFunc(draw);
	glutIdleFunc(idle);

	//Looping 시작
	glutMainLoop();

	return 0;
}