//
// Sorting.h
// 2023-03-31
// Ivan Konishchev
//

#pragma once

class Sorting
{
    static void swap(int *a, int *b) // функция для смены местами двух значений
    {
        int temp = *a;
        *a = *b;
        *b = temp;
    }

    static int partition(int *arr, int low, int high)
    {
        int pivot = arr[high]; // опорный элемент
        int i = (low - 1);     // индекс наименьшего элемента
        for (int j = low; j <= high - 1; j++)
        {
            // если текущий элемент меньше или равен опорнуму
            if (arr[j] <= pivot)
            {
                i++; // увеличиваем индекс минимального элемента
                swap(&arr[i], &arr[j]);
            }
        }
        swap(&arr[i + 1], &arr[high]);
        return (i + 1);
    }

    public:
    /// @brief quick sort
    /// @param arr array
    /// @param low min index array
    /// @param high max index array
    static void quickSort(int *arr, int low, int high)
    {
        if (low <= high)
        {
            int pi = partition(arr, low, high); // индекс опорного элемента
            // повторяем подмассив, содержащий элементы, меньшие опорной точки
            quickSort(arr, low, pi - 1);
            // повторяем подмассив, содержащий элементы, превышающие точку опоры
            quickSort(arr, pi + 1, high);
        }
    }
};
