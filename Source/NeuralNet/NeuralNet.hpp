#ifndef NEURALNET_HPP
#define NEURALNET_HPP


#include "Matrix.hpp"
#include "Source\NeuralNet\Mutation.hpp"
#include "Source\NeuralNet\NeuralNetExceptions.hpp"


class Layer;
class DenseLayer;


typedef float NetNodeType;
typedef Mat::Vector<NetNodeType> NetInput;
typedef Mat::Vector<NetNodeType> NetOutput;



///////////////////////////
//Abstract class: NeuralNet

class NeuralNet
{
private:


public:
	NeuralNet() = default;
	virtual ~NeuralNet() = default;
	NeuralNet(NeuralNet const &) = default;
	NeuralNet& operator=(NeuralNet const &) = default;

public:
	virtual void addLayer(Layer const & layer) = 0;
	virtual void compile() = 0;

public:
	virtual NetOutput apply(NetInput const & input) const = 0;
	virtual void mutate(Mutation const & mutation) = 0;

public:
	virtual void saveToFile(std::string const & path) const = 0;
	virtual void loadFromFile(std::string const & path) = 0;



};






#endif //NEURALNET_HPP

