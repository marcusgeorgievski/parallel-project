#include "monte_carlo.h"
#include <iostream>
#include <chrono>
#include <tbb/tbb.h>       // Include TBB header
#include "omp.h"       // Include TBB header


double run_simulation_with_mode(MonteCarlo::Mode mode, int num_trials, int num_assets, double confidence_level) {
    MonteCarlo mc(num_trials, num_assets, confidence_level);

    auto start_time = std::chrono::high_resolution_clock::now();
    mc.run_simulation(mode); // Pass mode as parameter
    auto end_time = std::chrono::high_resolution_clock::now();

    double runtime = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
    double var = mc.calculate_var();

    // Print results based on mode
    std::string mode_name = (mode == MonteCarlo::Mode::SEQUENTIAL) ? "Sequential" :
                            (mode == MonteCarlo::Mode::OPENMP) ? "OpenMP" : "TBB";

    std::cout << mode_name << " Simulation Results:\n";
    std::cout << "  Value-at-Risk (VaR): " << var << "\n";
    std::cout << "  Runtime: " << runtime << " ms\n\n";

    return runtime;
}

int main() {
    // Simulation parameters
    const std::vector<int> trial_sizes = {5000000}; // Different trial sizes
    const int num_assets = 10;       // Number of assets
    const double confidence_level = 0.95; // 95% confidence level

    std::cout << "Monte Carlo Simulation for Portfolio Risk Analysis\n";
    std::cout << "Number of Assets: " << num_assets << "\n";
    std::cout << "Confidence Level: " << confidence_level * 100 << "%\n\n";

    // Loop through each trial size
    for (int num_trials : trial_sizes) {
        std::cout << "-----------------------------------\n";
        std::cout << "Number of Trials: " << num_trials << "\n";

        // Run simulations with all modes
        double sequential_time = run_simulation_with_mode(MonteCarlo::Mode::SEQUENTIAL, num_trials, num_assets, confidence_level);
        double openmp_time = run_simulation_with_mode(MonteCarlo::Mode::OPENMP, num_trials, num_assets, confidence_level);
        double tbb_time = run_simulation_with_mode(MonteCarlo::Mode::TBB, num_trials, num_assets, confidence_level);

        // Efficiency
        double efficiency_openmp = (sequential_time / openmp_time) / omp_get_max_threads();
        double efficiency_tbb = (sequential_time / tbb_time) / tbb::this_task_arena::max_concurrency();

        // Compare performance
        std::cout << "Performance Comparison:\n";
        std::cout << "  Runtime:" << std::endl;
        std::cout << "    Sequential Runtime: " << sequential_time << " ms\n";
        std::cout << "    OpenMP Runtime: " << openmp_time << " ms\n";
        std::cout << "    TBB Runtime: " << tbb_time << " ms\n";
        std::cout << "  Speedup:" << std::endl;
        std::cout << "    OpenMP Speedup: " << sequential_time / openmp_time << "x\n";
        std::cout << "    TBB Speedup: " << sequential_time / tbb_time << "x\n";
        std::cout << "  Efficiency:" << std::endl;
        std::cout << "    OpenMP: " << efficiency_openmp * 100 << "%\n";
        std::cout << "    TBB: " << efficiency_tbb * 100 << "%\n";
        std::cout << "-----------------------------------\n\n";
    }

    return 0;
}
