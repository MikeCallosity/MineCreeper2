#include <exception> // Required for std::exception

// Definition of the GameOver exception
struct GameOver : public std::exception { // Inherit from std::exception for standard exception behavior compatibility
	const char* what() const throw() { // Override the what() method to provide an error message
		return "Game Is Over"; // Return a C-style string indicating the game is over
    }
};