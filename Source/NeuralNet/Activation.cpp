#include "Source\NeuralNet\Activation.hpp"

Activation::Activation(std::function<NetNodeType(NetNodeType const & x)> f, Type type)
	: f(f), type(type)
{}


NetNodeType Activation::operator()(NetNodeType const & x) const
{
	return f(x);
}
Mat::Vector<NetNodeType> Activation::operator()(Mat::Vector<NetNodeType> const & vec) const
{
	Mat::Vector<NetNodeType> res(vec);
	res.doForEveryEntry([this](NetNodeType& entry, Mat::VectorEntry const & entryPos) {entry = f(entry); });
	return res;
}
Mat::Vector<NetNodeType>&& Activation::operator()(Mat::Vector<NetNodeType> && vec) const
{
	vec.doForEveryEntry([this](NetNodeType& entry, Mat::VectorEntry const & entryPos) {entry = f(entry); });
	return std::move(vec);
}

Activation::Type Activation::getType() const
{
	return type;
}

const Activation Activation::RectifiedLinearUnit([](NetNodeType const & x) { return ((x>0) ? x : 0); }, Type::Relu);
const Activation Activation::Sigmoid([](NetNodeType const & x) { return (1.f/(1.f + std::exp(-x))); }, Type::Sigmoid);
const Activation Activation::Theta([](NetNodeType const & x) { return (x >= 0); }, Type::Theta);
const Activation Activation::Atan([](NetNodeType const & x) { return std::atan(x); }, Type::Atan);



Activation Activation::getActivationOfType(Type type)
{
	switch (type)
	{
	case Type::Relu:
		return Activation::RectifiedLinearUnit;
	case Type::Sigmoid:
		return Activation::Sigmoid;
	case Type::Theta:
		return Activation::Theta;
	case Type::Atan:
		return Activation::Atan;
	}
}

std::string Activation::mapTypeToString(Type type)
{
	switch (type)
	{
	case Type::Relu:
		return "Relu";
	case Type::Sigmoid:
		return "Sigmoid";
	case Type::Theta:
		return "Theta";
	case Type::Atan:
		return "Atan";
	}
}

Activation::Type Activation::mapStringToType(std::string const & string)
{
	if (string == "Relu")
	{
		return Type::Relu;
	}
	if (string == "Sigmoid")
	{
		return Type::Sigmoid;
	}
	if (string == "Theta")
	{
		return Type::Theta;
	}
	if (string == "Atan")
	{
		return Type::Atan;
	}
	throw "String is no type!";
}






