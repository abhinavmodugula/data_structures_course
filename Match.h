
#include <vector>
#include <string>

//Represents a single tennis match
//Tournament -> Match -> Set -> Game
class Match {

public:
	Match();
	Match(std::string winner, std::string loser, std::vector<std::vector<int>> games);
	//Accessors
	std::string get_winner() const;
	std::string get_loser() const;
	std::vector<std::vector<int>> get_all_games() const;
	//Other functions
	void print() const;
	int games_won(int ply) const;//0 for ply 1, 1 for ply 2
	int games_lost(int ply) const;
	std::vector<double> compute_agd(int ply) const;
	int sets_won(int ply) const;
	int sets_lost(int ply) const;
	
private:
	std::string winner;
	std::string loser;
	std::vector<std::vector<int>> games;
	
};
