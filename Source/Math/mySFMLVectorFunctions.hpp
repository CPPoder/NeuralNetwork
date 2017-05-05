#ifndef MYSFMLVECTORFUNCTIONS_HPP
#define MYSFMLVECTORFUNCTIONS_HPP

#include "SFML\Graphics.hpp"

#include <iostream>
#include <math.h>
#include <string>
#include <map>


namespace mySFML {

	namespace Class
	{

		////////////
		//Class Text
		class Text
		{
		public:
			sf::Text* pointer;

		public:
			Text() = delete;
			explicit Text(sf::Font const & font, sf::Vector2f const & position, std::string const & text = "", unsigned int characterSize = 12u, sf::Color const & fillColor = sf::Color::Black);
			Text(Text const & text);
			Text& operator=(Text const & text);
			~Text();

			void setString(std::string const & string);

		}; //Class Text

		/////////////////
		//Class RectShape
		class RectShape
		{
		public:
			sf::RectangleShape* pointer;

		public:
			RectShape();
			explicit RectShape(sf::Vector2f const & position, sf::Vector2f const & size, sf::Color const & fillColor, bool centeredOrigin, float outlineThickness = 0.f, sf::Color const & outlineColor = sf::Color::Black);
			RectShape(RectShape const & rectShape);
			RectShape& operator=(RectShape const & rectShape);
			~RectShape();

			void setFillColor(sf::Color const & fillColor);

		}; //Class RectShape

		/////////////
		//Class Fonts
		enum class FontName
		{
			ARIAL,
			INFORMAL_ROMAN
		};

		class Fonts
		{
		private:
			static unsigned int sNumberOfFontsObjects;
			static std::map<FontName, sf::Font*> sFontsMap;


		public:
			Fonts();
			~Fonts();
			Fonts(Fonts const &) = delete;
			Fonts& operator=(Fonts const &) = delete;

			sf::Font const * getFont(FontName fontName);


		};

	} //Namespace Class


	namespace Simple
	{

		////////
		//Length
		template <typename T> T lengthOf(sf::Vector2<T> const & vector)
		{
			return sqrt(vector.x * vector.x + vector.y * vector.y);
		}
		template <typename T> T lengthOf(sf::Vector3<T> const & vector)
		{
			return sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
		}

		//////////////////
		//Angle in radiant
		template <typename T> T angleOf(sf::Vector2<T> const & vector)
		{
			if (vector.x == T(0) && vector.y == T(0))
			{
				std::cerr << "template <typename T> T mySFML::angleOf(sf::Vector2<T> vector) : Error! vector is (0,0)!" << std::endl;
				return T(0);
			}
			sf::Vector2<T> normVec = mySFML::normalize(vector);
			normVec = sf::Vector2<T>(normVec.x, -normVec.y);
			float angle;
			if (myMath::abs(normVec.y) < myMath::SQRT2f / 2.f)
			{
				angle = atan(normVec.y / normVec.x);
			}
			else
			{
				bool secondOrFourthQuadrant = ((normVec.x < 0 && normVec.y > 0) || (normVec.x > 0 && normVec.y < 0));
				if (!secondOrFourthQuadrant)
				{
					angle = (myMath::PId / 2.0 - atan(normVec.x / normVec.y));
				}
				else
				{
					angle = (myMath::PId * 3.0 / 2.0 - atan(normVec.x / normVec.y));
				}
			}
			if (vector.x < 0)
			{
				angle += myMath::PId;
			}
			while (angle > 2.0 * myMath::PId)
			{
				angle -= 2.0 * myMath::PId;
			}
			while (angle < 0.0)
			{
				angle += 2.0 * myMath::PId;
			}
			return angle;
		}

		///////////
		//Normalize
		template <typename T> sf::Vector2<T> normalize(sf::Vector2<T> const & vector)
		{
			T length = lengthOf(vector);
			if (length == T(0))
			{
				std::cerr << "template <typename T> sf::Vector2<T> normalize(sf::Vector2<T> vector) : Error! vector is (0, 0)!" << std::endl;
				return sf::Vector2<T>(T(0), T(0));
			}
			else
			{
				return (vector / length);
			}
		}
		template <typename T> sf::Vector3<T> normalize(sf::Vector3<T> const & vector)
		{
			T length = lengthOf(vector);
			if (length == T(0))
			{
				std::cerr << "template <typename T> sf::Vector3<T> normalize(sf::Vector3<T> vector) : Error! vector is (0, 0, 0)!" << std::endl;
				return sf::Vector3<T>(T(0), T(0), T(0));
			}
			else
			{
				return (vector / length);
			}
		}

		//////////////////////////////////////////////////////////////////////////////
		//Componentwise Multiplication (Returns a Vector with the first arguments type)
		template <typename T, typename S> sf::Vector2<T> compMult(sf::Vector2<T> const & vec1, sf::Vector2<S> const & vec2)
		{
			return sf::Vector2<T>(vec1.x * vec2.x, vec1.y * vec2.y);
		}
		template <typename T, typename S> sf::Vector3<T> compMult(sf::Vector3<T> const & vec1, sf::Vector3<S> const & vec2)
		{
			return sf::Vector3<T>(vec1.x * vec2.x, vec1.y * vec2.y, vec1.z * vec2.z);
		}

		/////////////
		//Mean Vector
		sf::Vector2f meanVector(sf::Vector2f const & vec1, sf::Vector2f const & vec2);

		///////////////
		//Inner product
		template <typename T> T scalarProduct(sf::Vector2<T> const & vec1, sf::Vector2<T> const & vec2)
		{
			return (vec1.x * vec2.x + vec1.y * vec2.y);
		}
		template <typename T> T scalarProduct(sf::Vector3<T> const & vec1, sf::Vector3<T> const & vec2)
		{
			return (vec1.x * vec2.x + vec1.y * vec2.y + vec1.z * vec2.z);
		}

		/////////////////
		//Cross Product
		template <typename T> sf::Vector3<T> crossProduct(sf::Vector3<T> const & vec1, sf::Vector3<T> const & vec2)
		{
			return sf::Vector3<T>(vec1.y * vec2.z - vec1.z * vec2.y, vec1.z * vec2.x - vec1.x * vec2.z, vec1.x * vec2.y - vec1.y * vec2.x);
		}

		//////
		//Trim
		template <typename T> sf::Vector2<T> trim(sf::Vector2<T> const & lowerBound, sf::Vector2<T> const & input, sf::Vector2<T> const & upperBound)
		{
			return sf::Vector2<T>(myMath::Simple::trim(lowerBound.x, input.x, upperBound.x), myMath::Simple::trim(lowerBound.y, input.y, upperBound.y));
		}
		template <typename T> sf::Vector3<T> trim(sf::Vector3<T> const & lowerBound, sf::Vector3<T> const & input, sf::Vector3<T> const & upperBound)
		{
			return sf::Vector3<T>(myMath::Simple::trim(lowerBound.x, input.x, upperBound.x), myMath::Simple::trim(lowerBound.y, input.y, upperBound.y), myMath::Simple::trim(lowerBound.z, input.z, upperBound.z));
		}

		///////////
		//SwapXandY
		template <typename T> sf::Vector2<T> swapXandY(sf::Vector2<T> const & vec)
		{
			return sf::Vector2<T>(vec.y, vec.x);
		}


	} //Namespace Simple

	namespace Rand
	{

		///////////////////////
		//DirtyRandNormalVector
		sf::Vector2f dirtyRandNormalVector(int parameter);

	} //Namespace Rand
	
	namespace Comp
	{

		bool smaller(sf::Vector2f const & smallerVector, sf::Vector2f const & largerVector);
		bool smaller(sf::Vector2f const & smallerVector, sf::Vector2f const & largerVector, bool & xSmaller, bool & ySmaller);
		bool smaller(sf::Vector2i const & smallerVector, sf::Vector2i const & largerVector);
		bool smaller(sf::Vector2i const & smallerVector, sf::Vector2i const & largerVector, bool & xSmaller, bool & ySmaller);

	} //Namespace Comp
	
	namespace IO
	{

		/////////////////
		//Terminal Output
		template <typename T> sf::Vector2<T> outputOnTerminal(sf::Vector2<T> const & vector, std::string const & description = "", std::string const & seperator = " ")
		{
			std::cout << description << vector.x << seperator << vector.y << std::endl;
			return vector;
		}
		template <typename T> sf::Vector3<T> outputOnTerminal(sf::Vector3<T> const & vector, std::string const & description = "", std::string const & seperator = " ")
		{
			std::cout << description << vector.x << seperator << vector.y << seperator << vector.z << std::endl;
			return vector;
		}

	} //Namespace IO

	namespace Color
	{

		//////////////////////////////////////////////////
		//ColorMultiplication (Result has Alpha Value 255)
		sf::Color colorMultiplication(sf::Color const & color, float factor);

		////////////
		//Mix colors
		sf::Color mixColors(sf::Color const & color1, sf::Color const & color2, float weight1 = 0.5f, float weight2 = 0.5f);

		////////////
		//Rand Color
		sf::Color randColor(sf::Uint8 alpha = 255);

	} //Color

	namespace Shape
	{

		///////////////////////
		//Create RectangleShape
		sf::RectangleShape createRectangleShape(sf::Vector2f const & pos, sf::Vector2f const & size, float rotation, sf::Color const & color, float outlineThickness = 0.f, sf::Color const & outlineColor = sf::Color::Black);

	} //Namespace Shape

	namespace Create
	{

		//////////////////
		//OrthogonalVector
		template <typename T> sf::Vector2<T> createOrthogonalVector(sf::Vector2<T> const & vector)
		{
			sf::Vector2<T> result;
			result.x = -vector.y;
			result.y = vector.x;
			return result;
		}

		/////////////////////////////
		//CreateNormalVectorWithAngle
		sf::Vector2f createNormalVectorWithAngle(float angleInRadiant);

	} //Namespace Create

} //Namespace mySFML


#endif //MYSFMLVECTORFUNCTIONS_HPP
