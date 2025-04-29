#ifndef MIDIFILE_CPP
#define MIDIFILE_CPP

MIDIFileHeader::MIDIFileHeader()
{
	MThd = 0;
	length = 0;
	format = 0;
	tracks = 0;
	ticksPerQuarter = 0;
}

short MIDIFileHeader::check()
{
	short ret = 1;

	// Four first characters of the file should be "MThd",
	// byte order and values in file (hexadecimal): 4D 54 68 64
 
	if (invb(MThd)!=0x4D546864) ret = 0;
	
	// Format should be:
	// 0 = single track
	// 1 = multi-track
	// 2 = multi-sequence with single tracks  
	if (ret==1) if (invb(format) > 2) ret = 0;
   
	// I'm not sure is this actually required,
	// but it ensures there will be no errors
	// in other methods of MIDIFileHeader
	if (invb(format)==0 && invb(tracks)==0)
		tracks = invb((unsigned short)1);
   
	return ret;
}

void MIDIFileHeader::read(ifstream &infile)
{
	infile.read(&MThd, 4);
	infile.read(&length, 4);
	infile.read(&format, 2);
	infile.read(&tracks, 2);
	infile.read(&ticksPerQuarter, 2);
}

MIDIFileTrackHeader::MIDIFileTrackHeader()
{
	MTrk = 0;
	length = 0;
}

short MIDIFileTrackHeader::check()
{
	// Four first characters of the file should be "MTrk",
	// byte order and values in file (hexadecimal): 4D 54 72 6B
 
	if (invb(MTrk)!=0x4D54726B) return 0;
	else return 1;
}

void MIDIFileTrackHeader::read(ifstream &infile)
{
	infile.read(&MTrk, 4);
	infile.read(&length, 4);
}

MIDIFile::MIDIFile()
{
	NoError = 1;
	ErrorNum = 0;
	ErrorPos = 0;

	debug = 0;
	ConductorTrack = new MIDIConductorTrack;
	NoteTrack = new MIDINoteTrack;
	Track = new MIDITrack;

	if (ConductorTrack==NULL || NoteTrack==NULL || Track==NULL)
	{
		NoError = 0;
		ErrorNum = 1;
	}
}

MIDIFile::MIDIFile(const MIDIFile &op)
{
	NoError = 1;
	ErrorNum = 0;
	ErrorPos = 0;

	debug = 0;

	this->clear();

	Conductors = op.Conductors;
	Notes = op.Notes;
	Tracks = op.Tracks;

	ConductorTrack = new MIDIConductorTrack;
	NoteTrack = new MIDINoteTrack;
	Track = new MIDITrack;

	if (ConductorTrack==NULL || NoteTrack==NULL || Track==NULL)
	{
		NoError = 0;
		ErrorNum = 1;
	}
	else
	{
		*ConductorTrack = *(op.ConductorTrack);
		*NoteTrack = *(op.NoteTrack);
		*Track = *(op.Track);
	}
}

MIDIFile& MIDIFile::operator= (const MIDIFile &op)
{
	if (this == &op)
		return *this;

	NoError = op.NoError;
	ErrorNum = op.ErrorNum;
	ErrorPos = op.ErrorPos;

	debug = op.debug;

	this->clear();

	Conductors = op.Conductors;
	Notes = op.Notes;
	Tracks = op.Tracks;

	if (ConductorTrack==NULL || NoteTrack==NULL || Track==NULL)
	{
		NoError = 0;
		ErrorNum = 1;
	}
	else
	{
		*ConductorTrack = *(op.ConductorTrack);
		*NoteTrack = *(op.NoteTrack);
		*Track = *(op.Track);
	}

	return *this;
}

MIDIFile::~MIDIFile()
{
	if (debug==1) debugout.close();
	delete [] ConductorTrack;
	delete [] NoteTrack;
	delete [] Track;
}

char * MIDIFile::errDesc() const
{
	switch(ErrorNum) {
	case 0:
	{
		return "No error (MIDIFile)";
		break;
	}

	case 1:
	{
		return "Out of memory";
		break;
	}

	// debugOn(char * logfilename)
	case 9:
	{
		return "Could not open output for debug (MIDIFile)";
		break;
	}
	
	// read()
	case 100:
	{
		return "File not found or file system error";
		break;
	}
	case 101:
	{
		return "File is not a standard MIDI file";
		break;
	}
	case 102:
	{
		return "File is a multi-sequence file (not support)";
		break;
	}
	case 103:
	{
		return "Delta-time scale is zero (not supported)";
		break;
	}
	case 104:
	{
		return "Unknown command in MIDI file(byte is < 0xA0)";
	}
	case 109:
	{
		return "Unexpected end of file";
		break;
	}	 
	case 110:
	{
		return "Cannot read file: Track not found";
		break;
	}
	}
}

void MIDIFile::errPos_cout() const
{
	if (ErrorNum==104 || ErrorNum==109 || ErrorNum==110)
	{
		cout << " Position = " << ErrorPos;
	}
}

void MIDIFile::read(char * filename)
{
	// See MIDIFile::errDesc() for error details

	this->clear();

	// Allocating memory for temporary objects to store data
	// before putting it in a vector<>
	this->alloctemp();

	if (NoError)
	{

		// Open file
		infile.open(filename, ios::bin | ios::in);
		if (!infile.good())
		{
			NoError = 0;
			ErrorNum = 100;
		}

		// Read and check main header
		if (NoError)
		{
			// debug - remove

			header.read(infile);

			if (!header.check())
			{
				NoError = 0;
				ErrorNum = 101;
			}
			else
			{
				if (header.getFormat()==2)
				{
					NoError = 0;
					ErrorNum = 102;
				}
				if (header.getDiv()==0)
				{
					NoError = 0;
					ErrorNum = 103;
				}
			}
		}
		if (debug) debugout << "Header: deltaDiv=" << header.getDiv() << endl;

		// Reallocate for tracks if required
		if (NoError) Notes.reserve(header.getTracks());
		if (NoError) Tracks.reserve(header.getTracks());

		// Reallocate for conductor tracks if required
		// (.header.getFormat() = 2 = multi-sequence = multi-song)
		if (NoError && header.getFormat()==2 ) Conductors.reserve(header.getTracks());

		// Read track(s)
		if (NoError) readTracks();

		// Close file
		infile.close();

		// Free temp memory
		this->freetemp();
	}

	if (NoError) {
		assert(Conductors.size() > 0);
		assert(Notes.size()==header.getTracks());
		assert(Tracks.size()==header.getTracks());
	}
}

void MIDIFile::clear()
{
	Conductors.clear();
	Notes.clear();
	Tracks.clear();
	this->freetemp();
}

void MIDIFile::alloctemp()
{
	if (ConductorTrack!=NULL) delete ConductorTrack;
	if (NoteTrack!=NULL) delete NoteTrack;
	if (Track!=NULL) delete Track;

	ConductorTrack = new MIDIConductorTrack;
	NoteTrack = new MIDINoteTrack;
	Track = new MIDITrack;

	if (ConductorTrack==NULL || NoteTrack==NULL || Track==NULL)
	{
		NoError = 0;
		ErrorNum = 1;
	}
}

void MIDIFile::freetemp()
{
	delete ConductorTrack;
	delete NoteTrack;
	delete Track;
	ConductorTrack=0;
	NoteTrack=0;
	Track=0;
}

void MIDIFile::readTracks()
{
	// Reads all tracks from MIDI file after the main header,
	// Collects track data to vector<> sequences

	// debug - remove
	if (debug) debugout << "---- MIDIFile::readTracks()" << endl;
	// debug-end

	MIDIFileTrackHeader trkHdr;

	unsigned long i = 0;

	while (!infile.eof() && NoError==1 && TrackCount < header.getTracks()) {

		// debug - remove
		if (debug) debugout << " loop start, i=" << i << ", infile.eof()=" << infile.eof() << endl;
		// debug-end

		// Find new track header
		trkHdr.clear();
		trkHdr.read(infile);
		i+=8; // sizeof(MIDIFileTrackHeader) = 8

		if (!trkHdr.check())
		{
			NoError = 0;
			ErrorNum = 110;
			ErrorPos = infile.tellg();

			// debug - remove
			if (debug) debugout << " no track header found." << endl;
			if (debug) debugout << "  pos=" << infile.tellg() << " MTrk="
			  << trkHdr.getMTrk() << " length=" << trkHdr.getLength() << endl;
			// debug-end
		}
		else
		{
			// debug - remove
			if (debug) debugout << " track header found: pos= " << infile.tellg() <<
			  " length=" << trkHdr.getLength() << endl;
			// debug-end

			readTrack(trkHdr.getLength());
			i+=trkHdr.getLength();

			// Put data to vector<>s

			if (TrackCount==0 || header.getFormat()==2)
			{
				Conductors.push_back(*ConductorTrack);
			}
			Notes.push_back(*NoteTrack);
			Tracks.push_back(*Track);

			TrackCount++;
			if (header.getFormat()==2) ConductorCount++;
		}
	}

	if (header.getFormat()<2) ConductorCount=1;
}

void MIDIFile::readTrack(unsigned long length)
{
	// Reads a track from MIDI file after track header
	// Puts data to temporary objects ConductorTrack, NoteTrack, Track

	unsigned long count = 0;
	last_time = 0;
	unsigned char running;

	delete NoteTrack;
	delete Track;
	NoteTrack = new MIDINoteTrack;
	Track = new MIDITrack;
	if (NoteTrack == NULL || Track == NULL)
	{
		NoError = 0;
		ErrorNum = 1;
	}

	// Channels of notes can vary inside each track.
	// When trying to determine a channel for track,
	// the channel value will be the same the most
	// of the notes have.

	unsigned long * noteChannels;
	short i;

	if (NoError == 1)
	{
		noteChannels = new unsigned long[16];
		if (noteChannels == NULL)
		{
			NoError = 0;
			ErrorNum = 1;
		}
		else
		{
			for (i = 0; i < 16; i++) noteChannels[i] = 0;
		}
	}

	if (NoError == 1)
	{
		// Read track data
		running = 255;
		while(count < length && NoError == 1)
		{
			if (infile.eof())
			{
				NoError = 0;
				ErrorNum = 109;
				ErrorPos = infile.tellg();
			}

			if (NoError == 1) readDelta(count);
			if (NoError == 1) readEvent(count, running, noteChannels);
		}
	}

	if (NoError == 1)
	{
		// Determine the channel

		unsigned long highest = 0;
		short channel = 0;
		for (i = 0; i < 16; i++)
		{
			if (noteChannels[i] > highest)
			{
				highest = noteChannels[i];
				channel = i;
			}
		}

		Track->setChannel(channel+1);
	}

	delete [] noteChannels;
}

unsigned long MIDIFile::readDelta(unsigned long & readcounter)
{
	// Only reads, interprets and returns delta time data

	// Delta time is converted into 7-bit bytes
	// and is four bytes long at maximum
	// If MSB (first bit of 8-bit byte) is 1,
	// the value is continued on the next byte:
	//
	// xAAAAAAA xBBBBBBB xCCCCCCC xDDDDDDD
	// if (x=1) include_also_next_byte;
	// result = 0xAAAAAAABBBBBBBCCCCCCCDDDDDDD;

	unsigned char deltabyte[4];
	unsigned char i, i2, b1;
	unsigned long deltatime;

	unsigned char deltadebug[4];

	for (i=0; i<4; i++) { deltabyte[i]=0; deltadebug[i]=0; }
	i = 0;	// Length of delta time in bytes
	i2 = 0; // Counter
	deltatime = 0;

	do {
		if (infile.eof())
		{
			NoError = 0;
			ErrorNum = 109;
			ErrorPos = infile.tellg();
		}

		infile.read(&b1, 1);
		readcounter++;

		if (b1 > 127) { deltabyte[i] = b1 - 128; }
		else		 { deltabyte[i] = b1; }
		deltadebug[i] = b1;

		i++;
	} while(b1 > 127);

	deltatime = last_time;
	if (i>1)
	{
		for (i2 = 0; i2<i; i2++)
		{
			deltatime += (((unsigned char)deltabyte[i2] << 1) >> 1) << ((i-1)*7 - i2*7);
		}
	}
	else deltatime += deltabyte[0];

	// debug - remove
	if (debug)
	{
		debugout << "  DELTA = " << deltatime << " [";
		for (i2=0; i2<i; i2++)
		{
			if (i2>0) debugout << " ";
			debugout << (short)deltadebug[i2];
		}
		debugout << "] (pos = " << (infile.tellg()-i) << ")" << endl;
	}
	// debug-end

	last_time = deltatime;
}

void MIDIFile::readEvent(unsigned long & readcounter, unsigned char & running,
						 unsigned long * noteChannels)
{
	// Reads and interprets event data followed by delta time
	// Stores data to temporary objects with delta time

	if (debug) debugout << "  EVENT-";
	unsigned char b1 = 0;
	unsigned char b2 = 0;
	unsigned char b3 = 0;

	unsigned char channel;

	if (infile.eof())
	{
		NoError = 0;
		ErrorNum = 109;
		ErrorPos = infile.tellg();
	}
	infile.read(&b1, 1);
	readcounter++;

	// debug - remove
	if (debug) debugout << "(" << infile.tellg()-1 << ")[" << short(b1) <<
	  "/" << (short)(b1>>4) << "]: ";
	// debug-end

	switch(b1>>4)
	{
		case 0x8:  // Note off (ignored)
		{
			// NOTE FOR NOTE OFF COMMANDS' DELTA TIMES:
			// _Note off's delta time means actually THE LENGTH OF NOTE._
			// To calculate the actual delta time where the note is stop,
			// seek previous note ON command with same key value and
			// add its delta time to this note off's delta.

			running = b1;
			infile.read(&b2, 1);	// key number
			infile.read(&b3, 1);	// velocity
			readcounter+=2;

			// debug - remove
			if (debug) debugout << "note off (" << (short)b2 << ", " << (short)b3 << ")";
			if (debug) debugout << " CHANNEL " << (short)((unsigned char)(b1<<4)>>4) + 1;
			// debug-end

			break;
		}

		case 0x9:  // Note on
		{
			running = b1;
			infile.read(&b2, 1);	// key number
			infile.read(&b3, 1);	// velocity
			readcounter+=2;

			NoteTrack->addNote(last_time, b2, b3);

			channel = (unsigned char)(b1<<4)>>4;
			noteChannels[channel]++;

			// debug - remove
			if (debug) debugout << "note on (" << (short)b2 << ", " << (short)b3 << ")";
			if (debug) debugout << " CHANNEL " << (short)((unsigned char)(b1<<4)>>4) + 1;

			// debug-end

			break;
		}

		case 0xA:  // Key aftertouch
		{
			running = b1;
			infile.read(&b2, 1);	// key number
			infile.read(&b3, 1);	// velocity
			readcounter+=2;
			// debug - remove
			if (debug) debugout << "key aftertouch (" << (short)b2 << ", " << (short)b3 << ")";
			if (debug) debugout << " CHANNEL " << (short)((unsigned char)(b1<<4)>>4) + 1;
			// debug-end
			break;
		}

		case 0xB:  // Control change
		{
			running = b1;
			infile.read(&b2, 1);	// controller number
			infile.read(&b3, 1);	// value
			readcounter+=2;
			// debug - remove
			if (debug) debugout << "controller (" << (short)b2 << ", " << (short)b3 << ")";
			if (debug) debugout << " CHANNEL " << (short)((unsigned char)(b1<<4)>>4) + 1;
			// debug-end
			break;
		}

		case 0xC:  // Patch (program) change
		{
			running = b1;
			infile.read(&b2, 1);	// Patch number
			readcounter++;
			Track->setPatch1(b2);

			// debug - remove
			if (debug) debugout << "patch (" << (short)b2 << ")";
			if (debug) debugout << " CHANNEL " << (short)((unsigned char)(b1<<4)>>4) + 1;
			// debug-end

			break;
		}

		case 0xD:  // Channel aftertouch
		{
			running = b1;
			infile.read(&b2, 1);	// Channel number
			readcounter++;
			// debug - remove
			if (debug) debugout << "channel aftertouch (" << (short)b2 << ")";
			if (debug) debugout << " CHANNEL " << (short)((unsigned char)(b1<<4)>>4) + 1;
			// debug-end
			break;
		}

		case 0xE:  // Pitch wheel change
		{
			// This is 14-bit value
			// Value 0x2000 = 8192 is normal / no change

			running = b1;
			infile.read(&b2, 1);	// least 7 bits
			infile.read(&b3, 1);	// most 7 bits
			readcounter+=2;

			// unsigned short value = b2 + b3 * 128
			// short value = (b2 + b3 * 128) - 8192

			// debug - remove
			if (debug) debugout << "pitch bend (" << (short)(b2 + b3 * 128) - 8192 << ")";
			if (debug) debugout << " CHANNEL " << (short)((unsigned char)(b1<<4)>>4) + 1;
			// debug-end
			break;
		}

		case 0xF:	 // Meta event, special handling
		{
			running = b1;
			readMeta(readcounter, 0, 0);
			break;
		}

		default:  // Running mode
		{
			if (debug) debugout << "running event";
			switch(running>>4)
			{
			case 0x8:
				infile.read(&b2, 1);
				readcounter++;
				break;
			case 0x9:
				infile.read(&b2, 1);
				readcounter++;
				NoteTrack->addNote(last_time, b1, b2);
				channel = (unsigned char)(b1<<4)>>4;
				noteChannels[channel]++;
				break;
			case 0xA:
				infile.read(&b2, 1);
				readcounter++;
				break;
			case 0xB:
				infile.read(&b2, 1);
				readcounter++;
				break;
			case 0xC:
				// Track->setPatch1(b1);
				break;
			case 0xD:
				break;
			case 0xE:
				infile.read(&b2, 1);
				readcounter++;
				break;
			case 0xF:
				readMeta(readcounter, 1, b1);
				break;
			}
		}
	}
	// debug - remove
	if (debug) debugout << " (afterpos:" << infile.tellg() << ")" << endl;
	// debug-end
}

void MIDIFile::readMeta(unsigned long & readcounter,
			   char runningOn, unsigned char b2)
{
	// Reads and interprets META event data
	// Stores data to temporary objects with delta time
	// runningOn: if this is 1, file is readed in
	// running mode which means that byte b2 has already been read.

	unsigned char b3 = 0;
	unsigned char mdata[255];

	// debug - remove
	short i;
	short debugcount = 0;
	if (debug) debugout << "META: ";
	// debug-end

	if (infile.eof())
	{
		NoError = 0;
		ErrorNum = 109;
		ErrorPos = infile.tellg();
	}

	if (runningOn == 0)
	infile.read(&b2, 1);	 // Meta command
	infile.read(&b3, 1);	 // Data length in bytes

	if (b3 > 0)
	{
		infile.read(&mdata, b3); // Data
	}
	readcounter+=(2-runningOn+b3);	   // b2 + b3 + mdata
	debugcount+=(2-runningOn+b3);

	if (debug) debugout << (short)b2 << " len=" << (short)b3 << " ";

	switch(b2)
	{
		// 0x00  Sequence number		 0x20  MIDI channel prefix assignment
		// 0x01  Text event 			 0x2F  End of track
		// 0x02  Copyright notice		 0x51  Tempo setting
		// 0x03  Sequence or track name  0x54  SMPTE offset
		// 0x04  Instrument name		 0x58  Time signature
		// 0x05  Lyric text 			 0x59  Key signature
		// 0x06  Marker text			 0x7F  Sequencer specific event
		// 0x07  Cue point

		case 0x00: // Track sequence number
		{
			if (debug) debugout << "track sequence number";
			break;
		}

		case 0x01: // Text event (mdata[] is b3 characters long text)
		{
			if (debug) debugout << "text event";
			break;
		}

		case 0x02: // Text: copyright info
		{
			if (debug) debugout << "copyright text";
			break;
		}

		case 0x03: // Text: sequence (track) name
		{
			Track->setName1(mdata, b3);
			if (debug) debugout << "track name: " << Track->getName();
			break;
		}

		case 0x04: // Text: track instrument name
		{
			if (debug) debugout << "track instrument name";
			break;
		}

		case 0x05: // Text: lyric
		{
			if (debug) debugout << "lyric text ";
			break;
		}

		case 0x06: // Text: marker
		{
			if (debug) debugout << "marker text";
			break;
		}

		case 0x07: // Text: cue point
		{
			if (debug) debugout << "cue point text";
			break;
		}

		case 0x2F: // End of track
		{
			if (debug) debugout << "end of track";
			break;
		}

		case 0x51: // Tempo
		{
			if (debug) debugout << "tempo";
			unsigned long tpo = 0;
			tpo = (unsigned long)(mdata[0]<<16) + (mdata[1]<<8) + mdata[2];
			ConductorTrack->addTempo(last_time, tpo);
			break;
		}

		case 0x58: // Time signature (meter)
		{
			if (debug) debugout << "meter";
			ConductorTrack->addMeter(last_time, mdata[0], mdata[1]);
			break;
		}

		case 0x59: // Key signature
		{
			// (char) mdata[0] = sharps/flats: -7=7 flats, 0=C maj 7=7 sharps
			//		  mdata[1] = major/minor: 0=major 1=minor

			ConductorTrack->addKeySig(last_time, (char)mdata[0], mdata[1]);
			// debug - remove
			if (debug) debugout << "key signature (" << (short)mdata[0] << ", " << (short)(char)mdata[1] << ")";
			// debug-end
			break;
		}

		case 0x7F: // Sequencer specific information
		{
			if (debug)
			{
				debugout << "sequencer specific info:";
				for (i=0; i < b3; i++)
				{
					debugout << " " << short(mdata[i]);
				}
			}
		}

		default:
		{
			if (debug) debugout << "UNKNOWN!";
		}
	}
	// debug - remove
	if (debug) debugout << "(" << debugcount << " bytes of meta)";
	// debug-end
}

short MIDIFile::getTrackChannel(unsigned short num) const
{
	if (num < Tracks.size()) return Tracks[num].getChannel();
	else return 0;
}

char * MIDIFile::getTrackName(unsigned short num) const
{
	if (num < Tracks.size()) return Tracks[num].getName();
	else return 0;
}

void MIDIFile::report() const
{
	unsigned short i;
	unsigned long j;

	cout << "--- MIDIFile::report ---------------------------------------" << endl;
	cout << "good() = " << NoError;

	if (!NoError) cout << " Error " << err() << ": " << errDesc() <<
	  " (byte " << ErrorPos << ")";
	cout << endl;

	if (NoError)
	{
		cout << "*** FILE" << endl;
		cout << "format: " << header.getFormat() << "/";
		switch(header.getFormat())
		{
			case 0: { cout << "single track"; break; }
			case 1: { cout << "multi-track"; break; }
			case 2: { cout << "multi-sequence"; break; }
		}
		cout << " tracks: " << header.getTracks() << " ticks/quarter: " <<
			 header.getDiv() << endl;

		cout << "Type		Used\tAllocated" << endl;
		cout << "Conductors \t" << Conductors.size() << "\t" << Conductors.capacity() << endl;
		cout << "Tracks 	\t" << Tracks.size() << "\t" << Tracks.capacity() << endl;
		cout << "Notes		" << Notes.size() << "\t" << Notes.capacity() << endl;
		cout << "*** CONDUCTORS" << endl;

		unsigned long time = 0;
		float tpo = 0;
		unsigned char num = 0;
		unsigned char den = 0;
		for (i=0; i<ConductorCount; i++)
		{
			Conductors[i].getTempo(0, time, tpo);
			Conductors[i].getMeter(0, time, num, den);
			cout << i << " time: " << (short)num << "/" << (short)den <<
			  " tempo: " << tpo <<
			  " / times: " << Conductors[i].getMeterCount() <<
			  " tempos: " << Conductors[i].getTempoCount() << endl;
		}

		cout << "*** TRACKS" << endl;
		for (i=0; i<TrackCount; i++)
		{
			cout << i << " ";
			Tracks[i].printName(20);
			cout <<
			  " ch:" << (short)Tracks[i].getChannel() <<
			  " pch:" << (short)Tracks[i].getPatch() <<
			  " notes:" << Notes[i].getNoteCount() << endl;
		}
	}

	cout << "------------------------------------------------------------" << endl;
}

void MIDIFile::debugOn()
{
	if (debug==0)
	{
		debugout.open("debugout.txt", ios::out);

		if (!debugout.good())
		{
			NoError = 0;
			ErrorNum = 9;
			debug = 0;
		}
		else debug = 1;
	}
}

void MIDIFile::debugOff()
{
	if (debug==1) debugout.close();
	debug=0;
}

void MIDIFile::printNoteCount(unsigned short track)
{
	if (track < TrackCount)
	{
		cout << "MIDIFile::printNoteCount(" << track << "): " <<
		  Notes[track].getNoteCount() << endl;
	}
}

void MIDIFile::listTracks(short selection)
{
	// selection: 0=show all tracks
	//			  1=show only tracks containing notes

	unsigned short i;
	unsigned short j;
	unsigned short a;
	unsigned short b;
	char * trackname;

	a = short(floor(log10(TrackCount)));

	cout << "------------------------------------------------------------" << endl;

	for (i = 0; i < TrackCount; i++)
	{
		if (selection==0 || (selection==1 && Notes[i].getNoteCount() > 0) )
		{
			// Track number
			b = short(floor(log10(i)));
			for (j = 0; j < a-b; j++) cout << " ";
			cout << i << " ";

			// Track name (20 characters)
			trackname = Tracks[i].getName();
			b = strlen(trackname);
			if (b > 20) b = 20;
			for (j = 0; j < b; j++) cout << trackname[j];
			if (b < 20)
			{
				for (j = 0; j < 20-b; j++) cout << " ";
			}

			// Channel
			cout << " ch:";
			if (Tracks[i].getChannel() == 0) cout << " -";
			else {
				if (Tracks[i].getChannel() < 10) cout << " ";
				cout << (short)Tracks[i].getChannel();
			}

			// Patch (program)
			cout << " pch:";
			if (Tracks[i].getPatch() < 100) cout << " ";
			if (Tracks[i].getPatch() < 10) cout << " ";
			cout << (short)Tracks[i].getPatch();

			// Note count
			cout << " notes:" << getNoteCount(i) << endl;
		}
	}
}

unsigned long MIDIFile::getNoteCount(unsigned short track) const
{
	if (track < Notes.size()) return Notes[track].getNoteCount();
	else return 0;
}

MIDIConductorTrack MIDIFile::getCondTrk(unsigned short num) const
{
	if (num<Conductors.size())
		return Conductors[num];
}

MIDINoteTrack MIDIFile::getNoteTrk(unsigned short num) const
{
	if (num<Notes.size())
		return Notes[num];
}

MIDITrack MIDIFile::getTrack(unsigned short num) const
{
	if (num<Tracks.size())
	  return Tracks[num];
}

#endif
