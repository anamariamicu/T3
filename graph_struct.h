// Copyright 2017 Micu Ana-Maria, 311CA

#ifndef __GRAPH_STRUCT_H__
#define __GRAPH_STRUCT_H__

#include <string>

using std::string;

template<typename Tkey, typename Tvalue>
struct data {
  struct data<Tkey, Tvalue> *next;
  Tvalue value;
  Tkey key;

  data(Tkey key, Tvalue value) {
    this->value = value;
    this->key = key;
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
    value = Tvalue();
    key = Tkey();
  }
};


template<typename T>
struct Node {
  std::list<struct data<T, int>> neighbors;
  T nodeValue;
  struct Node<T> *next;

  explicit Node(T nodeValue) {
    this->nodeValue = nodeValue;
  }

  Node() {
    nodeValue = T();
  }
};

#endif
