#include <iostream>
#include <string>

std::string deleteOperation = "delete";
std::string addOperation = "add";
std::string updateOperation = "update";
std::string readOperation = "show";

void firstInit()
{
    std::cout << "Name: Thomi Aditya Alhakiim" << std::endl << "NIM: 19051204057\n";
    std::cout << "usage:\n\tadd <NAME>\t\texample: add Thomi Aditya\n\tdelete <INDEX>\t\texample: delete 1\n\tshow\n";
}

void set()
{
    std::cout << "\n>";
}

std::string stringInput()
{
    std::string operation;

    getline(std::cin, operation);

    return operation;
}

std::string firstInput()
{
    std::string operation;

    set();
    std::cin >> operation;

    return operation;
}

int intInput()
{
    int input;

    set();

    std::cin >> input;

    return input;
}

void show(std::string name[])
{
    int count = 1;
    int stack = 1;

    for (int i = 0; i < 100; i++) {
        if (name[i] != "deleted" && name[i] != "") {
            std::cout << count << ". " << name[i] << std::endl;
            count++;
        }
    }
}

void sort(std::string theArray[])
{
    for (int i = 0; i < theArray->size(); i++) {
        if (theArray[i] == "deleted" || theArray[i] == "") {
            if (theArray[i + 1] != "deleted" || theArray[i] == "") {
                theArray[i] = theArray[i + 1];
                theArray[i + 1] = "";
            }
        }
    }
}

void runDeletation(std::string theArray[], int index)
{
    theArray[index - 1] = "deleted";
}

void runAddition(std::string theArray[], std::string name)
{
    for (int i = 0; i < 100; i++) {
        if (theArray[i] == "deleted" || theArray[i] == "") {
            theArray[i] = name;
            return;
        }
    }
}

void del(std::string nameMahasiswa[])
{
    int number = intInput();

    std::cout << "Row " << number << " deleted!" << std::endl;


    runDeletation(nameMahasiswa, number);

    sort(nameMahasiswa);
}

void add(std::string nameMahasiswa[])
{
    std::cout << "One row added!" << std::endl;

    std::string name = stringInput();

    runAddition(nameMahasiswa, name);

    show(nameMahasiswa);
}

int main()
{
    firstInit();

    std::string nameMahasiswa[100];
    bool stopped = false;

    while (!stopped) {
        std::string operation = firstInput();

        if (operation == "add") {
            add(nameMahasiswa);
        }

        if (operation == "delete") {
            del(nameMahasiswa);
        }

        if (operation == "show") {
            show(nameMahasiswa);
        }
    }
}

