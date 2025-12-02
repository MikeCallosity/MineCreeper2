#ifndef GAMESTATS_HPP
#define GAMESTATS_HPP
#include <SFML/Graphics.hpp>
#include <string>
#include <SFML/Graphics/RenderWindow.hpp> // Required for sf::RenderWindow parameter in draw method
#include <SFML/Graphics/Text.hpp>        // Required for sf::Text member variables
#include <SFML/Graphics/Font.hpp>        // Required for sf::Font member variable
#include <SFML/System/Clock.hpp>         // Required for sf::Clock member variable
#include <SFML/System/Vector2.hpp>       // Required for sf::Vector2f parameter in setFlagsTextPosition

// Class to display game statistics such as timer and flags count
class GameStatsDisplay { // Class to display game statistics such as timer and flags count
private:
	sf::Font font; // Font for rendering text
	sf::Text timerText; // Text object for displaying the timer
	sf::Text flagsText; // Text object for displaying the flags count
	sf::Clock clock; // Clock to track elapsed time
	int totalMines; // Total number of mines in the game
	bool gameRunning; // Flag to indicate if the game is running
	sf::Texture backgroundTexture; // Add texture member
	sf::Sprite backgroundSprite;   // Add sprite member
	// Add member variables to store dimensions
	float windowWidth; // Store window width
	float windowHeight; // Store window height

public:
	void reset(); // Resets the timer and flags count
	void startGame(); // Starts the game timer
	virtual void stopGame(); // Stops the game timer
	void draw(sf::RenderWindow* window, int flaggedCount); // Draws the stats display onto the provided SFML RenderWindow
	GameStatsDisplay(int minesTotal, float windowWidth, float windowHeight); // Constructor to initialize the stats display with total mines and window dimensions
	void setFlagsTextPosition(const sf::Vector2f& position); // Method to set the position of the flags text



#endif // GAMESTATS_HPP // End of include guard
};