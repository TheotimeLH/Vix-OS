#include "../stdlib/stdlib.h"

#define VIDEO_H 25
#define VIDE0_W 80

char* buffer = 0; // Au départ on a un buffer vide

typedef struct text{
	char c;
	uint8 fg; // La couleur du premier plan
	uint8 bg; // La couleur de l'arriere plan
	uint8 cursor; // Le curseur est-il dessus
} text_t;


typedef struct line{
	text_t* line_buffer;
}line_t;

void* memset(void p*, int value, size_t count){
	uint8 *p = pointer;
	while(count--)
		*p++ = (uint8) value;
	return pointer;
}

int main(){
	// Donc déjà il faut lire un fichier (mais bon ça c'est pour plus tard
	//
	int line_under = 0; // la premiere ligne affichée à l'écran
	text_t buffer[100000]; // Pour l'instant on a juste un buffer alloué n'importe comment, il faudra utiliser malloc
	memset(buffer, 0, sizeof(text_t)*10000);
	int cursorX = 0, cursorY = 0;
	
	while(1){ // main loop
		// On va afficher à l'écran le buffer
		int s = line_under * VIDEO_W;
		for(i = 0; i < (VIDEO_H -2)*VIDEO_W; i++){
			text_t actuel = buffer[i+s];
			if(actuel.cursor){
				print_screen(i%VIDEO_W, i/VIDEO_W, actuel.c, bg, fg);
			}
			else{
				print_screen(i%VIDEO_W, i/VIDEO_W, actuel.c, fg, bg);
			}
		}
		
		// On a pas encore les gestions claviers
		// Il faudrait plutot considerer ça ligne par ligne, parce que la on a le probleme que chaque ligne fait au plus 80 caracteres...

	}
	
}
