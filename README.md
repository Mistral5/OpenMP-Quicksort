To run the code specify:
1. Input file name
2. Output file name
3. Number of threads to be used
4. Operating mode:
	- `0` Without parallelism
	- `1` With using omp_section
	- `2` With using omp_task

Examples:
- in.txt out.txt 1 0
- in.txt out.txt 4 1
- in.txt out.txt 8 2