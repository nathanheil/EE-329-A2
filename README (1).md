# EE 329 – Lab A2: 4×4 Keypad Interface (STM32L4)

This project implements a **4×4 matrix keypad interface** on the STM32L4 NUCLEO‑L4A6ZG board.  
It scans the keypad using row/column GPIO multiplexing, debounces key presses, and displays the key value on **LEDs (PC0–PC3)**.

---

## 🔧 Project Overview
- **Keypad scanning** on PD0–PD7 (rows = inputs, columns = outputs)
- **Debounced key detection** for stable input
- **Keymap** includes digits 0–9 plus A–D, * and #
- **LED output** on Port C pins PC0–PC3 to represent pressed key value
- Combines **STM32 HAL** initialization with **direct register-level GPIO control**

---

## 🧠 How it Works
1. `Keypad_Config()` configures PD0–PD3 as inputs with pulldowns and PD4–PD7 as outputs.  
2. `Keypad_IsKeyPressed()`:
   - Iterates columns, driving one high at a time.  
   - Checks rows for a high signal, then double-checks (debounce).  
   - Returns the mapped integer value from the keymap.  
   - Returns **-1** if no key is pressed.  
3. `main()` configures PC0–PC3 as LED outputs and continuously polls the keypad.  
   - On valid key press, clears LEDs and sets them to the mapped value.  

---

## 📐 Wiring (NUCLEO‑L4A6ZG)
- **Rows (inputs):** PD0, PD1, PD2, PD3  
- **Columns (outputs):** PD4, PD5, PD6, PD7  

See [`docs/KEYPAD_WIRING.md`](docs/KEYPAD_WIRING.md) for full details.

---

## 🔢 Keymap
|     | Row0 | Row1 | Row2 | Row3 |
|-----|------|------|------|------|
| Col0 | 1 | 4 | 7 | * |
| Col1 | 2 | 5 | 8 | 0 |
| Col2 | 3 | 6 | 9 | # |
| Col3 | A | B | C | D |

- `*` = 14, `0` = 0, `#` = 15, A/B/C/D = 10–13.  
- CSV and visual diagram are provided in [`docs/`](docs).

---

## 🚀 Getting Started
1. Open in **STM32CubeIDE** or import the sources into a new project.  
2. Build and flash to **NUCLEO‑L4A6ZG** (MSI @ 4 MHz to AHB2 as configured).  
3. Connect a 4×4 keypad to **PD0–PD7**.  
4. Observe LEDs on **PC0–PC3** reflect the pressed key.  

---

## 📂 Repository Structure
```
.
├── main.c / main.h          # Application entry, LED control
├── keypad.c / keypad.h      # Keypad driver (config + scan)
├── .gitignore               # Ignore build/IDE artifacts
├── README.md                # Project description
└── docs/
   ├── KEYPAD_WIRING.md      # Wiring guide
   ├── KEYMAP.csv            # 4×4 keymap values
   ├── KEYMAP.md             # Markdown table of keymap
   └── KEYMAP_GRID.png       # Visual diagram of keymap
```

---

## ✅ Learning Outcomes
- Embedded interfacing of a **matrix keypad**  
- **Debouncing** mechanical inputs  
- GPIO control at both **HAL** and **register** level  
- Professional repository documentation and structure  

---

## 📜 License
This project is licensed under the MIT License – see [LICENSE](LICENSE).

---

👤 **Author:** Nathan Heil  
📅 **Course:** EE‑329 (Embedded Systems)  
