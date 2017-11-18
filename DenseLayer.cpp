#include "DenseLayer.hpp"


DenseLayer::DenseLayer(Mat::VectorSize const & layerSize, Activation activation)
	: mLayerSize(layerSize), mActivation(activation)
{
}







Mat::Vector<NetNodeType> DenseLayer::apply(Mat::Vector<NetNodeType> const & in) const
{
	return mActivation(mMatrix * in + mBias);
}


Mat::Vector<NetNodeType>&& DenseLayer::apply(Mat::Vector<NetNodeType> && in) const
{
	return std::move(mActivation(mMatrix * in + mBias));
}







Mat::Matrix<NetNodeType> const & DenseLayer::getMatrix() const
{
	return mMatrix;
}
void DenseLayer::setMatrix(Mat::Matrix<NetNodeType> const & matrix)
{
	mMatrix = matrix;
}
void DenseLayer::setMatrix(Mat::Matrix<NetNodeType> && matrix)
{
	mMatrix = std::move(matrix);
}



Mat::Vector<NetNodeType> const & DenseLayer::getBias() const
{
	return mBias;
}
void DenseLayer::setBias(Mat::Vector<NetNodeType> const & vector)
{
	mBias = vector;
}
void DenseLayer::setBias(Mat::Vector<NetNodeType> && vector)
{
	mBias = std::move(vector);
}



Activation DenseLayer::getActivation() const
{
	return mActivation;
}
Mat::VectorSize DenseLayer::getLayerSize() const
{
	return mLayerSize;
}

