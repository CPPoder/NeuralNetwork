#include "NeuralNetExceptions.hpp"
#include "DenseLayer.hpp"


InvalidLayerPointerException::InvalidLayerPointerException(std::string const & message, Layer const* layerPointer)
	: message(message), layerPointer(layerPointer)
{
}




