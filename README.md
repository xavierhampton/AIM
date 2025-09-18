# aim

<div >
  <img src="https://img.shields.io/badge/C-555555?style=for-the-badge&logo=c&logoColor=white" height="50" />
  <img src="https://img.shields.io/badge/CMake-v3.25-blue?style=for-the-badge&logo=cmake&logoColor=white" height="50" />
  <img src="https://img.shields.io/badge/raylib-v5.0-red?style=for-the-badge&logo=raylib&logoColor=white" height="50" />
</div>

**aim** is a **fast, minimalistic, and lightweight aim trainer** built with **C** and **raylib**.  
It’s designed for **pure aim practice** without distractions  

##  Features

- Lightweight and fast — **designed for smooth aim practice without unnecessary features**  
- Minimal, centered HUD with **time, hits, and accuracy**  
- **Extremely customizable colors** for targets, crosshair, and HUD    
- Multiple game modes for varied aim practice  
- Built with **raylib** for cross-platform support

  
##  Map Types

- **Gridshot** – shoot targets that appear in a grid pattern.  
- **Float Shot** – shoot moving airborne targets that drift across the map.
- **Float Track** – keep your crosshair locked on moving airborne targets that drift across the map.  
- **Tracking** – keep your crosshair locked on a moving target to build accuracy over time.  


##  Demos

**Gameplay**  
----
<img src="https://github.com/xavierhampton/aim/blob/main/images/MapsDemo.gif"/>

**Settings**  
----
<img src="https://github.com/xavierhampton/aim/blob/main/images/SettingsDemo.gif"/>

 **Color Variety**  
 ----
<img src="https://github.com/xavierhampton/aim/blob/main/images/colors.png"/>




## Build & Run

### Prerequisites
- **C compiler** (GCC, Clang, or MSVC)
- **CMake** 


### Build Instructions
```bash

# Clone the repository
git clone https://github.com/xavierhampton/aim.git
cd aim

# Create build directory
mkdir build && cd build

```
#### Run CMake

| Platform          | Command                                                                 |
|-------------------|-------------------------------------------------------------------------|
| Linux / macOS     | ```cmake ..```<br> ```make```                                          |
| Windows (MinGW)   | ```cmake -G "MinGW Makefiles" ..```<br> ```cmake --build .```          |




```bash
# Run the game
./aim
