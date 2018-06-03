#include <iostream>
#include "Network.hpp"

void Network::addLayer(int numNeurons)
{
    if (layers_.size() > 0)
    {
        MatrixXd weights{numNeurons, layers_.back()};
        VectorXd biases{numNeurons};

        for (int i = 0; i < weights.size(); ++i)
            weights(i) = randomGenerator_();

        for (int i = 0; i < biases.size(); ++i)
            biases(i) = randomGenerator_();

        weights_.emplace_back(std::move(weights));
        biases_.emplace_back(std::move(biases));
    }

    layers_.emplace_back(numNeurons);
}

inline double sigmoid(double x)
{
    return 1.0 / (1.0 + exp(-x));
}

inline double sigmoidPrime(double x)
{
    return sigmoid(x) * (1.0 - sigmoid(x));
}

VectorXd Network::getOutput(const VectorXd& input) const
{
    VectorXd layerOutput = input;
    for (int i = 0; i < layers_.size() - 1; ++i)
    {
        layerOutput = weights_[i] * layerOutput + biases_[i];
        layerOutput = layerOutput.unaryExpr(&sigmoid);
    }
    return layerOutput;
}

Network::Network(std::function<double()> randomGenerator) : randomGenerator_{randomGenerator} 
{

}

std::tuple<std::vector<VectorXd>, std::vector<MatrixXd>> Network::backpropagateOne(const VectorXd& input, const VectorXd& desiredOutput)
{
	const auto numLayers = layers_.size();

	std::vector<VectorXd> layerOutputs{ numLayers - 1 };
	std::vector<VectorXd> weightedInputs{ numLayers };
    weightedInputs[0] = input;

    // Feedforward
    VectorXd layerOutput = input;
    for (int i = 0; i < numLayers - 1; ++i)
    {
        VectorXd weightedInput = weights_[i] * layerOutput + biases_[i];
        weightedInputs[i + 1] = weightedInput;

        layerOutput = weightedInput.unaryExpr(&sigmoid);
        layerOutputs[i] = layerOutput;
    }

	std::vector<VectorXd> biasesGrad{ numLayers - 1 };
	std::vector<MatrixXd> weightsGrad{ numLayers - 1 };

    // Backpropagation
    VectorXd delta = layerOutputs.back() - desiredOutput;
	
    biasesGrad[numLayers - 2] = delta;
    weightsGrad[numLayers - 2] = delta * weightedInputs[numLayers - 2].transpose();

    for (int i = (int)numLayers - 3; i >= 0; --i)
    {
        const VectorXd a = weights_[i + 1].transpose() * delta;
        const VectorXd b = layerOutputs[i].unaryExpr(&sigmoidPrime);

        delta = a.cwiseProduct(b);

        biasesGrad[i] = delta;
        weightsGrad[i] = delta * weightedInputs[i].transpose();
    }

    return std::make_tuple(biasesGrad, weightsGrad);
}

void Network::applyMinibatch(std::vector<Sample>::const_iterator begin,
                             std::vector<Sample>::const_iterator end,
                             double learningRate)
{
    const auto numSamples = end - begin;
	const double alpha = learningRate / numSamples;

    std::vector<VectorXd> biasesGrad;
    for (const auto& biases : biases_)
        biasesGrad.emplace_back(VectorXd::Zero(biases.rows()));

    std::vector<MatrixXd> weightsGrad;
    for (const auto& weights : weights_)
        weightsGrad.emplace_back(MatrixXd::Zero(weights.rows(), weights.cols()));

    for (auto it = begin; it != end; ++it)
    {
		const auto& sample = *it;
        const auto& grad = backpropagateOne(std::get<0>(sample), std::get<1>(sample));
		
        for (int j = 0; j < layers_.size() - 1; ++j)
        {
			//const auto a = std::get<0>(grad)[j];
			//if (a.norm() > 100000)
			//{
			//	//std::cout << std::get<0>(*it) << "\n";
			//	std::cout << std::get<1>(*it) << "\n";
			//}
			//
            biasesGrad[j] += std::get<0>(grad)[j];
            weightsGrad[j] += std::get<1>(grad)[j];
        }
    }

    // Perform update of parameters
    for (int j = 0; j < layers_.size() - 1; ++j)
    {
        weights_[j] -= alpha * weightsGrad[j];
        biases_[j] -= alpha * biasesGrad[j];
    }
}
