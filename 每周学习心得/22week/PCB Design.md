# PCB Design

## Practical operation

### Design Flow

-  

### Practical steps

- Drawing Library: Draw the devices you need. Device = symbol + package (+3D model + physical picture)

- Place the device on the schematic and connect

- Schematic to PCB

- Export BOM and Gerber order

## Definition

### Printed Circuit Board

### It is an important component in electronic products, a support body for components, and a carrier for electrical connections of many electronic components.

### Copper + substrate + green oil (solder mask) = PCB board

## PCB laminated structure

### PCB single layer board

### PCB Multiple layer board

- When it comes to bypassing the copper foil, the single-layer board does not meet the requirements.

- Therefore, we need to develop double-layer boards

	- How to switch layer?

		- If the same layer cannot be passed, the substrate can be plated with copper to achieve layer-changing via holes.

		-  

### Four-layer board: = 2 2-layer boards + core board (non-conductive)

-  

### Six-layer board: 3 2-layer boards + 2 core boards

-  

### Why are there no single-layer boards?

- The single-layer board has one substrate and no stamping on both sides, so the cost difference is not big.

- Increased symmetrical stamping stability

- The fewer levels of the same project, the more awesome it is.

### PCB through holes, blind holes, buried holes

- Through holes are multi-layer board changing layers, from the top layer to the bottom layer

- Blind hole: Can’t see the bottom, don’t know which layer it leads to

- Buried via: inner layer switching, invisible to the outside

## PCB production process

### PCBA: Finished product with electronic components attached to the PCB board

- The PCB is loaded through SMT (SMD installation), and then the plug-in becomes PCBA through DIP (Plug-in component installation).

### Process

- Cutting: the process of cutting the original copper-clad laminate (copper + substrate + copper) into a board that can be made on the production line

- Drilling: Drilling via holes, buried holes, blind holes

- Immersed copper: The PCB board undergoes an oxidation-reduction reaction in the copper immersed cylinder, and copper is plated to the inner wall of the via hole.

- Lamination: Laminate the blue film on the top and bottom layers

- Exposure: Expose the circuit film to the PCB under an exposure machine, and the circuit will be transferred to the dry film. There are black lines, and there are no lines that are transparent.

- Develop: Develop the wireless line part, causing the line part to be yellow copper

- Electric copper: The board is placed in the electric copper equipment, and the developing area is electrolytically coated with copper.

- Electric tin: the preparation work of drying out the copper protected by the film

- Film removal: remove the blue film to expose unnecessary copper

- Etching: Use potion to corrode the unnecessary copper in the circuit board

- Removing tin: removing the tin from the line

- Optical AOI line scanning: ensuring line quality

- Printing resist solder oil: Apply green oil to all areas to prevent electric shock and copper oxidation

- Solder mask exposure and development: remove the green oil on the pad

- Characters (baking board): silk screen, writing on the board

- Surface treatment: protect the exposed copper of the pad

- Gong edge forming: large plate becomes smaller plate

- Electrical test: Use a needle test or a general electromechanical performance check to see if there is an open or short circuit.

- FQC: quality control

- Final inspection, random testing, packaging

## PCB welding technology

### Manual welding

### Dip soldering: For straight plug-ins, a lot of tin is required

-  

### Reflow soldering: Brush solder paste onto the steel mesh, which has openings for the pins

### Wave soldering: for connectors

-  

## Component symbols and packages

### wire

-  

###  via hole

-  

### silk screen

-  

### Pad: soldering components

-  

### Positioning holes: not copper plated, mechanical structure, used to install PCB

-  

## Graphical representation

### Resistance

-  

### 子主题 2

