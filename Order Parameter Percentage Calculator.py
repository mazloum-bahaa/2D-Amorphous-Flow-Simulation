import os
import numpy as np
import matplotlib.pyplot as plt

base_dir = "phi1.2"
results = {}

for subdir in sorted(os.listdir(base_dir)):
    full_path = os.path.join(base_dir, subdir)
    if not os.path.isdir(full_path):
        continue

    force_str = subdir.split("fx_ext")[-1].split("_")[0]
    force_val = float(force_str)

    jammed = 0
    flowing = 0

    for filename in os.listdir(full_path):
        if not filename.endswith(".txt"):
            continue

        file_path = os.path.join(full_path, filename)
        try:
            with open(file_path, "r") as f:
                lines = f.readlines()[-2:]
            if len(lines) < 2:
                continue

            e1 = float(lines[-2].split()[1])
            e2 = float(lines[-1].split()[1])

            if np.isclose(e1, e2, rtol=1e-12, atol=1e-15):
                jammed += 1
            else:
                flowing += 1
        except Exception as e:
            print(f"Error reading {file_path}: {e}")

    results[force_val] = {'jammed': jammed, 'flowing': flowing}

forces = sorted(results.keys())
percent_flowing = [
    100 * results[f]['flowing'] / (results[f]['jammed'] + results[f]['flowing'])
    for f in forces
]

print("\n=== Simulation Results ===")
for f, p in zip(forces, percent_flowing):
    print(f"Force fx_ext = {f:.6f}: Flowing = {p:.1f}%")

plt.figure(figsize=(7,5))
plt.semilogx(forces, percent_flowing, 'o-', linewidth=2, markersize=8, label="Flowing fraction")

plt.xlabel(r"External Force $f_{x,\mathrm{ext}}$", fontsize=13)
plt.ylabel("Flowing Simulations (%)", fontsize=13)
plt.title("Fraction of Flowing Simulations vs Applied Force", fontsize=14)
plt.grid(True, which="both", linestyle='--', alpha=0.6)
plt.ylim(-2, 102)
plt.xlim(min(forces)*0.8, max(forces)*1.2)
plt.legend(frameon=False, fontsize=12)
plt.tight_layout()
plt.show()
