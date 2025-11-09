# 🧬 2D Amorphous System Simulation

This repository contains the complete numerical framework used to study the **flow of a 2D polydisperse amorphous system** in a rectangular channel with a central obstacle.  
The simulation is implemented in **C++**, and the subsequent analysis and visualization are performed with **Python** scripts.  
This work investigates steady-state dynamics, plastic rearrangements, anisotropy, and order parameters as a function of particle polydispersity and applied external force.

This repository serves as the **research foundation** for the dataset and challenge published separately in:  
👉 [**Shear and Shuffle Challenge**](https://github.com/mazloum-bahaa/ShearAndShuffle)

---

## 📖 Overview

Amorphous materials such as foams, emulsions, or dense colloids exhibit complex flow behavior due to their disordered microstructure and the interplay between elastic and plastic responses.  
Here, we simulate a **2D polydisperse soft-particle system** driven through a **rectangular channel** containing a **circular obstacle** at its center.

The model captures:

- The emergence of a steady state under constant external forcing.
- The role of **polydispersity** on microscopic **anisotropy** and flow patterns.
- The statistics of **T1 rearrangements** and **non-affine displacement (D²min)**.
- The **transition** between flowing and arrested regimes.

---

## ⚙️ Simulation Code

The simulation is implemented in **C++** using a Mersenne Twister random number generator for initialization.

### 📂 Source Files

| File | Description |
|------|--------------|
| `Foam_Dollet_2D.cpp` | Main simulation code implementing particle dynamics. |
| `twister.cpp`, `twister.h` | Mersenne Twister pseudorandom number generator. |

---

## 🧮 Compilation and Execution

### Compilation (Linux)
g++ -c Foam_Dollet_2D.cpp
g++ -c twister.cpp
g++ Foam_Dollet_2D.o twister.o -o Foam_Dollet_2D.exe

Execution

./Foam_Dollet_2D.exe \
  -N 900 \
  -phi 0.9 \
  -delta 0.05 \
  -gamma 3 \
  -w 0.5 \
  -sigm_obs 10 \
  -K 10.0 \
  -fx_ext 0.001 \
  -dt 0.1 \
  -trajectory 1 \
  -total_step 100000 \
  -output_step 100 \
  -run 1

Parameter Description
Parameter	Meaning	Example
-N	Number of particles	900
-phi	Area fraction (packing density). Higher than 1 means overlapping particles.	1.2
-delta	Standard deviation of the particle size distribution (polydispersity).	0.05
-gamma	Ratio of box lengths along x and y (Lx/Ly).	3
-w	Width of the walls confining the system.	0.5
-sigm_obs	Diameter of the central obstacle.	10
-K	Stiffness constant of walls and obstacle.	10.0
-fx_ext	External driving force along x.	0.001
-dt	Time step for the Euler method.	0.1
-trajectory	Output trajectory flag (1 = save).	1
-total_step	Total number of simulation iterations.	100000
-output_step	Frequency of trajectory outputs.	100
-run	Simulation ID for reproducibility.	1
🧠 Post-Processing and Analysis

The simulation produces trajectory and observable data analyzed with dedicated Python scripts.
Each script corresponds to a specific aspect of the study.
Script	Description
Steady_State_Graph.py	Determines the steady-state regime by analyzing energy, pressure, and stress versus time.
T1_Event_and_D2min.py	Detects T1 rearrangements and computes local non-affine deformation parameter (D²min).
Polydisperse_Neighbor_List.py	Builds neighbor lists that account for the size distribution of particles.
Displacement_Vector.py	Visualizes displacement vectors to highlight regions of large deformation.
Data_Generator.py	Generates datasets for the Shear and Shuffle Challenge
.
Average_Anisotropic_Parameter.py	Computes average anisotropy ⟨A⟩ versus polydispersity δ.
Order_Parameter_Percentage_Calculator.py	Evaluates the fraction of flowing vs. arrested configurations at different forces.
Scatter_Plot.py	Creates Δy vs. Δx scatter plots to visualize flow heterogeneity.
📊 Example Studies

    Steady-State Detection — Identify time regimes where energy and stress stabilize.

    Microscopic Rearrangements — Quantify T1 events and D²min distributions.

    Anisotropy Analysis — Compute average anisotropy ⟨A⟩ vs. polydispersity δ.

    Order Parameter Analysis — Measure fraction of flowing vs arrested configurations under different forces.

    Flow Field Visualization — Scatter plots of displacements reveal heterogeneous dynamics.

📁 Repository Structure

2D_Amorphous_System_Simulation/
│
├── src/
│   ├── Foam_Dollet_2D.cpp
│   ├── twister.cpp
│   └── twister.h
│
├── python_scripts/
│   ├── Steady_State_Graph.py
│   ├── T1_Event_and_D2min.py
│   ├── Polydisperse_Neighbor_List.py
│   ├── Displacement_Vector.py
│   ├── Data_Generator.py
│   ├── Average_Anisotropic_Parameter.py
│   ├── Order_Parameter_Percentage_Calculator.py
│   └── Scatter_Plot.py
│
├── data/                # Example outputs
├── results/             # Example figures
└── README.md

🧰 Dependencies
Simulation

    g++ compiler

    Standard C++ libraries

Python Analysis

pip install numpy scipy matplotlib pandas

Optional (for visualization):

pip install ovito seaborn

📈 Example Outputs

    Energy and stress plots showing steady-state behavior

    D²min maps highlighting non-affine rearrangements

    Anisotropy vs. polydispersity trends

    Order parameter vs. driving force phase diagrams

    Scatter plots showing Δy vs. Δx displacements

🔗 Related Work

The dataset generated here is used in the companion challenge:
📂 Shear and Shuffle Challenge
🧾 Citation

If you use this repository or dataset, please cite:

    Your Name, “2D Amorphous System Simulation”, 2025.
    GitHub Repository: https://github.com/YourUsername/2D_Amorphous_System_Simulation

👩‍🔬 Author

Your Name
Université Grenoble Alpes — Laboratoire Interdisciplinaire de Physique (LiPhy)
📧 your.email@domain.com

---

✅ **Instructions to add this properly:**
1. Go to your repository on GitHub.  
2. Click **“Add file → Create new file”**.  
3. Name it exactly: `README.md`.  
4. Paste **everything** from the code block above.  
5. Commit.  

When you view the repo, you’ll see:  
- The headings properly formatted (`#`, `##` etc.).  
- Tables nicely aligned.  
- Code blocks for bash commands.  
- Lists and bold text all rendered cleanly.

---

Would you like me to insert your **real name, email, and GitHub link** into the last section and citation before you upload it? That way it’s publication-ready.
