//
// Created by DN on 18.02.2017.
//

#pragma once

#include <vector>
#include <random>
#include <functional>
#include <Eigen/Dense>

using Eigen::MatrixXd;
using Eigen::VectorXd;

using Sample = std::tuple<VectorXd, VectorXd>;

class Network
{
private:
    std::vector<int> layers_;
    std::vector<MatrixXd> weights_;
    std::vector<VectorXd> biases_;

    std::function<double()> randomGenerator_;

public:
    Network() = delete;
    Network(std::function<double()> randomGenerator);
    Network(const Network& other) = delete;
    Network(Network&& other) = delete;
    Network& operator=(const Network& other) = delete;
    Network& operator=(Network&& other) = delete;
    ~Network() = default;

    /*
     * Add new layer with given number of neurons into the network.
     */
    void addLayer(int numNeurons);

    /*
     * Calculate output of the network for given input vector.
     */
    VectorXd getOutput(const VectorXd& input) const;

    /*
     *
     */
    std::tuple<std::vector<VectorXd>, std::vector<MatrixXd>> backpropagateOne(const VectorXd& input, const VectorXd& desiredOutput);

    /*
     *
     */
    void applyMinibatch(std::vector<Sample>::const_iterator begin,
                        std::vector<Sample>::const_iterator end,
                        double learningRate);
};