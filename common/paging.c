uint32 *frames;
uint32 nframes;

extern uint32 placement_adress;
// ON va utiliser des bitmaps
#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

static void set_frame(uint32 frame_addr){
	uint32 frame = frame_addr/0x1000;
	uint32 idx = INDEX_FROM_BIT(frame);
	uint32 off = OFFSET_FROM_BIT(frame);
	frames[idx] |= (0x1 << off);
}


static void clear_frame(uint32 frame_addr){
	uint32 frame = frame_addr/0x1000;
	uint32 idx = INDEX_FROM_BIT(frame);
	uint32 off = OFFSET_FROM_BIT(frame);
	frames[idx] &= ~(0x1 << off);
}


static uint32 test_frame(uint32 frame_addr){
	uint32 frame = frame_addr/0x1000;
	uint32 idx = INDEX_FROM_BIT(frame);
	uint32 off = OFFSET_FROM_BIT(frame);
	return (frames[idx] & (0x1 << off));
}


static uint32 first_frame(){ // On cherche la premiere frame libre
	uint32 i, j;
	for(i = 0; i < INDEX_FROM_BIT(nframes); i++){
		if(frames[i] != 0xFFFFFFFF){
			for(j = 0; j < 32; j++){
				uint32 to_test = 0x1 << j;
				if(frames[i] & to_test){
					return i*4*8 +j;
				}
			}
		}
	}
}
