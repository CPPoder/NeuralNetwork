#include "Source\NeuralNet\SequentialNet.hpp"
#include "Source\NeuralNet\DenseLayer.hpp"
#include "Source\Math\Random.hpp"


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


SequentialNet& SequentialNet::operator=(SequentialNet const & sequentialNet)
{
	//Be careful here! Make a deep copy of layers and destroy old ones!
	//Beacause there are only denseLayers, create only those!

	//Avoid self assignment
	if (this == &sequentialNet)
	{
		return *this;
	}

	//Delete old layers
	for (auto layerPointer : this->mVecOfLayers)
	{
		delete layerPointer;
	}
	this->mVecOfLayers.clear();
	
	//Copy
	mInputLayerSize = sequentialNet.mInputLayerSize;
	for (auto layerPointer : sequentialNet.mVecOfLayers)
	{
		DenseLayer const * denseLayerPointer = dynamic_cast<DenseLayer const *>(layerPointer); //Only possible, because this the the only inherited type
		if (denseLayerPointer != nullptr)
		{
			mVecOfLayers.push_back(new DenseLayer(*denseLayerPointer));
		}
		else
		{
			throw InvalidLayerPointerException("SequentialNet::addLayer(Layer const & layer): Could not dynamic_cast layer to DenseLayer const *!", layerPointer);
		}
	}

	//Return
	return *this;
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
	for (auto layer : mVecOfLayers)
	{
		layer->mutate();
	}
}



void SequentialNet::setMatrixOfLayer(unsigned int layer, Mat::Matrix<NetNodeType> const & matrix)
{
	Layer* layerPointer = mVecOfLayers.at(layer);
	DenseLayer* denseLayerPointer = dynamic_cast<DenseLayer*>(layerPointer);
	if (denseLayerPointer == nullptr)
	{
		throw "I can not program!";
	}
	denseLayerPointer->setMatrix(matrix);
}

void SequentialNet::setBiasOfLayer(unsigned int layer, Mat::Vector<NetNodeType> const & bias)
{
	Layer* layerPointer = mVecOfLayers.at(layer);
	DenseLayer* denseLayerPointer = dynamic_cast<DenseLayer*>(layerPointer);
	if (denseLayerPointer == nullptr)
	{
		throw "I can not program!";
	}
	denseLayerPointer->setBias(bias);
}

void SequentialNet::setActivationOfLayer(unsigned int layer, Activation activation)
{
	Layer* layerPointer = mVecOfLayers.at(layer);
	DenseLayer* denseLayerPointer = dynamic_cast<DenseLayer*>(layerPointer);
	if (denseLayerPointer == nullptr)
	{
		throw "I can not program!";
	}
	denseLayerPointer->setActivation(activation);
}



void SequentialNet::saveToFile(std::string const & path) const
{
	std::cout << "Save SequentialNet to file \"" << path << "\"...";
	std::ofstream oStream(path);

	//Net type
	oStream << "<NetType:Begin>" << std::endl;
	oStream << "Sequential" << std::endl;
	oStream << "<NetType:End>" << std::endl;
	
	//Layers
	oStream << "<Layers:Begin>" << std::endl;
	for (auto const layerPointer : mVecOfLayers)
	{
		DenseLayer const * denseLayerPointer = dynamic_cast<DenseLayer const *>(layerPointer);

		oStream << "<Layer:Begin>" << std::endl;
		
		//Layer type
		oStream << "<LayerType:Begin>" << std::endl;
		oStream << "Dense" << std::endl;
		oStream << "<LayerType:End>" << std::endl;

		//Matrix
		oStream << "<Matrix:Begin>" << std::endl;
		Mat::Matrix<NetNodeType> matrix = denseLayerPointer->getMatrix();
		for (unsigned int y = 0u; y < matrix.getSize().y(); ++y)
		{
			for (unsigned int x = 0u; x < matrix.getSize().x(); ++x)
			{
				if (x != 0u)
				{
					oStream << " ";
				}
				oStream << matrix.at(Mat::XY(x, y));
			}
			oStream << std::endl;
		}
		oStream << "<Matrix:End>" << std::endl;

		//Bias
		oStream << "<Bias:Begin>" << std::endl;
		Mat::Vector<NetNodeType> bias = denseLayerPointer->getBias();
		for (unsigned int i = 0u; i < bias.getSize(); ++i)
		{
			if (i != 0u)
			{
				oStream << " ";
			}
			oStream << bias.at(i);
		}
		oStream << std::endl;
		oStream << "<Bias:End>" << std::endl;

		//Activation
		oStream << "<Activation:Begin>" << std::endl;
		oStream << Activation::mapTypeToString(denseLayerPointer->getActivation().getType()) << std::endl;
		oStream << "<Activation:End>" << std::endl;

		oStream << "<Layer:End>" << std::endl;
	}
	oStream << "<Layers:End>" << std::endl;

	oStream.close();
	std::cout << "Complete!" << std::endl;
}



void SequentialNet::loadFromFile(std::string const & path)
{
	//Open input file stream
	std::cout << "Load SequentialNet from file \"" << path << "\"...";
	std::ifstream inputStream(path);
	if (!inputStream)
	{
		std::cout << "Could not open file!" << std::endl;
		return;
	}

	//Extract lines
	std::vector<std::string> vecOfLines;
	while (!inputStream.eof())
	{
		std::string line;
		std::getline(inputStream, line);
		vecOfLines.push_back(line);
	}

	//Define function for extraction of blocks
	std::function<std::vector<std::vector<std::string>>(std::vector<std::string> const &, std::string const &)> getLinesBetween = 
		[](std::vector<std::string> const & vecOfLines, std::string const & keyword) -> std::vector<std::vector<std::string>>
	{
		std::vector<std::vector<std::string>> vecOfBlocks;
		std::vector<std::string> vecOfStrings;
		bool inside = false;
		for (auto const & line : vecOfLines)
		{
			if (line == ("<" + keyword + ":Begin>"))
			{
				if (!inside)
				{
					inside = true;
					continue;
				}
			}
			else if (line == ("<" + keyword + ":End>"))
			{
				if (inside)
				{
					inside = false;
					vecOfBlocks.push_back(vecOfStrings);
					vecOfStrings.clear();
					continue;
				}
			}
			else if (inside)
			{
				vecOfStrings.push_back(line);
			}
		}
		return vecOfBlocks;
	};

	//NetType
	std::vector<std::vector<std::string>> netTypeBlocks = getLinesBetween(vecOfLines, "NetType");
	if (netTypeBlocks.size() != 1)
	{
		std::cout << "Could not identify net type!" << std::endl;
		return;
	}
	if (netTypeBlocks.front().size() != 1)
	{
		std::cout << "Could not identify net type!" << std::endl;
		return;
	}
	std::string netTypeString = netTypeBlocks.front().front();
	bool netTypeIsSequential = false;
	if (netTypeString == "Sequential")
	{
		netTypeIsSequential = true;
	}
	else
	{
		std::cout << "Could not identify net type!" << std::endl;
		return;
	}

	//LayersBlock
	std::vector<std::vector<std::string>> layersBlocks = getLinesBetween(vecOfLines, "Layers");
	if (layersBlocks.size() != 1)
	{
		std::cout << "Could not identify layers!" << std::endl;
		return;
	}
	std::vector<std::string> layersBlock = layersBlocks.front();

	//LayerBlocks
	std::vector<std::vector<std::string>> layerBlocks = getLinesBetween(layersBlock, "Layer");
	if (layerBlocks.empty())
	{
		std::cout << "Could not identify layers!" << std::endl;
		return;
	}

	//Extract layers from layerBlocks
	std::vector<Mat::Matrix<NetNodeType>> vecOfMatrices;
	std::vector<Mat::Vector<NetNodeType>> vecOfBiases;
	std::vector<std::string> vecOfActivations;
	for (auto const & layerBlock : layerBlocks)
	{
		//Extract matrix
		std::vector<std::vector<std::string>> matrixBlocks = getLinesBetween(layerBlock, "Matrix");
		if (matrixBlocks.size() != 1)
		{
			std::cout << "Could not identify matrix!" << std::endl;
			return;
		}
		std::vector<std::string> matrixBlock = matrixBlocks.front();
		std::vector<std::vector<NetNodeType>> vecOfMatrixRows;
		for (auto const & line : matrixBlock)
		{
			std::stringstream sstream(line);
			std::vector<NetNodeType> rowVec;
			while (!sstream.eof())
			{
				float f;
				sstream >> f;
				rowVec.push_back(f);
			}
			vecOfMatrixRows.push_back(rowVec);
		}
		Mat::Matrix<NetNodeType> matrix(vecOfMatrixRows);
		vecOfMatrices.push_back(matrix);

		//Extract bias
		std::vector<std::vector<std::string>> biasBlocks = getLinesBetween(layerBlock, "Bias");
		if (biasBlocks.size() != 1)
		{
			std::cout << "Could not identify bias!" << std::endl;
			return;
		}
		std::vector<std::string> biasBlock = biasBlocks.front();
		if (biasBlock.size() != 1)
		{
			std::cout << "Could not identify bias!" << std::endl;
		}
		std::string biasString = biasBlock.front();
		std::vector<NetNodeType> vecOfBiasEntries;
		std::stringstream sstream(biasString);
		while (!sstream.eof())
		{
			float f;
			sstream >> f;
			vecOfBiasEntries.push_back(f);
		}
		Mat::Vector<NetNodeType> bias(vecOfBiasEntries);
		vecOfBiases.push_back(bias);

		//Extract activation
		std::vector<std::vector<std::string>> activationBlocks = getLinesBetween(layerBlock, "Activation");
		if (activationBlocks.size() != 1)
		{
			std::cout << "Could not identify activation!" << std::endl;
			return;
		}
		std::vector<std::string> activationBlock = activationBlocks.front();
		if (activationBlock.size() != 1)
		{
			std::cout << "Could not identify activation!" << std::endl;
			return;
		}
		std::string activationString = activationBlock.front();
		vecOfActivations.push_back(activationString);

	}

	//Build net
	SequentialNet newNet(0);
	for (auto const & matrix : vecOfMatrices)
	{
		newNet.addLayer(DenseLayer(0, Activation::Atan));
	}
	for (unsigned int i = 0; i < vecOfMatrices.size(); ++i)
	{
		newNet.setMatrixOfLayer(i, vecOfMatrices.at(i));
		newNet.setBiasOfLayer(i, vecOfBiases.at(i));
		newNet.setActivationOfLayer(i, Activation::getActivationOfType(Activation::mapStringToType(vecOfActivations.at(i))));
	}
	*this = newNet;


	//Clean up
	inputStream.close();
	std::cout << "Complete!" << std::endl;
}






