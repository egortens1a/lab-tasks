#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

enum lex_type { NUM, LETTER, LEES, MORE, EQUALLY, ARITH, SEP, SC, OTHER };//группы символов
enum state { S, VL0, ID0, CO0, CO1, WL1, CO2, EQ0, AO0, sSC0, EQ, AO, VL, ID, CO, WL, KW, sSC };//состояния

struct Lex {
	char* word;
	state Type;
};

state matrix[11][9] =
{//  0-9 |A-z | < |  >  | = | +- |' '|  ; | Other
	{VL0, ID0, CO0, CO1, EQ0, AO0, S, sSC0, WL1},// S
	{VL0, WL1, VL,  VL,  VL,  VL, VL, VL, WL1},//  VL0
	{ID0, ID0, ID,  ID,  ID,  ID, ID, ID, WL1},//  ID0
	{CO,  CO,  CO,  CO2, CO2, CO, CO, CO, WL1 },// CO0
	{CO,  CO,  CO,  CO,  CO2, CO, CO, CO, WL1 },// CO1
	{WL1, WL1, WL,  WL,  WL,  WL, WL, WL,WL1},//   WL1
	{CO,  CO,  CO,  CO,  CO,  CO, CO, CO, WL1 },// CO2
	{EQ,  EQ,  EQ,  EQ,  EQ,  EQ, EQ, EQ, WL1 },// EQ0
	{AO,  AO,  AO,  AO,  AO,  AO, AO, AO, AO }, // AO0
	{sSC ,sSC ,sSC ,sSC ,sSC ,sSC ,sSC ,sSC ,sSC }//sSC0
};

lex_type check(char c)// выделяем группы символов
{
	if (isdigit(c)) return NUM;
	if (isalpha(c)) return LETTER;
	if (c == '<') return LEES;
	if (c == '>') return MORE;
	if (c == '=') return EQUALLY;
	if (c == '+' || c == '-' || c == '*' || c == '/') return ARITH;
	if (c == ';') return SC;
	if (c == ' ' || c == '\n' || c == '\t' || c == '\0') return SEP;
	return OTHER;
}

void LexAnalysis(char* text, vector<Lex>& res)
{
	const char* kw[6] = { "for", "to", "next", "and", "or", "not" };//массив ключевых лексем
	unsigned begin = 0;//первый символ слова
	state current_state = S; //начальное состояние
	Lex lex;
	int i = 0;//текущая позиция в тексте
	while (text[i])
	{
		--i;//каждый раз при проходе лексемы мы останавливаемся на следующей после неё символе, чтобы понять, что она закончилась
		//для того, чтобы проверить ее принадлежность к следующей лексеме и не пропустить его, нужно вернуться на один символ назад
		while (current_state != EQ && current_state != AO && current_state != VL && current_state != ID && current_state != CO && current_state != WL && current_state != sSC)
		{//читаем слово пока не встретим какое то из конечных состояний
			++i;
			if (current_state == S) begin = i;//скипаем нечитаемые лексемы
			current_state = matrix[current_state][check(text[i])];//переходим по состояниям
		}

		int length = i - begin;//длина лексемы
		lex.word = new char[length + 1];//копируем лексему
		for (int l = 0; l < length; ++l)
		{
			lex.word[l] = text[l + begin];
		}
		lex.word[length] = '\0';//детерминирующий 0

		if (current_state == VL && (atoi(lex.word) > 32768)) current_state = WL;//проверяем размер констант
		if (current_state == VL && (atoi(lex.word) > 32767 && (!res.size() || *res[res.size() - 1].word != '-'))) current_state = WL; //для отрицаельных отдельно
		lex.Type = current_state;

		if (lex.Type == ID)//уточняем идентефикаторы
		{
			for (int j = 0; j < 6; ++j)//сравниваем с ключевыми словами
			{
				if (!(strcmp(lex.word, kw[j])))//если слова совпадают
					lex.Type = KW;
			}
			if (length > 5) lex.Type = WL;//если длина больше 5 - некорректная лексема
		}

		res.push_back(lex);//добавляем лексему
		current_state = S;//обновляем состояние
		begin = i;
		while (text[i] == ' ' || text[i] == '\n' || text[i] == '\t') ++i;//скипаем разделители лишние
	}
}

void print(vector<Lex>& res, ostream& out) {
	if (out && res.size())
	{
		const char* types[8] = { "eq", "ao", "vl", "id", "co", "wl", "kw","sc" };//список типов
		for (unsigned long i = 0; i < res.size(); ++i)
		{
			out << res[i].word << '[' << types[res[i].Type - 10] << "] ";
			cout << res[i].word << '[' << types[res[i].Type - 10] << "] ";
		}
		out << '\n';
		cout << '\n';
	}
}
void printID(vector<Lex>& res, ostream& out) {

	if (out && res.size()) {
		for (unsigned long i = 0; i < res.size(); ++i)
		{
			if (res[i].Type == ID)//если совпадает - выводим
			{
				out << res[i].word << ' ';
				cout << res[i].word << ' ';
			}
		}
		out << '\n';
		cout << '\n';
	}
}
void printCO(vector<Lex>& res, ostream& out) {

	if (out && res.size()) {
		for (unsigned long i = 0; i < res.size(); ++i)
		{
			if (res[i].Type == VL)//если совпадает - выводим
			{
				out << res[i].word << ' ';
				cout << res[i].word << ' ';
			}
		}
		out << '\n';
		cout << '\n';
	}
}

int main()
{
	ifstream fin("input.txt");
	if (fin)
	{
		fin.seekg(0, ios::end);//считаем длину текста
		unsigned size = 1 + fin.tellg();
		fin.seekg(0, ios::beg);

		char* text = new char[size + 1];
		fin.getline(text, size, '\0');

		vector<Lex> res; LexAnalysis(text, res);//читаем

		ofstream fout("output.txt");

		print(res, fout);
		printID(res, fout);
		printCO(res, fout);

		fout.close();
		delete[] text;
	}
	fin.close();
}