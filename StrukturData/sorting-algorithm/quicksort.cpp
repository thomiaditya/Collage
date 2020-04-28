#include <iostream>
#include <vector>
#include <exception>

void showArray(std::vector<int> const& arrays, int n)
{
    for (int i = 0; i < n; i++) {
        std::cout << arrays[i] << " ";
    }
}

void swap(std::vector<int>& arrays, int first, int second)
{
    int temp = arrays[first];

    arrays[first] = arrays[second];
    arrays[second] = temp;
}

int division( std::vector<int>& arrays, int start, int end, int pivot, bool(*decider)(int, int))
{
    int left = start;
    int right = end;

    while (left <= right) {
        if (decider(arrays[left], pivot)) {
            left++;
        }
        else if(left <= right) {
            swap(arrays, left, right);
        }

        if (decider(pivot, arrays[right])) {
            right--;
        }
        else if (left <= right) {
            swap(arrays, left, right);
        }
    }

    return left;
}

void quickSort( std::vector<int>& arrays, int start, int end, bool(*decider)(int, int) )
{
    if (start >= end) {
        return;
    }

    int pivot = arrays[end];
    int pivotIndex = end;

    int indexToPlace = division( arrays, start, end - 1, pivot, decider );

    swap(arrays, indexToPlace, pivotIndex);

    quickSort(arrays, indexToPlace + 1, end, decider);
    quickSort(arrays, start, indexToPlace - 1, decider);
}

bool fillIn( std::vector<int>& arrays, int total )
{
    for (int i = 0; i < total; i++) {
        int val;
        std::cin >> val;

        arrays.push_back(val);
    }

    return true;
}

int main()
{
    std::vector<int> arrays;
    int n;
    std::cin >> n;

    fillIn( arrays, n );

    quickSort(arrays, 0, (n - 1), [](int x, int y) {
        //Gunakan <= untuk mengurutkan mulai dari terkecil hingga terbesar dan
        // >= untuk mengurutkan mulai besar ke kecil.
        // Untuk mengurutkan custom bisa menggunakan return true atau false
        // pada kondisi tertentu.
        return x <= y;
    });

    showArray(arrays, n);
}
