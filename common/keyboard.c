#include "keyboard.h"

int available = 1;

uint8 get_scancode(){
	uint8 code;
	code = inb(0x60);
	return code;
}

union key qwerty_config(uint8 code, int shift_key){
	union key res;
	res.ch = 0;
	res.sp = 0;
	switch (code){
		case 0x01:
			res.sp = ESCAPE;
			break;
		case 0x02:
				res.ch = '1';
			break;
		case 0x03:
				res.ch = '2';
			break;
		case 0x04:
				res.ch = '3';
			break;
		case 0x05:
				res.ch = '4';
			break;
		case 0x06:
				res.ch = '5';
			break;
		case 0x07:
				res.ch = '6';
			break;
		case 0x08:
				res.ch = '7';
			break;
		case 0x09:
				res.ch = '8';
			break;
		case 0x0a:
				res.ch = '9';
			break;
		case 0x0b:
				res.ch = '0';
			break;
		case 0x0c:
				res.ch = '-';
			break;
		case 0x0d:
				res.ch = '=';
			break;
		case 0x0e:
			res.sp = BACKSPACE;
			break;
		case 0x0f:
			res.sp = TAB;
			break;
		case 0x10:
				res.ch = 'q';
			break;
		case 0x11:
			res.ch = 'w';
			break;
		case 0x12:
			res.ch = 'e';
			break;
		case 0x13:
			res.ch = 'r';
			break;
		case 0x14:
			res.ch = 't';
			break;
		case 0x15:
			res.ch = 'y';
			break;
		case 0x16:
			res.ch = 'u';
			break;
		case 0x17:
			res.ch = 'i';
			break;
		case 0x18:
			res.ch = 'o';
			break;
		case 0x19:
			res.ch = 'p';
			break;
		case 0x1a:
			res.ch = '[';
			break;
		case 0x1b:
			res.ch = ']';
			break;
		case 0x1c:
			res.sp = ENTER;
			break;
		case 0x1d:
			res.sp = L_CTRL;
			break;
		case 0x1e:
			res.ch = 'a';
			break;
		case 0x1f:
			res.ch = 's';
			break;
		case 0x20:
			res.ch = 'd';
			break;
		case 0x21:
			res.ch = 'f';
			break;
		case 0x22:
			res.ch = 'g';
			break;
		case 0x23:
			res.ch = 'h';
			break;
		case 0x24:
			res.ch = 'j';
			break;
		case 0x25:
			res.ch = 'k';
			break;
		case 0x26:
			res.ch = 'l';
			break;
		case 0x27:
			res.ch = ';';
			break;
		case 0x28:
			res.ch = '\'';
			break;
		case 0x29:
			res.ch = '`';
			break;
		case 0x2a:
			res.sp = L_SHIFT;
			break;
		case 0x2b:
			res.ch = '\\';
			break;
	}
	return res;
}


static void make_available(){
	available = 1;
}

void init_keyboard(){
	register_interrupt_handler(33, make_available);
}

union key keyboard_handler(){
	uint8 code = 0;
	int shift_key = 0;
	if(available){
		code = get_scancode();
		available = 0;
	}
	/*
	if(code & 0x2A){
		shift_key = 1;
	}
	*/
	return qwerty_config(code, shift_key);
}
