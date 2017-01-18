//execute.cpp

//***************************************************************
// MLH PRIME HACKATHON ENTRY
//Post Hackathon
//Find a way to apply function that can be distributed throughout
//the graph
//For example, can mimck shattering of glass as rock impacts it 
//at different points
//***************************************************************

//Global Documentation...
// The radiation graph


//Developed and Tested by 
//@author = Brandon Daniel

#include "stdafx.h"
#include "radiationgraph.h"
#include <fstream>

#define ADD 1
#define DELETE 2
#define SIZE 3
#define DISPLAY 4
#define CLUSTERS 5
#define HISTOGRAM 6
#define EXIT 7

void main_loop(RadiationGraph*);
void prompt_help();

int main(int argc, char *argv[]) {

	string line;
	RadiationGraph globe;
	ifstream in;

	if (argc == HAS_FILE) {
		//open the file and perform addition to the graph
		in.open(argv[ADD]);

		while (!in.eof()) {
			getline(in, line);
			globe.add(&line);
		}

		in.close();
		main_loop(&globe);
	}
	else {
		main_loop(&globe);
	}

	return 0;
}

//running the cmd user interface and signals the 
//interaction with the graph
//makes the huge assumption that the user only enters valid coordinates
//every time....
void main_loop(RadiationGraph *globe) {

	bool run = true;
	string coordinates;
	int option, cluster_dist = 0;
	const string HELP_KEYWORD = "HELP";

	while (run) {
		std::cout << globe->printOptions();
		cin >> option;

		switch (option) {
		case ADD:
			//adding to the graph
			std::cout << "Enter the coordinates or type \"help\" to see an example" << endl;
			cin >> coordinates;

			//if the help command was invoked
			boost::to_upper(coordinates);

			if (!coordinates.compare(HELP_KEYWORD)) {
				prompt_help();
			}
			else {
				globe->add(&coordinates);
			}

			break;
		case DELETE:
			
			//adding to the graph
			std::cout << "Enter the coordinates or type \"help\" to see an example" << endl;
			cin >> coordinates;

			//if the help command was invoked
			boost::to_upper(coordinates);

			if (!coordinates.compare(HELP_KEYWORD)) {
				prompt_help();
			}
			else {
				globe->remove(&coordinates);
			}
			break;
		case SIZE:
			cout << "There were " << globe->getSize() << " total nodes added to the graph" <<
				" with " << globe->explicit_size() << " being empty" << endl;
			break;
		case DISPLAY:
			globe->display(-1);
			break;
		case CLUSTERS:
			cout << "Maximum node distance for each cluster" << endl;
			cin >> cluster_dist;

			globe->print_cluster(cluster_dist);
			break;
		case HISTOGRAM:
			cout << "Displaying histogram now..." << endl;
			break;
		case EXIT:
			run = false;
			cout << "Exiting..." << endl;
			break;
		default:
			cerr << "Error: Invalid selection: " << option << endl;

		}
	}
}

//prompt to enter a coordinate or display the 
//help display
void prompt_help() {
	cout << "An example command is shown below..." << endl;
	cout << "A2W2N5-45 (Ascend 2, West 2, North 5 value 45)\n" << endl;

	cout << "The list of valid commands are as follows." << endl;
	cout << " N = North\n S = South\n E = East\n W = West\n"
		<< " A = Ascend\n D = Descend\n" << endl;
}
