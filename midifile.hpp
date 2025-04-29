// MIDIFILE.HPP
// For reading standard MIDI files

// NOTE: this version accepts only tempo, time signature and note-ons.
// It ingores all other data, including MIDI channels (1-16).
// Every track in file is stored separately.

// 12-25-2002 alkimohap

#ifndef MIDIFILE_HPP
#define MIDIFILE_HPP

#include <fstream.h>
#include <math.h>		// MIDIFile::listTracks

#include "invert.h"
#include "miditrk.hpp"

// NOTE ABOUT "INVERT.H"
// It seems IOSTREAM file i/o uses different byte order
// than the one that is in use in standard MIDI files.
// When reading variables that consists of more than
// one byte the byte order is *inverted* from the original source.
//
// An example:
// Bytes in file:					4D 54 68 64 (1297377380)
// Same when read to unsigned long: 64 68 54 4D (1684558941)
// Unsigned long after invb(..)   : 4D 54 68 64 (1297377380)
//
// This is the reason for using INVERT.H.

class MIDIFileHeader
{
	// Standard MIDI file main header. (MThd)
	// After reading data from file to this
	// .check() returns 1 if a standard MIDI file header is read.

	// .getFormat() returns file format (0-2), refer to .check for details
	// .getTracks() returns number of tracks in the file
	// .getDiv() returns time scale of the file: delta time ticks per quarter note

public:
	MIDIFileHeader();
	short check();
	unsigned long getLength() const { return invb(length); }
	unsigned short getFormat() const { return invb(format); }
	unsigned short getTracks() const { return invb(tracks); }
	unsigned short getDiv() const { return invb(ticksPerQuarter); }
	void read(ifstream &);
	
private:
	unsigned long MThd;
	unsigned long length;
	unsigned short format;
	unsigned short tracks;
	unsigned short ticksPerQuarter;
};

class MIDIFileTrackHeader
{
	// Standard MIDI file track header. (MTrk)
	// After reading data from file to this
	// .check() returns 1 if a standard MIDI file header is read.
	
public:
	MIDIFileTrackHeader();

	short check();
	unsigned long getMTrk() const { return invb(MTrk); }
	unsigned long getLength() const { return invb(length); }
	void read(ifstream &infile);
	void clear() { MTrk = 0; length = 0; }

private:
	unsigned long MTrk;
	unsigned long length;		 
};

class MIDIFile {
	// MIDIFile class.
	// This is for reading standard MIDI files and
	// storing note and other MIDI data.
	
	// NOTE: .good() must be called also after
	// the constructor function MIDIFile()	

public:
	MIDIFile();
	MIDIFile(const MIDIFile&);
	MIDIFile& operator= (const MIDIFile &);
	~MIDIFile();

	short good() const { return NoError; }
	short err() const { return ErrorNum; }
	char * errDesc() const;
	unsigned long errPos() const { return ErrorPos; }
	void errPos_cout() const;

	void read(char * filename);
	void clear();

	short getTrackCount() const { return TrackCount; }
	short getConductorCount() const { return ConductorCount; }
	unsigned long getNoteCount(unsigned short track) const;

	MIDIFileHeader getHeader() const { return header; }
	MIDIConductorTrack getCondTrk(unsigned short num) const;
	MIDINoteTrack getNoteTrk(unsigned short num) const;
	MIDITrack getTrack(unsigned short num) const;

	void listTracks(short selection);

	short getTrackChannel(unsigned short num) const;
	char * getTrackName(unsigned short num) const;

	// debug (default = no)
	void report() const;
	void debugOn(); 	 // Creates file debugout.txt
	void debugOff();
	short getDebug() { return debug; }
	void printNoteCount(unsigned short track);

private:
	void alloctemp();
	void freetemp();
	void readTracks();
	void readTrack(unsigned long length);
	unsigned long readDelta(unsigned long & readcounter);
	void readEvent(unsigned long & readcounter, unsigned char & running,
				   unsigned long * noteChannels);
	void readMeta(unsigned long & readcounter, char runningOn, unsigned char b2);

	// Final MIDI data containers
	MIDIFileHeader header;
	vector<MIDIConductorTrack> Conductors;
	vector<MIDINoteTrack> Notes;
	vector<MIDITrack> Tracks;

	unsigned short ConductorCount;
	unsigned short TrackCount;

	// For temporary data, used by read(char *)
	MIDIConductorTrack * ConductorTrack;
	MIDINoteTrack * NoteTrack;
	MIDITrack * Track;

	// Miscellaneous
	ifstream infile;
	unsigned long last_time; // counter for combining delta times in MIDI
	short NoError;
	short ErrorNum;
	unsigned long ErrorPos;

	// Debug
	short debug;
	ofstream debugout;
};

#include "midifile.cpp"

#endif
