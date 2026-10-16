#include <windows.h>
#include <iostream>
#include <vector>
using namespace std;

struct Thread {
    vector<int>* arr;
    int min = 0;
    int max = 0;
    double average = 0.0;
};

DWORD WINAPI MinMaxThread(LPVOID lpParam) {
    Thread* data = (Thread*)lpParam;
    vector<int>& arr = *(data->arr);
    data->min = arr[0];
    data->max = arr[0];

    for (size_t i = 1; i < arr.size(); ++i) {
        if (arr[i] < data->min) {
            data->min = arr[i];
        }
        Sleep(7);

        if (arr[i] > data->max) {
            data->max = arr[i];
        }
        Sleep(7);
    }
    return 0;
}

DWORD WINAPI AverageThread(LPVOID lpParam) {
    Thread* data = (Thread*)lpParam;
    vector<int>& arr = *(data->arr);

    double sum = 0;
    for (size_t i = 0; i < arr.size(); ++i) {
        sum += arr[i];
        Sleep(12);
    }
    data->average = sum / arr.size();
    return 0;
}

int main() {
    setlocale(LC_ALL, "Russian");
    int size;
    cin >> size;

    vector<int> arr(size);
    for (int i = 0; i < size; ++i) {
        cin >> arr[i];
    }

    Thread data;
    data.arr = &arr;
    HANDLE hMin_Max = CreateThread(NULL, 0, MinMaxThread, &data, 0, NULL);
    HANDLE hAverage = CreateThread(NULL, 0, AverageThread, &data, 0, NULL);

    if (hMin_Max == NULL || hAverage == NULL) {
        cout << "Ошибка при создании потоков" << endl;
        return 1;
    }

    WaitForSingleObject(hMin_Max, INFINITE);
    WaitForSingleObject(hAverage, INFINITE);

    int avg_converted = static_cast<int>(data.average);
    for (int i = 0; i < size; ++i) {
        if (arr[i] == data.min || arr[i] == data.max) {
            arr[i] = avg_converted;
        }
    }
    cout << "Новый массив: ";
    for (int i = 0; i < size; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;
    CloseHandle(hMin_Max);
    CloseHandle(hAverage);

    return 0;
}
