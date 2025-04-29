#define OS_TYPE 1 // 1=DOS/WINDOWS 2=UNIX
#define DEBUG

#ifndef DEBUG
    #define debugprint(x)
    #define debugcout(x)
    #define dbcout(x)
#else
    #include <assert.h>
    #define debugprint(x) \
            cout << __FILE__ << ":" << __LINE__; \
            cout << ": '" << #x << "' = " << x << endl;
    #define debugcout(x) \
            cout << __FILE__ << ":" << __LINE__; \
            cout << ": " << x << endl;
    #define dbcout(x) cout << x;
#endif

const double VERSION_ID = 30122002.1412;

#include <fstream.h>
#include "intface.hpp"

int main(int argc, char * argv[])
{
	TabInterface * interface;
	interface = new TabInterface;

	if(interface == NULL)
	{
		cout << "Out of memory" << endl;
		return 1;
	}
	else
	{
		int ret;
		int requestKeypress;
		if(interface->good())
        {
            ret = interface->activate(argc, argv, requestKeypress);
            
            if(interface->good() == 0) interface->printerror();
            
            if(ret > requestKeypress)
            {
                cout << "(Press any key to quit)" << endl;
                getch();
            }
        }
		else
        {
            ret = 1;
        }

		delete interface;
		return ret;
	}
}
