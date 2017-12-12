#include "Source\NeuralNet\SequentialNet.hpp"
#include "Source\NeuralNet\DenseLayer.hpp"


SequentialNet::SequentialNet(Mat::VectorSize inputLayerSize)
	: mInputLayerSize(inputLayerSize)
{
}


SequentialNet::~SequentialNet()
{
	for (auto pointer : mVecOfLayers)
	{
		delete pointer;
	}
	mVecOfLayers.clear();
}


void SequentialNet::addLayer(Layer const & layer)
{
	Layer const * layerPointer = &layer;

	DenseLayer const * denseLayerPointer = dynamic_cast<DenseLayer const *>(layerPointer);
	if (denseLayerPointer != nullptr)
	{
		mVecOfLayers.push_back(new DenseLayer(*denseLayerPointer));
	}
	else
	{
		throw InvalidLayerPointerException("SequentialNet::addLayer(Layer const & layer): Could not dynamic_cast layer to DenseLayer const *!", layerPointer);
	}
}


void SequentialNet::compile()
{
	std::vector<Mat::VectorSize> vecOfLayerSizes;
	vecOfLayerSizes.push_back(this->mInputLayerSize); //Add input layer, which has neither matrix nor bias
	for (auto layerPointer : mVecOfLayers)
	{
		vecOfLayerSizes.push_back(layerPointer->getLayerSize());
	}

	for (unsigned int layerNum = 1; layerNum < vecOfLayerSizes.size(); ++layerNum) //Skip input layer! std::vector gives access to previous layerSizes!
	{
		DenseLayer* denseLayerPointer = dynamic_cast<DenseLayer*>(this->mVecOfLayers.at(layerNum - 1));
		if (denseLayerPointer != nullptr)
		{
			Mat::VectorSize sizeOfPreviousLayer = vecOfLayerSizes.at(layerNum - 1);
			Mat::VectorSize sizeOfThisLayer = vecOfLayerSizes.at(layerNum);
			denseLayerPointer->setMatrix(Mat::Matrix<NetNodeType>(Mat::MN(sizeOfThisLayer, sizeOfPreviousLayer)));
			denseLayerPointer->setBias(Mat::Vector<NetNodeType>(sizeOfThisLayer));
		}
		else
		{
			throw InvalidLayerPointerException("SequentialNet::compile(): Could not dynamic_cast layer to DenseLayer*!", this->mVecOfLayers.at(layerNum));
		}
	}

}


NetOutput SequentialNet::apply(NetInput const & input) const
{
	Mat::Vector<NetNodeType> vec = input;
	for (auto layerPointer : mVecOfLayers)
	{
		//vec = layerPointer->apply(std::move(vec));
		vec = layerPointer->apply(vec);
	}
	return vec;
}


void SequentialNet::mutate(Mutation const & mutation)
{

}

