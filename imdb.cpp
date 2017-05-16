#include <iterator>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <iostream>
#include "struct.h"
#include "imdb.h"

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
    this->movie_ids.push_back(movie_id); // ?
    struct movie *movie_searched;
    for (auto it = actor_ids.begin(); it != actor_ids.end(); ++it) {
		auto actor_searched = actors.find(*it);
		if (actor_searched == actors.end()) {
			struct actor new_actor(*it);
			actors.insert(std::make_pair(*it, new_actor));
		}
		movie_searched = &(movies.find(movie_id)->second);
        actors.find(*it)->second.add_movie(movie_searched);
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
	auto actor_searched = actors.find(actor_id);
    if (actor_searched == actors.end()) {
    	struct actor new_actor(actor_id, name);
    	actors.insert(std::make_pair(actor_id, new_actor));
    	actor_searched = actors.find(actor_id);
    } else {
		actor_searched->second.name = name;
		actor_searched->second.on_site = true;
    }
    this->actor_ids.push_back(actor_id); // ?
    if (longest_career_actor == nullptr ||
    	actor_searched->second.career_timestamp() > longest_career_actor->career_timestamp() ||
    	(actor_searched->second.career_timestamp() == longest_career_actor->career_timestamp() &&
    		actor_searched->first < longest_career_actor->name)) {
    	longest_career_actor = &(actor_searched->second);
    }

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
    double max_rating = 0;
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
	    	if (year_searched->second.get_average_rating() > max_rating ||
	    		(year_searched->second.get_average_rating() == max_rating &&
	    			year_searched->first < year_max)) {
	    			max_rating = year_searched->second.get_average_rating();
	    			year_max = year_searched->first;
	    	}
		}
	}
    if (year_max == 0) {
    	return "none";
    }
    return std::to_string(year_max);
}

std::string IMDb::get_2nd_degree_colleagues(std::string actor_id) {
    return "";
}

std::string IMDb::get_top_k_most_recent_movies(int k) {
    return "";
}

std::string IMDb::get_top_k_actor_pairs(int k) {
    return "";
}

std::string IMDb::get_top_k_partners_for_actor(int k, std::string actor_id) {
    return "";
}

std::string IMDb::get_top_k_most_popular_movies(int k) {
    return "";
}

std::string IMDb::get_avg_rating_in_range(int start, int end) {
    return "";
}
