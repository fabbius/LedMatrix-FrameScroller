#ifndef ANIMATIONS_TESTS_H
#define ANIMATIONS_TESTS_H

#include <Arduino.h>  
/*
// ===== FRAME LARGO (30x8) - Per HOR scroll =====
// Frame più largo della matrice, perfetto per testare HOR BIDIR/MONODIR
uint8_t frame_wide[30 * 8] = {
  0,0,0,0,0,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,0,0,0,0,0,
  0,0,0,0,0,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,0,0,0,0,0,
  0,0,0,0,0,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,0,0,0,0,0,
  0,0,0,0,0,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,0,0,0,0,0,
  0,0,0,0,0,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,0,0,0,0,0,
  0,0,0,0,0,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,0,0,0,0,0,
  0,0,0,0,0,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};
const uint8_t frame_wide_w = 30;
const uint8_t frame_wide_h = 8;

// ===== FRAME ALTO (13x16) - Per VER scroll =====
// Frame più alto della matrice, perfetto per testare VER BIDIR/MONODIR
uint8_t frame_tall[13 * 16] = {
  0,0,0,5,5,5,5,5,5,5,0,0,0,
  0,0,0,7,7,7,7,7,7,7,0,0,0,
  0,0,0,7,7,7,7,7,7,7,0,0,0,
  0,0,0,7,7,7,7,7,7,7,0,0,0,
  0,0,0,7,7,7,7,7,7,7,0,0,0,
  0,0,0,7,7,7,7,7,7,7,0,0,0,
  0,0,0,7,7,7,7,7,7,7,0,0,0,
  0,0,0,5,5,5,5,5,5,5,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,3,3,3,3,3,3,3,0,0,0,
  0,0,0,3,3,3,3,3,3,3,0,0,0,
  0,0,0,3,3,3,3,3,3,3,0,0,0,
  0,0,0,3,3,3,3,3,3,3,0,0,0,
  0,0,0,3,3,3,3,3,3,3,0,0,0,
  0,0,0,3,3,3,3,3,3,3,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,
};
const uint8_t frame_tall_w = 13;
const uint8_t frame_tall_h = 16;

// ===== FRAME PICCOLO (8x6) - Per posizionamento fisso =====
// Frame più piccolo della matrice, testa START/CENTER/END senza scroll
uint8_t frame_small[8 * 6] = {
  0,7,7,7,7,7,7,0,
  0,7,0,0,0,0,7,0,
  0,7,0,7,7,0,7,0,
  0,7,0,7,7,0,7,0,
  0,7,0,0,0,0,7,0,
  0,7,7,7,7,7,7,0,
};
const uint8_t frame_small_w = 8;
const uint8_t frame_small_h = 6;

// ===== FRAME UGUALE MATRICE (13x8) - Nessuno scroll =====
// Frame uguale alla matrice, testa che rimane fermo
uint8_t frame_exact[13 * 8] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,3,3,3,3,3,3,0,0,0,0,
  0,3,0,3,0,0,0,0,3,0,3,0,0,
  3,7,3,3,0,7,7,0,3,3,7,3,0,
  0,3,0,3,0,0,0,0,3,0,3,0,0,
  0,0,0,3,3,3,3,3,3,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,
};
const uint8_t frame_exact_w = 13;
const uint8_t frame_exact_h = 8;

// ===== FRAME DANGER (originale) - 13x6 =====
// Triangolo di avvertimento con contrasto
uint8_t frame_danger[13 * 6] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,1,2,2,1,0,0,
  0,1,0,0,0,1,2,7,7,7,7,5,1,
  2,7,2,1,7,7,7,7,7,7,7,7,3,
  0,1,0,0,0,1,2,7,7,7,7,5,1,
  0,0,0,0,0,0,0,1,2,2,1,0,0,
};
const uint8_t frame_danger_w = 13;
const uint8_t frame_danger_h = 6;
*/


// ===== FRAME LARGO (30x8) - Per HOR scroll =====
// Frame più largo della matrice, perfetto per testare HOR BIDIR/MONODIR
const uint8_t frame_wide[30 * 8] = {
  2,0,0,0,0,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,0,0,0,0,2,
  2,0,0,0,0,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,0,0,0,0,2,
  2,0,0,0,0,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,0,0,0,0,2,
  2,0,0,0,0,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,0,0,0,0,2,
  2,0,0,0,0,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,0,0,0,0,2,
  2,0,0,0,0,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,0,0,0,0,2,
  2,0,0,0,0,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,0,0,0,0,2,
  2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
};
static char* frame_wide_log = "Frame più largo della matrice, perfetto per testare HOR BIDIR/MONODIR";
const uint8_t frame_wide_w = 30;
const uint8_t frame_wide_h = 8;
 
// ===== FRAME ALTO (13x16) - Per VER scroll =====
// Frame più alto della matrice, perfetto per testare VER BIDIR/MONODIR
const uint8_t frame_tall[13 * 16] = {
  0,0,0,5,5,5,5,5,5,5,0,0,0,
  0,0,0,7,7,7,7,7,7,7,0,0,0,
  0,0,0,7,7,7,7,7,7,7,0,0,0,
  0,0,0,7,7,7,7,7,7,7,0,0,0,
  0,0,0,7,7,7,7,7,7,7,0,0,0,
  0,0,0,7,7,7,7,7,7,7,0,0,0,
  0,0,0,7,7,7,7,7,7,7,0,0,0,
  0,0,0,5,5,5,5,5,5,5,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,3,3,3,3,3,3,3,0,0,0,
  0,0,0,3,3,3,3,3,3,3,0,0,0,
  0,0,0,3,3,3,3,3,3,3,0,0,0,
  0,0,0,3,3,3,3,3,3,3,0,0,0,
  0,0,0,3,3,3,3,3,3,3,0,0,0,
  0,0,0,3,3,3,3,3,3,3,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,
};
static char* frame_tall_log = "Frame più alto della matrice, perfetto per testare VER BIDIR/MONODIR";
const uint8_t frame_tall_w = 13;
const uint8_t frame_tall_h = 16;
 
// ===== FRAME PICCOLO (8x6) - Per posizionamento fisso =====
// Frame più piccolo della matrice, testa START/CENTER/END senza scroll
const uint8_t frame_small[6 * 4] = {
  0,7,7,7,7,0,
  0,7,7,0,7,0,
  0,7,7,0,7,0,
  0,7,7,7,7,0,
};
static char* frame_small_log = "Frame più piccolo della matrice, testa START/CENTER/END senza scroll";
const uint8_t frame_small_w = 6;
const uint8_t frame_small_h = 4;
 
// ===== FRAME UGUALE MATRICE (13x8) - Nessuno scroll =====
// Frame uguale alla matrice, testa che rimane fermo
const uint8_t frame_exact[13 * 8] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,3,3,3,3,3,3,0,0,0,0,
  0,3,0,3,0,0,0,0,3,0,3,0,0,
  3,7,3,3,0,7,7,0,3,3,7,3,0,
  0,3,0,3,0,0,0,0,3,0,3,0,0,
  0,0,0,3,3,3,3,3,3,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,
};
static char* frame_exact_log = "Frame uguale alla matrice, testa che rimane fermo";
const uint8_t frame_exact_w = 13;
const uint8_t frame_exact_h = 8;
 
// ===== FRAME DANGER (originale) - 13x6 =====
// Triangolo di avvertimento con contrasto
const uint8_t frame_danger[13 * 6] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,1,2,2,1,0,0,
  0,1,0,0,0,1,2,7,7,7,7,5,1,
  2,7,2,1,7,7,7,7,7,7,7,7,3,
  0,1,0,0,0,1,2,7,7,7,7,5,1,
  0,0,0,0,0,0,0,1,2,2,1,0,0,
};
static char* frame_danger_log = "Triangolo di avvertimento con contrasto";
const uint8_t frame_danger_w = 13;
const uint8_t frame_danger_h = 6;

#endif
