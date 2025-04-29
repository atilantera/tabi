// DRUMKIT.HPP
// Drum kit system needed when interpreting
// MIDI notes to different drums in drum tab.
// 12-26-2002 alkimohap

#ifndef DRUMKIT_HPP
#define DRUMKIT_HPP

// When a General MIDI track is drum track,
// every key is interpreted as a separate drum.

// Drum kits contain information about
// all the drums that may be used in drum tab.
// Each key has its own drum name, symbol, note mark
// and vertical positioning order in tab line.

// In GM default drum kit all 128 keys are not defined.

class TabDrumkit {
public:
	TabDrumkit();
	TabDrumkit(const TabDrumkit&);
	~TabDrumkit();
	TabDrumkit & operator= (const TabDrumkit &);

	void read(ifstream & infile);
	void write(ofstream & outfile);

	short good() const { return NoError; }

	void setBuiltIn(short kit);
	void setKitName(char * str);
	void setKey(unsigned char no, char * sName, char * lName, short ord, short base_, char ch);
	void shortName(unsigned char no, char * str);
	void longName(unsigned char no, char * str);
	void order(unsigned char no, short ord);
	void base(unsigned char no, short base_);
	void mark(unsigned char no, char ch);
	void clear();

	char * kitName() const;
	char * shortName(unsigned char no, unsigned short get_len);
	char * longName(unsigned char no) const;
	short order(unsigned char no) const;
	short maxorder() const;
	short base(unsigned char no) const;
	char mark(unsigned char no) const;

	void printDrum(unsigned char no);
	void testprint();

private:

	char * drumkitName;
	char * longNames[128];		// drum name
	char shortNames[5*128]; 	// short text symbol for each drum
	unsigned char nameLen[128]; // shortNames
	short Order[128];			// vertical order in bar in final drum tab = line number.
								// 0 = lowest, maxorder - 1 = highest
	short Base[128];			// if (>=0): drum is marked on another drum's line
								// (for ex. hi-hat: closed/open/pedal/bell)
	char Mark[128]; 			// Character that will be used to mark notes
								// played by this drum (usually 'o' or 'x')

	char * shortNameRet;		// For shortName()
	short NoError;
};

#include "drumkit.cpp"

#endif
