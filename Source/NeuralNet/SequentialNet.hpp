#ifndef SEQUENTIALNET_HPP
#define SEQUENTIALNET_HPP


#include <vector>
#include <fstream>
#include <sstream>

#include "Source\NeuralNet\NeuralNet.hpp"



class SequentialNet final : public NeuralNet
{
private:
	Mat::VectorSize mInputLayerSize;
	std::vector<Layer*> mVecOfLayers;

public:
	SequentialNet(Mat::VectorSize inputLayerSize);
	virtual ~SequentialNet() override;
	SequentialNet(SequentialNet const &) = delete;
	SequentialNet(SequentialNet&&) = delete;
	SequentialNet& operator=(SequentialNet const & sequentialNet);

public:
	virtual void addLayer(Layer const & layer) override;
	virtual void compile() override;

public:
	virtual NetOutput apply(NetInput const & input) const override;
	virtual void mutate(Mutation const & mutation) override;

public:
	void setMatrixOfLayer(unsigned int layer, Mat::Matrix<NetNodeType> const & matrix);
	void setBiasOfLayer(unsigned int layer, Mat::Vector<NetNodeType> const & bias);

public:
	virtual void saveToFile(std::string const & path) const override;



};







#endif //SEQUENTIALNET_HPP

