#ifndef UTILITY_H
#define UTILITY_H 

#include "stdafx.h"
#include "radiationgraph.h"

#define MAX_PERMUTATIONS 6
#define ENTRY_SIZE 2

//support utility class with all methods to be accessed statically 
//for use on the 3d graph
class Utility {
	public:
		static set<string> permutations(string);


	private:
		Utility() {};
		static string swap_perm_spot(string, int, int);
};


#endif // !UTILITY_H
