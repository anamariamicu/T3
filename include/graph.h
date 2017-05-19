// Copyright 2017 Micu Ana-Maria, 311CA

#ifndef __GRAPH__H
#define __GRAPH__H

#include <list>
#include "./graph_struct.h"

template <typename T>
class Graph {
 private:
  // Implementare cu liste de vecini.
  std::list<struct Node<T>> nodes;

 public:
  void addNode(T node) {
    struct Node<T> n(node);
    nodes.push_back(n);
  }

  void removeNode(T node) {
    for (auto it = nodes.begin(); it != nodes.end(); ++it) {
      if (it->nodeValue == node) {
        nodes.erase(it);
        break;
      }
    }
    for (auto it = nodes.begin(); it != nodes.end(); ++it) {
      std::list<struct data<T, int>> *neighbors = nodes.getNeighbors(*it);
      for (auto itNeighbors = neighbors.begin(); itNeighbors != neighbors.end(); ++itNeighbors) {
        if (itNeighbors->value == node) {
          it->neighbors.erase(itNeighbors);
          break;
        }
      }
    }
  }

  void addEdge(T src, T dst) {
    std::list<struct data<T, int>> *neighbors = getNeighbors(src);
    struct data<T, int> aux(dst, 0);
    if (neighbors != nullptr) {
      neighbors->push_back(aux);
    } 
  }

  void addEdge(T src, T dst, int dist) {
    // Intre cele doua noduri va fi distanta dist.
    std::list<struct data<T, int>> *neighbors = getNeighbors(src);
    struct data<T, int> aux(dst, dist);
    neighbors->push_back(aux);
  }

  void increaseDistance(T src, T dst, int plus) {
    if (hasEdge(src, dst) == false) {
      addEdge(src, dst, plus);
    } else {
      std::list<struct data<T, int>> *neighbors = getNeighbors(src);
      for (auto itNeighbors = neighbors.begin(); itNeighbors != neighbors.end(); ++itNeighbors) { 
        if (itNeighbors->key == dst) {
          itNeighbors->value = itNeighbors->value + plus;
          break;
        }
      }
    }
  }

  int distance(T src, T dst) {
    std::list<struct data<T, int>> *neighbors = getNeighbors(src);
    for (auto itNeighbors = neighbors.begin(); itNeighbors != neighbors.end(); ++itNeighbors) { 
      if (itNeighbors->key == dst) {
        return itNeighbors->value;
      }
    }
    return 0;
  }

  void removeEdge(T src, T dst) {
    if (hasEdge(src, dst) == false) {
      return;
    }
    struct data<T, int> *itNeighbors;
    std::list<struct data<T, int>> *neighbors = getNeighbors(src);
    for (auto itNeighbors = neighbors.begin(); itNeighbors != neighbors.end(); ++itNeighbors) {  
      if (itNeighbors->key == dst) {
        neighbors->erase(itNeighbors);
        break;
      }
    }
  }

  void removeAllEdges() {
    for (auto it = nodes.begin(); it != nodes.end(); ++it) {   
      // Se elimina lista de vecini.
      std::list<struct data<T, int>> *neighbors = getNeighbors(it->nodeValue);
      neighbors->removeAll();
    }
  }

  bool hasEdge(T src, T dst) {
    std::list<struct data<T, int>> *neighbors = getNeighbors(src);
    if (neighbors == nullptr) {
        return false;
    }
    for (auto it = neighbors->begin(); it != neighbors->end(); ++it) {
      if (it->key == dst) {
        return true;
      }
    }
    return false;
  }

  std::list<struct data<T, int>>* getNeighbors(T node) {
    for (auto it = nodes.begin(); it != nodes.end(); ++it) {
      if (it->nodeValue == node) {
        return &(it->neighbors);
      }
    }
    return nullptr;
  }

  bool hasEdges() {
    for (auto it = nodes.begin(); it != nodes.end(); ++it) {
      if (getNeighbors(it->nodeValue)->isEmpty() == false) {
        return true;
      }
    }
    return false;
  }

  bool hasNode(T node) {
    for (auto it = nodes.begin(); it != nodes.end(); ++it) {
      if (it->nodeValue == node) {
        return true;
      }
    }
    return false;
  }
};

#endif  // _HOME_STUDENT___RESOURCES___GRAPH_H_
