# 🎯 AIM

<div >
  <img src="https://img.shields.io/badge/C-555555?style=for-the-badge&logo=c&logoColor=white" height="50" />
  <img src="https://img.shields.io/badge/CMake-v3.25-blue?style=for-the-badge&logo=cmake&logoColor=white" height="50" />
  <img src="https://img.shields.io/badge/raylib-v5.0-red?style=for-the-badge&logo=raylib&logoColor=white" height="50" />
</div>

**AIM** is a **fast, minimalistic, and lightweight aim trainer** built with **C** and **raylib**.  
It’s designed for **pure aim practice** without distractions, featuring classic map types like **Gridshot**, **Floatshot**, and **Tracking**, powered by a **custom map engine** that dynamically loads user created maps.  
Plus, it offers an **extremely customizable color scheme** for targets, crosshair, and HUD.

---

##  Demos

**Gameplay (Gridshot / Floatshot / Tracking)**  
----
<img src="https://github.com/xavierhampton/AIM/blob/main/images/MapsDemo.gif"/>

**Settings Menu (Color customization + Map selection)**  
----
<img src="https://github.com/xavierhampton/AIM/blob/main/images/SettingsDemo.gif"/>

 **Color Variety (Examples of unique player-tailored color schemes)**  
 ----
<img src="https://github.com/xavierhampton/AIM/blob/main/images/colors.png"/>


---

## ✨ Features

- Lightweight and fast — **designed for smooth aim practice without unnecessary features**  
- Minimal, centered HUD with **time, hits, and accuracy**  
- **Extremely customizable colors** for targets, crosshair, and HUD  
- Map-based training with **dynamic target engines**  
- Multiple game modes for varied aim practice  
- Built with **raylib** for cross-platform support  
- Extremely xtendable  

---

##  Map Types

- **Gridshot** – shoot targets that appear in a grid pattern.  
- **Float Shot** – shoot moving airborne targets that drift across the map.
- **Float Track** – keep your crosshair locked on moving airborne targets that drift across the map.  
- **Tracking** – keep your crosshair locked on a moving target to build accuracy over time.  

---

##  Custom Game Engine & Map Selector

AIM runs on a **custom-built engine** designed to load and manage multiple "maps" dynamically. Each map can define:

- Target count, size, and health  
- Target movement patterns (floating, tracking, or grid-based)  
- Map layout parameters (X/Y variation, gaps, min/max Z positions)  

Players can choose their preferred map with a **map selector UI**:

- Scroll through available maps  
- Preview map info: type, target size, count, speed, and layout  
- Instantly switch maps for a new training experience  

This system allows endless combinations of challenges and keeps your practice fresh.

---

## 🛠️ Build & Run

### Prerequisites
- **C compiler** (GCC, Clang, or MSVC)
- **raylib** installed (v5.0+, same directory as project)  
- **CMake** 


### Build Instructions
```bash
# Clone the repository
git clone https://github.com/yourusername/AIM.git
cd AIM

# Create build directory
mkdir build && cd build

# Run CMake
cmake ..
make

# Run the game
./aim
