
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

//Reads a file and converts it to a vector of vectors representation
vector<vector<char>> read_file(string image_file_name) {
	ifstream image_file;
	image_file.open(image_file_name);
	if (!image_file) {
		cerr << "Can't open " << image_file_name << " to write.\n";
		exit(1);
	}
	char temp;
	//Extracts the chars in the image
	string line;
	vector<vector<char>> image_vec;
	while (getline(image_file, line)) {
		cout << line << "\n";
		std::vector<char> temp(line.begin(), line.end()); //copies the line into the char vector
		image_vec.push_back(temp);
	}
	
	return image_vec;
}

//Prints an image to the console for testing
void print_vec(vector<vector<char>> vector) {
	
	for (int i = 0; i < vector.size(); i++) {
		for (int j = 0; j < vector[0].size(); j++) {
			cout << vector[i][j];
		}
		cout << "\n";
	}
}

//Returns a vector of coordinates representing all four fiection from the given point
vector<vector<int>> shift(int x, int y) {
	//"up, down, left, right" are the possible directions
	vector<vector<int>> coordinates;
	
	
	vector<int> up;
	up.push_back(x);
	up.push_back(y + 1);
	coordinates.push_back(up);
	
	vector<int> down;
	down.push_back(x);
	down.push_back(y - 1);
	coordinates.push_back(down);
	
	vector<int> right;
	right.push_back(x + 1);
	right.push_back(y);
	coordinates.push_back(right);
	
	vector<int> left;
	left.push_back(x - 1);
	left.push_back(y);
	coordinates.push_back(left);
	
	return coordinates;
	
}

//Returns true if the given cooridnates are in the image vector
bool valid_dimensions(vector<vector<char>> image, int x, int y) {
	if (x < 0 || x > image.size()) { //x is negative or greater than the number of lines
		return false;
	} else if (y < 0 || y > image[0].size()) { //y is negative or greaetr than the number of columns
		return false;
	} else {
		return true;
	}
}

//Expands the image through the dilation operation
vector<vector<char>> dilation(vector<vector<char>> image, char foreground) {
	vector<vector<char>> new_image = image;
	for (int i = 0; i < image.size(); i++) {
		for (int j = 0; j < image[0].size(); j++) { //Goes through each pixel
			if (image[i][j] == foreground) { //Center must be equal to the foreground in order to expand
				vector<vector<int>> coors = shift(i, j); // a vector of coordinates
				for (int r = 0; r < 4; r++) { //goes through each direction
					if (valid_dimensions(image, coors[r][0], coors[r][1])) {
						new_image[coors[r][0]][coors[r][1]] = foreground;
					}
				}
			}
		}
	}
	return new_image;
}


//Replaces all "remove" characters with the "sub" character
vector<vector<char>> replace(vector<vector<char>> image, char remove, char sub) {
	for (int i = 0; i < image.size(); i++) {
		for (int j = 0; j < image[i].size(); j++) {
			if (image[i][j] == remove) {
				image[i][j] = sub;
			}
		}
	}
	return image;
}

//Shrinks pixels of the foreground character and replaces them with the replace char
vector<vector<char>> erosion(vector<vector<char>> image, char foreground, char replace) {
	vector<vector<char>> new_image = image;
	for (int i = 0; i < image.size(); i++) {
		for (int j = 0; j < image[0].size(); j++) { //Goes through each pixel
			if (image[i][j] == foreground) { //The pixel must be equal to the foreground
				vector<vector<int>> coors = shift(i, j);
				for (int r = 0; r < 4; r++) { //Goes through up, down, left, and right dimensions
					if (valid_dimensions(image, coors[r][0], coors[r][1])) { //prevents a segmentation error
						if (image[coors[r][0]][coors[r][1]] != foreground) { 
							new_image[i][j] = replace; //replaces-deletes the pixel if all of the surrounding pixels are not the foreground
						}
					}
				}
				
			}
		}
	}
	return new_image;
}

//implements floodfill through a recursive function
//the '&' operator makes sure the image is passed by reference
void floodfill_recursion(vector<vector<char>> &image, int x, int y, char prev_char, char replace_char) {
	if (!valid_dimensions(image, x, y)) {//prevents a memory error
		return;
	}
	if (image[x][y] != prev_char) {//ends if we exit the cluster
		return;
	}
	image[x][y] = replace_char; //changes the pixel
	
	//Call the function again for each of the four directions
	floodfill_recursion(image, x, y+1, prev_char, replace_char); //up
	floodfill_recursion(image, x, y-1, prev_char, replace_char); //down
	floodfill_recursion(image, x+1, y, prev_char, replace_char); //right
	floodfill_recursion(image, x-1, y, prev_char, replace_char); //left
}

//Starts at a given coordinate and changes the cluster to the replace char
void floodfill(vector<vector<char>> &image, int x, int y, char replacement) {
	
	char prev = image[x][y]; //used to determine if we are still in a cluster of pixels
	cout << "Previous char: " << prev << "\n";
	floodfill_recursion(image, x, y, prev, replacement);
	
}


//Writes a vector of vectors to an output file
void write_to_file(vector<vector<char>> vector, string output_file) {
	ofstream out_file;
	out_file.open(output_file);
	if (!out_file.good()) {
		cerr << "Could not open the fule for output." << "\n";
		exit(1);
	}
	for (int i = 0; i < vector.size(); i++) {
		for (int j = 0; j < vector[0].size(); j++) {
			out_file << vector[i][j];
		}
		out_file << "\n";
	}
	out_file.close();
}

int main(int argc, char* argv[]) {
	
	//Checks if there are not enough arguments
	if (argc < 5) {
		cerr << "Not enough arguments.\n";
		exit(1);
	}
	
	//char[1] is the input file, 2 the output file, 2 the command, and the rest are dependent on the command entered
	string input_file = string(argv[1]);
	string output_file = string(argv[2]);
	string command = string(argv[3]);
	
	vector<vector<char>> original_image = read_file(input_file); //From the input file
	vector<vector<char>> new_image; //Going to be written to the output file
	
	//argv[x][0] becuase each argv is a vector, but char is only one character
	if (command == "dilation") {
		new_image = dilation(original_image, char(argv[4][0]));
	} else if (command == "replace") {
		new_image = replace(original_image, char(argv[4][0]), char(argv[5][0]));
	} else if (command == "erosion") {
		new_image = erosion(original_image, char(argv[4][0]), char(argv[5][0]));
	} else if (command == "floodfill") {
		//the numbers start from ASCII 48 in C++
		//char form of a number to int type ex. '1' to 1
		int x = (int)argv[4][0] - 48;
		int y = (int)argv[5][0] - 48;
		floodfill(original_image, x, y, char(argv[6][0]));
		new_image = original_image;
	} else {
		cerr << "Invalid command:" << command << ".\n";
		exit(1);
	}
	
	write_to_file(new_image, output_file);
	cout << "The new image has been successfully created:" << "\n";
	print_vec(new_image);
	return 0;
}