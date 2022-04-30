#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../common/common.h"

enum keyboard_disposition{
	QWERTY,
	AZERTY,
};

enum special_key{
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
};


union key
{
	char ch;
	enum special_key sp;
};

#ifdef __cplusplus
extern "C" {
#endif

uint8 get_scancode();
union key keyboard_handler();
void init_keyboard();
#ifdef __cplusplus
}
#endif

#endif
