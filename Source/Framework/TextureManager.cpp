#include "stdafx.h"
#include "Source\Framework\TextureManager.hpp"



void TextureManager::Init()
{
	sf::Texture* checkBoxTextureAtlas = new sf::Texture;
	checkBoxTextureAtlas->loadFromFile("./Data/Textures/CheckBox.png");
	mMapOfTextures.insert(std::make_pair(TextureManager::TextureName::CHECK_BOX_TEXTURE_ATLAS, checkBoxTextureAtlas));
}

sf::Texture const * TextureManager::getTexture(TextureName textureName)
{
	return mMapOfTextures.at(textureName);
}
sf::Texture * TextureManager::getTextureChangeable(TextureName textureName)
{
	return mMapOfTextures.at(textureName);
}







std::map<TextureManager::TextureName, sf::Texture*> TextureManager::mMapOfTextures = std::map<TextureManager::TextureName, sf::Texture*>();

