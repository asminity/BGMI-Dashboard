# 🎮 BGMI Dashboard — C++ OOP Project

A **console-based BGMI-like dashboard** built in **C++17**, showcasing advanced **Object-Oriented Programming (OOP)** principles such as encapsulation, inheritance, composition, and polymorphism — along with **STL containers** and **file handling** for persistent data management.

---

## ✨ Overview

This project simulates a simplified version of **BGMI (Battlegrounds Mobile India)** gameplay logic within the terminal.  
It demonstrates how C++ can be used to design an interactive dashboard for players, including features like profiles, inventory, matches, and leaderboards — all backed by file-based data persistence.

---

## 🧩 Key Features

### 👤 Authentication
- Register and login system  
- Data stored securely in `data/users.txt`

### 🧍 Player Profile
- Tracks **Level**, **XP**, **KD ratio**, **Rank**, **Matches**, **Wins**, **Kills**, and **Damage**
- Automatically levels up when XP exceeds thresholds

### ⚔️ Match Simulation
- Randomized match outcomes (kills, damage, rank)
- Supports different modes and maps
- Stores match history using a **stack-based system**

### 🎒 Inventory System
- Central catalog for all available items
- Player-specific inventory unlocked based on level
- Persistent storage across sessions

### 🏆 Achievements
- Uses **polymorphism** for dynamic achievement tracking
- Separate achievement types (e.g., kill-based, win-based)
- XP rewards upon completion

### 🥇 Leaderboard
- Sortable by **Rank**, **Level**, or **KD ratio**
- Uses STL containers and algorithms for sorting and ranking

### 💾 File Handling
- Persistent player, match, and inventory data
- Auto-generated structure on first run

---

## ⚙️ Build Instructions

### 🧱 Requirements
- **C++17 or later**
- **MinGW (Windows)** or any compiler supporting C++17

### 🪟 Windows PowerShell
```powershell
# Basic build
g++ -std=c++17 -O2 -o bgmi *.cpp

# Or using the provided script
./build.ps1

# Run the application
./bgmi.exe