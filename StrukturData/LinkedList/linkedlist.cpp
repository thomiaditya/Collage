#include <iostream>
#include <string>
#include <fstream>
#include "prettyprint.h"

template<typename Type> class linked
{
    struct Node
    {
        Type data;
        Node* next;

        Node()
        {
            next = nullptr;
        }

        Node(Type data)
        {
            this->data = data;
        }

        Node(Type data, Node* next)
        {
            this->data = data;
            this->next = next;
        }
    };

protected:
    Node* head;
    int liveCount;

public:
    linked()
    {
        head = nullptr;
        liveCount = 0;
    }

    void push(Type data)
    {
        Node* newNode = new Node;

        newNode->data = data;

        newNode->next = this->head;

        this->head = newNode;

        liveCount++;
    }

    void del(int index)
    {
        if (index + 1 > this->count()) {
            return;
        }

        Node* current = this->head;
        Node* prev = this->head;

        for (int i = 0; i < index; i++) {
            if (i == 0) {
                current = current->next;
                continue;
            }

            current = current->next;
            prev = prev->next;
        }

        if (index == 0) {
            this->head = current->next;
        }

        prev->next = current->next;

        delete current;

        this->liveCount--;
    }

    void append(Type data)
    {
        Node* newNode = new Node(data, nullptr);
        Node* current = head;

        if (current == nullptr) {
            current = newNode;

            head = current;

            liveCount++;

            return;
        }

        while (current->next != nullptr) {
            current = current->next;
        }

        current->next = newNode;

        liveCount++;
    }

    int count()
    {
        return this->liveCount;
    }

    Type* onArray()
    {
        Node* first = head;
        Type* datas = new Type[liveCount + 1];

        for (int i = 0; first != nullptr; i++) {
            *datas = first->data;
            first = first->next;

            datas++;
        }

        datas -= liveCount;

        return datas;
    }

    void insertAfter(int index, Type data)
    {
        if (index > this->count()) {
            std::cout << "Just " << this->count() << " available!";
            return;
        }

        Node* current = head;
        Node* after = head;

        for (int i = 0; i < index; i++) {
            current = after;
            after = after->next;
        }

        Node* newNode = new Node(data, after);

        current->next = newNode;

        liveCount++;
    }

    void operator>>(Type data)
    {
        this->append(data);
    }

    void operator<<(Type data)
    {
        this->push(data);
    }
};

struct Date
{
    int date;
    int month;
    int year;

    Date()
    {
        date = 1;
        month = 1;
        year = 2000;
    }

    Date(int date, int month, int year)
    {
        this->date = date;
        this->month = month;
        this->year = year;
    }

    std::string showDate()
    {
        std::string dates = std::to_string(date) + " - " + std::to_string(month) + " - " + std::to_string(year);

        return dates;
    }
};

struct Mahasiswa
{
    std::string name;
    std::string identity;
    Date birthday;
};

void addMahasiswa(linked<Mahasiswa>& mahasiswas, bool append = true)
{
    using namespace std;

    Mahasiswa* newMahasiswa = new Mahasiswa;

    cout << "\tEnter Name : ";
    cin.ignore();
    getline(cin, newMahasiswa->name);

    cout << "\tEnter NIM : ";
    cin >> newMahasiswa->identity;

    cout << "\tEnter Date of Birthday (dd mm yyyy) : ";
    cin >> newMahasiswa->birthday.date >> newMahasiswa->birthday.month >> newMahasiswa->birthday.year;

    if (append == true) {
        mahasiswas >> *newMahasiswa;
        return;
    }

    mahasiswas << *newMahasiswa;
}

void showMahasiswa(linked<Mahasiswa> mahasiswas)
{
    Mahasiswa* mhs = mahasiswas.onArray();

    bprinter::TablePrinter tp(&std::cout);
    tp.AddColumn("Ind", 4);
    tp.AddColumn("Name", 25);
    tp.AddColumn("NIM", 13);
    tp.AddColumn("Birtday", 15);

    tp.PrintHeader();
    for (int i = 0; i < mahasiswas.count(); i++) {
        tp << i << mhs[i].name << mhs[i].identity << mhs[i].birthday.showDate();
    }
    tp.PrintFooter();
}

void init()
{
    std::cout << "Enter the command :" << std::endl
        << '\t' << "add (Menambahkan paling belakang)" << std::endl
        << '\t' << "addpush (Menambahkan di depan)" << std::endl
        << '\t' << "show (Menampilkan)" << std::endl
        << '\t' << "del [INDEX] (Menghapus mahasiswa)" << std::endl;
}

void delMahasiswa(linked<Mahasiswa>& mhs)
{
    int index;
    std::cin >> index;

    mhs.del(index);
}

int main()
{
    linked<Mahasiswa> Mahasiswas;

    init();
    while (true) {
        std::string command;
        std::cout << "\n> ";

        std::cin >> command;

        if (command == "add") {
            addMahasiswa(Mahasiswas);
            continue;
        }

        if (command == "addpush") {
            addMahasiswa(Mahasiswas, false);
            continue;
        }

        if (command == "show") {
            showMahasiswa(Mahasiswas);
            continue;
        }

        if (command == "del") {
            delMahasiswa(Mahasiswas);
            continue;
        }
    }
}
