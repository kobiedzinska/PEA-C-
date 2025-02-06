

import sys
import os

def add_dll_directory():
    # Dodaj ścieżkę do bibliotek MinGW
    mingw_path = "C:\\Program Files\\JetBrains\\CLion 2024.2.2\\bin\\mingw\\bin"
    if os.path.exists(mingw_path):
        os.add_dll_directory(mingw_path)

    # Dodaj ścieżkę do bibliotek Pythona
    python_path = os.path.dirname(sys.executable)
    if os.path.exists(python_path):
        os.add_dll_directory(python_path)

    # Dodaj ścieżkę do folderu build
    build_path = os.path.abspath(os.path.join(os.path.dirname(__file__), '../cmake-build-debug'))
    if os.path.exists(build_path):
        os.add_dll_directory(build_path)

# Najpierw dodaj ścieżki DLL
add_dll_directory()

# Dodaj ścieżkę do modułu
module_path = os.path.join(os.path.dirname(__file__), '../cmake-build-debug')
print(f"Szukam modułu w: {module_path}")
sys.path.append(module_path)

print(f"Zawartość folderu {module_path}:")
for file in os.listdir(module_path):
    if file.startswith('genetic_tsp_cpp'):
        print(f"Znaleziono moduł: {file}")

try:
    from genetic_tsp_cpp import GeneticTSP, TSPResult
    print("Moduł został pomyślnie załadowany")
except ImportError as e:
    print(f"Błąd importowania modułu: {e}")
    # Wyświetl więcej informacji o błędzie
    import traceback
    traceback.print_exc()


import mlflow

import numpy as np
from hyperopt import fmin, tpe, hp, STATUS_OK, Trials

from genetic_tsp_cpp import GeneticTSP, TSPResult
import matplotlib.pyplot as plt

class TSPExperiment:
    def __init__(self, xml_path: str):
        """
        Inicjalizacja eksperymentu
        xml_path: ścieżka do pliku XML z danymi
        """
        self.xml_path = xml_path

    def objective(self, params):
        with mlflow.start_run(nested=True):
            # Convert to absolute path and normalize slashes


            ga = GeneticTSP(
                self.xml_path,  # Dodaj ścieżkę do pliku XML jako pierwszy argument
                int(params['population_size']),
                params['crossover_rate'],
                params['mutation_rate'],
                params['crossover_method'],
                params['mutation_method']
            )

            # Uruchomienie algorytmu
            result = ga.run()

            # Logowanie metryk
            mlflow.log_metric("final_distance", result.final_distance)

            # Generowanie i zapisywanie wykresu
            self._plot_evolution(
                result.evolution_data,
                f"Evolution with params: {params}",
                "evolution_plot.png"
            )
            mlflow.log_artifact("evolution_plot.png")

            return {
                'loss': result.final_distance,  # używamy dystansu jako loss
                'status': STATUS_OK
            }

def run_experiments():

    space = {
        'population_size': hp.choice('population_size', [50, 100, 200]),
        'crossover_rate': hp.uniform('crossover_rate', 0.5, 0.9),
        'mutation_rate': hp.uniform('mutation_rate', 0.01, 0.1),
        'crossover_method': hp.choice('crossover_method', ['OX', 'PMX']),
        'mutation_method': hp.choice('mutation_method', ['swap', 'inversion'])
    }


    # Ustaw katalog wejściowy
    input_dir = r"C:\Users\obied\CLionProjects\PEA_zad4\cpp\Input"
    os.chdir(input_dir)

    instances = [
        "ftv47.xml",
        "ftv170.xml",
        "rbg403.xml"
    ]


    for filename in instances:
        full_path = os.path.join(input_dir, filename)
        if not os.path.exists(full_path):
            print(f"POMINIĘTO: Plik {filename} nie istnieje!")
            continue

        print(f"Rozpoczynam test dla: {filename}")


        experiment = TSPExperiment(filename)

        with mlflow.start_run():
            trials = Trials()
            best = fmin(
                fn=experiment.objective,
                space=space,
                algo=tpe.suggest,
                max_evals=50,
                trials=trials
            )

            mlflow.log_params(best)

            trials_data = []
            for trial in trials.trials:
                if trial['result']['status'] == STATUS_OK:
                    trials_data.append({
                        'params': trial['misc']['vals'],
                        'loss': trial['result']['loss']
                    })

            mlflow.log_dict(trials_data, "all_trials.json")


if __name__ == "__main__":

    run_experiments()

def _plot_evolution(self, evolution_data, title: str, filename: str):
    times, errors = zip(*evolution_data)
    plt.figure(figsize=(10, 6))
    plt.plot(times, errors)
    plt.xlabel('Czas [s]')
    plt.ylabel('Dystans')
    plt.title(title)
    plt.grid(True)
    plt.savefig(filename)
    plt.close()