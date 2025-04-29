// MIDITRK.HPP
// Basic structures for storing MIDI data read from file
// 12-6-2002 0:02 Artturi Tilanterä artturi@mbnet.fi

#ifndef MIDITRK_HPP
#define MIDITRK_HPP

#include <vector.h>

// Simple classes for *single* notes and other MIDI controller data read from file

class TrackEvent {
public:
	TrackEvent() { }
	TrackEvent(unsigned long tick): Tick(tick) { }
	void setTime(unsigned long tick)	{ Tick = tick; }
	unsigned long getTime() const		{ return Tick; }

protected:
	unsigned long Tick;
};

class TrackNote : public TrackEvent {
public:
	TrackNote(unsigned long tick, unsigned long key): TrackEvent(tick), keyNumber(key) { }
	void setKey(unsigned char key)		{ keyNumber = key; }
	void setTime(unsigned long tick)	{ Tick = tick; }
	unsigned char getKey() const		{ return keyNumber; }
	unsigned long getTime() const		{ return Tick; }
	// void setVel(unsigned short num, unsigned char vel);
	// unsigned char getVel() const;

private:
	unsigned char keyNumber;
	// unsigned char velocity;
};

class TrackMeter : public TrackEvent {
public:
	TrackMeter(unsigned long tick, unsigned char numerator, unsigned char denominator);
	void setMIDIstyle(unsigned char numerator, unsigned char denominator);
	void set(unsigned char numerator, unsigned char denominator);
	unsigned char getNum() const { return Numerator; }
	unsigned char getDen() const { return Denominator; }

private:
	unsigned char Numerator;
	unsigned char Denominator;
};

class TrackTempo : public TrackEvent {
public:
	TrackTempo(unsigned long tick, unsigned long MicrosecsPerQuarter);
	void setMsec(unsigned long MicrosecsPerQuarter);
	void setQ(float QuartersPerMin);
	unsigned long getMsec() const;
	float getQ() const;

private:
	unsigned long tempo;
};

class TrackKeySig : public TrackEvent {
public:
	TrackKeySig(unsigned long tick, char sf, unsigned char minortrue);
	void set(char sf, unsigned char minortrue);
	void get(char &sf, unsigned char &minortrue) const;
	char getSharps() const;
	unsigned char getMajmin() const;

private:
	char sharps;		  // -7=7 flats, 0=C maj 7=7 sharps
	unsigned char majmin; // 0=major, 1=minor
};

// Classes for MIDI tracks to store note and other MIDI controller data

// Conductor track contains only meter, tempo and possible key signature

class MIDIConductorTrack {
public:
	MIDIConductorTrack();
	~MIDIConductorTrack();

	// These parametres' values are same as in the MIDI file
	// (tempo microseconds per quarter note vs. quarters per second,
	//	actual denominator is 2 exp(value); 2=quarter, 3=eigth...)

	// addMeter() parameters are same as TrackMeter::setMIDIstyle
	// getMeter() returns actual value of denominator

	// getTempo(..unsigned long &tempo) = tempo in microseconds per quarter note
	// getTempo(..float &tempo) 		= tempo in quarter notes per minute

	void addTempo(unsigned long TimeInTicks, unsigned long MicrosecsPerQuarter);
	void addMeter(unsigned long TimeInTicks, unsigned char numerator,
	  unsigned char denominator);
	void addKeySig(unsigned long TimeInTicks, char sf, unsigned char majmin);
	void setDiv(short ticksPerBeat);

	void getTempo(unsigned long number, unsigned long &time, unsigned long &tempo) const;
	void getTempo(unsigned long number, unsigned long &time, float &tempo) const;
	void getMeter(unsigned long number, unsigned long &time,
	  unsigned char &numerator, unsigned char &denominator) const;
	void getKeySig(unsigned long number, unsigned long &time,
	  char &sf, unsigned char &majmin) const;
	short getDiv() const { return TicksPerBeat; }

	unsigned long getTempoCount() const { return Tempos.size(); }
	unsigned long getMeterCount() const { return Meters.size(); }
	unsigned long getKeySigCount() const { return KeySigs.size(); }

private:

	vector<TrackTempo> Tempos;
	vector<TrackMeter> Meters;
	vector<TrackKeySig> KeySigs;

	// This is the actual number of ticks per quarter note
	short TicksPerBeat;
};

// Note track contains notes (data: note on, time and key)

class MIDINoteTrack {
public:
	MIDINoteTrack();
	~MIDINoteTrack() { Notes.clear(); }
	void addNote(unsigned long TimeInTicks, unsigned char keynumber, unsigned char velocity);
	void getNote(unsigned long number, unsigned long &time, unsigned char &key) const;
	unsigned long getNoteCount() const { return Notes.size(); }

private:
	vector<TrackNote> Notes;
};

// MIDI Track contains general data about a track read from file:
// possible track name, first patch change and channel of first note-on
// NOTE: channel values are 1-16, NOT 0-15

class MIDITrack {
	public:
	MIDITrack();
	MIDITrack(const MIDITrack &);
	MIDITrack & operator= (const MIDITrack &);

	short good() { return NoError; }

	void setName(char * name);
	void setName1(unsigned char data[255], unsigned char len);
	void setPatch(unsigned char Patch);
	void setPatch1(unsigned char Patch);
	void setChannel(unsigned char ch);
	void setChannel1(unsigned char ch);    

	char * getName() const;
	void printName(unsigned char space) const;
	unsigned short getNameLen() const { return NameLength; }
	unsigned char getPatch() const;
	unsigned char getChannel() const { return channel; }
	
	private:
	char * pName;
	unsigned short NameLength;

	unsigned char patch;
	unsigned char channel;

	short NoError;
};

#include "miditrk.cpp"

#endif
