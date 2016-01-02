#include "system/u.h"
#include <stdint.h>
#include "system/field.h"
#include "system/macros.h"
#include <string.h>
#include <endian.h>

uint16_t
getfield16(uchar const **mem)
{
	uint16_t field;

	memcpy(&field, *mem, 2);
	*mem += BIT16SZ;

	return htole16(field);
}

uint32_t
getfield32(uchar const **mem)
{
	uint32_t field;

	memcpy(&field, *mem, 4);
	*mem += BIT32SZ;

	return htole32(field);
}

