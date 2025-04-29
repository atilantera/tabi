#ifndef INTFACE_CPP
#define INTFACE_CPP

#include "string.hpp"

TabInterface::TabInterface()
{
	NoError = 1;
	ErrorNum = 0;

	inputname = 0;
	outputname = 0;

	if(OS_TYPE == 1) dirslash = 92; // '\'
	if(OS_TYPE == 2) dirslash = 47; // '/'

	// File names
	midifileName = new char[256];
	tabfileName = new char[256];
	titleText = 0;

	if (midifileName==NULL)
	{
		NoError = 0;
		ErrorNum = 1;
	}
	else
	{
		midifileName[0] = '\0';
	}

	if (tabfileName==NULL)
	{
		NoError = 0;
		ErrorNum = 1;
	}
	else
	{
		tabfileName[0] = '\0';
	}

	// MIDI file input
	if (NoError)
	{
		midifile = new MIDIFile;
		if (midifile==NULL)
		{
			NoError = 0;
			ErrorNum = 1;
		}
		else
		{
			if (!midifile->good())
			{
				NoError = 0;
				ErrorNum = 1;
			}
		}
	}

	// Tabulature converter, output
	if (NoError)
	{
		tab = new TabTrack;
		if (tab == NULL)
		{
			NoError = 0;
			ErrorNum = 1;
		}
		else
		{
			if (tab->good() == 0)
			{
				NoError = 0;
				ErrorNum = 1;
			}
		}
	}

	// Drum kits: setting built-in GM kit
	if (NoError)
	{
		TabDrumkit GMdefault;
		GMdefault.setBuiltIn(0);
		drumkits.push_back(GMdefault);
	}

	// Options
	if (NoError)
	{
		options = new TabOptions;
		if (options==NULL)
		{
			NoError = 0;
			ErrorNum = 1;
		}
		else
		{
			// Load options from file if possible
			ifstream infile;

			if(OS_TYPE==1) infile.open("dtab.dat", ios::bin | ios::in);
			if(OS_TYPE==2) infile.open("/usr/dtab.dat", ios::bin | ios::in);
			if (infile.good())
			{
				options->read(infile);
				infile.close();
			}
			else
			{
				cout << "Starting with default options." << endl;
			}

			// Remember: in TabOptions::write() drumkit is disabled
			options->drumkit(&(drumkits[0]));
		}
	}

	if (NoError != 1) printerror();
}

TabInterface::~TabInterface()
{
	// Write tabbing options to file if possible
	ofstream outfile;

	if(OS_TYPE == 1) outfile.open("dtab.dat", ios::bin | ios::out);
	if(OS_TYPE == 2) outfile.open("/usr/dtab.dat", ios::bin | ios::in);

	if (outfile.good()) options->write(outfile);
	outfile.close();

	drumkits.clear();

	delete options;
	delete midifile;
	delete tab;

	delete [] midifileName;
	delete [] tabfileName;
	delete [] inputname;
	delete [] outputname;
}

int TabInterface::activate(int argc, char * argv[], int & requestKeypress)
{
	// Return codes:
	// 0 everything ok
	// 1 command-line parametres / filename error
	// 2 midi file / tabbing error

	// requestKeypress will be set to
	// the lowest return code on with the main program
	// can exit without notifying user about it.
	requestKeypress = 2-options->windowautoclose();

	if (arghandle(argc, argv) == 0) return 1;

	if (strlen(midifileName) == 0)
	{
		cout << "midi file: ";
		cin >> midifileName;
		if (strlen(midifileName) == 0)
		{
			return 1;
		}
	}

	if (strlen(tabfileName) == 0)
	{
		cout << "tab file: ";
		cin >> tabfileName;
		if (strlen(tabfileName) == 0)
		{
			return 1;
		}
	}

	// Input or output directory set by command line args?
	checkpaths();

	cout << "in: " << inputname << endl;
	cout << "out: " << outputname << endl;

	// Read the midi file
	midifile->read(inputname);

	if (!midifile->good())
	{
		cout << midifile->errDesc();
		midifile->errPos_cout();
		cout << " (midifile error " << midifile->err() << ")" << endl;
		return 2;
	}

	// Select track to tab
	long track = 0;
	track = selectTrack();
	if (track == -1) return 2;	// Unable to select track

	// Title of tabulature
	titleText = 0;
/*
	delete [] titleText;
	titleText = new char[options->cols()+1];
	if(titleText != NULL)
	{
		cout << "Title: ";
		cin.ignore(255, '\n');
		cin.getline(titleText, options->cols()+1);
	}
*/
	// Quantize midi notes and create drum tab
	tab->import(midifile, (short)track, *options);

	if (tab->good() == 0)
	{
		tab->printerror();
		return 2;
	}
	else
	{
		// tab->testprintQuantz();
		tab->writetab(outputname, titleText, *options);

		if (tab->good() == 0)
		{
			tab->printerror();
			return 2;
		}
	}

	return 0;
}

void TabInterface::printerror()
{
	switch(ErrorNum)
	{
	case 1:
		cout << "Out of memory" << endl;
		break;
	}
}

long TabInterface::selectTrack()
{
	// Selects the midi track to write a tab.

	// Returns -1 if cannot select track.
	// (Otherwise returns the number of the selected track.)
	if (midifile->getTrackCount()==0) return -1;
	long track;

	// Try to auto-select?
	// Method: return first track that has notes and
	// MIDI channel of which is set to 10
	if (options->autoselect() == 1)
	{
		unsigned short i;
		char channelOK = 0; // Boolean

		// Try to find a track with channel set to 10
		i = 0;
		while(i < midifile->getTrackCount() || channelOK != 1)
		{
			if (midifile->getTrackChannel(i) == 10 &&
			   midifile->getNoteCount(i) > 0)
			{
				channelOK = 1;
			}
			i++;
		}
		if (channelOK == 1)
		{
			// Inform the used that a track has been selected.
			char * trackname;
			trackname = midifile->getTrackName(i-1);

			cout << "Track " << i-1;
			if (strlen(trackname) > 0) {
				cout << " (" << trackname << ")";
			}
			cout << " selected." << endl;
			return i-1;
		}
		// Search failed, track has to be selected manually.
	}

	// Manual select
	midifile->listTracks(1);
	while(1)
	{
		cout << endl << "Select track: ";
		cin >> track;
		if (track < 0) return -1;
		if (midifile->getNoteCount((unsigned short)track) > 0) return track;
		if (track == 0) return -1;
		cout << endl;
	}
}

short TabInterface::arghandle(int argc, char * argv[])
{
	// Handles command line parametres.
	// Returns: 1 continue run after calling this function
	//			0 program should quit after calling this function

	int i = 1;
	int argset; 			  // If 0, argument by argnext is:
	int argnext = 0;		  // 1 - input directory
							  // 2 - output directory
	int fileargs = 0;		  // 0 - midifile or tabfile (fileargs = 0 or 1)
	char arg[80];			  // argv[i] without '\0'

	while(i < argc)
	{
		argset = 0;

		if(argset==0 && fileargs == 0 && argnext == 0)
		{
			midifileName = argv[i];
			fileargs = 1;
			argset = 1;
		}

		if(argset==0 && fileargs == 1 && argnext == 0)
		{
			tabfileName = argv[i];
			fileargs = 2;
			argset = 1;
		}

		i++;
	}

	return 1;
}

void TabInterface::checkpaths()
{
	unsigned long i;
	unsigned long len1;
	unsigned long len2;
	short needsSlash;

	char * inPath;
	char * outPath;
	inPath = 0;
	outPath = 0;
	// options->getinpath(inPath);
	// options->getoutpath(outPath);

	if(inPath != NULL)
	{
		needsSlash = 0;

		len1 = strlen(inPath);
		len2 = strlen(midifileName);
		if(inPath[len1-1] != dirslash) needsSlash = 1;

		inputname = new char[len1 + needsSlash + len2 + 1];

		if(inputname == NULL)
		{
			NoError = 0;
			ErrorNum = 1;
		}
		else
		{
			for(i = 0; i < len1; i++)
				inputname[i] = inPath[i];

			if(needsSlash == 1) inputname[len1] = dirslash;

			for(i = 0; i < len2; i++)
				inputname[len1 + needsSlash + i] = midifileName[i];

			inputname[len1 + needsSlash + len2] = '\0';
		}
	}
	else inputname = midifileName;

	if(outPath != NULL)
	{
		needsSlash = 0;

		len1 = strlen(outPath);
		len2 = strlen(tabfileName);
		if(outPath[len1-1] != dirslash) needsSlash = 1;

		outputname = new char[len1 + needsSlash + len2 + 1];

		if(outputname == NULL)
		{
			NoError = 0;
			ErrorNum = 1;
		}
		else
		{
			for(i = 0; i < len1; i++)
				outputname[i] = outPath[i];

			if(needsSlash == 1) outputname[len1] = dirslash;
			for(i = 0; i < len2; i++)
				outputname[len1 + needsSlash + i] = tabfileName[i];

			outputname[len1 + needsSlash + len2] = '\0';
		}
	}
	else outputname = tabfileName;
}

#endif
