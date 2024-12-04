#ifndef MONTE_CARLO_H
#define MONTE_CARLO_H

#include <vector> // For storing portfolio data and losses

// Monte Carlo Simulation for Portfolio Risk Analysis
class MonteCarlo {
public:
    enum class Mode {SEQUENTIAL, OPENMP, TBB}; // Enumeration for simulation modes

    MonteCarlo(int num_trials, int num_assets, double confidence_level); // Constructor

    void run_simulation(Mode mode);         // Runs the Monte Carlo simulation
    double calculate_var() const;  // Calculates the Value-at-Risk (VaR)
    const std::vector<double>& get_losses() const; // Returns the simulated losses
    
private:
    int num_trials;                // Number of Monte Carlo trials
    int num_assets;                // Number of assets in the portfolio
    double confidence_level;       // Confidence level for VaR calculation (e.g., 95%)
    std::vector<double> weights;   // Portfolio asset weights
    std::vector<double> returns;   // Expected returns for each asset
    std::vector<double> volatilities; // Volatilities for each asset
    std::vector<double> losses;    // Simulated portfolio losses

    void generate_random_portfolio(); // Generates random portfolio data
};

#endif
