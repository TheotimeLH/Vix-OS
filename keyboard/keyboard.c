#include "keyboard.h"


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
			break;
		case 0x15:
			break;
		case 0x16:
			break;
		case 0x17:
			break;
		case 0x18:
			break;
	}
	return res;
}


union key keyboard_handler(){
	uint8 code;
	int shift_key = 0;
	code = get_scancode();
	/*
	if(code & 0x2A){
		shift_key = 1;
	}
	*/
	return qwerty_config(code, shift_key);
}
