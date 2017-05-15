#include <string>
#include <vector>
#include <list>
#include <unordered_map>

#include "struct.h"

movie::movie(std::string name, std::string id, int timestamp, std::vector<std::string> categories,
  	         std::string director, std::vector<std::string> actor_ids) {
	this->name = name;
	this->id = id;
	this->timestamp = timestamp;
	this->director = director;
	this->categories = categories;
	this->actor_ids = actor_ids;
	number_ratings = 0;
	total_rating = 0;
}

void movie::add_rating(int rating) {
	++number_ratings;
	total_rating += rating;
}

user::user(std::string name, std::string id) {
	this->name = name;
	this->id = id;
}

void user::add_rating(std::string movie_id, int rating) {
	this->ratings.insert(std::make_pair(movie_id, rating));
}

actor::actor(std::string name, std::string id) {
	this->name = name;
	this->id = id;
}

