#ifndef VAR_H
#define VAR_H

#include <stdint.h>

typedef enum {
	OT_NULL,
	OT_STRING,
	OT_INTEGER,
	OT_FLOAT
} BaseObjectType;

typedef struct {
	BaseObjectType base;

} TypeObject;

struct {
	TypeObject * type;
	union {
		struct {
			char data[256];
			uint16_t length;
			
		} tString;

		struct {

		} tInteger;
	};
} Object;

typedef struct Object Object;

#endif