// Copyright 2017 Bukkosi George - Daniel, Micu Ana - Maria
#ifndef __IMDB__H__  // _HOME_STUDENT_RESOURCES_INCLUDE_IMDB_H_
#define __IMDB__H__  // _HOME_STUDENT_RESOURCES_INCLUDE_IMDB_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <map>
#include <utility>

#include "./graph.h"
#include "./struct.h"
#include "./heap.h"

#define NONE "none"

#define ADD_MOVIE "add_movie"
#define ADD_USER "add_user"
#define ADD_ACTOR "add_actor"
#define ADD_RATING "add_rating"
#define UPDATE_RATING "update_rating"
#define REMOVE_RATING "remove_rating"
// queries
#define GET_RATING "get_rating"
#define GET_LONGEST_CAREER_ACTOR "get_longest_career_actor"
#define GET_MOST_INFLUENTIAL_DIRECTOR "get_most_influential_director"
#define GET_BEST_YEAR_FOR_CATEGORY "get_best_year_for_category"
#define GET_2ND_DEGREE_COLLEAGUES "get_2nd_degree_colleagues"
#define GET_TOP_K_MOST_RECENT_MOVIES "get_top_k_most_recent_movies"
#define GET_TOP_K_ACTOR_PAIRS "get_top_k_actor_pairs"
#define GET_TOP_K_PARTNERS_FOR_ACTOR "get_top_k_partners_for_actor"
#define GET_TOP_K_MOST_POPULAR_MOVIES "get_top_k_most_popular_movies"
#define GET_AVG_RATING_IN_RANGE "get_avg_rating_in_range"

class IMDb {
 public:
  IMDb();
  ~IMDb();

  void add_movie(std::string movie_name, std::string movie_id, int timestamp,
                 std::vector<std::string> categories, std::string director_name,
                 std::vector<std::string> actor_ids);

  void add_user(std::string user_id, std::string name);
  void add_actor(std::string actor_id, std::string name);
  void add_director(std::string name, unsigned int number_actors);

  void add_rating(std::string user_id, std::string movie_id, int rating);
  void update_rating(std::string user_id, std::string movie_id, int rating);
  void remove_rating(std::string user_id, std::string movie_id);

  // queries
  std::string get_rating(std::string movie_id);
  std::string get_longest_career_actor();
  std::string get_most_influential_director();
  std::string get_best_year_for_category(std::string category);
  std::string get_2nd_degree_colleagues(std::string actor_id);
  std::string get_top_k_most_recent_movies(int k);
  std::string get_top_k_actor_pairs(int k);
  std::string get_top_k_partners_for_actor(int k, std::string actor_id);
  std::string get_top_k_most_popular_movies(int k);
  std::string get_avg_rating_in_range(int start, int end);

  // input reader helper.
  std::string read_input_line(const std::string &line);

 private:
  // Hashtable-uri nesortate pentru a retine toate fileme, toti actorii,
  // toti utilizatorii, toti directorii si toate categoriile.
  std::unordered_map<std::string, struct movie> movies;
  std::unordered_map<std::string, struct user> users;
  std::unordered_map<std::string, struct actor> actors;
  std::unordered_map<std::string, struct director> directors;
  std::unordered_map<std::string, std::vector<struct movie *>> categories;

  // Graf cu cost pentru a stabili relatiile dintre actori
  Graph<std::string> colleagues;

  // Pointeri necesari unor operatii de interogare
  struct director *most_influential_director;
  struct actor *longest_career_actor;

  // Heap ce stocheaza cele mai populare filme ordonate dupa numarul
  // de rating-uri si variabila ce se schimba daca top-ul se schimba
  Heap<struct movie *> popular_movies;
  bool top_popularity_has_changed;

  // Heap ce stocheaza cele mai recente filme ordonate dupa timestamp
  // si variabila ce se schimba daca top-ul de schimba
  Heap<struct movie *> recent_movies;
  bool top_recent_movies_has_changed;
};

#endif  // _HOME_STUDENT_RESOURCES_INCLUDE_IMDB_H_
