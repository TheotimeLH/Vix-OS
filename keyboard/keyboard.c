uint8 get_scancode(){
	uint8 code;
	code = inb(0x60);
	return code;
}

union key azerty_config(uint8 code, int shift_key){
	union key res;
	res.ch = 0;
	res.sp = 0;
	switch (code){
		case 0x01:
			res.sp = ESCAPE;
			break;
		case 0x02:
			if(!shift_key)
				res.ch = '&';
			else
				res.ch = '1';
			break;
		case 0x03:
			if(!shift_key)
				res.ch = 'é';
			else
				res.ch = '2';
			break;
		case 0x04:
			if(!shift_key)
				res.ch = '"';
			else
				res.ch = '3';
			break;
		case 0x05:
			if(!shift_key)
				res.ch = '\'';
			else
				res.ch = '4';
			break;
		case 0x06:
			if(!shift_key)
				res.ch = '(';
			else
				res.ch = '5';
			break;
		case 0x07:
			if(!shift_key)
				res.ch = '-';
			else
				res.ch = '6';
			break;
		case 0x08:
			if(!shift_key)
				res.ch = 'è';
			else
				res.ch = '7';
			break;
		case 0x09:
			if(!shift_key)
				res.ch = '_';
			else
				res.ch = '8';
			break;
		case 0x0a:
			if(!shift_key)
				res.ch = 'ç';
			else
				res.ch = '9';
			break;
		case 0x0b:
			if(!shift_key)
				res.ch = 'à';
			else
				res.ch = '0';
			break;
		case 0x0c:
			if(!shift_key)
				res.ch = ')';
			else
				res.ch = '°';
			break;
		case 0x0d:
			if(!shift_key)
				res.ch = '=';
			else
				res.ch = '+';
			break;
		case 0x0e:
			break;
		case 0x0f:
			break;
		case 0x10:
			break;
		case 0x11:
			break;
		case 0x12:
			break;
		case 0x13:
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
		default : 
	}
	return res;
}


char keyboard_handler(){
	uint8 code;
	int shift_key = 0;
	code = get_scancode();
	if(code == 0x2A){
		shift_key = 1;
	}
	return azerty_config(uint8 code, int shift_key);
}
