#ifndef CAR_HPP
#define CAR_HPP

#include "SFML\Graphics.hpp"

#include "Source\Math\mySFMLVectorFunctions.hpp"


class Car
{
private:
	sf::Vector2f mPosition;
	sf::Vector2f mDirection;
	float mVelocity;
	float mGasOrBrakeForce = 0.f;
	float mSteeringWheelAngle = 0.f; //Negative Sign for Left

	float mGasBrakeForceDerivative = 0.f;
	float mSteeringWheelAngleDerivative = 0.f;

	float const mMaximalGasOrBrakeForce = 3000.f; //~10s from 0 to 100 kmh with 1000kg
	float const mMaximalSteeringWheelAngle = 0.5f; //~30°

	float const mMass = 1000.f;
	float const mFrictionCoefficient = 0.5f;
	float const mDistanceBetweenFrontAndBackWheels = 3.f;

	sf::VertexArray mVertexArray;


public:
	Car();
	Car(sf::Vector2f const & position, sf::Vector2f const & direction, float velocity);
	~Car() = default;
	Car(Car const & car) = default;
	Car& operator=(Car const & car) = default;

	void render(sf::RenderWindow * renderWindow) const;
	void update(sf::Time const & time, sf::RenderWindow const * renderWindow);

private:
	void setVertexArray();



};





#endif //CAR_HPP

