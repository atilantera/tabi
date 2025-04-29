#ifndef DEBUG_CPP
#define DEBUG_CPP

#include <conio.h>

void dumpchar(char * d, unsigned long length)
{
	unsigned long i;
	cout << "----------------" << endl;
	for (i = 0; i < length; i++)
	{
		cout << short(d[i]) << " ";
		if ((i > 0) && (i % 16 == 0)) cout << endl;
		if ((i > 0) && (i % (16 * 20) == 0)) getch();
	}
	cout << "----------------" << endl;
	getch();
}

void dumpchar2(char * d, unsigned long length)
{
	unsigned long i;
	cout << "----------------" << endl;
	for (i = 0; i < length; i++)
	{
		cout << d[i] << " [" << i << "]=" << short(d[i]) << endl;
		if ((i > 0) && i % 20 == 0) getch();
	}
	cout << "----------------" << endl;
	getch();
}


#endif
