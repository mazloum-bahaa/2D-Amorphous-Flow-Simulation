import numpy as np

path = "Flow_N900_alpha2.500_phi1.200_delta0.150_gamma3.000_w0.500_sigm_obs10.000_K10.000_fx_ext1.000000_dt0.100/"
start_index = 1000
step = 30
end_index = 9999

A_values = []

for i in range(start_index, end_index, step):
    file1 = path + f"data_{i:04d}"
    file2 = path + f"data_{i+step:04d}"
    
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
        dx -= Lx * np.round(dx / Lx)
        
        dx -= np.mean(dx)
        dy -= np.mean(dy)
        
        m11 = np.sum(dx**2) / N
        m12 = np.sum(dx * dy) / N
        m22 = np.sum(dy**2) / N
        M = np.array([[m11, m12],
                      [m12, m22]])
        
        eigvals = np.linalg.eigvalsh(M)
        lam_min, lam_max = eigvals[0], eigvals[1]
        A = 0.0 if lam_max + lam_min == 0 else (lam_max - lam_min) / (lam_max + lam_min)
        
        A_values.append(A)
        print(f"{file1} → {file2}: A = {A:.6f}")
        
    except FileNotFoundError:
        print(f"Skipping missing file: {file2}")
        continue

A_values = np.array(A_values)
A_mean = np.mean(A_values)
A_std = np.std(A_values)

print("\n=== Summary over all pairs ===")
print(f"Number of pairs analyzed: {len(A_values)}")
print(f"Average A = {A_mean:.6f}")
print(f"Standard deviation = {A_std:.6f}")
