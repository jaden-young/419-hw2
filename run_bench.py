#!/usr/bin/env python3

import time
import subprocess
import csv
import os

EXECUTABLE_NAME = './assign2Graph'
THREAD_NUMS = [2, 4, 8, 16]
FILENAME = 'networkDatasets/HcNetwork.txt'


def time_run(filename, num_threads):
    """Return the wall time taken for a single run"""
    start = time.time()
    subprocess.run(
        [EXECUTABLE_NAME, filename,
         str(num_threads)],
        stdout=subprocess.DEVNULL)
    elapsed = time.time() - start
    return elapsed


def write_csv(filename, rows):
    with open(filename, 'w') as f:
        writer = csv.writer(f)
        writer.writerows(rows)


def bench_file(filename, thread_nums):
    threads = [1]
    seconds = []
    speedups = [1]
    single_threaded_time = time_run(filename, 1)
    seconds.append(single_threaded_time)

    for thread in thread_nums:
        threads.append(thread)
        t = time_run(filename, thread)
        seconds.append(t)
        speedup = single_threaded_time / t
        speedups.append(speedup)

    csv_name = filename.split('.')[0] + '_bench.csv'
    runs = list(zip(threads, seconds, speedups))
    runs.insert(0, ('Threads', 'Seconds', 'Speedup'))
    write_csv(csv_name, runs)
    return runs


def main():
    return bench_file(FILENAME, THREAD_NUMS)


if __name__ == '__main__':
    main()
