#include "Source\NeuralNet\Activation.hpp"

Activation::Activation(std::function<NetNodeType(NetNodeType const & x)> f)
	: f(f)
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


const Activation Activation::RectifiedLinearUnit([](NetNodeType const & x) { return ((x>0) ? x : 0); });
const Activation Activation::Sigmoid([](NetNodeType const & x) { return (1.f/(1.f + std::exp(-x))); });
const Activation Activation::Theta([](NetNodeType const & x) { return (x >= 0); });
const Activation Activation::Atan([](NetNodeType const & x) { return std::atan(x); });


