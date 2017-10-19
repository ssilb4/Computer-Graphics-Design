#include <Windows.h>
#include <gl/glut.h>
#include <stdio.h>

float g_pPosition[6] = { 0.0f, 144.0f, -100.0f, -100.0f, 100.0f, -100.0f };

int g_nX, g_nY;
int g_nSelect = 0;

int g_nGLWidth = 500, g_nGLHeight = 500;
#define red 100
#define green 010
#define blue 001
bool is_color = true;
struct object {
	int color;
	int size;
};
object ob[3];
void init(void) {
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	GLfloat light_ambient[] = { 0.1,0.1,0.1,1.0 };
	GLfloat light_diffuse[] = { 1.0,1.0,1.0,1.0 };
	GLfloat light_specular[] = { 1.0,1.0,1.0,1.0 };
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	GLfloat light_position[] = { 1.0,1.0,1.0,0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	GLfloat mat_specular[] = { 1.0f,1.0f,1.0f,1.0f };
	glMaterialf(GL_FRONT, GL_SHININESS, 64);
	glEnable(GL_COLOR_MATERIAL);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-g_nGLWidth / 2.0f, g_nGLWidth / 2.0f, -g_nGLHeight / 2.0f, g_nGLHeight / 2.0f, -100, 100);
}
void DrawBackground() {
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	glColor3f(1.0f, 1.0f, 1.0f);
	int i;
	int offset = 50;
	glBegin(GL_LINES);
	for (i = -g_nGLWidth / 2.0f; i < g_nGLWidth / 2.0f; i += offset) {
		glVertex3f(i, -g_nGLHeight / 2.0f, 0.0f);
		glVertex3f(i, g_nGLHeight / 2.f, 0.0f);
	}
	for (i = -g_nGLHeight / 2.0f; i < g_nGLHeight / 2.0f; i += offset) {
		glVertex3f(-g_nGLWidth / 2.0f, i, 0.0f);
		glVertex3f(g_nGLWidth / 2.0f, i, 0.0f);
	}
	glEnd();
	glPopAttrib();
}
void DrawSphere() {
	glLoadName(1);
	glPushMatrix();
	glTranslatef(g_pPosition[0], g_pPosition[1], 0.0f);
	glColor3f(1, 0, 0);
	glutSolidSphere(30.0f, 30, 30);
	glPopMatrix();
	ob[0].color = red;
	ob[0].size = 30;

	glLoadName(2);
	glPushMatrix();
	glTranslatef(g_pPosition[2], g_pPosition[3], 0.0f);
	glColor3f(0, 1, 0);
	glutSolidSphere(25.0f, 30, 30);
	glPopMatrix();
	ob[1].color = green;
	ob[1].size = 25;

	glLoadName(3);
	glPushMatrix();
	glTranslatef(g_pPosition[4], g_pPosition[5], 0.0f);
	glColor3f(0, 0, 1);
	glutSolidSphere(20.0f, 30, 30);
	glPopMatrix();
	ob[2].color = blue;
	ob[2].size = 20;
}
void draw(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	DrawBackground();
	DrawSphere();

	glFlush();
	glutSwapBuffers();
}
void resize(int width, int height) {
	g_nGLWidth = width;
	g_nGLHeight = height;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f, -100, 100);
	glMatrixMode(GL_MODELVIEW);
}
void Picking(int x, int y) {
	GLuint selectBuf[256];

	//first step
	glSelectBuffer(256, selectBuf);
	//second step
	glRenderMode(GL_SELECT);
	//third step
	glInitNames();
	glPushName(0);
	//third step

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	//fourth step
	gluPickMatrix(x, y, 10, 10, viewport);

	glOrtho(-g_nGLWidth / 2.0f, g_nGLWidth / 2.0f, -g_nGLHeight / 2.0f, g_nGLHeight / 2.0f, -100, 100);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	DrawSphere();

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glFlush();

	GLint hits;
	//six step
	hits = glRenderMode(GL_RENDER);
	if (hits <= 0)return;

	int stack = selectBuf[0];
	unsigned int color = ob[(int)selectBuf[3]-1].color;
	unsigned int size = ob[(int)selectBuf[3]-1].size;
	g_nSelect = selectBuf[3];


	int index = 3 + stack;
	int i;
	
	for (i = 1; i < hits; i++) {
		stack = selectBuf[index];
		if (is_color == true) {
			if (color < ob[(int)selectBuf[index + 3]-1].color) {
				color = ob[(int)selectBuf[index + 3]-1].color;
				g_nSelect = selectBuf[index + 3];
			}
		}
		else {
			if (size > ob[(int)selectBuf[index + 3]-1].size) {
				size = ob[(int)selectBuf[index + 3]-1].size;
				g_nSelect = selectBuf[index + 3];
			}
		}
		index += 3 + stack;
	}
}
void mouse(int button, int state, int x, int y) {
	y = g_nGLHeight - y;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		Picking(x, y);
		g_nX = x;
		g_nY = y;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		g_nSelect = 0;
	}
}
void motion(int x, int y) {
	y = g_nGLHeight - y;
	if (g_nSelect > 0) {
		g_pPosition[(g_nSelect - 1) * 2 + 0] += x - g_nX;
		g_pPosition[(g_nSelect - 1) * 2 + 1] += y - g_nY;
		g_nX = x;
		g_nY = y;
		glutPostRedisplay();
	}
}
void keyboard(unsigned char key, int x, int y) {
	if (key == 'a') {
		is_color = is_color == true ? false : true;
	}
}
int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(g_nGLWidth, g_nGLHeight);
	glutInitWindowPosition(300, 300);
	glutCreateWindow("Picking");
	init();

	glutDisplayFunc(draw);
	glutReshapeFunc(resize);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	return 0;
}