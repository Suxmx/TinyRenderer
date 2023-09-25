#ifndef MYRENDERER_BUFFER_H
#define MYRENDERER_BUFFER_H

#include <vector>

template<class T>
class Buffer {
private:
    std::vector<T> data;
    int columns;
    int rows;



public:
    void setValue(int x, int y, T value);

    T getValue(int x, int y);

    void clear();

    void setAll(T value);
    Buffer(int m, int n) {
        data.reserve(m * n);
        rows = m;
        columns = n;
    }
};

template<class T>
void Buffer<T>::setValue(int x, int y, T value) {
    data[x * columns + y] = value;
}

template<class T>
T Buffer<T>::getValue(int x, int y) {
    return data[x * columns + y];
}

template<class T>
void Buffer<T>::clear() {
    data.clear();
}

template<class T>
void Buffer<T>::setAll(T value) {
    for (int i=0;i<rows*columns;i++)
        data[i] = value;
}


#endif //MYRENDERER_BUFFER_H
