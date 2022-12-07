#pragma once

#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <string>
#include <set>
#include <fstream>
#include <vector>
#include <utility>
#include <queue>
#include <stack>


using namespace std;

class ActorList{
public:
	//adjacency list with the actor as the key, which then has a vector of actors which were in the same movie, and the movieID of the movie they were in
	//------------actor----------------actor--movieID-------
	unordered_map<string, vector<pair<string, int>>> _actors;
	int edgeCount = 0;

	// this map will allow us to connect the movieID to the movie name
	//------------movieID--movieName-rating-------
	unordered_map<int, pair<string, int>> _movies;
 
	ActorList();
	void insert(vector<string>& actors, int movieID, string movieName, int rating);
	int getActorCount();
	void printAllEdges();
	void printAll();
	void printPath(vector<pair<string, int>>& path);
	vector<pair<string, int>> pathBFS(string actor1, string actor2); 
	vector<pair<string, int>> pathDFS(string actor1, string actor2);
	vector<pair<string, int>> pathDjikstras(string actor1, string actor2);

};

ActorList::ActorList() {}

//---------------------------------------------------------------------------------------------------------------------------------

void ActorList::insert(vector<string>& actors, int movieID, string movieName, int rating) {
	//first inserting the movie into the movies map
	_movies[movieID].first = movieName;
	_movies[movieID].second = rating;
	//now inserting the actor
	for (int i = 0; i < actors.size(); i++) {
		for (int j = 0; j < actors.size(); j++) {
			if (i != j) {
				_actors[actors[i]].push_back(make_pair(actors[j], movieID));
			}
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------------------

int ActorList::getActorCount() {
	return _actors.size();
}

//---------------------------------------------------------------------------------------------------------------------------------

void ActorList::printAllEdges() {
	for (auto i : _actors) {
		edgeCount += i.second.size();
	}
	edgeCount /= 2;
	cout << "Edge Count :" <<edgeCount << endl;
}

//---------------------------------------------------------------------------------------------------------------------------------

void ActorList::printAll() {
	for (auto i : _actors) {
		cout << i.first << ": ";
		for (auto j : i.second) {
			cout << j.first << ",";
		}
		cout << endl;
	}
}

//---------------------------------------------------------------------------------------------------------------------------------

// prints out the path from one vertice to another, given a list of steps
void ActorList::printPath(vector<pair<string, int>>& path) {
	cout << "Path from " << path[0].first << " to " << path[path.size() - 1].first << ":" << endl;

	// print each connection with the movie than connects two actors
	cout << "\t" << path[0].first << " -> ";
	for (int i = 1; i < path.size() - 1; i++) {
		cout << path[i].first << " (" << _movies[path[i].second].first<< " rating: "<< _movies[path[i].second].second << ")" << endl;
		cout << "\t" << path[i].first << " -> ";
	}
	cout << path[path.size() - 1].first << " (" << _movies[path[path.size() - 1].second].first<< " rating: " << _movies[path[path.size() - 1].second].second << ")" << endl;

	// prints the entire path on one line
	for (int i = 0; i < path.size() - 1; i++) {
		cout << path[i].first << " -> ";
	}
	cout << path[path.size() - 1].first << endl;
}

//---------------------------------------------------------------------------------------------------------------------------------

vector<pair<string, int>> ActorList::pathBFS(string actor1, string actor2) {
	// check if both actors appear in the data set
	bool valid_actors = true;
	if (_actors.count(actor1) == 0) {
		cout << actor1 << " does not appear in the data set" << endl;
		valid_actors = false;
	}
	if (_actors.count(actor2) == 0) {
		cout << actor2 << " does not appear in the data set" << endl;
		valid_actors = false;
	}
	if (!valid_actors)
		return vector<pair<string, int>>();


	queue<string> vertices;
	set<string> visited;
	queue<vector<pair<string, int>>> paths;

	vector<pair<string, int>> tempPath;
	tempPath.push_back(make_pair(actor1, 0));

	vertices.emplace(actor1);
	visited.insert(actor1);
	paths.emplace(tempPath);

	while (vertices.size()!=0) {
		tempPath = paths.front();
		for (auto i : _actors[vertices.front()]) {
			if (visited.count(i.first)==0) {
				vector<pair<string, int>> nowPath = tempPath;
				nowPath.push_back(make_pair(i.first, i.second));
				paths.emplace(nowPath);
				// destination actor is found, print + return path
				if (i.first == actor2) {
					printPath(nowPath);
					return nowPath;
				}
				visited.insert(i.first);
				vertices.emplace(i.first);
			}
		}
		vertices.pop();
		paths.pop();
	}

	cout << "No path exists" << endl;
	tempPath.clear();
	tempPath.push_back(make_pair("No path exists" , 0));
	return tempPath;
}

//---------------------------------------------------------------------------------------------------------------------------------

vector<pair<string, int>> ActorList::pathDFS(string actor1, string actor2) {
	// check if both actors appear in the data set
	bool valid_actors = true;
	if (_actors.count(actor1) == 0) {
		cout << actor1 << " does not appear in the data set" << endl;
		valid_actors = false;
	}
	if (_actors.count(actor2) == 0) {
		cout << actor2 << " does not appear in the data set" << endl;
		valid_actors = false;
	}
	if (!valid_actors)
		return vector<pair<string, int>>();


	stack<string> vertices;
	set<string> visited;
	stack<vector<pair<string, int>>> paths;

	vector<pair<string, int>> tempPath;
	tempPath.push_back(make_pair(actor1, 0));

	vertices.emplace(actor1);
	visited.insert(actor1);
	paths.emplace(tempPath);

	while (vertices.size() != 0) {
		tempPath = paths.top();
		for (auto i : _actors[vertices.top()]) {
			if (visited.count(i.first) == 0) {
				vector<pair<string, int>> nowPath = tempPath;
				nowPath.push_back(make_pair(i.first, i.second));
				paths.emplace(nowPath);
				// destination actor is found, print + return path
				if (i.first == actor2) {
					printPath(nowPath);
					return nowPath;
				}
				visited.insert(i.first);
				vertices.emplace(i.first);
			}
		}
		vertices.pop();
		paths.pop();
	}

	cout << "No path exists" << endl;
	tempPath.clear();
	tempPath.push_back(make_pair("No path exists", 0));
	return tempPath;
}


vector<pair<string, int>> ActorList::pathDjikstras(string actor1, string actor2) {
	vector<pair<string, int>> path;
	//map used to determine whether a vertex has been visited
	unordered_set <string> visited;
	unordered_set <string> unVisited;

	//map that stores all the distances from the source node, 
	unordered_map<string, int> distances;

	//stores the previous node used to calculate the distance
	unordered_map<string, string> previous;


	for (auto i : _actors) {
		unVisited.insert(i.first);
		distances[i.first] = INT_MAX;
		previous[i.first] = "NV";
	}
	visited.insert(actor1);
	unVisited.erase(actor1);
	distances[actor1] = 0;
	previous[actor1] = "first one";

	return path;
}

