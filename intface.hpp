// INTFACE.HPP
// Text-based / console interface for drum tabber
// 12-26-2002 alkimohap

#ifndef INTFACE_HPP
#define INTFACE_HPP

#include "midifile.hpp"
#include "tab.hpp"

class TabInterface {
public:
	TabInterface();
	~TabInterface();

	int activate(int argc, char * argv[], int & requestKeypress);
	short good() { return NoError; }
	void printerror();

private:
	long selectTrack();
	short arghandle(int argc, char * argv[]);
	void checkpaths();

	// Drum kit, tabbing options,
	// MIDI file access and tabulature converter
	vector<TabDrumkit> drumkits;
	TabOptions * options;
	MIDIFile * midifile;
	TabTrack * tab;

	// File names and tabulature name
	char * titleText;
	char * midifileName;
	char * tabfileName;

	char * inputname;	// argIndir + midifileName
	char * outputname;	// argOutdir + tabfileName

	// Miscellaneous
	short NoError;
	short ErrorNum;

	char dirslash;			// '\' or '/' depending on DOS_VERSION UNIX_VERSION

	// For debug and testing
	void testdrumkit();

};

// Return values for TabInterface::activate():
// 0 - everything ok
// 2 - midi file error
// 3 - tab file error

#include "intface.cpp"

#endif
