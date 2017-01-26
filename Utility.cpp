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
	int i;

	permutations.insert(toPermute);

	//there is only permutation of a string of length of 1
	if (toPermute.length() / 2 <= 1) {
		return permutations;
	}//there are only two permutations of length 2
	else if (toPermute.length() / 2 == 2) {
		permutations.insert(swap_perm_spot(toPermute, 0, 1));
		return permutations;
	}

	int lower_indx, upper_indx;

	//generate and add since length is 3
	for (i = 1; i < MAX_PERMUTATIONS; i++) {
		lower_indx = i % MAX_COORDINATE_ENTRIES;
		upper_indx = (i + 1) % MAX_COORDINATE_ENTRIES;

		if (i == 5) {
			permutations.insert(swap_perm_spot(swap_perm_spot(toPermute, lower_indx,
				upper_indx), upper_indx, lower_indx / 2));
		}
		else if (i == 4) {
			permutations.insert(swap_perm_spot(
				swap_perm_spot(toPermute, (i - 1) % MAX_COORDINATE_ENTRIES, upper_indx),
				lower_indx, upper_indx));
		}
		else {
			permutations.insert(swap_perm_spot(toPermute, lower_indx, upper_indx));
		}

	}

	return permutations;
}

//given the coordinate string, swap position loc1 with loc2
//and return the string
string Utility::swap_perm_spot(string toPerm, int loc1, int loc2) {
	string loc1_string, loc2_string;

	//redefined index since each bucket is two indices
	loc1 *= 2;
	loc2 *= 2;

	loc1_string.push_back(toPerm.at(loc1));
	loc1_string.push_back(toPerm.at(loc1 + 1));

	loc2_string.push_back(toPerm.at(loc2));
	loc2_string.push_back(toPerm.at(loc2 + 1));

	toPerm.replace(loc2, ENTRY_SIZE, loc1_string);
	toPerm.replace(loc1, ENTRY_SIZE, loc2_string);

	return toPerm;
}

//given the array consisting of occurrences of all values
//in the graph.  Determine how the values are distributed
string Utility::distribution_type(map<int, int> data) {
	string findings;
	float mu, sigma;
	const int one_std_perc = 68, two_std_perc = 95, three_std_perc = 99, perfect_median = 50;
	int size = 0;

	size = get_num_vals(data);
	mu = get_mean(data, size);
	sigma = get_std_dev(data, mu, size);

	//check for skew
	if (trunc(mu) == perfect_median) {
		findings.append("Perfectly Centered ");
	}
	else if (trunc(mu) <= perfect_median) {
		findings.append("Negatively Skewed ");
	}
	else {
		findings.append("Positvely Skewed ");
	}

	//check if values are between the 1,2,3 std ranges to check normality strength
	if (within_std_dev(data, sigma * 3, mu, size) >= three_std_perc) {
		if (within_std_dev(data, sigma * 2, mu, size) >= two_std_perc) {
			if (within_std_dev(data, sigma, mu, size) >= one_std_perc) {
				findings.append("Strong Normal Distribution");
			}
			else {
				findings.append("Weak Normal Distribution");
			}
		}
		else {
			findings.append("Extremely Weak Normal Distribution");
		}
	}
	else {
		findings.clear();
		findings.append("Not Normally Distributed");
	}
	return findings;
}

//returns the mean of the data set
float Utility::get_mean(map<int, int> data, int total_vals) {
	float mean = 0;
	int counter = 0;

	for each(pair<int, int> val in data) {
		while (counter < val.second) {
			mean += val.first;
			counter++;
		}
		counter = 0;
	}

	return mean / total_vals;
}

//gets the std deviation of the data set
float Utility::get_std_dev(map<int, int> data, float mu, int total_vals) {
	float sum = 0;
	int counter = 0;

	for each(pair<int, int> val in data) {
		while (counter < val.second) {
			sum += pow((val.first - mu), 2);
			counter++;
		}
		counter = 0;
	}

	return sqrt((1 / total_vals) * sum);
}

//gets the number of items in the map which also includes
//the number of occurrences of each item
int Utility::get_num_vals(map<int, int> data) {
	int total = 0;

	for each(pair<int, int> val in data) {
		total += val.second;
	}
	return total;
}

int Utility::within_std_dev(map<int, int> data, int std_dev, float mu, int size) {
	float lower, upper;
	int within_range = 0;

	lower = mu - std_dev;
	upper = mu + std_dev;

	for each(pair<int, int> val in data) {
		if (val.first <= upper && val.first >= lower) {
			within_range += val.second;
		}
	}
	return trunc(double(100 * within_range) / size);
}

