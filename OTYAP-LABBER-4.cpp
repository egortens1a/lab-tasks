#include <fstream>
#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

enum lex_type { NUM, LETTER, LEES, MORE, EQUALLY, ARITH1, ARITH, SEP, OTHER };//группы символов
enum state { S, VL0, ID0, CO0, CO1, WL1, CO2, EQ0, AO0, AO01, EQ, AO1, AO, VL, ID, CO, WL, FO, TO, NE };//состояния переходов ЛА + типы лексем
enum S_state {
	Start, A, B, C, C1, D, E, G, H1, H, I, I1, K, L, L1, M, N, Z,//состояния для переходов СА
	wr_FO, wr_ID, wr_EQ, wr_VL, wr_AO, wr_IDVL, wr_AOTO, wr_AOID, wr_AONE, wr_NE, NEXT
};
S_state s_matrix[17][10] =
{//     eq      ao1(-)    ao	   vl	    id	     co	      wl	  for	   to	    next
	{  wr_FO,   wr_FO,   wr_FO,   wr_FO,   wr_FO,   wr_FO,   wr_FO,       A,   wr_FO,   wr_FO},//S 0
	{  wr_ID,   wr_ID,   wr_ID,   wr_ID,       B,   wr_ID,   wr_ID,   wr_ID,   wr_ID,   wr_ID},//A 1
	{      C,   wr_EQ,   wr_EQ,   wr_EQ,   wr_EQ,   wr_EQ,   wr_EQ,   wr_EQ,   wr_EQ,   wr_EQ},//b 2
	{wr_IDVL,      C1, wr_IDVL,       D,       D, wr_IDVL, wr_IDVL, wr_IDVL, wr_IDVL, wr_IDVL},//C 3
	{  wr_VL,   wr_VL,   wr_VL,       D,   wr_VL,   wr_VL,   wr_VL,   wr_VL,   wr_VL,   wr_VL},//C1 4
	{wr_AOTO,       E,       E, wr_AOTO, wr_AOTO, wr_AOTO, wr_AOTO, wr_AOTO,       G, wr_AOTO},//D 5
	{wr_IDVL, wr_IDVL, wr_IDVL,       D,       D, wr_IDVL, wr_IDVL, wr_IDVL, wr_IDVL, wr_IDVL},//E 6
	{wr_IDVL,      H1, wr_IDVL,       H,       H, wr_IDVL, wr_IDVL, wr_IDVL, wr_IDVL, wr_IDVL},//G 7
	{  wr_VL,   wr_VL,   wr_VL,       H,   wr_VL,   wr_VL,   wr_VL,   wr_VL,   wr_VL,   wr_VL},//H1 8
	{wr_AOID,       I,       I, wr_AOID,       K, wr_AOID, wr_AOID, wr_AOID, wr_AOID, wr_AOID},//H 9
	{wr_IDVL, wr_IDVL, wr_IDVL,       I1,     I1, wr_IDVL, wr_IDVL, wr_IDVL, wr_IDVL, wr_IDVL},//I 10
	{wr_AOID,       I,       I, wr_AOID,       K, wr_AOID, wr_AOID, wr_AOID, wr_AOID, wr_AOID},//I1 11
	{      L,   wr_EQ,   wr_EQ,   wr_EQ,   wr_EQ,   wr_EQ,   wr_EQ,   wr_EQ,   wr_EQ,   wr_EQ},//K 12
	{wr_IDVL,      L1, wr_IDVL,       M,       M, wr_IDVL, wr_IDVL, wr_IDVL, wr_IDVL, wr_IDVL},//L 13
	{  wr_VL,   wr_VL,   wr_VL,       M,   wr_VL,   wr_VL,   wr_VL,   wr_VL,   wr_VL,   wr_VL},//L1 14
	{wr_AONE,       N,       N, wr_AONE, wr_AONE, wr_AONE, wr_AONE, wr_AONE, wr_AONE,    NEXT},//M 15
	{wr_IDVL, wr_IDVL, wr_IDVL,       M,       M, wr_IDVL, wr_IDVL, wr_IDVL, wr_IDVL, wr_IDVL}//N 16
};

struct Lex {
	char* word;
	state Type;
};

state matrix[10][9] =
{//  0-9 |A-z | < |  >  | = | -	 | + |' '| Other
	{VL0, ID0, CO0, CO1, EQ0, AO01, AO0, S, WL1},// S
	{VL0, WL1, VL,  VL,  VL,  VL, VL, VL, WL1},//  VL0
	{ID0, ID0, ID,  ID,  ID,  ID, ID, ID, WL1},//  ID0
	{CO,  CO,  CO,  CO2, CO2, CO, CO, CO, WL1 },// CO0
	{CO,  CO,  CO,  CO,  CO2, CO, CO, CO, WL1 },// CO1
	{WL1, WL1, WL,  WL,  WL,  WL, WL, WL, WL1},//   WL1
	{CO,  CO,  CO,  CO,  CO,  CO, CO, CO, WL1 },// CO2
	{EQ,  EQ,  EQ,  EQ,  EQ,  EQ, EQ, EQ, WL1 },// EQ0
	{AO,  AO,  AO,  AO,  AO,  AO, AO, AO, AO }, // AO0
	{AO1,AO1, AO1, AO1, AO1, AO1, AO1,AO1,AO1}//AO1
};
lex_type check(char c)// выделяем группы символов
{
	if (isdigit(c)) return NUM;
	if (isalpha(c)) return LETTER;
	if (c == '<') return LEES;
	if (c == '>') return MORE;
	if (c == '=') return EQUALLY;
	if (c == '+') return ARITH;
	if (c == '-') return ARITH1;
	if (c == ' ' || c == '\n' || c == '\t' || c == '\0') return SEP;
	return OTHER;
}
state check_kw(Lex& lexema) {
	if (!strcmp(lexema.word, "for")) return FO;
	if (!strcmp(lexema.word, "to")) return TO;
	if (!strcmp(lexema.word, "next")) return NE;
	return ID;
}
void LexAnalysis(char* text, vector<Lex>& res)
{
	unsigned begin = 0;//первый символ слова
	state current_state = S; //начальное состояние
	Lex lex;
	int i = 0;//текущая позиция в тексте
	while (text[i])
	{
		--i;//каждый раз при проходе лексемы мы останавливаемся на следующей после неё символе, чтобы понять, что она закончилась
		//для того, чтобы проверить ее принадлежность к следующей лексеме и не пропустить его, нужно вернуться на один символ назад
		while (current_state != EQ && current_state != AO && current_state != AO1 && current_state != VL && current_state != ID && current_state != CO && current_state != WL)
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
			lex.Type = check_kw(lex);
			if (length > 5) lex.Type = WL;//если длина больше 5 - некорректная лексема
		}
		res.push_back(lex);//добавляем лексему
		current_state = S;//обновляем состояние
		begin = i;
		while (text[i] == ' ' || text[i] == '\n' || text[i] == '\t') ++i;//скипаем разделители лишние
	}
}
struct Lex_wl {//структура специально для СА
	unsigned long pos;
	S_state Type;
};

Lex_wl SinAnalisys(vector<Lex>& res)
{
	S_state current_state = Start;
	Lex_wl lex;
	lex.pos = 0;
	lex.Type = Start;
	for (unsigned long i = 0; i < res.size(); ++i)//пока вектор лексем не закончился 
	{
		current_state = s_matrix[current_state][((res[i]).Type) - 10];//переходим по матрице (от 10 потому что предыдущие 10 состояний - для переходов по матрице ЛА)
		if (current_state >= 18 && current_state <= 28)
		{
			lex.pos = i;
			lex.Type = current_state;
			return lex;
		}
	}

	if (current_state < 18)//если посоедовательность оборвалась не закончившись
	{
		lex.pos = (int)res.size();
		lex.Type = current_state;
	}
	return lex;
}

void print(vector<Lex>& res, ostream& out) {
	if (out && res.size())
	{
		const char* types[11] = { "eq", "ao", "ao", "vl", "id", "co", "wl", "fo", "to", "ne" };//список типов
		for (unsigned long i = 0; i < res.size(); ++i)
		{
			out << res[i].word << '[' << types[res[i].Type - 10] << "] ";
			cout << res[i].word << '[' << types[res[i].Type - 10] << "] ";
		}
		out << '\n';
		cout << '\n';
	}
}

void printWL(Lex_wl& lex, ostream& out) {
	if (lex.Type == NEXT) {
		out << "OK";
		cout << "OK";
		return;
	}
	const char arr[29][6] = {//массив вариантов для вывода (сопоставляется состоянию лексемы из СА)
	"fo", "id", "eq", "id vl", "vl", "ao to", "id vl", "id vl", "vl", "ao id",
	"id vl", "ao id", "eq", "id vl", "vl","ao ne", "id vl", "fo ne",
	"fo", "id", "eq", "vl", "ao", "id vl", "ao to", "ao id", "ao ne", "ne", "OK"
	};

	out << lex.pos << ' ' << arr[lex.Type];
	cout << lex.pos << ' ' << arr[lex.Type];
}

void delete_vec(vector<Lex>& vec) {
	for (unsigned long i = 0; i < vec.size(); ++i)
		delete[] vec[i].word;//удаляем строки
	vec.clear();
}

int main()
{
	ifstream fin("input.txt", ios::binary);
	if (fin)
	{
		fin.seekg(0, ios::end);//считаем длину текста
		unsigned size = 1 + fin.tellg();
		fin.seekg(0, ios::beg);

		char* text = new char[size + 1];
		fin.getline(text, size, '\0');

		vector<Lex> res; LexAnalysis(text, res);//читаем

		ofstream fout("output.txt", ios::binary);

		print(res, fout);
		Lex_wl l = SinAnalisys(res);
		printWL(l, fout);
		delete_vec(res);
		fout.close();

		delete[] text;
	}
	fin.close();
}