#include <iostream>

template<typename Type> class linked
{
    struct Node
    {
        Type data;
        Node* next;

        Node() {}

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
            datas[i] = first->data;
            first = first->next;
        }

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
};

int main()
{
    linked<int> Mahasiswa;

    for (int i = 1; i <= 5; i++) {
        Mahasiswa.append(i);
    }

    std::cout << std::endl << Mahasiswa.count() << std::endl;

    Mahasiswa.insertAfter(2, 100);

    int* arrays = Mahasiswa.onArray();

    for (int i = 0; i < 6; i++) {
        std::cout << arrays[i] << " ";
    }

    std::cout << std::endl << Mahasiswa.count() << std::endl;
}
