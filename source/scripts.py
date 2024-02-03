import numpy as np


def linear_interpolation(x_values, y_values):
    """
    Performs linear interpolation for a set of points.

    Parameters:
    - x_values: List or numpy array of x values
    - y_values: List or numpy array of corresponding y values

    Returns:
    - Function that takes an x value or array and outputs the interpolated y value or array
    """
    x_values, y_values = np.asarray(x_values), np.asarray(y_values)

    def interpolate(x):
        # Find the indices of the closest x values in the given data
        indices = np.searchsorted(x_values, x, side='right')

        # Handle cases where x is beyond the range of provided data
        indices = np.clip(indices, 1, len(x_values) - 1)

        # Linear interpolation formula using numpy array operations
        x0, x1 = x_values[indices - 1], x_values[indices]
        y0, y1 = y_values[indices - 1], y_values[indices]
        return y0 + (y1 - y0) * (x - x0) / (x1 - x0)

    return np.vectorize(interpolate)


def energy_calculation(coord_dict, energies):
    """
    Performs energy calculation using the interpolation for a set of energy points.

    Parameters:
    - coord_dict: Dict of calculated distances for each nucleotide pairs loaded from a json
    - energies: DataFrame of calculated pseudo energies

    Returns:
    - the calculated value of Gibbs free energy
    """
    scores = np.array([])
    
    for nt_pair in coord_dict.keys():
        pair_energy = energies.loc[nt_pair]
        interpol = linear_interpolation(x_values=pair_energy.index.astype('float').values, 
                                        y_values=pair_energy.values)
        scores = interpol(coord_dict[nt_pair])
        scores = np.append(scores, np.sum(pair_energy) / energies.shape[1])

        
    gibbs_free_energy = np.sum(scores)
    return(gibbs_free_energy)