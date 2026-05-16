# LedMatrix-FrameScroller

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Arduino](https://img.shields.io/badge/Arduino-UNO%20Q-brightgreen)](https://www.arduino.cc/)
[![GitHub release](https://img.shields.io/github/v/release/fabbius/LedMatrix-FrameScroller)](https://github.com/fabbius/LedMatrix-FrameScroller/releases)
[![GitHub stars](https://img.shields.io/github/stars/fabbius/LedMatrix-FrameScroller?style=social)](https://github.com/fabbius/LedMatrix-FrameScroller)
[![GitHub forks](https://img.shields.io/github/forks/fabbius/LedMatrix-FrameScroller?style=social)](https://github.com/fabbius/LedMatrix-FrameScroller)

A powerful and flexible Arduino library for controlling onboard 13×8 LED matrices of UNO-Q, with smooth frame scrolling animations in both horizontal and vertical directions.

## Features

- **Bidirectional Scrolling**
  - Horizontal (HOR) and vertical (VER) animations
  - Forward (FWDIR) and reverse (RWDIR) directions
  
- **Multiple Loop Modes**
  - `MONODIR`: Single direction (forward or reverse)
  - `BIDIR`: Bidirectional (back and forth)
  
- **8 Grayscale Levels**: Full control over pixel brightness with three precision modes (1-bit, 3-bit, 8-bit)

- **Smart Positioning**
  - `START`: Frame at start of matrix (top/left)
  - `CENTER`: Frame centered on matrix
  - `END`: Frame at end of matrix (bottom/right)

- **Dynamic Frame Handling**
  - Frames larger than matrix: true scrolling animation
  - Frames smaller than matrix: oscillation/positioning
  - Frames equal to matrix: static display

- **Comprehensive Logging**: DEBUG, INFO, or NONE modes for easy troubleshooting

- **Easy API**: Intuitive setters and straightforward control methods

## Hardware Requirements

- **Arduino UNO R4** (or compatible board with `Arduino_LED_Matrix` support)
- **13×8 LED Matrix** (integrated or external)

## Installation

### Via Arduino IDE (Recommended)

1. Download this repository as ZIP
2. Sketch → Include Library → Add .ZIP Library
3. Select the downloaded file
4. Restart Arduino IDE

### Manual Installation

1. Clone or download this repository
2. Copy the entire folder to your Arduino libraries directory:
   - **Linux/Mac**: `~/Arduino/libraries/`
   - **Windows**: `Documents\Arduino\libraries\`
3. Restart Arduino IDE

## Quick Start

```cpp
#include "FrameScroller.h"

Arduino_LED_Matrix matrix;
FrameScroller scroller(&matrix);

// Your frame data (13 pixels wide × 8 pixels tall)
uint8_t my_frame[13 * 8] = {
  0,0,0,7,7,7,0,0,0,0,0,0,0,
  0,0,7,7,7,7,7,0,0,0,0,0,0,
  // ... rest of frame data
};

void setup() {
  Serial.begin(115200);
  matrix.begin();
  
  // Configure animation
  scroller.setFrame(my_frame, 13, 8, FrameScroller::EASY);
  scroller.setDirection(FrameScroller::HOR);
  scroller.setModeLoop(FrameScroller::BIDIR);
  scroller.setStartMode(FrameScroller::FWDIR);
  scroller.setPosition(FrameScroller::START);
  scroller.setWaitFrame(50);
  
  scroller.play();  // Start animation
}

void loop() {
  scroller.ticker();   // Update animation
  scroller.show();     // Display frame
  delay(scroller.getWaitFrame());
}
```

## API Reference

### Configuration Methods

#### `setDirection(Direction dir)`
Controls animation direction:
- `HOR`: Horizontal scrolling (left/right movement)
- `VER`: Vertical scrolling (up/down movement)

#### `setModeLoop(ModeLoop mode)`
Controls loop behavior:
- `MONODIR`: Animation goes in one direction only
- `BIDIR`: Animation oscillates (back and forth)

#### `setStartMode(StartMode mode)`
Controls initial direction of movement:
- `FWDIR`: Forward (left/top)
- `RWDIR`: Reverse (right/bottom)

#### `setPosition(Position pos)`
Controls frame positioning on matrix:
- `START`: Top/left position
- `CENTER`: Centered on matrix
- `END`: Bottom/right position

#### `setWaitFrame(uint16_t ms)`
Sets delay between animation frames in milliseconds.
- Lower value = faster animation
- Higher value = slower animation

#### `setFrame(const uint8_t* frameData, size_t width, size_t height, PixType pixType, uint8_t n_loops = 0)`
Loads a frame for animation:
- `frameData`: Pointer to frame pixel array (row-major format)
- `width`, `height`: Frame dimensions in pixels
- `pixType`: Pixel color depth:
  - `MONO`: 1-bit (on/off only)
  - `EASY`: 3-bit (8 brightness levels)
  - `FULL`: 8-bit (256 brightness levels)
- `n_loops`: Number of animation loops (0 = infinite)

#### `setLog(LogType type, LogFunction func)`
Enables debug logging:
- `type`: `NONE` (disabled), `INFO` (basic info), `DEBUG` (detailed)
- `func`: Callback function for log messages

Example:
```cpp
void logCallback(const char* msg) {
  Serial.println(msg);
}
scroller.setLog(FrameScroller::INFO, logCallback);
```

### Control Methods

- **`play(uint8_t n_loops = 0)`**: Start or resume animation (optionally change loop count)
- **`pause()`**: Pause animation at current frame
- **`stop()`**: Stop animation and clear display
- **`ticker()`**: Update animation state (call once per loop iteration)
- **`show()`**: Render current frame to LED matrix

### Query Methods

- **`isPlaying()`**: Returns `true` if animation is running
- **`isPaused()`**: Returns `true` if animation is paused
- **`isLoopTerminated()`**: Returns `true` when animation completes all loops
- **`getLoopsCompleted()`**: Returns current loop count
- **`getLoopsCount()`**: Returns total loops configured
- **`getWaitFrame()`**: Returns current frame delay in milliseconds

## Frame Format

Frames are stored as linear `uint8_t` arrays in **row-major** order:

```
Frame[width * height] = {
  row0_col0, row0_col1, ..., row0_colN,
  row1_col0, row1_col1, ..., row1_colN,
  ...
}
```

**Pixel Values** (for EASY mode):
- `0`: Off
- `1-6`: Intermediate brightness
- `7`: Maximum brightness

## Usage Examples

### Horizontal Scrolling with Large Frame

Animate a 30×8 frame scrolling left and right across the matrix:

```cpp
uint8_t wide_frame[30 * 8] = { /* frame data */ };

scroller.setFrame(wide_frame, 30, 8, FrameScroller::EASY, 3);
scroller.setDirection(FrameScroller::HOR);
scroller.setModeLoop(FrameScroller::BIDIR);
scroller.setStartMode(FrameScroller::FWDIR);
scroller.setPosition(FrameScroller::START);  // Top of matrix
scroller.play(3);  // 3 complete loops
```

### Vertical Scrolling with Tall Frame

Animate a 13×16 frame scrolling up and down:

```cpp
uint8_t tall_frame[13 * 16] = { /* frame data */ };

scroller.setFrame(tall_frame, 13, 16, FrameScroller::EASY, 2);
scroller.setDirection(FrameScroller::VER);
scroller.setModeLoop(FrameScroller::BIDIR);
scroller.setPosition(FrameScroller::CENTER);  // Centered vertically
scroller.play(2);  // 2 complete loops
```

### Static Frame Positioning

Display a smaller frame (8×6) centered on the matrix without scrolling:

```cpp
uint8_t small_frame[8 * 6] = { /* frame data */ };

scroller.setFrame(small_frame, 8, 6, FrameScroller::EASY);
scroller.setPosition(FrameScroller::CENTER);  // Centered on matrix
scroller.play();  // Infinite static display
```

### Single Direction Scrolling

Scroll frame in one direction only (no return):

```cpp
scroller.setFrame(frame_data, 20, 8, FrameScroller::EASY, 1);
scroller.setDirection(FrameScroller::HOR);
scroller.setModeLoop(FrameScroller::MONODIR);  // Single direction
scroller.setStartMode(FrameScroller::FWDIR);   // Move left to right
scroller.play();
```

## Scrolling Behavior Reference

### Horizontal (HOR)
| Frame Size | Behavior | Position Effect |
|------------|----------|-----------------|
| > 13 px | Frame scrolls left/right | Vertical position (TOP/CENTER/BOTTOM) |
| = 13 px | No movement | No effect |
| < 13 px | Frame oscillates | Horizontal positioning (LEFT/CENTER/RIGHT) |

### Vertical (VER)
| Frame Size | Behavior | Position Effect |
|------------|----------|-----------------|
| > 8 px | Frame scrolls up/down | Horizontal position (LEFT/CENTER/RIGHT) |
| = 8 px | No movement | No effect |
| < 8 px | Frame oscillates | Vertical positioning (TOP/CENTER/BOTTOM) |

## Testing

The library includes a comprehensive test suite. Load `led_m.ino` to test all animation modes:

- Horizontal scrolling with large frames
- Vertical scrolling with tall frames
- Positioning small frames
- Single direction animations
- And more...

## Logging Output Example

```
========================================
TEST: Wide frame, HOR, BIDIR
Frame set: 30x8, loops: 2
Config: Dir=HOR (scroll orizzontale), Mode=BIDIR (andata/ritorno), Start=FWDIR, Pos=START
Scroll: max_offset=17 (SCROLL ENABLED)
HOR: Frame 30 px wide, Matrix 13 px -> oscillazione di 17 px orizzontale. Vertical pos: START
Animation started: offset=0, max_offset=17, start_mode=FWDIR
Direction changed: reverse
Loop 1 completed
Loop 2 completed
Animation finished
```

## Troubleshooting

### Animation doesn't start
- Check that `setFrame()` is called before `play()`
- Verify frame data pointer is valid
- Ensure `matrix.begin()` is called in setup

### Animation runs forever
- Check `n_loops` parameter in `setFrame()` (0 = infinite)
- Use `getLoopsCompleted()` to debug loop counting
- Enable `DEBUG` logging to see detailed state changes

### Frame appears incorrect
- Verify frame array size matches width × height
- Check pixel values are in valid range (0-7 for EASY mode)
- Enable `INFO` logging to confirm frame dimensions

### Animation too fast/slow
- Adjust `setWaitFrame()` value
- Increase value for slower animation
- Decrease value for faster animation

## Performance Notes

- **Memory**: Frame arrays stored in PROGMEM for large frames
- **CPU**: Lightweight animation loop, safe for realtime applications
- **Power**: LED brightness affects power consumption

## API Stability

This is version 1.0.0. The API is stable and suitable for production use.

## Contributing

Contributions are welcome! Please:
1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit changes (`git commit -m 'Add amazing feature'`)
4. Push to branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see [LICENSE](LICENSE) file for details.

## Credits

**Library Design & Implementation**: [Your Name]

Special thanks to Arduino and the open-source community.

## Support

For issues, questions, or suggestions:
- Open an [Issue](https://github.com/yourusername/LedMatrix-FrameScroller/issues) on GitHub
- Check existing issues and discussions first

---

**Happy Animating!** 🎨✨

