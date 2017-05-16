#include <iterator>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <iostream>
#include "struct.h"
#include "imdb.h"

IMDb::IMDb() {}
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
    this->movie_ids.push_back(movie_id);
    for (auto it = actor_ids.begin(); it != actor_ids.end(); ++it) {
	auto actor_searched = actors.find(*it);
	if (actor_searched == actors.end()) {
		struct actor new_actor(*it);
		actors.insert(std::make_pair(*it, new_actor));
	}
        actors.find(*it)->second.add_movie(&movies.find(movie_id)->second);
    }
        
}

void IMDb::add_user(std::string user_id, std::string name) {
    struct user new_user(user_id, name);
    users.insert(std::make_pair(user_id, new_user));
}

void IMDb::add_actor(std::string actor_id, std::string name) {
    if (actors.find(actor_id) == actors.end()) {
    	struct actor new_actor(actor_id, name);
    	actors.insert(std::make_pair(actor_id, new_actor));
    } else {
	auto actor_searched = actors.find(actor_id);
	actor_searched->second.name = name;
	actor_searched->second.on_site = true;
    }
    this->actor_ids.push_back(actor_id);
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
    int max = -1;
    std::string longest_career_actor;
    for (auto it = actor_ids.begin(); it != actor_ids.end(); ++it) {
        auto actor_searched = actors.find(*it);
        if (actor_searched->second.career_timestamp() > max || 
	   (actor_searched->second.career_timestamp() == max &&
	    *it < longest_career_actor)) {
		max = actor_searched->second.career_timestamp();
		longest_career_actor = *it;
	}
    }
    if (max == -1) {
	longest_career_actor = "none";
    }
    return longest_career_actor;
}

std::string IMDb::get_most_influential_director() {
    return "";
}

std::string IMDb::get_best_year_for_category(std::string category) {
    return "";
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
