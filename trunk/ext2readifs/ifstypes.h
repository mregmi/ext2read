#ifndef __IFS_TYPES_H
#define __IFS_TYPES_H

//#ifdef	_CPU_X86_
 typedef	char			int8_t;
 typedef	short			int16_t;
 typedef	int				int32_t;

 typedef	unsigned char	uint8_t;
 typedef	unsigned short	uint16_t;
 typedef	unsigned int	uint32_t;

//#else
// #error "Types not defined for this architecture"

//#endif	// if _CPU_X86_

#endif  // __IFS_TYPES_H
