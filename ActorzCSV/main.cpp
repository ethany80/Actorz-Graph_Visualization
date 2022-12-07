#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <unordered_map>

using namespace std;

bool validName(string& name);

int main() {
    // stores movie - actor pairs
    unordered_map<string, vector<string>> movie_actor;
    // maps movie_id to movie_title and movie_rating
    unordered_map<string, pair<string, string>> movie_map;
    
    // FILE 1 CREDITS : parse in movies and actor pairs from credits csv file
    ifstream input{ "credits.csv" };
    if (input.is_open()) {
        cout << "READING CREDITS.CSV" << endl;
        string line;
        getline(input, line);

        // while loop iterates for each movie in data set
        while (getline(input, line)) {
            int line_size = line.length();
            int ind = line_size - 1;

            // get movie id, which is at the end of line
            string movie_id = "";
            while (isdigit(line[ind])) {
                movie_id = line[ind] + movie_id;
                ind--;
            }

            // ignore the cast
            line = line.substr(0, line.find("}]"));

            // add actors to set
            while (line.find("'name'") != string::npos) {
                line = line.substr(line.find("'name'") + 9);
                string name = line.substr(0, line.find("'"));
                if (validName(name)) {
                    movie_actor[movie_id].push_back(line.substr(0, line.find("'")));
                }
            }
        }
    } 
    input.close();
    

    // FILE 2 METADATA : links movie_id to movie title
    input.open("movies_metadata.csv");
    if (input.is_open()) {
        cout << "READING MOVIES_METADATA.CSV" << endl;
        string line;
        getline(input, line);

        // parse through each column in entry
        while (getline(input, line)) {
            string movie_id = line.substr(0, line.find(','));
            for (int i = 0; i < 3; i++) {
                line = line.substr(line.find(',') + 1);
            }
            string movie_rating = line.substr(0, line.find(','));

            // convert movie rating from scale /10 to /100 ex. "6.8" -> "68"
            if (movie_rating.find('.') != string::npos) {
                movie_rating = movie_rating.substr(0, movie_rating.find('.')) + movie_rating.substr(movie_rating.find('.') + 1);
            }
            else {
                if (movie_rating != "0")
                    movie_rating += '0';
            }

            for (int i = 0; i < 1; i++) {
                line = line.substr(line.find(',') + 1);
            }
            string movie_name = line;

            // sorts out movies with foreign characters
            bool valid_name = true;
            for (auto c : movie_name) {
                if (static_cast<unsigned char>(c) > 127)
                    valid_name = false;
            }
            if (valid_name) {
                movie_map[movie_id] = make_pair(movie_name, movie_rating);
            }
        }
    }
    
    //WRITE : A new csv file with only movie and actors (a list separated by '|') columns
    ofstream output("movieActor.csv");
    if (output.is_open()) {
        cout << "WRITING MOVIEACTORTEST.CSV" << endl;
        output << "Movie ID,Movie Name,Movie Rating,Actors\n";

        // writes each entry
        for (auto p : movie_actor) {
            if (movie_map.count(p.first) != 0) {
                output << p.first << ",|" << movie_map[p.first].first << "|," << movie_map[p.first].second << ",";
                for (int i = 0; i < p.second.size(); i++) {
                    output << p.second[i];
                    if (i != p.second.size() - 1)
                        output << "|";
                }
                output << "\n";
            }    
        }
    }

    cout << "COMPLETED" << endl;
    return 0;
}

// checks if name contains only alpha characters and space
bool validName(string& name) {
    for (unsigned char c : name) {
        if (!isalpha(c) && c != ' ' && c != '.')
            return false;
    }
    return true;
}