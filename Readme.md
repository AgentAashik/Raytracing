# Raycasting Simulation

A simple raycasting simulation using SDL2. Rays are emitted from a light source (sun) and interact with an obstacle to create shadows. The light source can be moved with the mouse, and the obstacle moves up and down.

---

## How It Works

- **Rays**: Emitted from the light source in all directions.
- **Obstacle**: Blocks rays, creating shadows.
- **Light Source**: Movable with the mouse.
- **Obstacle Movement**: Moves up and down automatically.

---

## Compile and Run

**Install SDL2**:
   ```bash
   sudo apt install libsdl2-dev

## Compile
    gcc -o raytracer raytracer.c -IC:/SDL2/include -LC:/SDL2/lib -lSDL2main -lSDL2
    
## Run
    ./raytracer
