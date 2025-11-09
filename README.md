# Shear and Shuffle: 2D Polydisperse System Simulation with Obstacle

A C++ simulation of dense amorphous polydisperse systems under shear in a confined geometry with a central obstacle. This research code investigates anisotropic behavior and non-affine displacements in complex fluids.

## 📋 Project Overview

This project simulates a 2D polydisperse system (dense amorphous material) confined in a rectangular channel with a central obstacle. The system undergoes shear deformation, and various analysis tools are provided to study:
- Steady state detection
- T1 events and non-affine displacements
- Anisotropic behavior vs polydispersity
- Order parameter analysis

The simulation generates data for studying how polydispersity affects the mechanical response and structural rearrangements in dense systems.

## 🏗️ Repository Structure

ShearAndShuffle/
├── src/ # Source code
│ ├── Foam_Dollet_2D.cpp # Main simulation code
│ ├── twister.h # Random number generator header
│ └── twister.cpp # Random number generator implementation
├── analysis/ # Python analysis scripts
│ ├── Steady State Graph.py
│ ├── T1 Event and D2min, polydisperse neighbor list.py
│ ├── Displacement vector.py
│ ├── Data Generator.py
│ ├── Average Anisotropic Parameter.py
│ ├── Order Parameter Percentage Calculator.py
│ └── Scatter Plot.py
├── data/ # Generated simulation data
├── results/ # Analysis results and figures
└── README.md
text


## ⚙️ Compilation and Execution

### Prerequisites
- GCC compiler (g++)
- Linux/Ubuntu environment
- Python 3 with standard scientific stack (numpy, matplotlib)

### Compilation

bash
g++ -c Foam_Dollet_2D.cpp
g++ -c twister.cpp
g++ Foam_Dollet_2D.o twister.o -o Foam_Dollet_2D.exe

Execution
bash

./Foam_Dollet_2D.exe -N 900 -phi 0.9 -delta 0.05 -gamma 3 -w 0.5 -sigm_obs 10 -K 10.0 -fx_ext 0.001 -dt 0.1 -trajectory 1 -total_step 100000 -output_step 100 -run 1

🎯 Simulation Parameters
Parameter	Description	Typical Value
-N	Number of particles	900
-phi	Area packing fraction (1.2 = particles overlap)	0.9-1.2
-delta	Standard deviation of particle size distribution (0.0 = monodisperse)	0.0-0.15
-gamma	Ratio between linear box length along x and y	1.5-3
-w	Width of the walls	0.5-1.0
-sigm_obs	Diameter of the central obstacle	5.0-10.0
-K	Magnitude of hardness for walls and obstacle	10.0
-fx_ext	External force along x-direction	0.0001-0.005
-dt	Step size for Euler integration method	0.1
-trajectory	Flag to output trajectory data	1
-total_step	Total number of simulation iterations	100000
-output_step	Frequency of configuration output	100
-run	Simulation ID for multiple runs	1
🔬 Analysis Workflow
1. Steady State Detection
bash

python "analysis/Steady State Graph.py"

Detects when the system reaches steady state by monitoring energy, pressure, and stress over time.
2. Structural Analysis
bash

python "analysis/T1 Event and D2min, polydisperse neighbor list.py"
python "analysis/Displacement vector.py"

Analyzes T1 events, D2min, and non-affine displacements between different time steps.
3. Data Generation
bash

python "analysis/Data Generator.py"

Generates processed data for further analysis from raw simulation outputs.
4. Anisotropy Analysis
bash

python "analysis/Average Anisotropic Parameter.py"

Studies anisotropy behavior (A) vs polydispersity (δ) at constant φ=1.2 and f_ext=0.0001.
5. Order Parameter Analysis
bash

python "analysis/Order Parameter Percentage Calculator.py"

Calculates order parameter percentages for:

    Different forces at φ=1.2, δ=0.15 (polydisperse)

    Different packing fractions at φ=0.9

6. Preliminary Scatter Analysis
bash

python "analysis/Scatter Plot.py"

Initial analysis of Δy vs Δx behavior before anisotropy studies.
📊 Key Research Questions

    How does polydispersity (δ) affect anisotropic behavior in confined systems?

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

For detailed questions about the physics or methodology, please refer to the associated research publications or contact the author directly.
text


## Next Steps for Your Repository:

1. **Create the folder structure** I showed above
2. **Organize your files** into the appropriate folders:
   - Move C++ files to `src/`
   - Move Python scripts to `analysis/`
   - Create `data/` and `results/` folders
3. **Add your actual code files** to the repository
4. **Consider adding**:
   - A `requirements.txt` for Python dependencies
   - Example input/output files
   - Any relevant research papers

This README provides a clear, professional structure that will help others understand and potentially build upon your research. Would you like me to modify any section or add more specific details about any part of the analysis?
