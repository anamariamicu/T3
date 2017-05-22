// Copyright 2017 Micu Ana-Maria, 311CA

#ifndef __GRAPH_STRUCT_H__
#define __GRAPH_STRUCT_H__

// Structura utilizata pentru a salva vecinul unui nod impreuna cu
// costul dintre cele doua noduri
template<typename Tkey, typename Tvalue>
struct data {
  struct data<Tkey, Tvalue> *next;
  Tvalue value;
  int nodeIndex;
  Tkey key;

  data(Tkey key, int nodeIndex, Tvalue value) {
    this->value = value;
    this->key = key;
    this->nodeIndex = nodeIndex;
    next = nullptr;
  }

  bool operator==(struct data& el) {
    if (this->key == el.key) {
      return true;
    }
    return false;
  }

  data() {
    next = nullptr;
    nodeIndex = -1;
    value = Tvalue();
    key = Tkey();
  }
};

// Structura ce defineste un nod al grafului
template<typename T>
struct Node {
  std::list<struct data<T, int>> neighbors;
  T nodeValue;
  int nodeIndex;

  explicit Node(T nodeValue, int nodeIndex) {
    this->nodeValue = nodeValue;
    this->nodeIndex = nodeIndex;
  }

  Node() {
    nodeValue = T();
  }
};

#endif
