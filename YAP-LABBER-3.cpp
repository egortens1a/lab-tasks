#include <fstream>
#include <iostream>

using namespace std;

struct list
{
    int value;
    list* next;
};

void swap(list* first, list* second) {
    if (first == second) {
        return;
    }
    list* prev_first = first;
    list* prev_second = second;

    do {
        prev_first = prev_first->next;
    } while (prev_first->next != first);//проходим весь список чтобы дойти до предыдущих элементов

    do {
        prev_second = prev_second->next;
    } while (prev_second->next != second);

    prev_first->next = second;//перекидываем "мостики"
    prev_second->next = first;

    list* temp = first->next;
    first->next = second->next;
    second->next = temp;
}

void insertion(list*& head) {
    if (head == nullptr || head->next == head) {
        return;
    }
    list* sec_value = new list;
    list* t1 = head->next;
    while (t1 != head) {//пока не прошли все элементы
        sec_value->value = t1->value;//присваиваем первое значение
        bool found = 0;
        list* t2 = head;
        while (t2 != t1) {//пока не прошли все элементы
            if (t2->value > t1->value && !found) { //если число больше чем текущее и ранее не находили такого
                sec_value->value = t2->value; //обновляем
                t2->value = t1->value;
                found = true;
            }
            else {
                if (found) {
                    swap(sec_value->value, t2->value);//свапаем щначения
                }
            }
            t2 = t2->next;
        }
        t2->value = sec_value->value;
        t1 = t1->next;
    }
}

void push(list*& p, int v)
{
    list* l = new list; // NODE 2
    l->value = v;

    if (p) // p != NULL
    {
        l->next = p->next;
        p->next = l;
    }
    else // создаем первый элемент списка
    {
        l->next = l;
    }
    p = l;
}

void print(ostream& out, list* head, int size)//выводим 
{
    if (!size) return;
    list* p;
    p = head;
    if (p)
    {
        do
        {
            out << p->value << " ";//ваводим значение
            p = p->next;//передвигаем указатель
        } while (p != head);//пока не дошли до конца
    }
}

int pop(list* p)//удаляем 
{
    if (p)
    {
        list* d = p->next;

        p->next = d->next; //переставляем указатель
        int value = d->value;//сохраняем значение 
        delete d;//удаляем элемент 

        return value;
    }

    else return 0;

}

int main()
{
    ifstream fin("input.txt");//читаем из файла 

    bool key;
    int size = 0;
    fin >> key;

    list* head = nullptr;
    list* p;

    int v = 0;

    fin >> v;
    push(head, v); // Вставка первого элемента

    p = head;
    while (!fin.eof())
    {
        ++size;
        fin >> v;
        push(p, v);

    }

    p = head;

    if (key) insertion(head);
    else merge_sort(head);

    ofstream fout("output.txt");//записываем в файл
    p = head;
    fout << size + 1 << " ";//записываем размер в файл


    p = head;
    print(fout, p, size);//выводим список
    print(cout, p, size);//выводим список

    fin.close();
    fout.close();

    p = head;

    while (p != head)//очищаем память
        pop(p);

    return 0;
}
