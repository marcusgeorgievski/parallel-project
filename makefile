# Parallel version
pv: main.cpp monte_carlo.cpp utils.cpp
	/opt/homebrew/bin/g++-14 -std=c++11  -fopenmp -Wall  main.cpp monte_carlo.cpp utils.cpp -I/opt/homebrew/Cellar/tbb/2022.0.0/include -L/opt/homebrew/Cellar/tbb/2022.0.0/lib -ltbb -o parallel_version && ./parallel_version && rm parallel_version