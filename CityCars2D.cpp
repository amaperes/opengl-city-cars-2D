/*
Proiect City Cars 2D  
Perescu Antonela-Madalina 
Grupa 252 
*/

#include <GL/freeglut.h>
#include <iostream>
#include <stdlib.h>

struct coordonate {
	int x, y, a;
} ve[4];

const double PI = 3.141592654;

int window, returnmenu, value = 0;
int xTrans = 0;
int yTrans = 0;

int pozdr = 0, pozst = -300, vehX = 0, stg = 0;
int vehY[4];
int voy = 0, voyd = 0, a2 = 0, n = 0, n4 = 4;

int xAutoTrans = 0;
int yAutoTrans = 400;
int yAutoTrans2 = 0;

bool inDepasire = true;

int frameNumber = 0;

int move = 0;
int nd = 0; /*nr depasiri*/
int gameover = 0;
int start = 0;

void write(char *string)
{
	while (*string)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *string++);
}

void menu(int n) {
	if (n == 0) {
		glutDestroyWindow(window);
		exit(0);
	}
	else { value = n; }
	glutPostRedisplay();
}

void createMenu() {

	returnmenu = glutCreateMenu(menu);
	glutAddMenuEntry("Intro", 1);
	glutAddMenuEntry("Demo", 2);
	glutAddMenuEntry("Play", 3);
	glutAddMenuEntry("Re-Play", 4);
	glutAddMenuEntry("Quit", 0);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}


void drawDisk(double radius) {
	int d;
	glBegin(GL_POLYGON);
	for (d = 0; d < 32; d++) {
		double angle = 2 * PI / 32 * d;
		glVertex2d(radius*cos(angle), radius*sin(angle));
	}
	glEnd();
}

void drawSun() {
	int i, d;
	glColor3f(1.0, 1.0, 0.0);
	for (i = 0; i < 19; i++) { 
		glRotatef(360 / 18, 0, 0, 1); 
		glBegin(GL_LINES);
		glVertex2f(0, 0);
		glVertex2f(200, 0);
		glEnd();
	}
	drawDisk(100);
	glColor3f(0, 0, 0);
}



void init(void)  // initializare fereastra de vizualizare
{

	glClearColor(0.0, 0.7, 1.0, 1.0); // precizeaza culoarea de fond a ferestrei de vizualizare
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION); // se precizeaza este vorba de o reprezentare 2D, realizata prin proiectie ortogonala
	gluOrtho2D(-800.0, 800.0, -600.0, 600.0); // sunt indicate coordonatele extreme ale ferestrei de vizualizare
	glClear(GL_COLOR_BUFFER_BIT); // reprezentare si colorare fereastra de vizualizare

	glClearColor(0.0, 0.7, 1.0, 1.0);
	for (n = 0; n < n4; n++) {
		ve[n].a = 1;
	}
	ve[1].a = 0;

}

void drawRoad() {

	glLineWidth(10);
	glColor3d(0.6, 0.6, 0.6);
	glRecti(-300, -800, 300, 800);

	glBegin(GL_LINES);
	glColor3d(0.4, 0.4, 0.4);
	glVertex2f(300, -800);
	glVertex2f(300, 800);
	glEnd();

	glBegin(GL_LINES);
	glColor3d(0.4, 0.4, 0.4);
	glVertex2f(-300, -800);
	glVertex2f(-300, 800);
	glEnd();

	glEnable(GL_LINE_STIPPLE);
	glLineStipple(3, 0x00FF);
	glLineWidth(5);
	glPushMatrix();
	glTranslated(0.0, move, 0.0);
	glBegin(GL_LINES);
	glColor3d(1.0, 1.0, 1.0);
	glVertex2f(0, -800);
	glVertex2f(0, 800);
	glEnd();
	glPopMatrix();
	glDisable(GL_LINE_STIPPLE);

}

void drawCar() {

	glPushMatrix();
	glColor3d(0.5, 0.9, 0.0);
	glRecti(120, -550, 180, -400);

	glBegin(GL_POLYGON);
	glColor3d(1.0, 0.5, 0.0);
	glVertex2f(120, -400);
	glVertex2f(125, -355);
	glVertex2f(175, -355);
	glVertex2f(180, -400);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3d(0.0, 0.0, 0.0);
	glVertex2f(125, -540);
	glVertex2f(130, -545);
	glVertex2f(170, -545);
	glVertex2f(175, -540);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3d(0.0, 0.0, 0.0);
	glVertex2f(125, -396);
	glVertex2f(130, -360);
	glVertex2f(170, -360);
	glVertex2f(175, -396);
	glEnd();

	glEnable(GL_POINT_SMOOTH);
	glPointSize(4);
	glBegin(GL_POINTS);
	glVertex2f(118, -371);
	glVertex2f(182, -371);
	glEnd();
	glDisable(GL_POINT_SMOOTH);

	glEnd();
	glPopMatrix();
	/*vehX = pozdr;*/

}
void deplj(int vn, int s0, int incrmnt) {     //Pentru Play - masinile de pe stanga (int vn,int s0,int incrmnt) 
											  //(nr.masina , distanta initiala (dintre masini),pt deplasare masina(viteza) 
	ve[vn].y = s0 - incrmnt * 4;
	ve[vn].x = -304;                         /*colona stg*/
	glPushMatrix();
	glTranslated(0, ve[vn].y, 0.0);
	glRotated(180.0, 0.0, 0.0, 1.0);
	drawCar();
	glPopMatrix();
}
void depls(int vn, int s0, int incrmnt) {             //Pentru Play - masinile de pe dreapta
	ve[vn].y = s0 - incrmnt * 2;
	ve[vn].x = 4;                                     /*colona dr*/
	glPushMatrix();
	glTranslated(0, ve[vn].y, 0.0);
	drawCar();
	glPopMatrix();
}

void overComeLogic(void)
{
	if (yAutoTrans < -1200 && xTrans > -300 && xTrans != -300 && inDepasire == true) { // A trecut curu lu ala cu caruta de mine intru in depasire
		glTranslated(xTrans -= 4, yTrans += 1, 0.0);
		glRotated(6.0, 0.0, 0.0, 1.0);
	}
	if (xTrans == -300 && yTrans  < yAutoTrans2 + 300 && inDepasire == true) { //Am intrat in depasire ii dam talpa
		glTranslated(xTrans, yTrans += 5, 0.0);
	}
	if (yTrans > yAutoTrans2 + 200 /*300*/ && xTrans != 0) { //L-am depasit ne incadram inapoi
		if (xTrans == -300) inDepasire = false;
		glTranslated(xTrans += 5, yTrans += 2, 0.0);
		glRotated(-4.0, 0.0, 0.0, 1.0);
	}
	if (xTrans == 0 && inDepasire == false) {
		//Am terminat de depasit plecam in fata
		glTranslated(0.0, yTrans += 2, 0.0);
	}
}

void display(void)
{

	int i;
	char bchr[10];
	if (value == 4) {    //replay
		gameover = 0;
		value = 3;
		voy = 0;    //increment stanga 1-500
		voyd = 0;  //increment dreapta 1-800
		nd = 0;   //nr.depasiri 
		for (n = 0; n < n4; n++) {
			ve[n].a = 1;  //activa
		}
		ve[1].a = 0; //inactiva - numarata
	}
	if (gameover == 0) {
		glClear(GL_COLOR_BUFFER_BIT);

		//soare
		glPushMatrix();
		glTranslated(400, 300, 0);
		glRotated(-frameNumber*0.7, 0.0, 0.0, 1.0);
		drawSun();
		glPopMatrix();

		//nori 
		glPushMatrix();
		glColor3f(0.8, 0.8, 0.8);
		glTranslatef(-400, 300, 0.0);
		glScalef(2.0, 1.0, 1.0);
		drawDisk(50);
		glPopMatrix();

		glPushMatrix();
		glColor3f(0.8, 0.8, 0.8);
		glTranslatef(frameNumber - 200.0, 300.0, 0.0);
		glScalef(3.0, 1.0, 1.0);
		drawDisk(100);
		//glPopMatrix();

		glTranslatef(frameNumber - 300.0, 200.0, 0.0);
		glScalef(2.0, 1.0, 1.0);
		drawDisk(50);

		glTranslatef(100.0, -25.0, 0.0);
		glScalef(1.0, 1.0, 1.0);
		drawDisk(40);

		glTranslatef(250.0, -30.0, 0.0);
		glScalef(1.0, 1.0, 1.0);
		drawDisk(50);
		glTranslatef(-frameNumber - 10.0, 0.0, 0.0);
		glScalef(1.0, 1.0, 1.0);
		drawDisk(10);

		glPopMatrix();

		glColor3f(1.0, 1.0, 1.0);
		glRasterPos2f(-300, -350);
		write("Right Click for Menu");

		if (value == 1) {

			

				glClear(GL_COLOR_BUFFER_BIT);


				//soare 
				glPushMatrix();
				glTranslated(400, 300, 0);
				glRotated(-frameNumber*0.7, 0.0, 0.0, 1.0);
				drawSun();
				glPopMatrix();

				//nori 
				glPushMatrix();
				glColor3f(0.8, 0.8, 0.8);
				glTranslatef(-400, 300, 0.0);
				glScalef(2.0, 1.0, 1.0);
				drawDisk(50);
				glPopMatrix();

				glPushMatrix();
				glColor3f(0.8, 0.8, 0.8);
				glTranslatef(frameNumber - 200.0, 300.0, 0.0);
				glScalef(3.0, 1.0, 1.0);  
				drawDisk(100);
				//glPopMatrix();

				glTranslatef(frameNumber - 300.0, 200.0, 0.0);
				glScalef(2.0, 1.0, 1.0);
				drawDisk(50);

				glTranslatef(100.0, -25.0, 0.0);
				glScalef(1.0, 1.0, 1.0);
				drawDisk(40);

				glTranslatef(250.0, -30.0, 0.0);
				glScalef(1.0, 1.0, 1.0);
				drawDisk(50);
				glTranslatef(-frameNumber - 10.0, 0.0, 0.0);
				glScalef(1.0, 1.0, 1.0);
				drawDisk(10);

				glPopMatrix();


			}


		if (value == 2) {

			glClear(GL_COLOR_BUFFER_BIT);

			drawRoad();

			move = move - 5;
			if (move < -50)
				move = 0;

			glPushMatrix();
			glTranslated(0.0, yAutoTrans2++, 0.0); //Presupun ca atat eu cat si ala din fata mergem cu aceeasi viteza
			glPushMatrix();

			overComeLogic();
			drawCar();                       //Masina care depaseste
			glPopMatrix();

			glPushMatrix();
			glTranslated(0.0, 300.0, 0.0);
			drawCar();                       //Masina din fata
			glPopMatrix();
			glPopMatrix();

			glPushMatrix();
			glTranslated(0.0, yAutoTrans -= 10, 0.0);
			glPushMatrix();
			glRotated(180.0, 0.0, 0.0, 1.0);
			drawCar();                                             //Masina sens opus
			glPopMatrix();

			glPopMatrix();

			glColor3f(1.0, 1.0, 1.0);
			glRasterPos2f(-750, 350);
			write("Right Click for Menu");
		}

		if (value == 3) {

			glClear(GL_COLOR_BUFFER_BIT);
			/*Deplasare stanga-dreapta masina*/
			drawRoad();
			if (stg == 0) {
				if (vehX <= pozdr) {
					glPushMatrix();
					glTranslated(vehX += 4, 0, 0);
					drawCar();
					glPopMatrix();
				}
				else {
					glPushMatrix();
					glTranslated(vehX, 0, 0);
					drawCar();
					glPopMatrix();
				}
			}
			else
			{
				if (pozst <= vehX) {
					glPushMatrix();
					glTranslated(vehX -= 4, 0, 0);
					drawCar();
					glPopMatrix();
				}

				else {
					glPushMatrix();
					glTranslated(vehX, 0, 0);
					drawCar();
					glPopMatrix();
				}
			}                                        /*Deplasare stanga-dreapta masina*/


			deplj(0, 200, voy += 1);   // prima masina de pe stanga 

			if (a2 == 1 && ve[1].a == 1)
				deplj(1, 500, voy);

			deplj(2, 800, voy);
			depls(3, 1200, voyd += 1);

			if (voy > 500 && ve[2].a == 0) {
				voy = 0;
				ve[0].a = 1;
				ve[2].a = 1;
				ve[0].y = 0;
				ve[2].y = 0;
				if (rand() % 2 == 0)
				{
					a2 = 1;
					ve[1].a = 1;
				}
				else
				{
					a2 = 0;
					ve[1].a = 0;
					ve[1].y = 0;
				}
			}
			if (voyd > 800 && ve[3].a == 0) {
				voyd = 0;
				ve[3].a = 1;
			}

			//coliziune

			for (n = 0; n < n4; n++) {
				if (ve[n].a > 0) {          //daca a depasit masina
					if (ve[n].y<-1100 && vehX>-240 && ve[n].x == -304)
					{
						/*nd += 1;*/
						ve[n].a = 0;
					}
					if (ve[n].y < -200 && vehX < -54 && ve[n].x == 4)
					{
						nd += 1;
						ve[n].a = 0;
					}

					if (((ve[n].y < -700 && vehX < -240 && ve[n].x == -304) && (ve[n].y>-1100 && vehX < -240 && ve[n].x == -304))
						||
						((ve[n].y < 200 && vehX > -54 && ve[n].x == 4) && (ve[n].y>-200 && vehX > -54 && ve[n].x == 4)))
						gameover = 1;
				}
			};

			glColor3f(1.0, 1.0, 1.0);
			glRasterPos2f(-750, 450);
			write("depasiri:");
			_itoa_s(nd, bchr, 10);
			write(bchr);

			glColor3f(1.0, 1.0, 1.0);
			glRasterPos2f(-750, 60);
			write("Instructiuni:");
			glRasterPos2f(-750, 30);
			write(" - sageti LEFT & RIGHT ");
			glRasterPos2f(-750, 0);
			write(" - evitare coliziune");


			move = move - 5;
			if (move < -50)
				move = 0;
		}


	}
	else
	{
		glRasterPos2f(-750, 350);
		write("Game Over");
	}

	glutPostRedisplay();
	glutSwapBuffers();

	glFlush();

}




void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-800.0, 800.0, -600.0, 600.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void doFrame(int v) {
	frameNumber++;
	glutPostRedisplay();
	glutTimerFunc(30, doFrame, 0);
}

void Specialkey(int key, int x, int y) {
	switch (key)
	{
	case GLUT_KEY_LEFT:
		stg = 1;
		break;

	case GLUT_KEY_RIGHT:
		stg = 0;
		break;
	}
}
int main(int argc, char** argv)
{
	glutInit(&argc, argv); // initializare GLUT
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); // se utilizeaza un singur buffer | modul de colorare RedGreenBlue (= default)
	glutInitWindowPosition(100, 100); // pozitia initiala a ferestrei de vizualizare (in coordonate ecran)
	glutInitWindowSize(1000, 800); // dimensiunile ferestrei
	window = glutCreateWindow("City Cars 2D");
	init();
	createMenu();
	glutDisplayFunc(display);
	glutTimerFunc(200, doFrame, 0);
	glutSpecialFunc(Specialkey);
	glutReshapeFunc(reshape);
	glutMainLoop();

	return 0;

}

