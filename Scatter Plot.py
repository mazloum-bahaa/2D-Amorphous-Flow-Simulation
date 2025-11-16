"""
This script visualizes particle displacements between two snapshots as a scatter plot.
It performs the following steps:

- Loads particle positions from two data files.
- Applies periodic boundary conditions in x (minimum image convention).
- Subtracts the mean displacement to remove global drift.
- Plots the resulting displacements (dx, dy) as a scatter plot.
- Formats the plot with equal aspect ratio, grid, limits, and customized axes.
"""

import numpy as np
import matplotlib.pyplot as plt


path = "Flow_N900_alpha2.500_phi1.200_delta0.001_gamma3.000_w0.500_sigm_obs10.000_K10.000_fx_ext0.001000_dt0.100/"

# Box dimensions (Lx, Ly)
Lx, Ly = float(3*15.5730668511), float(15.5730668511)  

# -----------------------------------------
# Load data files (skip first row which contains header)
# -----------------------------------------
data1 = np.loadtxt(path + "data_4000", skiprows=1)
data2 = np.loadtxt(path + "data_4050", skiprows=1)

# Extract x and y coordinates
x1, y1 = data1[:, 0], data1[:, 1]
x2, y2 = data2[:, 0], data2[:, 1]

# -----------------------------------------
# Compute displacements
# -----------------------------------------
dx = x2 - x1
dy = y2 - y1

# Apply periodic boundary conditions in x
dx -= Lx * np.round(dx / Lx)

# Subtract mean displacement (remove global drift)
dx -= np.mean(dx)
dy -= np.mean(dy)

# -----------------------------------------
# Scatter plot of displacements
# -----------------------------------------
plt.figure(figsize=(6,6))
plt.scatter(dx, dy, s=10, alpha=0.7, color='royalblue', edgecolors='none')

plt.title("Displacement Scatter Plot (PBC-corrected, mean-centered)", fontsize=14, fontweight='bold')
plt.grid(True, linestyle='--', alpha=0.5)
plt.axis("equal")

plt.xlim(dx.min()-1, dx.max()+1)
plt.ylim(dy.min()-1, dy.max()+1)

# -----------------------------------------
# Customize axes appearance
# -----------------------------------------
ax = plt.gca()
ax.tick_params(axis='both', which='major', labelsize=12, width=1.5)
ax.spines['bottom'].set_linewidth(1.5)
ax.spines['left'].set_linewidth(1.5)
ax.spines['top'].set_visible(False)
ax.spines['right'].set_visible(False)

plt.tight_layout()
plt.show()
