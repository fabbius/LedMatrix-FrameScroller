#ifndef FRAME_SCROLLER_H
#define FRAME_SCROLLER_H

#include <Arduino.h>
#include "Arduino_LED_Matrix.h"


//include if you want test methods
#include "FrameScroll_Test_frames.h"

#if __has_include("FrameScroll_Test_frames.h")
#define is_use_test 1
#else
#define is_use_test 0
#endif
// Tipo per la funzione di logging
typedef void (*LogFunction)(const char*);

class FrameScroller {
public:


  // Stato dei test
  enum TestMode {
    TEST_WIDE_HOR_BIDIR,    // Frame largo, HOR, BIDIR
    TEST_WIDE_HOR_MONODIR,  // Frame largo, HOR, MONODIR
    TEST_TALL_VER_BIDIR,    // Frame alto, VER, BIDIR
    TEST_TALL_VER_MONODIR,  // Frame alto, VER, MONODIR
    TEST_SMALL_POS_START,   // Frame piccolo, posizione START
    TEST_SMALL_POS_CENTER,  // Frame piccolo, posizione CENTER
    TEST_SMALL_POS_END,     // Frame piccolo, posizione END
    TEST_EXACT_NOSCROLL,    // Frame uguale matrice, no scroll
    TEST_DANGER_HOR         // Frame danger, HOR scroll
  };

  enum Direction {
    HOR,  // Orizzontale
    VER   // Verticale
  };

  enum ModeLoop {
    MONODIR,  // Una sola direzione
    BIDIR     // Andata e ritorno
  };

  enum StartMode {
    FWDIR,  // Forward (da sinistra/alto)
    RWDIR   // Reverse (da destra/basso)
  };

  enum Position {
    START,  // Inizio della matrice
    END,    // Fine della matrice
    CENTER  // Centro della matrice
  };

  enum PixType {
    MONO,  // 0..1 (1 bit)
    EASY,  // 0..7 (3 bit)
    FULL   // 0..255 (8 bit)
  };

  enum LogType {
    NONE,  // Nessun logging
    INFO,  // Messaggi informativi
    DEBUG  // Messaggi di debug dettagliati
  };

  FrameScroller(Arduino_LED_Matrix* matrix);

  // Configurazione
  void setDirection(Direction dir);
  void setModeLoop(ModeLoop mode);
  void setStartMode(StartMode mode);
  void setWaitFrame(uint16_t ms);
  void setPosition(Position pos);
  void setFrame(const uint8_t* frameData, size_t width, size_t height, PixType pixType, uint8_t n_loops = 0);
  void setLog(LogType type, LogFunction func);

  // Controllo
  void play(uint8_t n_loops = 0);
  void pause();
  void stop();

  // Update
  void ticker();
  void show();

  // Query stato
  bool isPlaying() const {
    return _playing;
  }
  bool isPaused() const {
    return _paused;
  }
  bool isLoopTerminated() const {
    return _loop_terminated;
  }
  uint16_t getWaitFrame() const {
    return _wait_frame_ms;
  }
  uint8_t getLoopsCompleted() const {
    return _loops_completed;
  }
  uint8_t getLoopsCount() const {
    return _n_loops;
  }
#if is_use_test
  TestMode currentTest;
  unsigned long testStartTime;
  const unsigned long testDuration;
  void startTest(TestMode test, size_t n_loops);
#endif
private:
  Arduino_LED_Matrix* _matrix;

  // Configurazione
  Direction _direction;
  ModeLoop _mode_loop;
  StartMode _start_mode;
  uint16_t _wait_frame_ms;
  Position _position;

  // Frame
  uint8_t* _frame_data;
  size_t _frame_width;
  size_t _frame_height;
  PixType _pix_type;
  uint8_t _n_loops;
  uint8_t _loops_completed;

  // Stato
  bool _playing;
  bool _paused;
  bool _loop_terminated;  // Flag per indicare che i loop sono finiti
  uint32_t _last_update;
  int16_t _current_offset;  // Offset attuale in pixel
  bool _forward;            // Direzione attuale nel movimento BIDIR

  // Logging
  LogType _log_type;
  LogFunction _log_func;

  // Matrice di visualizzazione (8x13)
  static const uint8_t MATRIX_WIDTH = 13;
  static const uint8_t MATRIX_HEIGHT = 8;
  static const uint8_t MATRIX_SIZE = MATRIX_WIDTH * MATRIX_HEIGHT;
  uint8_t _display_buffer[MATRIX_SIZE];

  // Calcoli interni
  void updateOffset();
  void extractWindow();
  int16_t getMaxOffset();
  int16_t getStartOffset();
  int16_t getPositionOffsetX();
  int16_t getPositionOffsetY();
  void log(LogType level, const char* message);
};

#endif