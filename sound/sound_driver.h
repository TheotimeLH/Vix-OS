#ifndef SOUND_H
#define SOUND_H

#include "../common/common.h"


#define E 2.71828
#define PI 3.14159265358979323846264338327950

#define NUM_NOTES 8

#define NUM_OCTAVES 7
#define OCTAVE_SIZE 12

#define OCTAVE_1 0
#define OCTAVE_2 1
#define OCTAVE_3 2
#define OCTAVE_4 3
#define OCTAVE_5 4
#define OCTAVE_6 5
#define OCTAVE_7 6

#define NOTE_C      0
#define NOTE_CS     1
#define NOTE_DF     NOTE_CS
#define NOTE_D      2
#define NOTE_DS     3
#define NOTE_EF     NOTE_DS
#define NOTE_E      4
#define NOTE_F      5
#define NOTE_FS     6
#define NOTE_GF     NOTE_FS
#define NOTE_G      7
#define NOTE_GS     8
#define NOTE_AF     NOTE_GS
#define NOTE_A      9
#define NOTE_AS     10
#define NOTE_BF     NOTE_AS
#define NOTE_B      11

#define NOTE_NONE   12

#define WAVE_SIN        0
#define WAVE_SQUARE     1
#define WAVE_NOISE      2
#define WAVE_TRIANGLE   3

void sound_init();
void sound_note(uint8 index, uint8 octave, uint8 note);
void sound_master(uint8 v);
void sound_volume(uint8 index, uint8 v);
void sound_wave(uint8 index, uint8 wave);

#endif
