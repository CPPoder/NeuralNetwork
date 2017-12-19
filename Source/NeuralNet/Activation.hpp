#ifndef ACTIVATION_HPP
#define ACTIVATION_HPP

#include <functional>
#include <cmath>
#include <string>

#include "Source\NeuralNet\NeuralNet.hpp"



class Activation
{
private:
	std::function<NetNodeType(NetNodeType const & x)> f;
	enum class Type {Relu, Sigmoid, Theta, Atan} type;

public:
	Activation() = delete;
	Activation(std::function<NetNodeType(NetNodeType const & x)> f, Type type);

public:
	NetNodeType operator()(NetNodeType const & x) const;
	Mat::Vector<NetNodeType> operator()(Mat::Vector<NetNodeType> const & vec) const;
	Mat::Vector<NetNodeType>&& operator()(Mat::Vector<NetNodeType> && vec) const;

public:
	Type getType() const;


public:
	static const Activation RectifiedLinearUnit;
	static const Activation Sigmoid;
	static const Activation Theta;
	static const Activation Atan;

public:
	static Activation getActivationOfType(Type type);
	static std::string mapTypeToString(Type type);
	static Type mapStringToType(std::string const & string);


};





#endif //ACTIVATION_HPP

