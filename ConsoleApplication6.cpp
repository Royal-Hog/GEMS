#include <iostream>
#include <GL/glut.h>
#include <vector>




using namespace std;
// Константы для игры
const int BOARD_SIZE = 8; // Размер игрового поля
const int NUM_ROWS = 8;
const int NUM_COLS = 8;
const int SQUARE_SIZE = 50; // Размер квадратов на игровом поле
const int BONUS_RADIUS = 3;
struct Square {
	int x; // Позиция по горизонтали
	int y; // Позиция по вертикали
	int color; // Цвет квадрата
	bool selected; // Выбран ли квадрат пользователем
	bool removed; // Был ли квадрат удален из игры
};

// Массив для хранения информации обо всех квадратах на игровом поле
vector<Square> squares(BOARD_SIZE* BOARD_SIZE);

// Функция для отрисовки квадрата на игровом поле
void drawSquare(int x, int y, int color, bool selected) {
	// Выбираем цвет для квадрата
	if (selected) {
		glColor3f(1.0, 1.0, 1.0); // Белый цвет для выделенного квадрата
	}
	else {
		switch (color) {
		case 0: // Красный цвет
			glColor3f(1.0, 0.0, 0.0);
			break;
		case 1: // Зеленый цвет
			glColor3f(0.0, 1.0, 0.0);
			break;
		case 2: // Синий цвет
			glColor3f(0.0, 0.0, 1.0);
			break;
			// Добавьте другие цвета здесь
		default:
			break;
		}
	}

	// Отрисовываем квадрат
	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x + SQUARE_SIZE, y);
	glVertex2f(x + SQUARE_SIZE, y + SQUARE_SIZE);
	glVertex2f(x, y + SQUARE_SIZE);
	glEnd();
}

// Функция для отрисовки игрового поля
void drawBoard() {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			Square square = squares[i * BOARD_SIZE + j];
			drawSquare(square.x, square.y, square.color, square.selected);
		}
	}
}

void generateBonus(int color) {
	// Генерируем случайное число от 0 до 99
	int r = rand() % 100;
	if (r < 30) {
		// 30% вероятность выпадения бонуса "Перекраска"
		int count = 0;
		int bonusIndex = -1;
		// Ищем квадраты в окрестности радиуса 3, которые можно перекрасить
		for (int i = 0; i < BOARD_SIZE; i++) {
			for (int j = 0; j < BOARD_SIZE; j++) {
				if (abs(i - BOARD_SIZE / 2) <= 3 && abs(j - BOARD_SIZE / 2) <= 3) {
					Square& square = squares[i * BOARD_SIZE + j];
					if (!square.removed && square.color != color) {
						// Квадрат не удален и его цвет не совпадает с цветом бонуса
						count++;
						if (rand() % count == 0) {
							bonusIndex = i * BOARD_SIZE + j;
						}
					}
				}
			}
		}
		if (bonusIndex >= 0) {
			// Нашли квадрат, который нужно перекрасить, и его двух непосредственных соседей
			Square& bonusSquare = squares[bonusIndex];
			int firstNeighborIndex = -1;
			int secondNeighborIndex = -1;
			for (int i = -1; i <= 1; i++) {
				for (int j = -1; j <= 1; j++) {
					int k = bonusIndex / BOARD_SIZE + i;
					int l = bonusIndex % BOARD_SIZE + j;
					if (k >= 0 && k < BOARD_SIZE && l >= 0 && l < BOARD_SIZE
						&& !(k == bonusIndex / BOARD_SIZE && l == bonusIndex % BOARD_SIZE)
						&& abs(k - bonusIndex / BOARD_SIZE) <= 1
						&& abs(l - bonusIndex % BOARD_SIZE) <= 1
						&& !squares[k * BOARD_SIZE + l].removed
						&& squares[k * BOARD_SIZE + l].color != color) {
						if (firstNeighborIndex < 0) {
							firstNeighborIndex = k * BOARD_SIZE + l;
						}
						else if (secondNeighborIndex < 0) {
							secondNeighborIndex = k * BOARD_SIZE + l;
						}
					}
				}
			}
			if (firstNeighborIndex >= 0 && secondNeighborIndex >= 0) {
				// Нашли двух соседей, перекрашиваем их и квадрат бонуса в цвет бонуса
				squares[firstNeighborIndex].color = color;
				squares[secondNeighborIndex].color = color;
				bonusSquare.color = color;
			}
		}
	}

	else if (r < 50) {
		vector<int> bonusIndices;
		int count = 0;

		// Ищем квадраты, которые можно удалить
		for (int i = 0; i < BOARD_SIZE; i++) {
			for (int j = 0; j < BOARD_SIZE; j++) {
				if (!squares[i * BOARD_SIZE + j].removed && squares[i * BOARD_SIZE + j].color != color) {
					// Квадрат не удален и его цвет не совпадает с цветом бонуса
					count++;
					bonusIndices.push_back(i * BOARD_SIZE + j);
				}
			}
		}
		if (count > 0) {
			// Нашли квадраты, которые нужно удалить
			for (int i = 0; i < 5; i++) {
				int index = bonusIndices[rand() % count];
				squares[index].removed = true;
			}
		}
	}
}

void shiftSquaresDown() {
	for (int i = BOARD_SIZE - 1; i >= 0; i--) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			Square& square = squares[i * BOARD_SIZE + j];
			if (square.removed) {
				// Квадрат был удален, ищем ближайший непустой квадрат выше
				int k = i - 1;
				while (k >= 0 && squares[k * BOARD_SIZE + j].removed) {
					k--;
				}
				if (k >= 0) {
					// Нашли непустой квадрат, сдвигаем текущий квадрат на его место
					Square& destSquare = squares[k * BOARD_SIZE + j];
					destSquare.color = square.color;
					destSquare.removed = false;
					square.removed = true;
				}
			}
		}
	}
}

int removeSequences() {
	int removedCount = 0;
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			Square& square = squares[i * BOARD_SIZE + j];
			if (!square.removed) {
				// Находим все соседние квадраты того же цвета
				std::vector<Square*> sequence;
				sequence.push_back(&square);
				for (int k = i - 1; k <= i + 1; k++) {
					for (int l = j - 1; l <= j + 1; l++) {
						if (k >= 0 && k < BOARD_SIZE
							&& l >= 0
							&& l < BOARD_SIZE
							&& !(k == i && l == j)
							&& !squares[k * BOARD_SIZE + l].removed
							&& squares[k * BOARD_SIZE + l].color == square.color) {
							sequence.push_back(&squares[k * BOARD_SIZE + l]);
						}
					}
				}
				if (sequence.size() >= 3) {
					// Найдена последовательность одноцветных квадратов, удаляем их
					for (Square* seqSquare : sequence) {
						seqSquare->removed = true;
						removedCount++;
					}
				}
			}
		}
	}
	return removedCount;
}

// Функция для обработки событий кликов мыши
void mouseClick(int button, int state, int x, int y) {
	// Вычисляем позицию клика в координатах игрового поля
	int i = x / SQUARE_SIZE;
	int j = y / SQUARE_SIZE;

	// Обрабатываем только клики в пределах игрового поля
	if (i >= 0 && i < BOARD_SIZE && j >= 0 && j < BOARD_SIZE) {
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			// Пользователь кликнул на квадрат
			Square& square = squares[j * BOARD_SIZE + i];
			if (square.selected) {
				// Пользователь уже выбрал этот квадрат, снимаем выделение
				square.selected = false;
			}
			else {
				for (int k = 0; k < BOARD_SIZE * BOARD_SIZE; k++) {
					if (squares[k].selected) {
						squares[k].selected = false;
						break;
					}
				}
				// Выделяем новый выбранный квадрат
				square.selected = true;
			}
		}
		else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
			// Пользователь кликнул правой кнопкой мыши, пытаемся поменять местами два выбранных квадрата
			int selectedCount = 0;
			Square* selectedSquares[2];
			for (int k = 0; k < BOARD_SIZE * BOARD_SIZE; k++) {
				if (squares[k].selected) {
					selectedSquares[selectedCount++] = &squares[k];
					if (selectedCount == 2) {
						// Меняем местами два выбранных квадрата
						int tempColor = selectedSquares[0]->color;
						selectedSquares[0]->color = selectedSquares[1]->color;
						selectedSquares[1]->color = tempColor;
						// Снимаем выделение с выбранных квадратов
						selectedSquares[0]->selected = false;
						selectedSquares[1]->selected = false;
						// Проверяем, образовалась ли последовательность одноцветных квадратов
						int removedCount = removeSequences();
						if (removedCount > 0) {
							// Были удалены некоторые квадраты, сдвигаем оставшиеся квадраты вниз
							shiftSquaresDown();
							// Проверяем, образовалась ли еще какая-то последовательность одноцветных квадратов
							while (removeSequences() > 0) {
								// Были удалены некоторые квадраты, сдвигаем оставшиеся квадраты вниз
								shiftSquaresDown();
							}
							// Генерируем бонус, если был удален хотя бы один квадрат
							if (removedCount > 1) {
								generateBonus(selectedSquares[0]->color);
							}
						}
						break;
					}
				}
			}
		}
	}
}

// Функция для проверки, образовалась ли на игровом поле последовательность одноцветных квадратов


// Функция для сдвига оставшихся квадратов вниз

// Функция для генерации бонуса


// Функция для проверки, остались ли на поле еще квадраты одного цвета
bool hasSameColorSquares() {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			Square& square = squares[i * BOARD_SIZE + j];
			if (!square.removed) {
				// Квадрат не удален, ищем квадраты того же цвета вокруг
				for (int k = -1; k <= 1; k++) {
					for (int l = -1; l <= 1; l++) {
						if (k != 0 || l != 0) {
							int x = i + k;
							int y = j + l;
							if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE
								&& !squares[x * BOARD_SIZE + y].removed
								&& squares[x * BOARD_SIZE + y].color == square.color) {
								// Нашли квадрат того же цвета
								return true;
							}
						}
					}
				}
			}
		}
	}
	// Не нашли квадраты одного цвета
	return false;
}

// Функция для обработки нажатий клавиш на клавиатуре





void init() {
	// Устанавливаем цвет фона
	glClearColor(1, 0.5, 0.5, 0.0);

	// Устанавливаем параметры OpenGL
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Устанавливаем параметры GLUT

	//glutInitWindowPosition(windowX, windowY);
}

int main(int argc, char** argv) {
	// Инициализируем GLUT и создаем окно
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(500, 500);
	glutCreateWindow("GEMS");

	// Регистрируем обработчики событий GLUT
	glutDisplayFunc(drawBoard);
	glutMouseFunc(mouseClick);

	//glutTimerFunc(1000 / 60, update, 0);

	// Инициализируем игру
	init();

	// Запускаем главный цикл обработки событий GLUT
	glutMainLoop();
	return 0;
}