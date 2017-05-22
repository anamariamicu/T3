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


IMDb::IMDb() {
	most_influential_director = nullptr;
	longest_career_actor = nullptr;
}
IMDb::~IMDb() {}

void IMDb::add_movie(std::string movie_name,
                     std::string movie_id,
                     int timestamp, // unix timestamp when movie was launched
                     std::vector<std::string> categories,
                     std::string director_name,
                     std::vector<std::string> actor_ids) {
    struct movie new_movie(movie_name, movie_id, timestamp, categories,
                           director_name, actor_ids);
    movies.insert(std::make_pair(movie_id, new_movie));
   // this->movie_ids.push_back(movie_id); // ?
    struct movie *movie_searched = &(movies.find(movie_id)->second);
int contor = 0;
    for (auto it = actor_ids.begin(); it != actor_ids.end(); ++it) {
        	actors.find(*it)->second.add_movie(movie_searched, &longest_career_actor);

        /// colleagues
	if (colleagues.hasNode(*it) == false) {
		colleagues.addNode(*it);
	}
contor++;
/*        for (auto it2 = actor_ids.begin(); it2 != actor_ids.end() && *it2 != *it; ++it2) {
		if (colleagues.hasEdge(*it, *it2) == false) { // un if in plus?
        		colleagues.addEdge(*it, *it2);
		}
		if (colleagues.hasEdge(*it2, *it) == false) {
        		colleagues.addEdge(*it2, *it);
		}
        } */
	for (auto it2 = actor_ids.begin() + contor; it2 != actor_ids.end() && *it2 != *it; ++it2) {
		if (colleagues.hasNode(*it2) == false) {
			colleagues.addNode(*it2);
		}
		int index_it = colleagues.getIndex(*it), index_it2 = colleagues.getIndex(*it2);
		if (colleagues.hasEdge(*it, *it2) == false) {

			colleagues.addEdge(*it, *it2, index_it2, 1);
			colleagues.addEdge(*it2, *it, index_it, 1);
		} else {
			colleagues.increaseDistance(*it, *it2, index_it2, 1);
			colleagues.increaseDistance(*it2, *it, index_it, 1);
		}
	}
    }
    add_director(director_name, actor_ids.size());
	for (auto it = categories.begin(); it!= categories.end(); ++ it){
		auto category_searched = this->categories.find(*it);
		if (category_searched == this->categories.end()) {
			std::vector<struct movie *> movie_pointers;
			movie_pointers.push_back(movie_searched);
			this->categories.insert(std::make_pair(*it, movie_pointers));
		} else {
			category_searched->second.push_back(movie_searched);
		}
	}  
}

void IMDb::add_user(std::string user_id, std::string name) {
    struct user new_user(user_id, name);
    users.insert(std::make_pair(user_id, new_user));
}

void IMDb::add_actor(std::string actor_id, std::string name) {
    struct actor new_actor(actor_id, name);
    actors.insert(std::make_pair(actor_id, new_actor));
 //   auto actor_searched = actors.find(actor_id);
/*    if (longest_career_actor == nullptr ||
    	(actor_searched->second.career_timestamp() > longest_career_actor->career_timestamp()) ||
    	(actor_searched->second.career_timestamp() == longest_career_actor->career_timestamp() &&
    		actor_searched->second.id < longest_career_actor->id)) {
    		longest_career_actor = &(actor_searched->second);
		std::cout << longest_career_actor->id << " acum\n";
    }
 */
}

void IMDb::add_director(std::string name, unsigned int number_actors) {
	auto director_searched = directors.find(name);
	if (director_searched == directors.end()) {
		struct director new_director(name, number_actors);
		directors.insert(std::make_pair(name, new_director));
		director_searched = directors.find(name);
	} else {
		director_searched->second.number_actors += number_actors;
	}
	if (most_influential_director == nullptr || 
		director_searched->second.number_actors > most_influential_director->number_actors ||
		 (director_searched->second.number_actors == most_influential_director->number_actors &&
		 	director_searched->second.name < most_influential_director->name)) {
		most_influential_director = &(director_searched->second);
	}
}

void IMDb::add_rating(std::string user_id, std::string movie_id, int rating) {
    auto user_searched = users.find(user_id);
    user_searched->second.add_rating(movie_id, rating);
    auto movie_searched = movies.find(movie_id);
    movie_searched->second.add_rating(rating);
}

void IMDb::update_rating(std::string user_id, std::string movie_id, int rating) {
    remove_rating(user_id, movie_id);
    add_rating(user_id, movie_id, rating);
}

void IMDb::remove_rating(std::string user_id, std::string movie_id) {
    auto user_searched = users.find(user_id);
    int rating_removed = user_searched->second.remove_rating(movie_id);
    auto movie_searched = movies.find(movie_id);
    movie_searched->second.remove_rating(rating_removed); 
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
    std::unordered_map<int, struct ratings> years;
    auto category_searched = categories.find(category);
    double max_rating = -1;
    int year_max = 0;
    if (category_searched == categories.end()) {
    	return "none";
    }
    std::vector<struct movie *> movie_pointers = category_searched->second;
    for (auto it = movie_pointers.begin(); it != movie_pointers.end(); ++it) {
    	if ((*it)->get_rating() != "none") {
    		auto year_searched = years.find((*it)->get_year());
    		if (year_searched == years.end()) {
	    		struct ratings new_ratings(std::stod((*it)->get_rating()));
	    		years.insert(std::make_pair((*it)->get_year(), new_ratings));
	    		year_searched = years.find((*it)->get_year());
	    	} else {
	    		year_searched->second.add_rating(std::stod((*it)->get_rating()));
    		}
		}
	}
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
	std::cerr << "calc for " << category << " result: " << year_max << "\n";
    return std::to_string(year_max);
}

std::string IMDb::get_2nd_degree_colleagues(std::string actor_id) {
    std::vector<std::string> all_second_colleagues;
    std::string result = "";
    std::list<struct data<std::string, int>> *first_colleagues;
    first_colleagues = colleagues.getNeighbors(actor_id);
  
    if (first_colleagues == nullptr) {
        return "none";
    }
    for (auto it = first_colleagues->begin(); it != first_colleagues->end(); ++it) {
    	std::list<struct data<std::string, int>> *second_colleagues;
    	second_colleagues = colleagues.getNeighbors(it->key);
      	for (auto it2 = second_colleagues->begin(); it2 != second_colleagues->end(); ++it2) {
    		if (actor_id != it2->key && colleagues.hasEdge(actor_id, it2->key) == false) {
			auto found = std::find(std::begin(all_second_colleagues), std::end(all_second_colleagues), it2->key);
			if (found == std::end(all_second_colleagues)) {
			 	all_second_colleagues.push_back(it2->key);
			}
    		}
    	}
    }
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

int compare_timestamp(struct movie *X, struct movie *Y) {
	if (X->timestamp > Y->timestamp) {
		return 1;
	}
	if (X->timestamp < Y->timestamp) {
		return -1;
	}
	return 0;
}

std::string IMDb::get_top_k_most_recent_movies(int k) {
    std::string result = "";
    int i = 0;
    Heap<struct movie *> most_recent_movies(100, &compare_timestamp);
    for (auto it = movies.begin(); it != movies.end(); ++it) {
		most_recent_movies.insert(&it->second);
    }
    while (i < k) {
		if (most_recent_movies.hasNodes() == true) {
	     		result = result + most_recent_movies.extractMax()->id + " ";
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
   	Heap<struct actor_pair> all_actor_pairs(100, &compare_actor_pair);
   	std::list<struct Node<std::string>> *actor_nodes = colleagues.getNodes();
   	int **actors_matrix, number_actors = actor_nodes->size();
   	actors_matrix = new int*[number_actors];
   	for (int i = 0 ; i < number_actors; ++i) {
   		actors_matrix[i] = new int[number_actors];
   		for (int j = 0; j < number_actors; ++j) {
   			actors_matrix[i][j] = 0;
   		}
   	}
   	for (auto it = actor_nodes->begin(); it != actor_nodes->end(); ++it) {
   		std::list<struct data<std::string, int>>* neighbours;
   		neighbours = colleagues.getNeighbors(it->nodeValue);
   		if (neighbours != nullptr) {
	   		for (auto it2 = neighbours->begin(); it2 != neighbours->end(); ++ it2) {
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
	if (X->key < Y->key) {
		return 1;
	} else if (X->key > Y->key) {
		return -1;
	}
	return 0;
}

std::string IMDb::get_top_k_partners_for_actor(int k, std::string actor_id) {
    Heap<struct data<std::string, int> *> all_partners(100, &compare_partners);
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

int compare_popularity(struct movie *X, struct movie *Y) {
	if (X->number_ratings < Y->number_ratings) {
		return -1;
	} else if (X->number_ratings > Y->number_ratings) {
		return 1;
	}
	if (X->id > Y->id) {
		return -1;
	} else if (X->id < Y->id) {
		return 1;
	}
	return 0;
}

std::string IMDb::get_top_k_most_popular_movies(int k) {
    Heap<struct movie *> popular_movies(100, &compare_popularity);
    for (auto it = movies.begin(); it != movies.end(); ++ it) {
    	popular_movies.insert(&(it->second));
    }
    int count = 0;
    std::string result = "";
    while (count < k) {
    	if (popular_movies.hasNodes()) {
    		result = result + popular_movies.extractMax()->id + " ";
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
    double average_rating = (double) sum_ratings / number_ratings;
	average_rating = roundf(average_rating * 100) / 100;
	std::string average_rating_string = std::to_string(average_rating);
	average_rating_string.resize(average_rating_string.size() - 4);
	return average_rating_string;
}
