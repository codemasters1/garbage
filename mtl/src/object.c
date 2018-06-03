#include <malloc.h>
#include "object.h"

bool Object_Init(Object * obj, TypeObject * to, size_t size)
{
	obj = malloc(sizeof(Object) + size);

	return true;
}