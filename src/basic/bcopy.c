#include <string.h>


void _DEFUN (bcopy, (src_void, dst_void, length),
	     _CONST char *src_void _AND
	     char *dst_void _AND
	     size_t length)
{
	char *dst = dst_void;
	_CONST char *src = src_void;

	if (src < dst && dst < src + length) {
		/* Have to copy backwards */
		src += length;
		dst += length;
		while (length--) {
			*--dst = *--src;
		}
	} else {
		while (length--) {
			*dst++ = *src++;
		}
	}

	return;
}
