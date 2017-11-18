#ifndef LAYER_HPP
#define LAYER_HPP

#include "Matrix.hpp"
#include "Source\NeuralNet\NeuralNet.hpp"



class Layer
{
private:


protected:
	Layer() = default;

public:
	virtual ~Layer() = 0;
	virtual Mat::VectorSize getLayerSize() const = 0;

public:
	virtual Mat::Vector<NetNodeType> apply(Mat::Vector<NetNodeType> const & in) const = 0;
	virtual Mat::Vector<NetNodeType>&& apply(Mat::Vector<NetNodeType> && in) const = 0;


};





#endif //LAYER_HPP

