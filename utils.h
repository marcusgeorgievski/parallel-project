#ifndef UTILS_H
#define UTILS_H

#include <vector>   // For storing and manipulating data
#include <string>   // For string formatting

// Utility functions for the Monte Carlo simulation
namespace Utils {
    double generate_random(double mean, double stddev); // Generates a random number from a normal distribution
    void normalize_weights(std::vector<double>& weights); // Normalizes weights to sum to 1
    double calculate_percentile(const std::vector<double>& data, double percentile); // Calculates a percentile value
    double measure_execution_time(void (*func)()); // Measures the execution time of a function
}

#endif
