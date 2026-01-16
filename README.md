# SDL Paint

A simple paint / drawing program written in **C using SDL2** on Windows.

This project demonstrates:
- Direct pixel drawing
- Persistent canvas rendering
- Brush preview vs paint
- Mouse input handling
- Smooth stroke interpolation

---

## Controls

- **Left Click**  
  Draw with the current brush

- **Left Click + Drag**  
  Draw continuous strokes

- **Right Click**  
  Cycle through brush colors

- **Mouse Wheel Scroll**  
  Increase / decrease brush size

---

## Notes

- The brush shows a preview of the current color and size  
- Paint is drawn permanently to the canvas until the program is closed  

---

## Build (MinGW + SDL2)

Example make command:
mingw32-make all

Make sure `SDL2.dll` is in the same directory as `paint.exe`.

---

## Dependencies

- SDL2  
- MinGW-w64 (Windows)

---

## License

This project is for learning and experimentation.
