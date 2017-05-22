#ifndef MAX_HEAP_H
#define MAX_HEAP_H

#include <iostream>
#include <cstring>
#include <vector>

template <typename T>
class Heap {
private:
    std::vector<T> values;
    int dimVect;
    int capVect;

public:

    int (*compare)(T, T);

    Heap(int capVect, int (*compare) (T, T));

    int parent(int poz);

    int leftSubtree(int poz);

    int rightSubtree(int poz);

    void pushUp(int poz);

    void pushDown(int poz);

    void insert(T x);

    T peek();

    T extractMax();

    void displayVect();

    std::vector<T>& getVect();

    void deleteNodes();

    bool hasNodes();
};

// Constructorul contine si o functie de comparare a doua elemente de tip T
template <typename T>
Heap<T>::Heap(int capVect, int (*compare) (T, T)) {
    this->capVect = capVect;
    dimVect = 0;
    values.resize(capVect);
    this->compare = compare;
}

// Stergerea tuturor nodurilor din Heap
template <typename T>
void Heap<T>::deleteNodes() {
   dimVect = 0;
   values.resize(1);
   capVect = 1;
}

// Returneaza pozitia in vector a parintelui
template <typename T>
int Heap<T>::parent(int poz) {
    return (poz - 1) / 2;
}

// Returneaza pozitia in vector a copilului stang
template <typename T>
int Heap<T>::leftSubtree(int poz) {
    return 2 * poz + 1;
}

// Returneaza pozitia in vector a copilului drept
template <typename T>
int Heap<T>::rightSubtree(int poz) {
    return 2 * poz + 2;
}

// Realizeaza operatia de urcare necesara inserarii in Heap
template <typename T>
void Heap<T>::pushUp(int poz) {
    int parent = this->parent(poz);

    // Se realizeaza interschimbarea cat timp nodul nu a ajuns sa fie radacina
    // sau cat timp este mai mare decat parintele sau
    while (poz > 0 && compare(values[parent], values[poz]) < 0) {
      T aux = values[poz];
      values[poz] = values[parent];
      values[parent] = aux;
      poz = parent;
      parent = this->parent(poz);
    }
}

// Realizeaza operatia de coborare necesara stergerii din Heap
template <typename T>
void Heap<T>::pushDown(int poz) {
    while (1) {
      if (rightSubtree(poz) > dimVect - 1) {  // nu exista right
        if (leftSubtree(poz) > dimVect - 1) {  // nu exista left
          break;
        } else {  // exista left
          if (compare(values[poz], values[leftSubtree(poz)]) < 0) {
            T aux = values[poz];
            values[poz] = values[leftSubtree(poz)];
            values[leftSubtree(poz)] = aux;
            poz = leftSubtree(poz);
          } else {  // se respecta conditia de ordine crescatoare
            break;
          }
        }
      } else {  // exista si left si right
          if (compare(values[leftSubtree(poz)], values[rightSubtree(poz)]) > 0) {
            if (compare(values[poz], values[leftSubtree(poz)]) < 0) {  // ordine
              T aux = values[poz];
              values[poz] = values[leftSubtree(poz)];
              values[leftSubtree(poz)] = aux;
              poz = leftSubtree(poz);
            } else {
              break;  // se respecta ordinea
            }
          } else {
            if (compare(values[poz], values[rightSubtree(poz)]) < 0) {
              T aux = values[poz];
              values[poz] = values[rightSubtree(poz)];
              values[rightSubtree(poz)] = aux;
              poz = rightSubtree(poz);
            } else {
              break;  // se respecta ordinea
            }
          }
      }
    }
}

// Ajuta la inserarea in Heap a unui element nou
template <typename T>
void Heap<T>::insert(T x) {
    // Redimensionarea vectorului, daca e cazul
    if (dimVect == capVect) {
      capVect = capVect * 2;
      values.resize(capVect);
    }
    values[dimVect] = x;
    ++dimVect;
    pushUp(dimVect - 1);
}

// Returneaza elementul maxim
template <typename T>
T Heap<T>::peek() {
    if (dimVect > 0) {
      return values[0];
    }
    return T();
}

// Elimina din Heap elementul maxim, adica radacina
template <typename T>
T Heap<T>::extractMax() {
  if (dimVect == 0) {
    return T();
  }
    T aux = values[0];
    values[0] = values[dimVect - 1];
    values[dimVect - 1] = aux;
    --dimVect;
    if (dimVect < capVect / 2 && capVect / 2 != 0) {
      capVect = capVect / 2;
    values.resize(capVect);
    }
    pushDown(0);
    return aux;
}

// Verifica daca Heap-ul mai are cel putin un nod
template <typename T>
bool Heap<T>::hasNodes() {
	if (dimVect == 0) {
		return false;
	}
	return true;
}

#endif // MAX_HEAP_H
