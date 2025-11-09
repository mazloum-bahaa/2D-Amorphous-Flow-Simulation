# 2D Amorphous System Simulation with Obstacle

A C++ simulation of dense amorphous polydisperse systems in a confined geometry with a central obstacle. This research code investigates anisotropic parameter and non-affine displacements in dense amorphous systems.

## 📋 Project Overview

This project simulates a 2D polydisperse system (dense amorphous material) confined in a rectangular channel with a central obstacle. The system undergoes non affine deformation, and various analysis tools are provided to study:
- Steady state detection
- T1 events and non-affine displacements
- Anisotropic behavior vs polydispersity
- Order parameter analysis

The simulation generates data for studying how polydispersity, packing factor and many other parameters affects the dynamics and structural rearrangements in dense systems.

## 🧮 Compilation and Execution

### Compilation (Linux)
```bash
g++ -c Foam_Dollet_2D.cpp
g++ -c twister.cpp
g++ Foam_Dollet_2D.o twister.o -o Foam_Dollet_2D.exe

Execution

./Foam_Dollet_2D.exe -N 900 -phi 0.9 -delta 0.05 -gamma 3 -w 0.5 -sigm_obs 10 -K 10.0 -fx_ext 0.001 -dt 0.1 -trajectory 1 -total_step 100000 -output_step 100 -run 1

(parameters can be changed and tuned as desired)

🎯 Simulation Parameters

| Parameter      | Description                                                           | Typical Value |
| -------------- | --------------------------------------------------------------------- | ------------- |
| `-N`           | Number of particles                                                   | 900           |
| `-phi`         | Area packing fraction (1.2 = particles overlap)                       | 0.9–1.2       |
| `-delta`       | Standard deviation of particle size distribution (0.0 = monodisperse) | 0.0–0.15      |
| `-gamma`       | Ratio between box lengths along x and y                               | 1.5–3         |
| `-w`           | Width of the walls                                                    | 0.5–1.0       |
| `-sigm_obs`    | Diameter of the central obstacle                                      | 5.0–10.0      |
| `-K`           | Stiffness of walls and obstacle                                       | 10.0          |
| `-fx_ext`      | External force along x-direction                                      | 0.0001–0.005  |
| `-dt`          | Step size for Euler integration                                       | 0.1           |
| `-trajectory`  | Flag to output trajectory data                                        | 1             |
| `-total_step`  | Total number of simulation iterations                                 | 100000        |
| `-output_step` | Frequency of configuration output                                     | 100           |
| `-run`         | Simulation ID for multiple runs                                       | 1             |

🔬 Analysis Workflow
1. Steady State Detection

python "analysis/Steady State Graph.py"

Detects when the system reaches steady state by monitoring energy, pressure, and stress over time.
2. Structural Analysis

python "analysis/T1 Event and D2min, polydisperse neighbor list.py"
python "analysis/Displacement vector.py"

Analyzes T1 events, D²min, and non-affine displacements between different time steps.
3. Data Generation

python "analysis/Data Generator.py"

Generates processed data for further analysis from raw simulation outputs.
4. Anisotropy Analysis

python "analysis/Average Anisotropic Parameter.py"

Studies anisotropy behavior (A) vs polydispersity (δ) at constant φ=1.2 and f_ext=0.0001.
5. Order Parameter Analysis

python "analysis/Order Parameter Percentage Calculator.py"

Calculates order parameter percentages for:

    Different forces at φ=0.9, 1.2, δ=0.15 (polydisperse)

6. Preliminary Scatter Analysis

python "analysis/Scatter Plot.py"

Initial analysis of Δy vs Δx behavior before anisotropy studies.
📊 Key Research Questions

    How does polydispersity (δ) affect anisotropic parameter in confined systems?

    What is the relationship between external force and structural order?

    How do non-affine displacements correlate with T1 events?

    What is the effect of packing fraction on system ordering?

🤝 Contributing

This is a research codebase. For questions about the methodology or collaboration opportunities, please contact the maintainer.
📝 Citation

If you use this code in your research, please acknowledge the original work.
👤 Author

Bahaa Mazloum
GitHub: @mazloum-bahaa

For detailed questions about the physics or methodology, contact the author directly.
