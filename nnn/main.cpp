#include <iostream>
#include <Eigen/Eigen>
#include <chrono>
#include "Network.hpp"

using Eigen::MatrixXd;

const int numEpochs = 1000;
const int batchSize = 100;
const double learningSpeed = 0.1;

std::vector<Sample> loadSamples()
{
    std::vector<Sample> samples;

    FILE* fa = fopen("X.csv", "r");
    FILE* fb = fopen("y.csv", "r");

    for (int i = 0; i < 5000; ++i)
    {
        VectorXd input{400};

        for (int j = 0; j < 400; ++j)
        {
            double a;
            fscanf(fa, "%lf", &a);
            input(j) = a;
        }

		VectorXd output = VectorXd::Zero(10);

        int b;
        fscanf(fb, "%d", &b);
        if (b == 10)
            b = 0;
        output(b) = 1.0;

        samples.emplace_back(std::make_tuple(input, output));
    }

    fclose(fa);
    fclose(fb);

    return samples;
}

int main()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);

    std::cout << "Creating network...\n";
    Network network{[&dis, &gen]() { return dis(gen); }};
    network.addLayer(400);
    network.addLayer(25);
    network.addLayer(10);

    std::cout << "Loading samples...\n";
    auto samples = loadSamples();

    std::cout << "Shuffling and splitting samples...\n";
    std::shuffle(std::begin(samples), std::end(samples), gen);

    const auto numTrainSamples = samples.size() * 4 / 5;
    const auto numTestSamples = samples.size() - numTrainSamples;

    std::cout << "Got " << numTrainSamples << " training samples and " << numTestSamples << " test samples\n";
    std::vector<Sample> trainSet{std::begin(samples), std::begin(samples) + numTrainSamples};
    std::vector<Sample> testSet{std::begin(samples) + numTrainSamples, std::end(samples)};

    std::cout << "Started training...\n";
    for (int i = 0; i < numEpochs; ++i)
    {
        printf("Epoch: %d/%d ", i + 1, numEpochs);
        std::shuffle(std::begin(trainSet), std::end(trainSet), gen);

        for (auto it = std::begin(trainSet); it < std::end(trainSet); it += batchSize)
        {
			network.applyMinibatch(it, it + batchSize, learningSpeed);
        }

        const int numInvalid = (int) std::count_if(std::begin(testSet), std::end(testSet), [&network](const Sample& sample)
        {
            const auto& input = std::get<0>(sample);
            const auto& expectedOutput = std::get<1>(sample);

            const auto actualOutput = network.getOutput(input);

            int idx1;
            actualOutput.maxCoeff(&idx1);

            int idx2;
            expectedOutput.maxCoeff(&idx2);

            return idx1 != idx2;
        });

        printf("@ Error: %d/%d [%0.2f%%]\n", numInvalid, (int)testSet.size(), 100.0 * numInvalid / testSet.size());
    }

    return 0;
}