
# MPS - Material Particle Simulation

A simple material simulation inspired by the [Noita](https://noitagame.com) game engine [Fallingeverything](https://nollagames.com/fallingeverything/).

This a type of cellular automaton simulation, designed to simulate sand (for now!) in a 2D grid.
Each grid cell represents a small sand particle, that is updating every amount of time using simple rules based on realworld behavior.

![preview](assets/img/preview.gif)

---

## Features

- adjustable brush for placing particles.
- toggleable grid visibility.
- adjustable grid.
- adjustable simulation speed.
- toggleable debug overlay.
- ability to pause/resume simulation.

---

## Dependencies

- g++
- SDL3
- cmake

---

## Building

```bash
git clone --recursive https://github.com/DevLyas/MPS.git
cd MPS
cmake -B build
cmake --build build
./build/main
```

---

## Controls

- `UP/DOWN` increase/decrease brush size.
- `CTRL + -/=` increase/decrease grid size.
- `S` show/hide grid.
- `LEFT/RIGHT` increase/decrease simulation speed.
- `D` show/hide debug overlay.
- `P` pause/resume simulation.

---

## License

This project is licensed under the MIT License. Copyright © 2026 Ahmed Lyas.

