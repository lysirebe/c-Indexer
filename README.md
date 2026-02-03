# c-Indexer
Compile both versions:
bashg++ -std=c++17 -O0 -o indexer_O0 indexer_simple.cpp
g++ -std=c++17 -O2 -o indexer_O2 indexer_simple.cpp
3. Run benchmarks (3 runs each):
bashecho "=== O0 (no optimisation) ===" 
./indexer_O0 "/Users/lys/My Downloads" results_O0_run1.jsonl
./indexer_O0 "/Users/lys/My Downloads" results_O0_run2.jsonl
./indexer_O0 "/Users/lys/My Downloads" results_O0_run3.jsonl

echo "=== O2 (optimised) ===" 
./indexer_O2 "/Users/lys/My Downloads" results_O2_run1.jsonl
./indexer_O2 "/Users/lys/My Downloads" results_O2_run2.jsonl
./indexer_O2 "/Users/lys/My Downloads" results_O2_run3.jsonl
