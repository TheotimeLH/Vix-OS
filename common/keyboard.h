#ifndef KEYBOARD_H
#define KEYBOARD_H

#ifndef STDLIB_VERSION
#include "common.h"
#include "isr.h"
#endif

enum keyboard_disposition{
	QWERTY,
	AZERTY,
};

enum special_key{
	UNUSED = 0,
	BACKSPACE,
	ESCAPE,
	SPACE,
	ENTER,
	L_ALT,
	L_CTRL,
	L_SHIFT,
	R_SHIFT,
	TAB,
	CAPS_LOCK,
	RELEASE
};

union key
{
	char ch;
	enum special_key sp;
};

typedef struct keyboard 
{
	uint8 type; // Si c'est 0 c'est un char sinon c'est un caractere spécial
	union key k;
} keyboard_t;

#ifndef STDLIB_VERSION
#ifdef __cplusplus
extern "C" {
#endif

uint8 get_scancode();
keyboard_t keyboard_handler();
void init_keyboard();
#ifdef __cplusplus
}
#endif
#endif

#endif
