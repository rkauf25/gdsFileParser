#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>
#include <cstdlib>

#include "Removal.h"

using namespace std;


vector<int> width;
vector<pair<int,int>> corners;

string replace(string s, 
               char c1, char c2)
{
    int l = s.length();
 
    // loop to traverse in the string
    for (int i = 0; i < l; i++) 
    {
        // check for c1 and replace
        if (s[i] == c1)
            s[i] = c2;
 
        // check for c2 and replace
        else if (s[i] == c2)
            s[i] = c1;
    }
    return s;
}

vector<pair<int, int>> find_corners(istream &input, int iterations) {
    string temp_str;
    stringstream y;
    vector<int> coords;
    vector<pair<int, int>> path_corners;
    pair<int, int> curr_path;

    for(int i = 0; i < iterations; ++i) {
        getline(input, temp_str, ')');
        temp_str.erase(0, 1);
        temp_str = replace(temp_str, '\n', ',');
        coords.push_back(atoi(temp_str.substr(0, temp_str.find('\n')).c_str()));
        coords.push_back(atoi(temp_str.substr(temp_str.find('\n'), temp_str.size()).c_str()));
    }

    for(int i = 0; i < coords.size() - 1; i += 2) {
        curr_path.first = coords[i];
        curr_path.second = coords[i + 1];
        path_corners.push_back(curr_path);
    }

    return path_corners;
}

void find_path(istream &input, int layer, vector<int>& removed_paths, int& paths_seen) {
    string line;
    string junk;
    string read_layer;
    int num_coords;

    while(true) {
        while(line != "Path start") {
            getline(input, line);
    
            if(!input.good()) {
                return;
            }
        }
        line = "";

        input >> junk >> read_layer; //ingoring beginning of line (always "Layer:")

        if(atoi(read_layer.c_str()) == layer) {
            // int x = rand() % 5;
            int x = 1;
            if(x == 1) {
                break;
            }
        }

        ++paths_seen;
    }

    removed_paths.push_back(paths_seen);
    ++paths_seen;

    getline(input, junk); // reading endline
    getline(input, junk); // Data Type line (ignored)

    int curr_width;
    input >> junk >> curr_width; //reading width of the path and ignoring "Width:"

    getline(input, junk); // reading endline
    
    input >> junk >> num_coords; //reading number of coordinates

    getline(input, junk); // reading endline

    vector<pair<int,int>> temp = find_corners(input, num_coords);
    corners.insert(corners.end(), temp.begin(), temp.end());

    for(int i = 0; i < temp.size(); ++i) {
        if(atoi(read_layer.c_str()) == 15) {
            curr_width += 40;
        }

        width.push_back(curr_width);
    }

    return;

}

void find_vias(istream &input, int layer, vector<int>& removed_vias, int& vias_seen) {
    string line;
    string junk;
    string read_layer;
    int num_coords;

    while(true) {
        while(line != "Boundry start") {
            getline(input, line);
    
            if(input.eof()) {
                return;
            }
        }
        line = "";

        input >> junk >> read_layer; //ingoring beginning of line (always "Layer:")

        if(atoi(read_layer.c_str()) == layer + 1 || atoi(read_layer.c_str()) == layer - 1) { //checking that the layer is one higher or lower than the path
            break;
        }

        ++vias_seen;
    }


    vector<pair<int,int>> via_corners;

    getline(input, junk); // reading endline
    getline(input, junk); // Data Type line (ignored)
    
    input >> junk >> num_coords; //reading number of coordinates

    getline(input, junk); // reading endline

    via_corners = find_corners(input, num_coords);

    bool inside_path = false;

    for(int i = 0; i < via_corners.size(); ++i) { //check if a corner is inside the path and if one is then the via needs to be removed
        for(int j = 0; j < corners.size() - 1; ++j) {
            if(corners[j].first == corners[j + 1].first) { //path is vertical
                if((via_corners[i].first >= corners[j].first - (width[j] / 2) && via_corners[i].first <= corners[j].first + (width[j] / 2)) //checking x coords
                    && (via_corners[i].second >= min(corners[j].second, corners[j + 1].second) && via_corners[i].second <= max(corners[j].second, corners[j + 1].second))) {  //checing y coords
                        inside_path = true;
                        break;
                }
            } else {
                if((via_corners[i].second >= corners[j].second - (width[j] / 2) && via_corners[i].second <= corners[j].second + (width[j] / 2)) //checking y coords
                    && (via_corners[i].first >= min(corners[j].first, corners[j + 1].first) && via_corners[i].first <= max(corners[j].first, corners[j + 1].first))) {  //checing x coords
                        inside_path = true;
                        break;
                }
            }
        }
    }

    if(inside_path) {
        removed_vias.push_back(vias_seen);
    }
    ++vias_seen;

    return;
}


void remove_path(int layer, string pathName, vector<int> &removed_paths, vector<int> &removed_vias) {

    std::ifstream fileIn(pathName, std::ios::in);
    if (!fileIn.is_open()) {
      std::cout << "Input Human Readable GDS File not able to be read in\n";
      exit(1);
    }

    stringstream copy;
    stringstream first_pass;
    stringstream second_pass;
    string junk;

    while(!fileIn.eof()) {
        getline(fileIn, junk);
        copy << junk << endl;
    }

    first_pass << copy.str();
    second_pass << copy.str();

    int paths_seen = 0;
    while(!first_pass.eof()) {
        find_path(first_pass, layer, removed_paths, paths_seen);
    }

    if (!removed_paths.empty()) {
        int vias_seen = 0;
        while(!second_pass.eof()) {
            find_vias(second_pass, layer, removed_vias, vias_seen);
        }
    }

    fileIn.close();

    return;
}