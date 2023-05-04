#include <iostream>
#include <cstdlib>
#include <ctime>
#include <GL/glut.h>
#include <vector>
#include<Windows.h>
#include <cmath>

/*перед началом игры нажать на любую клавишу на клавиатуре, чтобы поменять квадраты местами - нажать левой кнопкой мыши на первый, потом на второй, потом третий раз в случайное место. Черные и серые квадраты - бонусы*/

using namespace std;

int cells[8][8];

int xc1=0;
int xc2 = 0;
int yc1=0;
int yc2 = 0;;
int clic=0;
int buf;
int c=0;
int ic;
int coun;
int rando;
int randx[4];
int randy[4];
int randcount;
void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			switch (cells[i][j]%10){
			case 0: glColor3f(0.0f, 1.0f, 0.0f);
				break;
			case 1: glColor3f(1.0f, 0.0f, 0.0f);
				break;
			case 2: glColor3f(0.0f, 0.0f, 1.0f);
				break;
			case 3: glColor3f(1.0f, 0.0f, 1.0f);
				break;
			case 4: glColor3f(1.0f, 1.0f, 0.0f);
				break;


			}
			glBegin(GL_QUADS);
			glVertex2d(i*0.1, j*0.1);
			glVertex2d(i*0.1+0.1, j*0.1);
			glVertex2d(i*0.1+0.1, j*0.1+0.1);
			glVertex2d(i*0.1, j*0.1 + 0.1);
			glEnd();
	
			if (cells[i][j]/10 == 1) {
				glColor3f(0.0f, 0.0f, 0.0f);
				glBegin(GL_QUADS);
				glVertex2d(i * 0.1 + 0.02, j * 0.1 + 0.02);
				glVertex2d(i * 0.1 + 0.08, j * 0.1 + 0.02);
				glVertex2d(i * 0.1 + 0.08, j * 0.1 + 0.08);
				glVertex2d(i * 0.1 + 0.02, j * 0.1 + 0.08);
				glEnd();
			}
			if (cells[i][j]/10 == 2) {
				glColor3f(0.5f, 0.5f, 0.5f);
				glBegin(GL_QUADS);
				glVertex2d(i * 0.1 + 0.02, j * 0.1 + 0.02);
				glVertex2d(i * 0.1 + 0.08, j * 0.1 + 0.02);
				glVertex2d(i * 0.1 + 0.08, j * 0.1 + 0.08);
				glVertex2d(i * 0.1 + 0.02, j * 0.1 + 0.08);
				glEnd();
			}

		}
	}

	

	if (clic == 1) {
		glColor3f(1.0f,1.0f, 1.0f);
		glBegin(GL_QUADS);
		glVertex2d(xc1 * 0.1+0.02, yc1 * 0.1+0.02);
		glVertex2d(xc1 * 0.1 + 0.08, yc1 * 0.1+0.02);
		glVertex2d(xc1 * 0.1 + 0.08, yc1 * 0.1 + 0.08);
		glVertex2d(xc1 * 0.1+0.02, yc1 * 0.1 + 0.08);
		glEnd();
	}
	if (clic == 2) {
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
		glVertex2d(xc1 * 0.1 + 0.02, yc1 * 0.1 + 0.02);
		glVertex2d(xc1 * 0.1 + 0.08, yc1 * 0.1 + 0.02);
		glVertex2d(xc1 * 0.1 + 0.08, yc1 * 0.1 + 0.08);
		glVertex2d(xc1 * 0.1 + 0.02, yc1 * 0.1 + 0.08);
		glEnd();

		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
		glVertex2d(xc2 * 0.1+0.02, yc2 * 0.1+0.02);
		glVertex2d(xc2 * 0.1 + 0.08, yc2 * 0.1+0.02);
		glVertex2d(xc2 * 0.1 + 0.08, yc2 * 0.1 + 0.08);
		glVertex2d(xc2 * 0.1+0.02, yc2 * 0.1 + 0.08);
		glEnd();
		glColor3f(1.0f, 1.0f, 1.0f);
	
		
	}
	
	glutSwapBuffers();
}

void check(unsigned char q,int x, int y){
	c=0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (i<6 && cells[i][j]%10 == cells[i + 1][j]%10 && cells[i][j]%10 == cells[i + 2][j]%10) {
				ic = i ;
				while(ic<8 && cells[i][j]%10==cells[ic][j]%10){
					
					
					if (cells[ic][j] / 10 == 1) {

						
						randx[0] = rand() % 8;
						randy[0] = rand() % 8;


						while ((abs(randx[0] - ic) < 2 && abs(randy[0] - j) <2 )||(cells[randx[0]][randy[0]]%10== cells[ic][j] % 10)) {
							randx[0] = rand() % 8;
							randy[0] = rand() % 8;
						}
						cells[randx[0]][randy[0]] = cells[ic][j] % 10;

						randx[1] = rand() % 8;
						randy[1] = rand() % 8;


						while ((abs(randx[1] - ic) < 2 && abs(randy[1] - j) < 2)||((randx[1]==randx[0])&&(randy[1]==randy[0])) || (cells[randx[1]][randy[1]] % 10 == cells[ic][j] % 10)) {
							randx[1] = rand() % 8;
							randy[1] = rand() % 8;
						}
						cells[randx[1]][randy[1]] = cells[ic][j] % 10;

					}
					if (cells[ic][j] / 10 == 2) {
						randx[0] = rand() % 8;
						randy[0] = rand() % 8;
						while (randx[0] == ic && randy[0] == j) {
							randx[0] = rand() % 8;
							randy[0] = rand() % 8;
						}
						for (int n = randy[0]; n < 7; n++) {
							cells[randx[0]][n] = cells[randx[0]][n + 1];

						}
						
						cells[randx[0]][7] = rand() % 5;
						for (int k = 1; k < 4; k++) {

							randx[k] = rand() % 8;
							randy[k] = rand() % 8;
						
							while (true) {
								randcount = 0;
								for (int a = 0; a < k; a++) {
									if (randx[a] != randx[k] || randy[a] != randy[k]) {
										randcount++;
									}
								}
								if (randcount == k)break;
								randx[k] = rand() % 8;
								randy[k] = rand() % 8;
							}
							for (int n = randy[k]; n < 7; n++) {
								cells[randx[k]][n] = cells[randx[k]][n + 1];

							}
							cells[randx[k]][7] = rand() % 5;

						}


					}
					ic++;
				}
				
			
				for (int k = j; k < 7; k++) {
					for (int n=i;n<ic;n++)
					cells[n][k] = cells[n][k + 1];
					
				}
				
				cells[i][7] = rand() % 5;
				cells[i + 1][7] = rand() % 5;
				cells[i + 2][7] = rand() % 5;
			


				rando = rand() % 20;
				if (rando == 0) {
					cells[(ic + i) / 2][j] += 10;
				}
				if (rando == 1) {
					cells[(ic + i) / 2][j] += 20;
				}
				renderScene();
				Sleep(2000);
				i = 0;
				j = 0;
				c++;
			}
			
			if (j<6 && cells[i][j]%10 == cells[i][j+1]%10 && cells[i][j]%10 == cells[i][j+2]%10) {
				ic = j;
				coun = 0;
				while (ic < 8 && cells[i][j]%10 == cells[i][ic]%10) {

					if (cells[i][ic] / 10 == 1) {


						randx[0] = rand() % 8;
						randy[0] = rand() % 8;


						while ((abs(randx[0] - i) < 2 && abs(randy[0] - ic) < 2) || (cells[randx[0]][randy[0]] % 10 == cells[i][ic] % 10)) {
							randx[0] = rand() % 8;
							randy[0] = rand() % 8;
						}
						cells[randx[0]][randy[0]] = cells[i][ic] % 10;

						randx[1] = rand() % 8;
						randy[1] = rand() % 8;


						while ((abs(randx[1] - i) < 2 && abs(randy[1] - ic) < 2) || ((randx[1] == randx[0]) && (randy[1] == randy[0])) || (cells[randx[1]][randy[1]] % 10 == cells[i][ic] % 10)) {
							randx[1] = rand() % 8;
							randy[1] = rand() % 8;
						}
						cells[randx[1]][randy[1]] = cells[i][ic] % 10;

					}

					if (cells[i][ic] / 10 == 2) {
						randx[0] = rand() % 8;
						randy[0] = rand() % 8;
						while (randx[0] == i && randy[0] == ic) {
							randx[0] = rand() % 8;
							randy[0] = rand() % 8;
						}
						for (int n = randy[0]; n < 7; n++) {
							cells[randx[0]][n] = cells[randx[0]][n + 1];

						}

						cells[randx[0]][7] = rand() % 5;
						for (int k = 1; k < 4; k++) {

							randx[k] = rand() % 8;
							randy[k] = rand() % 8;

							while (true) {
								randcount = 0;
								for (int a = 0; a < k; a++) {
									if (randx[a] != randx[k] || randy[a] != randy[k]) {
										randcount++;
									}
								}
								if (randcount == k)break;
								randx[k] = rand() % 8;
								randy[k] = rand() % 8;
							}
							for (int n = randy[k]; n < 7; n++) {
								cells[randx[k]][n] = cells[randx[k]][n + 1];

							}
							cells[randx[k]][7] = rand() % 5;

						}


					}



					ic++;
					coun++;
				}
				
				for (int k = j; k < 8-coun; k++) {
					cells[i][k] = cells[i][k + coun];
					
				}
				for(int n=0;n<coun;n++) cells[i][7-n] = rand() % 5;

				rando = rand() % 20;
				if (rando == 0) {
					cells[i][(ic + j) / 2] += 10;
				}
				if (rando == 1) {
					cells[i][(ic + j) / 2] += 20;
				}
			
				renderScene();
				Sleep(2000);
				i = 0;
				j = 0;
				c++;

			}
		}
	}
}
void click(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		switch (clic) {
		case 0:clic = 1;
			break;
		case 1:clic = 2;
			break;
		case 2:clic = 0;
			break;
		}

			if (clic == 1) {

				xc1 = (x - 500) / 50;
				yc1 = (450 - y) / 50;

				renderScene();

			}

			if (clic == 2) {
				xc2 = (x - 500) / 50;
				yc2 = (450 - y) / 50;

				renderScene();
			}
			if(clic==0){
				if ((abs(xc1 - xc2) == 1 && abs(yc1 - yc2) == 0) || (abs(xc1 - xc2) == 0 && abs(yc1 - yc2) == 1)) {
					buf = cells[xc1][yc1];
					cells[xc1][yc1] = cells[xc2][yc2];
					cells[xc2][yc2] = buf;


					renderScene();
					Sleep(2000);
					check(GLUT_KEY_DOWN, 1, 1);


					if (c == 0) {
						buf = cells[xc1][yc1];
						cells[xc1][yc1] = cells[xc2][yc2];
						cells[xc2][yc2] = buf;
						renderScene();
					}
				}
				else {
					renderScene();
				}

			}
		
	}
}
int main(int argc, char** argv) {

	srand(time(0));
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			cells[i][j]= rand() % 5;
			

		}
	}

	// инициализация
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1000, 1000);
	glutCreateWindow("Урок 1");

	// регистрация обратных вызовов
	glutDisplayFunc(renderScene);
	glutKeyboardFunc(check);
	glutMouseFunc(click);

	// Основной цикл GLUT
	glutMainLoop();

	return 1;
}