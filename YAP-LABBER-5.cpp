#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

class vector3D {
private:
	double x, y, z;
	friend istream& operator >> (istream&, vector3D&);//дружественные функции для перегрузки
	friend ostream& operator << (ostream&, vector3D);//операторов ввода и вывода
public:
	vector3D() : x(0), y(0), z(0) {};//по умолчанию
	vector3D(int xx, int yy, int zz) : x(xx), y(yy), z(zz) {};//конструктор инициализации
	vector3D(const vector3D& other) :x(other.x), y(other.y), z(other.z) {}//конструктор копирования
	~vector3D() {};//деструктор
	double abs() const;//длина вектора
	double angle(vector3D&) const;//угол между векторами
	double angle_X() const;//угол с осью Х
	double angle_Y() const;//угол с осью Y
	double angle_Z() const;//угол с осью Z

	void operator+=(const vector3D&);//перегрузка оператора +=
	void operator-=(const vector3D&);//перегрузка оператора -=
	void operator*=(const double);//перегрузка оператора *=
	void operator/=(const double);//перегрузка оператора /=

	void operator=(vector3D&);//перегрузка оператора присваивания

	vector3D operator+(const vector3D&) const;//перегрузка оператора +
	vector3D operator-(const vector3D&) const;//перегрузка оператора -
	vector3D operator*(const double) const;//перегрузка оператора *
	vector3D operator/(const double) const;//перегрузка оператора /

	double key_cos_X() const;//направляющий косинус по Х
	double key_cos_Y() const;//направляющий косинус по Y
	double key_cos_Z() const;//направляющий косинус по Z

	bool operator>(const vector3D&) const;//операторы сравнения
	bool operator>=(const vector3D&) const;
	bool operator<(const vector3D&) const;
	bool operator<=(const vector3D&) const;
	bool operator==(const vector3D&) const;
};
//булевые операции
bool vector3D::operator>(const vector3D& other) const { return abs() > other.abs(); }
bool vector3D::operator>=(const vector3D& other) const { return abs() >= other.abs(); }
bool vector3D::operator<(const vector3D& other) const { return abs() < other.abs(); }
bool vector3D::operator<=(const vector3D& other) const { return abs() <= other.abs(); }
bool vector3D::operator==(const vector3D& other) const { return ((x == other.x) && (y == other.y) && (z == other.z)); }
//арифм операции
vector3D vector3D::operator+(const vector3D& other) const {
	vector3D new_vec(*this);
	new_vec += other;
	return new_vec;
}
vector3D vector3D::operator-(const vector3D& other) const {
	vector3D new_vec(*this);
	new_vec -= other;
	return new_vec;
}
vector3D vector3D::operator*(const double a) const {
	vector3D new_vec(*this);
	new_vec *= a;
	return new_vec;
}
vector3D vector3D::operator/(const double a) const {
	vector3D new_vec(*this);
	new_vec /= a;
	return new_vec;
}
void vector3D::operator*=(const double a) {
	x *= a;
	y *= a;
	z *= a;
}
void vector3D::operator/=(const double a) {
	x /= a;
	y /= a;
	z /= a;
}
void vector3D::operator+=(const vector3D& other) {
	x += other.x;
	y += other.y;
	z += other.z;
}
void vector3D::operator-=(const vector3D& other) {
	x -= other.x;
	y -= other.y;
	z -= other.z;
}
//присваивание
void vector3D::operator=(vector3D& other) {
	x = other.x;
	y = other.y;
	z = other.z;
}
//методы по заданию
double vector3D::abs() const { return sqrt(x * x + y * y + z * z); }//длина вектора
double vector3D::angle(vector3D& oth) const {//угол между векторами
	return acos(((x * oth.x) + (y * oth.y) + (z * oth.z)) / (abs() * oth.abs() ) ) * (180 / 3.141592653589793); 
}

double vector3D::angle_X() const { return acos(x / abs()) / 3.141592653589793 * 180; }//угол с осями
double vector3D::angle_Y() const { return acos(y / abs()) / 3.141592653589793 * 180; }
double vector3D::angle_Z() const { return acos(z / abs()) / 3.141592653589793 * 180; }

double vector3D::key_cos_X() const { return x / abs(); }//направляющие вектора
double vector3D::key_cos_Y() const { return y / abs(); }
double vector3D::key_cos_Z() const { return z / abs(); }

istream& operator >> (istream& in, vector3D& vec) { return in >> vec.x >> vec.y >> vec.z; }//перегрузки операторов ввода и вывода
ostream& operator << (ostream& out, vector3D vec) { return out << vec.x << ", " << vec.y << ", " << vec.z; }

void functions(istream& in, ostream& out, char c) {//демонстрация функционала
	vector3D v1;
	vector3D v2;
	switch (c) {
	case('1'):
		in >> v1;
		out << v1.abs();
		break;
	case('2'):
		in >> v1;
		out << v1.angle_X() << ' ' << v1.angle_Y() << ' ' << v1.angle_Z();
		break;
	case('3'):
		in >> v1 >> v2;
		out << '(' << v1 + v2 << ") (" << v1 - v2 << ") (" << v1 * 2 << ") (" << v1 / 2 << ')';
		break;
	case('4'):
		in >> v1 >> v2;
		out << v1.angle(v2);
		break;
	case('5'):
		in >> v1;
		out << v1.key_cos_X() << ' ' << v1.key_cos_Y() << ' ' << v1.key_cos_Z();
		break;
	case('6'):
		in >> v1 >> v2;
		out << ((v1 > v2) ? "1-ый вектор больше 2-го" : "2-ой вектор больше либо равен 1-му");
		break;
	case('7'):
		in >> v1 >> v2;
		out << ((v1 == v2) ? "Вектора равны" : "Вектора не равны");
		break;
	default:
		out << "Неизвестный метод";
	}
}
int main() {
	ifstream fin("input.txt");
	ofstream fout("output.txt");
	char c; fin >> c;

	functions(fin, fout, c);

	fin.close();
	fout.close();

	return 0;
}