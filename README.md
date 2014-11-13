# Super Mario in C

This project implements a simplified version of Super Mario written entirely in C, without any third-party rendering engines.

## ⚙️ Prerequisites

To compile and run this program, you will need the following:
*   **DOSBox:** An emulator for legacy DOS programs. 
*   **Turbo C:** This is required for compilation and is already included in the project under the `TC` folder.

## 🚀 Compilation & Execution

Follow these steps to compile and run the game within DOSBox:

1. **Install DOSBox** on your system.

2. **Mount the project directory:** Open DOSBox and mount the root folder of this project as your `C:` drive. Replace `/path/to/project` with your actual file path:
   ```dos
   mount C: /path/to/project
   ```

3. **Navigate to the project folder:** Switch to the `C:` drive and enter the `GDC` directory:
   ```dos
   C:
   cd C:/GDC
   ```

4. **Compile the code:** Run the included batch file to compile the project:
   ```dos
   compile.bat
   ```

5. **Launch the game:**
   ```dos
   GDC.EXE
   ```

## 🎮 Emulation Adjustments

Because this runs in a DOS emulator, you will need to adjust the performance settings for the game to run smoothly:

*   **Adjust CPU Cycles:** Once the game is running, use **Ctrl+F12** (to increase) and **Ctrl+F11** (to decrease) to adjust the DOSBox CPU cycles to approximately **20,000**.
*   **Loading Time:** Please be patient! It will take about **30 seconds** for the game to fully load.

## 👥 Developers

*   Yan Chen
*   Liang Guo
*   Bilin Dai
