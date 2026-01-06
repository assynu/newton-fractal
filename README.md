# Newton Fractal Generator

A C++ program that generates **Newton fractal images**, using **ISPC** to accelerate computation through data-parallel multithreading.

## Overview

This project implements a Newton fractal renderer.
Performance-critical parts of the computation are written in ISPC to take advantage of SIMD and multithreading, while the rest of the application is written in standard C++ and built with CMake.

## Purpose

This was a **learning project**, created to:
- better understand Newton fractals,
- experiment with ISPC and data-parallel programming,
- practice integrating ISPC with a CMake-based C++ project.

## Project status

**Archived / learning-only project**

- This project **will not be actively maintained**
- No new features or improvements are planned
- Issues and pull requests may not be reviewed

The code is provided as-is for reference and educational purposes.

## Build

### Build
```bash
$: make
```

### Run
```bash
$: make run
```

### Clean
```bash
$: make clean
```
