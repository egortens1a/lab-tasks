#include <iostream>
#include <fstream>
#include <queue>

using namespace std;

struct tree {
    int value;
    tree* left;
    tree* right;
};

tree* create_node(int a) {//функция создания узла
    tree* N = new tree;
    N->value = a;
    N->left = nullptr; N->right = nullptr;
    return N;
}

void rev_data(tree* root) {
    if (!root) return;

    int a = root->value;

    root->value = 0;
    while (a) { //переворот числа
        root->value *= 10;
        root->value += a % 10;
        a /= 10;
    }
    rev_data(root->left);//повторяем действие рекурсивно со следующими элементами
    rev_data(root->right);
}

void in_order_print(ostream& out, tree* root) {
    if (!root) return;
    in_order_print(out, root->left);//Л-К-П (симметричный) обход дерева
    out << root->value << ' ';
    in_order_print(out, root->right);
}

void delete_tree(tree* root) {
    if (!root) return;//пока не дошли до листьев - удаляем
    delete_tree(root->left);
    delete_tree(root->right);
    delete root;
}

void fillTree(tree*& root, int arr[], int i, int size)
{
    if (i < size){ // пока массив не закончился
        root = create_node(arr[i]);
        fillTree(root->left, arr, 2 * i + 1, size); //          0
        fillTree(root->right, arr, 2 * i + 2, size);//    2*i+1/ \2*i+2
    }                                               //       1    2
}                                                   //      3 4  5 6

int main(){ 
    ifstream fin("input.txt", ios::binary);

    int a;
    if (fin) {
        tree* root = nullptr;
        int size = 0;
        while (fin >> a) ++size; //считаем количество чисел в файле

        fin.clear();
        fin.seekg(0, ios::beg);//очищаем флаги и возвращаемся в начало файла

        int* arr = new int[size];
        for (int i = 0; i < size; ++i) fin >> arr[i];


        fillTree(root, arr, 0, size);
        rev_data(root);//переворачиваем значения

        ofstream fout("output.txt", ios::binary);
        in_order_print(fout, root);
        fout.close();

        delete_tree(root);//удаляем дерево
    }
    fin.close();
}