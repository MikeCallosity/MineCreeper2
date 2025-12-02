#include "gamestats.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp> // Ensure RenderWindow is defined
#include <SFML/System/Vector2.hpp> // Ensure this is included for sf::Vector2f
#include <SFML/Graphics/Color.hpp> // Ensure Color is defined
#include <SFML/Graphics/Text.hpp>  // Ensure Text is defined
#include <SFML/Graphics/Font.hpp>  // Ensure Font is defined
#include <SFML/System/Clock.hpp>  // Ensure Clock is defined
#include <SFML/System/Time.hpp> // Ensure Time is defined
#include <SFML/Graphics/RenderWindow.hpp> // Ensure RenderWindow is defined

// Implementation of the GameStatsDisplay constructor
GameStatsDisplay::GameStatsDisplay(int minesTotal, float windowWidth, float windowHeight)
// In SFML 3, sf::Text no longer accepts font as a constructor argument.
// Use default constructors and set them using setFont() later.
	: font() // Default constructor for font
	, timerText(font) // Default constructor for timerText
	, flagsText(font) // Default constructor for flagsText
	, clock() // Default constructor for clock
	, totalMines(minesTotal) // Initialize totalMines
	, gameRunning(false) // Initialize gameRunning to false
	, backgroundTexture() // Default constructor for backgroundTexture
	, backgroundSprite(backgroundSprite) // Default constructor for backgroundSprite
    , windowWidth(windowWidth) // Default width, can be set later
    , windowHeight(windowHeight) // Default height, can be set later
{
	// *** Load the font ***
	if (!font.openFromFile("../res/mojangles-extended.ttf")) { // Load font from file
		std::cerr << "Error loading font file '../res/mojangles-extended.ttf'! Timer/Score won't display." << std::endl; // Log error message
        // Add specific error handling here if the font is critical
    }
	// *** Configure the text objects ***
	timerText.setFont(font); // Set font for timer text
	timerText.setCharacterSize(24); // Set character size for timer text
	timerText.setFillColor(sf::Color::Red); // Set text color to red
    // Use the explicit sf::Vector2f constructor as good practice
	timerText.setPosition(sf::Vector2f(10.f, 10.f)); // Position timer text at top-left corner
	// Configure flagsText similarly
	flagsText.setFont(font); // Set font for flags text
	flagsText.setCharacterSize(24); // Set character size for flags text
	flagsText.setFillColor(sf::Color::Blue); // Set text color to blue
    // Position flags counter on the right side of the window, adjust as needed in main.cpp
	flagsText.setPosition(sf::Vector2f(100.f, 10.f)); // Initial position; can be updated later

    // *** Load the background image ***
	if (!backgroundTexture.loadFromFile("../res/Background.png")) { // Load background image from file
		std::cerr << "Error loading background image '../res/Background.png'! Falling back to a clear color." << std::endl; // Log error message
    }
	else { // If background image loaded successfully
		backgroundSprite.setTexture(backgroundTexture); // Set texture for background sprite

        // Scale sprite to window size
		const auto texSize = backgroundTexture.getSize(); // Get original texture size
		// Use static_cast to ensure floating-point division for scaling factors //
		//Scale the background sprite to fit the window dimensions
        backgroundSprite.setScale( 
			sf::Vector2f(
            windowWidth / static_cast<float>(texSize.x),
            windowHeight / static_cast<float>(texSize.y) 
            )
        );
    }
	reset(); // Reset timer and flags display
}
// Implementation of the reset method
void GameStatsDisplay::reset() { // Resets the timer and flags count
	clock.restart(); // Restart the clock
	gameRunning = false; // Set gameRunning to false
    // setString takes std::string in SFML 3.
	// Use std::to_string to convert integers to strings.
	timerText.setString("Time: 0s"); // Reset timer text
	flagsText.setString("Mines Left: " + std::to_string(totalMines)); // Reset flags text
}
// Implementation of the startGame method
void GameStatsDisplay::startGame() { // Starts the game timer
	gameRunning = true; // Set gameRunning to true
	clock.restart(); // Restart the clock
}
// Implementation of the stopGame method
void GameStatsDisplay::stopGame() { // Stops the game timer
	gameRunning = false; // Set gameRunning to false
}
// Implementation of the draw method
void GameStatsDisplay::draw(sf::RenderWindow* window, int flaggedCount) { // Draws the stats display onto the provided SFML RenderWindow
    // Draw the background first
	window->draw(backgroundSprite); // Draw the background sprite
	if (gameRunning) { // If the game is running 
		// Update the timer text with elapsed time in seconds
        int seconds = static_cast<int>(clock.getElapsedTime().asSeconds());
        timerText.setString("Time: " + std::to_string(seconds) + "s");
    }
	// Update the flags text with remaining mines
	int minesLeft = totalMines - flaggedCount; // Calculate remaining mines
	flagsText.setString("Mines Left: " + std::to_string(minesLeft)); // Update flags text
	// Draw the timer and flags text
	window->draw(timerText); // Draw the timer text
	window->draw(flagsText); // Draw the flags text
}
// Implementation of the setFlagsTextPosition method
void GameStatsDisplay::setFlagsTextPosition(const sf::Vector2f& position) // Method to set the position of the flags text
    {
	flagsText.setPosition(position); // Set the position of the flags text
    }