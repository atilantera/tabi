// MIDITRK.CPP
// members functions of "miditrk.hpp" classes
// 12-25-2002 alkimohap

#ifndef MIDITRK_CPP
#define MIDITRK_CPP

#include <stdlib.h>

TrackMeter::TrackMeter(unsigned long tick, unsigned char numerator, unsigned char denominator):
	TrackEvent(tick) { setMIDIstyle(numerator, denominator); }

void TrackMeter::setMIDIstyle(unsigned char numerator, unsigned char denominator)
{
	Numerator = numerator;
	switch(denominator) {
	case 1: { Denominator = 2; break; }
	case 2: { Denominator = 4; break; }
	case 3: { Denominator = 8; break; }
	case 4: { Denominator = 16; break; }
	case 5: { Denominator = 32; break; }
	case 6: { Denominator = 64; break; }
	case 7: { Denominator = 128; break; }
	default: { Denominator = 4; }
	}
}

void TrackMeter::set(unsigned char numerator, unsigned char denominator)
{
	Numerator = numerator;
	Denominator = denominator;
}

TrackTempo::TrackTempo(unsigned long tick, unsigned long MicrosecsPerQuarter):
	TrackEvent(tick), tempo(MicrosecsPerQuarter) { }

void TrackTempo::setMsec(unsigned long MicrosecsPerQuarter)
{	tempo = MicrosecsPerQuarter; }

void TrackTempo::setQ(float QuartersPerMin)
{	tempo = (unsigned long) (60 / QuartersPerMin) * 1000000; }

unsigned long TrackTempo::getMsec() const
{	return tempo; }

float TrackTempo::getQ() const
{
	return float(60 * (1000000 / double(tempo)));
}

TrackKeySig::TrackKeySig(unsigned long tick, char sf, unsigned char minortrue):
	TrackEvent(tick), sharps(sf), majmin(minortrue) { }

void TrackKeySig::set(char sf, unsigned char minortrue)
{
	sharps = sf;
	majmin = minortrue;
}

void TrackKeySig::get(char &sf, unsigned char &minortrue) const
{
	sf = sharps;
	minortrue = majmin;
}

char TrackKeySig::getSharps() const
{	return sharps;	}

unsigned char TrackKeySig::getMajmin() const
{	return majmin;	}

MIDIConductorTrack::MIDIConductorTrack()
{
	TicksPerBeat = 0;
}

MIDIConductorTrack::~MIDIConductorTrack()
{
	Tempos.clear();
	Meters.clear();
	KeySigs.clear();
}

void MIDIConductorTrack::addTempo(unsigned long TimeInTicks,
  unsigned long MicrosecsPerQuarter)
{	
	TrackTempo tempo1(TimeInTicks, MicrosecsPerQuarter);
	Tempos.push_back(tempo1);
}

void MIDIConductorTrack::addMeter(unsigned long TimeInTicks,
  unsigned char numerator, unsigned char denominator)
{
	TrackMeter meter1(TimeInTicks, numerator, denominator);
	Meters.push_back(meter1);
}

void MIDIConductorTrack::addKeySig(unsigned long TimeInTicks,
  char sf, unsigned char majmin)
{
	TrackKeySig keysig1(TimeInTicks, sf, majmin);
	KeySigs.push_back(keysig1);
}

void MIDIConductorTrack::setDiv(short ticksPerBeat)
{
	// Only positive values are accepted.
	// Negative values are in SMPTE combatible format, which is not supported.

	if (ticksPerBeat > 0)
		TicksPerBeat = ticksPerBeat;
}

void MIDIConductorTrack::getTempo(unsigned long number,
  unsigned long &time, unsigned long &tempo) const
{
	if (number < Tempos.size())
	{
		TrackTempo tempo1 = Tempos[number];
		time = tempo1.getTime();
		tempo = tempo1.getMsec();
	}
}

void MIDIConductorTrack::getTempo(unsigned long number,
  unsigned long &time, float &tempo) const
{
	if (number < Tempos.size())
	{
		TrackTempo tempo1 = Tempos[number];
		time = tempo1.getTime();
		tempo = tempo1.getQ();
	}
}

void MIDIConductorTrack::getMeter(unsigned long number, unsigned long &time,
	unsigned char &numerator, unsigned char &denominator) const
{
	if (number < Meters.size())
	{
		TrackMeter meter1 = Meters[number];
		time = meter1.getTime();
		numerator = meter1.getNum();
		denominator = meter1.getDen();
	}
}

MIDINoteTrack::MIDINoteTrack()
{
	Notes.reserve(256);
}

void MIDINoteTrack::addNote(unsigned long TimeInTicks, unsigned char keynumber, unsigned char velocity)
{
	TrackNote note1(TimeInTicks, keynumber);
	Notes.push_back(note1);
}

void MIDINoteTrack::getNote(unsigned long number, unsigned long &time, unsigned char &key) const
{
	if (number < Notes.size())
	{
		time = Notes[number].getTime();
		key = Notes[number].getKey();
	}
	else
	{
		time = 0;
		key = 255;
	}
}

MIDITrack::MIDITrack()
{
	NameLength = 0;
	NoError = 1;

	patch = 255;
	channel = 0;

	pName = new char[1];
	* pName = '\0';
	if (pName==NULL) { NoError = 0; }
}

MIDITrack::MIDITrack(const MIDITrack &op)
{
	NoError = op.NoError;

	patch = op.patch;
	channel = op.channel;

	if (NoError)
	{
		NameLength = op.getNameLen();
		pName = new char[NameLength + 1];
		if (pName == NULL)
		{
			NoError = 0;
			NameLength = 0;
		}
		else
		{
			for (unsigned short i = 0; i < NameLength + 1; i++)
			{
				pName[i] = op.pName[i];
			}
		}
	}
}

MIDITrack & MIDITrack::operator= (const MIDITrack &op)
{
	if (this == &op)
		return *this;

	NoError = op.NoError;

	patch = op.patch;
	channel = op.channel;

	if (NoError)
	{
		NameLength = op.getNameLen();
		pName = new char[NameLength + 1];
		if (pName == NULL)
		{
			NoError = 0;
			NameLength = 0;
		}
		else
		{
			for (unsigned short i = 0; i < NameLength + 1; i++)
			{
				pName[i] = op.pName[i];
			}
		}
	}

	return *this;
}

char * MIDITrack::getName() const
{
	return pName;
}

void MIDITrack::printName(unsigned char space) const
{
	short end;
	short i;
	if (NameLength > space) end = space;
	else end = NameLength;

	for (i = 0; i < end; i++)
		cout << pName[i];
	if (space > end)
		for (i = 0; i < (space-end); i++)
			cout << ' ';
}

void MIDITrack::setName(char * name)
{
	unsigned long len = strlen(name);

	delete [] pName;
	pName = new char [len+1];
	if (pName==NULL) NoError = 0;
	else
	{
		for (unsigned long i=0; i<len; i++)
		{
			pName[i] = name[i];
		}
		NameLength = len;
		pName[len] = '\0';
	}		
}

void MIDITrack::setName1(unsigned char data[255], unsigned char len)
{
	// Sets track name from straight data in midi file
	delete [] pName;
	pName = new char [len+1];
	if (pName==NULL) NoError = 0;
	else
	{
		for (short i=0; i<len; i++)
		{
			*(pName+i) = *(data+i);
		}
		NameLength = len;
		pName[len] = '\0';
	}
}

void MIDITrack::setPatch(unsigned char Patch)
{
	if (Patch<128) patch=Patch;
}

void MIDITrack::setPatch1(unsigned char Patch)
{
	patch=Patch;
}

void MIDITrack::setChannel(unsigned char ch)
{
	if (channel<=16) channel=ch;
}

void MIDITrack::setChannel1(unsigned char ch)
{
	// Sets MIDI channel number for track

	// Value ch is the original byte read from file,
	// bits are YYYYxxxx, where xxxx are the 4-byte
	// value for channel

	channel = ((unsigned char)(ch<<4)>>4)+1;
}

unsigned char MIDITrack::getPatch() const
{
	if (patch<128) return patch;
	else return 0;
}

#endif
