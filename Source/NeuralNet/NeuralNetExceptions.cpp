#include "Source\NeuralNet\NeuralNetExceptions.hpp"
#include "Source\NeuralNet\DenseLayer.hpp"


InvalidLayerPointerException::InvalidLayerPointerException(std::string const & message, Layer const* layerPointer)
	: message(message), layerPointer(layerPointer)
{
}




