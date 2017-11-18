#ifndef SEQUENTIALNET_HPP
#define SEQUENTIALNET_HPP


#include <vector>

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
	SequentialNet& operator=(SequentialNet const &) = delete;

public:
	virtual void addLayer(Layer const & layer) override;
	virtual void compile() override;

public:
	virtual NetOutput apply(NetInput const & input) const override;
	virtual void mutate(Mutation const & mutation) override;




};







#endif //SEQUENTIALNET_HPP

