#include <cstdlib> // Required for rand and srand
#include <ctime> // Required for seeding rand with time
#include <queue> // Required for std::queue
#include <unordered_set> // Required for std::unordered_set
#include <vector> // Required for std::vector
#include <iostream> // For better error reporting
#include <memory> // Required for std::unique_ptr
#include "board.hpp" // Include the corresponding header file
#include "gameover.hpp" // Include GameOver exception definition
#include "texture_manager.hpp" // Include TextureManager definition
#include "cell.hpp" // Make sure cell.hpp is included so getIsFlagged is visible
#include <SFML/System/Vector2.hpp> // Required for sf::Vector2i
#include <SFML/Graphics/RenderWindow.hpp> // Required for sf::RenderWindow
#include <SFML/Graphics/Sprite.hpp> // Required for sf::Sprite
#include <SFML/Graphics/Texture.hpp>    // Required for sf::Texture

using std::pair; 
using std::make_pair;
using std::queue;
using std::unordered_set;
using std::vector;

// Implementation of the board_t constructor
board_t::board_t(const int W, const int H, const sf::Texture& placeholderTexture) {
    width = W;
    height = H;
    // It is highly recommended to use std::vector instead of raw 'new'
    // but we will keep the original structure for compatibility:
    cells = new cell_t * *[H];

	for (int i = 0; i < height; ++i) { // Allocate each row
		cells[i] = new cell_t * [W]; // Allocate each column
		for (int j = 0; j < width; ++j) { // Initialize each cell
			cells[i][j] = new cell_t(); // Create a new cell_t instance
            // Initialize the unique_ptr sprite here:
            cells[i][j]->sprite = std::make_unique<sf::Sprite>(placeholderTexture);
            // The next line is redundant comment/old code:
            // cells[i][j]->sprite = new sf::Sprite(); 
        }
    }
}
// Implementation of the board_t destructor
board_t::~board_t() {
	for (int i = 0; i < height; ++i) { // Deallocate each row
		for (int j = 0; j < width; ++j) { // Deallocate each cell
            // The next line is redundant comment/old code:
            // delete cells[i][j]->sprite;
			delete cells[i][j]; // Delete the cell_t instance
        }
		delete[] cells[i]; // Delete the column array
    }
	delete[] cells; // Delete the row array
}

// Implementation of the reset method
// Resets all cells on the board to their initial state
void board_t::reset() {
	for (int i = 0; i < height; ++i) { // Iterate through each row
		for (int j = 0; j < width; ++j) { // Iterate through each column
			cells[i][j]->reset(); // Reset the cell to its initial state
        }
    }
}

// Implementation of the place_mines method
void board_t::place_mines(const int count) { // Place 'count' mines randomly on the board
	int i = 0; // Counter for placed mines
	srand(static_cast<unsigned int>(time(nullptr))); // Seed the random number generator
	unordered_set<int> positions; // To track unique mine positions
	// Using unordered_set for O(1) average time complexity on lookups
	while (i < count) { // Loop until 'count' mines are placed
		int pos = rand() % (width * height); // Random position in the board
		if (positions.count(pos) > 0) // Check if position already has a mine
			continue;       // Skip if mine already placed here
		int row = pos / width; // Calculate row from position
		int col = pos % width; // Calculate column from position
		positions.insert(pos); // Mark this position as having a mine
		// Place the mine in the cell
        place_mine(row, col);
		++i; // Increment the mine counter
    }
}
// Implementation of the place_mine method
void board_t::place_mine(unsigned int row, unsigned int col) { // Place a mine at the specified row and column
	auto cell = cells[row][col]; // Get the cell at the specified position
	cell->place_mine(); // Place a mine in the cell
	// Increment adjacent mine counts for neighboring cells
	auto neighbours = get_adjacent_positions(row, col); // Get adjacent positions
	for (auto pos : neighbours) { // Iterate through each adjacent position
		cells[pos.first][pos.second]->increment_adjacent_mines(); // Increment the adjacent mine count
    }
}
// Implementation of the get_adjacent_positions method
vector<pair<int, int>> board_t::get_adjacent_positions(unsigned int row, unsigned int col) { // Get valid adjacent positions for a given cell
	vector<pair<int, int>> neighbours; // Vector to store adjacent positions
	for (int i = -1; i <= 1; i++) { // Iterate through rows
		for (int j = -1; j <= 1; j++) { // Iterate through columns
			if (i == 0 && j == 0) continue; // Skip the cell itself
			if ((static_cast<int>(row) + i >= 0) && (static_cast<int>(row) + i < static_cast<int>(height)) && // Check row bounds
				(static_cast<int>(col) + j >= 0) && (static_cast<int>(col) + j < static_cast<int>(width))) // Check column bounds
				neighbours.emplace_back(row + i, col + j); // emplace_back is more efficient here for runtime
        }
    }
	return neighbours; // Return the vector of adjacent positions
}

// Implementation of the reveal_cell method
void board_t::reveal_cell(const unsigned int row, const unsigned int col) { // Reveal the cell at the specified row and column
	if (row >= static_cast<unsigned int>(height) || col >= static_cast<unsigned int>(width)) // Check for out-of-bounds
		return; // Do nothing if out of bounds

	// Reveal the initial cell
	auto cell = cells[row][col]; // Get the cell at the specified position
	bool mine_revealed = cell->reveal(); // Reveal the cell and check if it was a mine
	if (mine_revealed) // If a mine was revealed
        throw GameOver(); // Throw GameOver exception
	// If the cell does not auto-reveal neighbors, return early
	queue<pair<int, int>> q; // Queue for BFS
	unordered_set<cell_t*> visited; // Set to track visited cells
	// Start BFS from the revealed cell
	q.push(make_pair(row, col)); // Push the initial position onto the queue
	// Perform BFS to reveal connected cells
	while (!q.empty()) { // While there are positions to process
		auto pos = q.front(); // Get the front position
		q.pop(); // Remove it from the queue
		cell = cells[pos.first][pos.second]; // Get the cell at the current position
		// Skip if already visited
        if (visited.count(cell) > 0) {
            continue;
        }
		visited.insert(cell); // Mark cell as visited
		// Skip if the cell does not auto-reveal
        if (!cell->auto_reveal())
            continue;
		// Reveal the cell
        cell->reveal();
		if (cell->auto_reveal_neighbors()) { // If the cell auto-reveals neighbors
			auto neighbors = board_t::get_adjacent_positions(pos.first, pos.second); // Get adjacent positions
			for (auto neighbor : neighbors) // Iterate through each neighbor
				q.push(neighbor); // Push neighbor position onto the queue
        }
    }
}
// Implementation of the reveal_all_cells method
void board_t::reveal_all_cells() { // Reveal all cells on the board
	for (int i = 0; i < height; ++i) { // Iterate through each row
		for (int j = 0; j < width; ++j) { // Iterate through each column
			cells[i][j]->unflag(); // Unflag the cell before revealing
			cells[i][j]->reveal(); // Reveal the cell
        }
    }
}
// Implementation of the toggle_flag_cell method
void board_t::toggle_flag_cell(const unsigned int row, const unsigned int col) {
	if (row < static_cast<unsigned int>(height) && col < static_cast<unsigned int>(width)) // Check for valid bounds
		cells[row][col]->toggle_flag(); // Toggle the flag state of the cell
}

// Implementation of the draw method
void board_t::draw(sf::RenderWindow* window, TextureManager* textureManager, int cell_size) { // Draw the board onto the provided SFML RenderWindow
	window->clear(); // Clear the window before drawing
	// Iterate through each cell and draw its sprite
	for (int i = 0; i < height; ++i) { // Iterate through each row
		for (int j = 0; j < width; ++j) { // Iterate through each column
			cell_t* cell = cells[i][j]; // Get the cell at the current position
			// Retrieve the appropriate texture based on the cell's display value
            const sf::Texture& texture = textureManager->retrieve(cell->get_display_value());
			// Update the sprite's texture and position
			if (cell->sprite) { // Ensure the sprite unique_ptr is valid
				cell->sprite->setTexture(texture, true); // Update texture, reset rect
				cell->sprite->setPosition(sf::Vector2f(static_cast<float>(j * cell_size), static_cast<float>(i * cell_size))); // Set position based on cell size
				// Draw the sprite onto the window
                window->draw(*cell->sprite);
            }
        }
    }
    // THE draw function ENDS HERE. The following functions must be defined OUTSIDE.
}


// Implementation of the getFlagCount function
int board_t::getFlagCount() const {
    int count = 0;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            // Call the public getter method
            if (cells[i][j]->getIsFlagged()) {
                count++;
            }
        }
    }
    return count;
}


// The getIsFlagged function for cell_t belongs in cell.cpp, not here.
/*
bool cell_t::getIsFlagged() const {
    return is_flagged;
}
*/
