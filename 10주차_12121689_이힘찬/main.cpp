#include <gl/glut.h>
#include <string.h>

//사용될 함수들
void init(void);									//사용자 정의 초기화 함수
void draw(void);									//그리는 함수
void resize(int width, int height);					//크기 조절 함수
void draw_can(void);								//can 그리는 함수
void draw_disk(void);								//disk 그리는 함수
void draw_teapot(void);								//주전자 그리는 함수
void draw_text(void);								//글자 쓰는 함수
void draw_axis(void);								//축 그리는 함수

bool is_rotate = true;								//회전여부
double spin = 0;									//회전각
float color0 = 0.0f, color1 = 1.0f;					//색 조절

void init(void) {
	//Depth 사용
	glEnable(GL_DEPTH_TEST);

	/*화면의 기본색으로 white 설정*/
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	/*화면 좌표 정보 설정*/
	gluOrtho2D(0.0f, 500.0f, 0.0f, 500.0f);
}
void idle(void) {
	if(is_rotate == true)										//is_rotate가 true면
		spin = spin + 0.1;										//0.1씩 회전
	if (spin >= 360) {											//회전각이 360을 넘으면
		spin -= 360;											//360을 빼줌
	}
	glutPostRedisplay();										//계속 다시 보여줌
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
	/*화면을 깨끗하게 지우기*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);

	draw_text();													//글자 표시
	draw_disk();													//디스크 표시
	draw_teapot();													//주전자 표시
	draw_can();														//캔 표시

	glutSwapBuffers();
}
void draw_can(void) {
	glPushMatrix();													//여기서만 실행되게 하기 위해 저장

	glViewport(0, 0, 800 / 2, 600 / 2);								//3사분면
	draw_axis();													//축 그림
	glRotatef(spin, 1, 0, 0);

	GLUquadricObj* m_pQuadric1;
	m_pQuadric1 = gluNewQuadric();
	gluQuadricDrawStyle(m_pQuadric1, GLU_FILL);
	gluQuadricNormals(m_pQuadric1, GLU_SMOOTH);

	GLUquadricObj* m_pQuadric2;
	m_pQuadric2 = gluNewQuadric();
	gluQuadricDrawStyle(m_pQuadric2, GLU_FILL);
	gluQuadricNormals(m_pQuadric2, GLU_SMOOTH);

	glTranslatef(0, 0, 2);											//뚜껑위치
	glColor3f(color1, color0, color0);
	gluDisk(m_pQuadric2, 0, 2, 20, 10);								//뚜껑 그림
	glTranslatef(0, 0, -4);											//통 위치
	glColor3f(color0, color1, color0);
	gluCylinder(m_pQuadric1, 2, 2, 4, 10, 10);						//통 그림
	glColor3f(color1, color0, color0);
	gluDisk(m_pQuadric2, 0, 2, 20, 10);								//뚜껑 그림

	glFlush();

	glPopMatrix();													//저장 끝
}
void draw_disk(void) {
	glPushMatrix();													//여기서만 실행되게 하기 위해 저장

	glViewport(800 / 2, 600 / 2, 800 / 2, 600 / 2);					//1사분면
	draw_axis();													//축 그림
	glColor3f(color0, color1, color1);
	glRotatef(spin, 0, 1, 0);
	GLUquadricObj* m_pQuadric2;
	m_pQuadric2 = gluNewQuadric();
	gluQuadricDrawStyle(m_pQuadric2, GLU_FILL);
	gluQuadricNormals(m_pQuadric2, GLU_SMOOTH);
	gluDisk(m_pQuadric2, 2, 4, 20, 10);								//디스크 그림

	glFlush();

	glPopMatrix();													//저장 끝
}
void draw_teapot(void) {
	glPushMatrix();													//여기서만 실행되게 하기 위해 저장

	glViewport(800 / 2, 0, 800 / 2, 600 / 2);						//2사분면
	draw_axis();													//축 그림
	glColor3f(color1, color1, color0);
	glRotatef(spin, 0, 0, 1);
	glutWireTeapot(2);												//주전자 그림

	glFlush();

	glPopMatrix();													//저장 끝
}
void draw_text(void) {
	glPushMatrix();

	glViewport(0, 600 / 2, 800 / 2, 600 / 2);						//4사분면
	glLoadIdentity();			//기존의 회전변환을 무시
	draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Graphics are cool!!", 0, 0, color0, color0, color1);		//폰트, 내용, 위치, 색 지정
	draw_string(GLUT_BITMAP_HELVETICA_18, "Multi-viewport Example!!", 0, 1, color1, color0, color1);


	glFlush();

	glPopMatrix();
}
void draw_axis(void) {
	glLineWidth(3);							//좌표축의 두께
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);						//x축은 red
	glVertex3f(0, 0, 0);
	glVertex3f(4, 0, 0);

	glColor3f(0, 1, 0);						//y축은 green
	glVertex3f(0, 0, 0);
	glVertex3f(0, 4, 0);

	glColor3f(0, 0, 1);						//z축은 blue
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 4);

	glEnd();
	glLineWidth(1);
}
void keyboard(unsigned char key, int x, int y) {
	if (key == 's') {										//s을 눌렀을 때
		is_rotate = is_rotate == true ? false : true;		//is_rotate 값을 토글
	}
	else if (key == 'c') {									//c을 눌렀을 때
		color0 = color0 == 1.0f ? 0.0f : 1.0f;				//color 값들을 0과 1로 토글
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
	/*window 초기화 */

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("My Forth GL Program");

	init();		//->사용자 초기화 함수

				//callback 함수 정의
	glutKeyboardFunc(keyboard);								//키보드입력
	glutReshapeFunc(resize);
	glutDisplayFunc(draw);
	glutIdleFunc(idle);

	//Looping 시작
	glutMainLoop();

	return 0;
}