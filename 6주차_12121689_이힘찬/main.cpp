#include <gl/glut.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

void init(void);					//사용자 초기화 함수
void idle(void);					//계속 실행되는 함수
void clear(void);					//화면 지우는 함수
void draw(void);					//그림 그리는 함수
void draw_point(void);				//점을 찍는 함수
void draw_line(void);				//선을 그리는 함수
void draw_string(void* font, const char* str, int x, int y);	//문자 출력함수
void draw_string();												//문자 출력함수 호출함수
void change_color();											//색 변화 함수

void sub_menu_function(int option);								//서브 메뉴 작동함수
void main_menu_function(int option);							//메인 메뉴 작동함수
void mouse(int button, int state, int x, int y);				//마우스 입력함수
void keyboard(unsigned char key, int x, int y);					//키보드 입력함수

bool antialiase_on = false;										//antialise 확인
bool string_on = false;											//string 출력 유무
bool pivot_on = false;											//pivot 위치
bool trans_on = false;											//이동 유무
bool rotate_on = false;											//회전 유무
int shape = GL_POINTS;											//기본 모양(point로 초기화)
float r = 0.0f, g = 0.0f, b = 0.0f, alpha = 1.0f;				//현재 r,g,b,alpha 값 (디폴트 검은색)
int xx[10000] = { 0, }, yy[10000] = { 0, };						//각 점들의 x,y 좌표
float cc[10000] = { 0.0f, };									//각 점들의 색
int count = 0;													//점의 개수
double spin = 0, trans = 0;										//회전량과 이동량
double cx=0, cy=0;												//회전축 좌표

void init(void) {
	/*화면의 기본색으로 white 설정*/
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	/*화면 좌표 정보 설정*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0f, 500.0f, 0.0f, 500.0f);
}

void idle(void) {
	if (rotate_on == true) {									//rotate가 on일 떄
		spin = spin + 0.1;										//spin은 0.1씩 증가
		if (spin > 360) spin -= 360;							//spin이 360을 넘으면 -360을 뺴줌
	}
	if (trans_on == true) {										//trans가 on일 떄
		trans = trans + 0.1;									//trans는 0.1Tlr 증가
		if (trans > 500) trans = -500;							//trans가 500을 넘으면 trans -500을 해줌
	}
	glutPostRedisplay();										//계속 다시 보여줌
}


void clear(void) {
	/*화면을 깨끗하게 지우기*/
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

void draw(void) {
	clear();														//화면 지우고 시작
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();												//행렬 초기화
	if (rotate_on == true) {										//rotation가 on일 떄
		glTranslatef(cx, cy, 0);									//회전 축으로 평행 이동
		glRotatef(spin, 0, 0, 1);									//회전
		glTranslatef(-cx, -cy, 0);									//원래 있던 곳으로 평행이동
	}
	if (trans_on == true) {											//rotation가 on일 떄
		glTranslatef(trans, 0, 0);									//회전 축으로 평행 이동z
	}
	glPointSize(10.0f);												//폰트 크기 10
	glBegin(shape);													//설정된 모양 값으로
	for (int i = 0; i < count; i++) {								//점의 개수만큼 출력시작
		float tr = 0.0f, tb = 0.0f, tg = 0.0f;						//임시 색 저장변수
		if (shape == GL_TRIANGLES || shape == GL_QUADS) {
			if ((int)cc[i] % 10 == 3) {								//색이 r인 경우
				tr = 1.0f;											//임시 색은 red
				glColor3f(tr, tg, tb);
			}
			else if ((int)cc[i] % 10 == 2) {						//색이 g인 경우
				tg = 1.0f;											//임시 색은 green
				glColor3f(tr, tg, tb);
			}
			else if ((int)cc[i] % 10 == 1) {						//색이 b인 경우
				tb = 1.0f;											//임시 색은 blue
				glColor3f(tr, tg, tb);
			}
			else {													//기본 색
				glColor3f(tr, tg, tb);
			}
			if (antialiase_on == true) {							//antialiase가 on일 경우
				if ((int)cc[i] / 10 == 0) {							//alpha 값이 0일 때
					alpha = 0.0f;
				}
				else if ((int)cc[i] / 10 == 25) {					//alpha 값이 0.25일 떄
					alpha = 0.25f;
				}
				else if ((int)cc[i] / 10 == 50) {					//alpha 값이 0.5일 때
					alpha = 0.5f;
				}
				else if ((int)cc[i] / 10 == 75) {					//alpha 값이 0.75일 떄
					alpha = 0.75f;
				}
				else if ((int)cc[i] / 10 == 100) {					//alpha 값이 1 일 떄
					alpha = 1.0f;
				}
				glColor4f(tr, tg, tb, alpha);						//색을 alpha를 포함해서 지정
			}
		}
		else {
			glColor3f(r, g, b);										//alpha를 제외해서 지정
		}
		glVertex2i(xx[i], yy[i]);									//점 들을 x,y 좌표에 찍음
	}
	glEnd();
	glFlush();
	if (string_on == true) {									//string이 on일 때
		draw_string();											//string을 켬
	}
	glutSwapBuffers();											//두 개의 버퍼를 번갈아 바꿈
}

void draw_string(void* font, const char* str, int x, int y) {	//string 그리기
	unsigned int i;
	glRasterPos2i(x, y);
	for (i = 0; i < strlen(str); i++) {
		glutBitmapCharacter(font, str[i]);
	}
}

void draw_string(void) {										//상황 별 그릴 string
																//글자
	glColor3f(0.0f, 0.0f, 0.0f);								//색은 검정
	if (antialiase_on == true) {								//antialiase 가 on 일 때
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Antialiase: ON", 10, 10);	//Antialiase: ON 출력
	}
	else {
		draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "Antialiase: OFF", 10, 10);//Antialiase: OFF 출력
	}
	draw_string(GLUT_BITMAP_TIMES_ROMAN_24, "12121689 Lee Him Chan", 250, 480); //이름 학번 출력
	glFlush();
}

void change_color() {											//색 묶어주는 함수
	int temp = count;											//모양이 안 만들어졌을 때, 색을 같이 바꾸기 위한 변수
	if (shape == GL_TRIANGLES) {								//삼각형일 때
		temp = temp % 3;										//삼각형이 안 만들어지고 점이 1,2개 찍혔을 때를 위해서
		for (int i = count - 3 - temp; i < count; i++) {		//끝 3~5개의 색을 똑같게 함
			cc[i] = r * 3 + g * 2 + b * 1 + alpha * 1000;			//r,g,b,alpha를 한 번에 저장
		}
		for (int i = 0; i < count - 3 - temp; i = i + 3) {		//처음부터 나머지를 3개씩 묶어서 색을 같게 함
			cc[i + 1] = cc[i];
			cc[i + 2] = cc[i];
		}
	}
	else if (shape == GL_QUADS) {								//quad 일 때
		temp = temp % 4;										//quad가 안 만들어졌을 때, 색을 같이 바꾸기 위한 변수
		for (int i = count - 4 - temp; i < count; i++) {		//끝 4~7개의 색을 똑같게함
			cc[i] = r * 3 + g * 2 + b * 1 + alpha * 1000;		//r,g,b,alpha를 한 번에 저장
		}
		for (int i = 0; i < count - 4 - temp; i = i + 4) {		//처음부터 나머지를 4개씩 묶어서 색을 같게 함
			cc[i + 1] = cc[i];
			cc[i + 2] = cc[i];
			cc[i + 3] = cc[i];
		}
	}
	draw();														//변경된 것을 그림
}


void sub_menu_function(int option) {							//서브 메뉴
	printf("Submenu %d has been slected\n", option);
	if (option == 21) {											//line을 선택
		shape = GL_LINES;										//shape를 line으로 바꿈
		draw();													//그림
	}
	else if (option == 22) {									//line_strip을 선택
		shape = GL_LINE_STRIP;									//shape를 line_strip으로 바꿈
		draw();													//그림
	}
	else if (option == 23) {									//line_loop를 선택
		shape = GL_LINE_LOOP;									//shape를 line_loop로 바꿈
		draw();													//그림
	}
	else if (option == 31) {									//triangle을 선택
		shape = GL_TRIANGLES;									//shape를 triangle로 바꿈
		change_color();											//색 변경
		draw();													//그림
	}
	else if (option == 32) {									//triangle_strip을 선택
		shape = GL_TRIANGLE_STRIP;								//shape를 triangle_strip로 바꿈												
		change_color();											//색 변경
		draw();													//그림
	}
	else if (option == 33) {									//quad를 선택
		shape = GL_QUADS;										//shape를 quad로 바꿈													
		change_color();											//색 변경
		draw();													//그림
	}
	else if (option == 34) {									//POLYGON을 선택
		shape = GL_POLYGON;										//shape를 POLYGON로 바꿈												
		change_color();											//색 변경
		draw();													//그림
	}
}

void main_menu_function(int option) {							//메인 메뉴
	printf("Main menu %d has been selected\n", option);
	if (option == 1) {											//포인트 선택
		shape = GL_POINTS;										//shpae를 point로 바꿈
		draw();													//그림
	}
	else {
		clear();												//clear 선택
		count = 0;												//지금까지 찍은 점 없앰
		draw();													//string이 필요하면 그림
	}
}

void mouse(int button, int state, int x, int y) {				//마우스 입력 함수
	if (pivot_on == false) {										//pivot_on 이 꺼져있을 때
		if (button == 0 && state == 1) {							//왼쪽 버튼을 눌렀을 때
			printf("mouse %d %d \n", x, y);
			xx[count] = x;											//x좌표 기억
			yy[count] = 500 - y;									//y좌표 기억
			cc[count] = r * 3 + g * 2 + b * 1 + alpha * 1000;		//색 기억
			count++;												//점의 개수 증가
			draw();													//그림
		}
	}
	else if(pivot_on == true){										//pivot_on이 켜져있을 때
		cx = x;														//회전축 저장
		cy = 500-y;
		draw();														//그림
	}
}

void keyboard(unsigned char key, int x, int y) {
		if (key == 49) {											//1을 눌렀을 때
			r = 1.0f;												//빨간색으로 바꿈
			g = 0.0f;
			b = 0.0f;
			printf("red\n");
			if (shape == GL_TRIANGLES || shape == GL_QUADS) {		//triangle이나 quad일 경우 색 저장
				cc[count] = r * 3 + g * 2 + b * 1 + alpha * 1000;
			}
			change_color();											//색 변경
		}
		else if (key == 50) {										//2를 눌렀을 때
			r = 0.0f;												//초록색으로 바꿈
			g = 1.0f;
			b = 0.0f;
			printf("green\n");
			if (shape == GL_TRIANGLES || shape == GL_QUADS) {		//triangle이나 quad일 경우 색 저장
				cc[count] = r * 3 + g * 2 + b * 1 + alpha * 1000;
			}
			change_color();											//색 변경
		}
		else if (key == 51) {										//3을 눌렀을 때
			r = 0.0f;												//파란색으로 바꿈	
			g = 0.0f;
			b = 1.0f;
			printf("blue\n");
			if (shape == GL_TRIANGLES || shape == GL_QUADS) {		//triangle이나 quad일 경우 색 저장
				cc[count] = r * 3 + g * 2 + b * 1 + alpha * 1000;
			}
			change_color();											//색 변경
		}
		else if (key == 97) {										//a를 눌렀을 때
			if (antialiase_on == false) {							//antialiase 가 off 일 때
				glEnable(GL_BLEND);									//antialiase 관련한 것들을 켜고
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glEnable(GL_POINT_SMOOTH);
				glEnable(GL_LINE_SMOOTH);
				glEnable(GL_POLYGON_SMOOTH);
				glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
				glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
				glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
				antialiase_on = true;								//antialiase 를 on
				draw();												//그림
			}
			else {													//antialiase가 on 일 때
				glDisable(GL_BLEND);								//antialiase 관련한 것들을 끄고
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glDisable(GL_POINT_SMOOTH);
				glDisable(GL_LINE_SMOOTH);
				glDisable(GL_POLYGON_SMOOTH);
				glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
				glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
				glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
				antialiase_on = false;								//antialiase를 off
				draw();												//그림
			}
		}
		else if (key == 99) {										//c를 눌렀을 때
			if (string_on == false) {								//string 이 off 일 때
				draw_string();										//string 을 그리고
				string_on = true;									//string 을 on 함
			}
			else {													//string 이 on 일 떄
				string_on = false;									//string 이 off 함
				draw();												//그림
			}
		}
		else if (key == 121) {										//y를 눌렀을 때
			alpha = 0.0f;											//alpha 값 0
			change_color();											//색 변경
		}
		else if (key == 117) {										//u를 눌렀을 때
			alpha = 0.25f;											//alpha 값 0.25
			change_color();											//색 변경
		}
		else if (key == 105) {										//i를 눌렀을 때
			alpha = 0.5f;											//alpha 값 0.5
			change_color();											//색 변경
		}
		else if (key == 111) {										//o를 눌렀을 때
			alpha = 0.75f;											//alpha 값 0.75
			change_color();											//색 변경
		}
		else if (key == 112) {										//p를 눌렀을 때
			alpha = 1.0f;											//alpha 값 1.0
			change_color();											//색 변경
		}
		else if (key == 114) {										//r를 눌렀을 때
			if (rotate_on == false) {								//rotate 이 off 일 때
				draw();												//그림
				rotate_on = true;									//rotate 을 on 함
			}
			else {													//rotate 이 on 일 떄
				rotate_on = false;									//rotate 이 off 함
				draw();												//그림
			}
		}
		else if (key == 116) {										//t를 눌렀을 때
			if (trans_on == false) {								//trans 이 off 일 때
				draw();												//그림
				trans_on = true;									//trans 을 on 함
			}
			else {													//trans 이 on 일 떄
				trans_on = false;									//trans 이 off 함
				draw();												//그림
			}
		}
		else if (key == 118) {										//v를 눌렀을 때
			if (pivot_on == false) {								//pivot 이 off 일 때
				draw();												//그림
				pivot_on = true;									//pivot 을 on 함
			}
			else {													//pivot 이 on 일 떄
				pivot_on = false;									//pivot 이 off 함
				draw();												//그림
			}
		}
}

int main(int argc, char** argv) {
	int submenu1 = 0, submenu2 = 0;					//서브 메뉴
													/*window 초기화 */

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("My Thrid GL Program");

	init();		//->사용자 초기화 함수

				//Popup menu 생성 및 추가
	submenu1 = glutCreateMenu(sub_menu_function);			//3개의 항목이 있는 서브메뉴 설정
	glutAddMenuEntry("GL_LINES", 21);						//line 은 21
	glutAddMenuEntry("GL_LINE_STRIP", 22);					//line_strip은 22
	glutAddMenuEntry("GL_LINE_LOOP", 23);					//line_loop는 23

	submenu2 = glutCreateMenu(sub_menu_function);			//4개의 항목이 있는 서브메뉴 설정
	glutAddMenuEntry("GL_TRIANGLES", 31);					//triangle은 31
	glutAddMenuEntry("GL_TRIANFGLE_STRIP", 32);				//triangle_strip은 32
	glutAddMenuEntry("GL_QUADS", 33);						//quad은 33
	glutAddMenuEntry("GL_POLYGON", 34);						//polygon은 34

	glutCreateMenu(main_menu_function);						//4개의 항목이 있는 메뉴 설정
	glutAddMenuEntry("Point Draw", 1);						//point 는 1
	glutAddSubMenu("Line Draw", submenu1);					//line draw는 서브메뉴1
	glutAddSubMenu("Polygon Draw", submenu2);				//polygon draw는 서브메뉴2
	glutAddMenuEntry("Clear", 0);							//화면지우기는 0
	glutAttachMenu(GLUT_RIGHT_BUTTON);						//메뉴는 오른쪽버튼

	glutKeyboardFunc(keyboard);								//키보드입력
	glutMouseFunc(mouse);									//마우스입력

															//callback 함수 정의
	glutDisplayFunc(draw);									//그림
	glutIdleFunc(idle);

															//Looping 시작
	glutMainLoop();


	return 0;
}

