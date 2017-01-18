//RadiationGraph.cpp

//The 3D graph allows the user to generate a 3d linkage of nodes
//which can be analyzed to determine where clusters are located
// and how to data stored within each node is distributed
//statistically. 
//Nodes are "deleted" by marking them as vacant in the moment
//and then performing systeming cleanings of the graph to remove any areas
//where two nodes are vacant.

//Nodes are added to the graph in a recursive manner to find the proper 
//location. Empty placeholder nodes are added to the graph in situations where
//the direction must change but there is no node present where the change in direction
//can take place

//The clustering algorithm (which more accurately described as a community
//algorithm) attempts to find groupings of nodes with at most the distance
//prompted by the user. 

#include "stdafx.h"
#include "Utility.h"
#include "boost\foreach.hpp"
#include "boost\lexical_cast.hpp"

//optimizes the IO operations upon initialization
RadiationGraph::RadiationGraph() {
	ios::sync_with_stdio(false);
	additions = 1;

	//the centroid is predefined as "C-VACANT"
	centroid = new Node;
	centroid->val = VACANT;
	centroid->location_info = new Location;
	centroid->location_info->directionals.push_back(CENTROID);
	centroid->location_info->distances.push_back(0);
	centroid->location_info->coordinate = CENTROID;

	knowledge_base.insert(pair<string, Node*>("CENTROID", centroid));
}

//deallocate the entire 3D graph by removing locations
//from the knowledge
RadiationGraph::~RadiationGraph() {

	Node* curr;

	for (auto it = knowledge_base.cbegin(); it != knowledge_base.cend();) {
		curr = it->second;

		delete curr->location_info;
		delete curr;

		knowledge_base.erase(it++);
	}
}

//return the amount of nodes created, nodes can be
//created while moving towards a desired coordinate
size_t RadiationGraph::getSize() { return knowledge_base.size(); }

//returns the number of nodes currently marked as
//vacant in the graph
int RadiationGraph::explicit_size()
{
	int empty_nodes = 0;
	typedef pair<string, Node*> entry;

	BOOST_FOREACH(entry curr, knowledge_base) {
		if (curr.second->val == VACANT) {
			empty_nodes++;
		}
	}

	return empty_nodes;
}

//of some predefined distance, print out all of the
//found unique clusters in the graph less than or equal 
//to the passed in size
void RadiationGraph::print_cluster(const int dist) {

	vector<set<Node*>> communities;
	int counter = 1;

	if (dist <= 0) {
		cout << "Invalid distance " << dist << endl;
	}
	else {

		communities = get_communities_of_size(dist);

		if (communities.size() != 0) {
			//print out all of the clusters found
			for each(set<Node*> cluster in communities) {
				cout << "Cluster " << counter << endl;

				for each(Node* curr in cluster) {
					cout << to_string(curr);
				}
				cout << endl;
				counter++;
			}
		}
		else {
			cout << "No clusters of size " << dist << " were found." << endl;
		}

	}
}

//go through the nodes of the graph and determine is there are adjacent
//nodes that satisfy the dist constraint. 
vector<set<Node*>> RadiationGraph::get_communities_of_size(const int dist) {
	vector<set<Node*>> community;
	set<Node*>* cluster;

	//get each set which consists of the evaluated cluster and dealloc memory
	for each(pair<string, Node*> curr in knowledge_base) {
		cluster = depth_first_analysis(curr.second, dist);

		//exclude clusters of self only
		if (cluster->size() > 1) {
			community.push_back(*cluster);
		}
		delete cluster;
	}

	return community;
}

//Starting at some node in the graph. Go outwards in
//all directions and see if the adjacent node is within the distance
//constraint specified in the parameter
set<Node*>* RadiationGraph::depth_first_analysis(Node* curr, const int dist) {

	set<Node*>* community = new set<Node*>;

	community->insert(curr);

	//null check even though items from knowledge base should never be null (precaution)
	if (curr != nullptr) {
		depth_first_analysis_helper(community, curr, dist);
	}
	return community;

}

// see depth_first_anlysis
void RadiationGraph::depth_first_analysis_helper(set<Node*>* community,
	Node* curr, const int dist) {

	int directional_dist = 0, current_dist = 0;

	//ascend upwards as far as possible first.
	if (curr->ascend != nullptr && curr->ascend->val != VACANT) {
		directional_dist = get_directional_dist(curr->ascend, ASCEND);
		current_dist = get_directional_dist(curr, ASCEND);

		//current should never be -1. set to zero if case arises
		if (current_dist == VACANT) {
			current_dist = 0;
		}

		if (directional_dist != VACANT && (curr->ascend->location_info->directionals
			.at(directional_dist) - curr->location_info->directionals.at(current_dist)) <= dist) {
			community->insert(curr->ascend);
			depth_first_analysis_helper(community, curr->ascend, dist);
		}
	}

	//descend as far as possible
	if (curr->descend != nullptr && curr->descend->val != VACANT) {
		directional_dist = get_directional_dist(curr->descend, DESCEND);
		current_dist = get_directional_dist(curr, DESCEND);

		//current should never be -1. set to zero if case arises
		if (current_dist == VACANT) {
			current_dist = 0;
		}

		if (directional_dist != VACANT && (curr->descend->location_info->directionals
			.at(directional_dist) - curr->location_info->directionals.at(current_dist)) <= dist) {
			community->insert(curr->descend);
			depth_first_analysis_helper(community, curr->descend, dist);
		}
	}

	//Move northward as far as possible
	if (curr->north != nullptr && curr->north->val != VACANT) {
		directional_dist = get_directional_dist(curr->north, NORTH);
		current_dist = get_directional_dist(curr, NORTH);

		//current should never be -1. set to zero if case arises
		if (current_dist == VACANT) {
			current_dist = 0;
		}

		if (directional_dist != VACANT && (curr->north->location_info->directionals
			.at(directional_dist) - curr->location_info->directionals.at(current_dist)) <= dist) {
			community->insert(curr->north);
			depth_first_analysis_helper(community, curr->north, dist);
		}
	}

	//move as far south as possible
	if (curr->south != nullptr && curr->south->val != VACANT) {
		directional_dist = get_directional_dist(curr->south, SOUTH);
		current_dist = get_directional_dist(curr, SOUTH);

		//current should never be -1. set to zero if case arises
		if (current_dist == VACANT) {
			current_dist = 0;
		}

		if (directional_dist != VACANT && (curr->south->location_info->directionals
			.at(directional_dist) - curr->location_info->directionals.at(current_dist)) <= dist) {
			community->insert(curr->south);
			depth_first_analysis_helper(community, curr->south, dist);
		}
	}

	//move as far east as possible
	if (curr->east != nullptr && curr->east->val != VACANT) {
		directional_dist = get_directional_dist(curr->east, EAST);
		current_dist = get_directional_dist(curr, EAST);

		//current should never be -1. set to zero if case arises
		if (current_dist == VACANT) {
			current_dist = 0;
		}

		if (directional_dist != VACANT && (curr->east->location_info->directionals
			.at(directional_dist) - curr->location_info->directionals.at(current_dist)) <= dist) {
			community->insert(curr->east);
			depth_first_analysis_helper(community, curr->east, dist);
		}
	}

	//go as far westward as possible
	if (curr->west != nullptr && curr->west->val != VACANT) {
		directional_dist = get_directional_dist(curr->west, WEST);
		current_dist = get_directional_dist(curr, WEST);

		//current should never be -1. set to zero if case arises
		if (current_dist == VACANT) {
			current_dist = 0;
		}

		if (directional_dist != VACANT && (curr->west->location_info->directionals
			.at(directional_dist) - curr->location_info->directionals.at(current_dist)) <= dist) {
			community->insert(curr->west);
			depth_first_analysis_helper(community, curr->west, dist);
		}
	}
}


//given one of the predefined directionals, attempt to find the distance
//associated with it.  If not found, then return error code -1 (vacant)
int RadiationGraph::get_directional_dist(Node* node, const char direct) {
	Location* loc = node->location_info;

	for (int i = 0; i < loc->directionals.size(); i++) {
		if (loc->directionals.at(i) == direct) {
			return i;
		}
	}

	return VACANT;
}

//to string, what is this.. java?
string RadiationGraph::to_string(Node* curr) {
	string to_ret;

	to_ret = curr->location_info->coordinate;
	to_ret.append(" ");
	to_ret.append(boost::lexical_cast<string>(curr->val));
	to_ret.append(" ");

	return to_ret;
}
//displays all of the nodes in the graph currently as well
// as all of the neighbors to each node. Nodes that were created 
//with the VACANT macro will also be displayed as empty
void RadiationGraph::display(int predefined_choice) {

	int choice;

	// predefined is used for testing purposes only
	if (predefined_choice == -1) {
		cout << "Would you like a list of current coordinates(1) or full details(2)?" << endl;
		cin >> choice;
	}
	else {
		choice = predefined_choice;
	}


	//list off the elements in the graph with val
	if (choice == 1) {
		for (auto &iter : knowledge_base) {
			cout << "Coordinate " << iter.first << " with a value of " << iter.second->val << endl;
		}
	}
	else if (choice == 2) {
		//full breakdown - elements with vals of all existent adjacent nodes
		for (auto &iter : knowledge_base) {
			if (iter.second->val == VACANT) {
				cout << "Coordinate " << iter.first << " is EMPTY " << endl;
			}
			else {
				cout << "Coordinate " << iter.first << " with a value of " << iter.second->val << endl;
			}


			// ASCEND value testing
			if (iter.second->ascend != nullptr) {
				//testing for empty node
				if (iter.second->ascend->val == VACANT) {
					cout << "with an empty node above it" << endl;
				}
				else {
					cout << "with a node of value " << iter.second->ascend->val << " above it" << endl;
				}
			}

			// DESCEND value testing
			if (iter.second->descend != nullptr) {
				if (iter.second->descend->val == VACANT) {
					cout << "with an empty node below it" << endl;
				}
				else {
					cout << "with a node of value " << iter.second->descend->val << " below it" << endl;
				}
			}

			// NORTH value testing
			if (iter.second->north != nullptr) {
				if (iter.second->north->val == VACANT) {
					cout << "with an EMPTY node to the north of it" << endl;
				}
				else {
					cout << "with a node of value " << iter.second->north->val << " to the north of it" << endl;
				}
			}

			// SOUTH value testing
			if (iter.second->south != nullptr) {
				if (iter.second->south->val == VACANT) {
					cout << "with an EMPTY node to the south of it" << endl;
				}
				else {
					cout << "with a node of value " << iter.second->south->val << " to the south of it" << endl;
				}
			}

			// EAST value testing
			if (iter.second->east != nullptr) {
				if (iter.second->east->val == VACANT) {
					cout << "with an EMPTY node to the east of it" << endl;
				}
				else {
					cout << "with a node of value " << iter.second->east->val << " to the east of it" << endl;
				}
			}

			// WEST value testing 
			if (iter.second->west != nullptr) {
				if (iter.second->west->val == VACANT) {
					cout << "with an EMPTY node to the west of it" << endl;
				}
				else {
					cout << "with a node of value " << iter.second->west->val << " to the west of it" << endl;
				}

			}

			cout.flush();
		}
	}
	else {
		cout << "Invalid option " << choice << endl;
	}
}

//lists out the abilities that are available
//for the graph
const std::string RadiationGraph::printOptions() {

	return "Enter the number associated with your selection:\nAdd Item(1)\n"
		"Delete(2)\nSize(3)\nDisplay(4)\nClusters(5)\nHistogram(6)\nExit(7)\n";
}

//given a dyanamically allocated node, updates its information to
//correctly correspond to the current direction and distance while
//maintaining the VACANT status
void RadiationGraph::updateLocation(Node* empty, Location* info, int index) {

	int count = 0;

	empty->location_info = new Location;
	empty->val = VACANT;

	//each "index" is actually two buckets 
	empty->location_info->coordinate = info->coordinate.substr(0, (2 * index) + 2);

	//copying the location(s) information over to the empty node
	while (count <= index) {
		empty->location_info->directionals.push_back(info->directionals[count]);
		empty->location_info->distances.push_back(info->distances[count]);
		count++;
	}
}

//given a full coordinate, determines if there is a node at that 
//coordinate with the inputted value
Found* RadiationGraph::in_graph(string *command) {

	Node* new_node = new Node;
	Found *status = new Found;
	map<string, Node*>::iterator found;
	struct Location* parsed_location = new Location;
	set<string> perms;

	//parsed the commands and create location node as normal
	new_node->location_info = parsed_location;
	parseCommand(command, new_node);
	status->has_node = false;
	status->has_value = false;

	//check all permutations just in case
	perms = Utility::permutations(parsed_location->coordinate);

	for (string coordinate : perms) {
		if ((found = knowledge_base.find(coordinate)) != knowledge_base.end()) {
			status->has_node = true;
			found->second->val == VACANT ? status->has_value = false : status->has_value = true;
		}
	}
	return status;
}

//adds a value to the graph by cycling through all
//of the directional and distance pairs until the correct
//location is reached.  Then stores the int value
void RadiationGraph::add(string *command) {

	Node* new_node = new Node;
	Node* match;
	struct Location* parsed_location = new Location;

	new_node->location_info = parsed_location;
	parseCommand(command, new_node);

	//permutation swap and check if node equivalent position
	if ((match = isMatch(parsed_location)) != nullptr) {

		//match found so update its value and free memory for unnecessary new node
		match->val = new_node->val;
		delete new_node->location_info;
		delete new_node;
		return;
	}

	//update of the centroid value
	if (new_node->location_info->directionals.at(0) == CENTROID) {
		centroid->val = new_node->val;
		knowledge_base.erase(parsed_location->coordinate);
		delete new_node->location_info;
		delete new_node;
	}
	else {
		//adding the node to the 3D graph
		addRecursive(centroid, nullptr, new_node, 0);
	}
}

//removes a node from the graph and deallocs all memory associated
//with that node.  If the centroid is targeted, then sets its value
//to vacant
void RadiationGraph::remove(string * command) {

	Node* to_remove;
	map<string, Node*>::iterator node_iterator;

	//centroid removal
	if (command->at(0) == CENTROID) {
		to_remove = knowledge_base.find("CENTROID")->second;
		to_remove->val = VACANT;
	} // we need to remove node and update all references
	else {
		to_remove = new Node;
		to_remove->location_info = new Location;
		parseCommand(command, to_remove);
		to_remove->location_info->coordinate = *command;

		//check if the node exists within the graph
		if ((node_iterator = knowledge_base.find(to_remove->location_info->coordinate))
			== knowledge_base.end()) {
		}// the node exists, mark as vacant for possible cleanup
		else {
			node_iterator->second->val = VACANT;
		}

		//delete the temp node
		delete to_remove->location_info;
		delete to_remove;
	}
}

//determine if a permutation of the inputted coordinate 
//is equivalent to one that is already in the graph
//ex. N2E2 == E2N2.  If there is no perm found, 
//then nullptr is returned
Node* RadiationGraph::isMatch(Location* loc) {

	int counter = 0;
	map<string, Node*>::iterator foundEntry;

	set<string> permutations = Utility::permutations(loc->coordinate);

	//find match
	for each(string perm in permutations) {
		if ((foundEntry = knowledge_base.find(perm)) != knowledge_base.end()) {
			return foundEntry->second;
		}
	}
	return nullptr;
}

//returns an immutable copy of the knowledge base map
const map<string, Node*> RadiationGraph::getCurrentKnowledgeBase() const {
	return knowledge_base;
}

//after finding correct directional, determine if the distance from the current
//location is less than or greater than the referenced node to see where
//the insertion will be made
void RadiationGraph::addRecursive(Node* curr, Node* prev, Node* new_node,
	int current_location) {

	bool last_coordinate = false;

	//if the last coordinate is being read then we will be placing the new node.
	current_location == new_node->location_info->directionals.size() - 1 ?
		last_coordinate = true : last_coordinate = false;

	//each location in the list of directionals, move in the desired direction.
	switch (new_node->location_info->directionals[current_location]) {

	case ASCEND:
		//we are at the end, so the addition will be made now
		if (last_coordinate) {

			//no nodes to further ascend to
			if (curr->ascend == nullptr) {
				curr->ascend = new_node;
				new_node->descend = curr;
				knowledge_base.insert(pair<string, Node*>
					(new_node->location_info->coordinate, new_node));
			}	//there is already a node here of same dist, overwrite vals
			else if (curr->ascend->location_info->distances.at(current_location) ==
				new_node->location_info->distances.at(current_location)) {
				cout << "Overwritting node..." << endl;
				curr->val = new_node->val;

				//clean up, new node not necessary
				delete new_node->location_info;
				delete new_node;
			}//the ascending is larger, insert between the curr and ascend
			else if (curr->ascend->location_info->distances.at(current_location) >
				new_node->location_info->distances.at(current_location)) {

				new_node->ascend = curr->ascend;
				curr->ascend->descend = new_node;

				curr->ascend = new_node;
				new_node->descend = curr;

				knowledge_base.insert(pair<string, Node*>
					(new_node->location_info->coordinate, new_node));
			}//ascend is smaller so move forward
			else {
				return addRecursive(curr->ascend, curr, new_node, current_location);
			}
		} //not final location continue searching
		else {
			//no node above, create placeholder and move through coordinates
			if (curr->ascend == nullptr) {
				Node* empty = new Node;
				updateLocation(empty, new_node->location_info, current_location);
				empty->descend = curr;
				curr->ascend = empty;
				knowledge_base.insert(pair<string, Node*>
					(empty->location_info->coordinate, empty));

				return addRecursive(empty, curr, new_node, ++current_location);
			}//found location, iterate to next directional
			else if (curr->ascend->location_info->distances.at(current_location) ==
				new_node->location_info->distances.at(current_location)) {
				return addRecursive(curr, prev, new_node, ++current_location);
			}//current larger; create temporary node with empty value 
			 //then continue moving through graph
			else if (curr->ascend->location_info->distances.at(current_location) >
				new_node->location_info->distances.at(current_location)) {
				Node* empty = new Node;
				updateLocation(empty, new_node->location_info, current_location);

				empty->ascend = curr;
				curr->descend = empty;

				prev->ascend = empty;
				empty->descend = prev;
				knowledge_base.insert(pair<string, Node*>
					(empty->location_info->coordinate, empty));

				return addRecursive(empty, prev, new_node, ++current_location);
			}//move forwards in current dir, the node is smaller direction val
			else {
				return addRecursive(curr->ascend, curr, new_node, current_location);
			}
		}
		break;

	case DESCEND:
		if (last_coordinate) {

			if (curr->descend == nullptr) {
				curr->descend = new_node;
				new_node->ascend = curr;
				knowledge_base.insert(pair<string, Node*>
					(new_node->location_info->coordinate, new_node));
			}	//there is already a node here of same dist, overwrite vals
			else if (curr->descend->location_info->distances.at(current_location) ==
				new_node->location_info->distances.at(current_location)) {
				cout << "Overwritting node..." << endl;
				curr->val = new_node->val;

				//clean up, new node not necessary
				delete new_node->location_info;
				delete new_node;
			}//the descending is larger, insert between the curr and descend
			else if (curr->descend->location_info->distances.at(current_location) >
				new_node->location_info->distances.at(current_location)) {

				new_node->descend = curr->descend;
				curr->descend->ascend = new_node;

				curr->descend = new_node;
				new_node->ascend = curr;

				knowledge_base.insert(pair<string, Node*>
					(new_node->location_info->coordinate, new_node));
			}//descend is smaller so move forward
			else {
				return addRecursive(curr->descend, curr, new_node, current_location);
			}
		} //not final location continue searching
		else {
			//no node above, create placeholder and move through coordinates
			if (curr->descend == nullptr) {
				Node* empty = new Node;
				updateLocation(empty, new_node->location_info, current_location);
				empty->ascend = curr;
				curr->descend = empty;
				knowledge_base.insert(pair<string, Node*>
					(empty->location_info->coordinate, empty));

				return addRecursive(empty, curr, new_node, ++current_location);
			}//found location, iterate to next directional
			else if (curr->descend->location_info->distances.at(current_location) ==
				new_node->location_info->distances.at(current_location)) {
				return addRecursive(curr, prev, new_node, ++current_location);
			}//current larger; create temporary node with empty value 
			 //then continue moving through graph
			else if (curr->descend->location_info->distances.at(current_location) >
				new_node->location_info->distances.at(current_location)) {
				Node* empty = new Node;
				updateLocation(empty, new_node->location_info, current_location);

				empty->descend = curr;
				curr->ascend = empty;

				prev->descend = empty;
				empty->ascend = prev;
				knowledge_base.insert(pair<string, Node*>
					(empty->location_info->coordinate, empty));

				return addRecursive(empty, prev, new_node, ++current_location);
			}//move forwards in current dir, the node is smaller direction val
			else {
				return addRecursive(curr->descend, curr, new_node, current_location);
			}
		}
		break;

	case NORTH:

		if (last_coordinate) {
			//no nodes to further north to
			if (curr->north == nullptr) {
				curr->north = new_node;
				new_node->descend = curr;
				knowledge_base.insert(pair<string, Node*>
					(new_node->location_info->coordinate, new_node));
			}	//there is already a node here of same dist, overwrite vals
			else if (curr->north->location_info->distances.at(current_location) ==
				new_node->location_info->distances.at(current_location)) {
				cout << "Overwritting node..." << endl;
				curr->val = new_node->val;

				//clean up, new node not necessary
				delete new_node->location_info;
				delete new_node;
			}//the northern is larger, insert between the curr and north
			else if (curr->north->location_info->distances.at(current_location) >
				new_node->location_info->distances.at(current_location)) {

				new_node->north = curr->north;
				curr->north->south = new_node;

				curr->north = new_node;
				new_node->south = curr;

				knowledge_base.insert(pair<string, Node*>
					(new_node->location_info->coordinate, new_node));
			}//north is smaller so move forward
			else {
				return addRecursive(curr->north, curr, new_node, current_location);
			}
		} //not final location continue searching
		else {
			//no node above, create placeholder and move through coordinates
			if (curr->north == nullptr) {
				Node* empty = new Node;
				updateLocation(empty, new_node->location_info, current_location);
				empty->south = curr;
				curr->north = empty;
				knowledge_base.insert(pair<string, Node*>
					(empty->location_info->coordinate, empty));

				return addRecursive(empty, curr, new_node, ++current_location);
			}//found location, iterate to next directional
			else if (curr->north->location_info->distances.at(current_location) ==
				new_node->location_info->distances.at(current_location)) {
				return addRecursive(curr, prev, new_node, ++current_location);
			}//current larger; create temporary node with empty value 
			 //then continue moving through graph
			else if (curr->north->location_info->distances.at(current_location) >
				new_node->location_info->distances.at(current_location)) {
				Node* empty = new Node;
				updateLocation(empty, new_node->location_info, current_location);

				empty->north = curr;
				curr->south = empty;

				prev->north = empty;
				empty->south = prev;
				knowledge_base.insert(pair<string, Node*>
					(empty->location_info->coordinate, empty));

				return addRecursive(empty, prev, new_node, ++current_location);
			}//move forwards in current dir, the node is smaller direction val
			else {
				return addRecursive(curr->north, curr, new_node, current_location);
			}
		}
		break;

	case SOUTH:

		if (last_coordinate) {
			//no nodes to further south to
			if (curr->south == nullptr) {
				curr->south = new_node;
				new_node->north = curr;
				knowledge_base.insert(pair<string, Node*>
					(new_node->location_info->coordinate, new_node));
			}	//there is already a node here of same dist, overwrite vals
			else if (curr->south->location_info->distances.at(current_location) ==
				new_node->location_info->distances.at(current_location)) {
				cout << "Overwritting node..." << endl;
				curr->val = new_node->val;

				//clean up, new node not necessary
				delete new_node->location_info;
				delete new_node;
			}//the southern is larger, insert between the curr and south
			else if (curr->south->location_info->distances.at(current_location) >
				new_node->location_info->distances.at(current_location)) {

				new_node->south = curr->south;
				curr->south->north = new_node;

				curr->south = new_node;
				new_node->north = curr;

				knowledge_base.insert(pair<string, Node*>
					(new_node->location_info->coordinate, new_node));
			}//south is smaller so move forward
			else {
				return addRecursive(curr->south, curr, new_node, current_location);
			}
		} //not final location continue searching
		else {
			//no node above, create placeholder and move through coordinates
			if (curr->south == nullptr) {
				Node* empty = new Node;
				updateLocation(empty, new_node->location_info, current_location);
				empty->north = curr;
				curr->south = empty;
				knowledge_base.insert(pair<string, Node*>
					(empty->location_info->coordinate, empty));

				return addRecursive(empty, curr, new_node, ++current_location);
			}//found location, iterate to next directional
			else if (curr->south->location_info->distances.at(current_location) ==
				new_node->location_info->distances.at(current_location)) {
				return addRecursive(curr, prev, new_node, ++current_location);
			}//current larger; create temporary node with empty value 
			 //then continue moving through graph
			else if (curr->south->location_info->distances.at(current_location) >
				new_node->location_info->distances.at(current_location)) {
				Node* empty = new Node;
				updateLocation(empty, new_node->location_info, current_location);

				empty->south = curr;
				curr->north = empty;

				prev->south = empty;
				empty->north = prev;
				knowledge_base.insert(pair<string, Node*>
					(empty->location_info->coordinate, empty));

				return addRecursive(empty, prev, new_node, ++current_location);
			}//move forwards in current dir, the node is smaller direction val
			else {
				return addRecursive(curr->south, curr, new_node, current_location);
			}
		}
		break;

	case EAST:

		if (last_coordinate) {
			//no nodes to further east to
			if (curr->east == nullptr) {
				curr->east = new_node;
				new_node->west = curr;
				knowledge_base.insert(pair<string, Node*>
					(new_node->location_info->coordinate, new_node));
			}	//there is already a node here of same dist, overwrite vals
			else if (curr->east->location_info->distances.at(current_location) ==
				new_node->location_info->distances.at(current_location)) {
				cout << "Overwritting node..." << endl;
				curr->val = new_node->val;

				//clean up, new node not necessary
				delete new_node->location_info;
				delete new_node;
			}//the easterly is larger, insert between the curr and east
			else if (curr->east->location_info->distances.at(current_location) >
				new_node->location_info->distances.at(current_location)) {

				new_node->east = curr->east;
				curr->east->west = new_node;

				curr->east = new_node;
				new_node->west = curr;

				knowledge_base.insert(pair<string, Node*>
					(new_node->location_info->coordinate, new_node));
			}//east is smaller so move forward
			else {
				return addRecursive(curr->east, curr, new_node, current_location);
			}
		} //not final location continue searching
		else {
			//no node above, create placeholder and move through coordinates
			if (curr->east == nullptr) {
				Node* empty = new Node;
				updateLocation(empty, new_node->location_info, current_location);
				empty->west = curr;
				curr->east = empty;
				knowledge_base.insert(pair<string, Node*>
					(empty->location_info->coordinate, empty));

				return addRecursive(empty, curr, new_node, ++current_location);
			}//found location, iterate to next directional
			else if (curr->east->location_info->distances.at(current_location) ==
				new_node->location_info->distances.at(current_location)) {
				return addRecursive(curr, prev, new_node, ++current_location);
			}//current larger; create temporary node with empty value 
			 //then continue moving through graph
			else if (curr->east->location_info->distances.at(current_location) >
				new_node->location_info->distances.at(current_location)) {
				Node* empty = new Node;
				updateLocation(empty, new_node->location_info, current_location);

				empty->east = curr;
				curr->west = empty;

				prev->east = empty;
				empty->west = prev;
				knowledge_base.insert(pair<string, Node*>
					(empty->location_info->coordinate, empty));

				return addRecursive(empty, prev, new_node, ++current_location);
			}//move forwards in current dir, the node is smaller direction val
			else {
				return addRecursive(curr->east, curr, new_node, current_location);
			}
		}
		break;

	case WEST:

		if (last_coordinate) {
			//no nodes to further west to
			if (curr->west == nullptr) {
				curr->west = new_node;
				new_node->east = curr;
				knowledge_base.insert(pair<string, Node*>
					(new_node->location_info->coordinate, new_node));
			}	//there is already a node here of same dist, overwrite vals
			else if (curr->west->location_info->distances.at(current_location) ==
				new_node->location_info->distances.at(current_location)) {
				cout << "Overwritting node..." << endl;
				curr->val = new_node->val;

				//clean up, new node not necessary
				delete new_node->location_info;
				delete new_node;
			}//the westerly is larger, insert between the curr and west
			else if (curr->west->location_info->distances.at(current_location) >
				new_node->location_info->distances.at(current_location)) {

				new_node->west = curr->west;
				curr->west->east = new_node;

				curr->west = new_node;
				new_node->east = curr;

				knowledge_base.insert(pair<string, Node*>
					(new_node->location_info->coordinate, new_node));
			}//west is smaller so move forward
			else {
				return addRecursive(curr->west, curr, new_node, current_location);
			}
		} //not final location continue searching
		else {
			//no node above, create placeholder and move through coordinates
			if (curr->west == nullptr) {
				Node* empty = new Node;
				updateLocation(empty, new_node->location_info, current_location);
				empty->east = curr;
				curr->west = empty;
				knowledge_base.insert(pair<string, Node*>
					(empty->location_info->coordinate, empty));

				return addRecursive(empty, curr, new_node, ++current_location);
			}//found location, iterate to next directional
			else if (curr->west->location_info->distances.at(current_location) ==
				new_node->location_info->distances.at(current_location)) {
				return addRecursive(curr, prev, new_node, ++current_location);
			}//current larger; create temporary node with empty value 
			 //then continue moving through graph
			else if (curr->west->location_info->distances.at(current_location) >
				new_node->location_info->distances.at(current_location)) {
				Node* empty = new Node;
				updateLocation(empty, new_node->location_info, current_location);

				empty->west = curr;
				curr->east = empty;

				prev->west = empty;
				empty->east = prev;
				knowledge_base.insert(pair<string, Node*>
					(empty->location_info->coordinate, empty));

				return addRecursive(empty, prev, new_node, ++current_location);
			}//move forwards in current dir, the node is smaller direction val
			else {
				return addRecursive(curr->west, curr, new_node, current_location);
			}
		}
		break;

	default:
		cout << "Invalid coordinate was given - " << new_node->location_info->directionals[current_location]
			<< endl;
		break;
	}
}

//given a "string" of text representing the command, parses it into a list of
//directionals and distances with the value at the tail
void RadiationGraph::parseCommand(string *command, Node* node) {

	int pos, count = 0, num_of_directions = 0;

	//finding all directionals and distances in command string
	for (pos = 0; pos < command->size(); ++pos) {
		if (isalpha(command->at(pos))) {
			//save the directional in the struct
			node->location_info->directionals.push_back(command->at(pos));
			++num_of_directions;
		}
		else if (command->at(pos) == '-') {
			//found the dash, coordinates are completed
			node->location_info->coordinate = command->substr(0, pos);

			//begin to parse the integer value 
			++pos;
			node->val = parseInt(command, &pos);
		}
		else {
			//integer is found, store the distance
			node->location_info->distances.push_back(parseInt(command, &pos));
			++count;
			--pos;
		}
	}
}

//given the current index and the command,parse the integer
//until a non-digit character is reached or EOS
int RadiationGraph::parseInt(string *command, int *index) {
	string dist = "";

	while (*index < command->size() && (isdigit(command->at(*index)))) {
		dist.push_back(command->at(*index));
		++*index;
	}
	//save distance associated with directional
	return stoi(dist);
}

//for all of the values currently in the graph, display all of the
//information as a histogram for the user
void RadiationGraph::display_histogram() {
	int hist[MAX_BINS] = { 0 };

	//load in count of all vals
	for each (pair<string, Node*> curr in knowledge_base) {
		hist[curr.second->val]++;
	}



}