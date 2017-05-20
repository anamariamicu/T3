#ifndef __STRUCT_H__
#define __STRUCT_H__

struct movie_popularity {
	std::string movie_id;
	int number_ratings;

	movie_popularity();
	movie_popularity(std::string movie_id, int number_ratings);

	friend bool operator>(const movie_popularity& X, const movie_popularity& Y);
	friend bool operator<(const movie_popularity& X, const movie_popularity& Y);
};

struct actor_partner {
	std::string actor_id;
	int number_movies;

	actor_partner();
	actor_partner(std::string actor_id, int number_movies);

	friend bool operator>(const actor_partner& X, const actor_partner& Y);
	friend bool operator<(const actor_partner& X, const actor_partner& Y);

};

struct actor_pair {
	std::string actor_id1;
	std::string actor_id2;
	int number_movies;

	actor_pair();
	actor_pair(std::string actor_id1, std::string actor_id2, int number_movies);
	std::string get_info();

	friend bool operator>(const actor_pair& X, const actor_pair& Y);
	friend bool operator<(const actor_pair& X, const actor_pair& Y);
};

class recent_movies {
public:
	std::string movie_id;
	int timestamp;

	friend bool operator>(const recent_movies& X, const recent_movies &Y);
    friend bool operator<(const recent_movies& X, const recent_movies &Y);

    recent_movies();
    recent_movies(std::string movie_id, int timestamp);
    ~recent_movies(){

    }
};

struct movie {
  std::string name;
  std::string id;
  int timestamp;
  std::string director;
  std::vector<std::string> actor_ids; // ?
  std::vector<std::string> categories;
  int number_ratings;
  int total_rating;

  movie(std::string name, std::string id, int timestamp, std::vector<std::string> categories,
  	    std::string director, std::vector<std::string> actor_ids);
  void add_rating(int rating);
  void remove_rating(int rating);
  std::string get_rating();
  int get_year();
};

struct user {
  std::string name;
  std::string id;
  std::unordered_map<std::string, int> ratings;

  user(std::string name, std::string id);
  void add_rating(std::string movie_id, int rating);
  int remove_rating(std::string movie_id);
};

struct actor {
  std::string name;
  std::string id;
  struct movie *first_movie;
  struct movie *last_movie;

  actor(std::string name, std::string id);
  actor(std::string id);
  void add_movie(struct movie*, struct actor**);
  int career_timestamp();
};

struct director {
	std::string name;
	unsigned int number_actors;

	director(std::string name, unsigned int number_actors);
};

struct ratings {
	int number_ratings;
	int total_rating;

	ratings();
	ratings(int rating);
	void add_rating(int rating);
	double get_average_rating();
};
#endif
