 ./build/src/bench_runnable/sort_1000_counting > ./data/sort_counting_1000/data.json
python3 scripts/run_benchmark_folder.py data/plots/sort_base.json build/src/bench_runnable/sort_int_1000 data
python3 scripts/run_benchmark_folder.py data/plots/sort_base.json build/src/bench_runnable/sort_double_1000 data
python3 scripts/run_benchmark_folder.py data/plots/sort_base.json build/src/bench_runnable/sort_std_int64_t_1000 data
python3 scripts/run_benchmark_folder.py data/plots/sort_base.json build/src/bench_runnable/sort_fake_url_1000 data
python3 scripts/run_benchmark_folder.py data/plots/sort_base.json build/src/bench_runnable/sort_fake_url_pair_1000 data
