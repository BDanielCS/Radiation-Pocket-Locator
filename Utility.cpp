//Utility.cpp
#include "stdafx.h"
#include "Utility.h"

//Supporting utility class in order to preform 
//mathematical operations for the 3D graph
//no instance required nor desired



	//given some coordinate represented as a string, generate all
	//of the possible permutations of it and add them to the set
	//each directional of the coordinate is located at every even index.
	//permute along the even indices only
set<string> Utility::permutations(string toPermute) {
	set<string> permutations;
	int i = 0, pos = 0;

	permutations.insert(toPermute);

	//there is only permutation of a string of length of 1
	if (toPermute.length() == 1) {
		return permutations;
	}//there are only two permutations of length 2
	else if (toPermute.length() == 2) {

		permutations.insert(swap_perm_spot(toPermute, 0, 2));
		return permutations;
	}

	//generate and add since length is 3
	while (i < MAX_PERMUTATIONS) {

	}

	return permutations;
}

//given the coordinate string, swap position loc1 with loc2
//and return the string
string Utility::swap_perm_spot(string toPerm, int loc1, int loc2) {
	string loc1_string, loc2_string;

	loc1_string.push_back(toPerm.at(loc1));
	loc1_string.push_back(toPerm.at(loc1 + 1));

	loc2_string.push_back(toPerm.at(loc2));
	loc2_string.push_back(toPerm.at(loc2 + 1));

	toPerm.replace(loc2, ENTRY_SIZE, loc1_string);
	toPerm.replace(loc1, ENTRY_SIZE, loc2_string);

	return toPerm;
}

