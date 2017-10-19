#include <gl/glut.h>
#include <string.h>
#include <iostream>
using namespace std;

void init(void);
void draw(void);
void resize(int width, int height);
void keyboard(unsigned char key, int x, int y);
void draw_string(void* font, const char* str, float x_position, float y_position, float red, float green, float blue);
void draw_text(void);

//각 행성들 공전, 자전 주기
double angle1 = 0, angle2 = 0, angle3 = 0, angle4 = 0, angle5 = 0, angle6 = 0
, angle7 = 0, angle8 = 0, angle9 = 0, angle10 = 0, angle11 = 0, angle12 = 0;

int light_location = 0;
double light_y = 0.0;

void idle(void) {
	angle1 = angle1 + 0.002;
	if (angle1 > 360) angle1 -= 360;				//지구 공전
	angle2 = angle2 + 0.01;
	if (angle2 > 360) angle2 -= 360;				//지구 자전
	angle3 = angle3 + 0.0083;
	if (angle3 > 360) angle3 -= 360;				//화성 공전
	angle4 = angle4 + 0.0097;
	if (angle4 > 360) angle4 -= 360;				//화성 자전
	angle5 = angle5 + 1.3051;
	if (angle5 > 360) angle5 -= 360;				//달 공전
	angle6 = angle6 + 0.0003;
	if (angle6 > 360) angle6 -= 360;				//달 자전
	angle7 = angle7 + 0.041;
	if (angle7 > 360) angle7 -= 360;				//수성 공전
	angle8 = angle8 + 0.00016;
	if (angle8 > 360) angle8 -= 360;				//수성 자전
	angle9 = angle9 + 0.3077;
	if (angle9 > 360) angle9 -= 360;				//목성 공전
	angle10 = angle10 + 0.0243;
	if (angle10 > 360) angle10 -= 360;				//목성 자전
	angle11 = angle11 + 0.1221;
	if (angle11 > 360) angle11 -= 360;				//토성 공전
	angle12 = angle12 + 0.0227;
	if (angle12 > 360) angle12 -= 360;				//토성 자전
	glutPostRedisplay();										//계속 다시 보여줌
}
void init(void) {
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

	glEnable(GL_DEPTH_TEST);
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
	GLfloat light_position[] = { 0.0, light_y, 0.0, 1.0 };
	if (light_location == 0 || light_location == 8 || light_location == 9)
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	gluLookAt(15, 15, 15, 0, 0, 0, 0, 1, 0);

	draw_text();

	if (light_location == 1)
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	//glColor3f(1, 1, 0);									//노랑
	glutSolidSphere(2, 30, 30);							//태양

	glPushMatrix();										//태양위치 기억
	glPushMatrix();										//태양위치 기억
	glPushMatrix();										//태양위치 기억
	glPushMatrix();										//태양위치 기억

	glRotatef(angle1, 0, 1, 0);							//태양 중심 지구 공전
	glTranslatef(5, 0, 0);								//지구 위치
	glRotatef(angle2, 0, 1, 0);							//지구 자전
														//glColor3f(0, 1, 0);									//지구는 초록색
	if (light_location == 3)
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glutSolidSphere(1.5, 30, 30);

	glRotatef(angle5, 0, 1, 0);							//지구 중심 달 공전
	glTranslatef(2, 0, 0);								//달 위치
	glRotatef(angle6, 0, 1, 0);							//달 자전
														//glColor3f(1, 1, 1);									//달 흰색
	if (light_location == 4)
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glutSolidSphere(0.8, 30, 30);						//달

	glPopMatrix();										//지구, 달 있기 전 위치

	glRotatef(angle3, 0, 1, 0);							//태양 중심 화성 공전
	glTranslatef(8, 0, 0);								//화성 위치
	glRotatef(angle4, 0, 1, 0);							//화성 자전
														//glColor3f(1, 0, 0);									//화성은 빨간색
	if (light_location == 5)
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glutSolidSphere(1.3, 30, 30);						//화성

	glPopMatrix();										//지구, 달 있기 전 위치

	glRotatef(angle7, 0, 1, 0);							//태양 중심 금성 공전
	glTranslatef(2.5, 0, 0);								//수성 위치
	glRotatef(angle8, 0, 1, 0);							//수성 자전
														//glColor3f(0, 0, 0.8);								//수성은 어두운 파란색
	if (light_location == 2)
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glutSolidSphere(0.5, 30, 30);						//수성

	glPopMatrix();										//지구, 달 있기 전 위치

	glRotatef(angle9, 0, 1, 0);							//태양 중심 목성 공전
	glTranslatef(10, 0, 0);								//목성 위치
	glRotatef(angle10, 0, 1, 0);						//목성 자전
														//glColor3f(0.3, 0, 0);								//목성은 갈색
	if (light_location == 6)
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glutSolidSphere(1.8, 30, 30);						//목성

	glPopMatrix();										//지구, 달 있기 전 위치

	glRotatef(angle11, 0, 1, 0);						//태양 중심 토성 공전
	glTranslatef(12, 0, 0);								//토성 위치
	glRotatef(angle12, 0, 1, 0);						//토성 자전
														//glColor3f(0.5, 0.5, 0);								//토성은 황토색
	if (light_location == 7)
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glutSolidSphere(1.7, 30, 30);						//토성

	glFlush();
	glutSwapBuffers();						//double buffering시

}
void resize(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)width / (float)height, 1, 500);
	glMatrixMode(GL_MODELVIEW);
}
void keyboard(unsigned char key, int x, int y) {
	if (key == '0') {										//카메라 위치
		light_location = 0;
		light_y = 0;
	}
	else if (key == '1') {									//태양
		light_location = 1;
		light_y = 0;
	}
	else if (key == '2') {									//수성
		light_location = 2;
		light_y = 0;
	}
	else if (key == '3') {									//지구
		light_location = 3;
		light_y = 0;
	}
	else if (key == '4') {									//달
		light_location = 4;
		light_y = 0;
	}
	else if (key == '5') {									//화성
		light_location = 5;
		light_y = 0;
	}
	else if (key == '6') {									//목성
		light_location = 6;
		light_y = 0;
	}
	else if (key == '7') {									//토성
		light_location = 7;
		light_y = 0;
	}
	else if (key == '8') {									//위
		light_location = 8;
		light_y = 30;
	}
	else if (key == '9') {									//아래
		light_location = 9;
		light_y = -30;
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
	draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "12121689 Lee Him Chan", -5, 4.5, 0, 1, 0);		//폰트, 내용, 위치, 색 지정
	draw_string(GLUT_BITMAP_HELVETICA_18, "Light: ", -5, 4, 1, 0, 0);
	char content[10][20] = { "camera", "sun", "mercury", "earth", "moon", "mars", "jupiter", "saturn", "top", "bottom" };		//광원위치를 나타내는 문자열
	draw_string(GLUT_BITMAP_HELVETICA_18, content[light_location], -4, 4, 1, 1, 0);												//광원위치 출력

	glFlush();

	glPopMatrix();
}
int main(int argc, char** argv) {
	/*window 초기화 */

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("My nineth GL Program");

	init();		//->사용자 초기화 함수

				//callback 함수 정의
	glutDisplayFunc(draw);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyboard);								//키보드입력
	glutIdleFunc(idle);

	//Looping 시작
	glutMainLoop();

	return 0;
}