#include <iostream>
#include <optional>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include "texture_manager.hpp"
#include "board.hpp"
#include "gameover.hpp"

// Using directives should be used with caution, but kept as per original code style
using namespace sf;

#define BOARD_WIDTH 12
#define BOARD_HEIGHT 12
#define MINES 20
#define CELL_SIZE 50
// Removed the old KEY_R macro, will use sf::Keyboard::Key::R directly in main

using std::cin;
using std::cout;
using std::endl;


void show_manual() {
    cout << "Manual:" << endl;
    cout << "\tReveal Cell\t - Mouse Left Click" << endl;
    cout << "\tFlag Cell\t - Mouse Right Click" << endl;
    cout << "\tReset Board\t - Press 'R' on Keyboard" << endl;
}

int main() {
    show_manual();
    board_t board(BOARD_WIDTH, BOARD_HEIGHT);
    board.place_mines(MINES);

    bool isGameOver = false;
    int width = BOARD_WIDTH * CELL_SIZE;
    int height = BOARD_HEIGHT * CELL_SIZE;
    // Use scoped enum for style
    unsigned int style = sf::Style::Titlebar | sf::Style::Close;
    sf::RenderWindow window(sf::VideoMode({static_cast<unsigned int>(width),static_cast<unsigned int> (height)}), "MineSweeper Game", style);
    TextureManager textureManager;
    textureManager.loadAll();

    board.draw(&window, &textureManager, CELL_SIZE);
    window.display();

    while (window.isOpen())
    {
        // Use std::optional and getIf for event handling
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
                break;
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                // Use the scoped enum sf::Keyboard::Key::R
                if (keyPressed->code == sf::Keyboard::Key::R) {
                    isGameOver = false;
                    board.reset();
                    board.place_mines(MINES);
                }
            }
            else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>(); mouseButtonPressed && !isGameOver)
            {
                // Mouse button events in SFML 3 require accessing the position via event data
                int i = mouseButtonPressed->position.x / CELL_SIZE;
                int j = mouseButtonPressed->position.y / CELL_SIZE;

                // Use the scoped enum sf::Mouse::Button::Left
                if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                    try
                    {
                        board.reveal_cell(i, j);
                    }
                    catch (GameOver&)
                    {
                        isGameOver = true;
                        board.reveal_all_cells();
                    }
                }
                // The original code used `else` which implies a right click (since it was inside a mouse button press block).
                // SFML 3 events require explicit checks if we want to differentiate button types.
                // Replaced 'else' with an explicit right button check for clarity and correctness
                else if (mouseButtonPressed->button == sf::Mouse::Button::Right) {
                    board.toggle_flag_cell(i, j);
                }
            }
        }
        // Drawing and displaying should happen outside the event loop
        board.draw(&window, &textureManager, CELL_SIZE);
        window.display();
    }

}