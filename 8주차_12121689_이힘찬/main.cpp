#include <gl/glut.h>
#include <math.h>
#include <stdio.h>

#define M_PI 3.14159265358979323846					//원주율

//함수
void idle(void);									//항상 실행되는 곳
void init(void);									//사용자 초기화
void draw(void);									//그리는 함수
void rotate(void);									//회전하는 함수
void resize(int width, int height);					//창 크기가 변경됐을 때
void specialkeyboard(int key, int x, int y);		//방향키를 눌렀을 떄
void keyboard(unsigned char key, int x, int y);		//일반키를 눌렀을 때

double m_RotateAngle;								//회전축의 각도
bool is_rotate = true;								//회전 유무 파악
double Xp = 1, Yp = 0, Zp = 0;			//사람의 기준으로 x,y,z 좌표 평면의 방정식을 나타내는 값
double xx = 10, yy = 10, zz = 10;					//카메라의 위치를 나타내는 변ㄴ수
int up = 1;											//카메라의 위를 나타내는 변수

const double r = sqrt(1);							//r은 고정값
double theta = acos(Zp / r);						//theta값 구하는 공식
double phi = atan(Yp / Xp);						//phi값 구하는 공식

void idle(void) {									//항상 실행
	if (is_rotate == true) {						//is_rotate가 트루일 때
		m_RotateAngle = m_RotateAngle + 0.5;		//회전각도가 0.5씩 증가
	}
	glutPostRedisplay();							//계속 다시 보여줌
}
void init(void) {
	//Light 설정 -> 일단 그냥 이용
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
	glFrontFace(GL_CW);						//teapot은 CW로 정의된
	glEnable(GL_DEPTH_TEST);
	/*화면의 기본색으로 Whtie 설정*/
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	/*화면 좌표 정보 설정*/
	gluOrtho2D(0.0f, 500.0f, 0.0f, 500.0f);
}
void rotate(void) {
	glRotatef(m_RotateAngle, 0, 1, 0);						//m_RotateAngle만큼 회전한다.
	GLdouble eq[4] = { Yp, Zp, Xp, 0.0 };					//잘린 면의 방정식
	glClipPlane(GL_CLIP_PLANE0, eq);						//잘린 면의 방정식으로 자름
	glEnable(GL_CLIP_PLANE0);								//실행
}
void draw(void) {
	/*화면을 깨끗하게 지우기*/
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);								//MODELVIEW 모드
	glLoadIdentity();										//초기화
	gluLookAt(xx, yy, zz, 0, 0, 0, 0, 1, 0);				//xx,yy,zz 좌표에서 0,0,0 을 바라봄

	glPushMatrix();											//현재 좌표를 저장
	rotate();												//회전
	glPopMatrix();											//저장한 좌표를 불러옴

	glColor3f(1, 0, 0);										//색은 빨강
	glutSolidTeapot(4);										//솔리드 주전자

	glFlush();
	glutSwapBuffers();										//double buffering시
}
void resize(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);								//projection 모드
	glLoadIdentity();											//초기화
	gluPerspective(45, (float)width / (float)height, 1, 500);	//perspective 모드
	glMatrixMode(GL_MODELVIEW);									//MODELVIEW 모드
}
void specialkeyboard(int key, int x, int y) {				//화살표 입력 됐을 때
	if (key == GLUT_KEY_LEFT) {								//왼쪽 화살표를 눌렀을 때
		phi = phi - M_PI / 180;								//phi가  1도씩 감소
		if (phi < 0) {										//phi가 0보다 작으면 2*원주율을 더해줌
			phi += 2 * M_PI;
		}
		if (phi >= 2 * M_PI) {								//phi가 2*원주율보다 크면  2*원주율을 빼줌
			phi -= 2 * M_PI;
		}
		Xp = r*sin(theta)*cos(phi);						//구면좌표계를 이용한 x좌표 구하는 공식
		Yp = r*sin(theta)*sin(phi);						//구면좌표계를 이용한 y좌표 구하는 공식
		Zp = r*cos(theta);								//구면좌표계를 이용한 z좌표 구하는 공식
	}
	else if (key == GLUT_KEY_RIGHT) {						//오른쪽 화살표를 눌렀을 때
		phi = phi + M_PI / 180;								//phi가 1도씩 증가
		if (phi < 0) {										//phi가 0보다 작으면 2*원주율을 더해줌
			phi += 2 * M_PI;
		}
		if (phi >= 2 * M_PI) {								//phi가 2*원주율보다 크면  2*원주율을 빼줌
			phi -= 2 * M_PI;
		}
		Xp = r*sin(theta)*cos(phi);						//구면좌표계를 이용한 x좌표 구하는 공식
		Yp = r*sin(theta)*sin(phi);						//구면좌표계를 이용한 y좌표 구하는 공식
		Zp = r*cos(theta);								//구면좌표계를 이용한 z좌표 구하는 공식
	}
	else if (key == GLUT_KEY_UP) {							//위쪽 화살표를 눌렀을 때
		theta = theta - M_PI / 180;							//theta가  1도씩 감소
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
		Xp = r*sin(theta)*cos(phi);						//구면좌표계를 이용한 x좌표 구하는 공식
		Yp = r*sin(theta)*sin(phi);						//구면좌표계를 이용한 y좌표 구하는 공식
		Zp = r*cos(theta);								//구면좌표계를 이용한 z좌표 구하는 공식
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
		Xp = r*sin(theta)*cos(phi);						//구면좌표계를 이용한 x좌표 구하는 공식
		Yp = r*sin(theta)*sin(phi);						//구면좌표계를 이용한 y좌표 구하는 공식
		Zp = r*cos(theta);								//구면좌표계를 이용한 z좌표 구하는 공식
	}
	draw();													//다시 그림
}
void keyboard(unsigned char key, int x, int y) {
	if (key == 's') {										//s을 눌렀을 때
		is_rotate = is_rotate == true ? false : true;		//is_rotate 값을 토글
	}
	else if (key == 'z') {									//z을 눌렀을 떄
		xx -= 0.2;											//xx,yy,zz가 0.2씩 감소
		yy -= 0.2;
		zz -= 0.2;
	}
	else if (key == 'x') {									//x을 눌렀을 떄
		xx += 0.2;											//xx,yy,zz가 0.2씩 증가
		yy += 0.2;
		zz += 0.2;
	}
}
int main(int argc, char** argv) {
	/*window 초기화 */

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("My Sixth GL Program");

	init();		//->사용자 초기화 함수

				//callback 함수 정의
	glutDisplayFunc(draw);									//그림을 그림
	glutReshapeFunc(resize);								//reshape 함수 실행
	glutIdleFunc(idle);										//idle 함수실행
	glutSpecialFunc(specialkeyboard);						//방향키를 눌렸을 때
	glutKeyboardFunc(keyboard);								//키보드입력

	//Looping 시작
	glutMainLoop();

	return 0;
}