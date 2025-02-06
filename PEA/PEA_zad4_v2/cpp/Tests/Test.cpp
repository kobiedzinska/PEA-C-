// Test.cpp
#include "Test.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <numeric>
#include <algorithm>
#include <filesystem>
#include <ctime>
#include <map>

void Test::runTests() {

    // Utwórz folder z timestampem dla wyników
    std::time_t now = std::time(nullptr);
    std::string timestamp = std::to_string(now);
    std::string resultsDir = "results_" + timestamp;
    std::filesystem::create_directory(resultsDir);

    // Mapa do przechowywania wszystkich podsumowań
    std::map<std::pair<std::string, int>, SummaryResult> all_summaries;

    for (const auto& instance : instances) {
        for (int pop_size : population_sizes) {
            std::cout << "\nTesting instance: " << instance
                     << " with population size: " << pop_size << std::endl;

            std::vector<RunResult> results;
            results.reserve(NUM_RUNS);

            // Wykonaj NUM_RUNS powtórzeń
            for (int i = 0; i < NUM_RUNS; i++) {
                std::cout << "Run " << (i + 1) << "/" << NUM_RUNS << std::endl;

                auto start = std::chrono::high_resolution_clock::now();

                // Utwórz i uruchom algorytm genetyczny
                genetic_tsp ga(instance, pop_size, 0.8, 0.01, "OX", "swap");
                int currentCost = ga.run();

                auto end = std::chrono::high_resolution_clock::now();
                double runTime = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

                // Zapisz wyniki
                RunResult result;
                result.evolution_data = ga.getEvolutionData();
                result.final_cost = currentCost;
                result.execution_time = runTime;
                result.best_path = ga.getBestPath();
                results.push_back(result);
            }

            // Zapisz szczegółowe wyniki dla każdego uruchomienia
            std::string instance_name = instance.substr(instance.find_last_of("/\\") + 1);
            saveDetailedResults(resultsDir, instance_name, pop_size, results);

            // Zapisz uśrednione wyniki
            saveAveragedResults(resultsDir, instance_name, pop_size, results);

            // Oblicz i zapisz podsumowanie
            all_summaries[{instance_name, pop_size}] = calculateSummary(instance, pop_size, results);
        }
    }

    // Zapisz ogólne podsumowanie
    saveSummary(resultsDir, all_summaries);
}

Test::SummaryResult Test::calculateSummary(const std::string& instance,
                                           int population_size,
                                           const std::vector<RunResult>& results) {
    SummaryResult summary;
    int optimal_cost = getOptimalCost(instance);
    summary.optimal_cost = optimal_cost;

    // Znajdź najlepszy koszt
    summary.best_cost = std::numeric_limits<double>::infinity();
    for (const auto& result : results) {
        summary.best_cost = std::min(summary.best_cost, (double)result.final_cost);
    }

    // Oblicz średni błąd końcowy
    double total_error = 0.0;
    double min_error = std::numeric_limits<double>::infinity();
    double max_error = 0.0;
    int success_count = 0;

    for (const auto& result : results) {
        double error = std::abs(result.final_cost - optimal_cost) / (double)optimal_cost;
        total_error += error;
        min_error = std::min(min_error, error);
        max_error = std::max(max_error, error);

        if (result.final_cost == optimal_cost) {
            success_count++;
        }
    }

    summary.avg_error = total_error / results.size();
    summary.min_error = min_error;
    summary.max_error = max_error;
    summary.success_rate = (double)success_count / results.size() * 100;

    // Oblicz średni czas wykonania
    double total_time = 0.0;
    for (const auto& result : results) {
        total_time += result.execution_time;
    }
    summary.avg_time = total_time / results.size();

    return summary;
}

void Test::saveSummary(const std::string& resultsDir,
                      const std::map<std::pair<std::string, int>, SummaryResult>& summaries) {
    std::string filename = resultsDir + "/summary.csv";
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filename << std::endl;
        return;
    }

    // Zapisz nagłówki
    file << "Instance,PopulationSize,AvgError,MinError,MaxError,AvgTime,BestCost,OptimalCost,SuccessRate\n";

    // Zapisz szczegółowe wyniki dla każdej kombinacji
    for (const auto& [key, summary] : summaries) {
        const auto& [instance, pop_size] = key;
        file << std::fixed << std::setprecision(4)
             << instance << ","
             << pop_size << ","
             << summary.avg_error << ","
             << summary.min_error << ","
             << summary.max_error << ","
             << summary.avg_time << ","
             << summary.best_cost << ","
             << summary.optimal_cost << ","
             << summary.success_rate << "\n";
    }

    // Oblicz i zapisz średnie dla każdej instancji
    file << "\nAverages per instance:\n";
    file << "Instance,AvgError,AvgTime,AvgSuccessRate\n";

    for (const auto& instance : instances) {
        std::string instance_name = instance.substr(instance.find_last_of("/\\") + 1);
        double total_error = 0.0;
        double total_time = 0.0;
        double total_success_rate = 0.0;
        int count = 0;

        for (int pop_size : population_sizes) {
            const auto& summary = summaries.at({instance_name, pop_size});
            total_error += summary.avg_error;
            total_time += summary.avg_time;
            total_success_rate += summary.success_rate;
            count++;
        }

        file << instance_name << ","
             << (total_error / count) << ","
             << (total_time / count) << ","
             << (total_success_rate / count) << "\n";
    }

    // Oblicz i zapisz średnie dla każdego rozmiaru populacji
    file << "\nAverages per population size:\n";
    file << "PopulationSize,AvgError,AvgTime,AvgSuccessRate\n";

    for (int pop_size : population_sizes) {
        double total_error = 0.0;
        double total_time = 0.0;
        double total_success_rate = 0.0;
        int count = 0;

        for (const auto& instance : instances) {
            std::string instance_name = instance.substr(instance.find_last_of("/\\") + 1);
            const auto& summary = summaries.at({instance_name, pop_size});
            total_error += summary.avg_error;
            total_time += summary.avg_time;
            total_success_rate += summary.success_rate;
            count++;
        }

        file << pop_size << ","
             << (total_error / count) << ","
             << (total_time / count) << ","
             << (total_success_rate / count) << "\n";
    }

    file.close();
    std::cout << "Summary saved to: " << filename << std::endl;
}

void Test::saveDetailedResults(const std::string& resultsDir,
                             const std::string& instance_name,
                             int population_size,
                             const std::vector<RunResult>& results) {
    std::string filename = resultsDir + "/detailed_" + instance_name + "_pop" +
                          std::to_string(population_size) + ".csv";
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filename << std::endl;
        return;
    }

    // Zapisz nagłówki
    file << "Run,Time,RelativeError,FinalCost,ExecutionTime,Path\n";

    // Zapisz dane dla każdego uruchomienia
    for (int i = 0; i < results.size(); i++) {
        for (const auto& [time, error, path] : results[i].evolution_data) {
            file << i + 1 << ","
                 << std::fixed << std::setprecision(2) << time << ","
                 << error << ","
                 << results[i].final_cost << ","
                 << results[i].execution_time << ",";

            // Dodaj ścieżkę
            for (size_t j = 0; j < results[i].best_path.size(); j++) {
                file << results[i].best_path[j];
                if (j < results[i].best_path.size() - 1) file << "-";
            }
            file << "\n";
        }
    }

    file.close();
    std::cout << "Detailed results saved to: " << filename << std::endl;
}


void Test::saveAveragedResults(const std::string& resultsDir,
                             const std::string& instance_name,
                             int population_size,
                             const std::vector<RunResult>& results) {
    std::string filename = resultsDir + "/averaged_" + instance_name + "_pop" +
                          std::to_string(population_size) + ".csv";
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filename << std::endl;
        return;
    }

    file << "Time,AverageError,MinError,MaxError,StdDev\n";

    // Znajdź maksymalny czas we wszystkich uruchomieniach
    double max_time = 0;
    for (const auto& run : results) {
        if (!run.evolution_data.empty()) {
            max_time = std::max(max_time, run.evolution_data.back().time);
        }
    }

    // Dla każdej sekundy zbierz i przeanalizuj wyniki
    for (double t = 0; t <= max_time; t += 1.0) {
        std::vector<double> errors;
        errors.reserve(results.size());

        // Zbierz błędy ze wszystkich uruchomień dla danego czasu
        for (const auto& run : results) {
            auto it = std::lower_bound(
                run.evolution_data.begin(),
                run.evolution_data.end(),
                t,
                [](const auto& a, double time) { return a.time < time; }
            );

            if (it != run.evolution_data.end()) {
                errors.push_back(it->error);
            }
        }

        if (!errors.empty()) {
            // Oblicz statystyki
            double avg_error = std::accumulate(errors.begin(), errors.end(), 0.0) / errors.size();
            double min_error = *std::min_element(errors.begin(), errors.end());
            double max_error = *std::max_element(errors.begin(), errors.end());

            // Oblicz odchylenie standardowe
            double sq_sum = std::accumulate(errors.begin(), errors.end(), 0.0,
                [avg_error](double sum, double error) {
                    double diff = error - avg_error;
                    return sum + diff * diff;
                });
            double std_dev = std::sqrt(sq_sum / errors.size());

            file << std::fixed << std::setprecision(4)
                 << t << ","
                 << avg_error << ","
                 << min_error << ","
                 << max_error << ","
                 << std_dev << "\n";
        }
    }

    file.close();
    std::cout << "Averaged results saved to: " << filename << std::endl;
}

int Test::getOptimalCost(const std::string& instance) {
    if (instance.find("ftv47") != std::string::npos) {
        return 1776;
    } else if (instance.find("ftv170") != std::string::npos) {
        return 2755;
    } else if (instance.find("rbg403") != std::string::npos) {
        return 2465;
    }
    return -1;
}
void Test::testMutationRates() {
    const int BEST_POP_SIZE = 100; // Ustaw najlepszy rozmiar populacji
    std::vector<double> mutation_rates = {0.01, 0.05, 0.10};
    const double CROSSOVER_RATE = 0.8;

    // Utwórz folder z timestampem dla wyników
    std::time_t now = std::time(nullptr);
    std::string timestamp = std::to_string(now);
    std::string resultsDir = "mutation_results_" + timestamp;
    std::filesystem::create_directory(resultsDir);

    // Mapa do przechowywania wszystkich podsumowań
    std::map<std::pair<std::string, double>, SummaryResult> all_summaries;

    for (const auto& instance : instances) {
        for (double mut_rate : mutation_rates) {
            std::cout << "\nTesting instance: " << instance
                     << " with mutation rate: " << mut_rate << std::endl;

            std::vector<RunResult> results;
            results.reserve(NUM_RUNS);

            // Wykonaj NUM_RUNS powtórzeń
            for (int i = 0; i < NUM_RUNS; i++) {
                std::cout << "Run " << (i + 1) << "/" << NUM_RUNS << std::endl;

                auto start = std::chrono::high_resolution_clock::now();

                // Utwórz i uruchom algorytm genetyczny
                genetic_tsp ga(instance, BEST_POP_SIZE, CROSSOVER_RATE, mut_rate, "OX", "swap");
                int currentCost = ga.run();

                auto end = std::chrono::high_resolution_clock::now();
                double runTime = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

                // Zapisz wyniki
                RunResult result;
                result.evolution_data = ga.getEvolutionData();
                result.final_cost = currentCost;
                result.execution_time = runTime;
                result.best_path = ga.getBestPath();
                results.push_back(result);
            }

            // Zapisz szczegółowe wyniki dla każdego uruchomienia
            std::string instance_name = instance.substr(instance.find_last_of("/\\") + 1);
            std::string mutation_str = std::to_string(mut_rate).substr(0, 4);
            saveDetailedResults(resultsDir, instance_name + "_mut" + mutation_str, BEST_POP_SIZE, results);

            // Zapisz uśrednione wyniki
            saveAveragedResults(resultsDir, instance_name + "_mut" + mutation_str, BEST_POP_SIZE, results);

            // Oblicz i zapisz podsumowanie
            all_summaries[{instance_name, mut_rate}] = calculateSummary(instance, BEST_POP_SIZE, results);
        }
    }

    // Zapisz ogólne podsumowanie
    saveMutationSummary(resultsDir, all_summaries, mutation_rates);
}

void Test::saveMutationSummary(const std::string& resultsDir,
                              const std::map<std::pair<std::string, double>, SummaryResult>& summaries,
                              const std::vector<double>& mutation_rates) {
    std::string filename = resultsDir + "/mutation_summary.csv";
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filename << std::endl;
        return;
    }

    file << "Instance,MutationRate,AvgError,MinError,MaxError,AvgTime,BestCost,OptimalCost,SuccessRate\n";

    for (const auto& [key, summary] : summaries) {
        const auto& [instance, mut_rate] = key;
        file << std::fixed << std::setprecision(4)
             << instance << ","
             << mut_rate << ","
             << summary.avg_error << ","
             << summary.min_error << ","
             << summary.max_error << ","
             << summary.avg_time << ","
             << summary.best_cost << ","
             << summary.optimal_cost << ","
             << summary.success_rate << "\n";
    }

    // Oblicz i zapisz średnie dla każdej instancji
    file << "\nAverages per instance:\n";
    file << "Instance,AvgError,AvgTime,AvgSuccessRate\n";

    for (const auto& instance : instances) {
        std::string instance_name = instance.substr(instance.find_last_of("/\\") + 1);
        double total_error = 0.0;
        double total_time = 0.0;
        double total_success_rate = 0.0;
        int count = 0;

        for (double mut_rate : mutation_rates) {
            const auto& summary = summaries.at({instance_name, mut_rate});
            total_error += summary.avg_error;
            total_time += summary.avg_time;
            total_success_rate += summary.success_rate;
            count++;
        }

        file << instance_name << ","
             << (total_error / count) << ","
             << (total_time / count) << ","
             << (total_success_rate / count) << "\n";
    }

    file << "\nAverages per mutation rate:\n";
    file << "MutationRate,AvgError,AvgTime,AvgSuccessRate\n";

    for (double mut_rate : mutation_rates) {
        double total_error = 0.0;
        double total_time = 0.0;
        double total_success_rate = 0.0;
        int count = 0;

        for (const auto& instance : instances) {
            std::string instance_name = instance.substr(instance.find_last_of("/\\") + 1);
            const auto& summary = summaries.at({instance_name, mut_rate});
            total_error += summary.avg_error;
            total_time += summary.avg_time;
            total_success_rate += summary.success_rate;
            count++;
        }

        file << mut_rate << ","
             << (total_error / count) << ","
             << (total_time / count) << ","
             << (total_success_rate / count) << "\n";
    }

    file.close();
    std::cout << "Mutation summary saved to: " << filename << std::endl;
}

void Test::testCrossoverRates() {
    const int BEST_POP_SIZE = 100;
    const double MUTATION_RATE = 0.01;
    std::vector<double> crossover_rates = {0.5, 0.7, 0.9};

    std::time_t now = std::time(nullptr);
    std::string timestamp = std::to_string(now);
    std::string resultsDir = "crossover_results_" + timestamp;
    std::filesystem::create_directory(resultsDir);

    std::map<std::pair<std::string, double>, SummaryResult> all_summaries;

    for (const auto& instance : instances) {
        for (double cross_rate : crossover_rates) {
            std::cout << "\nTesting instance: " << instance
                     << " with crossover rate: " << cross_rate << std::endl;

            std::vector<RunResult> results;
            results.reserve(NUM_RUNS);

            for (int i = 0; i < NUM_RUNS; i++) {
                std::cout << "Run " << (i + 1) << "/" << NUM_RUNS << std::endl;

                auto start = std::chrono::high_resolution_clock::now();
                genetic_tsp ga(instance, BEST_POP_SIZE, cross_rate, MUTATION_RATE, "OX", "swap");
                int currentCost = ga.run();
                auto end = std::chrono::high_resolution_clock::now();
                double runTime = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

                RunResult result;
                result.evolution_data = ga.getEvolutionData();
                result.final_cost = currentCost;
                result.execution_time = runTime;
                result.best_path = ga.getBestPath();
                results.push_back(result);
            }

            std::string instance_name = instance.substr(instance.find_last_of("/\\") + 1);
            std::string crossover_str = std::to_string(cross_rate).substr(0, 3);
            saveDetailedResults(resultsDir, instance_name + "_cross" + crossover_str, BEST_POP_SIZE, results);
            saveAveragedResults(resultsDir, instance_name + "_cross" + crossover_str, BEST_POP_SIZE, results);
            all_summaries[{instance_name, cross_rate}] = calculateSummary(instance, BEST_POP_SIZE, results);
        }
    }

    saveCrossoverSummary(resultsDir, all_summaries, crossover_rates);
}

void Test::saveCrossoverSummary(const std::string& resultsDir,
                               const std::map<std::pair<std::string, double>, SummaryResult>& summaries,
                               const std::vector<double>& crossover_rates) {
    std::string filename = resultsDir + "/crossover_summary.csv";
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filename << std::endl;
        return;
    }

    file << "Instance,CrossoverRate,AvgError,MinError,MaxError,AvgTime,BestCost,OptimalCost,SuccessRate\n";

    for (const auto& [key, summary] : summaries) {
        const auto& [instance, cross_rate] = key;
        file << std::fixed << std::setprecision(4)
             << instance << ","
             << cross_rate << ","
             << summary.avg_error << ","
             << summary.min_error << ","
             << summary.max_error << ","
             << summary.avg_time << ","
             << summary.best_cost << ","
             << summary.optimal_cost << ","
             << summary.success_rate << "\n";
    }

    file << "\nAverages per instance:\n";
    file << "Instance,AvgError,AvgTime,AvgSuccessRate\n";

    for (const auto& instance : instances) {
        std::string instance_name = instance.substr(instance.find_last_of("/\\") + 1);
        double total_error = 0.0, total_time = 0.0, total_success_rate = 0.0;
        int count = 0;

        for (double cross_rate : crossover_rates) {
            const auto& summary = summaries.at({instance_name, cross_rate});
            total_error += summary.avg_error;
            total_time += summary.avg_time;
            total_success_rate += summary.success_rate;
            count++;
        }

        file << instance_name << ","
             << (total_error / count) << ","
             << (total_time / count) << ","
             << (total_success_rate / count) << "\n";
    }

    file << "\nAverages per crossover rate:\n";
    file << "CrossoverRate,AvgError,AvgTime,AvgSuccessRate\n";

    for (double cross_rate : crossover_rates) {
        double total_error = 0.0, total_time = 0.0, total_success_rate = 0.0;
        int count = 0;

        for (const auto& instance : instances) {
            std::string instance_name = instance.substr(instance.find_last_of("/\\") + 1);
            const auto& summary = summaries.at({instance_name, cross_rate});
            total_error += summary.avg_error;
            total_time += summary.avg_time;
            total_success_rate += summary.success_rate;
            count++;
        }

        file << cross_rate << ","
             << (total_error / count) << ","
             << (total_time / count) << ","
             << (total_success_rate / count) << "\n";
    }

    file.close();
    std::cout << "Crossover summary saved to: " << filename << std::endl;
}