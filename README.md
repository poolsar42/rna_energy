# RNA folding problem

Here we describe what this repo does and how to run the code.

## Directory structure

- **data**: folder, contains `.pdb` files for 3D RNA structures.
- **headers**: folder, contains helper files which contain functions that are being used in `training.c`
- **results**: folder, contains outputs of `training.c` script (first script from the assignment). `.csv` files with 10 rows and 20 columns - log-ration values for all residue pairs for 0-20 Å distance range.
- **a.out**: executable files produced after compiling `training.c`
- **training.c**: main file which, when compiled, produces executable which solves task for first script.

## First Script

### Compilation

We use `glib` package for `C` to use implemented hash tables. Before compiling the code make sure to install the package on your machine. See how to install [here](https://gitlab.gnome.org/GNOME/glib/).
Once installed, run `gcc training.c` \``pkg-config --cflags --libs glib-2.0`\` to compile the code.

### Run

Once compiled, you will have execution file in your directory. By default it's called `a.out`.

It requires to pass 2 arguments when executed. First takes the input file, second - the output file. For instance, here's how we run it:

`./a.out "./data/1E95 Pseudoknot Structure.pdb" ./results/pseudo_energy_1E95.csv`

It will output `.csv` files in `results` directory.

## Colab URL

https://colab.research.google.com/drive/1-lLO0fifVIcNro4TlzP1OboB4euKd0Pj?authuser=0#scrollTo=hqOMRvM3A9lT

## Second Script

This script takes outputs of the first one (`.csv` files) and produces graphs for pseudo-energies as a function of distance for all residue-pairs.

## Third Script

Here we take outputs of the first script and compute Gibbs free energy using linear interpolation.
