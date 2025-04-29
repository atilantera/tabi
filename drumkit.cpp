#ifndef DRUMKIT_CPP
#define DRUMKIT_CPP

#include <stdlib.h>
#include <string.h>
#include <conio.h> // random getch()'s for debug purpose; seek & destroy

TabDrumkit::TabDrumkit()
{
   for (short i=0; i<128; i++)
	{
		for (short j=i*5; j<(i+1)*5; j++) shortNames[j] = '\0';

		longNames[i] = 0;
		nameLen[i] = 0;
		Order[i] = -1;
		Base[i] = -1;
		Mark[i] = 0;
	}

	drumkitName = 0;
	shortNameRet = 0;
	NoError = 1;
}

TabDrumkit::TabDrumkit(const TabDrumkit &op)
{
	unsigned long len;
	unsigned short i;

	NoError = 1;

	drumkitName = new char[strlen(op.drumkitName)];
	if (drumkitName != NULL) strcpy(drumkitName, op.drumkitName);
	else
	{
		NoError = 0;
	}

	for (i=0; i < 5 * 128; i++)
	{
		shortNames[i] = op.shortNames[i];
	}

	i = 0;
	while(i<128 && NoError)
	{
		if (op.longName(i)!=NULL)
		{
			len = strlen(op.longName(i));
			longNames[i] = new char[len+1];
			if (longNames[i]!=NULL)
			{
				strcpy(longNames[i], op.longNames[i]);
				longNames[i][len] = '\0';
			}
			else
			{
				NoError = 0;
			}
		}
		else
		{
			longNames[i] = new char;
			*longNames[i] = '\0';
		}

		nameLen[i] = op.nameLen[i];
		Order[i] = op.Order[i];
		Base[i] = op.Base[i];
		Mark[i] = op.Mark[i];
		i++;
	}
	shortNameRet = 0;
}

TabDrumkit::~TabDrumkit()
{
	for (unsigned char i=0; i<128; i++)
	{
		delete [] longNames[i];
		longNames[i] = 0;
	}

	delete shortNameRet;
	shortNameRet = 0;
}

TabDrumkit& TabDrumkit::operator= (const TabDrumkit &op)
{
	if (this== &op)
		return *this;

	unsigned long len;
	unsigned short i;

	NoError = 1;

	drumkitName = new char[strlen(op.drumkitName)];
	if (drumkitName != NULL) strcpy(drumkitName, op.drumkitName);
	else
	{
		NoError = 0;
	}

	for (i=0; i < 5 * 128; i++)
	{
		shortNames[i] = op.shortNames[i];
	}

	i = 0;
	while(i<128 && NoError)
	{
		if (op.longName(i)!=NULL)
		{
			len = strlen(op.longName(i));
			longNames[i] = new char[len+1];
			if (longNames[i]!=NULL)
			{
				strcpy(longNames[i], op.longNames[i]);
				longNames[i][len] = '\0';
			}
			else
			{
				NoError = 0;
			}
		}
		else
		{
			longNames[i] = new char;
			*longNames[i] = '\0';
		}

		nameLen[i] = op.nameLen[i];
		Order[i] = op.Order[i];
		Base[i] = op.Base[i];
		Mark[i] = op.Mark[i];
		i++;
	}

	shortNameRet = 0;
	return *this;
}

void TabDrumkit::read(ifstream & infile)
{
	unsigned long i;
	unsigned long namelength;

	// drumkitName
	if (NoError == 1 && infile.good()==1)
	{
		infile.read(&namelength, sizeof(unsigned long));

		delete [] drumkitName;
		drumkitName = new char[namelength];

		if (drumkitName == NULL)
		{
			NoError = 0;
		}
		else
		{
			infile.read(drumkitName, namelength);
		}
	}

	// longNames
	if (NoError == 1 && infile.good()==1)
	{
		i = 0;
		do {
			delete [] longNames[i];
			infile.read(&namelength, sizeof(unsigned long));
			longNames[i] = new char[namelength];
			if (longNames[i] == NULL)
			{
				NoError = 0;
			}
			else
			{
				infile.read(longNames[i], namelength);
				i++;
			}
		} while (i < 128 && NoError == 1);
	}

	// All the rest
	if (NoError == 1 && infile.good()==1)
	{
		infile.read(&shortNames, 5*128);
		infile.read(&nameLen, 128);
		infile.read(&Order, sizeof(Order)*128);
		infile.read(&Base, sizeof(Base)*128);
		infile.read(&Mark, 128);
	}
}

void TabDrumkit::write(ofstream & outfile)
{
	if (NoError == 1)
	{
		unsigned long i;
		unsigned long namelength;

		// drumkitName
		namelength = strlen(drumkitName);
		outfile.write(&namelength, sizeof(unsigned long));
		outfile.write(drumkitName, namelength);

		// longNames
		for (i = 0; i < 128; i++)
		{
			namelength = strlen(longNames[i]);
			outfile.write(&namelength, sizeof(unsigned long));
			outfile.write(longNames[i], namelength);
		}

		// All the rest
		outfile.write(&shortNames, 5*128);
		outfile.write(&nameLen, 128);
		outfile.write(&Order, sizeof(Order)*128);
		outfile.write(&Base, sizeof(Base)*128);
		outfile.write(&Mark, 128);
	}
}

void TabDrumkit::setKey(unsigned char no, char * sName, char * lName, short ord, short base_, char ch)
{
	// * sName
	shortName(no, sName);

	// * lName
	longName(no, lName);

	// ord, Base, ch
	if (no < 128 && NoError)
	{
		Order[no] = ord;
		Base[no] = base_;
		Mark[no] = ch;
	}
}

void TabDrumkit::setBuiltIn(short kit)
{
	// Sets built-in kits defined here

	// SLAVE AND MASTER DRUMS:
	// When drum (=key) is MASTER, it has an own line on which
	// its notes will be marked. .order is the line number of
	// that key. Different master keys cannot have same order (line) number.
	// For master keys, order >= 0 && base == -1

	// SLAVE keys do not have an own line for their notes.
	// All slave keys use line of some defined master key = .base
	// For slave keys, order == freely defined && base == (index of a master key)

	// NOTE: if (order(x)) { key[x] is not in use = is not defined }

	this->clear();
	switch(kit)
	{
		case 0:
		{
		this->setKitName("General MIDI default");

		//			 index symbol name						   order base mark
		this->setKey(44, "Hf",	  "Pedal hi-hat",			   0,	-1, 'x');
		this->setKey(35, "B",	  "Bass drum",				   1,	36, 'o');
		this->setKey(36, "B",	  "Bass drum",				   1,	-1, 'o');

		this->setKey(37, "S",	  "Snare side stick / rim",    2,	38, '@');
		this->setKey(38, "S",	  "Snare drum", 			   2,	-1, 'o');
		this->setKey(40, "S",	  "Snare drum 2",			   2,	38, 'o');
		this->setKey(84, "S",	  "Cross stick",			   2,	38, 'x');

		this->setKey(42, "H",	  "Closed hi-hat",			   3,	-1, 'x');
		this->setKey(46, "H",	  "Open hi-hat",			   3,	42, 'X');
		this->setKey(56, "CB",	  "Cowbell",				   4,	-1, 'x');

		this->setKey(41, "F2",	  "Low floor tom",			   5,	-1, 'o');
		this->setKey(43, "F",	  "High floor tom", 		   6,	-1, 'o');
		this->setKey(45, "T4",	  "Low large tom",			   7,	-1, 'o');
		this->setKey(47, "T3",	  "Hight large tom",		   8,	-1, 'o');
		this->setKey(48, "T2",	  "Low small tom",			   9,	-1, 'o');
		this->setKey(50, "T1",	  "High small tom", 		  10,	-1, 'o');

		this->setKey(49, "C ",	  "Crash cymbal",			  11,	-1, 'x');
		this->setKey(57, "C2",	  "Crash cymbal 2", 		  12,	-1, 'x');
		this->setKey(51, "R ",	  "Ride cymbal",			  14,	-1, 'x');
		this->setKey(53, "R ",	  "Ride bell",				  14,	51, '+');
		this->setKey(59, "R2",	  "Ride cymbal 2",			  15,	-1, 'x');
		this->setKey(55, "SC",	  "Splash cymbal",			  16,	-1, 'x');
		this->setKey(52, "CH",	  "Chinese cymbal", 		  17,	-1, 'x');

		this->setKey(54, "TA",	  "Tambourine", 			  18,	-1, 'x');
		this->setKey(39, "HC",	  "Hand clap",				  19,	-1, 'x');

		this->setKey(80, "TR",	  "Mute triangle",			  20,	-1, 'x');
		this->setKey(81, "TR",	  "Open triangle",			  21,	80, 'X');

		break;
		}

		case 1:
		{
		this->setKitName("Older by alkimohap");

		//			 index symbol name						   order base mark
		this->setKey(35, "B",	  "Bass drum",				   0,	36, 'o');
		this->setKey(36, "B",	  "Bass drum",				   0,	-1, 'o');

		this->setKey(37, "S",	  "Side stick", 			   1,	38, '/');
		this->setKey(38, "S",	  "Snare drum", 			   1,	-1, 'o');
		this->setKey(40, "S",	  "Snare drum 2",			   1,	38, 'O');
		this->setKey(39, "HC",	  "Hand clap",				   2,	-1, 'x');

		this->setKey(42, "HH",	  "Closed hi-hat",			   3,	-1, 'x');
		this->setKey(44, "HH",	  "Pedal hi-hat",			   3,	42, 'p');
		this->setKey(46, "HH",	  "Open hi-hat",			   3,	42, 'X');

		this->setKey(41, "FT",	  "Low floor tom",			   4,	-1, 'o');
		this->setKey(43, "fT",	  "High floor tom", 		   5,	-1, 'o');
		this->setKey(45, "T1",	  "Low tom",				   6,	-1, 'o');
		this->setKey(47, "T2",	  "Low mid tom",			   7,	-1, 'o');
		this->setKey(48, "T3",	  "High mid tom",			   8,	-1, 'o');
		this->setKey(50, "T4",	  "High tom",				   9,	-1, 'o');

		this->setKey(51, "R",	  "Ride cymbal",			  10,	-1, 'x');
		this->setKey(53, "R",	  "Ride bell",				  10,	53, 'b');
		this->setKey(59, "R2",	  "Ride cymbal 2",			  11,	-1, 'X');

		this->setKey(49, "C",	  "Crash cymbal",			  12,	-1, 'x');
		this->setKey(57, "C2",	  "Crash cymbal 2", 		  13,	-1, 'X');

		this->setKey(52, "CH",	  "China cymbal",			  14,	-1, 'x');
		this->setKey(54, "TA",	  "Tambourine", 			  15,	-1, 'x');
		this->setKey(55, "SP",	  "Splash cymbal",			  16,	-1, 'x');
		this->setKey(56, "CB",	  "Cowbell",				  17,	-1, 'x');
		this->setKey(58, "VS",	  "Vibra-slap", 			  18,	-1, 'x');

		break;
		}


	}
}

void TabDrumkit::setKitName(char * str)
{
	if (NoError)
	{
		unsigned long len = strlen(str);
		delete drumkitName;

		drumkitName = new char [len];
		if (drumkitName==NULL) NoError = 0;
		else
		{
			for (unsigned long i=0; i<len; i++)
			{
				drumkitName[i] = str[i];
			}
			drumkitName[len] = '\0';
		}
	}
}

void TabDrumkit::shortName(unsigned char no, char * str)
{
	if (no < 128 && NoError)
	{
		short len = 0;
		do {
			shortNames[no * 5 + len] = str[len];
			len++;
		} while(len<4);

		if (str[len]!='\0') shortNames[no * 5 + len] = '\0';
	}
}

void TabDrumkit::longName(unsigned char no, char * str)
{
	if (no < 128 && NoError)
	{
		unsigned long len = strlen(str);
		delete longNames[no];

		longNames[no] = new char [len];
		if (longNames[no]==NULL) NoError = 0;
		else
		{
			for (unsigned long i=0; i<len; i++)
			{
				longNames[no][i] = str[i];
			}
			nameLen[no] = len;
			longNames[no][len] = '\0';
		}
	}
}

void TabDrumkit::order(unsigned char no, short ord)
{
	if (no<128 && NoError) Order[no] = ord;
}

void TabDrumkit::base(unsigned char no, short base_)
{
	if (no<128 && NoError) Base[no] = base_;
}

void TabDrumkit::mark(unsigned char no, char ch)
{
	if (no<128 && NoError) Mark[no] = ch;
}

void TabDrumkit::clear()
{
	// Marks all keys undefined and frees their reserved memory
	short j;

	for (short i=0; i<128; i++)
	{
		for (j=(i*5)+1; j<(i+1)*5; j++)
			shortNames[j] = 0;
		shortNames[i*5] = '\0';

		delete [] longNames[i];
		longNames[i] = 0;

		nameLen[i] = 0;
		Order[i] = -1;
		Base[i] = -1;
		Mark[i] = 0;
	}

	delete drumkitName;
	delete shortNameRet;
	drumkitName = 0;
	shortNameRet = 0;

	NoError = 1;
}

char * TabDrumkit::kitName() const
{
	return drumkitName;
}

char * TabDrumkit::shortName(unsigned char no, unsigned short get_len)
{
	// get_len = how many chars to return

	if (no < 128 && NoError)
	{
		unsigned char len2 = 0;
		unsigned short i;

		while(shortNames[no * 5 + len2]!='\0' && len2 < 5)
			len2++;

		delete [] shortNameRet;
		shortNameRet = new char[get_len+1];

		if (shortNameRet!=NULL)
		{
			for (i=0; i<len2; i++)
			{
				shortNameRet[i] = shortNames[no * 5 + i];
			}
			for (i=len2; i<get_len; i++)
				shortNameRet[i] = ' ';
			shortNameRet[get_len] = '\0';
		}

		return shortNameRet;
	}
}

char * TabDrumkit::longName(unsigned char no) const
{
	if (no<128 && NoError) return longNames[no];
}

short TabDrumkit::order(unsigned char no) const
{
	if (no<128 && NoError) return Order[no];
}

short TabDrumkit::maxorder() const
{
	short ret = 0;
	for (unsigned char i=0; i<128; i++)
	{
		if (Order[i] > ret) ret = Order[i];
	}
	return ret;
}

short TabDrumkit::base(unsigned char no) const
{
	if (no<128 && NoError) return Base[no];
}

char TabDrumkit::mark(unsigned char no) const
{
	if (no<128 && NoError) return Mark[no];
}

void TabDrumkit::printDrum(unsigned char no)
{
	// 'TabDrumkit::printAll(...)'
	// Prints data of drum number [no]
	// in order 'no symbol mark order name'

	if (base(no) > 0) {
		cout << short(no) << " " <<
				"'" << shortName(base(no), 4) << "' '" <<
				mark(base(no)) << "' " <<
				order(no) << " " <<
				longName(no) <<
				" BASE=" << base(no) << endl;
	}
	else {
		cout << short(no) << " " <<
				"'" << shortName(no, 4) << "' '" <<
				mark(no) << "' " <<
				order(no) << " " <<
				longName(no) << endl;
	}
}

void TabDrumkit::testprint()
{
	// Prints data of all defined drums

	cout << "KIT: " << kitName() << endl;
	cout << "no symbol mark order name" << endl;
	for (short i = 0; i < 128; i++)
	{
		if (order(i) != -1) printDrum(i);
	}
}

#endif
