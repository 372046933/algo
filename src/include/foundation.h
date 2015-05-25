/*
 * foundation.h
 *
 *  Created on: Oct 20, 2014
 *      Author: tx
 */

#ifndef INCLUDE_FOUNDATION_H_
#define INCLUDE_FOUNDATION_H_

#include <cstring>
#include <functional>
#include <iostream>
#include <algorithm>

/**
 * This is the first part of CLRS.
 * implementations of insertion_sort, merge_sort, shell_sort
 */
//This class holds a pointer to type T of length sizeof(T)*size
template<typename T>
class _Temp {
public:
	T* temp;
	_Temp(int size)
	{
		temp = new T[size];
	}
	~_Temp()
	{
		delete[] temp;
	}
};
//forward declaration
template<typename T, typename _comp = std::less<T>>
void insertion_sort(T *arr, int begin, int end, _comp comp = _comp());
template<typename T, typename _comp = std::less<T>>
void merge_sort(T *arr, int begin, int end, _comp comp = _comp());
template<typename T, typename _comp = std::less<T>>
void merge(T *arr, int begin, int middle, int end, _comp comp = _comp());
template<typename T, typename _comp = std::less<T>>
size_t inversions(T *arr, int begin, int end, _comp comp = _comp());
template<typename T, typename _comp = std::less<T>>
size_t inversions(T *arr, int begin, int middle, int end, _comp comp = _comp());

template<typename T, typename _comp = std::less<T>>
void insertion_sort(T *arr, int size, _comp comp = _comp())
{
	insertion_sort(arr, 0, size, comp);
}
template<typename T, typename _comp = std::less<T>>
void insertion_sort(T *arr, int begin, int end, _comp comp = _comp())
{
	for (int i = begin; i < end; ++i) {
		T key = arr[i];
		int j = i - 1;
		while (comp(key, arr[j]) && j >= begin) {
			arr[j + 1] = arr[j];
			--j;
		}
		arr[j + 1] = key;
	}
}
template<typename T, typename _comp = std::less<T>>
void insertion_sort_recursive(T* arr, int size, _comp comp = _comp())
{
	if (size > 1) {
		insertion_sort_recursive(arr, size - 1, comp);
		int i = size - 2;
		int key = arr[size - 1];
		while (i >= 0 && comp(key, arr[i])) {
			arr[i + 1] = arr[i];
			--i;
		}
		arr[i + 1] = key;
	}
}

template<typename T, typename _comp = std::less<T>>
void merge_sort(T *arr, int size, _comp comp = _comp())
{
	merge_sort(arr, 0, size, comp);
}
template<typename T, typename _comp = std::less<T>>
void merge_sort(T *arr, int begin, int end, _comp comp = _comp())
{
	if (end - begin <= 1)
		return;
	// midle != (end-begin)/2
	int middle = (begin + end) / 2;
	merge_sort(arr, begin, middle, comp);
	merge_sort(arr, middle, end, comp);
	merge(arr, begin, middle, end, comp);
}
template<typename T, typename _comp = std::less<T>>
void merge(T *arr, int begin, int middle, int end, _comp comp = _comp())
{
	static _Temp<T> t(end - begin);
	int i1 = begin, i2 = middle;
	int i = 0;
	while (i1 < middle && i2 < end)
		if (comp(arr[i1], arr[i2]))
			t.temp[i++] = arr[i1++];
		else
			t.temp[i++] = arr[i2++];
	while (i1 < middle)
		t.temp[i++] = arr[i1++];
	while (i2 < end)
		t.temp[i++] = arr[i2++];
	// initial position != arr
	memcpy(arr + begin, t.temp, sizeof(T) * i);
}
template<typename T, typename _comp = std::less<T>>
size_t inversions(T *arr, int size, _comp comp = _comp())
{
	return inversions(arr, 0, size, comp);
}

template<typename T, typename _comp = std::less<T>>
size_t inversions(T *arr, int begin, int end, _comp comp = _comp())
{
	int inv = 0;
	if (end - begin <= 1)
		return 0;
	if (end - begin == 2) {
		if (comp(arr[begin], arr[end - 1]))
			return 0;
		using std::swap;
		swap(arr[begin], arr[end - 1]);
		return 1;
	}
	int middle = (begin + end) / 2;
	inv += inversions(arr, begin, middle, comp);
	inv += inversions(arr, middle, end, comp);
	inv += inversions(arr, begin, middle, end, comp);
	return inv;
}
template<typename T, typename _comp = std::less<T>>
size_t inversions(T *arr, int begin, int middle, int end, _comp comp = _comp())
{
	static _Temp<T> t(end - begin);
	int i1 = begin, i2 = middle;
	int i = 0;
	int inv = 0;
	while (i1 < middle && i2 < end)
		if (comp(arr[i1], arr[i2]))
			t.temp[i++] = arr[i1++];
		else {
			t.temp[i++] = arr[i2++];
			inv += middle - i1;
		}
	while (i1 < middle)
		t.temp[i++] = arr[i1++];
	while (i2 < end)
		t.temp[i++] = arr[i2++];
	// initial position != arr
	memcpy(arr + begin, t.temp, sizeof(T) * i);
	return inv;
}
template<typename T, typename _comp = std::less<T>>
void shell_sort(T *arr, int size, _comp comp = _comp())
{
	int gaps[] = { 701, 301, 132, 57, 23, 10, 4, 1 };
	T key;
	int j;
	for (int gap : gaps)
		for (int i = gap; i < size; ++i) {
			key = arr[i];
			for (j = i - gap; j >= 0 && comp(key, arr[j]); j -= gap) {
				arr[j + gap] = arr[j];
			}
			arr[j + gap] = key;
		}
}
template<typename T>
bool is_post_order_sorted(T *arr, int begin, int end)
{
	if (end - begin <= 1)
		return true;
	int left_root = end - 2;
	while (left_root >= begin && arr[left_root] > arr[end - 1])
		--left_root;
	if (std::any_of(arr + begin, arr + left_root + 1, [arr,end](T v) {
		return v > arr[end-1];
	}))
		return false;
	if (std::any_of(arr + left_root + 1, arr + end -1, [arr, end](T v) {
		return v < arr[end-1];
	}))
		return false;
	return is_post_order_sorted(arr, begin, left_root + 1)
			&& is_post_order_sorted(arr, left_root + 1, end - 1);
}

#endif /* INCLUDE_FOUNDATION_H_ */
