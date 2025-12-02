#ifndef BOARD_HPP // Assuming this guard based on previous file names
#define BOARD_HPP

#include <SFML/Graphics.hpp>
#include "texture_manager.hpp"
#include <vector>
#include <utility> 
#include <fstream> // Add missing include for fstream usage declared in the 'using' statement
#include "cell.hpp"


using std::vector;
using std::pair;
using std::fstream;

struct board_t { // Structure representing the game board
private:
    int width;
    int height;
    cell_t*** cells;
    // Note: It is highly recommended to use std::vector<std::vector<std::unique_ptr<cell_t>>> 
    // instead of raw 3-star pointers for better memory safety in C++.

public:
	board_t(int W, int H, const sf::Texture& placeholderTexture); // Constructor to initialize the board with given width and height

	void reset(); // Resets the board to its initial state

	void place_mines(int count); // Places a specified number of mines randomly on the board

	void place_mine(unsigned int row, unsigned int col); // Places a mine at the specified position

	void reveal_cell(unsigned int row, unsigned int col); // Reveals the cell at the specified position

	void reveal_all_cells(); // Reveals all cells on the board

	void toggle_flag_cell(unsigned int row, unsigned int col); // Toggles the flag state of the cell at the specified position

    int getFlagCount() const; // Add this new method declaration    
    
	void draw(sf::RenderWindow* window, TextureManager* textureManager, int cell_size); // Draws the board onto the provided SFML RenderWindow

	vector<pair<int, int> > get_adjacent_positions(unsigned int row, unsigned int col); // Returns a vector of valid adjacent positions for a given cell

	~board_t(); // Destructor to clean up allocated memory
};

#endif // BOARD_HPP
