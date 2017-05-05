#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP

#include "SFML\Graphics.hpp"

#include <map>




class TextureManager
{
public:
	enum class TextureName
	{
		CHECK_BOX_TEXTURE_ATLAS,

		NUM_OF_TEXTURE_NAMES
	};



private:
	static std::map<TextureName, sf::Texture*> mMapOfTextures;


public:
	TextureManager() = delete;
	~TextureManager() = delete;
	TextureManager(TextureManager const &) = delete;
	TextureManager& operator=(TextureManager const &) = delete;

	static sf::Texture const * getTexture(TextureName textureName);
	static sf::Texture * getTextureChangeable(TextureName textureName);

	static void Init();
};





#endif //TEXTUREMANAGER_HPP

