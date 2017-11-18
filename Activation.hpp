#ifndef ACTIVATION_HPP
#define ACTIVATION_HPP

#include <functional>
#include <cmath>

#include "NeuralNet.hpp"



class Activation
{
private:
	std::function<NetNodeType(NetNodeType const & x)> f;

public:
	Activation() = delete;
	Activation(std::function<NetNodeType(NetNodeType const & x)> f);

public:
	NetNodeType operator()(NetNodeType const & x) const;
	Mat::Vector<NetNodeType> operator()(Mat::Vector<NetNodeType> const & vec) const;
	Mat::Vector<NetNodeType>&& operator()(Mat::Vector<NetNodeType> && vec) const;


public:
	static const Activation RectifiedLinearUnit;
	static const Activation Sigmoid;
	static const Activation Theta;



};





#endif //ACTIVATION_HPP

