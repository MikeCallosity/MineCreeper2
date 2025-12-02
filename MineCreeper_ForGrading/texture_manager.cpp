#include "texture_manager.hpp"
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <string>
#include <map>
#include <iostream> // Include for error handling example if desired

using std::string;
using std::unique_ptr;

TextureManager::TextureManager() = default; // Default constructor

const sf::Texture& TextureManager::retrieve(const char key) const // Retrieve texture by key
{
	// 1. Find the texture in the map
	auto it = data.find(key); // Find the texture in the map
	if (it != data.end()) // If found
    {
        // Return a reference to the texture object held by the unique_ptr
        return *it->second;
    }

    // Error Handling: Returning a static empty texture is risky (might cause issues 
    // if used), throwing an exception is generally safer for a missing critical resource.
    // For this migration, we'll maintain the original behavior's structure.
    std::cerr << "Error: Texture with key '" << key << "' not found." << std::endl;
    static const sf::Texture empty; // This is dangerous as it might be used incorrectly
    return empty; // Consider using a 'default' texture or throwing an exception instead
}
// Load texture from file
bool TextureManager::load(const string& path, const char key)
{
	if (data.find(key) != data.end()) // Check if texture already loaded
    {
        // Texture already loaded, return success
        return true;
    }
	// Texture not loaded yet
    else
    {
        // 1. Create a new texture object dynamically using a smart pointer
        unique_ptr<sf::Texture> newTexturePtr = std::make_unique<sf::Texture>();

        // 2. Load the file into the new texture object
        if (newTexturePtr->loadFromFile(path))
        {
            // 3. Move the ownership of the texture pointer into the map
            data[key] = std::move(newTexturePtr);
            return true;
        }
		// 4. Load failed, unique_ptr will automatically clean up
        else
        {
            // Handle load failure
			std::cerr << "Error loading texture from path: " << path << std::endl; // Log error message
			return false; // Indicate failure
        }
    }
}
// Load all predefined textures
void TextureManager::loadAll() // Load all predefined textures
{
    // The calls remain the same, relying on the updated 'load' implementation by ley
    load("../res2/0.png", '0');
    load("../res2/1.png", '1');
    load("../res2/2.png", '2');
    load("../res2/3.png", '3');
    load("../res2/4.png", '4');
    load("../res2/5.png", '5');
    load("../res2/6.png", '6');
    load("../res2/7.png", '7');
    load("../res2/8.png", '8');
    load("../res2/b.png", 'b');
    load("../res2/f.png", 'f');
    load("../res2/n.png", 'n');
}

TextureManager::~TextureManager() = default;
// unique_ptr automatically cleans up the allocated textures when the map is destroyed.
