// Copyright 2017 Micu Ana-Maria, 311CA

#ifndef __GRAPH__H
#define __GRAPH__H

#include <list>
#include "./graph_struct.h"

template <typename T>
class Graph {
 private:

  // Implementare cu liste de vecini a grafului cu cost
  std::list<struct Node<T>> nodes;

 public:
  void addNode(T node) {
    struct Node<T> n(node, nodes.size());
    nodes.push_back(n);
  }

  void removeNode(T node) {

    // Stergerea din lista de noduri
    for (auto it = nodes.begin(); it != nodes.end(); ++it) {
      if (it->nodeValue == node) {
        nodes.erase(it);
        break;
      }
    }

    // Stergerea din lista de vecini a celorlalte noduri
    for (auto it = nodes.begin(); it != nodes.end(); ++it) {
      std::list<struct data<T, int>> *neighbors = getNeighbors(it->nodeValue);
      for (auto itNeighbors = neighbors->begin(); itNeighbors != neighbors->end(); ++itNeighbors) {
        if (itNeighbors->key == node) {
          it->neighbors.erase(itNeighbors);
          break;
        }
      }
    }
  }

  // Returnarea indexului nodului atribuit pentru a fi identificat mai usor  
  int getIndex(T node) {
    for (auto it = nodes.begin(); it != nodes.end(); ++it) {
      if (it->nodeValue == node) {
        return it->nodeIndex;
      }
    }
    return -1;
  }

  // Adaugarea unei muchii fara cost(cu cost 0)
  void addEdge(T src, T dst, int dstIndex) {
    std::list<struct data<T, int>> *neighbors = getNeighbors(src);
    struct data<T, int> aux(dst, dstIndex, 0);
    if (neighbors != nullptr) {
      neighbors->push_back(aux);
    } 
  }

  // Adugarea unei muchii cu cost
  void addEdge(T src, T dst, int dstIndex, int dist) {

    // Intre cele doua noduri va fi distanta dist.
    std::list<struct data<T, int>> *neighbors = getNeighbors(src);
    struct data<T, int> aux(dst, dstIndex, dist);
    if (neighbors != nullptr) {
      neighbors->push_back(aux);
    }
  }

  // Creste costul dintre doua noduri
  void increaseDistance(T src, T dst, int dstIndex, int plus) {
    if (hasEdge(src, dst) == false) {
      addEdge(src, dst, dstIndex, plus);
    } else {
      std::list<struct data<T, int>> *neighbors = getNeighbors(src);
      for (auto itNeighbors = neighbors->begin(); itNeighbors != neighbors->end(); ++itNeighbors) { 
        if (itNeighbors->key == dst) {
          itNeighbors->value = itNeighbors->value + plus;
          break;
        }
      }
    }
  }

  // Returneaza costul dintre doua noduri
  int distance(T src, T dst) {
    std::list<struct data<T, int>> *neighbors = getNeighbors(src);
    for (auto itNeighbors = neighbors.begin(); itNeighbors != neighbors.end(); ++itNeighbors) { 
      if (itNeighbors->key == dst) {
        return itNeighbors->value;
      }
    }
    return 0;
  }

  // Elimina muchia dintre doua noduri
  void removeEdge(T src, T dst) {
    if (hasEdge(src, dst) == false) {
      return;
    }
    struct data<T, int> *itNeighbors;
    std::list<struct data<T, int>> *neighbors = getNeighbors(src);

    // Eliminarea din lista de vecini
    for (auto itNeighbors = neighbors->begin(); itNeighbors != neighbors->end(); ++itNeighbors) {  
      if (itNeighbors->key == dst) {
        neighbors->erase(itNeighbors);
        break;
      }
    }
  }

  // Stergerea tuturor muchiilor
  void removeAllEdges() {
    for (auto it = nodes.begin(); it != nodes.end(); ++it) {
   
      // Se elimina lista de vecini.
      std::list<struct data<T, int>> *neighbors = getNeighbors(it->nodeValue);
      neighbors->removeAll();
    }
  }

  // Verifica existenta unei muchii intre cele doua noduri
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

  // Returneaza lista de vecini a unui nod
  std::list<struct data<T, int>>* getNeighbors(T node) {
    for (auto it = nodes.begin(); it != nodes.end(); ++it) {
      if (it->nodeValue == node) {
        return &(it->neighbors);
      }
    }
    return nullptr;
  }

  // Verifica daca un nod are cel putin o muchie, deci cel putin un vecin
  bool hasEdges() {
    for (auto it = nodes.begin(); it != nodes.end(); ++it) {
      if (getNeighbors(it->nodeValue)->isEmpty() == false) {
        return true;
      }
    }
    return false;
  }

  // Verifica daca in graf mai exista cel putin un nod
  bool hasNode(T node) {
    for (auto it = nodes.begin(); it != nodes.end(); ++it) {
      if (it->nodeValue == node) {
        return true;
      }
    }
    return false;
  }

  // Returneaza lista de noduri din graf
  std::list<struct Node<T>>* getNodes() {
    return &nodes;
  }

};

  
#endif  // _HOME_STUDENT___RESOURCES___GRAPH_H_
