# SDL Paint

A simple paint / drawing program written in **C using SDL2** on Windows.

Features:
- Brush preview / paint
- Mouse input handling
- Smooth brush strokes

Plans:
- Erase mode
- Other platforms (MacOS / Linux)
- Better color switching

---

## Controls

- **Left Click**  
  Draw with the current brush

- **Left Click + Drag**  
  Draw continuous strokes

- **Right Click**  
  Cycle through brush colors (Black, Red, Green, Blue)

- **Mouse Wheel Scroll**  
  Increase / decrease brush size

- **Any key**
  Close program

---

## Notes

- The brush shows a preview of the current color and size
- Paint is drawn permanently to the canvas until the program is closed

---

## Build (MinGW + SDL2)

You can just download the latest release instead of building, but if you prefer building:

Example make command:
mingw32-make all

Debug make command (show command prompt):
mingw32-make debug

Make sure `SDL2.dll` is in the same directory as `paint.exe` for the exe to run.

---

## Dependencies

- SDL2  
- MinGW-w64 (Windows)
