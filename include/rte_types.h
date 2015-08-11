#ifndef __RTE_TYPES_H__
#define __RTE_TYPES_H__

typedef signed char  int8_t;
typedef signed short int16_t;
typedef signed int	 int32_t;
typedef signed long	 int64_t;
typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;
typedef unsigned long  uint64_t;

#define typecheck(type, x)		\
		({type __dummy; 		\
		 typeof(x) __dummy2;	\
		 (void)(&__dummy == &__dummy2);	\
		 1;	\
		 })

#endif
