#include <iostream>
#include <fstream>
using namespace std;

// Состояния автомата
enum EState { S, Ai, Ac, As, Bs, Cs, Ds, E, F };
// Типы лексем (ключевое слово, оператор сравнения, арифметический оператор, идентификатор, константа)
enum ELexType { lKw, lCo, lAo, lId, lVl };

// Вектор лексем
struct Lex {
	ELexType type;
	int index;
	int pos;
	Lex* next;
}
*pFirst = NULL, * pLast = NULL;

// Функция лексического анализа
bool LexAnalysis(const char* text) {
	const char* str = text, * lexstart;
	EState state = S, prevState;
	int add;

	while ((state != E) && (state != F))
	{
		prevState = state;
		add = true;

		switch (state) {
		case S: {
			if (isspace(*str));
			else if (isalpha(*str)) state = Ai;
			else if (isdigit(*str)) state = Ac;
			else if (*str == '<') state = As;
			else if (*str == '=') state = Bs;
			else if ((*str == '+') || (*str == '-')) state = Cs;
			else if (*str == 0) state = F;
			else state = E;
			add = false;
			break;
		}
			  // Идентификаторы и ключевые слова
		case Ai: {
			if (isspace(*str)) state = S;
			else if (isalnum(*str)) add = false;
			else if (*str == '<') state = As;
			else if (*str == '=') state = Bs;
			else if ((*str == '+') || (*str == '-')) state = Cs;
			else if (*str == 0) state = F;
			else { state = E; add = false; }
			break;
		}
			   // Константы
		case Ac: {
			if (isspace(*str)) state = S;
			else if (isdigit(*str)) add = false;
			else if (*str == '<') state = As;
			else if (*str == '=') state = Bs;
			else if ((*str == '+') || (*str == '-')) state = Cs;
			else if (*str == 0) state = F;
			else { state = E; add = false; }
			break;
		}
			   // Сложные операции сравнения (2 символа)
		case As: {
			if (isspace(*str)) state = S;
			else if (isalpha(*str)) state = Ai;
			else if (isdigit(*str)) state = Ac;
			else if ((*str == '=') || (*str == '>'))
			{
				state = Ds; add = false;
			}
			else if (*str == 0) state = F;
			else { state = E; add = false; }
			break;
		}
			   // Присваивание, операции сравнения и арифметические операции
		case Bs: case Cs: case Ds: {
			if (isspace(*str)) state = S;
			else if (isalpha(*str)) state = Ai;
			else if (isdigit(*str)) state = Ac;
			else if (*str == 0) state = F;
			else { state = E; add = false; }
			break;
		}
		}

		// Добавление лексемы в список
		if (add) AddLex(...);
		// Сохранение начала лексемы
		if ((state != prevState) && (state == Ai || state == Ac || state == As || state == Bs || state == Cs))
			lexstart = str;
		// Переход к следующему символу
		if ((state != E) && (state != F)) str++;
	}

	return (state == F);
}