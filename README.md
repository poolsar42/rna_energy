# RNA folding problem

Here we describe what this repo does and how to run the code.

## Directory structure

- **data**: folder, contains `.pdb` files for 3D RNA structures.
- **headers**: folder, contains helper files which contain functions that are being used in `training.c`
- **outputs**: folder, contains results of `training.c` script (first script from the assignment). 
It has 2 sub-directories:
    - **pseudo_energies**: `.csv` files with 10 rows and 20 columns - log-ration values for all residue pairs for 0-20 Å distance range.
    - **distances**: `.json` and `.txt` files which contain distances for all 10 pairs of nucleotides. These distances then used to compute pseudo energy on them with interpolated curves. Summed energies for the final Gibbs free energy value.
- **a.out**: executable files produced after compiling `training.c`
- **training.c**: main file which, when compiled, produces executable which solves task for first script.
- **nodebooks**: directory which contains jupyter notebook with implementation of second and thirds scripts.
- **scripts**: contains functions in `.py` files, which are used in `.ipynb` notebook.

## First Script

### Compilation

We use `glib` package for `C` to use implemented hash tables. Before compiling the code make sure to install the package on your machine. See how to install [here](https://gitlab.gnome.org/GNOME/glib/).
Once installed, run `gcc training.c` \``pkg-config --cflags --libs glib-2.0`\` to compile the code.

### Run

Once compiled, you will have execution file in your directory. By default it's called `a.out`.

It requires to pass 2 arguments when executed. First takes the input file, second - the output file. For instance, here's how we run it:

`./a.out "./data/1E95 Pseudoknot Structure.pdb" ./results/pseudo_energy_1E95.csv`

It will output `.csv` files in `results` directory.

## Second Script and Third Scripts

We combined these two scripts in `./notebooks/RNA.ipynb` file.

They are visually and textually separated and also can be run separately.

**Second script** takes outputs of the first one (`.csv`- files) and produces graphs for pseudo-energies as a function of distance for all residue-pairs.

**Third Script** also takes outputs of the first script and compute Gibbs free energy using linear interpolation. 

Because we have multiple log-ratios for the same distance the only thing we could do to compute linear interpolation for any given distance - is to compute average of all log ratios for a given distance. Then sum up all these averaged scores from all distances to get a Gibbs free energy. This is what we did.

**Ideally** you should be able to run these scripts right out of the box, if you set up jupyter notebook in with root in this folder's root. All relative paths are respected in `.ipynb` file.
