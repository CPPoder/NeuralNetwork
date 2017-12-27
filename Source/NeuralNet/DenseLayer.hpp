#ifndef DENSELAYER_HPP
#define DENSELAYER_HPP

#include "Source\NeuralNet\SequentialNet.hpp"
#include "Source\NeuralNet\Layer.hpp"
#include "Source\NeuralNet\Activation.hpp"


class DenseLayer final : public Layer
{
private: //Members that are initialized at construction
	Mat::VectorSize mLayerSize;
	Activation mActivation;

private: //Members that are initialized at compilation
	Mat::Matrix<NetNodeType> mMatrix;
	Mat::Vector<NetNodeType> mBias;

public:
	DenseLayer(Mat::VectorSize const & layerSize, Activation activation);
	virtual ~DenseLayer() override = default;

public:
	virtual Mat::Vector<NetNodeType> apply(Mat::Vector<NetNodeType> const & in) const override;
	virtual Mat::Vector<NetNodeType>&& apply(Mat::Vector<NetNodeType> && in) const override;

	virtual void mutate() override;

public:
	Mat::Matrix<NetNodeType> const & getMatrix() const;
	void setMatrix(Mat::Matrix<NetNodeType> const & matrix);
	void setMatrix(Mat::Matrix<NetNodeType> && matrix);

	Mat::Vector<NetNodeType> const & getBias() const;
	void setBias(Mat::Vector<NetNodeType> const & vector);
	void setBias(Mat::Vector<NetNodeType> && vector);

	Activation getActivation() const;
	void setActivation(Activation activation);

	void setEntriesRandom();

	virtual Mat::VectorSize getLayerSize() const override;


};


#endif //DENSELAYER_HPP

