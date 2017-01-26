#ifndef UTILITY_H
#define UTILITY_H 

#include "stdafx.h"
#include "radiationgraph.h"
#include <math.h>

#define MAX_PERMUTATIONS 6
#define ENTRY_SIZE 2

//support utility class with all methods to be accessed statically 
//for use on the 3d graph
class Utility {
	public:
		static set<string> permutations(string);
		static string distribution_type(map<int,int>);


	private:
		Utility() {};
		static string swap_perm_spot(string, int, int);
		static float get_mean(map<int, int>, int);
		static float get_std_dev(map<int, int>, float, int);
		static int get_num_vals(map<int, int>);
		static int within_std_dev(map<int, int>, int, float, int size);
};


#endif // !UTILITY_H
