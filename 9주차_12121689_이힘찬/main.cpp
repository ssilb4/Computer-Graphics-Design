#include <gl/glut.h>

void init(void);
void draw(void);
void resize(int width, int height);

//각 행성들 공전, 자전 주기
double angle1 = 0, angle2 = 0, angle3 = 0, angle4 = 0, angle5 = 0, angle6 = 0
, angle7 = 0, angle8 = 0, angle9 = 0, angle10 = 0, angle11 = 0, angle12 = 0;

void idle(void) {
	angle1 = angle1 + 0.1;
	if (angle1 > 360) angle1 -= 360;				//지구 공전
	angle2 = angle2 + 1;
	if (angle2 > 360) angle2 -= 360;				//지구 자전
	angle3 = angle3 + 0.53;
	if (angle3 > 360) angle3 -= 360;				//화성 공전
	angle4 = angle4 + 0.97;
	if (angle4 > 360) angle4 -= 360;				//화성 자전
	angle5 = angle5 + 13.51;
	if (angle5 > 360) angle5 -= 360;				//달 공전
	angle6 = angle6 + 0.03;
	if (angle6 > 360) angle6 -= 360;				//달 자전
	angle7 = angle7 + 0.41;
	if (angle7 > 360) angle7 -= 360;				//수성 공전
	angle8 = angle8 + 0.016;
	if (angle8 > 360) angle8 -= 360;				//수성 자전
	angle9 = angle9 + 30.77;
	if (angle9 > 360) angle9 -= 360;				//목성 공전
	angle10 = angle10 + 2.43;
	if (angle10 > 360) angle10 -= 360;				//목성 자전
	angle11 = angle11 + 12.21;
	if (angle11 > 360) angle11 -= 360;				//토성 공전
	angle12 = angle12 + 2.27;
	if (angle12 > 360) angle12 -= 360;				//토성 자전

	glutPostRedisplay();										//계속 다시 보여줌
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
	
	/*화면의 기본색으로 black 설정*/
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	/*화면 좌표 정보 설정*/
	gluOrtho2D(0.0f, 2000.0f, 0.0f, 1000.0f);
}
void draw(void) {
	/*화면을 깨끗하게 지우기*/
	//glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(10, 10, 10, 0, 0, 0, 0, 1, 0);

	glColor3f(1, 1, 0);									//노랑
	glutWireTeapot(1);									//태양

	glPushMatrix();										//태양위치 기억
	glPushMatrix();										//태양위치 기억
	glPushMatrix();										//태양위치 기억
	glPushMatrix();										//태양위치 기억

	glRotatef(angle1, 0, 1, 0);							//태양 중심 지구 공전
	glTranslatef(5, 0, 0);								//지구 위치
	glRotatef(angle2, 0, 1, 0);							//지구 자전
	glColor3f(0, 1, 0);									//지구는 초록색
	glutWireTeapot(1);									//지구

	glRotatef(angle5, 0, 1, 0);							//지구 중심 달 공전
	glTranslatef(2, 0, 0);								//달 위치
	glRotatef(angle6, 0, 1, 0);							//달 자전
	glColor3f(1, 1, 1);									//달 흰색
	glutWireTeapot(1);									//달

	glPopMatrix();										//지구, 달 있기 전 위치

	glRotatef(angle3, 0, 1, 0);							//태양 중심 화성 공전
	glTranslatef(8, 0, 0);								//화성 위치
	glRotatef(angle4, 0, 1, 0);							//화성 자전
	glColor3f(1, 0, 0);									//화성은 빨간색
	glutWireTeapot(1);									//화성

	glPopMatrix();										//지구, 달 있기 전 위치

	glRotatef(angle7, 0, 1, 0);							//태양 중심 금성 공전
	glTranslatef(2, 0, 0);								//수성 위치
	glRotatef(angle8, 0, 1, 0);							//수성 자전
	glColor3f(0, 0, 0.8);								//수성은 어두운 파란색
	glutWireTeapot(1);									//수성

	glPopMatrix();										//지구, 달 있기 전 위치

	glRotatef(angle9, 0, 1, 0);							//태양 중심 목성 공전
	glTranslatef(10, 0, 0);								//목성 위치
	glRotatef(angle10, 0, 1, 0);						//목성 자전
	glColor3f(0.3, 0, 0);								//목성은 갈색
	glutWireTeapot(1);									//목성

	glPopMatrix();										//지구, 달 있기 전 위치

	glRotatef(angle11, 0, 1, 0);						//태양 중심 토성 공전
	glTranslatef(12, 0, 0);								//토성 위치
	glRotatef(angle12, 0, 1, 0);						//토성 자전
	glColor3f(0.5, 0.5, 0);								//토성은 황토색
	glutWireTeapot(1);									//토성

	glFlush();
	glutSwapBuffers();						//double buffering시
}
void resize(int width, int height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)width / (float)height, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
}
int main(int argc, char** argv) {
	/*window 초기화 */

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(2000, 1000);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("My Seventh GL Program");

	init();		//->사용자 초기화 함수

				//callback 함수 정의
	glutDisplayFunc(draw);
	glutReshapeFunc(resize);
	glutIdleFunc(idle);

	//Looping 시작
	glutMainLoop();

	return 0;
}