import numpy as np
import matplotlib.pyplot as plt

path = "Flow_N900_alpha2.500_phi1.200_delta0.150_gamma3.000_w0.500_sigm_obs10.000_K10.000_fx_ext0.100000_dt0.100/"

with open(path + "data_0300", "r") as f:
    header = f.readline().split()
Lx, Ly = float(header[2]), float(header[3])

data1 = np.loadtxt(path + "data_8000", skiprows=1)
data2 = np.loadtxt(path + "data_8090", skiprows=1)

x1, y1 = data1[:, 0], data1[:, 1]
x2, y2 = data2[:, 0], data2[:, 1]

dx = x2 - x1
dy = y2 - y1

dx -= Lx * np.round(dx / Lx)

dx -= np.mean(dx)
dy -= np.mean(dy)

plt.figure(figsize=(6,6))
plt.scatter(dx, dy, s=10, alpha=0.7, color='royalblue', edgecolors='none')

plt.title("Displacement Scatter Plot (PBC-corrected, mean-centered)", fontsize=14, fontweight='bold')
plt.grid(True, linestyle='--', alpha=0.5)
plt.axis("equal")

plt.xlim(dx.min()-1, dx.max()+1)
plt.ylim(dy.min()-1, dy.max()+1)

ax = plt.gca()
ax.tick_params(axis='both', which='major', labelsize=12, width=1.5)
ax.spines['bottom'].set_linewidth(1.5)
ax.spines['left'].set_linewidth(1.5)
ax.spines['top'].set_visible(False)
ax.spines['right'].set_visible(False)

plt.tight_layout()
plt.show()

