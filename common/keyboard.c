#include "keyboard.h"

int available = 1;
uint32 last_tick = 0;
int shift_key = 0;

uint8 get_scancode(){
	uint8 code;
	code = inb(0x60);
	return code;
}


static char shifter(char c){
	// On traite le cas de si c est une lettre

	if(0x61 <= c && c <= 0x7a){ // 
		return c-0x20;
	}
	else
	{
		switch(c)
		{
			case '&':
				return '1';
				break;
			case '{':
				return '2';
				break;
			case '"':
				return '3';
				break;
			case '\'':
				return '4';
				break;
			case '(':
				return '5';
				break;
			case '-':
				return '6';
				break;
			case '}':
				return '7';
				break;
			case '_':
				return '8';
				break;
			case '9': // ç
				return '9';
				break;
			case '0': // à
				return '0';
				break;
			case ')':
				return '°';
				break;
			case '=':
				return '+';
				break;
			case ';':
				return '.';
				break;
			case ':':
				return '/';
				break;
		}
	}
	return c;
}

keyboard_t azerty_config(uint8 code){
	union key res;
	uint8 type = 0;
	res.ch = 0;
	res.sp = 0;
	switch (code){
		case 0x01:
			res.sp = ESCAPE;
			type = 1;
			break;
		case 0x02:
				res.ch = '&';
			break;
		case 0x03:
				res.ch = '{'; // é
			break;
		case 0x04:
				res.ch = '"';
			break;
		case 0x05:
				res.ch = '\'';
			break;
		case 0x06:
				res.ch = '(';
			break;
		case 0x07:
				res.ch = '-';
			break;
		case 0x08:
				res.ch = '}'; // è
			break;
		case 0x09:
				res.ch = '_';
			break;
		case 0x0a:
				res.ch = '9';
			break;
		case 0x0b:
				res.ch = '0';
			break;
		case 0x0c:
				res.ch = ')';
			break;
		case 0x0d:
				res.ch = '=';
			break;
		case 0x0e:
			res.sp = BACKSPACE;
			type = 1;
			break;
		case 0x0f:
			res.sp = TAB;
			type = 1;
			break;
		case 0x10:
			res.ch = 'a';
			break;
		case 0x11:
			res.ch = 'z';
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
			res.ch = '^';
			break;
		case 0x1b:
			res.ch = '$';
			break;
		case 0x1c:
			res.sp = ENTER;
			type = 1;
			break;
		case 0x1d:
			res.sp = L_CTRL;
			type = 1;
			break;
		case 0x1e:
			res.ch = 'q';
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
			res.ch = 'm';
			break;
		case 0x28:
			res.ch = '%';
			break;
		case 0x29:
			print_string("Code 0x29\n");
			res.ch = '`';
			break;
		case 0x2a:
			res.sp = L_SHIFT;
			//if(!shift_key){
				shift_key = !(shift_key);
			//}
			type = 1;
			break;
		case 0x2b:
			res.ch = '<';
			break;
		case 0x2c:
			res.ch = 'w';
			break;
		case 0x2d:
			res.ch = 'x';
			break;
		case 0x2e:
			res.ch = 'c';
			break;
		case 0x2f:
			res.ch = 'v';
			break;
		case 0x30:
			res.ch = 'b';
			break;
		case 0x31:
			res.ch = 'n';
			break;
		case 0x32:
			res.ch = ',';
			break;
		case 0x33:
			res.ch = ';';
			break;
		case 0x34:
			res.ch = ':';
			break;
		case 0x35:
			res.ch = '!';
			break;
		case 0x36:
			res.sp = R_SHIFT;
			type = 1;
			break;
		case 0x37:
			res.ch = '*';
			break;
		case 0x38:
			res.sp = L_ALT;
			type = 1;
			break;
		case 0x39:
			res.sp = SPACE;
			type = 1;
			break;
		case 0xaa: // Si on relache le shift
			res.sp = RELEASE;
			shift_key = !(shift_key);
			type = 1;
			break;
	}
	keyboard_t fin;
	if(shift_key && type == 0){
		res.ch = shifter(res.ch);
	}
	fin.k = res;
	fin.type = type;
	return fin;
}

keyboard_t qwerty_config(uint8 code){
	union key res;
	uint8 type = 0;
	res.ch = 0;
	res.sp = 0;
	switch (code){
		case 0x01:
			res.sp = ESCAPE;
			type = 1;
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
			type = 1;
			break;
		case 0x0f:
			res.sp = TAB;
			type = 1;
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
			type = 1;
			break;
		case 0x1d:
			res.sp = L_CTRL;
			type = 1;
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
			//if(!shift_key){
				shift_key = !(shift_key);
			//}
			type = 1;
			break;
		case 0x2b:
			res.ch = '\\';
			break;
		case 0x2c:
			res.ch = 'z';
			break;
		case 0x2d:
			res.ch = 'x';
			break;
		case 0x2e:
			res.ch = 'c';
			break;
		case 0x2f:
			res.ch = 'v';
			break;
		case 0x30:
			res.ch = 'b';
			break;
		case 0x31:
			res.ch = 'n';
			break;
		case 0x32:
			res.ch = 'm';
			break;
		case 0x33:
			res.ch = ',';
			break;
		case 0x34:
			res.ch = '.';
			break;
		case 0x35:
			res.ch = '/';
			break;
		case 0x36:
			res.sp = R_SHIFT;
			type = 1;
			break;
		case 0x37:
			res.ch = '*';
			break;
		case 0x38:
			res.sp = L_ALT;
			type = 1;
			break;
		case 0x39:
			res.sp = SPACE;
			type = 1;
			break;
		case 0xaa: // Si on relache le shift
			res.sp = RELEASE;
			shift_key = !(shift_key);
			type = 1;
			break;
	}
	keyboard_t fin;
	if(shift_key && type == 0){
		res.ch = shifter(res.ch);
	}
	fin.k = res;
	fin.type = type;
	return fin;
}


static void make_available(){
	available = 1;
}

void init_keyboard(){
	register_interrupt_handler(33, make_available);
}

keyboard_t keyboard_handler(){
	uint8 code = 0;
	if(available){
		code = get_scancode();
		available = 0;
	}
	return azerty_config(code);
}
