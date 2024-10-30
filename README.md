# Data_Mining_Weighting
Certo, aqui está uma proposta de README.md melhorado para este projeto:

# Sparse Matrix Weighting

This project provides a tool for weighting sparse matrices, which is a common task in data mining and analysis.

## Description

The Sparse Matrix Weighting program takes input data in the form of a sparse matrix (either in dense or Matrix Market format) and applies various weighting algorithms to generate weighted matrices. These weighted matrices can then be used for further data analysis and mining tasks.

The program supports the following weighting algorithms:

- IDF (Inverse Document Frequency)
- ICF (Inverse Class Frequency)
- RF (Relevance Frequency)
- ICF-BASED
- RFXIDF
- QUIQUAD
- Output format (Matrix Market or LibSVM)

## Getting Started

### Prerequisites

- C/C++ compiler (e.g., GCC, Clang)

### Installation

1. Clone the repository:
```
git clone https://github.com/howardroatti/Data_Mining_Weighting.git
```

### Usage

1. Prepare your input data in a sparse matrix format (dense or Matrix Market).
2. Create a configuration file (e.g., `config.h`) to specify the weighting algorithm, normalization method, dimensionality reduction, and output format.
3. Compile the program:
```
g++ -o data_mining_weighting ponderacoes.cpp
```
4. Run the program:
```
./data_mining_weighting
```
5. The weighted matrices will be saved in the output directory, as specified in the configuration.

## Configuration

The program's behavior is controlled through command-line arguments or a configuration file (`config.h`). The available options include:

- Weighting algorithm (`-p`)
- Input matrix format (`-v` for dense, `-m` for Matrix Market)
- Class file (`-c`)
- Normalization method (`-n`)
- Dimensionality reduction method (`-r`)
- Output format (`-o`)
- Reduction threshold (`-a`)

Refer to the `main()` function in the `main.cpp` file for a detailed explanation of these options.

## Contributing

Contributions are welcome! If you have any suggestions, bug reports, or feature requests, please open an issue or submit a pull request.

## License

This project is licensed under the [MIT License](LICENSE).
