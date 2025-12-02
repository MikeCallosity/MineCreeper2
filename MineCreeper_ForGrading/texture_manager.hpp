

#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <string>
#include <map>
#include <SFML/Graphics.hpp>
#include <memory> // Required for std::unique_ptr
#include <SFML/Graphics/Texture.hpp> // Ensure the correct header is included

class TextureManager
{
    // Change the map to store unique pointers
private:
	typedef std::map<char, std::unique_ptr<sf::Texture>> TextureMap; // Map to hold textures with unique_ptr
	TextureMap data; // Map to hold textures with unique_ptr

public:
	TextureManager();// Constructor for our TextureManager
    // Retrieve returns a reference to the texture managed by the pointer
	const sf::Texture& retrieve(char key) const; // Retrieve texture by key (such as 0-9, f, b)
    // Load function needs to create a new texture on the heap
	bool load(const std::string& path, char key); // Load texture from file
	void loadAll(); // Load all predefined textures
	~TextureManager(); // Destructor to clean up resources
};

#endif // TEXTURE_MANAGER_H // End of include guard
