#ifndef KEYBOARD_H
#define KEYBOARD_H

enum keyboard_disposition{
	QWERTY;
	AZERTY;
};

enum special_key{
	ESCAPE;
	SHIFT;
	TAB;
	CAPS_LOCK;
};


union key
{
	char ch;
	enum special_key sp;
};


	uint8 get_scancode();
	void keyboard_handler();



#endif
