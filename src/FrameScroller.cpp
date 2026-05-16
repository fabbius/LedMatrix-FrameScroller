#include "./FrameScroller.h"

FrameScroller::FrameScroller(Arduino_LED_Matrix* matrix)
  : _matrix(matrix),
    _direction(HOR),
    _mode_loop(BIDIR),
    _start_mode(FWDIR),
    _wait_frame_ms(50),
    _position(START),
    _frame_data(nullptr),
    _frame_width(0),
    _frame_height(0),
    _pix_type(EASY),
    _n_loops(0),
    _loops_completed(0),
    _playing(false),
    _paused(false),
    _loop_terminated(false),
    _last_update(0),
    _current_offset(0),
    _forward(true),
    _log_type(NONE),
    _log_func(nullptr)
#if is_use_test
    ,
    currentTest(TEST_WIDE_HOR_BIDIR), testStartTime(0), testDuration(5000)
#endif
{
  memset(_display_buffer, 0, MATRIX_SIZE);
}

void FrameScroller::setDirection(Direction dir) {
  _direction = dir;
}

void FrameScroller::setModeLoop(ModeLoop mode) {
  _mode_loop = mode;
}

void FrameScroller::setStartMode(StartMode mode) {
  _start_mode = mode;
}

void FrameScroller::setWaitFrame(uint16_t ms) {
  _wait_frame_ms = ms;
}

void FrameScroller::setPosition(Position pos) {
  _position = pos;
}

void FrameScroller::setLog(LogType type, LogFunction func) {
  _log_type = type;
  _log_func = func;
  if (_log_type != NONE && _log_func != nullptr) {
    log(INFO, "Logging enabled");
  }
}

void FrameScroller::setFrame(const uint8_t* frameData, size_t width, size_t height, PixType pixType, uint8_t n_loops) {
  _frame_data = (uint8_t*)frameData;
  _frame_width = width;
  _frame_height = height;
  _pix_type = pixType;
  _n_loops = n_loops;
  _loops_completed = 0;

  // Configura la matrice con il numero di bit di grayscale appropriato
  switch (_pix_type) {
    case MONO:
      _matrix->setGrayscaleBits(1);
      break;
    case EASY:
      _matrix->setGrayscaleBits(3);
      break;
    case FULL:
      _matrix->setGrayscaleBits(8);
      break;
  }

  char msg[200];
  snprintf(msg, sizeof(msg), "Frame set: %ux%u, loops: %s",
           (unsigned)width, (unsigned)height,
           n_loops == 0 ? "infinite" : (char*)String(n_loops).c_str());
  log(INFO, msg);

  // Log dettagliato della configurazione
  const char* dir_str = (_direction == HOR) ? "HOR (scroll orizzontale)" : "VER (scroll verticale)";
  const char* mode_str = (_mode_loop == MONODIR) ? "MONODIR (una direzione)" : "BIDIR (andata/ritorno)";
  const char* start_str = (_start_mode == FWDIR) ? "FWDIR" : "RWDIR";
  const char* pos_str = (_position == START) ? "START" : (_position == CENTER) ? "CENTER"
                                                                               : "END";

  snprintf(msg, sizeof(msg), "Config: Dir=%s, Mode=%s, Start=%s, Pos=%s",
           dir_str, mode_str, start_str, pos_str);
  log(INFO, msg);

  // Calcola l'oscillazione
  int16_t max_offset = getMaxOffset();
  snprintf(msg, sizeof(msg), "Scroll: max_offset=%d (%s)", max_offset,
           max_offset == 0 ? "NO SCROLL - frame fisso" : "SCROLL ENABLED");
  log(INFO, msg);

  if (_direction == HOR && max_offset > 0) {
    snprintf(msg, sizeof(msg), "HOR: Frame %u px wide, Matrix 13 px -> oscillazione di %d px orizzontale. "
                               "Vertical pos: %s",
             (unsigned)width, max_offset, pos_str);
    log(INFO, msg);
  } else if (_direction == VER && max_offset > 0) {
    snprintf(msg, sizeof(msg), "VER: Frame %u px tall, Matrix 8 px -> oscillazione di %d px verticale. "
                               "Horizontal pos: %s",
             (unsigned)height, max_offset, pos_str);
    log(INFO, msg);
  } else if (max_offset == 0) {
    const char* scroll_dir = (_direction == HOR) ? "orizzontale" : "verticale";
    snprintf(msg, sizeof(msg), "NO SCROLL: Frame posizionato fisso, %s position=%s", scroll_dir, pos_str);
    log(INFO, msg);
  }

  // Reset dell'animazione
  _current_offset = getStartOffset();
}

void FrameScroller::play(uint8_t n_loops) {
  if (_frame_data == nullptr) {
    log(INFO, "Cannot play: no frame set");
    return;  // Nessun frame impostato
  }

  // Se n_loops è diverso da 0, cambia il valore
  if (n_loops != 0) {
    _n_loops = n_loops;
    char msg[50];
    snprintf(msg, sizeof(msg), "Loops changed to: %u", n_loops);
    log(INFO, msg);
  }

  _forward = (_start_mode == FWDIR);
  _loops_completed = 0;
  _loop_terminated = false;
  _playing = true;
  _paused = false;
  _last_update = millis();
  _current_offset = getStartOffset();

  extractWindow();

  char msg[150];
  snprintf(msg, sizeof(msg), "Animation started: offset=%d, max_offset=%d, start_mode=%s, f_w=%d, f_h=%d, direction: %s",
           _current_offset, getMaxOffset(),
           (_start_mode == FWDIR) ? "FWDIR" : "RWDIR", _frame_width, _frame_height, _forward ? "forward" : "reverse");
  log(INFO, msg);

  show();
  delay(getWaitFrame());
}

void FrameScroller::pause() {
  _paused = true;
}

void FrameScroller::stop() {
  _playing = false;
  _paused = false;
  _loop_terminated = false;
  _current_offset = getStartOffset();
  memset(_display_buffer, 0, MATRIX_SIZE);
  _matrix->clear();
  log(INFO, "Animation stopped");
}

void FrameScroller::ticker() {
  if (!_playing || _paused || _frame_data == nullptr) {
    return;
  }

  uint32_t now = millis();
  if (now - _last_update < _wait_frame_ms) {
    return;  // Non è ancora passato abbastanza tempo
  }

  _last_update = now;

  // Aggiorna l'offset in base alla configurazione
  updateOffset();

  // Estrae la finestra dal frame e riempie il buffer
  extractWindow();
}

void FrameScroller::show() {
  if (_frame_data != nullptr && _playing) {
    log(DEBUG, "show");
    _matrix->draw(_display_buffer);
    if (_forward)
     _current_offset++;
    else
     _current_offset--;
  }

  // Se i loop sono terminati, ferma completamente
  if (_loop_terminated && _playing) {
    _playing = false;
    log(INFO, "Animation finished");
  }
}

void FrameScroller::updateOffset() {
  int16_t max_offset = getMaxOffset();
  int16_t start_offset = getStartOffset();

  // Inizializza la direzione al primo avvio

  if (_direction == HOR) {
    if (_mode_loop == MONODIR) {
      // Muove in una sola direzione
      if (_start_mode == FWDIR) {
       //_current_offset++;
        char dbg[40];
        snprintf(dbg, sizeof(dbg), "MONODIR Offset HOR: %d (forward)", _current_offset);
        log(DEBUG, dbg);

        if (_current_offset >= max_offset) {
          _loops_completed++;
          char info[50];
          snprintf(info, sizeof(info), "MONODIR HOR Loop %u completed6 (forward)", _loops_completed);
          log(INFO, info);

          if (_n_loops == 0 || _loops_completed < _n_loops) {
            _current_offset = start_offset;  // Ricomincia
          } else {
            _loop_terminated = true;
          }
        }
      } else {
       //_current_offset--;
        char dbg[40];
        snprintf(dbg, sizeof(dbg), "MONODIR Offset HOR: %d (reverse)", _current_offset);
        log(DEBUG, dbg);

        if (_current_offset <= start_offset) {
          _loops_completed++;
          char info[50];
          snprintf(info, sizeof(info), "MONODIR HOR Loop %u completed5 (reverse)", _loops_completed);
          log(INFO, info);

          if (_n_loops == 0 || _loops_completed < _n_loops) {
            _current_offset = max_offset;  // Ricomincia
          } else {
            _loop_terminated = true;
          }
        }
      }
    } else {
      // Andata e ritorno (BIDIR)
      if (_forward) {
       //_current_offset++;
        char dbg[40];
        snprintf(dbg, sizeof(dbg), "BIDIR Offset HOR: %d (forward)", _current_offset);
        log(DEBUG, dbg);

        if (_current_offset >= max_offset) {
          _forward = false;
          log(INFO, "BIDIR HOR Direction changed: reverse");
        }
      } else {
       //_current_offset--;
        char dbg[40];
        snprintf(dbg, sizeof(dbg), "BIDIR Offset HOR: %d (reverse)", _current_offset);
        log(DEBUG, dbg);

        if (_current_offset <= start_offset) {
          _loops_completed++;
          char info[50];
          snprintf(info, sizeof(info), "BIDIR HOR Loop %u completed4 (reverse)", _loops_completed);
          log(INFO, info);

          if (_n_loops == 0 || _loops_completed < _n_loops) {
            _forward = true;
            log(INFO, "Direction changed: forward");
          } else {
            _loop_terminated = true;
          }
        }
      }
    }
  } else {
    // Verticale (VER)
    if (_mode_loop == MONODIR) {
      if (_start_mode == FWDIR) {
       //_current_offset++;
        char dbg[40];
        snprintf(dbg, sizeof(dbg), "MONODIR Offset VER: %d (forward)", _current_offset);
        log(DEBUG, dbg);

        if (_current_offset >= max_offset) {
          _loops_completed++;
          char info[50];
          snprintf(info, sizeof(info), "MONODIR VER Loop %u completed3 (forward)", _loops_completed);
          log(INFO, info);

          if (_n_loops == 0 || _loops_completed < _n_loops) {
            _current_offset = start_offset;
          } else {
            _loop_terminated = true;
          }
        }
      } else {
       //_current_offset--;
        char dbg[40];
        snprintf(dbg, sizeof(dbg), "MONODIR Offset VER: %d  (reverse)", _current_offset);
        log(DEBUG, dbg);

        if (_current_offset <= start_offset) {
          _loops_completed++;
          char info[50];
          snprintf(info, sizeof(info), "MONODIR VER Loop %u completed2 (reverse)", _loops_completed);
          log(INFO, info);

          if (_n_loops == 0 || _loops_completed < _n_loops) {
            _current_offset = max_offset;
          } else {
            _loop_terminated = true;
          }
        }
      }
    } else {
     //_current_offset++;
      // BIDIR verticale
      if (_forward) {
        char dbg[40];
        snprintf(dbg, sizeof(dbg), "BIDIR Offset VER: %d (forward)", _current_offset);
        log(DEBUG, dbg);

        if (_current_offset >= max_offset) {
          _forward = false;
          log(INFO, "BIDIR VER Direction changed: reverse");
        }
      } else {
       //_current_offset--;
        char dbg[40];
        snprintf(dbg, sizeof(dbg), "BIDIR Offset VER: %d (reverse)", _current_offset);
        log(DEBUG, dbg);

        if (_current_offset <= start_offset) {
          _loops_completed++;
          char info[50];
          snprintf(info, sizeof(info), "BIDIR VER Loop %u completed1", _loops_completed);
          log(INFO, info);

          if (_n_loops == 0 || _loops_completed < _n_loops) {
            _forward = true;
            log(INFO, "BIDIR VER Direction changed: forward");
          } else {
            _loop_terminated = true;
          }
        }
      }
    }
  }
}

void FrameScroller::extractWindow() {
  memset(_display_buffer, 0, MATRIX_SIZE);
  int16_t pos_offset_x = getPositionOffsetX();
  int16_t pos_offset_y = getPositionOffsetY();
  int16_t use_curr_off = _current_offset;
  int16_t max_offset = getMaxOffset();

  char info[150];
  snprintf(info, sizeof(info), "extractWindow: pos_offset_x=%d, pos_offset_y=%d, use_curr_off=%d, max_offset=%d",
           pos_offset_x, pos_offset_y, use_curr_off, max_offset);
  log(DEBUG, info);

  if (_direction == HOR) {
    // Scroll orizzontale, posizionamento verticale
    for (uint8_t row = 0; row < MATRIX_HEIGHT; row++) {
      int16_t src_row = row - pos_offset_y;

      // Controlla se questa riga del frame è visibile
      if (src_row >= 0 && src_row < (int16_t)_frame_height) {
        for (uint8_t col = 0; col < MATRIX_WIDTH; col++) {
          // Se frame > matrice (max_offset > 0): src_col oscilla, pos_offset_x = 0
          // Se frame < matrice (max_offset > 0): pos_offset_x oscilla, src_col fisso
          int16_t src_col;

          if (_frame_width > MATRIX_WIDTH) {
            // Frame largo: usa offset per scorrere il frame
            src_col = col + use_curr_off;
            snprintf(info, sizeof(info), "HOR LARGE: col=%d, use_curr_off=%d, src_col=%d", col, use_curr_off, src_col);
          } else {
            // Frame piccolo: oscilla la posizione sulla matrice
            int16_t actual_pos_x = pos_offset_x + use_curr_off;
            src_col = col - actual_pos_x;
            snprintf(info, sizeof(info), "HOR SMALL: col=%d, pos_offset_x=%d, use_curr_off=%d, actual_pos_x=%d, src_col=%d",
                     col, pos_offset_x, use_curr_off, actual_pos_x, src_col);
          }
          log(DEBUG, info);

          if (src_col >= 0 && src_col < (int16_t)_frame_width) {
            uint16_t src_idx = src_row * _frame_width + src_col;
            uint16_t dst_idx = row * MATRIX_WIDTH + col;
            _display_buffer[dst_idx] = _frame_data[src_idx];
          }
        }
      }
    }
  } else {
    // Scroll verticale, posizionamento orizzontale
    for (uint8_t row = 0; row < MATRIX_HEIGHT; row++) {
      int16_t src_row;
 
      if (_frame_height > MATRIX_HEIGHT) {
        // Frame alto: usa offset per scorrere il frame
        src_row = row + use_curr_off;
        snprintf(info, sizeof(info), "VER LARGE: row=%d, use_curr_off=%d, src_row=%d", row, use_curr_off, src_row);
      } else {
        // Frame piccolo: oscilla la posizione sulla matrice
        int16_t actual_pos_y = pos_offset_y + use_curr_off;
        src_row = row - actual_pos_y;
        snprintf(info, sizeof(info), "VER SMALL: row=%d, pos_offset_y=%d, use_curr_off=%d, actual_pos_y=%d, src_row=%d",
                 row, pos_offset_y, use_curr_off, actual_pos_y, src_row);
      }
      log(DEBUG, info);

      if (src_row >= 0 && src_row < (int16_t)_frame_height) {
        for (uint8_t col = 0; col < MATRIX_WIDTH; col++) {
          int16_t src_col = col - pos_offset_x;

          // Controlla se questa colonna del frame è visibile
          if (src_col >= 0 && src_col < (int16_t)_frame_width) {
            uint16_t src_idx = src_row * _frame_width + src_col;
            uint16_t dst_idx = row * MATRIX_WIDTH + col;
            _display_buffer[dst_idx] = _frame_data[src_idx];
          }
        }
      }
    }
  }
}

int16_t FrameScroller::getMaxOffset() {
  char info[150];
  if (_direction == HOR) {
    //return abs((int16_t)_frame_width - MATRIX_WIDTH);
    // Se il frame è più largo della matrice, calcola lo scroll
    if (_frame_width > MATRIX_WIDTH) {
      snprintf(info, sizeof(info), "getMaxOffset _frame_width > MATRIX_WIDTH _frame_width %d MATRIX_WIDTH %d v %d", _frame_width, MATRIX_WIDTH, (int16_t)_frame_width - MATRIX_WIDTH);
      log(DEBUG, info);
      return (int16_t)_frame_width - MATRIX_WIDTH;
    } else if (_frame_width < MATRIX_WIDTH) {
      snprintf(info, sizeof(info), "getMaxOffset _frame_width < MATRIX_WIDTH _frame_width %d MATRIX_WIDTH %d v %d", _frame_width, MATRIX_WIDTH, (int16_t)MATRIX_WIDTH - _frame_width);
      log(DEBUG, info);
      return (int16_t)MATRIX_WIDTH - _frame_width;
    }
    // Altrimenti nessuno scroll
    snprintf(info, sizeof(info), "getMaxOffset _frame_width %d MATRIX_WIDTH %d v %d", _frame_width, MATRIX_WIDTH, 0);
    log(DEBUG, info);
    return 0;
  } else {
    //return abs((int16_t)_frame_height - MATRIX_HEIGHT);
    // Verticale
    if (_frame_height > MATRIX_HEIGHT) {
      snprintf(info, sizeof(info), "getMaxOffset _frame_height > MATRIX_HEIGHT _frame_height %d MATRIX_HEIGHT %d v %d", _frame_height, MATRIX_HEIGHT, (int16_t)_frame_height - MATRIX_HEIGHT);
      log(DEBUG, info);
      return (int16_t)_frame_height - MATRIX_HEIGHT;
    } else if (_frame_height < MATRIX_HEIGHT) {
      snprintf(info, sizeof(info), "getMaxOffset _frame_height < MATRIX_HEIGHT _frame_height %d MATRIX_HEIGHT %d v %d", _frame_height, MATRIX_HEIGHT, (int16_t)MATRIX_HEIGHT - _frame_height);
      log(DEBUG, info);
      return (int16_t)MATRIX_HEIGHT - _frame_height;
    }
    snprintf(info, sizeof(info), "getMaxOffset _frame_height %d MATRIX_HEIGHT %d v %d", _frame_height, MATRIX_HEIGHT, 0);
    log(DEBUG, info);
    return 0;
  }
}

int16_t FrameScroller::getStartOffset() {
  int16_t max_offset = getMaxOffset();

  // Se non c'è oscillazione
  if (max_offset == 0) {
    return 0;
  }

  // Oscillazione dipende SOLO da FWDIR/RWDIR, non da Position
  if (_start_mode == FWDIR) {
    log(DEBUG, "getStartOffset FWDIR 0");
    return 0;  // Parte da inizio
  } else {
    log(DEBUG, "getStartOffset FWDIR max_offset");
    return max_offset;  // Parte da fine
  }
}

int16_t FrameScroller::getPositionOffsetX() {
  // Per scroll orizzontale, X è fisso; per verticale, X dipende da Position
  if (_direction == HOR) {
    return 0;  // Non viene usato in HOR
  } else {
    // VER: START=left(0), END=right, CENTER=centro
    switch (_position) {
      case START:
        return 0;  // Left
      case END:
        return MATRIX_WIDTH - (int16_t)_frame_width;  // Right (negativo se frame più largo)
      case CENTER:
        return (MATRIX_WIDTH - (int16_t)_frame_width) / 2;  // Centro
      default:
        return 0;
    }
  }
}

int16_t FrameScroller::getPositionOffsetY() {
  // Per scroll verticale, Y è fisso; per orizzontale, Y dipende da Position
  if (_direction == VER) {
    return 0;  // Non viene usato in VER
  } else {
    // HOR: START=top(0), END=bottom, CENTER=centro
    switch (_position) {
      case START:
        return 0;  // Top
      case END:
        return MATRIX_HEIGHT - (int16_t)_frame_height;  // Bottom (negativo se frame più alto)
      case CENTER:
        return (MATRIX_HEIGHT - (int16_t)_frame_height) / 2;  // Centro
      default:
        return 0;
    }
  }
}

void FrameScroller::log(LogType level, const char* message) {
  if (_log_type == NONE || _log_func == nullptr) {
    return;
  }

  if (level <= _log_type) {
    _log_func(message);
  }
}

#if is_use_test
void FrameScroller::startTest(TestMode test, size_t n_loops) {
  currentTest = test;
  testStartTime = millis();

  log(INFO, "\n========================================");

  switch (test) {
    case TEST_WIDE_HOR_BIDIR:
      {
        log(INFO, "TEST: Wide frame, HOR, BIDIR");
        log(INFO, frame_wide_log);
        setFrame(frame_wide, frame_wide_w, frame_wide_h, EASY, n_loops);
        setDirection(HOR);
        setModeLoop(BIDIR);
        setStartMode(FWDIR);
        setPosition(START);
        break;
      }
    case TEST_WIDE_HOR_MONODIR:
      {
        log(INFO, "TEST: Wide frame, HOR, MONODIR (FWDIR)");
        log(INFO, frame_wide_log);
        setDirection(HOR);
        setModeLoop(MONODIR);
        setStartMode(FWDIR);
        setPosition(START);
        setFrame(frame_wide, frame_wide_w, frame_wide_h, EASY, n_loops);
        break;
      }
    case TEST_TALL_VER_BIDIR:
      {
        log(INFO, "TEST: Tall frame, VER, BIDIR");
        log(INFO, frame_tall_log);
        setDirection(VER);
        setModeLoop(BIDIR);
        setStartMode(FWDIR);
        setPosition(CENTER);
        setFrame(frame_tall, frame_tall_w, frame_tall_h, EASY, n_loops);
        break;
      }
    case TEST_TALL_VER_MONODIR:
      {
        log(INFO, "TEST: Tall frame, VER, MONODIR (FWDIR)");
        log(INFO, frame_tall_log);
        setDirection(VER);
        setModeLoop(MONODIR);
        setStartMode(FWDIR);
        setPosition(CENTER);
        setFrame(frame_tall, frame_tall_w, frame_tall_h, EASY, n_loops);
        break;
      }
    case TEST_SMALL_POS_START:
      {
        log(INFO, "TEST: Small frame, Position START");
        log(INFO, frame_small_log);
        setDirection(HOR);
        setModeLoop(MONODIR);
        setPosition(START);
        setFrame(frame_small, frame_small_w, frame_small_h, EASY, n_loops);
        break;
      }
    case TEST_SMALL_POS_CENTER:
      {
        log(INFO, "TEST: Small frame, Position CENTER");
        log(INFO, frame_small_log);
        setDirection(HOR);
        setModeLoop(BIDIR);
        setPosition(CENTER);
        setFrame(frame_small, frame_small_w, frame_small_h, EASY, n_loops);
        break;
      }
    case TEST_SMALL_POS_END:
      {
        log(INFO, "TEST: Small frame, Position END");
        log(INFO, frame_small_log);
        setDirection(HOR);
        setModeLoop(BIDIR);
        setPosition(END);
        setFrame(frame_small, frame_small_w, frame_small_h, EASY, n_loops);
        break;
      }
    case TEST_EXACT_NOSCROLL:
      {
        log(INFO, "TEST: Exact size frame, no scroll");
        log(INFO, frame_exact_log);
        setDirection(HOR);
        setModeLoop(BIDIR);
        setPosition(START);
        setFrame(frame_exact, frame_exact_w, frame_exact_h, EASY, n_loops);
        break;
      }
    case TEST_DANGER_HOR:
      {
        log(INFO, "TEST: Danger frame, HOR scroll");
        log(INFO, frame_danger_log);
        setDirection(HOR);
        setModeLoop(BIDIR);
        setStartMode(FWDIR);
        setPosition(CENTER);
        setFrame(frame_danger, frame_danger_w, frame_danger_h, EASY, n_loops);
        break;
      }
  }

  setWaitFrame(200);
  play();
}
#endif