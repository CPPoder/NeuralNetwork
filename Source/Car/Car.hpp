#ifndef CAR_HPP
#define CAR_HPP

#include "SFML\Graphics.hpp"

#include "Source\Math\mySFMLVectorFunctions.hpp"

#include "Source\Car\Brain.hpp"
#include "Source\Car\PlayerBrain.hpp"


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

	float const mMaximalGasOrBrakeForce = 10000.f; //~3s from 0 to 100 kmh with 1000kg
	float const mMaximalSteeringWheelAngle = 0.5f; //~30°

	float const mMass = 1000.f;
	float const mFrictionCoefficient = 1.0f;
	float const mDistanceBetweenFrontAndBackWheels = 3.f;

	Brain* pBrain = nullptr;

	sf::VertexArray mVertexArray;
	sf::VertexArray mTiresVertexArray;


public:
	Car();
	Car(sf::Vector2f const & position, sf::Vector2f const & direction, float velocity);
	~Car();
	Car(Car const & car) = default;
	Car& operator=(Car const & car) = default;

	void render(sf::RenderWindow * renderWindow) const;
	void update(sf::Time const & time, sf::RenderWindow const * renderWindow, RaceSimulation const * raceSimPointer);

public:
	sf::Vector2f getPosition() const;
	sf::Vector2f getDirection() const;
	float getVelocity() const;
	float getGasBrakeForce() const;
	float getSteeringWheelAngle() const;
	float getMaximalGasBrakeForce() const;
	float getMaximalSteeringWheelAngle() const;
	float getMass() const;
	float getFrictionCoefficient() const;
	float getDistanceBetweenFrontAndBackWheels() const;

private:
	void setVertexArray();



};





#endif //CAR_HPP

