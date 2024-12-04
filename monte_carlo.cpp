#include "monte_carlo.h"   // Include header for MonteCarlo class
#include "utils.h"         // Include utility functions
#include <random>          // For random number generation
#include <algorithm>       // For sorting losses
#include <iostream>        // For console output
#include "omp.h"           // For OpenMP parallelization
#include <tbb/tbb.h>       // Include TBB header


// Constructor: Initializes the simulation with given parameters
MonteCarlo::MonteCarlo(int num_trials, int num_assets, double confidence_level)
    : num_trials(num_trials), num_assets(num_assets), confidence_level(confidence_level) {
    generate_random_portfolio(); // Generate initial random portfolio data
}

// Generates random weights, returns, and volatilities for the portfolio
void MonteCarlo::generate_random_portfolio() {
    weights.resize(num_assets);
    returns.resize(num_assets);
    volatilities.resize(num_assets);

    // Generate random weights, returns, and volatilities
    for (int i = 0; i < num_assets; ++i) {
        weights[i] = Utils::generate_random(0.0, 1.0);     // Random weight
        returns[i] = Utils::generate_random(0.05, 0.02);   // Mean return around 5%
        volatilities[i] = Utils::generate_random(0.1, 0.05); // Volatility around 10%
    }

    Utils::normalize_weights(weights); // Normalize weights to sum to 1
}

// Runs the Monte Carlo simulation in parallel using OpenMP
void MonteCarlo::run_simulation(Mode mode) {
    losses.resize(num_trials); // Allocate space for losses

    if (mode == Mode::SEQUENTIAL) {
        // Sequential loop
        for (int i = 0; i < num_trials; ++i) {
            double loss = 0.0;
            for (int j = 0; j < num_assets; ++j) {
                double random_return = Utils::generate_random(returns[j], volatilities[j]);
                loss += weights[j] * random_return;
            }
            losses[i] = -loss;
        }
    } else if (mode == Mode::OPENMP) {
        // Parallel loop using OpenMP
        #pragma omp parallel for schedule(dynamic)
        for (int i = 0; i < num_trials; ++i) {
            double loss = 0.0;
            for (int j = 0; j < num_assets; ++j) {
                double random_return = Utils::generate_random(returns[j], volatilities[j]);
                loss += weights[j] * random_return;
            }
            losses[i] = -loss;
        }
    } else if (mode == Mode::TBB) {
        // Parallel loop using TBB
        tbb::parallel_for(0, num_trials, [&](int i) {
            double loss = 0.0;
            for (int j = 0; j < num_assets; ++j) {
                double random_return = Utils::generate_random(returns[j], volatilities[j]);
                loss += weights[j] * random_return;
            }
            losses[i] = -loss;
        });
    }
}


// Calculates the Value-at-Risk (VaR) from the simulated losses
double MonteCarlo::calculate_var() const {
    std::vector<double> sorted_losses = losses; // Copy losses for sorting
    std::sort(sorted_losses.begin(), sorted_losses.end()); // Sort losses

    // Calculate percentile index
    int index = static_cast<int>((1.0 - confidence_level) * num_trials);
    return sorted_losses[index]; // Return VaR at the confidence level
}

// Returns the simulated losses
const std::vector<double>& MonteCarlo::get_losses() const {
    return losses;
}
