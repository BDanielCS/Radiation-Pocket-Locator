//RadiationGraph.cpp

// **************************************************************
// MLH PRIME HACKATHON ENTRY

//The 3D graph allows the user to generate a 3D object with 
//some integer values. A centroid is automatically generated and
// all nodes branch off from it in all directions in R^3. 
//

//****************************************************************



//Developed by 
//Brandon Daniel
//DJ Sharma 

#include "stdafx.h"
#include "radiationgraph.h"

//optimizes the IO operations upon initialization
RadiationGraph::RadiationGraph() {
	ios::sync_with_stdio(false);

	//the centroid is predefined as "C-VACANT"
	centroid = new Node;
	centroid->val = VACANT;
	centroid->location_info = new Location;
	centroid->location_info->directionals.push_back(CENTROID);
	centroid->location_info->distances.push_back(0);

	knowledge_base.insert(pair<string, Node*>("CENTROID", centroid));
}

//deallocate the entire 3D graph by removing locations
//from the knowledge
RadiationGraph::~RadiationGraph() {

	/*for (auto &iter : knowledge_base) {
		delete iter.second->location_info;
		delete iter.second;
	}

	delete centroid->location_info;
	delete centroid;
	*/
}

//return the amount of nodes created, nodes can be
//created while moving towards a desired coordinate
int RadiationGraph::getSize() { return knowledge_base.size(); }

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
		"Delete(2)\nSize(3)\nDisplay(4)\nClusters(5)\nExit(6)\n";
}

//given a dyanamically allocated node, updates its information to
//correctly correspond to the current direction and distance while
//maintaining the VACANT status
void RadiationGraph::updateLocation(Node* empty, Location* info, int index) {

	int count = 0;

	empty->location_info = new Location;
	empty->val = VACANT;
	empty->location_info->coordinate = info->coordinate.substr(0, index);

	//copying the location(s) information over to the empty node
	while (count <= index) {
		empty->location_info->directionals[count] = info->directionals[count];
		empty->location_info->distances[count] = info->distances[count];
	}
}

//given a full coordinate, determines if there is a node at that 
//coordinate with the inputted value
Found* RadiationGraph::find(string *command) {

	Node* new_node = new Node;
	Found *status = new Found;
	map<string, Node*>::iterator found;
	struct Location* parsed_location = new Location;

	//parsed the commands and create location node as normal
	new_node->location_info = parsed_location;
	parseCommand(command, new_node);

	found = knowledge_base.find(parsed_location->coordinate);

	//the node was found and it has the same value
	if (found != knowledge_base.end() && found->second->val == new_node->val) {
		status->node = true;
		status->value = true;
	}//only the node was found, but with different values
	else if (found != knowledge_base.end()) {
		status->node = true;
		status->value = false;
	}//nothing was found
	else {
		status->node = false;
		status->value = false;
	}

	return status;
}

//adds a value to the graph by cycling through all
//of the directional and distance pairs until the correct
//location is reached.  Then stores the int value
void RadiationGraph::add(string *command) {

	Node* new_node = new Node;
	struct Location* parsed_location = new Location;

	new_node->location_info = parsed_location;
	parseCommand(command, new_node);

	//permutation swap and check if node equivalent position
	if (isMatch(parsed_location)) {

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

//determine if a permutation of the inputted coordinate 
//is equivalent to one that is already in the graph
//ex. N2E2 == E2N2.  If there is no perm found, 
//then null is returned
Node* RadiationGraph::isMatch(Location* loc) {
	//create the permuations and see if knowledge base contains
	

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
	char response;

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
