#include <iostream>
#include <fstream>
#include <numeric>
#include "Match.h"
#include <vector>

using namespace std;

Match::Match() {
	winner = "N/A";
	loser = "N/A";
}

Match::Match(string win, string lose, vector<vector<int>> games_list) {
	winner = win;
	loser = lose;
	games = games_list;
}

string Match::get_winner() const{
	return winner;
}

string Match::get_loser() const{
	return loser;
}

vector<vector<int>> Match::get_all_games() const{
	return games;
}

void Match::print() const{
	cout << "Winner: " << winner << "\n";
	cout << "Loser: " << loser << "\n";
	for (size_t i = 0; i < games.size(); i++) {
		cout << games[i][0] << " " << games[i][1] << " \n";
	}
}

//0 for ply one , 1 for ply 2
int Match::games_won(int ply) const {
	int sum = 0;
	for (size_t i = 0; i < games.size(); i++) {
		sum += games[i][ply];
	}
	return sum;
}

int Match::sets_won(int ply) const {
	int sets_won;
	for (size_t i = 0; i < games.size(); i++) {
		if (ply == 0) {
			if ((games[i][0] - games[i][1]) > 0) {
				sets_won += 1;
			}
		} else if (ply == 1) {
			if ((games[i][1] - games[i][0]) > 0) {
				sets_won += 1;
			}
		}
	}
	return sets_won;
}

int Match::sets_lost(int ply) const {
	int sets_lost;
	for (size_t i = 0; i < games.size(); i++) {
		if (ply == 1) {
			if ((games[i][0] - games[i][1]) > 0) {
				sets_lost += 1;
			}
		} else if (ply == 0) {
			if ((games[i][1] - games[i][0]) > 0) {
				sets_lost += 1;
			}
		}
	}
	return sets_lost;
}

//Computes the average game difference in each set per game (wins and losses)
//0 for ply 1, 1 for poy 2 
vector<double> Match::compute_agd(int ply) const {
	vector<int> win_diffs;
	vector<int> loss_diffs;
	for (size_t i = 0; i < games.size();i++) {
		double d;
		
		if (ply == 0) {
			d = games[i][0] - games[i][1];
		} else if (ply == 1) {
			d = games[i][1] - games[i][0];
		}
		
		if (d > 0) {
			win_diffs.push_back(d);
		} else if (d < 0) {
			loss_diffs.push_back(d);
		}
	}
	double tot_win_diff = accumulate(win_diffs.begin(), win_diffs.end(), 0.0);
	double tot_loss_diff = accumulate(loss_diffs.begin(), loss_diffs.end(), 0.0);
	vector<double> res;
	res.push_back(tot_win_diff);
	res.push_back(tot_loss_diff);
	return res;
}

//0 for ply one , 1 for ply 2
int Match::games_lost(int ply) const {
	int sum = 0;
	int player;
	if (ply == 0) {
		player = 1;
	} else {
		player = 0;
	}
	for (size_t i = 0; i < games.size(); i++) {
		sum += games[i][player];
	}
	return sum;
}

