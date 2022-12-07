#include <unordered_map>
#include <iostream>
#include <string>
#include <set>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>
#include "ActorList.h"

using namespace std;

int main() {
	ActorList actorList;

	ifstream input("movieActor.csv");
	if (input.is_open()) {
		cout << "Reading Movie Data..." << endl;
		string line;
		getline(input, line);

		int bbb = 0;
		while(getline(input, line)){
			bbb++;

			string movie_id;
			string movie_name;
			vector<string> actorsInMovie;
			string rating;

			movie_id = line.substr(0, line.find(','));
			line = line.substr(line.find(',') + 2);
			movie_name = line.substr(0, line.find('|'));
			line = line.substr(line.find('|') + 2);
			rating = line.substr(0, line.find(','));
			line = line.substr(line.find(',') + 1);
			
			while (line.find('|') != string::npos) {
				actorsInMovie.push_back(line.substr(0, line.find('|')));
				line = line.substr(line.find('|') + 1);
			}
			actorsInMovie.push_back(line);

			//storing the movie and actors into the hashmaps
			actorList.insert(actorsInMovie, stoi(movie_id), movie_name, stoi(rating));
			

		}
		cout << "ACTOR SIZE : " << actorList.getActorCount() << endl << endl;
		//actorList.pathBFS("Jennifer Salt", "Jason Priestley");
		// read 100 lines
	}

	string srcActor;
	string destActor;
	while (true) {
		cout << "----------------------------------" << endl << endl;
		cout << "Enter source actor: ";
		getline(cin, srcActor);
		cout << "Enter destination actor: ";
		getline(cin, destActor);
		
		cout << endl << " B F S " << endl;
		auto start = chrono::high_resolution_clock::now();
		actorList.pathBFS(srcActor, destActor);
		auto stop = chrono::high_resolution_clock::now();
		auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

		cout << "BFS path search time: " << duration.count() << "ms" << endl;

		cout << endl << " D F S " << endl;
		start = chrono::high_resolution_clock::now();
		actorList.pathDFS(srcActor, destActor);
		stop = chrono::high_resolution_clock::now();
		duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

		cout << "DFS path search time: " << duration.count() << "ms" << endl << endl;

	}
		
	return 0;
}
