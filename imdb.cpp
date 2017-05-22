#include <iterator>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <iostream>
#include <cstring>
#include <algorithm>
#include "include/graph.h"
#include "include/struct.h"
#include "include/imdb.h"
#include "include/heap.h"

#define N 100

int compare_popularity(struct movie *X, struct movie *Y) {
	if (X->number_ratings < Y->number_ratings) {
		return -1;
	} else if (X->number_ratings > Y->number_ratings) {
		return 1;
	}

	// Cazul de egalitate al numarului de rating-uri determina compararea
	// id-urilor
	if (X->id > Y->id) {
		return -1;
	} else if (X->id < Y->id) {
		return 1;
	}
	return 0;
}

int compare_timestamp(struct movie *X, struct movie *Y) {

	// Timestamp-urile sunt unice, deci se compara doar acestea
	if (X->timestamp > Y->timestamp) {
		return 1;
	}
	if (X->timestamp < Y->timestamp) {
		return -1;
	}
	return 0;
}

IMDb::IMDb() :popular_movies(N, &compare_popularity), recent_movies(N, &compare_timestamp) {
	most_influential_director = nullptr;
	longest_career_actor = nullptr;
	top_popularity_has_changed = false;
	top_recent_movies_has_changed = false;
}

IMDb::~IMDb() {}

void IMDb::add_movie(std::string movie_name,
                     std::string movie_id,
                     int timestamp,
                     std::vector<std::string> categories,
                     std::string director_name,
                     std::vector<std::string> actor_ids) {

    // Adaugarea filmului in Hashtable-ul de filme
    struct movie new_movie(movie_name, movie_id, timestamp, categories,
                           director_name);
    movies.insert(std::make_pair(movie_id, new_movie));
    struct movie *movie_searched = &(movies.find(movie_id)->second);

    // Prelucrarea datelor despre actori
    int contor = 0;
    for (auto it = actor_ids.begin(); it != actor_ids.end(); ++it) {
        	actors.find(*it)->second.add_movie(movie_searched, &longest_career_actor);

        // Adaugarea informatiilor in graful de actori
	// Actorul nu exista in graf, deci se adauga
	if (colleagues.hasNode(*it) == false) {
		colleagues.addNode(*it);
	}
	contor++;
	for (auto it2 = actor_ids.begin() + contor; it2 != actor_ids.end() && *it2 != *it; ++it2) {

		// Daca celalalt actor nu exista in graf, se adauga
		if (colleagues.hasNode(*it2) == false) {
			colleagues.addNode(*it2);
		}
		int index_it = colleagues.getIndex(*it), index_it2 = colleagues.getIndex(*it2);

		// Daca nu au jucat in niciun film impreuna, se adauga muchia in graf
		if (colleagues.hasEdge(*it, *it2) == false) {

			colleagues.addEdge(*it, *it2, index_it2, 1);
			colleagues.addEdge(*it2, *it, index_it, 1);
		} else {

		// Daca au jucat in cel putin un alt film, se mai adauga un film
			colleagues.increaseDistance(*it, *it2, index_it2, 1);
			colleagues.increaseDistance(*it2, *it, index_it, 1);
		}
	}
    }

    // Actualizarea datelor despre director
    add_director(director_name, actor_ids.size());

	// Adaugarea filmului in Hashtable-urile categoriilor in care se incadreaza
	for (auto it = categories.begin(); it!= categories.end(); ++ it){
		auto category_searched = this->categories.find(*it);

		// Daca o categorie nu exista, aceasta se creeaza
		if (category_searched == this->categories.end()) {
			std::vector<struct movie *> movie_pointers;
			movie_pointers.push_back(movie_searched);
			this->categories.insert(std::make_pair(*it, movie_pointers));
		} else {
			category_searched->second.push_back(movie_searched);
		}
	} 

	// S-a adaugat un film nou, deci top-urile se schimba
	top_popularity_has_changed = true;
	top_recent_movies_has_changed = true;
}

void IMDb::add_user(std::string user_id, std::string name) {
    struct user new_user(user_id, name);
    users.insert(std::make_pair(user_id, new_user));
}

void IMDb::add_actor(std::string actor_id, std::string name) {
    struct actor new_actor(actor_id, name);
    actors.insert(std::make_pair(actor_id, new_actor));
}

void IMDb::add_director(std::string name, unsigned int number_actors) {
	auto director_searched = directors.find(name);

	// Daca directorul nu exista, atunci se adauga in Hashtable
	if (director_searched == directors.end()) {
		struct director new_director(name, number_actors);
		directors.insert(std::make_pair(name, new_director));
		director_searched = directors.find(name);
	} else {
		director_searched->second.number_actors += number_actors;
	}

	// Se verifica daca dupa noul film a devenit cel mai bun director de film
	if (most_influential_director == nullptr || 
		director_searched->second.number_actors > most_influential_director->number_actors ||
		 (director_searched->second.number_actors == most_influential_director->number_actors &&
		 	director_searched->second.name < most_influential_director->name)) {
		most_influential_director = &(director_searched->second);
	}
}

void IMDb::add_rating(std::string user_id, std::string movie_id, int rating) {

    // Se adauga rating-ul in cadrul celor date de utilizator
    auto user_searched = users.find(user_id);
    user_searched->second.add_rating(movie_id, rating);

    // Se adauga rating-ul pentru filmul respectiv
    auto movie_searched = movies.find(movie_id);
    movie_searched->second.add_rating(rating);

    // Un rating nou determina schimbari in top
	top_popularity_has_changed = true;
}

void IMDb::update_rating(std::string user_id, std::string movie_id, int rating) {
   auto user_searched = users.find(user_id);
   auto movie_searched = movies.find(movie_id);

   // Se elimina rating-ul vechi
   int rating_removed = user_searched->second.remove_rating(movie_id);
   movie_searched->second.remove_rating(rating_removed);

    // Se adauga rating-ul nou
   user_searched->second.add_rating(movie_id, rating);
   movie_searched->second.add_rating(rating);
}

void IMDb::remove_rating(std::string user_id, std::string movie_id) {

    // Se sterge rating-ul din lista de rating-uri a utilizatorului
    auto user_searched = users.find(user_id);
    int rating_removed = user_searched->second.remove_rating(movie_id);

    // Se sterge rating-ul din lista de rating-uri a filmului
    auto movie_searched = movies.find(movie_id);
    movie_searched->second.remove_rating(rating_removed); 

    // Un rating eliminat determina schimbari in top
	top_popularity_has_changed = true;
}

std::string IMDb::get_rating(std::string movie_id) {
    auto movie_searched = movies.find(movie_id);
    return movie_searched->second.get_rating();
}

std::string IMDb::get_longest_career_actor() {
    if (longest_career_actor == nullptr) {
    	return "none";
    }
    return longest_career_actor->id;
}

std::string IMDb::get_most_influential_director() {
    if (most_influential_director == nullptr) {
    	return "none";
    }
    return most_influential_director->name;
}

std::string IMDb::get_best_year_for_category(std::string category) {

    // Pentru categoria respectiva, se creeaza un Hashtable al anilor
    std::unordered_map<int, struct ratings> years;
    auto category_searched = categories.find(category);
    double max_rating = -1;
    int year_max = 0;
    if (category_searched == categories.end()) {
    	return "none";
    }

    // Hashtable-ul contine un vector de pointeri la filmele aparute in anul acela
    std::vector<struct movie *> movie_pointers = category_searched->second;
    for (auto it = movie_pointers.begin(); it != movie_pointers.end(); ++it) {
    	if ((*it)->get_rating() != "none") {
    		auto year_searched = years.find((*it)->get_year());

		// Daca anul nu exista in Hashtable, se adauga
    		if (year_searched == years.end()) {
	    		struct ratings new_ratings(std::stod((*it)->get_rating()));
	    		years.insert(std::make_pair((*it)->get_year(), new_ratings));
	    	} else {
	    		year_searched->second.add_rating(std::stod((*it)->get_rating()));
    		}
	}
    }

    // Se calculeaza anul cu cel mai bun rating mediu
    for (auto it = movie_pointers.begin(); it != movie_pointers.end(); ++it) {
	if ((*it)->get_rating() != "none") {
		auto year_searched = years.find((*it)->get_year());
		if (year_searched->second.get_average_rating() > max_rating ||
			    (year_searched->second.get_average_rating() == max_rating &&
			    			year_searched->first < year_max)) {
			    			max_rating = year_searched->second.get_average_rating();
			    			year_max = year_searched->first;
			}
		}
	}
	if (max_rating == -1) {
    	return "none";
	}
    return std::to_string(year_max);
}

std::string IMDb::get_2nd_degree_colleagues(std::string actor_id) {
    std::vector<std::string> all_second_colleagues;
    std::string result = "";

    // Se cauta colegii de gradul intai
    std::list<struct data<std::string, int>> *first_colleagues;
    first_colleagues = colleagues.getNeighbors(actor_id);
    if (first_colleagues == nullptr) {
        return "none";
    }

    // Se cauta colegii de gradul doi
    for (auto it = first_colleagues->begin(); it != first_colleagues->end(); ++it) {
    	std::list<struct data<std::string, int>> *second_colleagues;
    	second_colleagues = colleagues.getNeighbors(it->key);
      	for (auto it2 = second_colleagues->begin(); it2 != second_colleagues->end(); ++it2) {

		// Nu trebuie sa fie colegi de gradul intai!
    		if (actor_id != it2->key && colleagues.hasEdge(actor_id, it2->key) == false) {
			auto found = std::find(std::begin(all_second_colleagues), std::end(all_second_colleagues), it2->key);

			// Se adauga la vectorul de colegi de gradul doi
			if (found == std::end(all_second_colleagues)) {
			 	all_second_colleagues.push_back(it2->key);
			}
    		}
    	}
    }

    // Se sorteaza crescator vectorul de colegi de gradul doi
    std::sort(all_second_colleagues.begin(), all_second_colleagues.end());
    for (auto it = all_second_colleagues.begin(); it != all_second_colleagues.end(); ++it) {
    	result = result + *it + " ";
    }
    if (result == "") {
        return "none";
    }
    result.resize(result.size() - 1);
    return result;
}

std::string IMDb::get_top_k_most_recent_movies(int k) {

    // Daca s-au modificat top-ul, atunci acesta se reface
    if (top_recent_movies_has_changed == true) {
    	recent_movies.deleteNodes();
    	for (auto it = movies.begin(); it != movies.end(); ++ it) {
    		recent_movies.insert(&(it->second));
    	}
   	top_recent_movies_has_changed = false;
    }
    std::string result = "";
    int i = 0;

    // Se realizeaza o copie a Heap-ului pentru putea extrage maximele
    Heap<struct movie *> copy_recent_movies = recent_movies;
    while (i < k) {
		if (copy_recent_movies.hasNodes() == true) {
	     		result = result + copy_recent_movies.extractMax()->id + " ";
		}
		++i;
    }
    if (result == "") {
   		return "none";
    }
    result.resize(result.size() - 1);
    return result;
}

int compare_actor_pair(struct actor_pair X, struct actor_pair Y) {
	if (X.number_movies > Y.number_movies) {
		return 1;
	} else if (X.number_movies < Y.number_movies) {
		return -1;
	}

	// Compararea id-urilor in caz de egalitate a numarului de filme
	if (X.actor_id1 < Y.actor_id1) {
		return 1;
	} else if (X.actor_id1 > Y.actor_id1) {
		return -1;
	} 
	if (X.actor_id2 < Y.actor_id2) {
		return 1;
	} else if (X.actor_id2 > Y.actor_id2) {
		return -1;
	}
	return 0;
}

std::string IMDb::get_top_k_actor_pairs(int k) {
   	Heap<struct actor_pair> all_actor_pairs(N, &compare_actor_pair);
   	std::list<struct Node<std::string>> *actor_nodes = colleagues.getNodes();
   	int **actors_matrix, number_actors = actor_nodes->size();

	// Definirea unei matrici de adiacenta a actorilor
   	actors_matrix = new int*[number_actors];
   	for (int i = 0 ; i < number_actors; ++i) {
   		actors_matrix[i] = new int[number_actors];
   		for (int j = 0; j < number_actors; ++j) {
   			actors_matrix[i][j] = 0;
   		}
   	}

	// Formarea tuturor perechilor posibile si adaugarea lor in Heap
   	for (auto it = actor_nodes->begin(); it != actor_nodes->end(); ++it) {
   		std::list<struct data<std::string, int>>* neighbours;
   		neighbours = colleagues.getNeighbors(it->nodeValue);
   		if (neighbours != nullptr) {
	   		for (auto it2 = neighbours->begin(); it2 != neighbours->end(); ++ it2) {
				// Daca nu exista in Heap, se adauga
	   			if (actors_matrix[it2->nodeIndex][it->nodeIndex] == 0) {
	   				struct actor_pair new_actor_pair(it->nodeValue, it2->key, it2->value);
	   				all_actor_pairs.insert(new_actor_pair);
	   				actors_matrix[it2->nodeIndex][it->nodeIndex] = 1;
	   				actors_matrix[it->nodeIndex][it2->nodeIndex] = 1;
	   			}
	   		}
   		}
   	}
   	std::string result = "";
   	int count = 0;
   	while (count < k) {
   		if (all_actor_pairs.hasNodes()) {
   			result = result + all_actor_pairs.extractMax().get_info() + " ";

   		}
   		++ count;
   	}

	// Eliberarea memoriei ocupate de matricea de adiacenta
   	for (int i = 0; i < number_actors; ++i) {
   		delete[] actors_matrix[i];
   	}
   	delete[] actors_matrix;
   	if (result == "") {
   		return "none";
   	}
   	result.resize(result.size() - 1);
   	return result;
}

int compare_partners(struct data<std::string, int> *X, struct data<std::string, int> *Y) {
	if (X->value > Y->value) {
		return 1;
	} else if (X->value < Y->value) {
		return -1;
	}

	// Daca numarul de filme este egal, se compara id-urile
	if (X->key < Y->key) {
		return 1;
	} else if (X->key > Y->key) {
		return -1;
	}
	return 0;
}

std::string IMDb::get_top_k_partners_for_actor(int k, std::string actor_id) {
    Heap<struct data<std::string, int> *> all_partners(N, &compare_partners);

    // Preluarea tuturor actorilor cu care a jucat si introducerea lor in graf
    std::list<struct data<std::string, int>> *neighbours;
    neighbours = colleagues.getNeighbors(actor_id);
    if (neighbours != nullptr) {
    	for (auto it = neighbours->begin(); it != neighbours->end(); ++ it) {
    		all_partners.insert(&(*it));
    	}
    }
    int count = 0;
    std::string result = "";
    while (count < k) {
    	if (all_partners.hasNodes()) {
    		result = result + all_partners.extractMax()->key + " ";
    	}
    	++ count;
    }
    if (result == "") {
    	return "none";
    }
    result.resize(result.size() - 1);
    return result;
}


std::string IMDb::get_top_k_most_popular_movies(int k) {

    // Daca top-ul s-a schimbat, atunci el se reface
    if (top_popularity_has_changed == true) {
    	popular_movies.deleteNodes();
    	for (auto it = movies.begin(); it != movies.end(); ++ it) {
    		popular_movies.insert(&(it->second));
    	}
   	top_popularity_has_changed = false;
    }

    // Crearea unei copii pentru a putea extrage maximele
    Heap<struct movie *> copy_popular_movies = popular_movies;
    int count = 0;
    std::string result = "";
    while (count < k) {
    	if (copy_popular_movies.hasNodes()) {
    		result = result + copy_popular_movies.extractMax()->id + " ";
    	}
    	++ count;
    }
    if (result == "") {
    	return "none";
    }
    result.resize(result.size() - 1);
    return result;
}

std::string IMDb::get_avg_rating_in_range(int start, int end) {
  double sum_ratings = 0;
  int number_ratings = 0;

    // Parcurgerea filmelor si testarea apartenentei in interval
    for (auto it = movies.begin(); it != movies.end(); ++it) {
      if (it->second.timestamp >= start && it->second.timestamp <= end) {
        double rating = it->second.get_not_rounded_rating();
        if (rating != -1) {
          sum_ratings += rating;
          ++number_ratings;
        }
      }
    }
    if (number_ratings == 0) {
      return "none";
    }

    // Calcularea mediei mediilor cu doua zecimale
    double average_rating = (double) sum_ratings / number_ratings;
  average_rating = roundf(average_rating * 100) / 100;
  std::string average_rating_string = std::to_string(average_rating);
  average_rating_string.resize(average_rating_string.size() - 4);
  return average_rating_string;
}

