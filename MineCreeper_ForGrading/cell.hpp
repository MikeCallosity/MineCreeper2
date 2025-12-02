#ifndef CELL_HPP
#define CELL_HPP

#include <string> // Required for std::string
#include <memory> // Required for std::unique_ptr
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <optional>

using std::string;
using std::unique_ptr;

struct cell_t { // Structure representing a single cell on the game board

    private:

		bool is_mine = false; // Indicates if the cell contains a mine

		bool is_flagged = false; // Indicates if the cell is flagged

		bool is_revealed = false; // Indicates if the cell has been revealed

		int adjacent_mines = 0; // Number of adjacent mines

    public:

		cell_t(); // Constructor to initialize the cell

		void place_mine(); // Places a mine in the cell

		bool reveal(); // Reveals the cell and returns true if it was a mine

		void toggle_flag(); // Toggles the flagged state of the cell

		void unflag(); // Unflags the cell

		void increment_adjacent_mines(); // Increments the count of adjacent mines

		bool auto_reveal(); // Determines if the cell should auto-reveal

		bool auto_reveal_neighbors(); // Determines if neighbors should auto-reveal

		char get_display_value(); // Gets the display value of the cell

		void reset(); // Resets the cell to its initial state

		bool getIsFlagged() const; // Returns whether the cell is flagged

		~cell_t(); // Destructor to clean up resources

		std::unique_ptr<sf::Sprite> sprite; // Unique pointer to the sprite representing the cell

};

#endif // CELL_HPP