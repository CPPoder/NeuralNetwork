#ifndef CAR_HPP
#define CAR_HPP

#include "SFML\Graphics.hpp"

#include "Source\Math\mySFMLVectorFunctions.hpp"

#include "Source\Car\Brain.hpp"
#include "Source\Car\PlayerBrain.hpp"

class World;

class Car
{
private:
	//State variables
	sf::Vector2f mPosition;
	sf::Vector2f mDirection;
	float mVelocity;

	//Factors
	float mGasOrBrakeFactor = 0.f; //1.f = full acceleration, -1.f = full braking
	float mSteeringWheelFactor = 0.f; //1.f = full right, -1.f = full left

	//Derivatives
	float mGasBrakeFactorDerivative = 0.f;
	float mSteeringWheelFactorDerivative = 0.f;

	//Maximal values
	float const mMaximalPower = 500.f * 735.5f; //Power in W, 1 PS = 735.5 W
	float const mMaximalBrakeForce = 100000.f; //Find appropriate value for this!
	float const mMaximalSteeringWheelAngle = 0.5f; //~30°

	//Car Properties
	float const mMass = 1000.f;
	float const mFrictionCoefficient = 1.0f;
	float const mRollingFrictionCoefficient = 0.015f;
	float const mDistanceBetweenFrontAndBackWheels = 3.f;
	float const mAirArea = 5.f;
	float const mCW = 0.35f;

	//Drifting
	bool mDrifting = false;

	//Damage
	float mDamage = 0.f; //0.f = no damage, 1.f = full damage

	//Brain
	Brain* pBrain = nullptr;

	//Drawing
	sf::VertexArray mVertexArray;
	sf::VertexArray mTiresVertexArray;

	//Velocity text
	mySFML::Class::Fonts mFonts;
	mySFML::Class::Text mVelocityText;


public:
	//Constructors & Destructors
	Car();
	Car(sf::Vector2f const & position, sf::Vector2f const & direction, float velocity, BrainType brainType);
	~Car();
	Car(Car const & car);
	Car& operator=(Car const & car) = delete;

	//Render & Update
	void render(sf::RenderWindow * renderWindow) const;
	void renderSeeingLinesIfNetBrain(sf::RenderWindow * renderWindow, sf::View view) const;
	void update(sf::Time const & time, sf::RenderWindow const * renderWindow, World const * worldPointer);

public:
	//State Getter
	sf::Vector2f getPosition() const;
	sf::Vector2f getDirection() const;
	float getVelocity() const;

	//Factor Getter
	float getGasOrBrakeFactor() const;
	float getSteeringWheelFactor() const;

	//Maximal Values Getter
	float getMaximalPower() const;
	float getMaximalBrakeForce() const;
	float getMaximalSteeringWheelAngle() const;

	//Properties Getter
	float getMass() const;
	float getFrictionCoefficient() const;
	float getDistanceBetweenFrontAndBackWheels() const;

	//Damage Getter
	float getDamage() const;

	//Drawing Getter
	sf::VertexArray const & getVertexArrayReference() const;

	//Setter
	void setPosition(sf::Vector2f const & position);

private:
	void setVertexArray();

public:
	bool checkForBoundaryCollision(World const * worldPointer) const;



};





#endif //CAR_HPP

