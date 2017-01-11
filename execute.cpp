//execute.cpp

//Master documentation.. here..

//
//@author = Brandon Daniel

#include "stdafx.h"
#include "radiationgraph.h"
#include <fstream>

#define ADD 1
#define DELETE 2
#define SIZE 3
#define DISPLAY 4
#define CLUSTERS 5
#define EXIT 6

void main_loop(RadiationGraph*);

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
void main_loop(RadiationGraph *globe) {

	bool run = true;
	string coordinates;
	int option;
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
				cout << "An example command is shown below..." << endl;
				cout << "A2W2N5-45 (Ascend 2, West 2, North 5 value 45)\n" << endl;

				cout << "The list of valid commands are as follows." << endl;
				cout << "N = North\n S = South\n E = East\n W = West\n"
					<< "A = Ascend\n D = Descend" << endl;
			}
			else {
				globe->add(&coordinates);
			}

			break;
		case DELETE:
			//go into the knowledge base a redirect all of the neighbors to the new neighbors
			//the remove the node from the graph/knowledge_base
			break;
		case SIZE:
			cout << "There were " << globe->getSize() << " total nodes added to the graph" << endl;
			break;
		case DISPLAY:
			globe->display(-1);
			break;
		case CLUSTERS:
			//use some type of spanning algorithm, "kruskals"? for this.
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
