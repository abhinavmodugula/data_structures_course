#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <algorithm>
#include <iomanip>
#include "Match.h"

using namespace std;
//Taken from the course website
//takes a set "x-y" and turns it into a vector
vector<int> parse_set(string set) {
	int i = set.find('-');
	vector<int> res;
	res.push_back(atoi(set.substr(0,i).c_str()));
	res.push_back(atoi(set.substr(i+1,set.size()-i-1).c_str()));
	return res;
}

//parses one line of an input file and creates a match object
Match parse_match(string line) {
	//Splits the line into a vector separated by spaces
	istringstream buf(line);
	istream_iterator<string> beg(buf), end;
	vector<string> result(beg, end);
	
	//Extracts the information from the split line
	string winner = result[0] + " " + result[1];
	string loser = result[3] + " " + result[4];
	vector<vector<int>> games;
	for (size_t i = 5; i < result.size(); i++) {
		games.push_back(parse_set(result[i]));
	}
	
	//Creates the object and returns it
	Match m(winner, loser, games);
	return m;
	
}

//Reads an input file and creates a vector of match objects
vector<Match> read_file(string input_file) {
	ifstream file;
	file.open(input_file);
	if (!file) {
		cerr << "Can't open " << input_file << " .\n";
		exit(1);
	}
	string line;
	vector<Match> matches;
	while (getline(file, line)) {
		matches.push_back(parse_match(line));
	}
	return matches;
}

//Represents the stats of each individual player
struct Player {
	string name;
	int match_wins;
	int match_losses;
	int game_wins;
	int game_losses;
	double agd_win;
	double agd_loss;
};

//Determines if an element is present in a vector
bool in_vec(vector<Player> players, string name) {
	for (size_t i = 0; i < players.size(); i++) {
		if (players[i].name == name) {
			return true;
		}
	}
	return false;
}

//Takes a vector of matches and creates a list of all players who played
void construct_player_list(vector<Match> &matches, vector<Player> &players) {
	for (size_t i = 0; i < matches.size(); i++) {
		string winner = matches[i].get_winner();
		if (in_vec(players, winner) == false) { //Not in the vector of players
			players.push_back(Player());
			players[players.size() - 1].name = winner;
		}
		string loser = matches[i].get_loser();
		if (in_vec(players, loser) == false) {
			players.push_back(Player());
			players[players.size() - 1].name = loser;
		}
	}
}

//Stores all of the match stats in each player struct
void compute_match_stats(vector<Match> &matches, vector<Player> &players) {
	for (size_t i = 0; i < matches.size(); i++) { //goes through each match
		string winner = matches[i].get_winner();
		string loser = matches[i].get_loser();
		for (size_t j = 0; j < players.size(); j++) { //each player in list
			if (players[j].name == winner) {
				players[j].match_wins += 1;
			} else if (players[j].name == loser) {
				players[j].match_losses += 1;
			}
		}
	}
}

//Determines all of the game stats
void compute_game_stats(vector<Match> &matches, vector<Player> &players) {
	for (size_t i = 0; i < matches.size(); i++) { //each match
		string winner = matches[i].get_winner();
		string loser = matches[i].get_loser();
		for (size_t j = 0; j < players.size(); j++) { //each player
			if (players[j].name == winner) {
				players[j].game_wins += matches[i].games_won(0);
				players[j].game_losses += matches[i].games_lost(0);
			} else if (players[j].name == loser) {
				players[j].game_wins += matches[i].games_won(1);
				players[j].game_losses += matches[i].games_lost(1);
			}
		}
	}
}

void compute_msc_stats(vector<Match> &matches, vector<Player> &players) {
	for (size_t i = 0; i < matches.size(); i++) {
		string winner = matches[i].get_winner();
		string loser = matches[i].get_loser();
		for (size_t j = 0; j < players.size(); j++) {
			if (players[j].name == winner) {
				vector<double> sums = matches[i].compute_agd(0);
				players[j].agd_win += sums[0];
				players[j].agd_loss += sums[1];
			} else if (players[j].name == loser) {
				vector<double> sums = matches[i].compute_agd(1);
				players[j].agd_win += sums[0];
				players[j].agd_loss += sums[1];
			}
		}
	}
}

//takes a full name and resturns the last name
string last_name(string full_name) {
	istringstream buf(full_name);
	istream_iterator<string> beg(buf), end;
	vector<string> result(beg, end);
	return result[1];
}

//takes a full name and returns the first name
string first_name(string full_name) {
	istringstream buf(full_name);
	istream_iterator<string> beg(buf), end;
	vector<string> result(beg, end);
	return result[0];
}

//Used to sort the vector for the match stats
bool compare_function(const Player &one, const Player &two) {
	double win_perc_one = (double) one.match_wins / (one.match_wins + one.match_losses);
	double win_perc_two = (double) two.match_wins / (two.match_wins + two.match_losses);
	
	if (win_perc_one < win_perc_two) return false; //higher win percents shoudl go first
	if (win_perc_two < win_perc_one) return true;
	
	if (last_name(one.name) < last_name(two.name)) return true; //alphabetical
	if (last_name(two.name) < last_name(one.name)) return false;
	
	if (first_name(one.name) < first_name(two.name)) return true; //alphabetical
	if (first_name(two.name) < first_name(one.name)) return false;
	
	return true;
}

//Used to sort the vector for the game stats
bool game_compare_function(const Player &one, const Player &two) {
	double win_perc_one = (double) one.game_wins / (one.game_wins + one.game_losses);
	double win_perc_two = (double) two.game_wins / (two.game_wins + two.game_losses);
	
	if (win_perc_one < win_perc_two) return false; //higher percents go first
	if (win_perc_two < win_perc_one) return true;
	
	if (last_name(one.name) < last_name(two.name)) return true; //alphabetical
	if (last_name(two.name) < last_name(one.name)) return false;
	
	if (first_name(one.name) < first_name(two.name)) return true; //alphabetical
	if (first_name(two.name) < first_name(one.name)) return false;
	
	return true;
}

size_t longest_name(vector<Player> &players) {
	size_t longest = 0;
	for (size_t i = 0; i < players.size(); i++) {
		if (players[i].name.size() > longest) {
			longest = players[i].name.size();
		}
	}
	return longest;
}

//Writes all of the stats to the output file
void write_stats(vector<Player> &players, string output_file, vector<Match> matches) {
	//IO Objects
	ofstream file;
	file.open(output_file);
	if (!file.good()) {
		cerr << "Could not open the fule for output." << "\n";
		exit(1);
	}
	
	size_t MARGIN_CONST = longest_name(players);
	
	file << "MATCH STATISTICS\n";
	file << left << setw(MARGIN_CONST) << "Player" << setw(11) <<right  << "W" << setw(11) << right << "L" << right << setw(16) << "percentage" << endl;
	sort(players.begin(), players.end(), compare_function);
	for (size_t i = 0; i < players.size(); i++) {
		double perc_win = (double) players[i].match_wins / (players[i].match_wins + players[i].match_losses);
		//Output formatting using setw() and align
		file <<left << setw(MARGIN_CONST)<< players[i].name\
		<< right << setw(11) << players[i].match_wins << \
		right << setw(11) << players[i].match_losses << \
		right << setw(16) << setprecision(2) << perc_win << endl;
	}
	for (int i = 0; i < (16+11+11+16); i++) file << "-";
	file << "\n";
	
	file << "GAME STATISTICS\n";
	file << left << setw(MARGIN_CONST) << "Player" << setw(11) <<right  << "W" << setw(11) << right << "L" << right << setw(16) << "percentage" << endl;
	sort(players.begin(), players.end(), game_compare_function);
	for (size_t i = 0; i < players.size(); i++) {
		double perc_win = (double) players[i].game_wins / (players[i].game_wins + players[i].game_losses);
		//Output formatting using setw() and align
		file <<left << setw(MARGIN_CONST)<< players[i].name\
		<< right << setw(11) << players[i].game_wins << \
		right << setw(11) << players[i].game_losses << \
		right << setw(16) << setprecision(2) << perc_win << endl;
	}
	for (int i = 0; i < ((int) MARGIN_CONST+11+11+16); i++) file << "-";
	file << "\n";
	
	file << "MARGIN OF VICTORY STATISTICS\n";
	file << "The average game difference is a measure of the strenght of the wins and losses for a player." << endl;
	file << left << setw(MARGIN_CONST) << "Player" << setw(15) <<right  << "Set_diff (won games)" << setw(15) << right << "Set_diff (lost games)"  << endl;
	sort(players.begin(), players.end(), game_compare_function);
	for (size_t i = 0; i < players.size(); i++) {
		//Output formatting using setw() and align
		file <<left << setw(MARGIN_CONST)<< players[i].name\
		<< right << setw(15) << players[i].agd_win << \
		right << setw(15) << players[i].agd_loss  << endl;
	}
	for (int i = 0; i < ((int) MARGIN_CONST+11+11+16); i++) file << "-";
	file << "\n";
}

int main(int argc, char* argv[]) {
	
	if (argc < 2) {
		cerr << "Not enough input commands\n";
		exit(1);
	}
	
	string input_file = string(argv[1]);
	string output_file = string(argv[2]);
	
	vector<Match> matches = read_file(input_file);
	
	vector<Player> players;
	
	construct_player_list(matches, players);
	compute_match_stats(matches, players);
	compute_game_stats(matches, players);
	compute_msc_stats(matches, players);
	write_stats(players, output_file, matches);
	
	return 0;
}