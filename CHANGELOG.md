# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2024-05-15

### Added
- Initial release of LedMatrix-FrameScroller library
- Full bidirectional scrolling support (HOR and VER)
- Multiple loop modes (MONODIR and BIDIR)
- 8 grayscale brightness levels with three precision modes (MONO, EASY, FULL)
- Smart frame positioning (START, CENTER, END)
- Dynamic frame handling (frames larger, smaller, or equal to matrix)
- Forward (FWDIR) and reverse (RWDIR) start modes
- Comprehensive logging system (DEBUG, INFO, NONE modes)
- Complete API with 20+ methods and queries
- Extensive documentation and examples
- Test suite covering all animation modes and configurations
- MIT License

### Features
- Support for 13×8 LED matrices
- Arduino UNO R4 compatibility
- Easy-to-use API with chainable configuration
- Efficient memory usage with PROGMEM support
- Non-blocking animation updates
- Frame state tracking and loop counting
- Multiple simultaneous instances support (with multiple matrices)

### Documentation
- Comprehensive README with usage examples
- Full API reference with parameter descriptions
- Scrolling behavior reference table
- Troubleshooting guide
- Logging output examples
- Performance notes
- Contributing guidelines

### Testing
- Complete test suite (led_m.ino)
- Tests for all animation modes
- Frame size variation tests (larger, smaller, equal)
- Position and direction combination tests
- Loop mode validation tests

---

## Future Roadmap

### Planned for v1.1.0
- Pattern-based frame generation helpers
- Text scrolling utilities
- Animation composition (multiple frames in sequence)
- Custom easing functions
- Frame caching optimization

### Potential Additions
- Support for larger matrix sizes
- Color support (RGB LEDs)
- Frame blending/transitions
- Event callbacks (on loop complete, etc.)
- Hardware acceleration support
