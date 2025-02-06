// Test.h
#ifndef TEST_H
#define TEST_H

#include "../Genetic/genetic_tsp.h"
#include <string>
#include <vector>
#include <map>

class Test {
private:
    // Pliki testowe
    std::vector<std::string> instances = {
        "ftv47.xml",
        "ftv170.xml",
        "rbg403.xml"
    };

    // Rozmiary populacji do przetestowania
    std::vector<int> population_sizes = {50, 100, 200};

    // Liczba powtórzeń dla każdej instancji
    const int NUM_RUNS = 10;

    // Struktura do przechowywania wyników pojedynczego uruchomienia
    struct RunResult {
        std::vector<genetic_tsp::Result> evolution_data; // (czas, błąd względny)
        int final_cost;                                        // końcowy koszt ścieżki
        double execution_time;                                 // czas wykonania w sekundach
        std::vector<int> best_path;
    };

    // Struktura do przechowywania podsumowania wyników
    struct SummaryResult {
        double avg_error;        // średni błąd
        double min_error;        // minimalny błąd
        double max_error;        // maksymalny błąd
        double avg_time;         // średni czas wykonania
        double best_cost;        // najlepszy znaleziony koszt
        int optimal_cost;        // optymalny koszt dla instancji
        double success_rate;     // procent uruchomień, które znalazły optymalne rozwiązanie
    };

    // Zapisuje wyniki do pliku CSV dla pojedynczego uruchomienia
    void saveDetailedResults(const std::string& resultsDir,
                           const std::string& instance_name,
                           int population_size,
                           const std::vector<RunResult>& results);

    // Oblicza i zapisuje uśrednione wyniki dla wszystkich uruchomień
    void saveAveragedResults(const std::string& resultsDir,
                           const std::string& instance_name,
                           int population_size,
                           const std::vector<RunResult>& results);

    // Oblicza podsumowanie dla danej instancji i rozmiaru populacji
    SummaryResult calculateSummary(const std::string& instance,
                                 int population_size,
                                 const std::vector<RunResult>& results);

    // Zapisuje ogólne podsumowanie wszystkich testów
    void saveSummary(const std::string& resultsDir,
                    const std::map<std::pair<std::string, int>, SummaryResult>& summaries);

    // Zwraca optymalny koszt dla danej instancji
    int getOptimalCost(const std::string& instance);

public:
    // Główna metoda testująca
    void runTests();

    // Testowanie współczynników mutacji
    void testMutationRates();
    void saveMutationSummary(const std::string& resultsDir,
                            const std::map<std::pair<std::string, double>, SummaryResult>& summaries,
                            const std::vector<double>& mutation_rates);
    void testCrossoverRates();
    void saveCrossoverSummary(const std::string& resultsDir,
                             const std::map<std::pair<std::string, double>, SummaryResult>& summaries,
                             const std::vector<double>& crossover_rates);
};
#endif