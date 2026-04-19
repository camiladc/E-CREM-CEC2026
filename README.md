# E-CREM-CEC2026

This project implements E-CREM, an evolutionary algorithm for solving container yard optimization problems. It is designed for the CEC 2026.

## Description

The algorithm uses genetic operators including crossover, mutation, and selection to optimize container stacking in a yard layout. It reads instance files defining the problem parameters and outputs optimized solutions.

## Dependencies

- C++ compiler (g++)
- Standard C++ libraries

## Building

To build the project, run:

```bash
make
```

This will compile all source files and create the `main` executable.

## Usage

Run the program with the following command:

```bash
./main <instance_path> <seed> <max_gen> <popsize> <pcross> <pmut> <pmut_swap> <pmut_inversion> <alfa> <beta> <pcross_one> <p_swap> <p_intFlip>
```

### Parameters

- `instance_path`: Path to the instance file (e.g., `Original_Intances/Random/R020306_0020_001.txt`)
- `seed`: Random seed for reproducibility
- `max_gen`: Maximum number of generations
- `popsize`: Population size (must be multiple of 4)
- `pcross`: Crossover probability
- `pmut`: Mutation probability
- `pmut_swap`: Swap mutation probability
- `pmut_inversion`: Inversion mutation probability
- `alfa`: Alpha parameter
- `beta`: Beta parameter
- `pcross_one`: One-point crossover probability
- `p_swap`: Swap probability
- `p_intFlip`: Integer flip probability

### Example

```bash
./main Original_Intances/Random/R020306_0020_001.txt 11 1500 30 0.4 0.7 0.5 0.3 5 0.001 0.3 0.4 0.9
```

## Docker

A Dockerfile is provided for containerized execution. Build and run with:

```bash
docker build -t e-crem-cec2026 .
docker run e-crem-cec2026
```

## License

See LICENSE file for details.