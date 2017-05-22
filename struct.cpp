#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <iostream>
#include <cmath>
#include <time.h>

#include "include/struct.h"

// Constructorul pentru structura movie
movie::movie(std::string name, std::string id, int timestamp, std::vector<std::string> categories,
  	         std::string director) {
	this->name = name;
	this->id = id;
	this->timestamp = timestamp;
	this->director = director;
	this->categories = categories;
	number_ratings = 0;
	total_rating = 0;
}

// Schimbarile aparute in cadrul structurii movie la adaugarea unui rating
void movie::add_rating(int rating) {
	++number_ratings;
	total_rating += rating;
}

// Schimbarile aparute in cadrul structurii movie la stergerea unui rating
void movie::remove_rating(int rating) {
	--number_ratings;
	total_rating -= rating;
}

// Returneaza rating-ul mediu al unui film sub forma de string
std::string movie::get_rating() {
	if (number_ratings == 0) {
		return "none";
	}
	double average_rating = (double) total_rating / number_ratings;
	average_rating = roundf(average_rating * 100) / 100;
	std::string average_rating_string = std::to_string(average_rating);

	// Eliminarea ultimelor 4 zecimale
	average_rating_string.resize(average_rating_string.size() - 4);
	return average_rating_string;
}

// Returneaza rating-ul mediu al unui film nerotunjit, sub forma de double 
double movie::get_not_rounded_rating() {
	if (number_ratings == 0) {
		return -1;
	}
	return (double) total_rating / number_ratings;
}

// Functie necesara pentru a afla anul de aparitie al unui film
int movie::get_year() {
	time_t time = timestamp;
	struct tm *ptm = gmtime(&time);
	return ptm->tm_year + 1900;
}

// Constructorul structurii user
user::user(std::string name, std::string id) {
	this->name = name;
	this->id = id;
}

// Schimbarile aparute in cadrul structurii user la adaugarea unui rating
void user::add_rating(std::string movie_id, int rating) {
	ratings.insert(std::make_pair(movie_id, rating));
}

// Schimbarile aparute in cadrul structurii user la stergerea unui rating
int user::remove_rating(std::string movie_id) {

	// Se sterge rating-ul din lista de rating-uri ale utilizatorului
	auto rating_removed = ratings.find(movie_id);
	int rating;
	rating = rating_removed->second;
	ratings.erase(movie_id);
	return rating;
}

// Constructorii structurii actor
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

// Returneaza durata in secunde a carierei unui actor
int actor::career_timestamp() {
	if (first_movie == nullptr) {
		return -1;
	}
	return last_movie->timestamp - first_movie->timestamp;
}

// Schimbarile aparute in cadrul structurii actor in urma adaugarii unui film
void actor::add_movie(struct movie *new_movie, struct actor **longest_career_actor) {

	// Actualizarea primului, respectiv ultimului film al actorului
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

	// Se testeaza daca dupa adaugarea filmului, actorul a devenit cel cu
	// cea mai lunga cariera
	if (*longest_career_actor == nullptr ||
	    (career_timestamp() > (*longest_career_actor)->career_timestamp()) ||
	    (career_timestamp() == (*longest_career_actor)->career_timestamp() &&
	    	id < (*longest_career_actor)->id)) {
	    	*longest_career_actor = this;
	 }
}

// Constructorul structurii director
director::director(std::string name, unsigned int number_actors) {
	this->name = name;
	this->number_actors = number_actors;
}

// Constructorii structurii ratings
ratings::ratings() {
	number_ratings = 0;
	total_rating = 0;
}

ratings::ratings(double rating) {
	number_ratings = 1;
	total_rating = rating;
}

// Adaugarea unui rating la cele existente deja
void ratings::add_rating(double rating) {
	++number_ratings;
	total_rating = total_rating + rating;
}

// Aflarea mediei rating-urilor existente
double ratings::get_average_rating() {
	if (number_ratings == 0) {
		return -1;
	}
	return (double) total_rating / number_ratings;
}

// Constructorii structurii actor_pair
actor_pair::actor_pair() {
	this->actor_id1 = "";
	this->actor_id2 = "";
	this->number_movies = 0;
}

actor_pair::actor_pair(std::string actor_id1, std::string actor_id2, int number_movies) {

	// Definirea id-urilor in ordine crescatoare
	if (actor_id1 < actor_id2) {
		this->actor_id1 = actor_id1;
		this->actor_id2 = actor_id2;
	} else {
		this->actor_id1 = actor_id2;
		this->actor_id2 = actor_id1;
	}
	this->number_movies = number_movies;
}

// Returnarea rezultatului in formatul potrivit
std::string actor_pair::get_info() {
	std::string result = "(" + actor_id1 + " " + actor_id2 + " " + std::to_string(number_movies) + ")";
	return result;
}
