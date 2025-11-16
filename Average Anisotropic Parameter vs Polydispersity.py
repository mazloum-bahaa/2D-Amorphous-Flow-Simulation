"""
This script analyzes particle displacement data from multiple simulation runs
to compute the anisotropy parameter A for different values of delta (polydispersity). 

For each delta value:
- It loops over pairs of snapshot files from a corresponding Flow directory.
- Computes particle displacements, subtracts the mean drift, and constructs
  the covariance matrix.
- Calculates the anisotropy parameter A from the eigenvalues of the covariance matrix.
- Stores the mean A for all valid snapshot pairs for that delta.

Finally, it plots the mean anisotropy ⟨A⟩ versus delta with points marked as 'x'.
"""

import numpy as np
import os
import matplotlib.pyplot as plt

# -----------------------------------------
# PARAMETERS (We change the values of delta here fixing force and packing factor, and we calculate the parameter A for each value of delta then we plot the results)
# -----------------------------------------
deltas = [0.000,0.001, 0.05, 0.1, 0.15, 0.2, 0.3, 0.6]

prefix = "Flow_N900_alpha2.500_phi1.200_delta"
suffix = "_gamma3.000_w0.500_sigm_obs10.000_K10.000_fx_ext0.001000_dt0.100/"

start_index = 1000
step = 120
end_index = 9999

A_vs_delta = {}   # store mean A for each delta

# -----------------------------------------
# LOOP over all deltas
# -----------------------------------------
for delta in deltas:

    folder = prefix + f"{delta:.3f}" + suffix
    if not os.path.isdir(folder):
        print(f"Skipping missing folder: {folder}")
        continue

    print(f"\n=== Processing delta = {delta} ===")
    A_values = []

    for i in range(start_index, end_index, step):

        # Try 4-digit filenames first (split files in our simulations are either 4 or 5 digits depending on how much files we needed during the study)
        file1 = folder + f"data_{i:04d}"
        file2 = folder + f"data_{i+step:04d}"

        # If any file missing, try 5-digit filenames
        if not os.path.isfile(file1) or not os.path.isfile(file2):
            file1 = folder + f"data_{i:05d}"
            file2 = folder + f"data_{i+step:05d}"

        # If still missing, skip
        if not os.path.isfile(file1) or not os.path.isfile(file2):
            print(f"Missing file → skipping pair: {i} → {i+step}")
            continue

        try:
            with open(file1, "r") as f:
                header = f.readline().split()

            N = int(header[1])
            Lx, Ly = float(header[2]), float(header[3])

            data1 = np.loadtxt(file1, skiprows=1)
            data2 = np.loadtxt(file2, skiprows=1)

            x1, y1 = data1[:, 0], data1[:, 1]
            x2, y2 = data2[:, 0], data2[:, 1]

            dx = x2 - x1
            dy = y2 - y1

            # periodic BC only in x, y is confined by walls
            dx -= Lx * np.round(dx / Lx)

            # subtract mean drift
            dx -= np.mean(dx)
            dy -= np.mean(dy)

            # covariance matrix
            m11 = np.sum(dx**2) / N
            m12 = np.sum(dx * dy) / N
            m22 = np.sum(dy**2) / N
            M = np.array([[m11, m12],
                          [m12, m22]])

            eigvals = np.linalg.eigvalsh(M)
            lam_min, lam_max = eigvals
            A = 0.0 if lam_max + lam_min == 0 else (lam_max - lam_min) / (lam_max + lam_min)

            A_values.append(A)

        except Exception as e:
            print(f"Error reading {file1} or {file2}: {e}")
            continue

    # Store mean anisotropy
    if len(A_values) > 0:
        A_vs_delta[delta] = np.mean(A_values)
        print(f"delta={delta} → ⟨A⟩ = {A_vs_delta[delta]:.6f}")
    else:
        print(f"No valid pairs for delta = {delta}")


# -----------------------------------------
# PLOT A vs delta
# -----------------------------------------
d_sorted = sorted(A_vs_delta.keys())
A_sorted = [A_vs_delta[d] for d in d_sorted]

plt.figure(figsize=(6,4))
plt.scatter(d_sorted, A_sorted, marker='x')
plt.xlabel("delta")
plt.ylabel("mean A")
plt.title("Anisotropy vs delta,$\Delta t$=120")
plt.grid(True)
plt.tight_layout()
plt.show()
