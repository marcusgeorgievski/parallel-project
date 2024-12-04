#include "utils.h"       // Include utility function headers
#include <random>        // For random number generation
#include <numeric>       // For accumulate function
#include <chrono>        // For measuring execution time

// Generates a random number from a normal distribution
double Utils::generate_random(double mean, double stddev) {
    static thread_local std::mt19937 generator(std::random_device{}()); // Thread-local RNG
    std::normal_distribution<double> distribution(mean, stddev); // Normal distribution
    return distribution(generator); // Generate and return random number
}

// Normalizes a vector of weights to ensure they sum to 1
void Utils::normalize_weights(std::vector<double>& weights) {
    double sum = std::accumulate(weights.begin(), weights.end(), 0.0); // Calculate sum
    for (auto& weight : weights) {
        weight /= sum; // Normalize each weight
    }
}

// Calculates the percentile value from a sorted dataset
double Utils::calculate_percentile(const std::vector<double>& data, double percentile) {
    int index = static_cast<int>(percentile * data.size()); // Calculate index
    return data[index]; // Return the value at the percentile
}

// Measures the execution time of a function (lambda or function pointer)
double Utils::measure_execution_time(void (*func)()) {
    auto start = std::chrono::high_resolution_clock::now(); // Start timer
    func(); // Execute function
    auto end = std::chrono::high_resolution_clock::now(); // End timer

    // Return duration in milliseconds
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}
