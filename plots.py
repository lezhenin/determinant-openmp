import pandas as pd
import matplotlib.pyplot as plt
import json

import matplotlib

matplotlib.rc('font', **{'family': 'serif', 'serif': ['Computer Modern Roman']})
matplotlib.rc('text', usetex=True)
plt.rcParams['text.latex.preamble'] = [r'\usepackage[english,russian]{babel}',
                                       r'\usepackage{amsmath}']

repetitions = 100
unit = 'ms'


def parse_name(name):
    parts = name.split('/')
    matrix_size = int(parts[1])
    threads = int(parts[2])
    return matrix_size, threads


def convert_dictionary(benchmark):
    assert benchmark['time_unit'] == unit
    assert benchmark['repetitions'] == repetitions

    matrix_size, threads = parse_name(benchmark['name'])

    return {
        'aggregate': benchmark['aggregate_name'],
        'real_time': benchmark['real_time'],
        'cpu_time': benchmark['cpu_time'],
        'threads': threads,
        'matrix_size': matrix_size
    }


file = open('output/benchmark3.json', 'r')
data = json.load(file)

benchmarks = data['benchmarks']
results = list(map(
    lambda d: convert_dictionary(d), filter(
        lambda b: 'aggregate_name' in b, benchmarks
    ))
)

results = pd.DataFrame(results)
print(results.sort_values(by=['matrix_size', 'threads']).to_string())


plt.figure(figsize=(6.5, 4.5))
for i in [1, 2, 4, 8]:
    values = results.query(f'aggregate == "mean" and threads == {i}').loc[:, ['matrix_size', 'real_time']].values
    plt.plot(values[:, 0], values[:, 1], '-o')
plt.ylabel('Время (мс)', fontsize=14)
plt.xlabel('Размер матрицы', fontsize=14)
plt.xticks(fontsize=14)
plt.yticks(fontsize=14)
plt.legend(['1 поток'] + [f'{i} потока' for i in [2, 4, 8]], fontsize=14)
plt.savefig('./output/plot.svg')
plt.show()


plt.figure(figsize=(6.5, 4.5))
for i in [1, 2, 4, 8]:
    values = results.query(f'aggregate == "mean" and threads == {i}').loc[:, ['matrix_size', 'real_time']].values
    plt.plot(values[:, 0], values[:, 1], '-o')
plt.xscale('log', basex=2)
plt.yscale('log', basey=10)
plt.ylabel('Время (мс)', fontsize=14)
plt.xlabel('Размер матрицы', fontsize=14)
plt.xticks(fontsize=14)
plt.yticks(fontsize=14)
plt.legend(['1 поток'] + [f'{i} потока' for i in [2, 4, 8]], fontsize=14)
plt.savefig('./output/plot_log.svg')
plt.show()
