#include <gl/glut.h>
#include <math.h>
#include <stdio.h>

#define M_PI 3.14159265358979323846					//원주율

void init(void);									//사용자 정의 함수
void draw(void);									//그림 그리는 함수
void resize(int width, int height);					//창 크기 바꼈을 때 실행되는 함수
void draw_axis(void);								//축을 그리는 함수
void specialkeyboard(int key, int x, int y);		//방향키 눌렸을 때 실행되는 함수

double eyeX = 20, eyeY = 0, eyeZ = 0;				//사람의 기준으로 x,y,z 좌표
int up = 1;											//카메라의 위를 나타내는 변수

const double r = sqrt(402);							//r은 고정값
double theta = acos(eyeZ / r);						//theta값 구하는 공식
double phi = atan(eyeY / eyeX);						//phi값 구하는 공식

void init(void) {
	/*화면의 기본색으로 black 설정*/
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	/*화면 좌표 정보 설정*/
	gluOrtho2D(0.0f, 500.0f, 0.0f, 500.0f);
}
void draw(void) {									//그리는 함수
	/*화면을 깨끗하게 지우기*/
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyeY, eyeZ, eyeX, 0, 0, 0, 0, up, 0);		//eyeX,eyeY,eyeZ는 사람이 보기에 좌표고 인자의 순서는 opengl에서의 x,y,z 순서

	glColor3f(1, 1, 0);									//노란색
	glutWireTeapot(4);									//주전자
	draw_axis();										//x,y,z 축 그리기

	glFlush();
	glutSwapBuffers();									//double buffering시
}
void resize(int width, int height) {					//창 크기 바꼈을 때
	glViewport(0, 0, width, height);					//높이랑 너비가 바꼈을 때
	glMatrixMode(GL_PROJECTION);						//projection matrix 활성화
	glLoadIdentity();
	gluPerspective(45, (float)width / (float)height, 1, 500);
	glMatrixMode(GL_MODELVIEW);							//MODELVIEW로 환원
}
void draw_axis(void) {						//축 그리기
	glLineWidth(3);							//좌표축의 두께
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);						//opengl 상의 x축은 red
	glVertex3f(0, 0, 0);
	glVertex3f(4, 0, 0);

	glColor3f(0, 1, 0);						//opengl 상의 y축은 green
	glVertex3f(0, 0, 0);
	glVertex3f(0, 4, 0);

	glColor3f(0, 0, 1);						//opengl 상의 z축은 blue
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 4);

	glEnd();
	glLineWidth(1);							//선 길이 1
}
void specialkeyboard(int key, int x, int y) {				//화살표 입력 됐을 때
	if (key == GLUT_KEY_LEFT) {								//왼쪽 화살표를 눌렀을 때
		phi = phi - M_PI/180;								//phi가  1도씩 감소
		if (phi < 0) {										//phi가 0보다 작으면 2*원주율을 더해줌
			phi += 2 * M_PI;
		}
		if (phi >= 2 * M_PI) {								//phi가 2*원주율보다 크면  2*원주율을 빼줌
			phi -= 2 * M_PI;
		}
		eyeX = r*sin(theta)*cos(phi);						//구면좌표계를 이용한 x좌표 구하는 공식
		eyeY = r*sin(theta)*sin(phi);						//구면좌표계를 이용한 y좌표 구하는 공식
		eyeZ = r*cos(theta);								//구면좌표계를 이용한 z좌표 구하는 공식
	}
	else if (key == GLUT_KEY_RIGHT) {						//오른쪽 화살표를 눌렀을 때
		phi = phi + M_PI / 180;								//phi가 1도씩 증가
		if (phi < 0) {										//phi가 0보다 작으면 2*원주율을 더해줌
			phi += 2*M_PI;
		}
		if (phi >= 2 * M_PI) {								//phi가 2*원주율보다 크면  2*원주율을 빼줌
			phi -= 2 * M_PI;
		}
		eyeX = r*sin(theta)*cos(phi);						//구면좌표계를 이용한 x좌표 구하는 공식
		eyeY = r*sin(theta)*sin(phi);						//구면좌표계를 이용한 y좌표 구하는 공식
		eyeZ = r*cos(theta);								//구면좌표계를 이용한 z좌표 구하는 공식
	}
	else if (key == GLUT_KEY_UP) {							//위쪽 화살표를 눌렀을 때
		theta = theta - M_PI / 180;							//theta가  1도씩 감소
		if (theta < 0) {									//theta가 0보다 작으면 2*원주율을 더해줌
			theta += M_PI*2;
		}
		else if (theta >= M_PI * 2) {						//theta가 2*원주율보다 크면  2*원주율을 빼줌
			theta -= M_PI * 2;
		}
		if (theta >= 0 && theta < M_PI) {					//theta가 0~파이 사이면 카메라의 윗방향 = 1
			up = 1;
		}
		else {												//아니면 카메라의 윗방향 = -1
			up = -1;
		}
		eyeX = r*sin(theta)*cos(phi);						//구면좌표계를 이용한 x좌표 구하는 공식
		eyeY = r*sin(theta)*sin(phi);						//구면좌표계를 이용한 y좌표 구하는 공식
		eyeZ = r*cos(theta);								//구면좌표계를 이용한 z좌표 구하는 공식
	}
	else if (key == GLUT_KEY_DOWN) {						//아래쪽 화살표를 눌렀을 때
		theta = theta + M_PI / 180;							//theta가 1도씩 증가
		if (theta < 0) {									//theta가 0보다 작으면 2*원주율을 더해줌
			theta += M_PI * 2;
		}
		else if (theta >= M_PI * 2) {						//theta가 2*원주율보다 크면  2*원주율을 빼줌
			theta -= M_PI * 2;
		}
		if (theta >= 0 && theta < M_PI) {					//theta가 0~파이 사이면 카메라의 윗방향 = 1
			up = 1;
		}
		else {												//아니면 카메라의 윗방향 = -1
			up = -1;
		}
		eyeX = r*sin(theta)*cos(phi);						//구면좌표계를 이용한 x좌표 구하는 공식
		eyeY = r*sin(theta)*sin(phi);						//구면좌표계를 이용한 y좌표 구하는 공식
		eyeZ = r*cos(theta);								//구면좌표계를 이용한 z좌표 구하는 공식
	}
	draw();													//다시 그림
}
int main(int argc, char** argv) {
	/*window 초기화 */

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);							//창 크기
	glutInitWindowPosition(300, 300);						//위치
	glutCreateWindow("My Forth GL Program");

	init();		//->사용자 초기화 함수

				//callback 함수 정의
	glutDisplayFunc(draw);									//그리기
	glutReshapeFunc(resize);								//창크기가 바뀔 때
	glutSpecialFunc(specialkeyboard);						//방향키를 눌렸을 때

	//Looping 시작	
	glutMainLoop();											//반복

	return 0;
}