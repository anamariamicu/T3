#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <iostream>
#include <cmath>
#include <time.h>

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

void movie::remove_rating(int rating) {
	--number_ratings;
	total_rating -= rating;
}

std::string movie::get_rating() {
	if (number_ratings == 0) {
		return "none";
	}
	double average_rating = (double) total_rating / number_ratings;
	average_rating = roundf(average_rating * 100) / 100;
	std::string average_rating_string = std::to_string(average_rating);
	average_rating_string.resize(average_rating_string.size() - 4);
	return average_rating_string;
}

int movie::get_year() {
	time_t time = timestamp;
	struct tm *ptm = gmtime(&time);
	return ptm->tm_year + 1900;
}

user::user(std::string name, std::string id) {
	this->name = name;
	this->id = id;
}

void user::add_rating(std::string movie_id, int rating) {
	ratings.insert(std::make_pair(movie_id, rating));
}

int user::remove_rating(std::string movie_id) {
	auto rating_removed = ratings.find(movie_id);
	int rating;
	if (rating_removed != ratings.end()) {
		rating = rating_removed->second;
		ratings.erase(movie_id);
		return rating;
	}
	return 0;
}

actor::actor(std::string id, std::string name) {
	this->name = name;
	this->id = id;
	first_movie = nullptr;
	last_movie = nullptr;
}

actor::actor(std::string id) {
	this->id = id;
	first_movie = nullptr;
	last_movie = nullptr;
}

int actor::career_timestamp() {
	if (first_movie == nullptr) {
		return 0;
	}
	return last_movie->timestamp - first_movie->timestamp;
}

void actor::add_movie(struct movie *new_movie) {
	if (first_movie == nullptr) {
		first_movie = new_movie;
		last_movie = new_movie;
	} else {
		if (new_movie->timestamp < first_movie->timestamp) {
			first_movie = new_movie;
		} else if (new_movie->timestamp > last_movie->timestamp) {
			last_movie = new_movie;
		}
	}
}

director::director(std::string name, unsigned int number_actors) {
	this->name = name;
	this->number_actors = number_actors;
}

ratings::ratings() {
	number_ratings = 0;
	total_rating = 0;
}

ratings::ratings(int rating) {
	number_ratings = 1;
	total_rating = rating;
}

void ratings::add_rating(int rating) {
	++number_ratings;
	total_rating += rating;
}

double ratings::get_average_rating() {
	if (total_rating == 0) {
		return 0;
	}
	return (double) total_rating / number_ratings;
}
