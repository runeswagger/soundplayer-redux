/* soundplayer scripting language interpreter
 * Timothy Loiselle
 * 04/1/2014
 */
#include <stdio.h>
#include <stdlib.h>
#include <includes/common.h>
#include <includes/splib.h>

//the handle for the linked list
sp_module_t **module_vector = NULL;

//the names of the objects in sp_module_t
char **module_names = NULL;

//keep track of the size of module_vector and module_names
unsigned long nmodules = 0;

int spscript_new_module(char* name){
	sp_module_t *module = malloc(sizeof(sp_module_t));
	//won't allocate space for name...
	//assuming the pointer passed is not temporary
	nmodules++;
	realloc(module_vector, nmodules*sizeof(sp_module_t*));
	realloc(module_names, nmodules*sizeof(sp_module_t*));
	module_vector[nmodules] = module;
	module_names[nmodules] = name;

	return 1;
}

int spscript_module_by_name(char* name){
	//return the index in module_vector of the name
	int offset = 0;
	while(offset < nmodules){
		//linear search
		if(strcmp(name,module_names[offset])) break;
		offset++;
	}

	return (offset == nmodules) ? -1 : offset;
}

int spscript_module_linker(char* script){
	//assemble the module linkages
	int cur,next; //the current module and the next one
	char *curtoken, *nexttoken; //
	
	module_vector[cur]->next = module_vector[next]; //link them
	return 0;
}

int parser(char* script){
	//evaluate lines of soundplayer script and execute them
	return 0;
}
	
