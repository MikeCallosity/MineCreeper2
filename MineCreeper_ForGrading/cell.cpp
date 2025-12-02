#include "cell.hpp"
#include <memory>
// Note: You might want to include <iostream> or similar if you add error handling,
// but the logic here is self-contained C++.


// Initializing the unique_ptr to nullptr explicitly is fine, 
// but it is automatically null-initialized by default in C++11/14/17/20.
cell_t::cell_t() : sprite(nullptr) {}

// unique_ptr automatically cleans up the allocated sprite when cell_t is destroyed.
cell_t::~cell_t() {
}

void cell_t::place_mine() { // Places a mine in the cell
	is_mine = true; // Set the mine flag to true
}

bool cell_t::reveal() { // Reveals the cell and returns true if it was a mine
	if (is_flagged) return false; // Do not reveal if flagged

	is_revealed = true; // Mark the cell as revealed
	return is_mine; // Return true if it was a mine
}

void cell_t::toggle_flag() { // Toggles the flagged state of the cell
	is_flagged = !is_flagged; // Toggle the flag state and stores it in the is_flagged variable
}

void cell_t::unflag() { // Unflags the cell
	is_flagged = false; // Set the flag state to false and stores it in the is_flagged variable
}

void cell_t::increment_adjacent_mines() { // Increments the count of adjacent mines
	++adjacent_mines; // Increment the adjacent mines count by 1
}

bool cell_t::auto_reveal() { // Determines if the cell should auto-reveal
	return !is_mine; // Auto-reveal if not a mine
}

bool cell_t::auto_reveal_neighbors() { // Determines if neighbors should auto-reveal
	return !is_mine && adjacent_mines == 0; // Auto-reveal neighbors if not a mine and no adjacent mines
}
// Returns the display value of the cell based on its state
char cell_t::get_display_value() { // Gets the display value of the cell
	if (is_flagged) return 'f'; // 'f' for flagged
	if (!is_revealed) return 'n'; // 'n' for not revealed
	if (is_mine) return 'b'; // 'b' for bomb/mine

    // Converts the integer count (0-8) to its character representation ('0'-'8')
	return static_cast<char>(adjacent_mines + '0'); // '0'-'8' for number of adjacent mines
}
// Resets the cell to its initial state
void cell_t::reset() { // Resets the cell to its initial state
	adjacent_mines = 0; // Reset adjacent mines count
	is_flagged = false; // Reset flagged state
	is_revealed = false; // Reset revealed state
	is_mine = false; // Reset mine state
    // The sprite object is not reset here, but its texture and position are managed in board_t::draw/constructor.
}
// Returns whether the cell is flagged
bool cell_t::getIsFlagged() const { // Returns whether the cell is flagged
	return is_flagged; // Return the flagged state
}