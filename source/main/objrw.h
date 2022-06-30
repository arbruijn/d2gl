#ifndef _OBJRW_H
#define _OBJRW_H
#include "object.h"

#define OBJ_RW_SIZE 0x108

void object_to_object_rw(object *obj, void *obj_rw);
void object_rw_to_object(void *obj_rw, object *obj);

#endif
