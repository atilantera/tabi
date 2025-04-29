// INVERT.H
// Byte order inversions in standard variables

// 6-4-2002 alkimohap

#ifndef INVERT_H
#define INVERT_H

unsigned short invb(unsigned short s);
unsigned long invb(unsigned long l);

unsigned short invb(unsigned short s)
{
	// AA BB
	// (BB 00) + (00 AA)
	return (s<<8) + (s>>8);
}

unsigned long invb(unsigned long l)
{
	// AA BB CC DD
	// (DD 00 00 00) << 24
	// (00 AA BB CC) >> 8, (CC 00 00 00) << 24, (00 CC 00 00) >> 8
	// (00 00 AA BB) >> 16,(BB 00 00 00) << 24, (00 00 BB 00) >> 16
	// (00 00 00 AA) >> 24
	return (l<<24) + (((l>>8)<<24)>>8) + (((l>>16)<<24)>>16) + (l>>24);
}

#endif
