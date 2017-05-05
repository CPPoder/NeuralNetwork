#include "stdafx.h"
#include "mySFMLVectorFunctions.hpp"

namespace mySFML {

	namespace Class
	{

		////////////
		//Class Text
		Text::Text(Text const & text)
			: pointer(new sf::Text(*text.pointer))
		{
		}
		Text::~Text()
		{
			delete pointer;
			pointer = nullptr;
		}

		Text::Text(sf::Font const & font, sf::Vector2f const & position, std::string const & text, unsigned int characterSize, sf::Color const & fillColor)
			: pointer(new sf::Text(text, font, characterSize))
		{
			pointer->setPosition(position);
			pointer->setFillColor(fillColor);
		}

		Text& Text::operator=(Text const & text)
		{
			delete pointer;
			pointer = new sf::Text(*text.pointer);
			return *this;
		}

		void Text::setString(std::string const & string)
		{
			pointer->setString(string);
		}

		/////////////////
		//Class RectShape
		RectShape::RectShape()
			: RectShape(sf::Vector2f(200.f, 100.f), sf::Vector2f(30.f, 30.f), sf::Color::Green, false)
		{
		}

		RectShape::RectShape(RectShape const & rectShape)
			: pointer(new sf::RectangleShape(*rectShape.pointer))
		{
		}

		RectShape& RectShape::operator=(RectShape const & rectShape)
		{
			delete pointer;
			pointer = new sf::RectangleShape(*rectShape.pointer);
			return *this;
		}

		RectShape::RectShape(sf::Vector2f const & position, sf::Vector2f const & size, sf::Color const & fillColor, bool centeredOrigin, float outlineThickness, sf::Color const & outlineColor)
			: pointer(new sf::RectangleShape)
		{
			pointer->setSize(size);
			if (centeredOrigin)
			{
				pointer->setOrigin(size / 2.f);
			}
			pointer->setPosition(position);
			pointer->setFillColor(fillColor);
			pointer->setOutlineThickness(outlineThickness);
			pointer->setOutlineColor(outlineColor);
		}

		RectShape::~RectShape()
		{
			delete pointer;
			pointer = nullptr;
		}

		void RectShape::setFillColor(sf::Color const & fillColor)
		{
			pointer->setFillColor(fillColor);
		}
		
		/////////////
		//Class Fonts
		unsigned int Fonts::sNumberOfFontsObjects = 0u;
		std::map<FontName, sf::Font*> Fonts::sFontsMap = std::map<FontName, sf::Font*>();

		Fonts::Fonts()
		{
			if (sNumberOfFontsObjects == 0u)
			{
				sf::Font* arialFont = new sf::Font;
				arialFont->loadFromFile("./Data/Fonts/arial.ttf");
				sFontsMap.insert(std::pair<FontName, sf::Font*>(FontName::ARIAL, arialFont));

				sf::Font* informalRomanFont = new sf::Font;
				informalRomanFont->loadFromFile("./Data/Fonts/INFROMAN.TTF");
				sFontsMap.insert(std::pair<FontName, sf::Font*>(FontName::INFORMAL_ROMAN, informalRomanFont));
			}
			++sNumberOfFontsObjects;
#ifdef _DEBUG
			std::cout << "NumberOfFontsObjects: " << sNumberOfFontsObjects << std::endl;
#endif
		}

		Fonts::~Fonts()
		{
			if (sNumberOfFontsObjects == 0u)
			{
				std::cerr << "mySFML::Class::Fonts::~Fonts() : Error! sNumberOfFontsObject was already 0!" << std::endl;
				throw "Destructor call corrupted!";
			}
			--sNumberOfFontsObjects;
			if (sNumberOfFontsObjects == 0u)
			{
				for (auto& pair : sFontsMap)
				{
					delete pair.second;
					pair.second = nullptr;
				}
				sFontsMap.clear();
			}
#ifdef _DEBUG
			std::cout << "NumberOfFontsObjects: " << sNumberOfFontsObjects << std::endl;
#endif
		}

		sf::Font const * Fonts::getFont(FontName fontName)
		{
			return sFontsMap.at(fontName);
		}


	} //Namespace Class

	namespace Simple
	{

		/////////////
		//Mean Vector
		sf::Vector2f meanVector(sf::Vector2f const & vec1, sf::Vector2f const & vec2)
		{
			return sf::Vector2f(myMath::Simple::meanValue(vec1.x, vec2.x), myMath::Simple::meanValue(vec1.y, vec2.y));
		}

	} //Namespace Simple

	namespace Rand
	{

		///////////////////////
		//DirtyRandNormalVector
		sf::Vector2f dirtyRandNormalVector(int parameter)
		{
			return mySFML::Simple::normalize(sf::Vector2f(myMath::Rand::randIntervalf(-parameter, parameter), myMath::Rand::randIntervalf(-parameter, parameter)));
		}

	} //Namespace Rand

	namespace Comp
	{

		/////////
		//Smaller
		bool smaller(sf::Vector2f const & smallerVector, sf::Vector2f const & largerVector)
		{
			bool xSmaller = (smallerVector.x < largerVector.x);
			bool ySmaller = (smallerVector.y < largerVector.y);

			return (xSmaller && ySmaller);
		}
		bool smaller(sf::Vector2f const & smallerVector, sf::Vector2f const & largerVector, bool & xSmaller, bool & ySmaller)
		{
			xSmaller = (smallerVector.x < largerVector.x);
			ySmaller = (smallerVector.y < largerVector.y);

			return (xSmaller && ySmaller);
		}

		bool smaller(sf::Vector2i const & smallerVector, sf::Vector2i const & largerVector)
		{
			bool xSmaller = (smallerVector.x < largerVector.x);
			bool ySmaller = (smallerVector.y < largerVector.y);

			return (xSmaller && ySmaller);
		}
		bool smaller(sf::Vector2i const & smallerVector, sf::Vector2i const & largerVector, bool & xSmaller, bool & ySmaller)
		{
			xSmaller = (smallerVector.x < largerVector.x);
			ySmaller = (smallerVector.y < largerVector.y);

			return (xSmaller && ySmaller);
		}

	} //Namespace Comp

	namespace Color
	{

		//////////////////////////////////////////////////
		//ColorMultiplication (Result has Alpha Value 255)
		sf::Color colorMultiplication(sf::Color const & color, float factor)
		{
			return sf::Color(static_cast<sf::Uint8>(color.r * factor), static_cast<sf::Uint8>(color.g * factor), static_cast<sf::Uint8>(color.b * factor));
		}

		///////////
		//MixColors
		sf::Color mixColors(sf::Color const & color1, sf::Color const & color2, float weight1, float weight2)
		{
			return (colorMultiplication(color1, weight1) + colorMultiplication(color2, weight2));
		}

		////////////
		//Rand Color
		sf::Color randColor(sf::Uint8 alpha)
		{
			return sf::Color(myMath::Rand::randIntervali(0, 255), myMath::Rand::randIntervali(0, 255), myMath::Rand::randIntervali(0, 255), alpha);
		}

	} //Namespace Color

	namespace Shape
	{

		//////////////////////
		//CreateRectangleShape
		sf::RectangleShape createRectangleShape(sf::Vector2f const & pos, sf::Vector2f const & size, float rotation, sf::Color const & fillColor, float outlineThickness, sf::Color const & outlineColor)
		{
			sf::RectangleShape shape;
			shape.setPosition(pos);
			shape.setSize(size);
			shape.rotate(rotation);
			shape.setFillColor(fillColor);
			shape.setOutlineColor(outlineColor);
			shape.setOutlineThickness(outlineThickness);
			return shape;
		}

	} //Namespace Shape

	namespace Create
	{

		/////////////////////////////
		//CreateNormalVectorWithAngle
		sf::Vector2f createNormalVectorWithAngle(float angleInRadiant)
		{
			return sf::Vector2f(cos(angleInRadiant), sin(angleInRadiant));
		}

	} //Namespace Create


} //Namespace mySFML

