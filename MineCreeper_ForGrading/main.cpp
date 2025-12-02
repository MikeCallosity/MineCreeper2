#include "board.hpp" // Include the corresponding header file
#include "gameover.hpp" // Include GameOver exception definition
#include "texture_manager.hpp" // Include TextureManager definition
#include "gamestats.hpp" // Add the new include
#include <iostream> // For better error reporting
#include <optional> // Required for std::optional
#include <SFML/Audio.hpp> // Required for audio components if needed in future features
#include <SFML/Graphics.hpp> // Required for sf::RenderWindow and other graphics components
#include <SFML/Window/Event.hpp> // Explicitly include the new Event header

// Using directives should be used with caution, but kept as per original code style
using namespace sf;
// Define constants for board dimensions and mine count
#define BOARD_WIDTH 12 // Number of columns
#define BOARD_HEIGHT 12 // Number of rows
#define MINES 20 // Total number of mines
#define CELL_SIZE 50 // Size of each cell in pixels
// Using directives for standard library components
using std::cin;
using std::cout;
using std::endl;

// Function to display the game manual
void show_manual() {
	cout << "Manual:" << endl; // Print the game manual header
	cout << "\tReveal Cell\t - Mouse Left Click" << endl; // Instructions for revealing a cell
	cout << "\tFlag Cell\t - Mouse Right Click" << endl; // Instructions for flagging a cell
	cout << "\tReset Board\t - Press 'R' on Keyboard" << endl; // Instructions for resetting the board
}
// Placeholder texture for initializing cells with a black transparent texture which has no visible content
sf::Texture blankTexture;
// Main function to run the MineSweeper game
int main() {
	show_manual(); // Display the game manual
	board_t board(BOARD_WIDTH, BOARD_HEIGHT, blankTexture); // Initialize the game board with specified dimensions and placeholder texture
	board.place_mines(MINES); // Place mines on the board

	// Game state flags
	bool isGameOver = false; // Flag to track if the game is over
    // We start the timer once the user makes the first move
	bool gameStarted = false; // Flag to track if the game has started
	// Calculate window dimensions based on board size and cell size
	int width = BOARD_WIDTH * CELL_SIZE; // Calculate window width
	int height = BOARD_HEIGHT * CELL_SIZE;  // Calculate window height

	// Instantiate GameStatsDisplay with window dimensions ---
    GameStatsDisplay statsDisplay(MINES, static_cast<float>(width), static_cast<float>(height));
    // ^^^ Pass required constructor arguments: minesTotal, windowWidth, windowHeight

    // Adjust the flags text position now that we know the window width
	statsDisplay.setFlagsTextPosition(sf::Vector2f(static_cast<float>(width - 165), 10.f)); // Position flags counter on the right side of the window

    // Use scoped enum for style flags (Style::Titlebar, Style::Close)
    unsigned int style = sf::Style::Titlebar | sf::Style::Close;

    // Use the new Vector2u syntax for VideoMode arguments
    sf::RenderWindow window(sf::VideoMode({ static_cast<unsigned int>(width), static_cast<unsigned int>(height) }),
        "MineSweeper Game", style);
	// Create a blank transparent image for the placeholder texture
    sf::Image blankImage({ static_cast<unsigned int>(CELL_SIZE), static_cast<unsigned int>(CELL_SIZE) }, sf::Color(0, 0, 0, 0));

    // loadFromImage still works the same but we add error handling for robustness
    
	// --- FIX IS HERE: Load the blank texture properly ---
    if (!blankTexture.loadFromImage(blankImage)) { 
        std::cerr << "Error loading blank texture!" << std::endl;
        return -1; // Exit if the basic texture fails
    }

	(void)blankTexture.loadFromImage(blankImage); // Load the blank image into the texture
   
	// Initialize and load all textures using TextureManager
    TextureManager textureManager;
	textureManager.loadAll(); // Load all predefined textures

    // Initial draw before entering the main loop
	board.draw(&window, &textureManager, CELL_SIZE); // Draw the initial board state
	window.display(); // Display the initial window content
	// Main game loop
	while (window.isOpen()) // Loop while the window is open
    {
        // Use std::optional to handle the modern event loop
		while (const std::optional<sf::Event> event = window.pollEvent()) { // Poll events from the window

            // Use event->is<EventType>() and event->getIf<EventType>()
			if (event->is<sf::Event::Closed>())  // Check for window close event
            {
				window.close(); // Close the window
				break; // Exit the event loop
            }
			else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) // Check for key press event
            {
                // Use the scoped enum sf::Keyboard::Key::R
				// Reset the game if 'R' key is pressed
				if (keyPressed->code == sf::Keyboard::Key::R) { // If 'R' key is pressed
					isGameOver = false; // Reset game over flag
                    gameStarted = false; // Reset game state flags
                    statsDisplay.reset(); // Reset timer and counters
					board.reset(); // Reset the board state
					board.place_mines(MINES); // Place mines again
                }
            }
            // Combine mouse events into a single check
			else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>(); mouseButtonPressed && !isGameOver) // Check for mouse button press event and game not over
            {

                // Start the timer on the first interaction
				if (!gameStarted) { // If the game hasn't started yet
					gameStarted = true; // Mark the game as started
					statsDisplay.startGame(); // Start the timer
                }

                // SFML 3 stores mouse positions inside the event struct now (event.position.x)
                // Use integer division to find the pixel coordinates
				int pixelX = mouseButtonPressed->position.x; // Get the X coordinate of the mouse click
				int pixelY = mouseButtonPressed->position.y; // Get the Y coordinate of the mouse click

                // Calculate the board coordinates correctly:
                // Column (col) maps to X, Row (row) maps to Y
				int col = pixelX / CELL_SIZE; // Calculate the column index
				int row = pixelY / CELL_SIZE; // Calculate the row index

                // Use the scoped enum sf::Mouse::Button::Left

				// Handle left and right mouse button clicks
                if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
					try // Try to reveal the cell
                    {
                        // Pass (row, col) as expected by board.hpp/cpp
                        board.reveal_cell(row, col);
                    }
					catch (GameOver&) // Catch GameOver exception
                    {
						isGameOver = true; // Set game over flag
                        statsDisplay.stopGame(); // Stop the timer on game over
						board.reveal_all_cells(); // Reveal all cells on game over
                    }
                }
                // Check explicitly for the right button
				// Use the scoped enum sf::Mouse::Button::Right
				// Handle right mouse button click for flagging
                else if (mouseButtonPressed->button == sf::Mouse::Button::Right) {
                    // Pass (row, col) as expected
					board.toggle_flag_cell(row, col); // Toggle flag on the cell
                }
            }
        }

        // Drawing and displaying happens once per frame, outside the event-polling loop
		board.draw(&window, &textureManager, CELL_SIZE); // Draw the current board state
		// Update stats display with current flag count
		statsDisplay.draw(&window, board.getFlagCount()); // Draw the stats display onto the window
		window.display(); // Display the updated window content
    }

	return 0; // Exit the program successfully
}
