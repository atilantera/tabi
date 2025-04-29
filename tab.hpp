#ifndef TAB_HPP
#define TAB_HPP

#include <vector.h>
#include "drumkit.hpp"

// Classes to store tabulature data
class TabNote {
public:
	TabNote(): key(0), pos(0) { }

	unsigned char key;
	unsigned short pos;  // position = characters from beginning of bar

	// unsigned char velocity;
};

class TabBar {
public:
	TabBar();
	TabBar(unsigned char num, unsigned char den, float tpo):
				numerator(num), denominator(den), tempo(tpo) { }
/*	  TabBar(const TabBar&);
	TabBar& operator= (const TabBar &); */

	short good() { return NoError; }

	void setNum(unsigned char num) { numerator = num;}
	void setDen(unsigned char den) { denominator = den; }
	void setTempo(float QuartersPerMin) { tempo = QuartersPerMin; }
	void setTime(unsigned long d) { deltaStart = d; }
	void setTimeLen(unsigned long l) { deltaLength = l; }
	void setCharPDen(short cpd) { charsPerDen = cpd; }
	void addNotes(vector<TabNote> & allnotes);
	void addNote(TabNote note) { Notes.push_back(note); }

	void clear();
	// void sortNotes();	// by 1: key, 2: pos (not required)

	unsigned char getNum() const { return numerator; }
	unsigned char getDen() const { return denominator; }
	unsigned long getTime() const { return deltaStart; }
	unsigned long getTimeLen() const { return deltaLength; }
	unsigned short getCharLen() const { return numerator * charsPerDen; }
	short		  getCharPDen() const { return charsPerDen; }

	float getTempo() const { return tempo; }
	short getNoteCount() const { return Notes.size(); }
	void getUsed(char * used);
	class TabNote getNote(unsigned short num);

private:
	unsigned short length;
	unsigned long deltaStart;
	unsigned short deltaLength;

	unsigned char numerator;
	unsigned char denominator;
	short charsPerDen;
	float tempo;

	vector<TabNote> Notes;

	short NoError;
};

// General options for tabulature generation and output
const unsigned short MINCOLS = 40;
const unsigned short MINROWS = 20;

class TabOptions {
public:
	TabOptions();
	TabOptions(const TabOptions&);
	TabOptions & operator= (const TabOptions&);
	~TabOptions();

	void read(ifstream & infile);
	void write(ofstream & outfile);

	// Tab writing
	void minQ(unsigned char minQ);
	void tripletlength(unsigned char tl);
	void meterline(unsigned char ml);
	void meterlimit(unsigned char ml);
	void meterchanges(unsigned char mc);
	void cols(unsigned short Cols);
	void rows(unsigned short Rows);
	void userows(unsigned char yesno);
	void pagenum(unsigned char yesno);
	void writelegend(unsigned char yesno);
	void drumnamelen(unsigned char len);
	void rowsbetwbars(unsigned char r);
	void drumkit(class TabDrumkit * Drumkit);

	// Interface
	void setinpath(char * path);
	void setoutpath(char * path);
	void autoselect(unsigned char at);
	void windowautoclose(unsigned char w);
	void titlealign(unsigned char ta);
	void rowsbeforetitle(unsigned char rbt);
	void rowsfortitle(unsigned char rft);

	// Tab writing
	unsigned char minQ() const			  { return minQsize; }
	unsigned char tripletlength() const   { return tripletMode; }
	unsigned char meterline() const 	  { return meterLine; }
	unsigned char meterlimit() const	  { return meterLimit; }
	unsigned char meterchanges() const	  { return meterChanges; }
	unsigned short cols() const 		  { return pageCols; }
	unsigned short rows() const 		  { return pageRows; }
	unsigned char userows() const		  { return rowLimit; }
	unsigned char pagenum() const		  { return pageNumbers; }
	unsigned char writelegend() const	  { return writeDrumLegend; }
	unsigned char drumnamelen() const	  { return drumNameLength; }
	unsigned char rowsbetwbars() const	  { return rowsBetweenBars; }
	TabDrumkit * drumkit() const		  { return drumKit; }

	// Interface
	unsigned char autoselect() const	  { return autoSelectTrack; }
	unsigned char windowautoclose() const { return windowAutoClose; }
	unsigned char titlealign() const	  { return titleAlign; }
	unsigned char rowsbeforetitle();
	unsigned char rowsfortitle();
	void getinpath(char * path);
	void getoutpath(char * path);

	char * inPath;					// Directory for MIDI files
	char * outPath; 				// Directory for tab files

private:
	unsigned char minQsize; 		// minimum of number of characters per quarter note
	unsigned char tripletMode;		// triplet length; 1 =	75% quarter note
									//				   2 = 150% quarter note

	unsigned char meterLine;		// Below barline:
	unsigned char meterLimit;		// 0=no
									// 1=yes, when (bar length > meterLimit)

	unsigned char meterChanges; 	// Show meter changes? 0=no 1=yes

	unsigned short pageCols;		// limit: characters per line in page
	unsigned short pageRows;		// limit: lines per page
	unsigned char rowLimit; 		// use pageRows limit? 0=no 1=yes
	unsigned char pageNumbers;		// 0=no 1=yes (position=left) 2=yes (position=right)

	unsigned char writeDrumLegend;	// 0=no 1=yes (see drumNameLength)
	unsigned char drumNameLength;	// length of drum name ("t2 |----------------|") <- value is 3 (characters)

	unsigned char rowsBetweenBars;	// Empty rows to separate bar lines

	TabDrumkit * drumKit;			// drum kit used

	unsigned char autoSelectTrack;	// Try find automatically a proper MIDI track
									// to write the tab

	unsigned char windowAutoClose;	// This must be set to 1 if the program is run
									// in a window which will be automatically
									// closed when program run ends.

									// If title text for tab is set:
	unsigned char titleAlign;		// 0=left 1=center 2=right
	unsigned char rowsBeforeTitle;	// Empty lines before title text
	unsigned char rowsForTitle; 	// Empty lines before first bar line
};

// Main class for tabulature generation from MIDIFile class
// The most important members are import(...) and writetab(..)
class TabTrack {
public:
	TabTrack();
	TabTrack(const TabTrack&);
	TabTrack & operator= (const TabTrack &);

	// good() returns 1 if everything is OK, zero if out of memory
	short good() const { return NoError; }
	short err() const { return ErrorNum; }
	void printerror() const;

	// short err() const { return ErrorNum; }
	// char * errDesc() const;

	void import(const MIDIFile * src, short track, TabOptions options);
	unsigned long getBarCount() const { return BarCount; }
	short writetab(char * filename, char * titleText, TabOptions options);

	void testprintQuantz();

private:
	unsigned long barNoteCount(unsigned long barno, unsigned long firstnote);  // Search in Bars

	char quantizeBar(unsigned long barno, unsigned long firstnote,
	   unsigned long notecount, const TabOptions & options);

	char checkKeys(unsigned char & allKeys, unsigned char & drumLines,
				   char * usedKeys, char * ignoredKeys,
				   char * lineInUse, unsigned char * lineKeys, TabDrumkit & drumkit);

	void printIgnoredKeys(char * ignoredKeys,
		unsigned long firstBar, unsigned long lastBar) const;

	void sortKeys(unsigned char allKeys, unsigned char drumLines,
				  char * usedKeys, short * usedKeysPos, TabDrumkit & drumkit,
				  unsigned char * sortedKey, short * sortedPos,
				  unsigned char * lineKeys);

	void formatBars(char * outbars, TabDrumkit & drumkit, unsigned char drumLines,
					unsigned char * lineKeys,
					unsigned short colCount, unsigned char drumnamelen,
					unsigned long firstBar, unsigned long lastBar);

	void writeNotes(char * outbars, char * meterlineText, unsigned short colCount,
					TabDrumkit & drumkit, const TabOptions & options, char * usedKeys,
					short * usedKeysPos, unsigned char drumLines,
					unsigned long firstBar, unsigned long lastBar);

	char writeInfoline(char * infoText, const TabOptions & options,
					unsigned short colCount,
					unsigned long firstBar, unsigned long lastBar);

	void writeMeterline(char * meterlineText, unsigned short colCount,
					   unsigned long firstBar, unsigned long lastBar,
					   char meterline);

	void writePageNumbers(char * pageText, unsigned short pageCount,
									char * filename, char * title, unsigned short cols);

	vector<TabBar> Bars;		  // Empty bars with only tempo etc.
	vector<TabBar> Bars2;		  // Bars with quantized notes
	unsigned long BarCount;

	// MIDI data
	MIDIFileHeader midiheader;
	MIDIConductorTrack Conductor; // for accessing data from MIDIFile
	MIDINoteTrack Notes;
	MIDITrack Track;

	short trackno;	   // selected MIDI track to tab = addBar(, track, ..)
	short condtrack;   // conductor for MIDI track (see import(..) )

	// Error handling
	short NoError;
	short ErrorNum;
	unsigned long ErrorRef;

};

#include "tab.cpp"

#endif
