#include <unordered_map>
#include <iostream>
#include <string>
#include <set>
#include <fstream>
#include <sstream>
#include <string>

#include "Bridges.h"
#include "ActorList.h"
#include "GraphAdjList.h"

using namespace std;
using namespace bridges;

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
		cout << endl << endl;
		//actorList.pathBFS("Jennifer Salt", "Jason Priestley");
		// read 100 lines
	}

	string srcActor;
	string destActor;

  while (true)
  {
    //create bridges object for visualization
    Bridges bridges(1, "ethany80", "148411902362");
    //set title for visualization
    bridges.setTitle("Actors Graph Visualization");
    //parse in user input for traversal
    int option;
    cout << "Choose Traversal:\n1. BFS\n2. DFS\n3. Exit" << endl;
    cin >> option;
    //create bridges adj list object
    GraphAdjList<string, int, pair<string, int>> trav;
    string source, destination;
  
    cin.ignore();
    //parse in user input for actors
    cout << "Choose source actor: ";
    getline(cin, source);
    cout << "Choose destination actor: ";
    getline(cin, destination);
    cout << endl;
    //vector for traversal result
    vector<pair<string,int>> v;
    //function call based on chosen traversal,
    //visualization description set based on choice
    if (option == 3)
    {
      break;
    }
    else if (option == 1)
    {
      v = actorList.pathBFS(source,destination);
      
      bridges.setDescription("Visualization Depicts BFS Traversal");
    }
    else
    {
      v = actorList.pathDFS(source,destination);

      bridges.setDescription("Visualization Depicts DFS Traversal");
    }
  //insert vertices into GraphAdjList from
  //original unordered_map adj list in AdhList.h,
  //format size and color of initial and terminal
  //vertices
    for (auto i : v)
      {
        trav.addVertex(i.first);
      }
  
    trav.getVisualizer(v[0].first)->setSize(20);
    trav.getVisualizer(v[0].first)->setColor("red");
    trav.getVisualizer(v[v.size()-1].first)->setSize(20);
    trav.getVisualizer(v[v.size()-1].first)->setColor("red");
  //insert and fromat edges
    for (int i = 0; i < v.size()-1; ++i)
      {
        trav.addEdge(v[i].first, v[i+1].first, actorList._movies[v[i+1].second]);
        string label = actorList._movies[v[i+1].second].first + "\nRating: " + to_string(actorList._movies[v[i+1].second].second) + "/100";
        trav.getLinkVisualizer(v[i].first, v[i+1].first)->setLabel(label);
        trav.getLinkVisualizer(v[i].first, v[i+1].first)->setColor("black");
      }
  //set created data structure into bridges
  //object and create visualization
    bridges.setDataStructure(&trav);
    cout << endl;
    bridges.visualize();
  }
	return 0;
}