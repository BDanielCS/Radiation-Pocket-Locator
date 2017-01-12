//RadiationGraph.h

#ifndef RADIATIONGRAPH_H
#define RADIATIONGRAPH_H

#define HAS_FILE 2
#define MAX_LENGTH 24
#define VACANT -1

#include <iostream>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <map>
#include <set>

const char NORTH = 'N';
const char SOUTH = 'S';
const char EAST = 'E';
const char WEST = 'W';
const char ASCEND = 'A';
const char DESCEND = 'D';
const char YES = 'Y';
const char CENTROID = 'C';

using namespace std;


//contains the list of locations where each index in both arrays
//corresponds to the direction, then amount to travel in that direction.
//the last integer in the distances array represents the percentage at that location
struct Location {
	string coordinate;
	vector<char> directionals;
	vector<int> distances;
};

//returned from the find function.  node corresponds 
//to the node being found within the graph and value corresponds
//to the node where the value in the prompted coordinate is the same
//as the value associated with the node
struct Found {
	bool node, value;
};

//contains the location information and value with references 
//to the nearby nodes in order to establish a 3D grid of
//points
struct Node {
	Node* north = nullptr, *south = nullptr, *east = nullptr, *west = nullptr,
		*ascend = nullptr, *descend = nullptr;
	int val = VACANT;
	Location* location_info;
};

//graph which consists of dynamically allocated chunks of information
//with references to neighbors in 3D space (x,y,z)
class RadiationGraph {

public:
	RadiationGraph();
	~RadiationGraph();
	const string printOptions();
	size_t getSize();
	void add(string*);
	void display(int);
	Found* find(string*);
	const map<string, Node*> getCurrentKnowledgeBase() const;

private:
	Node* centroid = nullptr;
	map<string, Node*> knowledge_base;
	void parseCommand(string*, Node*);
	void addRecursive(Node*, Node*, Node*, int);
	int parseInt(string*, int*);
	void updateLocation(Node*, Location*, int dist);
	Node* isMatch(Location*);
};

#endif
