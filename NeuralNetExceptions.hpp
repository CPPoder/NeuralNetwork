#ifndef NEURALNETEXCEPTIONS_HPP
#define NEURALNETEXCEPTIONS_HPP


#include <string>

class NeuralNet;
class SequentialNet;
class Layer;
class DenseLayer;



struct InvalidLayerPointerException
{
	std::string message;
	Layer const* layerPointer;
	InvalidLayerPointerException(std::string const & message, Layer const* layerPointer);
};

struct CompileNeuralNetException
{

};






#endif //NEURALNETEXCEPTIONS_HPP

